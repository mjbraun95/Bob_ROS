#!/usr/bin/env python

import nxt.locator
import nxt.bluesock
from nxt.motor import *

class bob(object):

    def __init__(self):
        
        # This line looks for specific MAC address only, this is done for
        # quicker connection
        brick = nxt.bluesock.BlueSock('00:16:53:0C:FA:30').connect()

        self.thicc = 15.5 # With of wheelbase in cm

        self.l_motor = Motor(brick, PORT_B)
        self.r_motor = Motor(brick, PORT_A)

    def straight(self, power):
        self.l_motor.run(power)
        self.r_motor.run(power)

    def rotate(self, cw_power):
        self.l_motor.run(cw_power)
        self.r_motor.run(-cw_power)

    def turn(self, power, direction, turn_radius):
        outer_r = turn_radius + (self.thicc/2)
        inner_r = turn_radius - (self.thicc/2)
        outer_p = power
        inner_p = (inner_r/outer_r)*power

        if direction == 'left':
            self.l_motor.run(inner_p)
            self.r_motor.run(outer_p)
        elif direction == 'right':
            self.l_motor.run(outer_p)
            self.r_motor.run(inner_p)
    
    def halt(self):
        self.l_motor.brake()
        self.r_motor.brake()
        

