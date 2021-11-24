#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

int hook_id = 0;

#include "i8254.h"

uint32_t time_counter;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(freq > TIMER_FREQ)
    return 1;

  uint16_t newFreq = TIMER_FREQ/freq;
  uint8_t status, control;
  timer_get_conf(timer, &status);

  if(timer == 0)
    control = (status & 0x0F) | TIMER_LSB_MSB;
  else if(timer == 1)
    control = (status & 0x0F) | TIMER_LSB_MSB | TIMER_SEL1;
  else if(timer == 2)
    control = (status & 0x0F) | TIMER_LSB_MSB | TIMER_SEL2;
  else 
    return 1;
  
  sys_outb(TIMER_CTRL, control);

  uint8_t freq1, freq2;
  
  freq1 = (newFreq << 8) >> 8;
  freq2 = (newFreq >> 8);

  sys_outb(TIMER_0+timer, freq1);
  sys_outb(TIMER_0+timer, freq2);

  return 0;

}


int (timer_subscribe_int)(uint8_t *bit_no) {
   
   hook_id = TIMER0_IRQ;
   printf("b4 sys_irqsetpolicy: hood_id = 0x%02x\n",hook_id);
   if(sys_irqsetpolicy(TIMER0_IRQ ,IRQ_REENABLE,&hook_id) == OK){
     *bit_no = TIMER0_IRQ;
     printf("after sys_irqsetpolicy: hook_id = 0x%x\n", hook_id);
     return hook_id;
   }
   else { 
     printf("sys_irqsetpolicy() failed\n");
     return 1;
    }  
}

int (timer_unsubscribe_int)() {
  if (sys_irqdisable(&hook_id) != OK) {
		printf("Failure disabling IRQ line\n");
		return 1;
	}

	if (sys_irqrmpolicy(&hook_id) != OK) {
		printf("Failure removing policy\n");
		return 1;
	}

	return 0;
}

void (timer_int_handler)() {
  time_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *s){
  uint8_t cmd;
  cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | BIT(timer+1);
  if (sys_outb(TIMER_CTRL, cmd) != 0){
      return 1;
  }
  if(s == NULL){
      return 1;
  }
  util_sys_inb(TIMER_0+timer, s);

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum   timer_status_field field) {
    if(timer < 0 || timer > 2) return 1;

    uint8_t init,base,mode;

    union timer_status_field_val  config;

    switch(field){
      case tsf_all:
        config.byte = st;
        break;
      case tsf_initial:
        init = ((0x30 & st) >> 4);
        enum timer_init beginTimer;
        switch(init){
          case 0:
           beginTimer = INVAL_val;
           break;
          case 1:
           beginTimer = LSB_only;
           break;
          case 2:
           beginTimer = MSB_only;
           break;
          case 3:
           beginTimer = MSB_after_LSB;
           break;
        }
        config.in_mode = beginTimer;
        break;
      case tsf_mode:        
        mode = (( 0x0E & st) >> 1);
        if((mode & (BIT(2) | BIT(1) | BIT(0))) == (BIT(2) | BIT(1) | BIT(0)))
          config.count_mode = 0x03;
        else if((mode & (BIT(2) | BIT(1))) == (BIT(2) | BIT(1))){
          config.count_mode = 0x02;
        }
        else{
          config.count_mode = mode;
        }
        break;
      case tsf_base:
        base = (st & 0x01);
        config.bcd = base;
        break;
      default : return 1;
     }
    if (timer_print_config(timer,field,config)) return 1;
  return 0;
}
