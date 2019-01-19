# Bob-ROS
Just a friendly robot that doesn't make any mistakes, just happy little accidents.

# Dependencies
ROS Kinetic Kame

Python 2.6 =< Ver < 3.0:
```
nxt-python2
PyBluez # For Bluetooth connectivity (Windows or Linux)
PyUSB # For usb communication
```

OpenCV non-free components (for SURF feature detection).
To install on Ubuntu 16.04 you need to compile.

```
cd build_dir
git clone https://github.com/opencv/opencv
git clone https://github.com/opencv/opencv_contrib
mkdir opencv/build
cd opencv/build
cmake -D 

```
