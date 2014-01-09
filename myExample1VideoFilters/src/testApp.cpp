//procedural geometry

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
     //ofSetGlobalAmbientColor(ofColor(0, 255, 255));
    camera.move(50, 0, -100);
	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0);
	ofSetVerticalSync(true);
    
    //initialise three variables to store three different filters.
    lowVal = midVal = highVal = 0;
	
	/* You can put anything you would normally put in maximilian's 'setup' method in here. */
	beat.load(ofToDataPath("Gods.wav"));
	
	//myMovie.loadMovie("bball.mov");
	myMovie.play();

    //setup loop for mouse interaction starting at 0 and ending at 1
    loopStart = 0;
    loopEnd = 1;
    
	/* This is stuff you always need.*/
    maxiSettings::setup(44100, 2, 512);
	ofSoundStreamSetup(maxiSettings::channels, maxiSettings::channels, this, maxiSettings::sampleRate, maxiSettings::bufferSize, 4);/* Call this to start the sound system*/

}

//--------------------------------------------------------------
void testApp::update(){
	//myMovie.setSpeed(fabs(sample)*10);
	//this updates the movie...
	myMovie.update();

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0); // a white background
    light.enable();//OPENGL 3D world a light this gives a sense of depth.
    
    //light.setSpecularColor(ofColor(0, 255, 255, 200));
    
    //spot.setSpotlight();
    //spot.setSpotConcentration(5);
    //spot.setSpotlightCutOff(10);
    
    light.setPosition(ofGetWidth()/2, ofGetHeight()/2, 500);
    light.setSpecularColor(ofColor(0, 255, 0));
    
    //camera.rotateAround(1, ofVec3f(1,0,0), ofVec3f(1,0,0));
    //camera.begin();
  
    //try manipulating lights.. move them.
    
    //need to smooth out the frequency to minimise any big jumps in the filters. 
    lowValSmoothed = lowValSmoothing.lopass(lowVal, loopEnd);//constrain the frequency.
    midValSmoothed = midValSmoothing.lopass(midVal, loopEnd);
    highValSmoothed = highValSmoothing.lopass(highVal, loopEnd);
    
    
    //orbit(float longitude, float latitude, float radius, const ofVec3f& centerPoint = ofVec3f(0, 0, 0));
    light.rotate(360, ofGetWidth()/2, ofGetHeight()/2, 100);
    
   
//    //set boolean to true for spheres?
//    //rotate spheres relative to the bass, mid and treble in the sound
//    sphere[0].rotate(lowValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
//    sphere[1].rotate(midValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
//    sphere[2].rotate(highValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
//    
//    for(int i=0; i < 3; i++) {
//        float colMod = i / 3.0+lowValSmoothed;
//        ofSetColor(colMod * 255.0, (1-colMod) * 255.0, midValSmoothed, 100);//4th term is alpha value
//        
//        //spheres move in reaction to the bass mid and treble of the sound.
//        //sphere[i].setRadius(100 + (i * (50 + (50 * midValSmoothed))));
//        sphere[i].setRadius(100 + (i * (50 + (50 * midValSmoothed))));
//        sphere[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
//        sphere[i].drawWireframe();
//        sphere[i].draw();
//        
//    }
    //set boolean to true for boxes?
    //rotate spheres relative to mid only
    for (int i=0; i<30; i++) {
        box[1].rotate(midValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
    
        float colMod = i / 3.0+lowValSmoothed;
        ofSetColor(colMod * 255.0, (1-colMod) * 255.0, midValSmoothed, 100);//4th term is alpha value
        
        //spheres move in reaction to the bass mid and treble of the sound.
        //sphere[i].setRadius(100 + (i * (50 + (50 * midValSmoothed))));
        box[i].setDepth(100 + (i * (50 + (50 * midValSmoothed))));
        box[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
        box[i].drawWireframe();
        //box[i].draw();
        
    }
    
    //set boolean to true for another Cylinder set?
    //rotate spheres relative to mid only
    for (int i=0; i<30; i++) {
        cylinder2[1].rotate(midValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
        
        float colMod = i / 3.0+lowValSmoothed;
        ofSetColor(colMod * 255.0, (1-colMod) * 255.0, midValSmoothed, 100);//4th term is alpha value
        
        //spheres move in reaction to the bass mid and treble of the sound.
        //sphere[i].setRadius(100 + (i * (50 + (50 * midValSmoothed))));
        cylinder2[i].setRadius(100 + (i * (50 + (50 * midValSmoothed))));
        cylinder2[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
        cylinder2[i].drawWireframe();
        //box[i].draw();
        
    }
    
    //set boolean to true for cylinders?
    //rotate spheres relative to the bass only
    for (int i=0; i<30; i++) {
        cylinder[i].rotate(lowValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
        
        float colMod = i / 3.0+lowValSmoothed;
        ofSetColor(colMod * 255.0/i, (1-colMod) * 255.0/i, midValSmoothed, 50+i);//4th term is alpha value
        
        //spheres move in reaction to the bass mid and treble of the sound.
        cylinder[i].setRadius(2 + (i * (i/20+ (i * midValSmoothed))));
        cylinder[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
       // cylinder[i].drawWireframe();
        cylinder[i].draw();
        
    }
    camera.end();
    cout<< loopStart << " loopStart Value is .. , " << endl;
    cout<< loopEnd << " loopEnd Value is .. , " << endl;
}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	
	for (int i = 0; i < bufferSize; i++){
        
        //create a sample between -1 and 0 but with two variables creating a region
        //loopstart and loopend these are controled by the mouse.
		sample=beat.playLoop(loopStart, loopEnd);
        
        //record three different sample values using filters
        //1 resonant lowPass filter. Anything below 200Hz is kept
        lowVal = lowFilt.lores(sample, 50, 2);
        
        //2 making a bandpass from lowpass and a highpass by chopping off the bottom and top sequentially
        midVal = midFilt2.hires(midFilt1.lores(sample, 4000, 3), 200, 3);
        
        //3 resonant highPass filter gives the high frequecies anything above 4000Khz
        highVal = highFilt.hires(sample, 8000, 2);//get the high freq.
        
        //none of the filters are being played they are only used to take the frequency
        //information to control the graphics.
		mymix.stereo(sample, outputs, 0.5);
		
		
		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
		output[i*nChannels + 1] = outputs[1];
        
	}
	
}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){
	
	
	/* You can just grab this input and stick it in a double, then use it above to create output*/
	
	for (int i = 0; i < bufferSize; i++){
		
		/* you can also grab the data out of the arrays*/
//		sampleIn = input[i];
		
	}
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    loopStart = x / (double)ofGetWidth();//get mouse ponts in
    loopEnd = y / (double) ofGetHeight();
    loopEnd = max(loopStart + 0.05, loopEnd);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}