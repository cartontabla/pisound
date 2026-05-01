from __future__ import annotations

import enum
import threading
import time
from collections import deque
from dataclasses import dataclass


class StreamDirection(enum.Enum):
    PLAYBACK = "playback"
    CAPTURE = "capture"


class SampleFormat(enum.Enum):
    S16LE = "s16le"
    S24LE = "s24le"
    S32LE = "s32le"
    FLOAT32LE = "float32le"


class StreamState(enum.Enum):
    CLOSED = "closed"
    OPEN = "open"
    PREPARED = "prepared"
    RUNNING = "running"
    DRAINING = "draining"
    STOPPED = "stopped"


class StreamError(RuntimeError):
    pass


class InvalidFormatError(StreamError):
    pass


class UnsupportedFormatError(StreamError):
    pass


class BadStateError(StreamError):
    pass


class WouldBlockError(StreamError):
    pass


class XRunError(StreamError):
    pass


class BackendDisconnectedError(StreamError):
    pass


@dataclass(frozen=True)
class PcmFormat:
    direction: StreamDirection
    sample_rate_hz: int
    channels: int
    sample_format: SampleFormat
    frames_per_period: int
    period_count: int

    def __post_init__(self) -> None:
        if self.sample_rate_hz <= 0:
            raise InvalidFormatError("sample_rate_hz must be > 0")
        if self.channels <= 0:
            raise InvalidFormatError("channels must be > 0")
        if self.frames_per_period <= 0:
            raise InvalidFormatError("frames_per_period must be > 0")
        if self.period_count < 2:
            raise InvalidFormatError("period_count must be >= 2")

    @property
    def capacity_frames(self) -> int:
        return self.frames_per_period * self.period_count


@dataclass(frozen=True)
class BufferMeta:
    frame_count: int
    pts_monotonic_ns: int
    sequence: int

    @classmethod
    def for_frames(cls, frame_count: int, sequence: int) -> "BufferMeta":
        return cls(
            frame_count=frame_count,
            pts_monotonic_ns=time.monotonic_ns(),
            sequence=sequence,
        )


@dataclass(frozen=True)
class StreamStatus:
    state: StreamState
    queued_frames: int
    available_frames: int
    xruns: int
    delay_frames: int
    last_sequence: int


@dataclass(frozen=True)
class PlaybackChunk:
    frames: bytes
    meta: BufferMeta


class PlaybackRingBuffer:
    """Minimal playback backend expressed in frames instead of bytes."""

    def __init__(self, fmt: PcmFormat):
        if fmt.direction is not StreamDirection.PLAYBACK:
            raise UnsupportedFormatError("PlaybackRingBuffer only supports playback streams")
        self.format = fmt
        self.state = StreamState.OPEN
        self._queue: deque[PlaybackChunk] = deque()
        self._queued_frames = 0
        self._xruns = 0
        self._last_sequence = -1
        self._next_sequence = 0
        self._lock = threading.Lock()

    def prepare(self) -> None:
        with self._lock:
            if self.state not in (StreamState.OPEN, StreamState.STOPPED):
                raise BadStateError(f"cannot prepare from state {self.state.value}")
            self._queue.clear()
            self._queued_frames = 0
            self.state = StreamState.PREPARED

    def start(self) -> None:
        with self._lock:
            if self.state is not StreamState.PREPARED:
                raise BadStateError(f"cannot start from state {self.state.value}")
            self.state = StreamState.RUNNING

    def stop(self) -> None:
        with self._lock:
            if self.state not in (StreamState.PREPARED, StreamState.RUNNING, StreamState.DRAINING):
                raise BadStateError(f"cannot stop from state {self.state.value}")
            self.state = StreamState.STOPPED

    def close(self) -> None:
        with self._lock:
            self._queue.clear()
            self._queued_frames = 0
            self.state = StreamState.CLOSED

    def write(self, frames: bytes, frame_count: int) -> BufferMeta:
        with self._lock:
            if self.state is not StreamState.RUNNING:
                raise BadStateError(f"cannot write from state {self.state.value}")
            if frame_count <= 0:
                raise InvalidFormatError("frame_count must be > 0")
            free_frames = self.format.capacity_frames - self._queued_frames
            if frame_count > free_frames:
                self._xruns += 1
                raise WouldBlockError(
                    f"buffer full: requested={frame_count} free={free_frames}"
                )
            meta = BufferMeta.for_frames(frame_count=frame_count, sequence=self._next_sequence)
            self._next_sequence += 1
            self._queue.append(PlaybackChunk(frames=frames, meta=meta))
            self._queued_frames += frame_count
            self._last_sequence = meta.sequence
            return meta

    def pop_chunk(self) -> PlaybackChunk:
        with self._lock:
            if self.state not in (StreamState.RUNNING, StreamState.DRAINING):
                raise BadStateError(f"cannot pop from state {self.state.value}")
            if not self._queue:
                self._xruns += 1
                raise XRunError("playback underrun")
            chunk = self._queue.popleft()
            self._queued_frames -= chunk.meta.frame_count
            return chunk

    def drain(self) -> None:
        with self._lock:
            if self.state is not StreamState.RUNNING:
                raise BadStateError(f"cannot drain from state {self.state.value}")
            self.state = StreamState.DRAINING

    def recover(self) -> None:
        with self._lock:
            if self.state not in (StreamState.RUNNING, StreamState.DRAINING, StreamState.STOPPED):
                raise BadStateError(f"cannot recover from state {self.state.value}")
            self._queue.clear()
            self._queued_frames = 0
            self.state = StreamState.PREPARED

    def query_status(self) -> StreamStatus:
        with self._lock:
            return StreamStatus(
                state=self.state,
                queued_frames=self._queued_frames,
                available_frames=max(0, self.format.capacity_frames - self._queued_frames),
                xruns=self._xruns,
                delay_frames=self._queued_frames,
                last_sequence=self._last_sequence,
            )
