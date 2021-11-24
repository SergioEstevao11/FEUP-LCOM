#include <lcom/lcf.h>

#include <lcom/lab2.h>

#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

#include <minix/sysutil.h>

#include "i8042.h"
#include "keyboard.h"

extern uint8_t scan_code, scanaux;
extern int sys_inb_counter;
extern bool check;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  sys_inb_counter = 0;
  uint32_t irq_set;
  int r, ipc_status;
  uint8_t bytes[2];
  uint8_t size;  
  message msg;
  bool make;
  if(kbd_subscribe_int(&irq_set) == 1){
    return 1;
  }
  irq_set = BIT(irq_set);
  while (scan_code != ESC_BREAK){
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            //received notification/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: //hardware interrupt notification/
                    if (msg.m_notify.interrupts & irq_set) { //subscribed interrupt/
                        kbc_ih(); //variável check é usada para ver kbc_ih nao funcionar
                        

                        if(check){
                          if(scanaux == 0xE0){
                            bytes[0] = scanaux;
                            bytes[1] = scan_code;
                            size = 2;
                          }
                          else{
                            bytes[0] = scan_code;
                            size = 1;
                          }

                          if((scan_code >> 7) == 1){
                            make = false;
                          }
                          else{
                            make = true;
                          }

                          kbd_print_scancode(make, size, bytes);
                          scanaux = 0x00;
                          check = false;
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
  if (kbd_print_no_sysinb(sys_inb_counter) == 1){
    return 1;
  }
  if (kbd_unsubscribe_int() == 1){
    return 1;
  }
  return 0;

}

int(kbd_test_poll)() {
  sys_inb_counter = 0;
  uint8_t status = 0x00;
  uint8_t bytes[2];
  uint8_t size;  
  bool make;
  while (scan_code != ESC_BREAK){
        sys_inb_count(STATUS_PORT, &status);
        if (((status & OUT_BUF_STATUS) == 1) && ((status & BIT(5)) == 0)){
          kbc_ih(); //variável check é usada para ver kbc_ih nao funcionar
          if(check){
            if(scanaux == 0xE0){
              bytes[0] = scanaux;
              bytes[1] = scan_code;
              size = 2;
            }
            else{
              bytes[0] = scan_code;
              size = 1;
            }

            if((scan_code >> 7) == 1){
              make = false;
            }
            else{
              make = true;
            }

            kbd_print_scancode(make, size, bytes);
            scanaux = 0x00;
            check = false;
          }
        }
        
  }
  if (kbd_print_no_sysinb(sys_inb_counter) == 1){
    return 1;
  }
  if (interrupt_restart() == 1){
    return 1;
  }
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  sys_inb_counter = 0;
  uint32_t irq_set_keyboard;
  uint8_t irq_set_timer;
  int r, ipc_status;
  int tick = 0, seconds = 0;
  uint8_t bytes[2];
  uint8_t size;  
  message msg;
  bool make;
  if(kbd_subscribe_int(&irq_set_keyboard) == 1){
    return 1;
  }
  if(timer_subscribe_int(&irq_set_timer) == 1){
    return 1;
  }
  irq_set_keyboard = BIT(irq_set_keyboard);
  irq_set_timer = BIT(irq_set_timer);
  while ((scan_code != ESC_BREAK) && (seconds < n)){
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            //received notification/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: //hardware interrupt notification/
                    if (msg.m_notify.interrupts & irq_set_keyboard) { //subscribed interrupt/     
                        seconds = 0;
                        tick = 0;
                        
                        kbc_ih(); //variável check é usada para ver kbc_ih nao funcionar
                      
                        if(check){
                          if(scanaux == 0xE0){
                            bytes[0] = scanaux;
                            bytes[1] = scan_code;
                            size = 2;
                          }
                          else{
                            bytes[0] = scan_code;
                            size = 1;
                          }

                          if((scan_code >> 7) == 1){
                            make = false;
                          }
                          else{
                            make = true;
                          }

                          kbd_print_scancode(make, size, bytes);
                          scanaux = 0x00;
                          check = false;
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_timer){
                      tick++;
                      if (tick % 60 == 0){
                        seconds++;
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
  if (kbd_print_no_sysinb(sys_inb_counter) == 1){
    return 1;
  }
  if (kbd_unsubscribe_int() == 1){
    return 1;
  }
  if (timer_unsubscribe_int() == 1){
    return 1;
  }
  return 0;

}
