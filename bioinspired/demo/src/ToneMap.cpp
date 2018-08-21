//============================================================================
// Name        : retinademo.cpp
// Author      : Alexandre Benoit, benoit.alexandre.vision@gmail.com
// Version     : 0.1
// Copyright   : LISTIC/GIPSA French Labs, july 2011
// Description : Gipsa/LISTIC Labs retina demo in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>

#include "opencv2/bioinspired.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char *argv[])
{
    cv::Mat inputFrame;
    cv::VideoCapture videoCapture; // in case a video media is used, its manager is declared here

    videoCapture.open("../../../statics/video1.avi");
    videoCapture >> inputFrame;

    if (inputFrame.empty())
    {
        std::cout << "Failed to open media source\n";
        return 0;
    }
    // cv::imshow("test",inputFrame);

    //////////////////////////////////////////////////////////////////////////////
    // Program start in a try/catch safety context (Retina may throw errors)
    try
    {
        // create a retina instance with default parameters setup, uncomment the initialisation you wanna test
        cv::Ptr<cv::bioinspired::RetinaFastToneMapping> myRetina;

        myRetina = cv::bioinspired::RetinaFastToneMapping::create(inputFrame.size());

        myRetina->setup(
            100,
            100,
            100);

        // processing loop with stop condition
        int64 totalTime = 0;
        int64 totalFrames = 0;

        while (true)
        {
            // if using video stream, then, grabbing a new frame, else, input remains the same
            if (videoCapture.isOpened())
                videoCapture >> inputFrame;
            if (inputFrame.empty())
                break;
            cvtColor(inputFrame, inputFrame, COLOR_BGR2GRAY);

            // cv::Mat saturation;
            // run retina filter
            cv::Mat toneMap;
            int64 frameTime = cv::getTickCount();
            myRetina->applyFastToneMapping(inputFrame, toneMap);
            // Retrieve and display retina output
            frameTime = cv::getTickCount() - frameTime;
            totalTime += frameTime;
            totalFrames++;

            cv::imshow("Tone Map", toneMap);
            // cv::imshow("inputFrame", inputFrame);

            cv::Scalar mean_map = cv::mean(toneMap);
            const cv::Scalar test = cv::Scalar(0, 0, 0, 0);
            if (test != mean_map)
                std::cout << "nice!" << std::endl;

            int key = cv::waitKey(5);
            if (key == 'q')
                break;
        }
        std::cout << "\nMean frame processing time: " << (totalTime / cv::getTickFrequency()) / totalFrames << " s" << std::endl;
        std::cout << "Retina demo end" << std::endl;
    }
    catch (const cv::Exception &e)
    {
        std::cerr << "Error using Retina : " << e.what() << std::endl;
    }
    return 0;
}
