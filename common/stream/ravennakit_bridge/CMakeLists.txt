cmake_minimum_required(VERSION 3.20)

project(pisound_ravennakit_bridge LANGUAGES CXX)

option(PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO "Enable PortAudio-backed local audio I/O" OFF)
option(PISOUND_RAVENNAKIT_BRIDGE_ENABLE_RAVENNAKIT "Enable RAVENNAKIT-backed AoIP engine" OFF)
set(PISOUND_RAVENNAKIT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/ravennakit" CACHE PATH "Path to a local RAVENNAKIT source tree")

add_library(pisound_ravennakit_bridge_core
  src/bridge_config.cpp
  src/stub_ravennakit_engine.cpp
)

target_compile_features(pisound_ravennakit_bridge_core PUBLIC cxx_std_17)
target_include_directories(pisound_ravennakit_bridge_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(pisound_ravennakit_bridge_core
  PUBLIC
    $<$<BOOL:${PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO}>:PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO=1>
    $<$<BOOL:${PISOUND_RAVENNAKIT_BRIDGE_ENABLE_RAVENNAKIT}>:PISOUND_RAVENNAKIT_BRIDGE_ENABLE_RAVENNAKIT=1>
)

if(PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO)
  find_path(PORTAUDIO_INCLUDE_DIR portaudio.h)
  find_library(PORTAUDIO_LIBRARY NAMES portaudio portaudio_x64)
  if(NOT PORTAUDIO_INCLUDE_DIR OR NOT PORTAUDIO_LIBRARY)
    message(FATAL_ERROR "PortAudio requested but not found. Provide portaudio.h and the PortAudio library.")
  endif()

  target_sources(pisound_ravennakit_bridge_core PRIVATE
    src/portaudio_audio_driver.cpp
  )
  target_include_directories(pisound_ravennakit_bridge_core PRIVATE
    ${PORTAUDIO_INCLUDE_DIR}
  )
  target_link_libraries(pisound_ravennakit_bridge_core PRIVATE
    ${PORTAUDIO_LIBRARY}
  )
else()
  target_sources(pisound_ravennakit_bridge_core PRIVATE
    src/portaudio_audio_driver.cpp
  )
endif()

if(PISOUND_RAVENNAKIT_BRIDGE_ENABLE_RAVENNAKIT)
  if(PISOUND_RAVENNAKIT_SOURCE_DIR)
    set(RAV_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(RAV_TESTS OFF CACHE BOOL "" FORCE)
    set(RAV_BENCHMARKS OFF CACHE BOOL "" FORCE)
    set(RAV_TOOLS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${PISOUND_RAVENNAKIT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/ravennakit)
    target_link_libraries(pisound_ravennakit_bridge_core PRIVATE ravennakit)
    target_compile_definitions(pisound_ravennakit_bridge_core PRIVATE
      PISOUND_RAVENNAKIT_BRIDGE_HAVE_RAVENNAKIT=1
    )
  else()
    message(FATAL_ERROR "RAVENNAKIT requested but PISOUND_RAVENNAKIT_SOURCE_DIR is empty.")
  endif()
endif()

add_executable(pisound_ravennakit_bridge
  src/main.cpp
)

target_link_libraries(pisound_ravennakit_bridge
  PRIVATE
    pisound_ravennakit_bridge_core
)

message(STATUS "pisound_ravennakit_bridge configured")
message(STATUS "  PortAudio backend:   ${PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO}")
message(STATUS "  RAVENNAKIT backend: ${PISOUND_RAVENNAKIT_BRIDGE_ENABLE_RAVENNAKIT}")
