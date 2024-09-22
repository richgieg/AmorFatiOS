#ifndef NET_H
#define NET_H

#include <types.h>

void net_init(void);
void net_handle_frame_receive(u8 *buf, size_t length);
void net_subscribe(void);
void net_unsubscribe(void);
void net_unsubscribe_for_pid(int pid);
void net_read_frame(u8 *buf, size_t *length);
bool net_has_frame_for_process(int pid);

#endif
