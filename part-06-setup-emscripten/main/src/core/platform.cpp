#include "platform.hpp"

ast::Platform ast::getCurrentPlatform()
{
#if defined(__EMSCRIPTEN__)
    return ast::Platform::emscripten;
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
    return ast::Platform::ios;
#else
    return ast::Platform::mac;
#endif
#elif __ANDROID__
    return ast::Platform::android;
#endif
}
