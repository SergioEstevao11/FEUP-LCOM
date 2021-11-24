#include <lcom/lcf.h>
#include <lcom/lab2.h>
#include <minix/sysutil.h>

#include <stdint.h>
#include <stdbool.h>

#include "i8042.h"



void sys_inb_count(port_t port, uint8_t *value);

void (kbc_ih)();

int kbd_subscribe_int(uint32_t *bit_no);

int kbd_unsubscribe_int();

int util_sys_inb(int port, uint8_t *value);

int KBC_send_cmd(uint8_t c);

int interrupt_restart();

