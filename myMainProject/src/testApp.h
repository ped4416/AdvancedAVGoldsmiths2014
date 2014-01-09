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
	
	float 	* lAudioOut; /* outputs */
	float   * rAudioOut;
	
	float * lAudioIn; /* inputs */
	float * rAudioIn;
	
	int		initialBufferSize; /* buffer size */
	int		sampleRate;
	
	
	/* stick your maximilian stuff below */
	
	double wave, wave2 , wave3, outputs[2];
    double ifftVal, ifftVal2, ifftVal3 ,ifftVal4;
	maxiMix mymix, mymix2, mymix3, mymix4;
	maxiSample sample1, sample2, sample3;
	maxiOsc osc;
	
	float *ifftOutput;
	int ifftSize;
	
    //declare holders for No1 fft and ifft
    ofxMaxiFFT mfft;
	ofxMaxiIFFT ifft;
    //declare holders for No2 fft and ifft
    ofxMaxiFFT mfft2;
    ofxMaxiIFFT ifft2;
    //declare holders for No3 fft and ifft
    ofxMaxiFFT mfft3;
    ofxMaxiIFFT ifft3;
    //declare holders for No4 fft and ifft
    ofxMaxiFFT mfft4;
    ofxMaxiIFFT ifft4;
    
    //setup for length of fft
	int fftSize;
    //setup variable for the bins and datasize
	int bins, dataSize;
    
    //declare variables to hold the window and hop sizes
    int myWindowSize, myHopSize;
    
    //bools to trigger between sound examples
    bool monoToneVocoder;//magnitude data only partially getting through.
    bool industrialMixofGod;
    bool godsGivenUpNoNight;
    bool godsOrdinary;

    
	
};
