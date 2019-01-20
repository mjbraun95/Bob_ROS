#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;

int main(void) {
    // Detect keypoints and compute descriptors with SURF
    int minHessian = 600; // Hessian filter applied I think? Idk
    Ptr<SURF> detector = SURF::create(minHessian);
    // Match descriptors using FLANN matcher
    FlannBasedMatcher matcher;

    // Detect and compute for image you are trying to find
    std::vector<KeyPoint> home_keypoints;
    Mat home_descriptors;
    // Read in checking image
    Mat home = imread("/home/isthatme/homesymbol.png", CV_LOAD_IMAGE_COLOR);
    if (!home.data) {
        std::cout << "Error reading image" << std::endl;
        return -1;
    }
    cvtColor(home, home, COLOR_BGR2GRAY); // convert to greyscale
    detector->detectAndCompute(home, Mat(), home_keypoints, home_descriptors);
    matcher.add(home_descriptors);

    std::vector<KeyPoint> home_45deg_keypoints;
    Mat home_45deg_descriptors;
    Mat home_45deg = imread("/home/isthatme/homesymbol_45deg.png", CV_LOAD_IMAGE_COLOR);
    if (!home.data) {
        std::cout << "Error reading image" << std::endl;
        return -1;
    }
    cvtColor(home_45deg, home_45deg, COLOR_BGR2GRAY); // convert to greyscale
    detector->detectAndCompute(home_45deg, Mat(), home_45deg_keypoints, home_45deg_descriptors);
    matcher.add(home_45deg_descriptors);

    // Open camera
    VideoCapture cap(0);
    if(!cap.isOpened()) {
        std::cout << "Failed to open camera!" << std::endl;
        return -1;
    }

    namedWindow("Input_1", 1);
    namedWindow("Input_2", 1);

    // Loop
    for (;;) {
        Mat input;
        cap >> input; // Read in from camera

        // Detect and compute for image you want to find image in
        std::vector<KeyPoint> input_keypoints;
        Mat input_descriptors;
        cvtColor(input, input, COLOR_BGR2GRAY); // convert to greyscale
        detector->detectAndCompute(input, Mat(), input_keypoints, input_descriptors);

        std::vector<DMatch> matches;
        matcher.match(input_descriptors, matches);

        // Calculate minimum and maximum distances between keypoints
        double max_dist = 0;
        double min_dist = 100; // in pixels?
        for (int i = 0; i < input_descriptors.rows; i++) {
            double dist = matches[i].distance;
            if (dist < min_dist) { min_dist = dist; }
            if (dist > max_dist) { max_dist = dist; }
        }

        // Only draw the "good" matches
        std::vector<DMatch> good_matches;

            for (int i = 0; i < (int) input_descriptors.rows; i++) {
            if (matches[i].distance <= max(2*min_dist, 0.02)) {
                good_matches.push_back(matches[i]);
            }
        }

        // Draw matches
        Mat image1_matches;
        Mat image2_matches;
        /*
        drawMatches(input, input_keypoints, home, home_keypoints, good_matches, image1_matches, 
                Scalar::all(-1), Scalar::all(-1), std::vector<char>(), 
                DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
        */
        drawMatches(input, input_keypoints, home_45deg, home_45deg_keypoints,
                good_matches, image2_matches, 
                Scalar::all(-1), Scalar::all(-1), std::vector<char>(), 
                DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    

        //imshow("Image_1", image1_matches);
        imshow("Image_2", image2_matches);

        if(waitKey(30) >= 0) { break; }

    }

    return 0;
}
