# TimerOne
This tests if TimerOne does interfere with Serial (it was reported by someone
on the Arduino forum). It is also a proof of concept of using duty cycle of the
PWM to affect amplitude of the sound.

## Result
- TimerOne doesn't interfere with Serial in my case

## Warnings
- Arduino MEGA must use pin 11, 12 or 13 instead of 9, 10
- It sounds really bad with a piezo, duty doesn't affect the amplitude
(even with a RC filter). 

## Notes
- Tested with Arduino MEGA
- pinMode() is not needed


# Tone
This is a example code with tone() for comparison

## Notes
- Tested with Arduino MEGA
- pinMode() is not needed
