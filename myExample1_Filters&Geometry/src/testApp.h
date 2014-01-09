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
	
   
    //want to texture a sphere. Here is a call to get an image in
    //and allow for the wrapping of a 3D object by calling GLUquadraticObj.
    ofImage gods;
    GLUquadricObj *quadric;
    
    //bools to keep track of user interaction
    bool texture, sphereShow, boxShow, cylinderShow;
	
    /* stick you maximilian stuff below */
    double sample,outputs[2];
    //a mix to create the stereo out to left and right
    maxiMix mymix;
    //a holder for the sample coming in. 
    maxiSample godsGivenUp;
    //three variables to hold the filters
    double lowVal, midVal, highVal;
    //three more variables to hold the smoothed(constained) filters for visual manipulation.
    double lowValSmoothed, midValSmoothed, highValSmoothed;
    //filters to hold the information about each.
    maxiFilter lowFilt, midFilt1, midFilt2, highFilt;
    //filters to add a lowpass filter to for each of the smoothing function
    maxiFilter lowValSmoothing, midValSmoothing, highValSmoothing;
    
    //set up variables for loop
    double loopStart, loopEnd;
    
    //set up an array of 3D spheres
    ofSpherePrimitive sphere[3];
    //set up a large array of 3D boxes
    ofBoxPrimitive box[30];
    //set up a large array of 3D cylinders. 
    ofCylinderPrimitive cylinder[30];
    
    //add this in the extended version? Creates more of a landscape feel?
    ofCylinderPrimitive cylinder2[30];
   
    //declare some lights
    ofLight light;
    ofLight spot;
    

    
};
