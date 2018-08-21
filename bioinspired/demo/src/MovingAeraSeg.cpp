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
        cv::Ptr<cv::bioinspired::TransientAreasSegmentationModule> myRetina;

        myRetina = cv::bioinspired::TransientAreasSegmentationModule::create(inputFrame.size());

        // save default retina parameters file in order to let you see this and maybe modify it and reload using method "setup"
        // myRetina->write("RetinaDefaultParameters.xml");

        // load parameters if file exists
        // myRetina->setup("RetinaSpecificParameters.xml");

        cv::bioinspired::SegmentationParameters params;
        // params.thresholdOFF = 1000;
        // params.thresholdON = 1000;
        // params.contextEnergy_spatialConstant = 5;
        // params.contextEnergy_temporalConstant = 100;
        // params.localEnergy_spatialConstant = 10;
        // params.localEnergy_temporalConstant = 100;
        // params.neighborhoodEnergy_spatialConstant = 1;
        params.neighborhoodEnergy_temporalConstant = 100;
        myRetina->setup(params);

        myRetina->clearAllBuffers();

        // processing loop with stop condition
        int64 totalTime = 0;
        int64 totalFrames = 0;

        cv::String strp = myRetina->printSetup();
        std::cout << strp << std::endl;
        while (true)
        {
            // if using video stream, then, grabbing a new frame, else, input remains the same
            if (videoCapture.isOpened())
                videoCapture >> inputFrame;
            if (inputFrame.empty())
                break;
            cv::cvtColor(inputFrame, inputFrame, cv::COLOR_BGR2GRAY);

            // cv::Mat saturation;
            // run retina filter
            cv::Mat segMap;
            int64 frameTime = cv::getTickCount();
            myRetina->run(inputFrame);
            myRetina->getSegmentationPicture(segMap);
            // Retrieve and display retina output
            frameTime = cv::getTickCount() - frameTime;
            totalTime += frameTime;
            totalFrames++;

            // cv::Scalar mean_map = cv::mean(segMap);
            // const cv::Scalar test = cv::Scalar(0, 0, 0, 0);
            // if (test != mean_map)
            //     std::cout << "nice!" << std::endl;

            // std::cout << (segMap.at<float>(1, 1)*255) << std::endl;
            cv::imshow("seg Map", segMap * 255);
            // cv::imshow("inputFrame", inputFrame);

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
