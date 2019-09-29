# Buzzer volume adjustment (with opamp)
__WARNING__: WIP, this is still under development

This circuit can be used to adjust the buzzer's volume. This is achieved by
decreasing the voltage across the buzzer.

The buzzer is represented by a 1k resistor, the AC source represents the
volume adjustment input (filtered PWM will be used) - I can see the range of the
adjustment with an AC signal, so it's easier to test than having a variable DC.
There are no capacitors in the circuit so the frequency shouldn't matter.

I'd like to avoid using the opamp.

## TODO
- Do not use an opamp
- Put a capacitor in series with the resistor that represents the buzzer

## Source
```
$ 1 0.000005 12.050203812241895 52 5 43
t 112 240 144 240 0 1 -1.1336888371233524 0.6282052635610982 100
r 144 112 144 176 0 1000
g 144 256 144 288 0
R 144 16 144 -32 0 0 40 5 0 0 0.5
s 144 16 144 112 0 0 false
p 192 112 192 176 1 0
w 144 112 192 112 0
w 144 176 192 176 0
r -16 240 48 240 0 10000
w 144 176 144 224 0
R -224 256 -272 256 0 1 10 2.5 2.5 0 0.5
403 -512 272 -160 432 0 10_256_0_4107_10_0.00009765625_0_2_10_3
403 240 16 544 144 0 5_256_0_4107_5_0.1_0_1
p 192 176 192 256 1 0
g 192 256 192 288 0
403 240 192 544 320 0 13_256_0_4107_5_0.1_0_1
w 48 240 112 240 0
a -128 240 -16 240 9 15 -15 1000000 2.0562575497268027 2.0562673642585914 100000
w -128 224 -240 224 0
w -240 224 -240 176 0
w -128 176 144 176 0
r -240 176 -128 176 0 1000
r -240 176 -368 176 0 10000
R -368 176 -368 112 0 0 40 5 0 0 0.5
w -224 256 -128 256 0
o 10 256 0 4098 10 0.0125 0 3 10 3 5 0
```
