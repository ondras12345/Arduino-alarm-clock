# Checklist for tests
- [ ] Serial CLI
    - [ ] 'sel{i}' - select
    - [ ] 'sel' - deselect
    - [ ] 'ls'
    - [ ] 'en'
    - [ ] 'dis'
    - [ ] 'time{h}:{m}'
    - [ ] 'dow{d}:{s}'
    - [ ] 'snz{t};{c}'
    - [ ] 'sig{a};{l};{b}'
    - [ ] 'sav' (with DEBUG on) - saved
    - [ ] 'sav' (with DEBUG on) - nothing to save
    - [ ] Autosave (with DEBUG on)
    - [ ] 'rtc' - get time
    - [ ] 'sd{dd}.{mm}.{yy}' - set RTC date (year - 2000 !!)
    - [ ] 'st{h}:{m}' - set RTC time

- [ ] Buttons
    - [ ] 'snooze' button (with DEBUG on)
    - [ ] 'stop' button (with DEBUG on)

- [ ] Alarms
    - [ ] Trigger
        - sel{i}
        - rtc - get current time
        - time{h}:{m} - one minute to the future
        - dow{d}:{s} - enable this day
        - snz1:1
        - sig200;1;1
        - Wait for the alarm to trigger

    - [ ] Snooze
        - Do the 'Trigger' steps
        - Wait for the alarm to trigger
        - Press 'snooze' button
        - Wait 1 minute, alarm should retrigger
        - Ringing sound should be different (last ringing)
        - Press 'stop' button - alarm should stop

- [ ] RTC
    - Get time - see 'Serial CLI'
    - Set time - see 'Serial CLI'

- [ ] Compile time options
    - [ ] Alarm count
    - [ ] Alarm ringing frequency and period
    - [ ] DEBUG