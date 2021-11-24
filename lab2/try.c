int timr_get_conf(uint8_t timer, uint8_t *s){
    uint8_t control, status;
    uint8_t cmd;
    cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | BIT(1+timer);
    if (sys_outb(TIMER_CTRL, cmd) != 0){
        return 1;
    }
    if(s == NULL){
        return 1;
    }
    util_sys_inb(TIMER_0, s);

    return 0;
}

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
  if(timer == 0)
    control = TIMER_LSB_MSB | (status & 0x0F);
  else if(timer == 1){
    control = TIMER_SEL1 | TIMER_LSB_MSB | (status & 0x0F);
  }
  else{
    control = TIMER_SEL2 | TIMER_LSB_MSB | (status & 0x0F);
  }

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