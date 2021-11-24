#include <lcom/lcf.h>
#include <stdint.h>


int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(lsb == NULL)
    return 1;
  *lsb = val & 0x00FF;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if(msb == NULL)
    return 1;
  *msb = (*msb >> 8) & 0x00FF; 
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
