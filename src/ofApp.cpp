// * Name: ofApp.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(25);

	cout << "-------------------- Footfall --------------------" << endl;

	configManager.loadConfiguration("config.json");

	_logToCsv = configManager.getConfiguration().useCsvLogging;
	_logToServer = configManager.getConfiguration().useHttp;

	cameraManager.setup(configManager.getConfiguration().cameraConfig);
	trackingManager.setup(configManager.getConfiguration().trackingConfig);

	if (_logToServer) httpManager.setup(configManager.getConfiguration().httpConfig);
	if (_logToCsv) csvManager.setup("csvlogs");

	ofAddListener(trackingManager.blobIn, this, &ofApp::blobIn);
	ofAddListener(trackingManager.blobOut, this, &ofApp::blobOut);

	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );
	senderPackshot.setup(HOSTLIGHT,PORTLIGHT);
	senderCaptor.setup(HOSTCAPTEUR,PORTCAPTEUR);

	maxPeopleIn = 10;
	total = 0;
	peopleDetected = 0;
	lastBlob = 0;


}
//--------------------------------------------------------------
void ofApp::exit()
{
	if (_logToServer) httpManager.close();
	if (_logToCsv) csvManager.close();

	ofRemoveListener(trackingManager.blobIn, this, &ofApp::blobIn);
	ofRemoveListener(trackingManager.blobOut, this, &ofApp::blobOut);
}
//--------------------------------------------------------------
void ofApp::update()
{
	cameraManager.update();
	trackingManager.update(cameraManager.getImage());

		while( receiver.hasWaitingMessages() )
	{
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		if(m.getAddress()=="/answer"){
			bool answer = m.getArgAsBool(0);
			if(answer){
				std::cout << "Someone move : "<< peopleDetected << '\n';
				ofxOscMessage m;
				m.setAddress( "/updatePeopleInside" );
				m.addIntArg(peopleDetected);
				senderPackshot.sendMessage(m);
			}
			else{
				std::cout << "noone"<< '\n';
			}
			peopleDetected = 0;
		}
	}
}
void ofApp::confirmDetection(int val)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "demand confirmation" <<val<< '\n';
	peopleDetected +=val;
	ofxOscMessage m;
	m.setAddress( "/someone" );
	senderCaptor.sendMessage( m );
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofPushMatrix();
	ofScale(0.5,0.5);
	cameraManager.draw();
	trackingManager.draw();
	ofPopMatrix();

	stringstream ss;

	ss << " FPS: " << ofGetFrameRate() << endl;
	ofDrawBitmapStringHighlight(ss.str(),7,ofGetHeight()-20);

	cameraManager.guiCamera.draw();
	trackingManager.guiTracking.draw();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}
//--------------------------------------------------------------
void ofApp::blobIn(int &val)
{
	time_t s =  time(0)- lastBlob;
	std::cout << s << '\n';
	if(s>1){
		confirmDetection(val);
	}
	lastBlob  = time(0);


}
//--------------------------------------------------------------
void ofApp::blobOut(int &val)
{
	time_t s =  time(0)- lastBlob;
	std::cout << s << '\n';
	if(s>1){
		confirmDetection(val);
	}
	lastBlob  = time(0);

}
