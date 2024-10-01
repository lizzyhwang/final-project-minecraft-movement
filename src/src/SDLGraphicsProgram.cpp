#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h, std::string f):m_screenWidth(w),m_screenHeight(h),filepath(f){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;
	// Render flag

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Lab",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_screenWidth,
                                m_screenHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();


    // Setup our objects
    m_object = new Object(filepath);
    // Make a textured quad
    m_object->MakeTexturedQuad();
    
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    // Reclaim all of our objects
    delete m_object;

    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;

    // Setup our OpenGL State machine
    // TODO: Read this
    // The below command is new!
    // What we are doing, is telling opengl to create a depth(or Z-buffer) 
    // for us that is stored every frame.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); 
	return success;
}


// Update OpenGL
void SDLGraphicsProgram::Update(){
    m_object->GetTransform().LoadIdentity();
    m_object->GetTransform().Translate(0.0f,0.0f,0.0f);
    m_object->GetTransform().Scale(1.0f,1.0f,1.0f);
    m_object->Update(m_screenWidth,m_screenHeight);

    static float rot = 0;
    rot+=0.05;
    if(rot>360){rot=0;}

    //std::cout << rot << std::endl;

    Camera::Instance().UpdateHeight(-9.8f);
    // update jump time variables
    Camera::Instance().UpdateJumpTimes();

    // if (Camera::Instance().isJumping()) {
    //     Camera::Instance().UpdateHeight(-9.8f);
    //     // update jump time variables
    //     Camera::Instance().UpdateJumpTimes();
    // }

}



// Render
// The render function gets called once per loop
void SDLGraphicsProgram::Render(){
    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor( 0.2f, 0.2f, 0.2f, 1.f );
    // TODO: Read this
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	if (wireframeMode) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Render our object
    m_object->Render(wireframeMode);
    // Note: This line is equivalent to:
    // (*m_object).Render(); // But the easier style
    // is to use the '->' which dereferences and then
    // selects the field/function with the '.' operator.
   
    SDL_Delay(50); 
}


//Loops forever!
void SDLGraphicsProgram::Loop(){
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();
    // While application is running

    // update time variables
    // updateDeltaTime();
    // std::cout << curTime << std::endl;

	// Set the camera speed for how fast we move.
    float cameraSpeed = 0.03f;
    Camera::Instance().SetCameraEyePosition(0.0f,0.35f,0.0f);

    while(!quit){
     	 //Handle events on queue
		while(SDL_PollEvent( &e ) != 0){
        	// User posts an event to quit
	        // An example is hitting the "x" in the corner of the window.
    	    if(e.type == SDL_QUIT){
        		quit = true;
	        }
			// Handle keyboard input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                Camera::Instance().MouseLook(mouseX, mouseY);
            }
            switch(e.type){
                // Handle keyboard presses
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_q:
                            quit = true;
                            break;
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
						case SDLK_1:
							wireframeMode = !wireframeMode;
							break;
                        case SDLK_LEFT:
                            Camera::Instance().MoveLeft(cameraSpeed);
                            break;
                        case SDLK_a:
                            Camera::Instance().MoveLeft(cameraSpeed);
                            break;                            
                        case SDLK_RIGHT:
                            Camera::Instance().MoveRight(cameraSpeed);
                            break;
                        case SDLK_d:
                            Camera::Instance().MoveRight(cameraSpeed);
                            break;                            
                        case SDLK_UP:
                            Camera::Instance().MoveForward(cameraSpeed);
                            break;
                        case SDLK_w:
                            Camera::Instance().MoveForward(cameraSpeed);
                            break;                            
                        case SDLK_DOWN:
                            Camera::Instance().MoveBackward(cameraSpeed);
                            break;
                        case SDLK_s:
                            Camera::Instance().MoveBackward(cameraSpeed);
                            break;                            
                        case SDLK_RSHIFT:
                            Camera::Instance().MoveUp(cameraSpeed);
                            break;
                        case SDLK_LSHIFT:
                            Camera::Instance().MoveDown(cameraSpeed);
                            break;
                        case SDLK_SPACE:
                            if (!Camera::Instance().isJumping()) {
                                std::cout << "space bar pressed!" << std::endl;
                                Camera::Instance().Jump(1.0f);
                                break;
                            }
                    }
                break;
            }
      	} // End SDL_PollEvent loop.

		// Update our scene
		Update();
		// Render using OpenGL
	    Render(); 	// TODO: potentially move this depending on your logic
					// for how you handle drawing a triangle or rectangle.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
		
    }

    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}