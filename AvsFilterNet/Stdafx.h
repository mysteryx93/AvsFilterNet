// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef STDAFX_H
#define STDAFX_H


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "avisynth.h"

//using namespace Native;

#include "AvisynthException.h"
#include "FilterNotFoundException.h"
#include "VarNotFoundException.h"

#include "Enumerations.h"
#include "NativeString.h"
#include "VideoInfo.h"
#include "VideoFrame.h"
#include "Clip.h"
#include "AVSValue.h"
#include "ScriptEnvironment.h"
#include "AVSValueCollector.h"


#include "AvisynthFilter.h"
#include "AvisynthFilterNativeStub.h"


#include "AvisynthFilterClassAttribute.h"

#endif