#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern uint32_t time_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t s;
  if(timer < 0 || timer > 2) return 1;
  if(timer_get_conf(timer,&s) != OK){
    printf("error");
    return 1;
  }
  timer_display_conf(timer,s,field);
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if(timer_set_frequency(timer,freq)==0)
		return 0;

	return 1;
}

int(timer_test_int)(uint8_t time) {
    int ipc_status;
    uint8_t irq_set;
    int r;
    uint8_t freq = 60;
    timer_subscribe_int(&irq_set);
    irq_set = BIT(irq_set);
    time_counter = 0;
    message msg;
    while( time_counter != time*60) {//You may want to use a different condition/
                                                 //Get a request message./
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            //received notification/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: //hardware interrupt notification/
                    if (msg.m_notify.interrupts & irq_set) { //subscribed interrupt/
                        timer_int_handler();
                        if (time_counter % freq == 0){
                          timer_print_elapsed_time();
                        }

                                //process it/
                    }
                    break;
                default:
                    break; //no other notifications expected: do nothing/
            }
        } else { //received a standard message, not a notification/
                                                     //no standard messages expected: do nothing/
        }
    }
    timer_unsubscribe_int();
    return 0;
}
