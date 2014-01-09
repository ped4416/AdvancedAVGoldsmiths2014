/*The main code for the pitch analysis is from Joshua Nobel's book "Interactivity 2nd Edition" and the video grabbing code is based on an example in the main openframewords Example folder as well as an example from week 8 in Advanced Audiovisual Processing.
 
 */


#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // some standard setup stuff
    ofEnableAlphaBlending();
    ofSetupScreen();
    ofBackground(0);
    ofSetVerticalSync(true);
    
    //set a fade value to alter alpha of video and objects.
    fade = 200;
    
    //setup the videoGrabber size
    camWidth = ofGetWidth();
	camHeight = ofGetHeight();
	
    //we can now get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
	}
    //set up the video grabber
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
    
	vidGrabber.initGrabber(camWidth,camHeight);
	
    //set up to invert the pixel data through a new unsigned char.
	videoInverted = new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_RGB);
	ofSetVerticalSync(true);
    
    //set up bools for playback
    accurateOut = true;//start with this true so it mimicks the real input pitch at correct octave.
    octaveLower = false;
    perfect5th = false;
    
    //setup the sampleRate to a standard CD 44.1Khz
    sampleRate = 44100;
    //the buffer is set up and must be filled with sound
    initialBufferSize = 512;
    
    //set input arrays to store the incoming sound waves set to be 512 as seen above.
    lAudioIn = new float[initialBufferSize];
    rAudioIn = new float[initialBufferSize];
    
    //set up the FFT with a size, window size and hop size.
    //it is usually best to have the window size be the same as the fft size and the hop size should be around 1/4 of the fft size.
    //the fft size should also be a power of 2.
    fft.setup(16384, 1024, 512);//16384 was set by Joshua Nobel
    soundStream.setup(this, 2, 2, 44100, initialBufferSize, 4); estimatedPitch[0] = estimatedPitch[1] = estimatedPitch[2] = 0.f;
    
}

//--------------------------------------------------------------
void testApp::update(){
    //advance video if needed.
	vidGrabber.update();

	//if there is a new video frame get pixels
    if (vidGrabber.isFrameNew()){
        //declare a variable to store the pixels in and get them
		int totalPixels = camWidth*camHeight*3;
		unsigned char * pixels = vidGrabber.getPixels();
        //loop through all the pixels and invert them by reversing them.
		for (int i = 0; i < totalPixels; i++){
			videoInverted[i] = 255 - pixels[i];
		}
        //load the inverted pixels into a texture to be drawn to screen
		videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
	}

    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    
    //declare some local variables to keep monitoring the estimated pitches
    float estimate1 = estimatedPitch[0]/2;
    float estimate2 = estimatedPitch[1]/2;
    float estimate3 = estimatedPitch[2]/2;
    
    //constrain the estimated pitches to keep roughtly within the screen
    if (estimate1 > ofGetWidth()){
        estimate1 = ofGetWidth();
    }
    if (estimate1 > ofGetHeight()){
        estimate1 = ofGetHeight();
    }
    if (estimate2 > ofGetWidth()){
        estimate2 = ofGetWidth();
    }
    if (estimate2 > ofGetHeight()){
        estimate2 = ofGetHeight();
    }

    if (estimate3 > ofGetWidth()){
        estimate3 = ofGetWidth();
    }
    if (estimate3 > ofGetHeight()){
        estimate3 = ofGetHeight();
    }
    
    //to fade pixel data in videoGrabber and alter colours
    float fadeEstimate;
    //constrain the fade values
    for (int i = 0; i <3; i++){
        fadeEstimate = estimatedPitch[i]*0.5;
        if (fadeEstimate > 200) {
            fadeEstimate = 200;
        }
     
        ofSetColor(255, 255, 255,fadeEstimate);
    }
    
  
    //float a variable to keep track of pixel location
    float pixelSpread = ofGetWidth() / 320.0;
    //declare a
    unsigned char * pixels2 = vidGrabber.getPixels();
    int nChannels = vidGrabber.getPixelsRef().getNumChannels();
    
    videoTexture.draw(0,0,camWidth+estimate1,camHeight+estimate2);

    //The following code block takes pixels from the input video and draws circles in their place
    //with the colour and radius altered by the pitch estimations in the for loop above.
    //each frame the pixels from the movie image are stored in memory uncompressed
    pixels = vidGrabber.getPixels();
    int pixelHeight = 10;//distance between pixels in height
    int pixelWidth = 10;//distance between pixels in width
    
    //loop through pixel data in current movie image(frame) and get the colour of every 10th pixel
    for (int i = 4; i < 320; i+=pixelHeight){
        for (int j = 4; j < 240; j+=pixelWidth){
            
            ofSetColor(fadeEstimate, fadeEstimate*2, fadeEstimate, fadeEstimate*6);
            
            //pixel data is blocks of three values giving r,g,b for each
            //scale a bit to suit this
            unsigned char r = pixels[(j * 320 + i)*30];
            
            //cast pixel information to a float for use in circle radius equation
            float val = ((float)r / 255.0f);
            //draw circles instead of the selected pixels.
            ofCircle(i * pixelSpread, j * pixelSpread, val * fadeEstimate/20);
        }
    }
    
    //The following maps all input video to grey circles that map the shape of colour varience
    // let's move through the "RGB(A)" char array
    // again we are using teh estimated pitches in fadeEstimate to adjust circle size.
    for (int i = 4; i < ofGetWidth(); i+=8){
        for (int j = 4; j < ofGetHeight(); j+=8){
            //set colours to change with estimated pitches in very low red/ black spectrum
            ofSetColor(20, 0, 0, fadeEstimate*6);
            //pixel data is blocks of three values giving r,g,b for each
            //scale a bit to suit this
            unsigned char r = pixels2[(j * ofGetWidth() + i)*nChannels];
            //cast pixel information to a float for use in circle radius equation
            float val = 1 - ((float)r / ofGetHeight());
            //draw circles instead of the selected pixels.
            ofCircle( i+pixelSpread, j+pixelSpread, val * fadeEstimate/30);
        }
    }
    
    //draw the input magnitudes last
    for(int i = 0; i < fft.bins; i++) {
        ofSetColor(255, 255, 255,255);
        ofRect(i * 11, ofGetHeight()/2 - (fft.magnitudesDB[i] * 4), 11,
               fft.magnitudesDB[i] * 8);
        
    }
        //print out the fadeEstimate
	    cout<< fadeEstimate << " fadeEstimate Value is .. , " << endl;
        //print out the values of estimated frequencies.
        cout<< estimate1 << " estimate1 Value is .. , " << endl;
        cout<< estimate2 << " estimate2 Value is .. , " << endl;
        cout<< estimate3 << " estimate3 Value is .. , " << endl;
        
}

