/*
 Name:		AlarmClock.ino
 Created:	6/12/2019 5:26:39 PM
 Author:	Ondra

Days of week in GUI and EEPROM: 1=Monday

# TODO implement watchdog - external or internal (bootloader problems - flash optiboot)

Code directives:
 - all comments in English

 - Todo comments must contain '# TODO' (for automated searching)

 - constants should be #defined or const, never write te actual number to the code

 - millis() checking: https://www.baldengineer.com/arduino-how-do-you-reset-millis.html
     if ((unsigned long)(millis() - previousMillis) >= interval)

 - returning array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm
*/

// Compile-time options
#define alarms_count 6
#define VisualStudio
#define DEBUG

// 2,3 - reserved for buttons / rotary encoder
#define pin_lamp 4
#define pin_buzzer 5 // PWM
#define pin_ambient 6 // PWM
#define pin_LCD_enable 7
// 9, 10, 11, 12, 13 - reserved for SPI (ethernet, SD card)

#define I2C_LCD_address 0x27
#define I2C_DS3231_address 0x68
#define LCD_width 16
#define LCD_height 2

// error codes for self test
#define error_I2C_ping_DS3231 1
#define error_time_lost 2
#define error_critical_mask 0b1111111111111101 // time_lost is not critical

#ifdef DEBUG
#define DEBUG_print(x) Serial.print(x)
#define DEBUG_println(x) Serial.println(x)
#else
#define DEBUG_print(x)
#define DEBUG_println(x)
#endif // DEBUG


enum SpinDirection { // for rotary encoder
    left = 1,
    right = 2
};

enum SelfTest_level {
    POST,
    time
};


#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "Alarm.h"
#include "CountdownTimer.h"
#include "PWMfade.h"



// Global variables
LiquidCrystal_I2C lcd(I2C_LCD_address, LCD_width, LCD_height);
RTC_DS3231 rtc; // DS3231
AlarmClass alarms[alarms_count];
CountdownTimerClass countdownTimer;
PWMfadeClass ambientFader(pin_ambient);
DateTime now;

// function prototypes
#ifdef VisualStudio
unsigned int SelfTest(SelfTest_level level);
#endif
// Hardware
//void lamp(boolean status);
void buzzerTone(unsigned int freq, unsigned long duration = 0); // specifies default duration=0
//void buzzerNoTone();
//void ambient(byte intensity);


void setup() {
    // # TODO pinMode()s
    init_hardware();
    Wire.begin();
    Serial.begin(9600);
    lcd_on();
    unsigned int error = SelfTest(POST);
    if ((error & error_critical_mask) == 0) error = !readEEPROM();

    error |= SelfTest(time); // rtc.begin() can be omited (only calls Wire.begin())

    if ((error & error_critical_mask) != 0) factory_reset(); // # TODO nejdriv zobraz chybu, zaloguj pokud to neni chyba eeprom, pak pockej na uzivatele

}

void loop() {
    now = rtc.now(); // # TODO longer interval between reads ; # TODO + summer_time

    for (byte i = 0; i < alarms_count; i++) alarms[i].loop(now);
    countdownTimer.loop();

}

void init_hardware() {
    for (byte i = 0; i < alarms_count; i++) alarms[i].set_hardware(lamp, ambient, buzzerTone, buzzerNoTone);
    countdownTimer.set_hardware(lamp, ambient, buzzerTone, buzzerNoTone);
}


/*
EEPROM
*/
#define EEPROM_alarms_offset 10

boolean readEEPROM() {
    boolean error = false;
    // basic config:

    // alarms:
    for (byte i = 0; i < alarms_count && !error; i++) {
        byte data[AlarmClass_EEPROM_record_length];
        for (byte j = 0; j < AlarmClass_EEPROM_record_length; j++) {
            data[j] = EEPROM.read((i * AlarmClass_EEPROM_record_length) + j + EEPROM_alarms_offset);
        }
        error |= !alarms[i].readEEPROM(data);
    }

    return !error;
}

boolean writeEEPROM() {
    boolean error = false;
    // basic config:

    // alarms:
    for (byte i = 0; i < alarms_count && !error; i++) {
        byte * data = alarms[i].writeEEPROM();
        for (byte j = 0; j < AlarmClass_EEPROM_record_length; j++) {
            EEPROM.write((i * AlarmClass_EEPROM_record_length) + j + EEPROM_alarms_offset, data[j]);
        }
    }


    return !error; // # TODO
}

/*
Factory reset
*/
void factory_reset() {
    for (byte i = 0; i < alarms_count; i++) alarms[i] = AlarmClass();
    countdownTimer = CountdownTimerClass();

    boolean error = !writeEEPROM();
}


/*
LCD
*/
boolean lcd_on() {
    digitalWrite(pin_LCD_enable, HIGH);
    delay(100);
    if (I2C_ping(I2C_LCD_address)) {
        lcd.init();
        return true;
    }
    else return false;
}

void lcd_off() {
    digitalWrite(pin_LCD_enable, LOW);
}


/*
Self test
*/
unsigned int SelfTest(SelfTest_level level) {
    unsigned int error = 0; // each bit signalizes an error

    if (level == POST) {
        //digitalWrite(..., HIGH);
        tone(pin_buzzer, 1000, 100);
        delay(400);
        //digitalWrite(..., LOW);
    }

    if (level == POST || level == time) {
        if (!I2C_ping(I2C_DS3231_address)) error |= error_I2C_ping_DS3231;

    }

    if (level == time) {
        if ((error & error_I2C_ping_DS3231) == 0) { // DS3231 is responding ((POST || time) code block was executed earlier)
            if (rtc.lostPower()) error |= error_time_lost;
            if (rtc.now().year() == 2000) error |= error_time_lost;
        }
    }

    return error;
}

boolean I2C_ping(byte addr) {
    Wire.beginTransmission(addr);
    return (Wire.endTransmission() == 0);
}

/*
Hardware
Included classes can control the hardware trough these functions
*/
void lamp(boolean status) { digitalWrite(pin_lamp, status); }
void buzzerTone(unsigned int freq, unsigned long duration) { tone(pin_buzzer, freq, duration); } // default value duration=0 specified in prototype
void buzzerNoTone() { noTone(pin_buzzer); }
void ambient(byte start, byte stop, unsigned long duration) {
    int step_sign = (start > stop) ? 1 : -1;
    byte diff = abs(stop - start);
    int _step = 0;
    unsigned long _interval = 250;
    unsigned long _duration = duration;

    if (duration < 1000) _duration = 1000;

    _interval = _choose_interval(_duration, diff);
    _step = step_sign * ((_interval * diff) / _duration);
    if (_step == 0) _step = step_sign; // step must not be 0
    
    DEBUG_print("ambient - diff: ");
    DEBUG_println(diff);

    DEBUG_print("ambient - duration: ");
    DEBUG_println(_duration);

    DEBUG_print("ambient - interval: ");
    DEBUG_println(_interval);

    DEBUG_print("ambient - step: ");
    DEBUG_println(_step);


    ambientFader.set(start, stop, _step, _interval);
    ambientFader.start();
}

unsigned long _choose_interval(unsigned long duration, byte diff) {
    // prefered interval >= 250
    unsigned long interval = duration;
    unsigned long previousInterval = duration;
    int step;

    do {
        step = (interval * diff) / duration;
        previousInterval = interval;
        interval = interval / 2;
    } while ((step > 1 || step == 0) && interval > 250);

    if (interval < 250) interval = previousInterval;

    return interval;
}