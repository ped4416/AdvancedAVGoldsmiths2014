#include "ofMain.h"
#include "testApp.h"
#ifdef TARGET_OPENGLES
// import the fancy new renderer
#include "ofGlProgrammableRenderer.h"
#endif
//========================================================================
int main( ){
    
    // say that we're going to *use* the fancy new renderer
    ofSetLogLevel(OF_LOG_VERBOSE);
    #ifdef TARGET_OPENGLES
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    #endif
    
	ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
