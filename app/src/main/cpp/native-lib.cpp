#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>

#include "_android_debug.h"
#include "_egl_manager.h"


extern "C" JNIEXPORT void JNICALL
Java_i3d_glesdemo_MainActivity_getImage(
        JNIEnv *env, jclass, jobject bitmap) {

    cv::Mat image;
    _egl_init();

    _egl_framebuffer();
    _egl_shader();
    _egl_vao();
    _egl_draw(image, image);
    _egl_release();

    //    cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);

    cv::Mat bmp(500, 500, CV_8UC4);
    AndroidBitmap_lockPixels(env, bitmap, (void **)&bmp.data);


    //image.copyTo(bmp);
    if(image.channels() == 3)
        cv::cvtColor(image, bmp, cv::COLOR_BGR2BGRA);
    else if(image.channels() == 1)
        cv::cvtColor(image, bmp, cv::COLOR_GRAY2BGRA);
    else
        image.copyTo(bmp);


    AndroidBitmap_unlockPixels(env, bitmap);
}
