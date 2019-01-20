#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

// TODO: switch to FLANN matching
Ptr<BFMatcher> matcher;

vector<DMatch> find_good_matches(Mat in_descriptors, Mat image) {
    // List for storage of all matches   
    vector< vector<DMatch> > matches;
    vector<DMatch> good_matches;

    // Find matches
    matcher->knnMatch(in_descriptors, matches, 2);

    /*
     * matches is a 2d array of matches
     * the first dimension corresponds to descriptors that had matches.
     * the second dimension will be the k or less (from knnMatch()) best matches
     */
    for (int i = 0; i < static_cast<int>(matches.size()); i++) {
        // If match is significantly better than next best match
        if (matches[i][0].distance < matches[i][1].distance * 0.7) {
            good_matches.push_back(matches[i][0]);
            cout << "Found match" << endl;
        }
    }

    return good_matches;
}

int main(void) {
    // Detect keypoints and compute descriptors with SURF
    int minHessian = 500;
    Ptr<SURF> detector = SURF::create(minHessian);
    // Match descriptors using FLANN matcher
    matcher = BFMatcher::create();

    // Detect and compute for image you are trying to find
    vector<KeyPoint> input_keypoints;
    Mat input_descriptors;
    // Read in checking image
    Mat input = imread("/home/isthatme/homesymbol_noborder.png", CV_LOAD_IMAGE_COLOR);
    if (!input.data) {
        cout << "Error reading image" << endl;
        return -1;
    }
    cvtColor(input, input, COLOR_BGR2GRAY); // convert to greyscale
    detector->detectAndCompute(input, Mat(), input_keypoints, input_descriptors);
    matcher->add(input_descriptors);


    // Open camera
    VideoCapture cap(0);
    if(!cap.isOpened()) {
        cout << "Failed to open camera!" << endl;
        return -1;
    }

    namedWindow("Input_1", 1);
    namedWindow("Contours", 1);

    // Loop
    for (;;) {
        Mat frame;
        cap >> frame; // Read in from camera

        Mat frame_gray;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY); // convert to greyscale

        // Find descriptors for frame image
        vector<KeyPoint> frame_keypoints;
        Mat frame_descriptors;
        detector->detectAndCompute(frame_gray, Mat(), frame_keypoints, frame_descriptors);

        vector<DMatch> good_matches = find_good_matches(frame_descriptors, frame_gray);


        // Convert to edges
        Mat mask;
        Mat edges;
        inRange(frame_gray, 127, 255, mask);
        Canny(frame_gray, edges, 80, 170);

        // Find contours
        vector< vector<Point> > contours;
        vector<Vec4i> hier;
        findContours(edges, contours, hier, RETR_TREE, CHAIN_APPROX_SIMPLE);

        // Draw contours
        Mat contour_img = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
        int idx = 0;
        for (int i = 0; i < static_cast<int>(hier.size()); i++) {
            if (contourArea(contours[i]) > 400) {
                if (hier[i][3] >= 0) {
                    drawContours(contour_img, contours, i, Scalar(0, 0, 255));
                }
                if (hier[i][2] >= 0) {
                    drawContours(contour_img, contours, i, Scalar(0, 255, 0));
                }
            }
        }

        if (contours.size() > 1) { // only show images when there are contours
            imshow("Contours", contour_img);
        }


        // Draw matches
        Mat image1_matches;
        printf("Matches: %d", static_cast<int>(good_matches.size()));

        if (good_matches.size() >= 8) {
            drawMatches(frame, frame_keypoints, input, input_keypoints, good_matches, image1_matches, 
                    Scalar::all(-1), Scalar::all(-1), vector<char>(), 
                    DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

            imshow("Image_1", image1_matches);
        } else {
            cout << "Not enough Matches!" << endl;
        }


        if(waitKey(30) >= 0) { break; }

    }

    return 0;
}
