/*Basic functionality for the project taken from week 8 of the Advanced Audiovisual Processing course at Goldsmiths College 2013. http://doc.gold.ac.uk/CreativeComputing/creativecomputation/?page_id=1200Procedural 
 
 Three filters are set up to manipulate the drawing of geometric shapes and their positions.
 Addition of lighting and editing of filter settings have been completed. Code is commented throughout to make this clear.
 
*/
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //make sure that the objects can use alpha values
	ofEnableAlphaBlending();
	ofBackground(0);//black background
	ofSetVerticalSync(true);
    
    //this texture wrap code is taken from example code on a tutorial for OpenFrameworks.
    //https://sites.google.com/site/ofauckland/examples/12-textured-sphere-example
    //set up for texturing
    glEnable(GL_DEPTH_TEST); //enable depth comparisons and update the depth buffer
    ofDisableArbTex(); //needed for textures to work with gluSphere
    gods.loadImage("images/gods.jpg");
    
    //prepare quadric for sphere
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    texture = false;
    sphereShow = true;
    boxShow = true;
    cylinderShow = false;
    //initialise three variables to store three different filters.
    lowVal = midVal = highVal = 0;
	
	/* You can put anything you would normally put in maximilian's 'setup' method in here. */
    // load the song by Small Engine Repair "God's Given Up On Him"
	godsGivenUp.load(ofToDataPath("Gods.wav"));

    //setup loop for mouse interaction starting at 0 and ending at 1
    loopStart = 0;
    loopEnd = 1;
    
	/* This is stuff you always need.*/
    //setup for a sample rate of 44100 with two channels left and right and a buffer to hold 512 sampels.
    maxiSettings::setup(44100, 2, 512);
    //this starts the sound system by adding in number of input and ouput channels.
    // a call to baseApp for audio and adding the declared sampleRate, Buffersize and number of buffers.
	ofSoundStreamSetup(maxiSettings::channels, maxiSettings::channels, this, maxiSettings::sampleRate, maxiSettings::bufferSize, 4);/* Call this to start the sound system*/

}

