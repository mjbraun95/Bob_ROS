#!/usr/bin/env python2

from time import sleep
from drive_controls import bob

robot = bob()

sleep(5)

robot.halt()
robot.straight(100, True)

sleep(2)
robot.halt()
sleep(2)
robot.straight(-25, True)

sleep(2)
robot.halt()
sleep(2)
robot.rotate(100, True)

sleep(2)
robot.halt()
sleep(2)
robot.turn(50,'left',10, True)
sleep(2)
robot.halt()
