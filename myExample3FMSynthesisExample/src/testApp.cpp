/* The basic formula for the FM synthesis was taken from the Maximilian example folder and also the envelope function created has used parts of an example from the same library.
*/

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //load fonts to write to screen
    newFont.loadFont("Batang.ttf", 40, true, true, true);
    smallFont.loadFont("Batang.ttf", 12, true, true, true);
    //setup bools for single modulators
    synthSilence = false;
    synthPlay = false;
    ratio1to1 = false;
    ratio1to4 = false;
    ratio1to2 = false;
    ratio3to2 = false;
    ratio2to1 = false;
    ratio3to1 = false;
    ratio7to2 = false;
    ratio6to1 = false;
    //setup bools for dual modulators
    twoModulators1Ovtave = false;
    twoModualtorsSame = false;
    parallelFM = false;
    cascadeFM = false;
    twoModulatorsCrazy = false;
    
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
    ofBackground(0);
    ofSetHexColor(0xCCCCCC);
    newFont.drawString("FM SYNTH",20,50);
    smallFont.drawString("press t to turn synth on and f to turn it off",20,80);
    smallFont.drawString("mouseX position gives the pitch",20,110);
    smallFont.drawString("mouseY position gives the note duration",20,140);
    smallFont.drawString("mouseY position gives the note duration",20,170);
    smallFont.drawString("ONE MODULATOR\nPress 1 For RATIO 1:1 with the carrier making a pure sine tone out \nPress 2 For RATIO 1:4 (-24 semitones from the carrier) creating a darker sound \nPress 3 For RATIO 1:2 (-12 semitones from the carrier) creating a more wooden quality to the sound \nPress 4 For RATIO 3:2 (7 semitones above the carrier) creating a more metalic sound \nPress 5 For RATIO 2:1 (12 semitones above the carrier) creating a cleaner sound similar to a square wave \nPress 6 For RATIO 1:3 (19 semitones above the carrier) creating a bell like quality \nPress 7 For RATIO 7:2 (21 semitones above the carrier) creating a classic FM sound \nPress 8 For RATIO 6:1 (31 semitones above the carrier) creating a tighter bell like sound",20,200);
    smallFont.drawString("TWO MODULATORS\nPress q to hear 2 modulators set as the same giving a pure sine in parallel.\nPress w to hear modulator 1 at 5:4 ratio above carrier and modulator 2 at 5:2 above carrier in parallel\nPress e to hear 2 modulators set as the same giving a pure sine in parallel cascade.\nPress r to hear  modulator 1 at 5:4 ratio above carrier and modulator 2 at 5:2 above carrier in cascade\nPress a to here a crazy FM synth.",20,380);
   
}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	
	for (int i = 0; i < bufferSize; i++){
        //allow FM synth to activate properly upon startup.
        float fm = 0.0;
        
        //FM, or Frequency Modulation Synthesis
        //one wave modulates the pitch of another wave.
        //need a modulator(s) and a carrier wave. We should only hear the carrier wave with modulators effecting it.
        float carrier = int(mouseX)*2;//output frewuency is changed my the mouse position
        float modulator = 0;//initialise a modulator to 0
        float modulator2 = 0;//initialise another modulator to 0
        
/*-----------------------------------------------------------------------*/
        //Choices 1 only one modulator still loads of sounds!
        if (ratio1to1 == true) {
            //modulator set at a ratio 1:1 with the carrier making a pure sine tone out
            modulator = carrier;
        } else if (ratio1to4 == true){
            //modulator set at a ratio 1:4 (-24 semitones from the carrier) creating a darker sound
            //in lower octaves it almost sounds like an LFO vibrato effect
            modulator = carrier/4;
        } else if (ratio1to2 == true){
            //modulator set at a ratio 1:2 (-12 semitones from the carrier) creating a more wooden quality to the sound
            modulator = carrier/2;
        } else if (ratio3to2 == true){
            //modulator set at a ratio 3:2 (7 semitones above the carrier) creating a more metalic sound
            modulator = carrier*1.5;
        } else if (ratio2to1 == true){
            //modulator set at a ratio 2:1 (12 semitones above the carrier) creating a cleaner sound similar to a square wave
            modulator = carrier*2;
        } else if (ratio3to1 == true){
            //modulator set at a ratio 3:1 (19 semitones above the carrier) creating a bell like quality
            modulator = carrier*3;
        } else if (ratio7to2 == true){
            //modulator set at a ratio 7:2 (21 semitones above the carrier) creating a classic FM sound
            modulator = carrier*3.5;
        } else if (ratio6to1 == true){
            //modulator set at a ratio 6:1 (31 semitones above the carrier) creating a tighter bell like sound
            modulator = carrier*6;
        }
        
        //and finally output the result if the single modulator is set to true!
        if (singleMod == true) {
            //output frequency is the same in all instances now with the carrier and modulator in the following fm float.(tested with 440Hz)
            fm = mySine.sinewave(myOtherSine.sinewave(carrier)*modulator);
        }

/*-----------------------------------------------------------------------*/
        //choices 2 now there are two modulators that affect the carrier output.
        if (twoModualtorsSame == true) {
            modulator = carrier;
            modulator2 = modulator;
        } else if (twoModulators1Ovtave == true){
            //combining two modulators..
            //modulator1 set to a 5:4 ratio above the carrier
            modulator = carrier*1.25;
            //modulator2 set to a 5:2 ratio above the carrier and 1 octave above the first modulator creating a fuller sound.
            modulator2 = carrier*2.5;
        } else if (twoModulatorsCrazy == true){
            //combining two modulators..
            //modulator1 set to a 5:4 ratio above the carrier
            modulator = 0.1;
            //modulator2 set to a 5:2 ratio above the carrier and 1 octave above the first modulator creating a fuller sound.
            modulator2 = 30;
        }
        
        if (parallelFM == true){
            //parrallel algorithm modulation using two modulators added together to affect the carrier.
            //output frequency is the same in all instances now with the carrier and parallelMod in the following fm float.(tested with 440Hz)
            float parallelMod = modulator+modulator2;
            fm = mySine.sinewave(myOtherSine.sinewave(carrier)*parallelMod);
        }
        
        if(cascadeFM == true){
            //cascade algorithm modulator 1 feeds modulator 2 which then feeds the carrier
            //modulator 1 affects the wave shape of the 2nd modulator that in turn effects the actual frequency wave of the carrier
            //set up to acheive the carrier to give the correct output is moved as seen in fm below. (tested with 440Hz)
            fm = mySine.sinewave(myOtherSine.sinewave(myLastSine.sinewave(modulator)*modulator2)*carrier);
        }
        
        
        /*-----------------------------------------------------------------------*/
        
        //an envelope setup to play the fm synth at regular intervals.
        //this first bit is just a basic metronome so we can hear what we're doing.
        double setNoteSpeed = 0.5 + mouseY/100;//set the note speed to be altered by the y mouse position
        
        currentCount=(int)timer.phasor(setNoteSpeed);//this sets up a metronome that ticks every 2 seconds
        
        if (lastCount!=currentCount) {//if we have a new timer int this sample, play the sound
            
             ADSR.trigger(0, adsrEnv[0]);//trigger the envelope from the start
             cout<< currentCount << " tick is .. \n, " << endl;
             lastCount=0;//set lastCount to 0
        }
        
		//generate a ramp for the fm sounds using some envelope code from Maxi examples
        ADSRout=ADSR.line(8,adsrEnv);//our ADSR env has 8 value/time pairs.
        float fmOut = 0;
       
        fmOut=VCF.lores(fm, 250+(ADSRout*100), 10);//send signal into VCF, using the ADSR as the filter cutoff
        
        //a variable to allow synth to be assigned to 1 or 0 for on or off
        float synthOut;
        //if blocks to keep track of user input to either turn synth on or off.
        if (synthPlay == true){
            synthOut = fmOut*1;
        }
        
        if(synthSilence == true){
            synthOut = fmOut*0;
        }
        
		output[i*nChannels    ] = outputs[0] +synthOut*0.4;//Keep the output down its loud!;
		output[i*nChannels + 1] = outputs[1] +synthOut*0.4;
        
        lastCount=0;;//set lastCOunt to 0 at the end of each sample to guarantee looping.

	}
}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){
	
	
	/* You can just grab this input and stick it in a double, then use it above to create output*/
	
	for (int i = 0; i < bufferSize; i++){
		
		/* you can also grab the data out of the arrays*/
		sampleIn = input[i];
		
	}
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if( key == 't' ){
        synthPlay = true;
        synthSilence = false;
       
    } else if( key == 'f' ){
        synthPlay = false;
        synthSilence = true;
    }

    //FM choices 1 for single modulator ratios..
    if ( key == '1' ){
        singleMod = true;
        ratio1to1 = true;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModualtorsSame = false;
        twoModulators1Ovtave = false;
        twoModulatorsCrazy = false;
    } else if( key == '2' ){
        singleMod = true;
        ratio1to1 = false;
        ratio1to4 = true;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModualtorsSame = false;
        twoModulators1Ovtave = false;
        twoModulatorsCrazy = false;
    } else if( key == '3' ){
        singleMod = true;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = true;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModualtorsSame = false;
        twoModulators1Ovtave = false;
        twoModulatorsCrazy = false;
    } else if( key == '4' ){
        singleMod = true;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = true;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModualtorsSame = false;
        twoModulators1Ovtave = false;
        twoModulatorsCrazy = false;
    } else if( key == '5' ){
        singleMod = true;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = true;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModualtorsSame = false;
        twoModulators1Ovtave = false;
        twoModulatorsCrazy = false;
    } else if( key == '6' ){
        singleMod = true;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = true;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModualtorsSame = false;
        twoModulators1Ovtave = false;
        twoModulatorsCrazy = false;
    } else if( key == '7' ){
        singleMod = true;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = true;
        ratio6to1 = false;
        twoModualtorsSame = false;
        twoModulators1Ovtave = false;
        twoModulatorsCrazy = false;
    } else if( key == '8' ){
        singleMod = true;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = true;
        twoModualtorsSame = false;
        twoModulators1Ovtave = false;
        twoModulatorsCrazy = false;
      
    }
    //FM choices 2 for two modulators
    //twoModulators1Ovtave, twoModualtorsSame, parallelFM, cascadeFM;
    if( key == 'q' ){
        parallelFM = true;
        cascadeFM = false;
        twoModualtorsSame = true;
        twoModulators1Ovtave = false;
        singleMod = false;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModulatorsCrazy = false;
        
    } else if( key == 'w' ){
        parallelFM = true;
        cascadeFM = false;
        twoModulators1Ovtave = true;
        twoModualtorsSame = false;
        singleMod = false;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModulatorsCrazy = false;
     
    } else if( key == 'e' ){
        parallelFM = false;
        cascadeFM = true;
        twoModulators1Ovtave = false;
        twoModualtorsSame = true;
        singleMod = false;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModulatorsCrazy = false;
       
    } else if( key == 'r' ){
        parallelFM = false;
        cascadeFM = true;
        twoModulators1Ovtave = true;
        twoModualtorsSame = false;
        singleMod = false;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        twoModulatorsCrazy = false;
       
    } else if( key == 'a' ){
        twoModulatorsCrazy = true;
        parallelFM = false;
        cascadeFM = true;
        twoModulators1Ovtave = false;
        twoModualtorsSame = false;
        singleMod = false;
        ratio1to1 = false;
        ratio1to4 = false;
        ratio1to2 = false;
        ratio3to2 = false;
        ratio2to1 = false;
        ratio3to1 = false;
        ratio7to2 = false;
        ratio6to1 = false;
        
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