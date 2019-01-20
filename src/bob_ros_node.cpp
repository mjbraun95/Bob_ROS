#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;

int main(void) {
    Mat home = imread("/home/isthatme/homesymbol.png", CV_LOAD_IMAGE_COLOR);

    if (!home.data) {
        std::cout << "Error reading image" << std::endl;
        return -1;
    }

    int minHessian = 400; // Hessian filter applied I think? Idk
    Ptr<SURF> detector = SURF::create(minHessian);
    std::vector<KeyPoint> keypoints;
    detector->detect(home, keypoints);

    Mat image_w_keypoints;
    drawKeypoints(home, keypoints, image_w_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    

    imshow("Keypoints", image_w_keypoints);
    waitKey(30);

    return 0;
}
