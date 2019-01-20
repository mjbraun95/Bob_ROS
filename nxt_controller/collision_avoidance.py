#!/usr/bin/env python2

from time import sleep
from drive_controls import bob

bob = bob()
power = 60



bob.set_light('off')

while 1 < 10:
    distance = bob.get_distance()
    print(distance)
    if 10 < distance < 48:
        #v_power = int(round((float(distance)/float(48)*float(power))))
        v_power = power
        turn_radius = 10

        print 'V_POWER: ', v_power
        print 'TURN: ', turn_radius
        bob.turn(v_power, 'right', turn_radius, True)
        bob.set_light('red')
    elif distance < 15:
        bob.set_light('blue')
        bob.turn(-float(power)/float(2),'right',20)
        sleep(3)
    else:
        bob.set_light('green')
        bob.straight(power)