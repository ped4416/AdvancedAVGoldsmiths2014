/*Basic functionality for the project taken from week 10 of the Advanced Audiovisual Processing course at Goldsmiths College 2013. http://doc.gold.ac.uk/CreativeComputing/creativecomputation/?page_id=1200Procedural
*/

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	/* some standard setup stuff*/
	
	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0);
	ofSetFrameRate(60);
	
	/* This is stuff you always need.*/
	
	sampleRate 			= 44100; /* Sampling Rate */
	initialBufferSize	= 512;	/* Buffer Size. you have to fill this buffer with sound*/
	lAudioOut			= new float[initialBufferSize];/* outputs */
	rAudioOut			= new float[initialBufferSize];
	lAudioIn			= new float[initialBufferSize];/* inputs */
	rAudioIn			= new float[initialBufferSize];
	
	
	/* This is a nice safe piece of code */
	memset(lAudioOut, 0, initialBufferSize * sizeof(float));
	memset(rAudioOut, 0, initialBufferSize * sizeof(float));
	
	memset(lAudioIn, 0, initialBufferSize * sizeof(float));
	memset(rAudioIn, 0, initialBufferSize * sizeof(float));
	
	/* Now you can put anything you would normally put in maximilian's 'setup' method in here. */
	
    //load in 3 samples.
    //2 are the same to create a vocoder type effect as illustrated in audioRequested..
	sample1.load(ofToDataPath("nightQuiet.wav"));
	sample2.load(ofToDataPath("Gods.wav"));
    sample3.load(ofToDataPath("Gods.wav"));
    
	//set fft size
    // It should be a power of 2: 2048, 1024 or 512 is good..
    // any higher numbers start to smeer the output
	fftSize = 2048;
    //setting up a window size and a hop size. Best to have window size the same as the FFT size.
    myWindowSize = 2048;//same as FFT
    //The hop size should be around 1/4 of the FFT size.
    myHopSize = 512;//1/4 of FFT size is best
    
    //setup for the 4 fft and inverse ones too..
	mfft.setup(fftSize,  myWindowSize, myHopSize);//keep the ratios like this.
	ifft.setup(fftSize,  myWindowSize, myHopSize);
	mfft2.setup(fftSize,  myWindowSize, myHopSize);
	ifft2.setup(fftSize,  myWindowSize, myHopSize);
    mfft3.setup(fftSize,  myWindowSize, myHopSize);
	ifft3.setup(fftSize,  myWindowSize, myHopSize);
    mfft4.setup(fftSize,  myWindowSize, myHopSize);
	ifft4.setup(fftSize,  myWindowSize, myHopSize);
	
    //set all bools to false until triggerd for playing back all three fft versions.
    godsOrdinary = false;
    monoToneVocoder = false;
    industrialMixofGod = false;
    godsGivenUpNoNight = false;
	
    //setup for a sample rate of 44100 with two channels left and right and a buffer to hold 512 sampels
	ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    //this starts the sound system by adding in number of input and ouput channels.
    // a call to baseApp for audio and adding the declared sampleRate, Buffersize and number of buffers.
	ofSoundStreamSetup(2,2, this, sampleRate, initialBufferSize, 4);/* Call this last ! */
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
    //magnitude and phase of main Track "Gods Given Up On Him"
	if(godsOrdinary == true){
        //draw fft output scale numbers to show a decent spectogram image.
        float xinc = 900.0 / fftSize * 20.0;
        for(int i=0; i < fftSize / 2; i++) {
            float height = mfft2.magnitudesDB[i] / 50.0 * 600;
            ofSetColor(height, 255, height, 255);
            //height multiplyer must be in multiples of 2 for symmetry
            ofRect(100 + (i*xinc),350 - height,5, height*2);
        }
        //draw phases
        ofSetColor(0, 255, 0, 100);
        for(int i=0; i < fftSize / 2; i++) {
            float height = mfft2.phases[i] / 50.0 * 600;
            ofSetColor(height, 255, height, 255);
            ofRect(100 + (i*xinc),600 - height,5, height*4);
        }
    }
    //magnitude of "Gods Given up on him" and phase of "nightQuiet"
    if(godsGivenUpNoNight == true){
        //draw fft output
        float xinc = 900.0 / fftSize * 20.0;
        for(int i=0; i < fftSize / 2; i++) {
            float height = mfft2.magnitudesDB[i] / 50.0 * 600;
            ofSetColor(height, 255, height, 255);
            ofRect(100 + (i*xinc),350 - height,5, height*2);
        }
        //draw phases
        ofSetColor(0, 255, 0, 100);
        for(int i=0; i < fftSize / 2; i++) {
            float height = mfft.phases[i] / 50.0 * 600;
            ofRect(100 + (i*xinc),600 - height,5, height*4);
        }
    }
    
    //magnitude and phase is switched to reverse of above
    if(industrialMixofGod == true){
        //draw fft output
        float xinc = 900.0 / fftSize * 20.0;
        for(int i=0; i < fftSize / 2; i++) {
            float height = mfft.magnitudesDB[i] / 50.0 * 600;
            ofSetColor(height, 255, height, 255);
            ofRect(100 + (i*xinc),350 - height,5, height*2);
        }
        //draw phases
        ofSetColor(0, 255, 0, 100);
        for(int i=0; i < fftSize / 2; i++) {
            float height = mfft2.phases[i] / 50.0 * 600;
            ofRect(100 + (i*xinc),600 - height,5, height*4);
        }
    }
    
    //Magnitudes of Band Track only for vocoder effect.
    if(monoToneVocoder == true){
        //draw fft output
        float xinc = 900.0 / fftSize * 20.0;
        for(int i=0; i < fftSize / 2; i++) {
            //only need the pure magnitudes here not the magnitudesDB
            float height = mfft2.magnitudes[i] / 50.0 * 600;
            ofSetColor(height, 255, height, 255);
            ofRect(100 + (i*xinc),350 - height,5, height*2);
        }
        //no need to draw phases as no data is coming out of this
        ofSetColor(0, 255, 0, 100);
        for(int i=0; i < fftSize / 2; i++) {
            float height = mfft3.phases[i] / 50.0 * 600;
            ofRect(100 + (i*xinc),600 - height,5, height*4);
        }
    }
	
}

