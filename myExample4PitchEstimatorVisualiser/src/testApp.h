/*The main code for the pitch analysis is from Joshua Nobel's book "Interactivity 2nd Edition" and the video grabbing code is based on an example in the main openframewords Example folder as well as an example from week 8 in Advanced Audiovisual Processing.
 
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
    
    //variables to store the initial buffer size
    int initialBufferSize; /* buffer size */
    int sampleRate;
    
    //creating an instance of soundstream.
    ofSoundStream soundStream;
    
    double outputs[2];
    
    //Three sine waves to additively estimate the sound received from the microphone:
    ofxMaxiOsc sine, sine1, sine2;
    
    //variables to keep track of the three bins
    int bin_number, bin_number1, bin_number2;
    float largests[3];
    
    //The ofxMaxiFFT will do the estimation of what frequency ranges are loudest
    ofxMaxiFFT fft;
    ofxMaxiMix mix;
    
    //variables to take audio in.
    float *lAudioIn, *rAudioIn;
    //an array float to store estimated pitches in
    float estimatedPitch[3];
    //bools to alter teh output frequency
    bool accurateOut, octaveLower, perfect5th;
  

};
   
    
    
    
