#include "rtc.h"

int checkRTCupdate(){ // SE FOR 1 ESTÁ EM UPDATE

	uint32_t registoA;

	sys_outb(RTC_ADDR_REG,registerA);
	sys_inb(RTC_DATA_REG, &registoA);  //bit 7(UIP) do registo A indica se o rtc esta em update ou nao,so podemos ver dia e horas se nao estiver em update

	return (registoA & BIT(7));
}

uint32_t convertBCDToDecimal(uint32_t *hex){
     return (((*hex) & 0xF0) >> 4) * 10 + ((*hex) & 0x0F);
}

void date(uint32_t * day,uint32_t * month, uint32_t * year){
    if(checkRTCupdate() == 1)
        return;
    sys_outb(RTC_ADDR_REG, DAY);
	sys_inb(RTC_DATA_REG, day);

	sys_outb(RTC_ADDR_REG, MONTH);
	sys_inb(RTC_DATA_REG, month);

	sys_outb(RTC_ADDR_REG, YEAR);
	sys_inb(RTC_DATA_REG, year);

    (*day) = convertBCDToDecimal(day);
	(*month) = convertBCDToDecimal(month);
	(*year) = convertBCDToDecimal(year);
}
