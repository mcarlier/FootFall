// * Name: TrackingManager.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#include "TrackingManager.h"
//--------------------------------------------------------------
void TrackingManager::setup(Tracking_Configuration _trackingConfig)
{
	cout << "Setting Up Tracking Manager" << endl;
	contourFinder.setMinAreaRadius(_trackingConfig.minarea);
	contourFinder.setMaxAreaRadius(_trackingConfig.maxarea);
	contourFinder.setThreshold(_trackingConfig.threshold);
	tracker.setMaximumDistance(_trackingConfig.maxdistance);
	tracker.setPersistence(_trackingConfig.persistance);

	_oneBlob = _trackingConfig.minsizeone;
	_twoBlob = _trackingConfig.minsizetwo;
	_threeBlob = _trackingConfig.minsizethree;
	_flipVertically = _trackingConfig.flipvertically;
	_historyLength = _trackingConfig.history;
	_camerawidth = _trackingConfig.camerawidth;
	_cameraheight = _trackingConfig.cameraheight;
	_y =_trackingConfig.startPos.y;



	trackingHistory.setup(_oneBlob,_twoBlob,_threeBlob,_trackingConfig.startPos.y);

	centerRect = ofRectangle(0, _trackingConfig.startPos.y-(_trackingConfig.offset/2), _camerawidth, _trackingConfig.offset);

	guiTracking.setup();
	guiTracking.add(saveTrack.setup("save Track"));

	guiTracking.add(minarea.setup("minarea", _trackingConfig.minarea, 0, 400));
	guiTracking.add(maxarea.setup("maxarea", _trackingConfig.maxarea, 0, 400));
	guiTracking.add(threshold.setup("threshold", _trackingConfig.threshold, 0, 400));
	guiTracking.add(maxdistance.setup("maxdistance", _trackingConfig.maxdistance, 0, 400));
	guiTracking.add(persistance.setup("persistance", _trackingConfig.persistance, 0, 10));


	guiTracking.add(changeBlobSize.setup("Update blob size"));
	guiTracking.add(oneBlobGui.setup("oneBlobGui", _trackingConfig.minsizeone, 0, 400));
	guiTracking.add(twoBlobGui.setup("twoBlobGui", _trackingConfig.minsizetwo, 0, 1000));
	guiTracking.add(threeBlobGui.setup("threeBlobGui", _trackingConfig.minsizethree, 0, 1500));


	configTrack.open("config.json");
}
//--------------------------------------------------------------

void TrackingManager::updateGui()
{
	if(_minarea!=minarea){
		_minarea =minarea;
		contourFinder.setMinAreaRadius(_minarea);
	}
	if(_maxarea!=maxarea){
		_maxarea =maxarea;
		contourFinder.setMaxAreaRadius(maxarea);
	}
	if(_threshold!=threshold){
		_threshold =threshold;
		contourFinder.setThreshold(threshold);
	}
	if(_maxdistance!=maxdistance){
		_maxdistance =maxdistance;
		tracker.setMaximumDistance(maxdistance);
	}
	if(_persistance!=persistance){
		_persistance =persistance;
 		tracker.setPersistence(persistance);
	}
	if(saveTrack){
		saveGui();
	}
	if(changeBlobSize){
		_oneBlob=oneBlobGui;
		_twoBlob=twoBlobGui;
		_threeBlob=threeBlobGui;
		TrackingHistory trackingHistorytmp;
		trackingHistorytmp.setup(_oneBlob,_twoBlob,_threeBlob,_y);
		trackingHistory=trackingHistorytmp;
	}
}

void TrackingManager::saveGui(){

	configTrack["Footfall"]["TrackingConfig"]["minarea"]= _minarea;
	configTrack["Footfall"]["TrackingConfig"]["maxarea"]= _maxarea;
	configTrack["Footfall"]["TrackingConfig"]["threshold"]= _threshold;
	configTrack["Footfall"]["TrackingConfig"]["maxdistance"]= _maxdistance;
	configTrack["Footfall"]["TrackingConfig"]["persistance"]= _persistance;
	configTrack["Footfall"]["TrackingConfig"]["minsizeone"]= _oneBlob;
	configTrack["Footfall"]["TrackingConfig"]["minsizetwo"]= _twoBlob;
	configTrack["Footfall"]["TrackingConfig"]["minsizethree"]= _threeBlob;

	configTrack.save("config.json", true);
}


