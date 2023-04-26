#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

void lucas_kanade(const std::string& filename) {
    // Read the video 
    cv::VideoCapture capture(filename);
    if (!capture.isOpened()){
        //error in opening the video input
        std::cerr << "Unable to open file!" << std::endl;
        return;
    }
 
    cv::Scalar color(255, 0, 0);
   
    cv::Mat old_frame, old_gray;
    std::vector<cv::Point2f> p0, p1;
    p0.push_back(cv::Point2d(200, 250));
    p0.push_back(cv::Point2d(250, 250));
    p0.push_back(cv::Point2d(250, 350));
    p0.push_back(cv::Point2d(200, 350));

    std::cout << p0.size() << std::endl;
    // Read first frame and find corners in it
    capture >> old_frame;

    cv::resize(old_frame, old_frame, cv::Size(640, 480), cv::INTER_LINEAR);

    cvtColor(old_frame, old_gray, cv::COLOR_BGR2GRAY);
 
    // Create a mask image for drawing purposes
    cv::Mat mask = cv::Mat::zeros(old_frame.size(), old_frame.type());

    while(true){
        // Read new frame
        cv::Mat frame, frame_gray;
        capture >> frame;
        cv::resize(frame, frame, cv::Size(640, 480), cv::INTER_LINEAR);

        if (frame.empty())
            break;
        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
        std::vector<cv::Point2f> good_new;

        // Calculate optical flow
        if(p0.size() > 0) {
            std::vector<uchar> status;
            std::vector<float> err;
            cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03);
            
            calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, cv::Size(15,15), 2, criteria);
            
            // Visualization part ADZD
            for(uint i = 0; i < p0.size(); i++){
                // Select good points
                if(status[i] == 1) {
                    good_new.push_back(p1[i]);
                    // Draw the tracks
                    //line(mask,p1[i], p0[i], colors[i], 2);
                    circle(frame, p1[i], 6, color, -1);
                }
            }
        
        // Display the demo
        cv::Mat img;
        cv::add(frame, mask, img);
        cv::imshow("flow", img);
        } else {
            cv::imshow("flow", frame);
        }
        int keyboard = cv::waitKey(25);
        if (keyboard == 'q' || keyboard == 27)
            break;
    
        // Update the previous frame and previous points
        old_gray = frame_gray.clone();
        p0 = good_new;
    }
}

int main(int argc, char** argv){
    lucas_kanade(argv[1]);
    return EXIT_SUCCESS;
}

