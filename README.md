# LED pwm fader with arduino uno

### Problem: No aftermarket lights for my car

### Solution:
<img src="./resources/fade_in.gif">

This project got it's start when I decided I wanted to retrofit F-Series angel eyes to my E46 bmw.
There were no ready to go solution on market so started to build my own. It never actually got into
use since that car I was building this on, got sold.

It was a nice first touch to embedded systems and actually making something happen with my code.

Biggest challenge of this project was that human eye does not see brightness linearly...

It should be Logarithmic💡

Yeah, well I did not know this back then and it took days of debugging to figure out that even tho my log
seemed to be working nicely (it was basic while; loop) my brightness was not matching that.

After few days I found out another implementation of led fading in and out that was using logarithmic approach. It worked instantly. Now my log was weird since it was not going smoothly from 0 to 100 brightness but the leds were smooth. Damn human eyes so weird👀
