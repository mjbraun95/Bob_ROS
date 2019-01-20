#!/usr/bin/env python2

import nxt.locator
import nxt.bluesock
from nxt.motor import Motor, PORT_A, PORT_B, PORT_C
from nxt.sensor import Color20, PORT_2, Ultrasonic, PORT_1, Type

class bob(object):

    def __init__(self):
        
        # This line looks for specific MAC address only, this is done for
        # quicker connection
        brick = nxt.bluesock.BlueSock('00:16:53:0C:FA:30').connect()

        self.thicc = 15.5 # Width of wheelbase in cm

        # Define variables for the components of the robot
        self.l_motor = Motor(brick, PORT_B)
        self.r_motor = Motor(brick, PORT_A)
        self.light = Color20(brick, PORT_2)
        self.usonic = Ultrasonic(brick, PORT_1)

    # Set robot to go straight
    def straight(self, power, regulation=True):
        self.l_motor.run(power, regulation)
        self.r_motor.run(power, regulation)

    # Rotate the robot on the spot
    def rotate(self, cw_power, regulation=True):
        self.l_motor.run(cw_power, regulation)
        self.r_motor.run(-cw_power, regulation)

    # Drive the robot along a curve with a specific turning radius
    def turn(self, power, direction, turn_radius, regulation=True): # Turning radius in centimeters
        outer_r = turn_radius + (self.thicc/2)
        inner_r = turn_radius - (self.thicc/2)
        outer_p = power
        inner_p = (inner_r/outer_r)*power

        if direction == 'left':
            self.l_motor.run(inner_p, regulation)
            self.r_motor.run(outer_p, regulation)
        elif direction == 'right':
            self.l_motor.run(outer_p, regulation)
            self.r_motor.run(inner_p, regulation)
    
    # Set the motors to idle
    def halt(self):
        self.l_motor.idle()
        self.r_motor.idle()

    # Change the colour of the light on the colour sensor
    def set_light(self, color):
        if color == 'red':
            self.light.set_light_color(Type.COLORRED)
        elif color == 'blue':
            self.light.set_light_color(Type.COLORBLUE)
        elif color == 'green':
            self.light.set_light_color(Type.COLORGREEN)
        elif color == 'off':
            self.light.set_light_color(Type.COLORNONE)

    # Measure the distance from the robots ultrasonic sensor, measurment is in cenitmeters
    def get_distance(self):
        distance = self.usonic.get_distance()
        return distance
        
