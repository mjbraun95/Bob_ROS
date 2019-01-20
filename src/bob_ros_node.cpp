#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;

int main(void) {
    // Detect keypoints and compute descriptors with SURF
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create(minHessian);
    // Match descriptors using FLANN matcher
    Ptr<BFMatcher> matcher = BFMatcher::create();

    // Detect and compute for image you are trying to find
    std::vector<KeyPoint> home_keypoints;
    Mat home_descriptors;
    // Read in checking image
    Mat home = imread("/home/matthew/Desktop/bob_ros/img/tileImageStraight1.png", CV_LOAD_IMAGE_COLOR);
    if (!home.data) {
        std::cout << "Error reading image" << std::endl;
        return -1;
    }
    cvtColor(home, home, COLOR_BGR2GRAY); // convert to greyscale
    detector->detectAndCompute(home, Mat(), home_keypoints, home_descriptors);
    matcher->add(home_descriptors);


    // Open camera
    VideoCapture cap(0);
    if(!cap.isOpened()) {
        std::cout << "Failed to open camera!" << std::endl;
        return -1;
    }

    namedWindow("Input_1", 1);

    // Loop
    for (;;) {
        Mat input;
        cap >> input; // Read in from camera

        // Detect and compute for image you want to find image in
        std::vector<KeyPoint> input_keypoints;
        Mat input_descriptors;
        cvtColor(input, input, COLOR_BGR2GRAY); // convert to greyscale
        detector->detectAndCompute(input, Mat(), input_keypoints, input_descriptors);

        std::vector< std::vector<DMatch> > matches;
        matcher->knnMatch(input_descriptors, matches, 2);

        /*
         * matches is a 2d array of matches
         * the first dimension corresponds to descriptors that had matches.
         * the second dimension will be the k or less (from knnMatch()) best matches
         */


        std::vector<DMatch> good_matches;
        for (int i = 0; i < static_cast<int>(matches.size()); i++) {
            if (matches[i][0].distance < matches[i][1].distance * 0.5) {
                good_matches.push_back(matches[i][0]);
            }
        }

        // Draw matches
        Mat image1_matches;

        if (good_matches.size() >= 1) {
            drawMatches(input, input_keypoints, home, home_keypoints, good_matches, image1_matches, 
                    Scalar::all(-1), Scalar::all(-1), std::vector<char>(), 
                    DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

            imshow("Image_1", image1_matches);
        } else {
            std::cout << "No matches!" << std::endl;
        }


        if(waitKey(30) >= 0) { break; }

    }

    return 0;
}
