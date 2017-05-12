// * Name: ofApp.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#pragma once

#include "ofMain.h"
#include "ConfigurationManager.h"
#include "CameraManager.h"
#include "TrackingManager.h"
#include "HTTPManager.h"
#include "CsvManager.h"
#include "ofxOsc.h"
#include <ctime>
//#include "ofxGui.h"

#define PORT 9001
#define NUM_MSG_STRINGS 20

#define HOSTLIGHT "packshot02.local"
#define HOSTCAPTEUR "packshot01.local"
//#define HOSTCAPTEUR "192.168.1.70"

//#define HOSTLIGHT "192.168.1.53"

#define PORTLIGHT 9000
#define PORTCAPTEUR 9000


class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);

		ConfigurationManager configManager;
		CameraManager cameraManager;
		TrackingManager trackingManager;
		HTTPManager httpManager;
		CsvManager csvManager;

		void blobIn(int &val);
		void blobOut(int &val);

		bool _logToCsv;
		bool _logToServer;
		int peopleIn;
		int peopleOut;

		int maxPeopleIn;
		int total;

		int peopleDetected;
		time_t lastBlob;
		void confirmDetection(int val);

		ofxOscReceiver	receiver;
		ofxOscSender senderPackshot;
		ofxOscSender senderCaptor;



};
