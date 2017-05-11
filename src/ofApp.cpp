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
	senderPackshot.setup( HOSTLIGHT, PORTLIGHT );
	senderCaptor.setup(HOSTCAPTEUR,PORTCAPTEUR);

	maxPeopleIn = 10;
	total = 0;
	peopleDetected = 0;


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
		std::cout << m.getAddress() << '\n';
		// if(m.getAddress()=="/peopleInside"){
		// 	peopleIn = stoi(m.getArgAsString(0));
		// 	peopleOut = 0;
		// 	total = peopleIn;
		// 	std::cout << "update from Light Remote peopleInside = "<< total << '\n';
		// }
		// if(m.getAddress()=="/getPeopleInside"){
		// 	std::cout << "new connection on Light Remote send peopleInside = "<< total << '\n';
		// 	ofxOscMessage m;
		// 	m.setAddress( "/peopleInside" );
		// 	m.addIntArg(total);
		// 	senderPackshot.sendMessage( m );
		// }
		if(m.getAddress()=="/answerDetection"){
			std::cout << "answer"<< '\n';
			bool answer = m.getArgAsBool(0);
			if(answer){
				std::cout << "new connection on Light Remote send peopleInside = "<< peopleDetected << '\n';
				ofxOscMessage m;
				m.setAddress( "/updatePeopleInside" );
				m.addIntArg(peopleDetected);
				senderPackshot.sendMessage( m );
			}
			peopleDetected = 0;
		}
	}
	if (peopleOut>peopleIn){
		peopleIn = 0;
		peopleOut = 0;
	}
	// if((peopleIn-peopleOut)!=total){
	// 	total = peopleIn-peopleOut;
	// 	ofxOscMessage m;
	// 	std::cout << "update from Footfall peopleInside =  : "<<total << '\n';
	// 	m.setAddress( "/peopleInside" );
	// 	m.addIntArg(total);
	// 	senderPackshot.sendMessage( m );
	// }
}
void ofApp::confirmDetection(int val)
{
	peopleDetected +=val;
	ofxOscMessage m;
	m.setAddress( "/isSomeoneDetected" );
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
	ss << "Footfall" << endl;
	ss << "People In: " << peopleIn;
	ss << " People Out: " << peopleOut;
	ss << " Tally: " << total;
	ss << " maxPeopleIn: " << maxPeopleIn;
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
	peopleIn += val;
	confirmDetection(val);
	cout << val << " Blob(s) Came In" << endl;
}
//--------------------------------------------------------------
void ofApp::blobOut(int &val)
{
	peopleOut += abs(val);
	confirmDetection(val);
	cout << val << " Blob(s) Went Out" << endl;
}
