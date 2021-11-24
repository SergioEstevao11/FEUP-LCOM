#include "mouse.h"

int mouse_hook_id = MOUSE_IRQ;
bool check_mouse;
uint8_t pack;
uint8_t packetMouse[3];
int counter_mouse =0;

void mouse_builder(Mouse *mouse){
    mouse->color = (uint32_t *)xpm_load(ponteiro, XPM_8_8_8_8, &(mouse->img));
    mouse->x = 400;
    mouse->y = 300;
    mouse->lbpressed = false;
}

void update_mouse(struct packet *pp, Mouse * mouse){
    if (mouse->x + 10 + pp->delta_x < 800 && mouse->x - 10 + pp->delta_x >= 0){
        mouse->x += pp->delta_x;
    }
    if (mouse->y+20-pp->delta_y < 600 && mouse->y - pp->delta_y >= 0){
        mouse->y -= pp->delta_y;
    }

    if (pp->lb == 1){
        mouse->lbpressed = true;
    }
    else{
        mouse->lbpressed = false;
    }
}

int mouse_subscribe_int(uint32_t* bit_no){
  *bit_no = mouse_hook_id;
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK) return 1;
  return 0;
}

int (mouse_unsubscribe_int)(){
    if(sys_irqrmpolicy(&mouse_hook_id) != OK){
        printf("Failure removing policy\n");
        return 1;
    }
    return 0;
}
void (mouse_ih)(){
    uint8_t status;
    util_sys_inb(STATUS_PORT, &status);
    if ((status & OUT_BUF_STATUS) == 1){
        if (((status & PARITY_ERROR)== 1) || ((status & TIME_OUT_ERROR) == 1)){
            check_mouse = false;
            return ;
        }
        else{
            util_sys_inb(KBD_OUT_BUF, &pack);
            check_mouse = true;
            return;
        }
    }
    else{
        check_mouse = false;
        return;
    }
}

void packetbuilder(){
    if (check_mouse == false){
        return;
    }
    else if (counter_mouse == 0 && ((pack & BIT(3)) == 0)){
        check_mouse = false;
        return;
    }
    else{
        packetMouse[counter_mouse] = pack;
        counter_mouse++; //2 -> 3
    }
}

void arrangeStruct(struct packet * pp){
    pp->bytes[0] = packetMouse[0];
    pp->bytes[1] = packetMouse[1];
    pp->bytes[2] = packetMouse[2];
    pp->lb = BIT(0) & packetMouse[0];
    pp->rb = BIT(1) & packetMouse[0];
    pp->mb = BIT(2) & packetMouse[0];
    pp->delta_x = packetMouse[1] - ((packetMouse[0] & BIT(4)) << 4);
    pp->delta_y = packetMouse[2] - ((packetMouse[0] & BIT(5)) << 3);
    pp->x_ov = BIT(6) & packetMouse[0];
    pp->y_ov = BIT(7) & packetMouse[0];
}

int mouse_disable_data_reporting(){
    mouse_cmd(0xEA);
    if (mouse_cmd(0xF5) == 1) return 1;
    return 0;
}

int mouse_eenable_data_reporting(){
    mouse_cmd(0xEA);
    if(mouse_cmd(0xF4) == 1) return 1;
    return 0;
}

int mouse_cmd(uint8_t cmd){
    uint8_t status;
    uint8_t kbc_report = 0x00;
    while(kbc_report != 0xFC){
        if ((util_sys_inb(STATUS_PORT, &status)) != 0)
            return 1;
        if ((status & BIT(1)) == 1){
            break;
        }
        
        sys_outb(KBD_CMD_BUF, 0xD4);
        tickdelay(micros_to_ticks(DELAY_US));

        if ((util_sys_inb(STATUS_PORT, &status)) != 0)
            return 1;
        if ((status & BIT(1)) == 1){
            break;
        }

        sys_outb(KBD_OUT_BUF, cmd);

        tickdelay(micros_to_ticks(DELAY_US));

       if ((util_sys_inb(KBD_OUT_BUF, &kbc_report))!=0) {
            return 1;
       }
        if (kbc_report == 0xFA){
            return 0;
        }
    }
    return 1;
}


    
