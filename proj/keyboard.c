#include <keyboard.h>


uint8_t scan_code, scanaux;
int kbd_hook_id = 1;  //hook_id do keyboard
int util_sys_inber; //conta qts vezes sys_inb é chamada.
bool check_kbc;


void (kbc_ih)(){
    uint8_t status;
    util_sys_inb(STATUS_PORT,&status);
    scanaux = 0x00;
    if (scan_code == 0xE0){
        scanaux = scan_code;
    }
    if((status & OUT_BUF_STATUS) == 1){
      util_sys_inb(KBD_OUT_BUF,&scan_code);
    }
    if (((status & PARITY_ERROR)== 1) || ((status & TIME_OUT_ERROR) == 1)){
        check_kbc = false;
        return ;
    }
    check_kbc = true;
    return ;
}

int (kbd_subscribe_int)(uint32_t *bit_no) {
    *bit_no = KBD_IRQ;

   printf("b4 sys_irqsetpolicy: hood_id = 0x%02x\n",kbd_hook_id);
   if(sys_irqsetpolicy(KBD_IRQ ,IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) == OK){
     printf("after sys_irqsetpolicy: hook_id = 0x%x\n", kbd_hook_id);
     return kbd_hook_id;
   }
   else { 
     printf("sys_irqsetpolicy() failed\n");
     return 1;
    }

}

int (kbd_unsubscribe_int)(){
    if(sys_irqrmpolicy(&kbd_hook_id) != OK){
        printf("Failure removing policy\n");
        return 1;
    }
    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t num32;
  if (sys_inb(port, &num32) != OK){
    return 1;
  }
  *value = num32;
  return 0;

}

int KBC_send_cmd(uint8_t c){
  uint8_t status = 0;
  int counter = 0;

  while(counter < 10){
    util_sys_inb(STATUS_PORT, &status);
    if ((status & IN_BUF_STATUS) == 0){
      sys_outb(KBD_CMD_BUF, c);
      return 0;
    }
    counter++;
  }
  return 1;
}
