// * Name: CameraManager.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#ifndef CameraManager_h
#define CameraManager_h

//#define USE_VIDEO //ffmpeg -f avfoundation -video_size 800x600 -framerate 24 -i "0" out.mpg
#define USE_WEBCAM
//#define USE_PI_CAM

#include <stdio.h>
#include "ofMain.h"
#include "Configuration.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxJSON.h"


#ifdef USE_PI_CAM
	#include "ofxCvPiCam.h"
#endif

using namespace cv;
using namespace ofxCv;

class CameraManager
{
	public:
		//! Setup
		void setup(Camera_Configuration _cameraConfig);

		//! Update
		void update();

		//! Draw
		void draw();

		//! Get Image
		Mat getImage();

		void updateGui();
		void saveCameraSetting();

		ofxPanel guiCamera;
		ofxButton saveGui;
		ofxFloatSlider thresholdGui;
		ofxFloatSlider dilateGui;
		ofxFloatSlider erodeGui;
		ofxFloatSlider blurGui;
		ofxFloatSlider shadowPixelRatioGui;

		ofxJSONElement configCam;


	private:

#ifdef USE_VIDEO
		ofVideoPlayer videoPlayer;
#endif

#ifdef USE_WEBCAM
		ofVideoGrabber videoGrabber;
#endif

#ifdef USE_PI_CAM
		ofxCvPiCam piCamera;
#endif

		Mat videoMatrix;
		Mat processedMog;
		Mat mask;
		Mat combinedMask;
		Mat background;
		Mat unprocessed_MOG;

		BackgroundSubtractorMOG2 *pMOG2;

		vector<cv::Point> _maskPts;
		bool _useMask;
		bool _showShadows;
		int _threshold;
		int _dilateAmount;
		int _erodeAmount;
		int _blur;
};

#endif /* CameraManager_h */
