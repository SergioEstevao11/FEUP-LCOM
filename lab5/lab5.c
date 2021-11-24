// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "keyboard.h"
#include "timer.h"
#include "i8042.h"
extern uint8_t scan_code, scanaux;
extern bool check;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

    if (vh_set_mode(mode) == 1){
        return 1;
    }

    sleep(delay);

    if(vg_exit()!=0)
        return -1;

    printf("Back to Text mode!\n");

    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  uint32_t irq_set;
  int r, ipc_status;
  uint8_t bytes[2];
  uint8_t size;
  message msg;
  bool make;

  if (vh_set_mode(mode) == 1){
        return 1;
    }

  if(kbd_subscribe_int(&irq_set) == 1){
    return 1;
  }
  irq_set = BIT(irq_set);

  vg_draw_rectangle(x,y,width,height,color);

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
                        if (!check){
                          return 1;
                        }
                        if (scan_code != 0xE0 && scanaux == 0xE0){
                          bytes[0] = scanaux;
                          bytes[1] = scan_code;
                          size = 2;
                          if((scan_code >> 7) == 1)
                            make = false;
                          else
                            make = true;
                        }
                        else if(scan_code != 0x00 && scanaux == 0x00){
                          bytes[0] = scan_code;
                          size = 1;
                            if((scan_code >> 7) == 1)
                            make = false;
                          else
                            make = true;
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

  if (kbd_unsubscribe_int() == 1){
    return 1;
  }

  if(vg_exit()!=0)
    return -1;
  
  return 0;
      
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint32_t irq_set;
  int r, ipc_status;
  uint8_t bytes[2];
  uint8_t size;
  message msg;
  bool make;

  if (vh_set_mode(mode) == 1){
        return 1;
    }

  if(kbd_subscribe_int(&irq_set) == 1){
    return 1;
  }
  irq_set = BIT(irq_set);

  pattern(mode, no_rectangles, first, step);

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
                        if (!check){
                          return 1;
                        }
                        if (scan_code != 0xE0 && scanaux == 0xE0){
                          bytes[0] = scanaux;
                          bytes[1] = scan_code;
                          size = 2;
                          if((scan_code >> 7) == 1)
                            make = false;
                          else
                            make = true;
                        }
                        else if(scan_code != 0x00 && scanaux == 0x00){
                          bytes[0] = scan_code;
                          size = 1;
                            if((scan_code >> 7) == 1)
                            make = false;
                          else
                            make = true;
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

  if (kbd_unsubscribe_int() == 1){
    return 1;
  }

  if(vg_exit()!=0)
    return -1;
  
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  uint32_t irq_set;
  int r, ipc_status;
  uint8_t bytes[2];
  uint8_t size;
  message msg;
  bool make;
  enum xpm_image_type type = XPM_INDEXED;
  xpm_image_t img;

  if (vh_set_mode(index_mode) == 1){
        return 1;
    }

  if(kbd_subscribe_int(&irq_set) == 1){
    return 1;
  }
  irq_set = BIT(irq_set);

  uint8_t * location = xpm_load(xpm, type, &img);
  vg_draw_pixmap(x, y, img.width, img.height, location);


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
                        if (!check){
                          return 1;
                        }
                        if (scan_code != 0xE0 && scanaux == 0xE0){
                          bytes[0] = scanaux;
                          bytes[1] = scan_code;
                          size = 2;
                          if((scan_code >> 7) == 1)
                            make = false;
                          else
                            make = true;
                        }
                        else if(scan_code != 0x00 && scanaux == 0x00){
                          bytes[0] = scan_code;
                          size = 1;
                            if((scan_code >> 7) == 1)
                            make = false;
                          else
                            make = true;
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

  if (kbd_unsubscribe_int() == 1){
    return 1;
  }

  if(vg_exit()!=0)
    return -1;
  
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  uint32_t irq_set_keyboard;
  uint8_t irq_set_timer;
  int r, ipc_status;
  uint8_t bytes[2];
  uint8_t size;
  message msg;
  bool make;
  enum xpm_image_type type = XPM_INDEXED;
  xpm_image_t img;

  int framecounter = 0;

  timer_set_frequency(0, fr_rate);

  if (vh_set_mode(index_mode) == 1){
        return 1;
    }

  if(kbd_subscribe_int(&irq_set_keyboard) == 1){
    return 1;
  }	
  
  if(timer_subscribe_int(&irq_set_timer) == 1){
    return 1;
  }
  irq_set_keyboard = BIT(irq_set_keyboard);
  irq_set_timer = BIT(irq_set_timer);

  uint8_t * location = xpm_load(xpm, type, &img);
  vg_draw_pixmap(xi, yi, img.width, img.height, location);


  while ((scan_code != ESC_BREAK)){
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            //received notification/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: //hardware interrupt notification/
                    if (msg.m_notify.interrupts & irq_set_keyboard) { //subscribed interrupt/
                        kbc_ih(); //variável check é usada para ver kbc_ih nao funcionar
                        if (!check){
                          return 1;
                        }
                        if (scan_code != 0xE0 && scanaux == 0xE0){
                          bytes[0] = scanaux;
                          bytes[1] = scan_code;
                          size = 2;
                          if((scan_code >> 7) == 1)
                            make = false;
                          else
                            make = true;
                        }
                        else if(scan_code != 0x00 && scanaux == 0x00){
                          bytes[0] = scan_code;
                          size = 1;
                            if((scan_code >> 7) == 1)
                            make = false;
                          else
                            make = true;
                        }
                    }
					          else if (msg.m_notify.interrupts & irq_set_timer){
                      if (xi == xf && yi == yf){
                        continue;
                      }
                      
                      if (speed < 0){
                        framecounter++;
                        if (framecounter == abs(speed)){
                          clear_screen(xi, yi, img.width, img.height);
                          if (xi == xf){
                            yi++;
                          }
                          else{
                            xi++;
                          }
                          vg_draw_pixmap(xi, yi, img.width, img.height, location);
                          framecounter = 0;
                        }
                      }
                      else{
                        clear_screen(xi, yi, img.width, img.height);
                        if (xi == xf){
                            yi+= speed;
                          }
                        else{
                          xi+= speed;
                        }
                        vg_draw_pixmap(xi, yi, img.width, img.height, location);
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

  if (kbd_unsubscribe_int() == 1){
    return 1;
  }
  if (timer_unsubscribe_int() == 1){
    return 1;
  }

  if(vg_exit()!=0)
    return 1;

  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