void TrackingManager::update(Mat processedMat)
{
	updateGui();
	if(!processedMat.empty())
	{
		if (ofGetFrameNum() > _historyLength)
		{
			contourFinder.findContours(processedMat);
			tracker.track(contourFinder.getBoundingRects());
		}
	}

	vector<Blob> &blobs = tracker.getFollowers();
	for(int i = 0; i < blobs.size(); i++)
	{
		if (centerRect.inside(blobs[i].getCurrentPosition().x, blobs[i].getCurrentPosition().y) && !blobs[i]._evaluating)
		{
			if (_flipVertically)
			{
				if (blobs[i].getCurrentPosition().y > blobs[i].getOriginPosition().y)
				{
					int noOfBlobs = 0;
					int blobWidth = blobs[i].getWidth();
					if (blobWidth > _threeBlob)
					{
						noOfBlobs = 3;
					}

					if ((blobWidth > _twoBlob) && (blobWidth < _threeBlob))
					{
						noOfBlobs = 2;
					}

					if ((blobWidth > _oneBlob) && (blobWidth < _twoBlob))
					{
						noOfBlobs = 1;
					}



					trackingHistory.addNewData(blobs[i].getWidth(), true);
					ofNotifyEvent(blobIn, noOfBlobs, this);
					blobs[i].kill();
				}
				else if (blobs[i].getCurrentPosition().y < blobs[i].getOriginPosition().y)
				{
					int noOfBlobs = 0;
					int blobWidth = blobs[i].getWidth();
					if (blobWidth > _threeBlob)
					{
						noOfBlobs = -3;
					}

					if ((blobWidth > _twoBlob) && (blobWidth < _threeBlob))
					{
						noOfBlobs = -2;
					}

					if ((blobWidth > _oneBlob) && (blobWidth < _twoBlob))
					{
						noOfBlobs = -1;
					}

					trackingHistory.addNewData(blobs[i].getWidth(), false);
					ofNotifyEvent(blobOut, noOfBlobs, this);
					blobs[i].kill();
				}
			}
			else {
				if (blobs[i].getCurrentPosition().y < blobs[i].getOriginPosition().y)
				{
					int noOfBlobs = 0;
					int blobWidth = blobs[i].getWidth();
					if (blobWidth > _threeBlob)
					{
						noOfBlobs = 3;
					}

					if ((blobWidth > _twoBlob) && (blobWidth < _threeBlob))
					{
						noOfBlobs = 2;
					}

					if ((blobWidth > _oneBlob) && (blobWidth < _twoBlob))
					{
						noOfBlobs = 1;
					}

					trackingHistory.addNewData(blobs[i].getWidth(), true);
					ofNotifyEvent(blobIn, noOfBlobs, this);
					blobs[i].kill();
				}
				else if (blobs[i].getCurrentPosition().y > blobs[i].getOriginPosition().y)
				{
					int noOfBlobs = 0;
					int blobWidth = blobs[i].getWidth();
					if (blobWidth > _threeBlob)
					{
						noOfBlobs = -3;
					}

					if ((blobWidth > _twoBlob) && (blobWidth < _threeBlob))
					{
						noOfBlobs = -2;
					}

					if ((blobWidth > _oneBlob) && (blobWidth < _twoBlob))
					{
						noOfBlobs = -1;
					}

					trackingHistory.addNewData(blobs[i].getWidth(), false);
					ofNotifyEvent(blobOut, noOfBlobs, this);
					blobs[i].kill();
				}
			}

			blobs[i]._evaluating = true;
		}
		else if(blobs[i]._evaluating)
		{
			blobs[i]._evaluating = false;
		}
	}
}


//--------------------------------------------------------------
void TrackingManager::draw()
{
	ofSetLineWidth(1);
	ofFill();
	vector<Blob> &followers = tracker.getFollowers();
	for(int i = 0; i < followers.size(); i++)
	{
		followers[i].draw();
	}
	ofSetColor(ofColor::red);
	contourFinder.draw();

	ofNoFill();
	ofSetColor(255, 255, 255);
	ofDrawRectangle(centerRect);

	trackingHistory.draw(_camerawidth,_cameraheight);
}
