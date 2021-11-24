#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "i8042.h"
#include "math.h"

typedef enum {
    UPLINE, TRANSITION, DOWNLINE
} States;

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void packetbuilder();

void arrangeStruct(struct packet * pp);

int (util_sys_inb)(int port, uint8_t *value);

int mouse_disable_data_reporting();

int mouse_eenable_data_reporting();

int mouse_cmd(uint8_t cmd);

bool checkTolerance(float tolerance, struct packet *pp);

void gesture_manager(uint8_t x_len, uint8_t tolerance, struct packet *pp, States *st);
