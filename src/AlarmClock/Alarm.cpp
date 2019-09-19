// 
// 
// 

#include "Alarm.h"


void AlarmClass::loop(DateTime time)
{
    if (get_active()) { // alarm is already active
        if (get_current_snooze_status()) { // alarm is NOT ringing (snooze)
            if ((unsigned long)(millis() - previous_millis) >= (_snooze.time_minutes * 60000UL)) {
                set_current_snooze_status(false);
                if (_signalization.lamp) lamp(true);
                DEBUG_println(F("Alarm waking from snooze"));
            }

        }
        else if (_signalization.buzzer) { // alarm is ringing
	    // select either the regular or last ringing parameters
            unsigned long period = (get_current_snooze_count() == 0) ? Alarm_last_ringing_period : Alarm_regular_ringing_period;
            unsigned int frequency = (get_current_snooze_count() == 0) ? Alarm_last_ringing_frequency : Alarm_regular_ringing_frequency;

            if ((unsigned long)(millis() - previous_millis) >= period) { // inverse buzzer
                previous_millis = millis();
                if (get_current_beeping_status()) buzzerNoTone();
                else buzzerTone(frequency, 0);
                set_current_beeping_status(!get_current_beeping_status());
            }
        }

    }
    else { // alarm is not active
        if (_days_of_week.getDayOfWeek_Adafruit(time.dayOfTheWeek()) && time.hour() == _when.get_hours() && time.minute() == _when.get_minutes() && _enabled) { // time is matching
            if ((time - last_alarm).totalseconds() > 60) { // check for last_alarm - in case the alarm gets canceled during the same minute it started
                last_alarm = time;
                set_current_snooze_count(_snooze.count);
                set_current_snooze_status(false);

                // safety feature - if ambient() got stuck:
                if (_signalization.buzzer) buzzerTone(Alarm_regular_ringing_frequency, 0);

                // Do events - can only switch on
                if (_signalization.ambient > 0) ambient(0, _signalization.ambient, 900000UL); // 15 minutes
                if (_signalization.lamp) lamp(true);
                DEBUG_println(F("Alarm activated"));
            }
        }
    }
}

void AlarmClass::set_hardware(void(*lamp_)(boolean), void(*ambient_)(byte, byte, unsigned long), void(*buzzerTone_)(unsigned int, unsigned long), void(*buzzerNoTone_)())
{
    lamp = lamp_;
    ambient = ambient_;
    buzzerTone = buzzerTone_;
    buzzerNoTone = buzzerNoTone_;
}

// doesn't have any effect during last ringing
void AlarmClass::button_snooze()
{
    if (!get_current_snooze_status() && get_active()) {
        if (get_current_snooze_count() >= 1) {
            set_current_snooze_status(true);
            set_current_snooze_count(get_current_snooze_count() - 1);
            previous_millis = millis();

            // ambient(0, 0, 0);
            lamp(false);
            buzzerNoTone();
            set_current_beeping_status(false);
        }
    }
}

// stops everything (even if in snooze)
void AlarmClass::button_stop()
{
    current_snooze_count = AlarmClass_current_snooze_count_none;

    ambient(0, 0, 0);
    lamp(false);
    buzzerNoTone();
    //set_current_beeping_status(false); // this would break it (alarm would wake from snooze)
    // becasue current_snooze_count != AlarmClass_current_snooze_count_none
}

AlarmClass::AlarmClass()
{
    _when.timestamp = 0;
    _enabled = false;
    _days_of_week.DaysOfWeek = 0;
    _snooze = { 0,0 };
    _signalization = { 0, false, false };
    last_alarm = DateTime(2000, 1, 1);
    current_snooze_count = AlarmClass_current_snooze_count_none;
}

boolean AlarmClass::readEEPROM(byte data[EEPROM_AlarmClass_record_length])
{
#if defined(DEBUG) && defined(DEBUG_EEPROM_alarms)
    Serial.println();
    Serial.println(F("EEPROM alarm read:"));
    for (byte i = 0; i < EEPROM_AlarmClass_record_length; i++) {
        Serial.print(data[i], HEX);
        Serial.print(' ');
    }
    Serial.println();
#endif // DEBUG

    if (data[0] != EEPROM_alarms_identificator) return false;

    _when.timestamp = 0;
    _when.timestamp = data[1];
    _when.timestamp |= data[2] << 8;
    if (_when.get_hours() > 23 || _when.get_minutes() > 59) return false;

    _enabled = data[3];
    _days_of_week.DaysOfWeek = data[4];

    if (data[5] <= 99) _snooze.time_minutes = data[5];
    else return false;

    if (data[6] <= 9) _snooze.count = data[6];
    else return false;

    _signalization.ambient = data[7];
    _signalization.lamp = data[8];
    _signalization.buzzer = data[9];

    // not saved in the EEPROM:
    last_alarm = DateTime(2000, 1, 1);
    current_snooze_count = AlarmClass_current_snooze_count_none;

    DEBUG_println(F("EEPROM alarm read OK"));
    return true;
}

byte * AlarmClass::writeEEPROM()
{
    _EEPROM_data[0] = EEPROM_alarms_identificator;

    _EEPROM_data[1] = _when.timestamp & 0xFF;
    _EEPROM_data[2] = (_when.timestamp >> 8) & 0xFF;
    _EEPROM_data[3] = _enabled;
    _EEPROM_data[4] = _days_of_week.DaysOfWeek;
    _EEPROM_data[5] = _snooze.time_minutes;
    _EEPROM_data[6] = _snooze.count;
    _EEPROM_data[7] = _signalization.ambient;
    _EEPROM_data[8] = _signalization.lamp;
    _EEPROM_data[9] = _signalization.buzzer;

#if defined(DEBUG) && defined(DEBUG_EEPROM_alarms)
    Serial.println(F("EEPROM alarm write:"));
    for (byte i = 0; i < EEPROM_AlarmClass_record_length; i++) {
        Serial.print(_EEPROM_data[i], HEX);
        Serial.print(' ');
    }
    Serial.println();
#endif // DEBUG

    return _EEPROM_data;
}


boolean AlarmClass::set_enabled(boolean __enabled)
{
    _enabled = __enabled;
    return true;
}

boolean AlarmClass::set_time(byte __hours, byte __minutes)
{
    if (__hours > 23 || __minutes > 59) return false;
    _when = MinutesTimeStampClass(__hours, __minutes);
    return true;
}

boolean AlarmClass::set_days_of_week(DaysOfWeekClass __days_of_week)
{
    _days_of_week = __days_of_week;
    return true;
}

boolean AlarmClass::set_day_of_week(byte __day, boolean __status)
{
    return _days_of_week.setDayOfWeek(__day, __status);
}

boolean AlarmClass::set_snooze(byte __time_minutes, byte __count)
{
    if (__time_minutes > 99 || __count > 9) return false;
    _snooze.time_minutes = __time_minutes;
    _snooze.count = __count;
    return true;
}

boolean AlarmClass::set_signalization(byte __ambient, boolean __lamp, boolean __buzzer)
{
    _signalization.ambient = __ambient;
    _signalization.lamp = __lamp;
    _signalization.buzzer = __buzzer;
    return true;
}