void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
    
	for (int i = 0; i < bufferSize; i++){
        
        //assign wave variables to load in the three wav files to be used in the FFT analysis
		wave = sample1.play();
		wave2 = sample2.play();
        wave3 = sample3.play();

		//get fft
		if (mfft.process(wave)) {
            mfft.magsToDB();
		}
		
		if (mfft2.process(wave2)) {
           
            if(monoToneVocoder == false){
                //for version 4 comment this out. (This gives a vocoder effect
                //tones are rounded down. Bass and vocals in particular sound very monotonal
                mfft2.magsToDB();
            } else if(monoToneVocoder == true){
            //do nothing
            }
            
		}

		//inverse fft
        /* Version1 */
        //allow a version where magnitudes and phases from the "Gods Given Up ON Him Track" are both
        //added meaning the sound has been recreated back into the audio domain as it came in.
        if(godsOrdinary == true){
            ifftVal4 = ifft.process(mfft2.magnitudes, mfft2.phases);
            //play result of 3nd ifft
            mymix4.stereo(ifftVal4, outputs, 0.5);
            
            //loud signal needs to be at 0.5 to be at same volume as monoToneVocoder
            lAudioOut[i] = output[i*nChannels    ] = outputs[0]*0.4;
            rAudioOut[i] = output[i*nChannels + 1] = outputs[1]*0.4;
        }
         
        /* Version2 */
        //taking magnitudes from "nightQuiet" and phases from "Gods Given Up On Him"
        //The magnitudes come through much louder so samples set to "nightQuiet" as 1
        //and "Gods Given Up On Him" as 2 to have only the phase information left.
        //this is interesting as the track "Gods Given Up On Him" is still regognisable. More industrial.
        if (industrialMixofGod == true){
            ifftVal = ifft.process(mfft.magnitudes, mfft2.phases);
                //play result of 1st ifft
            mymix.stereo(ifftVal, outputs, 0.5);
            
            lAudioOut[i] = output[i*nChannels    ] = outputs[0]*0.6;
            rAudioOut[i] = output[i*nChannels + 1] = outputs[1]*0.6;
        }
        
        /* Version3 */
        //reverse the process. Now "nightQuiet" has only its phase information and is rather lost.
        //the result is a very filtered sound coming from the "Gods Given Up On Him" track.
        if (godsGivenUpNoNight) {
		   
            ifftVal2 = ifft.process(mfft2.magnitudes, mfft.phases);
              //play result of 2nd ifft
            mymix2.stereo(ifftVal2, outputs, 0.5);
            
            lAudioOut[i] = output[i*nChannels    ] = outputs[0]*0.3;
            rAudioOut[i] = output[i*nChannels + 1] = outputs[1]*0.3;
        }
        
        /* version4 */
        //taking magnitudes from the Main track "Gods given up on him"
        //this creates a monotone track expecially bass and vocals.
        
        if(monoToneVocoder == true){

            ifftVal3 = ifft.process(mfft2.magnitudes, mfft3.phases);
                //play result of 3nd ifft
            mymix3.stereo(ifftVal3, outputs, 0.5);
            
            //quiet signal needs to be at 1!
            lAudioOut[i] = output[i*nChannels    ] = outputs[0];
            rAudioOut[i] = output[i*nChannels + 1] = outputs[1];
        }
    
	}
}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){
	
	
	/* You can just grab this input and stick it in a double, then use it above to create output*/
	
	for (int i = 0; i < bufferSize; i++){
		
		/* you can also grab the data out of the arrays*/
		
		lAudioIn[i] = input[i*2];
		rAudioIn[i] = input[i*2+1];
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    //allow the user to iterate through the various fft effects on magnitudes and phases
    if( key == '1' ){//play a standard version much the same as original sample
        godsOrdinary = true;
        godsGivenUpNoNight = false;
        monoToneVocoder = false;
        industrialMixofGod = false;
    } else if( key == '2' ){//an idustrial mix is switched on
        godsGivenUpNoNight = true;
        monoToneVocoder = false;
        industrialMixofGod = false;
        godsOrdinary = false;
    } else if( key == '3' ){//blend more of the ambient night time sample in
        industrialMixofGod = true;
        monoToneVocoder = false;
        godsGivenUpNoNight = false;
        godsOrdinary = false;
    } else if( key == '4' ){//monotone version only magnitudes of main band track included
        monoToneVocoder = true;
        industrialMixofGod = false;
        godsGivenUpNoNight = false;
        godsOrdinary = false;

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