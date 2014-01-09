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
	
    double sample, outputs[2];
    maxiMix mymix;
    maxiSample beat;
    
    double sampleIn;
    
    maxiOsc sineBank[100];//create 100 sine tones to add together.
    
    //AM setup
    maxiOsc mySine,myOtherSine,myLastSine,myPhasor;
    
    maxiEnv envelope;//this is going to be an envelope
    
    double envelopeOut;
    int sampleTrigger;
    
    
    //These are the control values for the envelope
    //These are the synthesiser bits
    maxiOsc VCO1,VCO2,LFO1,LFO2;
    maxiFilter VCF;
    maxiEnvelope ADSR;
    
    //These are the control values for the envelope
    double adsrEnv[8]={1,5,0.125,250,0.125,125,0,500};
    
    //This is a bunch of control signals so that we can hear something
    
    maxiOsc timer;//this is the metronome
    int currentCount,lastCount;//these values are used to check if we have a new beat this sample
    
    //and these are some variables we can use to pass stuff around
    
    double VCO1out,VCO2out,LFO1out,LFO2out,VCFout,ADSRout;
    
    //declare a way to turn synth on and off. 
    bool synthSilence, synthPlay;
    
    //bools to keep track of active single fm modulators
    bool singleMod, ratio1to1, ratio1to4, ratio1to2, ratio3to2, ratio2to1, ratio3to1, ratio7to2, ratio6to1;
    //bools to keep track of the active double modulators
    bool twoModulators1Ovtave, twoModualtorsSame, parallelFM, cascadeFM, twoModulatorsCrazy;
        
};
