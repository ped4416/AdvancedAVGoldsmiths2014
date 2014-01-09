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
    
    //void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
    //void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
	
    
    void audioOut (float * input, int bufferSize, int nChannels); // output method 
    void audioIn (float * input, int bufferSize, int nChannels); // input method
    
    //setup for videoGrapper
    ofVideoGrabber 		vidGrabber;
    unsigned char * 	videoInverted;
    ofTexture			videoTexture;
    int 				camWidth;
    int 				camHeight;
    unsigned char * pixels;

    
   
    int initialBufferSize; /* buffer size */
    int sampleRate;
    
    ofSoundStream soundStream;
    
    //Three sine waves to additively estimate the sound received from the microphone:
    double outputs[2];
    
    ofxMaxiOsc sine, sine1, sine2;
    ofxMaxiOsc saw, saw1, saw2;
    
    int bin_number, bin_number1, bin_number2;
    float largests[3];
    
    //The ofxMaxiFFT will do the actual estimation of what frequency ranges the sound coming in from the microphone is loudest in:
    
    ofxMaxiFFT fft; ofxMaxiMix mix;
    float *inputL, *inputR;
    float estimatedPitch[3];
    
    bool accurateOut, octaveLower, perfect5th;
    
    int fade;

};
   
    
    
    
