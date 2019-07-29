//
// Created by ale on 19-7-22.
//

#ifndef GLESDEMO_EGL_MANAGER_H
#define GLESDEMO_EGL_MANAGER_H

#include "_android_debug.h"
//opengles, egl
#include <EGL/egl.h>
#include <EGL/eglplatform.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#include <glm/glm.hpp>
//opencv
#include <opencv2/opencv.hpp>

static EGLConfig eglConfig;
static EGLSurface eglSurface;
static EGLContext eglContext;
static EGLDisplay eglDisplay;

const int width = 500;
const int height = 500;

GLenum err;


void _egl_init()
{
    EGLint numConfigs;
    EGLBoolean SUCCEED;


    /* get an EGL display connection */
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if( eglDisplay == EGL_NO_DISPLAY)
        LOGE(" eglGetDisplay(): Unable to open connection to local windowing system");



    /* initialize the EGL display connection */
    EGLint majorVersion, minorVersion;
    SUCCEED = eglInitialize(eglDisplay, &majorVersion, &minorVersion);
    LOGE("EGL version: %d.%d", majorVersion, minorVersion);

    if(SUCCEED)
        ;//LOGI(" eglInitialize(): initialize EGL successful");
    else
        LOGE(" eglInitialize(): Unable to initialize EGL");



    /* get an appropriate EGL frame buffer configuration */
    static EGLint const configAttributes[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,// very important!
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
            EGL_RED_SIZE, 8,    EGL_GREEN_SIZE, 8,    EGL_BLUE_SIZE, 8,
            EGL_NONE };

    SUCCEED = eglChooseConfig(eglDisplay, configAttributes, &eglConfig, 1, &numConfigs);

    if(SUCCEED)
        ;//LOGI(" eglChooseConfig(): successful");
    else
        LOGE(" eglChooseConfig(): Unable to choose config");



    /* create an EGL rendering context */
    const EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3,    EGL_NONE};

    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttributes);

    if( eglContext == EGL_NO_CONTEXT)
        LOGE(" eglCreateContext(): Unable to create context");




    /* create an EGL pixel buffer surface */
    const EGLint surfaceAttributes[] = { EGL_WIDTH,width,    EGL_HEIGHT,height,    EGL_NONE };

    eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, surfaceAttributes);

    if( eglSurface == EGL_NO_SURFACE)
        LOGE(" eglCreatePbufferSurface(): Unable to create context");




    /* connect the context to the surface */
    SUCCEED = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    if(SUCCEED)
        ;//LOGI(" eglMakeCurrent(): successful");
    else
        LOGE(" eglMakeCurrent(): Unable to make current");


    const char* glesVersion = (const char*)glGetString(GL_VERSION);
    LOGE(" glGetString(): gles version: %s", glesVersion);
}


GLuint programObject;

void _egl_shader()
{
    const char*vertex_shader="#version 300 es                          \n"
                             "layout(location = 0) in vec4 vPosition;  \n"
                             "void main()                              \n"
                             "{                                        \n"
                             "   gl_Position = vPosition;              \n"
                             "}                                        \n";

    const char*fragment_shader="#version 300 es                                  \n"
                               "precision highp float;                           \n"
                               "out vec4 fragColor;                              \n"
                               "void main()                                      \n"
                               "{                                                \n"
                               "   fragColor = vec4 ( 0.0f, 6000.0f, 0.0f, 1.0f );  \n"
                               "}                                                \n";


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertex_shader,NULL);
    glCompileShader(vertexShader);


    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragment_shader,NULL);
    glCompileShader(fragmentShader);


    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    programObject = program;


}

GLuint frameBufferObject0;
void _egl_framebuffer()
{

    // framebuffer configuration
    glGenFramebuffers(1, &frameBufferObject0);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject0);
    //cout<<glGetError()<<endl;
    // create a color attachment texture
    GLuint colorTextureBuffer0;
    glGenTextures(1, &colorTextureBuffer0);
    glBindTexture(GL_TEXTURE_2D, colorTextureBuffer0);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,  GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,  GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureBuffer0, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    //更接近opengl原生数据结构，当不需要读取(比如glReadPixels)这些数据时效率非常快，一般用来depth test 和stencil test
    GLuint renderBufferObject0;
    glGenRenderbuffers(1, &renderBufferObject0);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject0);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject0); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOGE( "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" );

    LOGE(" _egl_framebuffer: fbo0 == %d", frameBufferObject0);
    for(; (err = glGetError()) != GL_NO_ERROR;)
    {
        LOGE(" _egl_framebuffer: err == %d", err);
    }


    glBindFramebuffer(GL_FRAMEBUFFER, 0);


}


GLuint vao;
void _egl_vao()
{
    std::vector<GLfloat> vertices = {  0.0f,  0.5f, 0.0f,
                                       -0.5f, -0.5f, 0.0f,
                                       0.5f, -0.5f, 0.0f};
    //GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}


void _egl_draw(cv::Mat & i_front, cv::Mat & i_back)
{



    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject0);
    glViewport(0, 0, width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram ( programObject );
    glBindVertexArray(vao);
    glDrawArrays ( GL_TRIANGLES, 0, 3 );

    if( (err = glGetError()) != GL_NO_ERROR)
    {
        LOGE(" _egl_draw:  after drawcall err == %d", err);
    }
    i_front = cv::Mat(height, width, CV_8UC4, cv::Scalar(128,128,128, 128));
    cv::imwrite("/sdcard/000GLESDemo/i_front_before.png", i_front);
    glReadPixels(0, 0, i_front.cols, i_front.rows, GL_RGBA, GL_UNSIGNED_BYTE, i_front.data);
    cv::flip(i_front, i_front, 0);
    cv::imwrite("/sdcard/000GLESDemo/i_front.png", i_front);



    if( (err = glGetError()) != GL_NO_ERROR)
    {
        LOGE(" _egl_draw:  after readpixels1, err == %d", err);
    }


    cv::Mat imageFloat = cv::Mat(height, width, CV_32FC4, cv::Scalar(0.5f, 0.5f, 0.5f));
    glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, imageFloat.data);
    cv::flip(imageFloat, imageFloat, 0);

    if( (err = glGetError()) != GL_NO_ERROR)
    {
        LOGE(" _egl_draw:  after readpixels2, err == %d", err);
    }


    double minV, maxV;
    cv::minMaxLoc(imageFloat, &minV, &maxV);
    LOGE("imageFloat.min max: %lf %lf", minV, maxV);


    imageFloat.convertTo(imageFloat, CV_8U);
    cv::cvtColor(imageFloat, imageFloat, cv::COLOR_RGBA2GRAY);
    cv::minMaxLoc(imageFloat, &minV, &maxV);
    LOGE("imageFloat.min max: %lf %lf", minV, maxV);
    if(cv::imwrite("/sdcard/000GLESDemo/imageFloat.png", imageFloat))
        ;//LOGE(" imwrite() : return success");
    else
        LOGE(" imwrite() : return failure");



    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if( (err = glGetError()) != GL_NO_ERROR)
    {
        LOGE(" _egl_draw:  after unbind, err == %d", err);
    }

}


void _egl_release()
{
    eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(eglDisplay, eglContext);
    eglDestroySurface(eglDisplay, eglSurface);
    eglTerminate(eglDisplay);

    eglDisplay = EGL_NO_DISPLAY;
    eglSurface = EGL_NO_SURFACE;
    eglContext = EGL_NO_CONTEXT;
}


#endif //GLESDEMO_EGL_MANAGER_H
