// * Name: TrackingManager.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#ifndef TrackingManager_h
#define TrackingManager_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxCv.h"
#include "Blob.h"
#include "Configuration.h"
#include "TrackingHistory.h"
#include "ofxGui.h"
#include "ofxJSON.h"



using namespace cv;
using namespace ofxCv;

class TrackingManager
{
	public:
		//! Setup the tracker
		void setup(Tracking_Configuration _trackingConfig);

		//! Pass the Camera image into the tracker
		void update(Mat processedMat);

		void updateGui();

		//! Draw the Tracker
		void draw();

		void saveGui();

		//! Close the Tracker
		void close();

		ofEvent<int> blobIn;
		ofEvent<int> blobOut;

		ofxPanel guiTracking;

		ofxButton saveTrack;
		ofxFloatSlider minarea;
		ofxFloatSlider maxarea;
		ofxFloatSlider threshold;
		ofxFloatSlider maxdistance;
		ofxFloatSlider persistance;

		ofxJSONElement configTrack;

		int _minarea;
		int _maxarea;
		int _threshold;
		int _maxdistance;
		int _persistance;






	private:
		ofxCv::ContourFinder contourFinder;
		ofxCv::RectTrackerFollower<Blob> tracker;
		ofRectangle centerRect;

		TrackingHistory trackingHistory;
		bool _flipVertically;

		int _oneBlob;
		int _twoBlob;
		int _threeBlob;
		int _historyLength;
		int _camerawidth;
		int _cameraheight;
};

#endif /* TrackingManager_h */
