#pragma once

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#include <OpenGLES/ES2/gl.h>
#else
#include <OpenGL/gl3.h>
#endif
#elif __ANDROID__
#include <GLES2/gl2.h>
#endif
