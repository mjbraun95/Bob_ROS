#!/usr/bin/env python2

from time import sleep
from drive_controls import bob

robot = bob()

i = 0

while i < 20:
    robot.set_light('red')
    sleep(0.25)
    robot.set_light('blue')
    sleep(0.25)
    robot.set_light('green')
    sleep(0.25)
    i = i + 1

robot.set_light('off')