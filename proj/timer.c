#include "timer.h"

int hook_id = 0;

#include "i8254.h"


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t status, control;
  unsigned char new_freq1, new_freq2;
  if(timer < 0 || timer > 2) return 1; 

  uint32_t new_freq = TIMER_FREQ/freq;  //bit 4,5 sao os da frequencia

  if(freq > TIMER_FREQ){
     printf("Invalid frequency. It cannot be higher than 1193182.");
     return 1;
   }

   if(timer_get_conf(timer, &status) != 0){
    printf("error"); 
    return 1;
  }
  new_freq1 = (new_freq << 8)>> 8;
  control = TIMER_LSB_MSB | (status & 0x0F);
  new_freq2 = (new_freq >> 8);
  if(sys_outb(TIMER_CTRL, control)!= 0){
    printf("error");
    return 1;
  }
    switch(timer){
      case 0:
        if(sys_outb(TIMER_CTRL,control)!= 0)
          return 1;

        if (sys_outb(TIMER_0, new_freq1) != OK) {
			    printf("Error writing LSB\n");
			    return 1;
		    }
	
		  if (sys_outb(TIMER_0, new_freq2) != OK) {
			    printf("Error writing MSB\n");
			    return 1;
		    } 
        break;

      case 1:
        if(sys_outb(TIMER_CTRL,control)!= 0)
          return 1;
          
        if (sys_outb(TIMER_1, new_freq1) != OK) {
			    printf("Error writing LSB\n");
			    return 1;
		    }
	
		  if (sys_outb(TIMER_1, new_freq2) != OK) {
			    printf("Error writing MSB\n");
			    return 1;
		    } 
        break; 
      case 2:
        if(sys_outb(TIMER_CTRL,control)!= 0)
          return 1;
          
        if (sys_outb(TIMER_2, new_freq1) != OK) {
			    printf("Error writing LSB\n");
			    return 1;
		    }
	
		  if (sys_outb(TIMER_2, new_freq2) != OK) {
			    printf("Error writing MSB\n");
			    return 1;
		    } 
        break;
    }
    return 0;
  }


int (timer_subscribe_int)(uint8_t *bit_no) {
   
   hook_id = TIMER0_IRQ;
   printf("b4 sys_irqsetpolicy: hood_id = 0x%02x\n",hook_id);
   if(sys_irqsetpolicy(TIMER0_IRQ ,IRQ_REENABLE,& hook_id) == OK){
     *bit_no = TIMER0_IRQ;
     printf("after sys_irqsetpolicy: hook_id = 0x%x\n", hook_id);
     return 0;
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

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t rb;
  if(timer < 0 || timer > 2) return 1;
  rb = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if (sys_outb(TIMER_CTRL, rb) == 1){
    return 1;
  }
  if (st == NULL){
    return 1;
  }

  util_sys_inb(TIMER_0 + timer, st);

  return 0;
}
