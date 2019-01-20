#!/usr/bin/env python
import rospy
from std_msgs.msg import String
import threading
import time

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)

def ros_init():
    global nxt_command

    rospy.init_node('nxt_listener', anonymous=True)

    nxt_command = rospy.Subscriber("/nxt_command", String, callback)


class spinThread(threading.Thread):
    def __init(self):
        super(spinThread, self).__init__()

    def run(self):
        rospy.spin()

t_ros = spinThread()


if __name__ == '__main__':
    ros_init()

    t_ros.daemon = True
    t_ros.start()

    while not rospy.is_shutdown():
        time.sleep(1)