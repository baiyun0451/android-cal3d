#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include "global.h"

#include "GameState.h"
#include "ARGameProgram.h"
#include "Sprite.h"

#include "model.h"
#include <vector>

// Constants:
static const float kObjectScale	= 10.f;

class RenderState : public GameState
{
	public:
        RenderState();
        ~RenderState();
		bool onEnter();
		bool onExit();
		bool onPause();
		bool onResume();

		void onRender();
        void onIdle();
		bool onTouch(float x,float y,int touchMode,int pointerID,int pointerCount);
		bool onRendererChange(int width,int height);
		bool loadTexture(const std::string& strFilename, int& pId);
    private:
        std::vector<Model *> m_vectorModel;
        unsigned int m_currentModel;
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_lastTick;
        float m_fpsDuration;
        bool m_bPaused;
        int m_fpsFrames;
        int m_fps;
        float m_averageCPUTime;
        float m_distance;
        float m_tiltAngle;
        float m_twistAngle;
        std::string m_strDatapath;
        std::string m_strCal3D_Datapath;

        Sprite * mFPSSprite[3];
        Sprite * mLogoSprite;
};
RenderState::RenderState()
{
    m_currentModel = 0;
    m_width = Program::getInstance()->mWidth;
    m_height = Program::getInstance()->mHeight;
    m_fpsDuration = 0;
    m_fps = 0;
    m_fpsFrames = 0;
    m_strDatapath = "";
    m_bPaused = false;
    m_averageCPUTime = 0;
    m_distance = 770.0f;
    m_tiltAngle = -70.0f;
    m_twistAngle = -45.0f;

}
RenderState::~RenderState()
{
    delete [] mFPSSprite;
}bool RenderState::onEnter()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_strDatapath = Program::getInstance()->mReadPath;
    m_strCal3D_Datapath = Program::getInstance()->mReadPath;

    int textureID;
    if(!loadTexture((m_strDatapath + "fps.raw").c_str(), textureID))
        LOG(("Fail to load texture : " + m_strDatapath + "fps.raw").c_str());

    for(int digitId = 2; digitId >= 0; digitId--)
    {
        int x = 29 + digitId * 16;
        float pos [] = {x, 94};
        float size [] = {16, 16};
        mFPSSprite[digitId] = new Sprite(pos, size, textureID);
    }

    if(!loadTexture((m_strDatapath + "logo.raw").c_str(), textureID))
        LOG(("Fail to load texture : " + m_strDatapath + "logo.raw").c_str());

    float pos [] = {0,0};
    float size [] = {128,128};
    mLogoSprite = new Sprite(pos, size, textureID);

    LOG(("File path : " + m_strDatapath).c_str());
    // initialize models
    Model *pModel;

    // load 'cally' model
    LOG("Loading 'cally' model ...");
    pModel = new Model();

    //if (m_strCal3D_Datapath != "")
    pModel->setPath( m_strCal3D_Datapath + "/" + "cally/" );

    if(!pModel->onInit(m_strDatapath + "cally.cfg"))
    {
        delete pModel;
        LOG("Model initialization failed! (cally)");
        return false;
    }

    m_vectorModel.push_back(pModel);
    LOG("Initialized");

	return true;
}
bool RenderState::onExit()
{
	return true;
}
bool RenderState::onPause()
{
	return true;
}
bool RenderState::onResume()
{
	return true;
}
static float getTime()
{
    //return milli seconds
    struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_sec*1000 + now.tv_nsec / 1000000.f;
}
void RenderState::onIdle()
{
    // get the current tick value
    unsigned int tick;
    tick = getTime();

    // calculate the amount of elapsed seconds
    float elapsedSeconds;
    elapsedSeconds = (float)(tick - m_lastTick)/1000.0f;

    // adjust fps counter
    m_fpsDuration += elapsedSeconds;
    if(m_fpsDuration >= 1.0f)
    {
        m_fps = (int)((float)m_fpsFrames / m_fpsDuration);
        m_fpsDuration = 0.0f;
        m_fpsFrames = 0;
    }

    static float start;
    static float firstTime, lastTime;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    start = getTime();

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

    float stop = getTime();

    stop -= start;
    static float cumul = 0;
    cumul += stop;

    if (!bFirst) {
        m_averageCPUTime = cumul / float(lastTime - firstTime) * 100;
    }
    bFirst = false;
    m_lastTick = tick;
}
void RenderState::onRender()
{
    onIdle();

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

    glTranslatef(0.0f, 0.0f, -m_distance * renderScale);
    glRotatef(m_tiltAngle, 1.0f, 0.0f, 0.0f);
    glRotatef(m_twistAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, -90.0f * renderScale);


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

    mLogoSprite->onRender();

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
	m_fpsFrames++;
}

bool RenderState::onTouch(float x,float y,int touchMode,int pointerID,int pointerCount)
{
	return true;
}

bool RenderState::onRendererChange(int width,int height)
{
}

bool RenderState::loadTexture(const std::string& strFilename, int& pId)
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
      std::cerr << "Texture file '" << strFilename << "' not found." << std::endl;
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
      std::cerr << "Error while readinf from texture file '" << strFilename << "'." << std::endl;
      return false;
    }

    // allocate a temporary buffer to hold the texture data
    unsigned char *pBuffer;
    pBuffer = new unsigned char[width * height * depth];
    if(pBuffer == 0)
    {
      std::cerr << "Memory allocation for texture file '" << strFilename << "' failed." << std::endl;
      return false;
    }

    // load the texture
    file.read((char *)pBuffer, width * height * depth);
    file.close();

    // generate the texture
    GLuint temp[1];
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, temp);
    glBindTexture(GL_TEXTURE_2D, temp[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, (depth == 3) ? GL_RGB : GL_RGBA, width, height, 0, (depth == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
    pId = temp[0];
    LOG(("Loaded Texture - " + strFilename).c_str());
  }


  return true;
}
#endif

