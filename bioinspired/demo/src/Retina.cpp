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

const std::string keys =
    "{image    |      | Input from image file }"
    "{video    | v    | Input from video file }"
    "{camera   | 0    | Index of input camera. If image or video is not specified, camera 0 will be used }"
    "{log      |      | Activate retina log sampling }"
    "{ocl      |      | Use OpenCL acceleration if possible }"
    "{help     |      | Print help}";

int main(int argc, char *argv[])
{
    // welcome message
    // std::cout<<"****************************************************"<<std::endl
    //   <<"* Retina demonstration : demonstrates the use of is a wrapper class of the Gipsa/Listic Labs retina model."<<std::endl
    //   <<"* This retina model allows spatio-temporal image processing (applied on still images, video sequences)."<<std::endl
    //   <<"* As a summary, these are the retina model properties:"<<std::endl
    //   <<"* => It applies a spectral whithening (mid-frequency details enhancement)"<<std::endl
    //   <<"* => high frequency spatio-temporal noise reduction"<<std::endl
    //   <<"* => low frequency luminance to be reduced (luminance range compression)"<<std::endl
    //   <<"* => local logarithmic luminance compression allows details to be enhanced in low light conditions\n"<<std::endl
    //   <<"* for more information, reer to the following papers :"<<std::endl
    //   <<"* Benoit A., Caplier A., Durette B., Herault, J., \"USING HUMAN VISUAL SYSTEM MODELING FOR BIO-INSPIRED LOW LEVEL IMAGE PROCESSING\", Elsevier, Computer Vision and Image Understanding 114 (2010), pp. 758-773, DOI: http://dx.doi.org/10.1016/j.cviu.2010.01.011"<<std::endl
    //   <<"* Vision: Images, Signals and Neural Networks: Models of Neural Processing in Visual Perception (Progress in Neural Processing),By: Jeanny Herault, ISBN: 9814273686. WAPI (Tower ID): 113266891."<<std::endl
    //   <<"* => reports comments/remarks at benoit.alexandre.vision@gmail.com"<<std::endl
    //   <<"* => more informations and papers at : http://sites.google.com/site/benoitalexandrevision/"<<std::endl
    //   <<"****************************************************"<<std::endl
    //   <<" NOTE : this program generates the default retina parameters file 'RetinaDefaultParameters.xml'"<<std::endl
    //   <<" => you can use this to fine tune parameters and load them if you save to file 'RetinaSpecificParameters.xml'"<<std::endl;

    cv::CommandLineParser parser(argc, argv, keys);
    if (!parser.check() || parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    bool useLogSampling = parser.has("log"); // check if user wants retina log sampling processing
    bool useOCL = parser.has("ocl");
    cv::ocl::setUseOpenCL(useOCL);
    if (useOCL && !cv::ocl::useOpenCL())
    {
        std::cout << "Failed to enable OpenCL\n";
    }

    // declare the retina input buffer... that will be fed differently in regard of the input media
    cv::Mat inputFrame;
    cv::VideoCapture videoCapture; // in case a video media is used, its manager is declared here

    // if(parser.has("video"))
    // std::cout << parser.get<cv::String>("video") << std::endl;
    // if (parser.has("video"))
    //     videoCapture.open("../../../statics/video1.avi");
    // else if (parser.has("image"))
    // inputFrame = cv::imread("../../../statics/cat.jpg");
    inputFrame = cv::imread("../../../statics/candle.png");

    // else
    //     videoCapture.open(parser.get<int>("camera"));

    // videoCapture >> inputFrame;

    if (inputFrame.empty())
    {
        std::cout << "Failed to open media source\n";
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////
    // Program start in a try/catch safety context (Retina may throw errors)
    try
    {
        // create a retina instance with default parameters setup, uncomment the initialisation you wanna test
        cv::Ptr<cv::bioinspired::Retina> myRetina;

        // // if the last parameter is 'log', then activate log sampling (favour foveal vision and subsamples peripheral vision)
        // if (useLogSampling)
        // {
        myRetina = cv::bioinspired::Retina::create(inputFrame.size(),
                                                   true,
                                                   cv::bioinspired::RETINA_COLOR_BAYER,
                                                   true,
                                                   1,
                                                   1);
        // }
        // else // -> else allocate "classical" retina :
        //     myRetina = cv::bioinspired::Retina::create(inputFrame.size());

        myRetina->activateContoursProcessing(true);
        myRetina->activateMovingContoursProcessing(true);
        // myRetina->setupIPLMagnoChannel(
        //     true,
        //     10.0f,
        //     10.0f,
        //     10.0f,
        //     10.0f,
        //     100.0f,
        //     100.0f,
        //     1000.0f
        // );
        // myRetina->setupOPLandIPLParvoChannel(
        //     true,
        //     true,
        //     0.0f,
        //     0.0f,
        //     0.0f,
        //     0.0f,
        //     100.0f,
        //     0.9f,
        //     0.7f);

        // myRetina->setColorSaturation(true, 0.9);

        // save default retina parameters file in order to let you see this and maybe modify it and reload using method "setup"
        // myRetina->write("RetinaDefaultParameters.xml");

        // load parameters if file exists
        // myRetina->setup("RetinaSpecificParameters.xml");
        myRetina->clearBuffers();

        // declare retina output buffers
        cv::UMat retinaOutput_parvo;
        cv::UMat retinaOutput_magno;

        // processing loop with stop condition
        int64 totalTime = 0;
        int64 totalFrames = 0;
        // while (true)
        {
            // if using video stream, then, grabbing a new frame, else, input remains the same
            // if (videoCapture.isOpened())
            //     videoCapture >> inputFrame;
            // if (inputFrame.empty())
            //     break;

            // cv::Mat saturation;
            // run retina filter
            int64 frameTime = cv::getTickCount();
            myRetina->run(inputFrame);
            // Retrieve and display retina output
            frameTime = cv::getTickCount() - frameTime;
            totalTime += frameTime;
            totalFrames++;
            // myRetina->getParvo(retinaOutput_parvo);
            // myRetina->getMagno(retinaOutput_magno);
            cv::imshow("retina input", inputFrame);
            // cv::imshow("Retina Parvo", retinaOutput_parvo);
            // cv::imshow("Retina Magno", retinaOutput_magno);

            cv::Mat toneMap;
            myRetina->applyFastToneMapping(inputFrame, toneMap);
            // cv::Mat test = toneMap - inputFrame;
            cv::imshow("Tone Map", toneMap);
            // cv::imshow("test", test);
            // cv::waitKey(0);
            char key = 0;
            do
            {
                key = cv::waitKey(0);
            } while (27 != key);
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
