#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "timer.h"
#include "i8042.h"

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define registerA 10
#define registerB 11

//Registos para as datas e horas
#define DAY   7
#define MONTH 8
#define YEAR  9

#define HOURS   4
#define MINUTES 2
#define SECONDS 0

/** @defgroup rtc rtc
 * @{
 * 
 * Functions for managing RTC
 */
/**
 * @brief Checks if RTC is updating
 * 
 * @return Return 1 if updating, zero otherwise
 */
int checkRTCupdate();


/**
 * @brief Sets the parameters to the present day, month and year
 * 
 * 
 * @param day
 * @param month
 * @param year
 */
void date(uint32_t * day,uint32_t * month,uint32_t * year);

/**
 * @brief Converts from BCD to decimal
 * 
 * @param hex Value to be converted
 * @return Converted value
 */
uint32_t convertBCDToDecimal(uint32_t *hex);
/**@}*/
