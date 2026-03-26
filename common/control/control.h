#ifndef CONTROL_H
#define CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

void control_init(void);
void control_process_rx(void);
void control_process_tx(void);
void control_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif