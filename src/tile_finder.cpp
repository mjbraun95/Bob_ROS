#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(void) {
    Mat frame;
    VideoCapture cap;
    cap.open(0);

    if (!cap.isOpened()) {
        return -1;
    }

    namedWindow("Webcam", 1);
    namedWindow("BW", 2);

    for (;;) {
        cap.read(frame);

        Mat edges;
        Mat frame_bw;
        cvtColor(frame, frame_bw, CV_BGR2GRAY);
        Scalar m = mean(frame_bw);
        Canny(frame_bw, edges, m[0]*0.75, m[0]*1.25);
        vector< vector<Point> > contours;
        vector<Vec4i> hier;
        findContours(edges, contours, hier, RETR_TREE, CHAIN_APPROX_SIMPLE);


        Mat image = Mat::zeros(frame.rows, frame.cols, CV_8UC3);

        for (int i = 0; i < static_cast<int>(contours.size()); i++) {
            if (contourArea(contours[i]) >= 300) {
                vector<Point> poly;
                approxPolyDP(contours[i], poly, arcLength(contours[i], 1) * 0.01, 1);
                if (poly.size() == 4) {
                    drawContours(image, contours, i, Scalar(20, 30, 240), 4);
                }
            }
        }



        if (contours.size() > 0) {
            imshow("Webcam", image);
        }

        imshow("BW", frame_bw);


        if(waitKey(30) >= 0) {
            break;
        }
    }



    return 0;
}
