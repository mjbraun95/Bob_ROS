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
cd opencv_contrib
git checkout 3.3.1
mkdir opencv/build
cd opencv/build
git checkout 3.3.1
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=build_dir/opencv_contrib/modules ../
make -j5
sudo make install
```
