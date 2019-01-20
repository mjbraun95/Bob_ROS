#!/usr/bin/env python2

from time import sleep
from drive_controls import bob

robot = bob()

while 1<10:
    print(robot.get_distance())