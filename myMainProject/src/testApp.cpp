/*Basic functionality for the project taken from weeks 8, 9 and 10 of the Advanced Audiovisual Processing course at Goldsmiths College 2013. http://doc.gold.ac.uk/CreativeComputing/creativecomputation/?page_id=1200Procedural
  The main code for the pitch analysis is from Joshua Nobel's book "Interactivity 2nd Edition"
  The code to iterate through pixels is similar to openFramework examples and some that were shown in the aforementioned Audiovisual Course.
 
 The song "Gods Given Up On Him" is an origional owned by "Small Engine Repair" c 2014.  
 The texture map image "Gods" is also owned and copyrighted to "Small Engine Repair" c 2014.
 The track "nightQuiet" was downloaded from freeSound.net and has no copyright attached. 
 Feel free to use any of the code as required. Pedro Kirk Jan 2014
 Find me at www.pedrokirk.co.uk
*/

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //load fonts to write to screen
    newFont.loadFont("Batang.ttf", 50, true, true, true);
    smallFont.loadFont("Batang.ttf", 13, true, true, true);
    
	//make sure that the objects can use alpha values
	ofEnableAlphaBlending();
	ofBackground(0);//black background
	ofSetVerticalSync(true);
    
    showInstructions = true;
    
    //this texture wrap code is taken from example code on a tutorial for OpenFrameworks.
    //https://sites.google.com/site/ofauckland/examples/12-textured-sphere-example
    //set up for texturing
    //glEnable(GL_DEPTH_TEST); //enable depth comparisons and update the depth buffer
    ofDisableArbTex(); //needed for textures to work with gluSphere
    gods.loadImage("images/gods.jpg");
    
    //prepare quadric for sphere
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
 
    //initialise bools to track visual displays
    texture = false;
    sphereShow = true;
    boxShow = true;
    cylinderShow = false;
    
    //initialise bools for tracking audio output
    filteredSound = false;
    playMagnitudes = false;
    videoGrabberShow = false;
    
    //initialise four variables to store three different filters.
    lowVal = midValLow = midValHigh = highVal = 0;
	
    // load the song by Small Engine Repair "God's Given Up On Him"
    //this has been loaded as different holders below as playback was being
    //comprimised creating a double speed issue.
	godsGivenUp.load(ofToDataPath("Gods.wav"));
    //load in 2 more samples for filtering
    //sample 2 is the same Band Track to create a vocoder type effect as illustrated in audioRequested
	sample1.load(ofToDataPath("nightQuiet.wav"));
    sample2.load(ofToDataPath("Gods.wav"));
    //load in sample again for mfcc to access a clean version.
    sample3.load(ofToDataPath("Gods.wav"));
    
    //setup loop for mouse interaction starting at 0 and ending at 1
    loopStart = 0;
    loopEnd = 1;

    // 2 setup for FFT example.
    //--------------------------------------------------------------
	
	//setup the main audio prperties
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
    //need to half the sample rate here for mfcc usage
    mfft4.setup(fftSize/2,  myWindowSize/2, myHopSize/2);
	ifft4.setup(fftSize/2,  myWindowSize/2, myHopSize/2);
    
    //setup for octave averaging
    nAverages = 24;//12;
    oct.setup(sampleRate, fftSize/2, nAverages);
	//setup for the mfcc
	mfccs = (double*) malloc(sizeof(double) * 13);
	mfcc.setup(512, 42, 13, 20, 20000, sampleRate);
	
    //set all bools to false until triggered for playing back all three fft versions.
    godsOrdinary = false;
    monoToneVocoder = false;
    industrialMixofGod = false;
    godsGivenUpNoNight = false;
    timbralMFCC = false;
    
    
    // 3 setup for FFT PITCH ESTIMATOR example.
    //--------------------------------------------------------------
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
    
    //set up bools for playback
    accurateOut = true;//start with this true so it mimicks the real input pitch at correct octave.
    octaveLower = false;
    perfect5th = false;
    
    //set up the FFT for Pitch Tracking with a size, window size and hop size.
    //it is usually best to have the window size be the same as the fft size and the hop size should be around 1/4 of the fft size.
    //the fft size should also be a power of 2.
    
	fftSize2 = 16384;
    //setting up a window size and a hop size. Best to have window size the same as the FFT size.
    myWindowSize2 = 1024;//same as FFT
    //The hop size should normally be around 1/4 of the FFT size but this produces a bad output.
    myHopSize2 = 512;//1/4 of FFT size is usually best but not in this instance
    fft.setup(fftSize2, myWindowSize2, myHopSize2);//16384 was set by Joshua Nobel
    
    //setup for a sample rate of 44100 with two channels left and right and a buffer to hold 512 sampels
	ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    //this starts the sound system by adding in number of input and ouput channels.
    // a call to baseApp for audio and adding the declared sampleRate, Buffersize and number of buffers.
	ofSoundStreamSetup(2,2, this, sampleRate, initialBufferSize, 4);/* Call this last ! */
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
    
    //the GL_DEPTH_TEST upsets the video grabber so set to false if
    //there is no call to texture the Sphere
    if (texture == true) {
        glEnable(GL_DEPTH_TEST); //enable depth comparisons and update the depth buffer
    } else  glDisable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0); // create a black background
    
    if (videoGrabberShow == true){
        
        //turn off all lights?
        light.disable();
        spot.disable();
        //the way open frameworks is set up simply calling disable() on a light does not turn them off fully!
        ofDisableLighting();
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
        
        
        //draw the mfcc bands with colour dictated by the audio input.
        //intentionally within videGrabber code to take in teh estimated pitch data to change alpha values
        if(timbralMFCC == true){
            //Mel bands
            ofSetColor(255, 0, 255,fadeEstimate*6);
            float xinc = 900.0 / fftSize * 2.0;
            xinc = 900.0 / 42.0;
            for(int i=0; i < 42; i++) {
                //		cout << mfcc.melBands[i] << ",";
                float height = mfcc.melBands[i] * 5.0;
                ofRect(100 + (i*xinc),400 - height,10, height);
            }
            //	cout << endl;
        }

        
    }
    
    if(filteredSound == true){
        
        //set up the lights for highlighting the graphics
        light.enable();//OPENGL 3D world a light gives a sense of depth.
        spot.enable();//enable a spot light
        
        //alter the specular qualites of the light
        light.setSpecularColor(ofColor(0, 255, 255, 200));
        
        //move positions of light so the mouse gives location
        light.setPosition(mouseX, mouseY, 500);
        
        light.rotate(360, ofGetWidth()/2, ofGetHeight()/2, 100);
        
        
        //need to smooth out the frequency to minimise any big jumps in the filters.
        //the low pass filter cuts off any high jump values.
        lowValSmoothed = lowValSmoothing.lopass(lowVal, loopEnd);//constrain the frequency.
        midValLowSmoothed = midValLowSmoothing.lopass(midValLow, loopEnd);
        midValHighSmoothed = midValHighSmoothing.lopass(midValHigh, loopEnd);
        highValSmoothed = highValSmoothing.lopass(highVal, loopEnd);
        
        //a spot light is set to flicker position with the midHigh filter on X and high on Y position.
        //only shines on spheres
        spot.setSpotConcentration(abs(midValHigh*5000)+10);
        spot.setSpotlightCutOff(abs(midValHigh)+10);
        spot.setPosition(ofGetWidth()/2+midValHigh*100, ofGetHeight()/2+highVal*500, 500);
        
        if (sphereShow == true) {
            //rotate spheres relative to the bass, midHigh and treble in the sound seperated
            //into the 3 different spheres
            sphere[0].rotate(lowValSmoothed * 10, lowValSmoothed, midValHighSmoothed, highValSmoothed);
            sphere[1].rotate(midValHighSmoothed * 10, lowValSmoothed, midValHighSmoothed, highValSmoothed);
            sphere[2].rotate(highValSmoothed * 10, lowValSmoothed, midValHighSmoothed, highValSmoothed);
            
            for(int i=0; i < 3; i++) {
                float colMod = i / 3.0+lowValSmoothed;
                ofSetColor(colMod * 255.0, (1-colMod) * 255.0, midValHighSmoothed, 50);//4th term is alpha value
                
                //spheres move in reaction to the bass mid and treble of the sound.
                //the bass frequency affects the radius.
                sphere[i].setRadius(100 + (i * (50 + (50 * lowValSmoothed))));
                sphere[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
                sphere[i].drawWireframe();
                sphere[i].draw();
                
            }
        }
        
        
        if (boxShow == true) {
            //rotate spheres relative to midLow only
            for (int i=0; i<30; i++) {
                box[1].rotate(midValLowSmoothed * 10, lowValSmoothed, midValLowSmoothed, highValSmoothed);
                
                float colMod = i / 3.0+lowValSmoothed;
                ofSetColor(colMod * 255.0, (1-colMod) * 255.0, midValLowSmoothed, 100);//4th term is alpha value
                
                //boxes move in reaction to the midLow of the sound.
                box[i].setDepth(100 + (i * (50 + (50 * midValLowSmoothed))));
                box[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
                //learve the box a wireframe as it created a tunel type vision onto the scene.
                box[i].drawWireframe();
                
            }
        }
        
        if (cylinderShow == true){
            //rotate cylinders relative to midLow only
            for (int i=0; i<30; i++) {
                cylinder2[1].rotate(midValLowSmoothed * 10, midValLowSmoothed, midValLowSmoothed, midValLowSmoothed);
                
                float colMod = i / 3.0+lowValSmoothed;
                ofSetColor(colMod * 255.0, (1-colMod) * 255.0, midValLowSmoothed, 100);//4th term is alpha value
                
                //cylinders move in reaction to the midLow
                cylinder2[i].setRadius(100 + (i * (50 + (500 * midValLowSmoothed))));
                cylinder2[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
                cylinder2[i].drawWireframe();
                //cylinder2[i].draw();
                
            }
            
            //rotate cylinders here relative to the bass only
            for (int i=0; i<30; i++) {
                cylinder[i].rotate(lowValSmoothed * 10, lowValSmoothed, lowValSmoothed, lowValSmoothed*50);
                
                float colMod = i / 3.0+lowValSmoothed;
                ofSetColor(colMod * 255.0/i, (1-colMod) * 255.0/i, lowValSmoothed, 50+i);//4th term is alpha value
                
                //spheres move in reaction to the bass mid and treble of the sound.
                cylinder[i].setRadius(2 + (i * (i/20+ (i * lowValSmoothed))));
                cylinder[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
                // cylinder[i].drawWireframe();
                cylinder[i].draw();
                
            }
        }
        
        
        //allow user to bring in a textured sphere of the band ALBUM COVER "SERVE YOURSELF"
        if(texture == true){
            //using push and pop Matrix so the translate function does not affect the
            //remaining geometry. The sphere is subdivided around the z axis into slices and along the z axis into stacks (similar to lines of longitude and latitude).
            ofPushMatrix();
            //change origin to center and Z axis set to - 400 so it is further away.
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2, mouseY/4);
            
            //rotate sphere over time
            ofRotateY(ofGetFrameNum()+lowValSmoothed);
            ofRotateX(-90); //north pole facing up
            ofRotate(lowValSmoothed * 10, lowValSmoothed, midValHighSmoothed, highValSmoothed);
            
            //bind and draw texture
            ofSetColor(255.0, 255.0, 255.0, 100);//4th term is alpha value
            gods.getTextureReference().bind();
            gluSphere(quadric, 200+midValLow*200, 100, 100);
            ofPopMatrix();
        } else gods.getTextureReference().unbind();
        
        //print out the values for loop start and end
        cout<< loopStart << " loopStart Value is .. , " << endl;
        cout<< loopEnd << " loopEnd Value is .. , " << endl;
        cout<< midValLow << " MidVal Value is .. , " << endl;
        cout<< midValLowSmoothed << " MidValLowSmoothed Value is .. , " << endl;
    }
    
    
    //play back the images to match magnitudes and phases
    if(playMagnitudes == true){
        
        //turn off all lights?
        light.disable();
        spot.disable();
        //the way open frameworks is set up simply calling disable() on a light does not turn them off fully!
        ofDisableLighting();

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
            for(int i=0; i < fftSize / 2; i++) {
                float height = mfft.phases[i] / 50.0 * 600;
                ofSetColor(height, 255, height, 255);
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
            //no need to draw phases as no data is coming out of this but call anyway to prove it.
            ofSetColor(0, 255, 0, 100);
            for(int i=0; i < fftSize / 2; i++) {
                float height = mfft3.phases[i] / 50.0 * 600;
                ofRect(100 + (i*xinc),600 - height,5, height*4);
            }
        }
        
    }
    
    //draw text last
    if (showInstructions == true) {
        //turn off all lights?
        light.disable();
        spot.disable();
        //the way open frameworks is set up simply calling disable() on a light does not turn them off fully!
        ofDisableLighting();
        ofSetColor(255,255,255,255);
        glDisable(GL_DEPTH_TEST);
        //draw instructions to the screen.
        newFont.drawString("Hack A Band Song \nGod's Given Up On Him",20,80);
        smallFont.drawString("Toggle DOWN ARROW to view the instructions. \nPlease press DOWN ARROW once to remove the instructions before you start the FUN!\nYou will only get breif flashes otherwise so be pre-warned.",20,200);
        smallFont.drawString("\n\nSPECTRAL ANALYSIS AUDIO FFT IFFT\nPress 1 for magnitude and phase of GOD'S GIVEN UP ON HIM to be output. \nPress 2 to hear magnitudes from AMBIENT track and phases from GOD'S GIVEN UP ON HIM\nPress 3 to hear a reversal. Magnitudes from SONG and phases from AMBIENT\nPress 4 for magnitudes from the Main track GOD'S GIVEN UP ON HIM this creates a quiet monotone.\nPress 9 to add Mel bands to join the Video Grabber Audio in. Need to make sure 5 is toggeled on!\n\nAUDIO FILTERING VISUALISED\nPress a, s, d, and f to see a textured sphere and other geometric shapes.\nMouseX and MouseY are used to create loops of GODS GIVEN UP ON HIM\nMouseY alters textured sphere position.\n\nLIVE AUDIO VIDEO GRABBER JAM\nToggle 5 to grab video input and see pixel data warped\nPress 7 to here an accurate pitch out.\nPress 8 to here an octave of live audio input.\nPress 8 to here a 5th lower.",20,240);
    }
    
    if(showInstructions == true){
        playMagnitudes = false;
        filteredSound = false;
        videoGrabberShow = false;
        
    }
}

void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
    
	for (int i = 0; i < bufferSize; i++){
        
        if(playMagnitudes == true){
            //assign wave variables to load in the three wav files to be used in the FFT analysis
            wave = sample1.play();
            wave2 = godsGivenUp.play();
            wave3 = sample2.play();
            wave4 = sample3.play(1.);

            //process the first sample "nightQuiet" and give back its magnitude
            if (mfft.process(wave)) {
                mfft.magsToDB();
            }
            
            //process wave2 "Gods Given Up On Him" (band track) and give back its magnitude
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
                //play result of 4th ifft
                mymix4.stereo(ifftVal4, outputs, 0.5);

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
            }
            
            /* Version3 */
            //reverse the process. Now "nightQuiet" has only its phase information and is rather lost.
            //the result is a very filtered sound coming from the "Gods Given Up On Him" track.
            if (godsGivenUpNoNight) {
               
                ifftVal2 = ifft.process(mfft2.magnitudes, mfft.phases);
                  //play result of 2nd ifft
                mymix2.stereo(ifftVal2, outputs, 0.5);
                
            }
            
            /* version4 */
            //taking magnitudes from the Main track "Gods given up on him"
            //this creates a monotone track expecially bass and vocals.
            if(monoToneVocoder == true){

                ifftVal3 = ifft.process(mfft2.magnitudes, mfft3.phases);
                //play result of 3nd ifft
                mymix3.stereo(ifftVal3, outputs, 0.5);
         
            }
            
            /* version5 */
            //Taking a Mel Frequency Cepstral Coefficient taht describes the texture or timbre of a sound.
            //it is pitch invarient. Useful for comparing sounds.
            if (timbralMFCC == true) {
                //process wave3 version 2 of Band Track for Version 5 MFCC
                if (mfft4.process(wave4)){
                    mfft4.magsToDB();
                    //oct.calculate(mfft4.magnitudesDB);
                    mfcc.mfcc(mfft4.magnitudes, mfccs);
                    //cout << mfft4.spectralFlatness() << ", " << mfft4.spectralCentroid() << endl;
                }
                     //oct.calculate(mfft2.magnitudesDB);
                     //mfcc.mfcc(mfft2.magnitudes, mfccs);
                mymix5.stereo(wave4, outputs, 0.5);
               
            }
        
           
        }
        
        

        //only run and output the filterd audio if user requests it
        if (filteredSound == true) {
            //setup for filtering the track "Gods Given Up on Him"
            //--------------------------------------------------------------
            //create a sample between -1 and 0 but with two variables creating a region
            //loopstart and loopend these are controled by the mouse.
            sample=godsGivenUp.playLoop(loopStart, loopEnd);
            
            //record three different sample values using filters
            //1 resonant lowPass filter. Anything below 100Hz is kept allowing amplitude through
            lowVal = lowFilt.lores(sample, 100, 2);
            
            //2 making a LopwMid bandpass filter from a lowres and a highres by chopping off the bottom and top sequentially
            //keeping frequencies from 100 to 1000Hz
            midValLow = midFilt2.hires(midFilt1.lores(sample, 1000, 3), 100, 3);
            
            //3 making a HiMid bandpass filter from a lowres and a highres by chopping off the bottom and top sequentially
            //keeping frequencies from 1000 to 4000Hz
            midValHigh = midFilt2.hires(midFilt1.lores(sample, 4000, 3), 1000, 3);

            
            //4 resonant highPass filter gives the high frequecies anything above 4000Khz
            highVal = highFilt.hires(sample, 8000, 2);//get the high freq.
        
        
            //none of the filters are being played they are only used to take the frequency
            //information to control the graphics.
            //make a call to MaxiMix to create a sterio out taking in the "sample"
            //outputs [2] for left and right and a volume x.
            mymix.stereo(sample, outputs, 0.2);//volume low to compensate for output of "Vocoder type sound"
          
           
        }
        //declare three waves for holding the estimated pitches and others for output
        double wave, originalWave, octaveWave, fifthWave;
        if (videoGrabberShow == true) {
            //add three signals together to give a unified output.
            //must use abs to set an absolute value of the resulting waves.
            //Joshua Nobels orgional example gave an output that was an octave too low but was relativly accurate and medium notes.
            //I have created an if block to play the output at his original or at the "correct" pitch by multiplying the outputs by 2 initiallly.
            
            if (accurateOut == true){
                //create a sterio mix of the combined waves using MaxiMix object
                //taking an input of the wave a simple array for sterio Left and Right [2], and a double x for volume
                originalWave = sine.sinebuf4(abs(estimatedPitch[0])*2) + sine1.sinebuf4(abs(estimatedPitch[1])*2) + sine1.sinebuf4(abs(estimatedPitch[2])*2);
                //mix.stereo(originalWave/3.f, outputs, 0.5);
            } else if (octaveLower == true){
                //create a sterio mix of the combined waves using MaxiMix object
                //taking an input of the wave a simple array for sterio Left and Right [2], and a double x for volume
                octaveWave = sine.sinebuf4(abs(estimatedPitch[0])) + sine1.sinebuf4(abs(estimatedPitch[1])) + sine1.sinebuf4(abs(estimatedPitch[2]));
                
                //mix.stereo(octaveWave/3.f, outputs, 0.5);
                
            } else if (perfect5th == true){
                fifthWave = sine.sinebuf4(abs(estimatedPitch[0])*0.5) + sine1.sinebuf4(abs(estimatedPitch[1])*0.5) + sine1.sinebuf4(abs(estimatedPitch[2])*0.5);
                //mix.stereo(fifthWave/3.f, outputs, 0.5);
            }

        }
        //variable to control the volume of the Pitch Estimate out.
        float pitchEVol = 0.03;//keep low!!;
        //output the current buffer for left and right channels. 
        lAudioOut[i] = output[i*nChannels    ] = outputs[0] + fifthWave*pitchEVol + octaveWave*pitchEVol + originalWave*pitchEVol;
        rAudioOut[i] = output[i*nChannels + 1] = outputs[1] + fifthWave*pitchEVol + octaveWave*pitchEVol + originalWave*pitchEVol;
	}
    
}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){

        //Code taken from Joshus nobels Pitch Tracking Example.
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
 
    
    //keys to switch the visuals
    switch (key) {
		case 'a':
		case 'A':
            //show the textured sphere remove the three wireframe ones
            texture = true;
            sphereShow = false;
            cylinderShow = true;
            //set filtered sound to true to match up with visuals
            filteredSound = true;
            //set fft player to false to stop filtered audio output
            playMagnitudes = false;
            break;
		case 's':
		case 'S':
            //show all geometry take out texture sphere
            texture = false;
            sphereShow = true;
            boxShow = true;
            cylinderShow = true;
            //set filtered sound to true to match up with visuals
            filteredSound = true;
            //set fft player to false to stop filtered audio output
            playMagnitudes = false;
			break;
		case 'd':
		case 'D':
            //take out all spheres
            texture = false;
            boxShow = true;
            sphereShow = false;
            cylinderShow = true;
            //set filtered sound to true to match up with visuals
            filteredSound = true;
            //set fft player to false to stop filtered audio output
            playMagnitudes = false;
            break;
        case 'f':
		case 'F':
            //take out box and all spheres
            texture = false;
            boxShow = false;
            sphereShow = false;
            cylinderShow = true;
            //set filtered sound to true to match up with visuals
            filteredSound = true;
            //set fft player to false to stop filtered audio output
            playMagnitudes = false;
            break;
            
        //keys to switch the audio filters
        //allow the user to iterate through the various fft effects on magnitudes and phases
        case '1':
            //play a standard version much the same as original sample
            godsOrdinary = true;
            godsGivenUpNoNight = false;
            monoToneVocoder = false;
            industrialMixofGod = false;
            timbralMFCC = false;
            //set filtered sound to false as not needed for the FFTs
            filteredSound = false;
            //set fft player to true
            playMagnitudes = true;
           
            break;
        case '2'://an idustrial mix is switched on
            godsGivenUpNoNight = true;
            monoToneVocoder = false;
            industrialMixofGod = false;
            godsOrdinary = false;
            timbralMFCC = false;
            //set filtered sound to false as not needed for the FFTs
            filteredSound = false;
            //set fft player to true
            playMagnitudes = true;
        break;
        case '3'://blend more of the ambient night time sample in
            industrialMixofGod = true;
            monoToneVocoder = false;
            godsGivenUpNoNight = false;
            godsOrdinary = false;
            timbralMFCC = false;
            //set filtered sound to false as not needed for the FFTs
            filteredSound = false;
            //set fft player to true
            playMagnitudes = true;
        break;
        case '4'://monotone version only magnitudes of main band track included
            monoToneVocoder = true;
            industrialMixofGod = false;
            godsGivenUpNoNight = false;
            godsOrdinary = false;
            timbralMFCC = false;
            //set filtered sound to false as not needed for the FFTs
            filteredSound = false;
            //set fft player to true
            playMagnitudes = true;
        break;
        case '9'://MFCC version only timbral qualities heard
            timbralMFCC = true;
            monoToneVocoder = false;
            industrialMixofGod = false;
            godsGivenUpNoNight = false;
            godsOrdinary = false;
            //set filtered sound to false as not needed for the FFTs
            filteredSound = false;
            //set fft player to true
            playMagnitudes = true;
            break;
        }
    
    if (key == OF_KEY_DOWN){
        showInstructions = !showInstructions;
    
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    //keys to switch the videoGrabber visuals
    switch (key) {
    case '5'://allow screenGrabber to activate.
          videoGrabberShow = !videoGrabberShow;
    case '6':
        accurateOut = true;
        perfect5th = false;
        octaveLower = false;
        break;
    case '7':
        octaveLower = true;
        perfect5th = false;
        accurateOut = false;
        break;
    //Expansion from Example 4 to allow more audio messing! Adding a Perfect 5th
    case '8':
        perfect5th = true;
        accurateOut = false;
        octaveLower = false;
        break;

    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    //set up the loop constraints for sample by creating a start and end point
    loopStart = x / (double)ofGetWidth();//get mouse ponts in
    loopEnd = y / (double) ofGetHeight();
    loopEnd = max(loopStart + 0.05, loopEnd);
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