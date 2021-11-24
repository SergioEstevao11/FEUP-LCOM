#include "mouse.h"

int mouse_hook_id = MOUSE_IRQ;
bool check;
uint8_t pack;
uint8_t packetMouse[3];
int counter =0;

//Used in mouse_gesture
bool done = false;
uint8_t totalX = 0, totalY = 0;

int mouse_subscribe_int(uint8_t* bit_no){
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
            check = false;
            return ;
        }
        else{
            util_sys_inb(KBD_OUT_BUF, &pack);
            check = true;
            return;
        }
    }
    else{
        check = false;
        return;
    }
}

void packetbuilder(){
    if (check == false){
        return;
    }
    else if (counter == 0 && ((pack & BIT(3)) == 0)){
        check = false;
        return;
    }
    else{
        packetMouse[counter] = pack;
        counter++; //2 -> 3
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

int util_sys_inb(int port, uint8_t *value) {
  uint32_t num32;
  if (sys_inb(port, &num32) != OK){
    return 1;
  }
  *value = num32;
  return 0;
}

int mouse_disable_data_reporting(){
    if (mouse_cmd(0xF5) == 1) return 1;
    return 0;
}

int mouse_eenable_data_reporting(){
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

bool checkTolerance(float tolerance, struct packet *pp){
    if (pp->lb == 1){
        if (pp->delta_x < 0){
            if (tolerance < abs(pp->delta_x))
                return false;
        }
        if (pp->delta_y < 0){
            if (tolerance < abs(pp->delta_y))
                return false;
        }
        return true;
    }
    else if (pp->rb == 1){
        if (pp->delta_x < 0){
            if (tolerance < abs(pp->delta_x))
                return false;
        }
        if (pp->delta_y > 0){
            if (tolerance < abs(pp->delta_y))
                return false;
        }
        return true;
    }
    else{
        return true;
    }
}

void gesture_manager(uint8_t x_len, uint8_t tolerance, struct packet *pp, States *st){
    switch (*st){
        case UPLINE:{
            if (pp->rb == 0 && pp->lb == 1 && pp->mb == 0){
                if (checkTolerance(tolerance, pp) == false){
                    totalX = 0;
                    totalY = 0;
                    break;
                }
                else{
                    totalX += abs(pp->delta_x);
                    totalY += abs(pp->delta_y);
                    break;
                }

            }
            else{
                if (checkTolerance(tolerance, pp) == false){
                    totalX = 0;
                    totalY = 0;
                    break;
                }
                else if ((totalX >= x_len) && (totalY > totalX)){
                    *st = TRANSITION;
                    totalX = 0;
                    totalY = 0;
                    break;
                }
                else{
                    totalX = 0;
                    totalY = 0;
                    break;
                }
            }
        }

        case TRANSITION:{
            if (pp->rb == 1 && pp->lb == 0 && pp->mb == 0){
                *st = DOWNLINE;
                totalX += abs(pp->delta_x);
                totalY += abs(pp->delta_y);
            }
            else if(pp->rb == 0 && pp->lb == 1 && pp->mb == 0){
                *st = UPLINE;
                totalX += abs(pp->delta_x);
                totalY += abs(pp->delta_y);
            }
            else{
                *st = UPLINE;
            }
        }

        case DOWNLINE:{
            if (pp->rb == 1 && pp->lb == 0 && pp->mb == 0){
                if (checkTolerance(tolerance, pp) == false){
                    *st = UPLINE;
                    totalX = 0;
                    totalY = 0;
                    break;
                }
                else{
                    totalX += abs(pp->delta_x);
                    totalY += abs(pp->delta_y);
                    break;
                }

            }
            else if (pp->rb == 0 && pp->lb == 0 && pp->mb == 0){
                if (checkTolerance(tolerance, pp) == false){
                    *st = UPLINE;
                    totalX = 0;
                    totalY = 0;
                    break;  
                }
                else if (totalX >= x_len && totalY > totalX){
                    totalX = 0;
                    totalY = 0;
                    done = true;
                }
                else{
                    *st = UPLINE;
                    totalX = 0;
                    totalY = 0;
                    break;  
                }
            }
            else{
                *st = UPLINE;
                totalX = 0;
                totalY = 0;
                break;  
            }
        }
    }
}
    
