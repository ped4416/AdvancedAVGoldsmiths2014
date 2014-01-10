//
//  FFT.h
//  myMainProject
//
//  Created by Pedro Kirk on 09/01/2014.
//
//

#ifndef __myMainProject__FFT__
#define __myMainProject__FFT__

#include <iostream>
#include "ofMain.h"
#include "ofxMaxim.h"

class FFT{ //this is the base class
    
public: // place public functions or variables declarations here
    
    //what state is the app in??
    enum state_t {
        state_read_toMe,
        state_music_play,
        state_music_off,
        state_read_toMyself,
        
    };
    state_t state;
    
    FFT();//constructor
    
    // methods, equivalent to specific functions of your class objects
    void setup();
    void update();  // update method, used to refresh your objects properties
    void draw();    // draw method, this where you'll do the object's drawing
    void keyPressed(int key);
    
    float playSounds(float x);//left channel
    float playSounds1(float y);//right channel
    
    void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
	void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
    //void playSounds(float x);
    void showPage(int x, int y);//a method to show the pages of the book
    
    bool showAccelPage(int x);
    
    // variables
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

private: // place private functions or variables declarations here
    
};// don't forget the semicolon!!
#endif