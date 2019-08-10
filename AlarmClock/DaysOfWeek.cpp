// 
// 
// 

#include "DaysOfWeek.h"

boolean DaysOfWeekClass::getDayOfWeek(byte num)
{
    return bitRead(DaysOfWeek, num);
}

// day of the week using the standard of the Adafruit RTC library - 0=Sunday, 6=saturday
boolean DaysOfWeekClass::getDayOfWeek_Adafruit(byte num)
{
    if (num == 0) return bitRead(DaysOfWeek, 7);
    else return bitRead(DaysOfWeek, num);
}

void DaysOfWeekClass::setDayOfWeek(byte num, boolean status)
{
    bitWrite(DaysOfWeek, num, status);
}

void DaysOfWeekClass::setDayOfWeek_Adafruit(byte num, boolean status)
{
    if (num == 0) bitWrite(DaysOfWeek, 7, status);
    else bitWrite(DaysOfWeek, num, status);
}
