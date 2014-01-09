#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"


class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
    void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
	
    ofVideoPlayer 		myMovie;
    
    unsigned char * pixels;
	
    /* stick you maximilian stuff below */
	
    double sample,outputs[2];
    maxiMix mymix;
    maxiSample beat;
    double lowVal, midVal, highVal;
    double lowValSmoothed, midValSmoothed, highValSmoothed;
    maxiFilter lowFilt, midFilt1, midFilt2, highFilt;
    maxiFilter lowValSmoothing, midValSmoothing, highValSmoothing;
    
    ofSpherePrimitive sphere[3];
    ofBoxPrimitive box[30];
    ofCylinderPrimitive cylinder[30];
    
    //add this in the extended version? Creates more of a landscape feel?
    ofCylinderPrimitive cylinder2[30];
   
    
    ofLight light;
    ofLight spot;
    ofCamera camera;//setup for Camera.
    ofVec3f axis[3];//
    
    double loopStart, loopEnd;
    
};
