#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;

int main(void) {
    // Detect keypoints and compute descriptors with SURF
    int minHessian = 500;
    Ptr<SURF> detector = SURF::create(minHessian);
    // Match descriptors using FLANN matcher
    Ptr<BFMatcher> matcher = BFMatcher::create();

    // Detect and compute for image you are trying to find
    std::vector<KeyPoint> home_keypoints;
    Mat home_descriptors;
    // Read in checking image
    Mat home = imread("/home/isthatme/homesymbol_noborder.png", CV_LOAD_IMAGE_COLOR);
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
    namedWindow("Contours", 1);

    // Loop
    for (;;) {
        Mat input;
        cap >> input; // Read in from camera

        // Detect and compute for image you want to find image in
        std::vector<KeyPoint> input_keypoints;
        Mat input_descriptors;
        Mat input_thing, input_gray;
        cvtColor(input, input_thing, COLOR_BGR2GRAY); //convert to HSV
        cvtColor(input, input_gray, COLOR_BGR2GRAY); // convert to greyscale
        detector->detectAndCompute(input, Mat(), input_keypoints, input_descriptors);

        // Find matches
        std::vector< std::vector<DMatch> > matches;
        matcher->knnMatch(input_descriptors, matches, 2);

        // Find contours
        Mat mask;
        std::vector< std::vector<Point> > contours;
        std::vector<Vec4i> hier;
        Mat edges;
        inRange(input_thing, 127, 255, mask);
        Canny(input_thing, edges, 80, 170);
        findContours(edges, contours, hier, RETR_TREE, CHAIN_APPROX_SIMPLE);

        Mat contour_img = Mat::zeros(input.rows, input.cols, CV_8UC3);

        int idx = 0;
        for (int i = 0; i < static_cast<int>(hier.size()); i++) {
            if (hier[i][3] >= 0) {
                drawContours(contour_img, contours, i, Scalar(0, 0, 255));
            }
            if (hier[i][2] >= 0) {
                drawContours(contour_img, contours, i, Scalar(0, 255, 0));
            }

        }

        if (contours.size() > 1) {
            imshow("Contours", contour_img);
        }


        /*
         * matches is a 2d array of matches
         * the first dimension corresponds to descriptors that had matches.
         * the second dimension will be the k or less (from knnMatch()) best matches
         */


        std::vector<DMatch> good_matches;
        for (int i = 0; i < static_cast<int>(matches.size()); i++) {
            if (matches[i][0].distance < matches[i][1].distance * 0.7) {
                good_matches.push_back(matches[i][0]);
            }
        }

        // Draw matches
        Mat image1_matches;

        if (good_matches.size() >= 10) {
            drawMatches(input, input_keypoints, home, home_keypoints, good_matches, image1_matches, 
                    Scalar::all(-1), Scalar::all(-1), std::vector<char>(), 
                    DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

            imshow("Image_1", image1_matches);
        } else {
            std::cout << "Not enough Matches!" << std::endl;
        }


        if(waitKey(30) >= 0) { break; }

    }

    return 0;
}
