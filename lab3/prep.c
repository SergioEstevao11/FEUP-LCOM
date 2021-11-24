#include <keyboard.h>

uint8_t scan_code = 0x00, scanaux;
int kbd_hook_id = 1;  //hook_id do keyboard
int sys_inb_counter; //conta qts vezes sys_inb é chamada.
bool check;

void sys_inb_count(port_t port, uint8_t *value){
    util_sys_inb(port, value);
    sys_inb_count++;
}

void (kbc_ih)(){
    uint8_t status;
    sys_inb_count(STATUS_PORT, &status);
    scanaux = 0x00;
    if(scan_code == 0xE0){
        scanaux = 0xE0;
    }

    if((status & OUT_BUF_STATUS) == 1){
        sys_inb_count(KBD_OUT_BUF, &scan_code);
    }

    if(((status & PARITY_ERROR) == 1) || ((status & TIME_OUT_ERROR) == 1)){
        check = false;
        return;
    }

    check = true;
    return;
}


int (kbd_subscribe_int)(unit32_t *bit_no){
    *bit_no = KBD_IRQ;

    if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) == OK){
        return 0;
    }
    else{
        return 1;
    }
}

int (kbd_unsubscribe_int)(){
    if(sys_irqrmpolicy(&kbd_hook_id) == OK){
        return 0;
    }
    else{
        return 1;
    }
}


int (util_sys_inb)(int port, uint8_t *value){
    uint32_t num;
    if(sys_inb(port, &num) != OK){
        return 1;
    }

    *value = num;
    return 0;
}

int KBC_send_cmd(uint8_t c){
    uint8_t status;
    int counter = 0;

    while(counter < 10){
        sys_inb_count(STATUS_PORT, &status);

        if((status & IN_BUF_STATUS) == 0){
            sys_outb(KBC_CMD_BUF, c);
            return 0;
        }

        counter++;
    }
    return 1;
}
