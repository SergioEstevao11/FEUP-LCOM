#include "game.h"

extern uint8_t scan_code;
extern bool check_kbc, check_mouse;
extern int counter_mouse;
extern uint16_t packetMouse[];

int game(){
 
  uint32_t irq_set_keyboard, irq_set_mouse;
  uint8_t irq_set_timer;
  int r, ipc_status;
  message msg;
  Penguin Penguin;
  Object c;
  Object objects[15] = {c,c,c,c,c,c,c,c,c,c,c,c,c,c,c};

  Button b;
  Button buttons[13] = {b,b,b,b,b,b,b,b,b,b,b,b,b};
  Game game;
  ButtonType highlighted = noButton;

  Mouse mouse;

  struct packet p;
  
  uint32_t *day = (uint32_t*) malloc(sizeof(uint32_t));
  uint32_t *month = (uint32_t*) malloc(sizeof(uint32_t));
	uint32_t *year = (uint32_t*) malloc(sizeof(uint32_t));

  uint16_t mode = 0x115;

  timer_set_frequency(0, 30);

  mouse_eenable_data_reporting();

  if (vh_set_mode(mode) == 1){
        return 1;
    }

  if(kbd_subscribe_int(&irq_set_keyboard) == 1){
    return 1;
  }	
  
  if(timer_subscribe_int(&irq_set_timer) == 1){
    return 1;
  }

  if(mouse_subscribe_int(&irq_set_mouse) == 1){
    return 1;
  }

  irq_set_keyboard = BIT(irq_set_keyboard);
  irq_set_timer = BIT(irq_set_timer);
  irq_set_mouse = BIT(irq_set_mouse);
  GameBuilder(&game);
  buildPenguin(&Penguin);
  ButtonBuilder(buttons);
  mouse_builder(&mouse);
  updateBestTimes(&game,day,month,year);
  while (game.mode != end){ //(state != Quit)
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
                        if (game.mode == inGame){               
                          if (scan_code == 0x11 || scan_code == 0x48){ //up
                                updatePenguin(&Penguin, up,mode, objects,game.level);   
                              }
                          else if (scan_code == 0x1F || scan_code == 0x50){ //down
                                  updatePenguin(&Penguin, down,mode, objects,game.level);  
                              }
                          else if (scan_code == 0x1E || scan_code == 0x4B){ //left
                                  updatePenguin(&Penguin, left,mode, objects,game.level); 
                              }
                          else if (scan_code == 0x20 || scan_code == 0x4D){ //right
                                 updatePenguin(&Penguin, right,mode,objects,game.level);
                              }
                          else if(scan_code == ESC_BREAK){
                                  game.mode = paused;
                              }                        
                          if(checkWin(Penguin)){
                            date(day,month,year);
                            updateBestTimes(&game,day,month,year);
                            game.mode = victoryscreen;
                          
                          }      
                        }    
                        else if (game.mode == paused){ //Tirar do pause com esc
                            if(scan_code == ESC_BREAK){
                                game.mode = inGame;
                              }
                          }
                              
                    } 
                  if(msg.m_notify.interrupts & irq_set_mouse){
                    mouse_ih();
                    packetbuilder();
                    if (counter_mouse == 3){
                        if (check_mouse && (game.mode != inGame)){
                          arrangeStruct(&p);
                          update_mouse(&p, &mouse);
                          CheckMouse(mouse, buttons, &game, &highlighted, objects, &Penguin);
                        }
                        counter_mouse = 0;
                        check_mouse = true;
                    }
                  }

                  if(msg.m_notify.interrupts & irq_set_timer){
                    if (game.mode == inGame){
                      incrementGameTick(&game);
                      update_objects(objects,game.level);
                      if(!checkCollisions(Penguin,objects,game.level) || checkInWater(&Penguin,objects,game.level)){
                          game.mode = deathscreen; 
                          break;
                        }
                      update_screen(Penguin,objects,&game);
                      }
                    else{
                      update_menu(buttons, &game, highlighted, mouse, Penguin, objects);
                    }
                  }
                  break;

                default:
                    break; //no other notifications expected: do nothing/
              }
        }
        
        else {}
  }

  free_buffer();

  if (kbd_unsubscribe_int() == 1){
    return 1;
  }
  if (timer_unsubscribe_int() == 1){
    return 1;
  }
  if (mouse_unsubscribe_int() == 1){
    return 1;
  }
  if (mouse_disable_data_reporting() != 0){
        return 1;
  }

  saveScorestoFile(&game);

  if(vg_exit()!=0)
    return 1;

  return 0;
}
