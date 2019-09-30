# Buzzer volume adjustment (with opamp)
__WARNING__: WIP, this is still under development

This circuit can be used to adjust the buzzer's volume. This is achieved by
decreasing the voltage across the buzzer.

The buzzer is represented by a 1k resistor in series with a 100n capacitor,
the AC source represents the volume adjustment input (filtered PWM will
be used) - I can see the range of the adjustment with an AC signal, so it's
easier to test than having a variable DC.

The diode is needed to avoid destroying the opamp by negative voltage on the
inputs.

The voltage divider on the feedback loop is there to make the window where
the voltage across the buzzer is zero wider (so that it's possible to set zero
volume with non-zero voltage).

I'd like to avoid using the opamp.

## TODO
- Do not use an opamp

## Source
```
$ 1 0.000005 12.050203812241895 52 5 43
t 112 240 144 240 0 1 0.3768937930166478 -0.0021423301967767277 100
r 144 112 144 176 0 1000
g 144 256 144 288 0
s 144 16 144 64 0 0 false
p 192 64 192 176 1 0
w 144 64 192 64 0
w 144 176 192 176 0
r -16 240 48 240 0 10000
w 144 176 144 224 0
R -224 256 -272 256 0 1 10 2.5 2.5 0 0.5
403 -512 272 -160 432 0 9_256_0_4107_10_0.00009765625_0_2_9_3
403 240 16 544 144 0 4_256_0_4107_5_0.1_0_1
p 192 176 192 256 1 0
g 192 256 192 288 0
403 240 192 544 320 0 12_256_0_4107_10_0.1_0_1
w 48 240 112 240 0
a -128 240 -16 240 9 5 0 1000000 0.6123103142222788 0.10996716071506862 100000
w -128 224 -240 224 0
w -240 224 -240 176 0
w 80 176 144 176 0
r -240 176 -128 176 0 1000
r -240 176 -368 176 0 10000
R -368 176 -368 112 0 0 40 5 0 0 0.5
w -224 256 -128 256 0
R 144 16 144 -32 0 1 3000 2.5 2.5 0 0.5
c 144 112 144 64 0 1.0000000000000001e-7 -1.7018676426907149
d 80 256 80 176 2 default
g 80 256 80 288 0
w 80 176 -128 176 0
o 9 256 0 4098 10 0.0125 0 3 9 3 4 0
```
