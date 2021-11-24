#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"

extern bool check;
extern int time_counter;
extern int counter;
extern uint16_t packetMouse[];
extern bool done;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    uint32_t irq_set;
    uint8_t bit_no;
    int r, ipc_status;
    struct packet p;
    message msg;

    mouse_cmd(0xEA);
    mouse_cmd(0xF4);


  if(mouse_subscribe_int(&bit_no)==1)
    return 1;
  irq_set = 0x001 << bit_no; //alternativa a BIT(bit_no);


  while (cnt > 0){
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            //received notification/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: //hardware interrupt notification/
                    if (msg.m_notify.interrupts & irq_set) { //subscribed interrupt/
                        mouse_ih();
                        packetbuilder();
                        if (counter == 3){
                            if (check){
                                arrangeStruct(&p);
                                mouse_print_packet(&p);
                            }
                            cnt --;
                            counter = 0;
                            check = true;
                        }
                    }
                    break;
                default:
                    break; //no other notifications expected: do nothing/
            }
        } else { //received a standard message, not a notification/
                                                     //no standard messages expected: do nothing/
        }
    }
    if(mouse_unsubscribe_int()!=0)
        return 1;
    if (mouse_disable_data_reporting() != 0){
        return 1;
    }
    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    uint32_t irq_set;
    uint8_t bit_no;
    uint8_t irq_set_timer;
    int r, ipc_status;
    unsigned int time = 0;
    struct packet p;
    message msg;

    time_counter = 0;

    mouse_cmd(0xEA);
    mouse_cmd(0xF4);


    if(mouse_subscribe_int(&bit_no) == 1)	
        return 1;
    irq_set = BIT(bit_no);

    if (timer_subscribe_int(&irq_set_timer) == 1) return 1;
    irq_set_timer = BIT(irq_set_timer);

  while (time < idle_time){
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            //received notification/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: //hardware interrupt notification/
                    if (msg.m_notify.interrupts & irq_set) { //subscribed interrupt
                        mouse_ih();
                        packetbuilder();
                        if (counter == 3){
                            if (check){
                                time = 0;
                                time_counter = 0;
                                arrangeStruct(&p);
                                mouse_print_packet(&p);
                            }
                            counter = 0;
                            check = true;
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_timer){
                        timer_int_handler();
                        if (time_counter% sys_hz()  == 0){
                            time++;
                        }
                    }
                    break;
                default:
                    break; //no other notifications expected: do nothing/
            }
        } else { //received a standard message, not a notification/
                                                     //no standard messages expected: do nothing/
        }
    }
    if (timer_unsubscribe_int() != 0)
        return 1;
    if(mouse_unsubscribe_int()!=0)
        return 1;
    if (mouse_disable_data_reporting() != 0){
        return 1;
    }
    return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    uint32_t irq_set;
    uint8_t bit_no;
    int r, ipc_status;
    struct packet p;
    message msg;
    States st = UPLINE;

    mouse_cmd(0xEA);
    mouse_cmd(0xF4);


  if(mouse_subscribe_int(&bit_no)==1)
    return 1;
  irq_set = BIT(bit_no);


  while (!done){
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            //received notification/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: //hardware interrupt notification/
                    if (msg.m_notify.interrupts & irq_set) { //subscribed interrupt/
                        mouse_ih();
                        packetbuilder();
                        if (counter == 3){
                            if (check){
                                arrangeStruct(&p);
                                mouse_print_packet(&p);
                                gesture_manager(x_len, tolerance, &p, &st);
                            }
                            counter = 0;
                            check = true;
                        }
                    }
                    break;
                default:
                    break; //no other notifications expected: do nothing/
            }
        } else { //received a standard message, not a notification/
                                                     //no standard messages expected: do nothing/
        }
    }
    if(mouse_unsubscribe_int()!=0)
        return 1;
    if (mouse_disable_data_reporting() != 0){
        return 1;
    }
    return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    uint8_t status;
    uint8_t cmd = '\0';
    struct packet p;

    // sys_outb(STATUS_PORT, KBC_READ_CMD);
    // tickdelay(micros_to_ticks(DELAY_US));
    // if ((util_sys_inb(STATUS_PORT, &status)) != 0)
    //     return 1;
    // if ((status & BIT(0)) == 1){
    //         return 1;
    // }

    // util_sys_inb(KBD_OUT_BUF, &cmd);
    // cmd = cmd & !(BIT(1));

    // sys_outb(STATUS_PORT, KBC_WRITE_CMD);
    // tickdelay(micros_to_ticks(DELAY_US));
    // if ((util_sys_inb(STATUS_PORT, &status)) != 0)
    //     return 1;
    // if ((status & BIT(1)) == 1){
    //         return 1;
    // }

    // sys_outb(KBD_OUT_BUF, cmd);



    // mouse_cmd(0xF5);
    // mouse_cmd(0xF0);

    while(cnt > 0){      
        mouse_cmd(0xEB);
        tickdelay(micros_to_ticks(period*1000));  
    
        for(int c = 0; c < 3; c++){
            mouse_ih();
            packetbuilder();
            if (counter == 3){
                if (check){
                    arrangeStruct(&p);
                    mouse_print_packet(&p);
                }
                cnt --;
                counter = 0;
                check = true;
            }
        }
    }

    mouse_cmd(0xEA);
    mouse_cmd(0xF5);

    cmd = minix_get_dflt_kbc_cmd_byte();
    
    sys_outb(STATUS_PORT, KBC_WRITE_CMD);
    tickdelay(micros_to_ticks(DELAY_US));
    if ((util_sys_inb(STATUS_PORT, &status)) != 0)
        return 1;
    if ((status & BIT(1)) == 1){
            return 1;
    }

    sys_outb(KBD_OUT_BUF, cmd);

    return 0;
}
