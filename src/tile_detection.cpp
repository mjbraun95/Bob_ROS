#include <opencv2/opencv.hpp>
using namespace cv;

int main() 
{
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) 
	{
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // show live and wait for a key with timeout long enough to show images
        imshow("Live", frame);
        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}