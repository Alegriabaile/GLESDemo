//
// Created by ale on 19-7-22.
//

#ifndef GLESDEMO_ANDROID_DEBUG_H
#define GLESDEMO_ANDROID_DEBUG_H

#include <jni.h>
#include <android/log.h>


#ifndef TAG_MY_LOG
#define TAG_MY_LOG
#define TAG "...................TAG_MY_LOG..................."
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
//#undef TAG
#endif


#endif //GLESDEMO_ANDROID_DEBUG_H
