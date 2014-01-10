/* The basic formula for the FM synthesis was taken from the Maximilian example folder and also the envelope function created has used parts of an example from the same library.
 */

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

    ofTrueTypeFont newFont;
    ofTrueTypeFont smallFont;
    
    void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
    void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
	
    //declare stero ouput left and right
    double sample, outputs[2];
    //a mix to create the stereo out to left and right
    maxiMix mymix;
    //incase we need a sample in
    double sampleIn;
    
    //decalre variables to store sinewaves for FM synthesis
    maxiOsc mySine,myOtherSine,myLastSine,myPhasor;
    
    maxiEnv envelope;//this is going to be an envelope

    //trigger the envelope to start and restart
    int sampleTrigger;
    
    //These are the control values for the envelope
    maxiFilter VCF;
    maxiEnvelope ADSR;
    
    //These are the control values for the envelope
    double adsrEnv[8]={1,5,0.125,250,0.125,125,0,500};
    
    //timer to set note duration
    maxiOsc timer;
    int currentCount,lastCount;//these values are used to check if we have a new beat this sample
    
    double ADSRout;
    
    //declare a way to turn synth on and off. 
    bool synthSilence, synthPlay;
    
    //bools to keep track of active single fm modulators
    bool singleMod, ratio1to1, ratio1to4, ratio1to2, ratio3to2, ratio2to1, ratio3to1, ratio7to2, ratio6to1;
    //bools to keep track of the active double modulators
    bool twoModulators1Ovtave, twoModualtorsSame, parallelFM, cascadeFM, twoModulatorsCrazy;
        
};
