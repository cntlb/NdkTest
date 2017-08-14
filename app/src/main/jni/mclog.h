//
// Created by jmu on 17-8-14.
//

#ifndef NDKTEST_LOG_H_H
#define NDKTEST_LOG_H_H
#include <android/log.h>

#define LOGE(fmt, argv...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##argv);

#endif //NDKTEST_LOG_H_H
