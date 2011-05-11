//----------------------------------------------------------------------------//
// demo.cpp                                                                   //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#if defined(_MSC_VER) && _MSC_VER <= 0x0600
#pragma warning(disable : 4786)
#endif

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "ARGameProgram.h"
#include "demo.h"
#include "model.h"
#include "menu.h"
#include "tga.h"
#include "Utils.h"


//----------------------------------------------------------------------------//
// The one and only Demo instance                                             //
//----------------------------------------------------------------------------//

Demo theDemo;

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

Demo::Demo()
{
  m_strDatapath = "";
  m_strCal3D_Datapath = "";
  //m_width = 640;
  //m_height = 480;
  m_bFullscreen = false;
  m_fpsDuration = 0.0f;
  m_fpsFrames = 0;
  m_fps = 0;
  m_mouseX = 0;
  m_mouseY = 0;
  m_tiltAngle = -70.0f;
  m_twistAngle = -45.0f;
  m_distance = 750.0f;
  m_bLeftMouseButtonDown = false;
  m_bRightMouseButtonDown = false;
  m_lastTick = Utils::getCurrentTime();
  m_currentModel = 0;
  m_bPaused = false;
  m_bOutputAverageCPUTimeAtExit = false;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

Demo::~Demo()
{
}

//----------------------------------------------------------------------------//
// Get the demo caption                                                       //
//----------------------------------------------------------------------------//

std::string Demo::getCaption()
{
  return "- Cal3D The Cally Demo -";
}

//----------------------------------------------------------------------------//
// Get the filepath to the data files                                         //
//----------------------------------------------------------------------------//

std::string Demo::getDatapath()
{
  return m_strDatapath;
}


//----------------------------------------------------------------------------//
// Get the current model                                                      //
//----------------------------------------------------------------------------//

Model *Demo::getModel()
{
  return m_vectorModel[m_currentModel];
}

//----------------------------------------------------------------------------//
// Get the window width                                                       //
//----------------------------------------------------------------------------//

int Demo::getWidth()
{
  return m_width;
}

//----------------------------------------------------------------------------//
// Load a texture from a given file                                           //
//----------------------------------------------------------------------------//

bool Demo::loadTexture(const std::string& strFilename, GLuint& pId)
{
  // initialize the id
  pId = 0;

  if (stricmp(strrchr(strFilename.c_str(),'.'),".raw")==0)
  {
  	 // open the texture file
  	 std::ifstream file;
  	 file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
    if(!file)
    {
      LOG(("Texture file '" + strFilename + "' not found.").c_str());
      return false;
    }

    // read the width, height and depth of the texture
    int width;
    file.read((char *)&width, 4);
    int height;
    file.read((char *)&height, 4);
    int depth;
    file.read((char *)&depth, 4);

    // check if an error has happend
    if(!file)
    {
      LOG(("Error while readinf from texture file '" + strFilename + "'.").c_str());
      return false;
    }

    // allocate a temporary buffer to hold the texture data
    unsigned char *pBuffer;
    pBuffer = new unsigned char[width * height * depth];
    if(pBuffer == 0)
    {
      LOG(("Memory allocation for texture file '" + strFilename + "' failed.").c_str());
      return false;
    }

    // load the texture
    file.read((char *)pBuffer, width * height * depth);
    file.close();

    // generate the texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &pId);
    glBindTexture(GL_TEXTURE_2D, pId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, (depth == 3) ? GL_RGB : GL_RGBA, width, height, 0, (depth == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
  }
  else if (stricmp(strrchr(strFilename.c_str(),'.'),".tga")==0)
  {

	  CTga *Tga;
	  Tga = new CTga();
	  if(Tga->ReadFile(strFilename.c_str())==0)
	  {
		  Tga->Release();
		  return false;
	  }

	  if(Tga->Bpp()!=32)
	  {
		  Tga->Release();
		  return false;
	  }

     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

     glGenTextures(1, &pId);

     glBindTexture(GL_TEXTURE_2D, pId);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Tga->GetSizeX(), Tga->GetSizeY(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Tga->GetPointer() );

	  Tga->Release();
  }


  return true;
}

//----------------------------------------------------------------------------//
// Make next model the current one                                            //
//----------------------------------------------------------------------------//

void Demo::nextModel()
{
  m_currentModel = (m_currentModel + 1) % m_vectorModel.size();
}

//----------------------------------------------------------------------------//
// Create the demo                                                            //
//----------------------------------------------------------------------------//
/*
bool Demo::onCreate(int argc, char *argv[])
{
  // show some information
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << "|                      The Cally Demo 2.10.0                     |" << std::endl;
  std::cout << "|       (C) 2001, 2002, 2003 Bruno 'Beosil' Heidelberger         |" << std::endl;
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << "| This program is free software; you can redistribute it and/or  |" << std::endl;
  std::cout << "| modify it under the terms of the GNU General Public License as |" << std::endl;
  std::cout << "| published by the Free Software Foundation; either version 2 of |" << std::endl;
  std::cout << "| the License, or (at your option) any later version.            |" << std::endl;
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << std::endl;

  // parse the command line arguments
  int arg;
  for(arg = 1; arg < argc; arg++)
  {
    // check for fullscreen flag
    if(strcmp(argv[arg], "--fullscreen") == 0) m_bFullscreen = true;
    // check for window flag
    else if(strcmp(argv[arg], "--window") == 0) m_bFullscreen = false;
    // check for dimension flag
    else if((strcmp(argv[arg], "--dimension") == 0) && (argc - arg > 2))
    {
      m_width = atoi(argv[++arg]);
      m_height = atoi(argv[++arg]);
      if((m_width <= 0) || (m_height <= 0))
      {
        std::cerr << "Invalid dimension!" << std::endl;
        return false;
      }
    }
    else if((strcmp(argv[arg], "--data") == 0) && (argc - arg > 1))
    {
       m_strCal3D_Datapath = argv[++arg];
    }
    else if((strcmp(argv[arg], "--bench") == 0))
		{
			 m_bOutputAverageCPUTimeAtExit = true;
		}
    else
    {
      std::cerr << "Usage: " << argv[0] << " [--fullscreen] [--window] [--dimension width height] [--data path_to_cal3d_data]" << std::endl;
      return false;
    }
  }

  return true;
}
*/
//----------------------------------------------------------------------------//
// Handle an idle event                                                       //
//----------------------------------------------------------------------------//

void Demo::onIdle()
{
  // get the current tick value
  unsigned int tick;
  tick = Utils::getCurrentTime();

  // calculate the amount of elapsed seconds
  float elapsedSeconds;
  elapsedSeconds = (float)(tick - m_lastTick) / 1000.0f;

  // adjust fps counter
  m_fpsDuration += elapsedSeconds;
  if(m_fpsDuration >= 1.0f)
  {
    m_fps = (int)((float)m_fpsFrames / m_fpsDuration);
    m_fpsDuration = 0.0f;
    m_fpsFrames = 0;
  }

	static double start;
	static double firstTime, lastTime;
	start = Utils::getCurrentTime();

	static bool bFirst = true;
	if (bFirst) {
		firstTime = start;
	}
	else {
		lastTime = start;
	}

  // update the current model
  if(!m_bPaused)
  {
		//for (int i = 0; i < 10; i++)
			m_vectorModel[m_currentModel]->onUpdate(elapsedSeconds);
  }

	double stop = Utils::getCurrentTime();

	stop -= start;
	static double cumul = 0;
	cumul += stop;

	if (!bFirst) {
		m_averageCPUTime = cumul / float(lastTime - firstTime) * 100;
	}
	bFirst = false;

  // update the menu
  theMenu.onUpdate(elapsedSeconds);

  // current tick will be last tick next round
  m_lastTick = tick;
}

//----------------------------------------------------------------------------//
// Initialize the demo                                                        //
//----------------------------------------------------------------------------//

bool Demo::onInit()
{
  m_width = Program::getInstance()->mWidth;
  m_height = Program::getInstance()->mHeight;
  m_strDatapath = Program::getInstance()->mReadPath;
  m_strCal3D_Datapath = Program::getInstance()->mReadPath;
  // load the cursor texture
  std::string strFilename;
  //strFilename = m_strDatapath + "cursor.raw";

  //if(!loadTexture(strFilename, m_cursorTextureId)) return false;

  // load the logo texture
  strFilename = m_strDatapath + "logo.raw";

  if(!loadTexture(strFilename, m_logoTextureId)) return false;

  float pos [] = {0,0};
  float size [] = {128,128};
  mLogoSprite = new Sprite(pos, size, m_logoTextureId);

  // load the fps texture
  strFilename = m_strDatapath + "fps.raw";

  if(!loadTexture(strFilename, m_fpsTextureId)) return false;

  for(int digitId = 2; digitId >= 0; digitId--)
  {
      int x = 29 + digitId * 16;
      float pos [] = {x, 94};
      float size [] = {16, 16};
      mFPSSprite[digitId] = new Sprite(pos, size, m_fpsTextureId);
  }

  // initialize models
  Model *pModel;

  // load 'cally' model
  LOG("Loading 'cally' model ...");

  pModel = new Model();

  if (m_strCal3D_Datapath != "")
    pModel->setPath( m_strCal3D_Datapath + "/" + "cally/" );

  if(!pModel->onInit(m_strDatapath + "cally.cfg"))
  {
    delete pModel;
    LOG("Model initialization failed! (cally)");
    return false;
  }

  m_vectorModel.push_back(pModel);

  // initialize menu
  if(!theMenu.onInit(m_width, m_height))
  {
    LOG("Menu initialization failed!");
    return false;
  }

  // we're done
  LOG("Initialization done.");
  return true;
}

//----------------------------------------------------------------------------//
// Handle a key event                                                         //
//----------------------------------------------------------------------------//

void Demo::onKey(unsigned char key, int x, int y)
{
  // test for quit event
  if((key == 27) || (key == 'q') || (key == 'Q')) exit(0);

  // test for pause event
  if(key == ' ') m_bPaused = !m_bPaused;

  // let the menu handle the rest
  theMenu.onKey(key, x, y);
}

//----------------------------------------------------------------------------//
// Handle a mouse button down event                                           //
//----------------------------------------------------------------------------//

void Demo::onMouseButtonDown(int button, int x, int y)
{
  // let the meu handle mouse buttons first
  if(!theMenu.onMouseButtonDown(button, x, y))
  {
    // update mouse button states
    if(button == 0)  //left
    {
      m_bLeftMouseButtonDown = true;
    }

    if(button == 1) //right
    {
      m_bRightMouseButtonDown = true;
    }
  }

  // update internal mouse position
  m_mouseX = x;
  m_mouseY = y;
}

//----------------------------------------------------------------------------//
// Handle a mouse button up event                                             //
//----------------------------------------------------------------------------//

void Demo::onMouseButtonUp(int button, int x, int y)
{
  // let the meu handle mouse buttons first
  if(!theMenu.onMouseButtonUp(button, x, y))
  {
    // update mouse button states
    if(button == 0) //left
    {
      m_bLeftMouseButtonDown = false;
    }

    if(button == 1) //right
    {
      m_bRightMouseButtonDown = false;
    }
  }

  // update internal mouse position
  m_mouseX = x;
  m_mouseY = y;
}

//----------------------------------------------------------------------------//
// Handle a mouse move event                                                  //
//----------------------------------------------------------------------------//

void Demo::onMouseMove(int x, int y)
{
  // let the meu handle mouse buttons first
  if(!theMenu.onMouseMove(x, y))
  {
/* DEBUG-CODE
if(m_bLeftMouseButtonDown && m_bRightMouseButtonDown)
{
  m_vectorModel[m_currentModel]->adjustSphere((float)(x - m_mouseX), (float)(y - m_mouseY), 0.0f);
}
else
{
*/
    // update twist/tilt angles
    if(m_bLeftMouseButtonDown)
    {
      // calculate new angles
      m_twistAngle += (float)(x - m_mouseX);
      m_tiltAngle -= (float)(y - m_mouseY);
    }

    // update distance
    if(m_bRightMouseButtonDown)
    {
      // calculate new distance
      m_distance -= (float)(y - m_mouseY) ;
      if(m_distance < 0.0f) m_distance = 0.0f;
    }
/* DEBUG-CODE
}
*/
  }

  // update internal mouse position
  m_mouseX = x;
  m_mouseY = y;
}

//----------------------------------------------------------------------------//
// Render the current scene                                                   //
//----------------------------------------------------------------------------//

void Demo::onRender()
{
  // clear all the buffers
  glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // get the render scale of the model
  float renderScale;
  renderScale = m_vectorModel[m_currentModel]->getRenderScale();

  // set the projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float ratio = (float) m_width / m_height;
  glFrustumf(-ratio, ratio, -1, 1, renderScale * 10.0, renderScale * 5000.0);
  //gluPerspective(45.0f, (GLdouble)m_width / (GLdouble)m_height, renderScale * 50.0, renderScale * 5000.0);

  // set the model transformation
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // set the light position and attributes
  const GLfloat lightPosition[] = { 1.0f, -1.0f, 1.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  const GLfloat lightColorAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightColorAmbient);
  const GLfloat lightColorDiffuse[] = { 0.52f, 0.5f, 0.5f, 1.0f };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColorDiffuse);
  const GLfloat lightColorSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightColorSpecular);

  // set camera position
  glTranslatef(0.0f, 0.0f, -m_distance * renderScale);
  glRotatef(m_tiltAngle, 1.0f, 0.0f, 0.0f);
  glRotatef(m_twistAngle, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.0f, 0.0f, -90.0f * renderScale);

  // render model
  m_vectorModel[m_currentModel]->onRender();

  // switch to orthogonal projection for 2d stuff
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrthof(0, (GLfloat)m_width, 0, (GLfloat)m_height, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // we will render some alpha-blended textures
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // render menu
  theMenu.onRender();

  // render logo
  mLogoSprite->onRender();

  // render fps
  int digit = m_fps;
  for(int digitId = 2; digitId >= 0; digitId--)
  {
      float * tempTextCoord = mFPSSprite[digitId]->getTextureCoord();
      float tx = (float)(digit % 10) * 0.0625f;
      tempTextCoord[0] = tx;              tempTextCoord[1] = 1.0f;
      tempTextCoord[2] = tx + 0.0625f;    tempTextCoord[3] = 1.0f;
      tempTextCoord[4] = tx;              tempTextCoord[5] = 0.0f;
      tempTextCoord[6] = tx + 0.0625f;    tempTextCoord[7] = 0.0f;
      mFPSSprite[digitId]->onRender();

      digit /= 10;
  }
    /*      int error = glGetError();
    if(error == GL_NO_ERROR)
        LOG("No Error");
    else if(error == GL_INVALID_ENUM)
        LOG("Invalid enum");
    else if(error == GL_INVALID_VALUE)
        LOG("Invalid value");
    else if(error == GL_INVALID_OPERATION)
        LOG("Invalid operation");
    else if(error == GL_STACK_OVERFLOW)
        LOG("overflow");
    else if(error == GL_STACK_UNDERFLOW)
        LOG("underflow");
    else if(error == GL_OUT_OF_MEMORY)
        LOG("out of memory");
    else
        LOG("Unknown error");*/
  m_fpsFrames++;
}

//----------------------------------------------------------------------------//
// Shut the demo down                                                         //
//----------------------------------------------------------------------------//

void Demo::onShutdown()
{
  // shut the menu down
  delete [] mFPSSprite;
  delete mLogoSprite;
  theMenu.onShutdown();

  // shut all models down
  unsigned int modelId;
  for(modelId = 0; modelId < m_vectorModel.size(); modelId++)
  {
    m_vectorModel[modelId]->onShutdown();
    delete m_vectorModel[modelId];
  }

	if (m_bOutputAverageCPUTimeAtExit)
		std::cout << m_averageCPUTime;
}

//----------------------------------------------------------------------------//
// Set the dimension                                                          //
//----------------------------------------------------------------------------//

void Demo::setDimension(int width, int height)
{
  // store new width and height values
  m_width = width;
  m_height = height;

  // set new viewport dimension
  glViewport(0, 0, m_width, m_height);

  // adjust menu
  theMenu.onResize(width, height);
}

//----------------------------------------------------------------------------//
