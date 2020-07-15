/*!
    @file
*/

#ifndef _ALARM_h
#define _ALARM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Settings.h"
#include "Constants.h"
#include "PWMDimmer.h"
#include "HALbool.h"
#include "BuzzerManager.h"
#include "DaysOfWeek.h"
#include <RTClib.h> // for datetime


//#define AlarmClass_EEPROM_length identifier(1B) + sizeof(TimeStampClass - jen  2 byte) + sizeof(AlarmsEnabled - 1 byte) + sizeof(DaysOfWeekClass - jen 1 byte (eeprom)) + sizeof(Snooze) + sizeOf(Signalization)

#define AlarmEnabled_max 3  // for input validation
enum AlarmEnabled {
    Off = 0,
    Single = 1,
    Repeat = 2,
    Skip = 3
};
struct Snooze {
    byte time_minutes; //!< max 99
    byte count; //!< max 9
};

struct hours_minutes {
    byte hours;
    byte minutes;
};

/*!
    @brief  Stores information about the means that should be used to wake the
            user up when the alarm activates.
*/
struct Signalization {
    byte ambient; //!< ambient light intensity - dimmable LED strips
    bool lamp;
    bool buzzer;
};


/*!
    @brief  An alarm. In normal use, there is an array of multiple instances of
            this class.
*/
class AlarmClass
{
protected:
    // This variable needs to exist all the time because a function is
    // returning a pointer to it
    byte _EEPROM_data[EEPROM_AlarmClass_length];

    /*
    not saved in EEPROM:
    */
    // Needed in case the alarm gets canceled during the same minute it started
    // Also used for Alarm_timeout
    // Needs to be initialised to prev_activation_millis_init to prevent alarms
    // starting in the first minute of runtime being ignored.
    // This also makes unit tests work.
    unsigned long prev_activation_millis;
    // 0xFFFF0000 would also work...
#define prev_activation_millis_init 0xDEADBEEF

#define current_snooze_count_inactive 255
    byte current_snooze_count;  // max 9; 255 --> inactive alarm
    unsigned long prev_millis;  // used for timing snooze
    bool inhibit;
    bool snooze_status;  // currently in snooze

    HALbool *lamp;
    PWMDimmerClass *ambientDimmer;
    BuzzerManager *buzzer;
    void(*writeEEPROM_all)();  // write all alarms to EEPROM
    void(*activation_callback)();
    void(*stop_callback)();

    /*
    saved in the EEPROM:
    */
    hours_minutes _when;
    AlarmEnabled _enabled;
    DaysOfWeekClass _days_of_week;
    Snooze _snooze;
    Signalization _signalization;

    // true --> alarm is on (ringing or snooze)
    bool get_active() const {
        return current_snooze_count < current_snooze_count_inactive;
    };


    bool should_trigger(DateTime time);


public:
    bool readEEPROM(byte data[EEPROM_AlarmClass_length]);
    byte * writeEEPROM();

    void loop(DateTime time);
    void set_hardware(HALbool *lamp,
                      PWMDimmerClass *ambientDimmer,
                      BuzzerManager *buzzer,
                      void(*writeEEPROM)(),
                      void(*activation_callback)(),
                      void(*stop_callback)());
    void button_snooze();
    void button_stop();
    AlarmClass();


    bool set_enabled(AlarmEnabled enabled);
    AlarmEnabled get_enabled() const { return _enabled; };

    bool set_time(byte hours, byte minutes);
    hours_minutes get_time() const { return _when; };

    bool set_days_of_week(DaysOfWeekClass days_of_week);
    bool set_day_of_week(byte day, bool status);
    DaysOfWeekClass get_days_of_week() const { return _days_of_week; };
    bool get_day_of_week(byte day) const { return _days_of_week.getDayOfWeek(day); }

    bool set_snooze(byte time_minutes, byte count);
    Snooze get_snooze() const { return _snooze; };

    bool set_signalization(byte ambient, bool lamp, bool buzzer);
    Signalization get_signalization() const { return _signalization; };

    bool get_inhibit() const { return inhibit; };
    bool set_inhibit(bool inhibit);
};


#endif