//--------------------------------------------------------------
void testApp::update(){
//nothing needed in here for this project example.

}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0); // create a black background
    light.enable();//OPENGL 3D world a light gives a sense of depth.
    
    //alter the specular qualites of the light
    light.setSpecularColor(ofColor(0, 255, 255, 200));

    //move positions of light so the mouse gives location
    light.setPosition(mouseX, mouseY, 500);
    
    light.rotate(360, ofGetWidth()/2, ofGetHeight()/2, 100);
    

    //need to smooth out the frequency to minimise any big jumps in the filters.
    //the low pass filter cuts off any high jump values.
    lowValSmoothed = lowValSmoothing.lopass(lowVal, loopEnd);//constrain the frequency.
    midValSmoothed = midValSmoothing.lopass(midVal, loopEnd);
    highValSmoothed = highValSmoothing.lopass(highVal, loopEnd);

    if (sphereShow == true) {
        //rotate spheres relative to the bass, mid and treble in the sound seperated
        //into the 3 different spheres
        sphere[0].rotate(lowValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
        sphere[1].rotate(midValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
        sphere[2].rotate(highValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
        
        for(int i=0; i < 3; i++) {
            float colMod = i / 3.0+lowValSmoothed;
            ofSetColor(colMod * 255.0, (1-colMod) * 255.0, midValSmoothed, 50);//4th term is alpha value
            
            //spheres move in reaction to the bass mid and treble of the sound.
            //the bass frequency affects the radius.
            sphere[i].setRadius(100 + (i * (50 + (50 * lowValSmoothed))));
            sphere[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
            sphere[i].drawWireframe();
            sphere[i].draw();
            
        }
    }
    
    
    if (boxShow == true) {
        //rotate spheres relative to mid only
        for (int i=0; i<30; i++) {
            box[1].rotate(midValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
        
            float colMod = i / 3.0+lowValSmoothed;
            ofSetColor(colMod * 255.0, (1-colMod) * 255.0, midValSmoothed, 100);//4th term is alpha value
            
            //boxes move in reaction to the mid of the sound.
            box[i].setDepth(100 + (i * (50 + (50 * midValSmoothed))));
            box[i].setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);//set to center of screen.
            //learve the box a wireframe as it created a tunel type vision onto the scene.
            box[i].drawWireframe();
            
        }
    }
    
    if (cylinderShow == true){
        //rotate cylinders relative to mid only
        for (int i=0; i<30; i++) {
            cylinder2[1].rotate(midValSmoothed * 10, midValSmoothed, midValSmoothed, midValSmoothed);
            
            float colMod = i / 3.0+lowValSmoothed;
            ofSetColor(colMod * 255.0, (1-colMod) * 255.0, midValSmoothed, 100);//4th term is alpha value
            
            //spheres move in reaction to the bass mid and treble of the sound.
            //sphere[i].setRadius(100 + (i * (50 + (50 * midValSmoothed))));
            cylinder2[i].setRadius(100 + (i * (50 + (500 * midValSmoothed))));
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
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2, -100);
        
        //rotate sphere over time
        ofRotateY(ofGetFrameNum()+lowValSmoothed);
        ofRotateX(-90); //north pole facing up
        ofRotate(lowValSmoothed * 10, lowValSmoothed, midValSmoothed, highValSmoothed);
        
        //bind and draw texture
        ofSetColor(255.0, 255.0, 255.0, 100);//4th term is alpha value
        gods.getTextureReference().bind();
        gluSphere(quadric, 200+midVal*200, 100, 100);
        ofPopMatrix();
    } else gods.getTextureReference().unbind();
    
    //print out the values for loop start and end
    cout<< loopStart << " loopStart Value is .. , " << endl;
    cout<< loopEnd << " loopEnd Value is .. , " << endl;
     cout<< midVal << " MidVal Value is .. , " << endl;
     cout<< midValSmoothed << " MidValSmoothed Value is .. , " << endl;
}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	
	for (int i = 0; i < bufferSize; i++){
        
        //create a sample between -1 and 0 but with two variables creating a region
        //loopstart and loopend these are controled by the mouse.
		sample=godsGivenUp.playLoop(loopStart, loopEnd);
        
        //record three different sample values using filters
        //1 resonant lowPass filter. Anything below 100Hz is kept allowing amplitude through
        lowVal = lowFilt.lores(sample, 100, 2);
        
        //2 making a bandpass filter from a lowres and a highres by chopping off the bottom and top sequentially
        midVal = midFilt2.hires(midFilt1.lores(sample, 4000, 3), 100, 3);
        
        //3 resonant highPass filter gives the high frequecies anything above 4000Khz
        highVal = highFilt.hires(sample, 8000, 2);//get the high freq.
        
        //none of the filters are being played they are only used to take the frequency
        //information to control the graphics.
        //make a call to MaxiMix to create a sterio out taking in the "sample"
        //outputs [2] for left and right and a volume x.
		mymix.stereo(sample, outputs, 0.5);
		
		//ouptut to the sound card for left and right speakers.
		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
		output[i*nChannels + 1] = outputs[1];
        
	}
	
}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){
	
	
	/* You can just grab this input and stick it in a double, then use it above to create output*/
	
	for (int i = 0; i < bufferSize; i++){
		
		/* you can also grab the data out of the arrays*/
//		sampleIn = input[i];
		
	}
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
		case 'a':
		case 'A':
            //show the textured sphere remove the three wireframe ones
            texture = true;
            sphereShow = false;
            cylinderShow = true;
            break;
		case 's':
		case 'S':
            //show all geometry take out texture sphere
			 texture = false;
             sphereShow = true;
             boxShow = true;
             cylinderShow = true;
			break;
		case 'd':
		case 'D':
            //take out all spheres
            texture = false;
            boxShow = true;
            sphereShow = false;
            cylinderShow = true;
            break;
        case 'f':
		case 'F':
            //take out box and all spheres
            texture = false;
            boxShow = false;
            sphereShow = false;
            cylinderShow = true;
            break;

    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
    //set up the loop constraints for sample by creating a start and end points
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