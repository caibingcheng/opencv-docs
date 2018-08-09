/*M///////////////////////////////////////////////////////////////////////////////////////
 //
 //  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
 //
 //  By downloading, copying, installing or using the software you agree to this license.
 //  If you do not agree to this license, do not download, install,
 //  copy or use the software.
 //
 //
 //                           License Agreement
 //                For Open Source Computer Vision Library
 //
 // Copyright (C) 2013, OpenCV Foundation, all rights reserved.
 // Third party copyrights are property of their respective owners.
 //
 // Redistribution and use in source and binary forms, with or without modification,
 // are permitted provided that the following conditions are met:
 //
 //   * Redistribution's of source code must retain the above copyright notice,
 //     this list of conditions and the following disclaimer.
 //
 //   * Redistribution's in binary form must reproduce the above copyright notice,
 //     this list of conditions and the following disclaimer in the documentation
 //     and/or other materials provided with the distribution.
 //
 //   * The name of the copyright holders may not be used to endorse or promote products
 //     derived from this software without specific prior written permission.
 //
 // This software is provided by the copyright holders and contributors "as is" and
 // any express or implied warranties, including, but not limited to, the implied
 // warranties of merchantability and fitness for a particular purpose are disclaimed.
 // In no event shall the Intel Corporation or contributors be liable for any direct,
 // indirect, incidental, special, exemplary, or consequential damages
 // (including, but not limited to, procurement of substitute goods or services;
 // loss of use, data, or profits; or business interruption) however caused
 // and on any theory of liability, whether in contract, strict liability,
 // or tort (including negligence or otherwise) arising in any way out of
 // the use of this software, even if advised of the possibility of such damage.
 //
 //M*/

#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/saliency.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include "../include/GetCostTime.h"

using namespace std;
using namespace cv;
using namespace saliency;

static const char *keys =
    {"{@saliency_algorithm | | Saliency algorithm <saliencyAlgorithmType.[saliencyAlgorithmTypeSubType]> }"
     "{@video_name      | | video name            }"
     "{@start_frame     |1| Start frame           }"
     "{@training_path   |ObjectnessTrainedModel| Path of the folder containing the trained files}"};

static void help()
{
  cout << "\nThis example shows the functionality of \"Saliency \""
          "Call:\n"
          "./example_saliency_computeSaliency <saliencyAlgorithmSubType> <video_name> <start_frame> \n"
       << endl;
}

