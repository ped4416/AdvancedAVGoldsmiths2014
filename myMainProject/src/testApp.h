/*Basic functionality for the project taken from weeks 8, 9 and 10 of the Advanced Audiovisual Processing course at Goldsmiths College 2013. http://doc.gold.ac.uk/CreativeComputing/creativecomputation/?page_id=1200Procedural
 The main code for the pitch analysis is from Joshua Nobel's book "Interactivity 2nd Edition"
 The code to iterate through pixels is similar to openFramework examples and some that were shown in the aforementioned Audiovisual Course.
 
 The song "Gods Given Up On Him" is an origional owned by "Small Engine Repair" c 2014.
 The texture map image "Gods" is also owned and copyrighted to "Small Engine Repair" c 2014.
 The track "nightQuiet" was downloaded from freeSound.net and has no copyright attached.
 Feel free to use any of the code as required. Pedro Kirk Jan 2014
 Find me at www.pedrokirk.co.uk
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

	void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
	void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
	
	float 	* lAudioOut; /* outputs */
	float   * rAudioOut;
	
	float * lAudioIn; /* inputs */
	float * rAudioIn;
	
	int		initialBufferSize; /* buffer size */
	int		sampleRate;
    
    ofTrueTypeFont newFont;
    ofTrueTypeFont smallFont;
    
    bool showInstructions;
	
	//--------------------------------------------------------------
    //AUDIO PLAYBACK OF BAND TRACK USING FILTERS WITH GEOMETRY SIMILAR TO EXAMPLE 1 BUT EXPANDED ON
    
    //need to texture a sphere. Here is a call to get an image in
    //and allow for the wrapping of a 3D object by calling GLUquadraticObj.
    ofImage gods;
    GLUquadricObj *quadric;
    
    //bools to keep track of user interaction
    bool texture, sphereShow, boxShow, cylinderShow;
	
    /* stick you maximilian stuff below */
    double sample,outputs[2];
    //a mix to create the stereo out to left and right
    maxiMix myFiltermix;
    //a holder for the sample coming in.
    maxiSample godsGivenUp;
    //three variables to hold the filters
    double lowVal, midValLow, midValHigh, highVal;//extra functionaloty from Example1
    //three more variables to hold the smoothed(constained) filters for visual manipulation.
    double lowValSmoothed, midValLowSmoothed, midValHighSmoothed, highValSmoothed;
    //filters to hold the information about each.
    maxiFilter lowFilt, midFilt1, midFilt2, highFilt;
    //filters to add a lowpass filter to for each of the smoothing function
    maxiFilter lowValSmoothing, midValLowSmoothing, midValHighSmoothing, highValSmoothing;
    
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
    //spot light and all following is additional to the Example1 code.
    ofLight spot;

    
    //--------------------------------------------------------------
    //FFT USING AUDIO FILTERS TO MANIPULATE THE BAND TRACK SIMILAR TO EXAMPLE 2 BUT EXPANDED ON
	/* stick your maximilian stuff below */
	
	double wave, wave2 , wave3, wave4;
    double ifftVal, ifftVal2, ifftVal3 ,ifftVal4, ifftVal5;
	maxiMix mymix, mymix2, mymix3, mymix4, mymix5;
	maxiSample sample1, sample2, sample3;
	maxiOsc osc;
    ofxMaxiFFTOctaveAnalyzer oct;
	
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
    
    //This is an extension of Example 2 allowing some MFCC action!
    //set up for MFCC
    maxiMFCC mfcc;
	double *mfccs;
    int nAverages;
    
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
    bool timbralMFCC;

    //bools to set main states between audio
    bool filteredSound;
    bool playMagnitudes;
    bool videoGrabberShow;
    
    //--------------------------------------------------------------
    //FFT PITCH ESTIMATOR TO CREATE A FEEDBACK OF BAND AUDIO AND INPUT FROM USER SIMILAR TO EXAMPLE 4 BUT EXPANDED ON
    //setup for videoGrapper
    ofVideoGrabber 		vidGrabber;
    unsigned char * 	videoInverted;
    ofTexture			videoTexture;
    int 				camWidth;
    int 				camHeight;
    unsigned char * pixels;
    
    //Three sine waves to additively estimate the sound received from the microphone:
    ofxMaxiOsc sine, sine1, sine2;
    ofxMaxiOsc saw, saw1, saw2;
    
    //variables to keep track of the three bins
    int bin_number, bin_number1, bin_number2;
    float largests[3];
    
    //setup for length of fft
	int fftSize2;
    //declare variables to hold the window and hop sizes
    int myWindowSize2, myHopSize2;
    
    //The ofxMaxiFFT will do the estimation of what frequency ranges are loudest
    ofxMaxiFFT fft;
    ofxMaxiMix mix;

    //an array float to store estimated pitches in
    float estimatedPitch[3];
    //bools to alter the output frequency
    bool accurateOut, octaveLower, perfect5th;
    //
    int fade;
};