//--------------------------------------------------------------
void testApp::audioOut 	(float * output, int bufferSize, int nChannels){
	
	for (int i = 0; i < bufferSize; i++) {
        double wave, originalWave, octaveWave, fifthWave;
        //add three signals together to give a unified output.
        //must use abs to set an absolute value of the resulting waves.
        //Joshua Nobels orgional example gave an output that was an octave too low but was relativly accurate and medium notes.
        //I have created an if block to play the output at his original or at the "correct" pitch by multiplying the outputs by 2 initiallly.
        
                if (accurateOut == true){
                    //create a sterio mix of the combined waves using MaxiMix object
                    //taking an input of the wave a simple array for sterio Left and Right [2], and a double x for volume
                    originalWave = sine.sinebuf4(abs(estimatedPitch[0])*2) + sine1.sinebuf4(abs(estimatedPitch[1])*2) + sine1.sinebuf4(abs(estimatedPitch[2])*2);
                mix.stereo(originalWave/3.f, outputs, 0.5);
                } else if (octaveLower == true){
                    //create a sterio mix of the combined waves using MaxiMix object
                    //taking an input of the wave a simple array for sterio Left and Right [2], and a double x for volume
                    octaveWave = sine.sinebuf4(abs(estimatedPitch[0])) + sine1.sinebuf4(abs(estimatedPitch[1])) + sine1.sinebuf4(abs(estimatedPitch[2]));

                    mix.stereo(octaveWave/3.f, outputs, 0.5);
        
                } else if (perfect5th == true){
                    fifthWave = sine.sinebuf4(abs(estimatedPitch[0])*0.5) + sine1.sinebuf4(abs(estimatedPitch[1])*0.5) + sine1.sinebuf4(abs(estimatedPitch[2])*0.5);
                    mix.stereo(fifthWave/3.f, outputs, 0.5);
                }
        
     
		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
		output[i*nChannels + 1] = outputs[1];
        
	}
	
}

//--------------------------------------------------------------
void testApp::audioIn 	(float * input, int bufferSize, int nChannels){
	
	double lIn, rIn;
    //Here, the input data is sent to the FFT, which figures out how loud each different magnitude in the sound is, using the magsToDB() method. Then we look through each ÒloudnessÓ registered in the magnitudesDB array and determine what the loudest three bins in that array are, so that they can be used to create sine waves:
    int i;//declare i here so it can be used by both following for loops..
    for (i = 0; i < bufferSize; i++){
        //left input
        lIn = input[i*2]; if(fft.process(lIn)) {
            fft.magsToDB(); }
        //right input
        rIn = input[i*2 + 1]; if(fft.process(rIn)) {
            fft.magsToDB(); }
    }
    
    bin_number = bin_number1 = bin_number2 = 0;//intitalise the bins to 0
    largests[0] = largests[1] = largests[2] = 0.f;//intialise the magnitude arrays to 0
    for (i = 0; i < fft.bins; i++) {
        if(abs(fft.magnitudesDB[i]) > largests[0]) {
            largests[2] = largests[1];
            largests[1] = largests[0];
            largests[0] = abs(fft.magnitudesDB[i]);
            bin_number2 = bin_number1;
            bin_number1 = bin_number;
            bin_number = i;
        }
    }
    
    //The 12.0 here simply seemed like a nice number to determine whether there was adedequate volume in that bin:
    if(largests[0] > 12.0)
        estimatedPitch[0] = ( (float) bin_number / fft.bins) * (sampleRate * 0.5);
    else
        estimatedPitch[0] = 0.f;
    if(largests[1] > 12.0)
        estimatedPitch[1] = ( (float) bin_number1 / fft.bins) * (sampleRate * 0.5);
    else
        estimatedPitch[1] = 0.f;
    if(largests[2] > 12.0)
        estimatedPitch[2] = ( (float) bin_number2 / fft.bins) * (sampleRate * 0.5);
    else
        estimatedPitch[2] = 0.f;
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
		case 'a':
		case 'A':
            accurateOut = true;
            perfect5th = false;
            octaveLower = false;
			break;
		case 's':
		case 'S':
			octaveLower = true;
            perfect5th = false;
            accurateOut = false;
			break;
		case 'd':
		case 'D':
			perfect5th = true;
            accurateOut = false;
            octaveLower = false;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
 
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    
    //camera.rotateAround(1, ofVec3f(1,0,0), ofVec3f(1,0,0));
    //camera.begin();
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