int main(int argc, char **argv)
{

  CommandLineParser parser(argc, argv, keys);

  String saliency_algorithm = parser.get<String>(0);
  String video_name = parser.get<String>(1);
  int start_frame = parser.get<int>(2);
  String training_path = parser.get<String>(3);

  if (saliency_algorithm.empty() || video_name.empty())
  {
    help();
    return -1;
  }

  //open the capture
  VideoCapture cap;
  cap.open(video_name);
  cap.set(CAP_PROP_POS_FRAMES, start_frame);

  if (!cap.isOpened())
  {
    help();
    cout << "***Could not initialize capturing...***\n";
    cout << "Current parameter's value: \n";
    parser.printMessage();
    return -1;
  }

  Mat frame;

  //instantiates the specific Saliency
  Ptr<Saliency> saliencyAlgorithm;

  Mat binaryMap;
  Mat image;

  if (saliency_algorithm.find("1") == 0) //SPECTRAL_RESIDUAL
  {
    Mat saliencyMap;
    saliencyAlgorithm = StaticSaliencySpectralResidual::create();
    FileStorage fs;

    GetCostTime time;
    time.GetCostTimeInit();
    double count = 0;

    for (;;)
    {
      cap >> frame;
      if (frame.empty())
      {
        break;
      }
      cvtColor(frame, frame, COLOR_BGR2GRAY);

      frame.copyTo(image);
      Size originSize = image.size();
      // resize(image, image, Size(256, 256));

      if (saliencyAlgorithm->computeSaliency(image, saliencyMap))
      {
        count++;
        if (count >= 200)
          waitKey(0);
        StaticSaliencySpectralResidual spec;
        spec.computeBinaryMap(saliencyMap, binaryMap);

        resize(binaryMap, binaryMap, originSize);
        cv::threshold(binaryMap, binaryMap, 0, 255, cv::THRESH_OTSU);
        // binaryMap &= frame;

        imshow("Saliency Map", saliencyMap);
        imshow("Original Image", frame);
        imshow("Binary Map", binaryMap);

        char c = waitKey(10);
        if (c == 27)
          break;
        if (c == 'p')
          c = waitKey(0);
      }
    }
    double costTime = time.CalCostTime();
    costTime /= count;
    std::cout << "average time = " << costTime << std::endl;
    std::cout << "counts = " << count << std::endl;
    // saliencyAlgorithm->write(fs);
  }
  else if (saliency_algorithm.find("2") == 0) //FINE_GRAINED
  {
    Mat saliencyMap;
    Mat binaryMap;
    saliencyAlgorithm = StaticSaliencyFineGrained::create();

    GetCostTime time;
    time.GetCostTimeInit();
    double count = 0;

    for (;;)
    {
      cap >> frame;
      if (frame.empty())
      {
        break;
      }

      cvtColor(frame, frame, COLOR_BGR2GRAY);
      frame.copyTo(image);
      Size originSize = image.size();
      // resize(image, image, Size(256, 256));

      if (saliencyAlgorithm->computeSaliency(image, saliencyMap))
      {
        count++;
        if (count >= 200)
          waitKey(0);

        cv::resize(saliencyMap, saliencyMap, originSize);
        cv::threshold(saliencyMap, binaryMap, 0, 255, cv::THRESH_OTSU);
        // binaryMap &= image;
        imshow("Binary Image", binaryMap);

        imshow("Saliency Map", saliencyMap);
        imshow("Original Image", image);
        char c = waitKey(10);
        if (c == 27)
          break;
      }
    }

    double costTime = time.CalCostTime();
    costTime /= count;
    std::cout << "average time = " << costTime << std::endl;
    std::cout << "counts = " << count << std::endl;
  }
  else if (saliency_algorithm.find("3") == 0) //BING
  {
    if (training_path.empty())
    {

      cout << "Path of trained files missing! " << endl;
      return -1;
    }

    else
    {
      saliencyAlgorithm = ObjectnessBING::create();
      vector<Vec4i> saliencyMap;
      saliencyAlgorithm.dynamicCast<ObjectnessBING>()->setTrainingPath(training_path);
      saliencyAlgorithm.dynamicCast<ObjectnessBING>()->setBBResDir("Results");
      // saliencyAlgorithm.dynamicCast<ObjectnessBING>()->write();

      GetCostTime time;
      time.GetCostTimeInit();
      double count = 0;

      for (;;)
      {
        cap >> frame;
        if (frame.empty())
        {
          break;
        }
        frame.copyTo(image);

        if (saliencyAlgorithm->computeSaliency(image, saliencyMap))
        {
          count++;

          if (count >= 200)
            waitKey(0);
          int ndet = int(saliencyMap.size());
          std::cout << "Objectness done " << ndet << std::endl;
          // double nss = saliencyAlgorithm.dynamicCast<ObjectnessBING>()->getBase();
          // std::cout << nss << std::endl;
          // The result are sorted by objectness. We only use the first maxd boxes here.
          int maxd = 7, step = 255 / maxd, jitter = 9; // jitter to seperate single rects
          Mat draw = image.clone();
          for (int i = 0; i < std::min(maxd, ndet); i++)
          {
            Vec4i bb = saliencyMap[i];
            Scalar col = Scalar(((i * step) % 255), 50, 255 - ((i * step) % 255));
            Point off(theRNG().uniform(-jitter, jitter), theRNG().uniform(-jitter, jitter));
            rectangle(draw, Point(bb[0] + off.x, bb[1] + off.y), Point(bb[2] + off.x, bb[3] + off.y), col, 2);
            //rectangle(draw, Rect(20, 20 + i * 10, 10, 10), col, -1); // mini temperature scale
          }
          imshow("BING", draw);
          imshow("saliencyMap", saliencyMap);
          char c = waitKey(10);
          if (c == 27)
            break;
        }
        else
        {
          std::cout << "No saliency found for " << video_name << std::endl;
        }
      }

      double costTime = time.CalCostTime();
      costTime /= count;
      std::cout << "average time = " << costTime << std::endl;
      std::cout << "counts = " << count << std::endl;
    }
  }
  else if (saliency_algorithm.find("4") == 0) //BinWangApr2014
  {
    cap >> frame;
    if (frame.empty())
    {
      return 0;
    }
    cvtColor(frame, frame, COLOR_BGR2GRAY);
    Size originSize = frame.size();
    Size convertSize = cv::Size(256, 256);
    // resize(frame, frame, convertSize);

    saliencyAlgorithm = MotionSaliencyBinWangApr2014::create();
    saliencyAlgorithm.dynamicCast<MotionSaliencyBinWangApr2014>()->setImagesize(frame.cols, frame.rows);
    // saliencyAlgorithm.dynamicCast<MotionSaliencyBinWangApr2014>()->setImageHeight(image.rows);
    // saliencyAlgorithm.dynamicCast<MotionSaliencyBinWangApr2014>()->setImageWidth(image.cols);
    saliencyAlgorithm.dynamicCast<MotionSaliencyBinWangApr2014>()->init();

    bool paused = false;
    Mat saliencyMap;

    GetCostTime time;
    time.GetCostTimeInit();
    double count = 0;

    for (;;)
    {
      if (!paused)
      {
        cap >> frame;
        if (frame.empty())
        {
          break;
        }
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        // frame.copyTo(image);
        // resize(frame, frame, convertSize);
        count++;
        if (count >= 200)
          waitKey(0);
        saliencyAlgorithm->computeSaliency(frame, saliencyMap);
        saliencyMap *= 255;
        resize(saliencyMap, saliencyMap, originSize);
        cv::threshold(saliencyMap, saliencyMap, 1, 255, 0);
        // saliencyMap &= image;

        imshow("image", frame);
        imshow("saliencyMap", saliencyMap);
      }

      char c = (char)waitKey(20);
      if (c == 27)
        break;
      if (c == 'p')
        paused = !paused;
    }

    double costTime = time.CalCostTime();
    costTime /= count;
    std::cout << "average time = " << costTime << std::endl;
    std::cout << "counts = " << count << std::endl;
  }

  return 0;
}