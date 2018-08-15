
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include "../include/samples_utility.hpp"

using namespace std;
using namespace cv;

static const char* keys =
{ "{@tracker_algorithm | | Tracker algorithm }"
    "{@video_name      | | video name        }"
    "{@start_frame     |0| Start frame       }"
    "{@bounding_frame  |0,0,0,0| Initial bounding frame}"};

static void help()
{
  cout << "\nThis example shows the functionality of \"Long-term optical tracking API\""
       "-- pause video [p] and draw a bounding box around the target to start the tracker\n"
       "Example of <video_name> is in opencv_extra/testdata/cv/tracking/\n"
       "Call:\n"
       "./tracker <tracker_algorithm> <video_name> <start_frame> [<bounding_frame>]\n"
       "tracker_algorithm can be: MIL, BOOSTING, MEDIANFLOW, TLD, KCF, GOTURN, MOSSE.\n"
       << endl;

  cout << "\n\nHot keys: \n"
       "\tq - quit the program\n"
       "\tp - pause video\n";
}

int main( int argc, char** argv ){
  CommandLineParser parser( argc, argv, keys );

  String tracker_algorithm = parser.get<String>( 0 );
  String video_name = parser.get<String>( 1 );
  int start_frame = parser.get<int>( 2 );

  if( tracker_algorithm.empty() || video_name.empty() )
  {
    help();
    return -1;
  }

  int coords[4]={0,0,0,0};
  bool initBoxWasGivenInCommandLine=false;
  {
      String initBoundingBox=parser.get<String>(3);
      for(size_t npos=0,pos=0,ctr=0;ctr<4;ctr++){
        npos=initBoundingBox.find_first_of(',',pos);
        if(npos==string::npos && ctr<3){
           printf("bounding box should be given in format \"x1,y1,x2,y2\",where x's and y's are integer cordinates of opposed corners of bdd box\n");
           printf("got: %s\n",initBoundingBox.substr(pos,string::npos).c_str());
           printf("manual selection of bounding box will be employed\n");
           break;
        }
        int num=atoi(initBoundingBox.substr(pos,(ctr==3)?(string::npos):(npos-pos)).c_str());
        if(num<=0){
           printf("bounding box should be given in format \"x1,y1,x2,y2\",where x's and y's are integer cordinates of opposed corners of bdd box\n");
           printf("got: %s\n",initBoundingBox.substr(pos,npos-pos).c_str());
           printf("manual selection of bounding box will be employed\n");
           break;
        }
        coords[ctr]=num;
        pos=npos+1;
      }
      if(coords[0]>0 && coords[1]>0 && coords[2]>0 && coords[3]>0){
          initBoxWasGivenInCommandLine=true;
      }
  }

  //open the capture
  VideoCapture cap;
  cap.open( video_name );
  cap.set( CAP_PROP_POS_FRAMES, start_frame );

  if( !cap.isOpened() )
  {
    help();
    cout << "***Could not initialize capturing...***\n";
    cout << "Current parameter's value: \n";
    parser.printMessage();
    return -1;
  }

  Mat frame;
  namedWindow( "Tracking API", 1 );

  Mat image;
  Rect2d boundingBox;
  bool paused = false;

  //instantiates the specific Tracker
  //Ptr<Tracker> tracker = createTrackerByName(tracker_algorithm);

  // cv::TrackerBoosting::Params params;
  // // params.iterationInit = 50;
  // // params.numClassifiers = 200;
  // // params.samplerOverlap = 50;
  // params.samplerSearchFactor = 50;
  // Ptr<TrackerBoosting> tracker = cv::TrackerBoosting::create(params);
 
  cv::TrackerCSRT::Params params;
  params.window_function = "hann";
  params.use_channel_weights = true;
  params.use_color_names = true;
  params.use_gray = true;
  params.use_hog = true;
  params.use_rgb = true;
  params.use_segmentation = true;
  // params.admm_iterations = 100;
  // params.background_ratio = 100;
  // params.cheb_attenuation = 10;
  // params.filter_lr = 1.5;
  // params.gsl_sigma = 8;
  // params.histogram_bins = 100;
  // params.histogram_lr = 10;
  // params.hog_clip = 10;
  // params.hog_orientations = 100;
  // params.kaiser_alpha = 100;
  // params.num_hog_channels_used = 10;
  // params.number_of_scales = 100;
  // params.padding = 10;
  // params.scale_lr = 1;
  // params.scale_model_max_area = 30;
  // params.scale_sigma_factor = 0.1;
  // params.scale_step = 10;
  // params.template_size = 100;
  Ptr<TrackerCSRT> tracker = cv::TrackerCSRT::create(params);

  // cv::TrackerKCF::Params params;
  // // params.compress_feature = true;
  // // params.compressed_size = 10;
  // // params.pca_learning_rate = 0.9;
  // // params.resize = false;
  // // params.desc_pca = cv::TrackerKCF::CN;
  // // params.detect_thresh = 0.5;
  // // params.lambda = 0.2;
  // // params.sigma = 1.5;
  // // params.split_coeff = true;
  // // params.wrap_kernel = true;
  // // params.interp_factor = 10.5;
  // // params.max_patch_size = 100;
  // // params.output_sigma_factor = 0.4;
  // Ptr<TrackerKCF> tracker = cv::TrackerKCF::create(params);


  // cv::TrackerMedianFlow::Params params;
  // params.maxLevel = 10;
  // params.maxMedianLengthOfDisplacementDifference = 10.0;
  // params.pointsInGrid = 10;
  // params.winSize = cv::Size(10,10);
  // params.winSizeNCC = cv::Size(10,10);
  // Ptr<TrackerMedianFlow> tracker = cv::TrackerMedianFlow::create(params);


  // cv::TrackerMIL::Params params;
  // // params.featureSetNumFeatures = 100;
  // params.samplerInitInRadius = 5.0;
  // params.samplerInitMaxNegNum = 50;
  // params.samplerSearchWinSize = 10.0;
  // params.samplerTrackInRadius = 10.0;
  // params.samplerTrackMaxNegNum = 100;
  // params.samplerTrackMaxPosNum = 1;
  // Ptr<TrackerMIL> tracker = cv::TrackerMIL::create(params);

  if( tracker == NULL )
  {
    cout << "***Error in the instantiation of the tracker...***\n";
    return -1;
  }

  //get the first frame
  cap >> frame;
  frame.copyTo( image );
  if(initBoxWasGivenInCommandLine){
      boundingBox.x = coords[0];
      boundingBox.y = coords[1];
      boundingBox.width = std::abs( coords[2] - coords[0] );
      boundingBox.height = std::abs( coords[3]-coords[1]);
      printf("bounding box with vertices (%d,%d) and (%d,%d) was given in command line\n",coords[0],coords[1],coords[2],coords[3]);
      rectangle( image, boundingBox, Scalar( 255, 0, 0 ), 2, 1 );
  }
  else
    boundingBox = selectROI("Tracking API", image);

  cout << boundingBox << endl;

  imshow( "Tracking API", image );

  bool initialized = false;
  int frameCounter = 0;
  int64 timeTotal = 0;

  for ( ;; )
  {
    if( !paused )
    {
      if(initialized){
          cap >> frame;
          if(frame.empty()){
            break;
          }
          frame.copyTo( image );
      }

      if( !initialized )
      {
        //initializes the tracker
        if( !tracker->init( frame, boundingBox ) )
        {
          cout << "***Could not initialize tracker...***\n";
          return -1;
        }
        initialized = true;
      }
      else if( initialized )
      {
        int64 frameTime = getTickCount();
        //updates the tracker
        if( tracker->update( frame, boundingBox ) )
        {
          rectangle( image, boundingBox, Scalar( 255, 0, 0 ), 2, 1 );
        }
        frameTime = getTickCount() - frameTime;
        timeTotal += frameTime;
      }
      imshow( "Tracking API", image );
      frameCounter++;
    }

    char c = (char) waitKey( 2 );
    if( c == 'q' )
      break;
    if( c == 'p' )
      paused = !paused;
  }

  double s = frameCounter / (timeTotal / getTickFrequency());
  printf("FPS: %f\n", s);

  return 0;
}
