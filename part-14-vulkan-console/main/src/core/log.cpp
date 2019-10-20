#ifndef NDEBUG
#ifdef __ANDROID__
#include <android/log.h>
#endif
#endif

#include "log.hpp"
#include <iostream>

/*
 * Our logger is very basic, and only accepts strings as input. We will wrap a debug check
 * so for release builds nothing will actually happen.
 */
void ast::log(const std::string& tag, const std::string& message)
{
#ifndef NDEBUG
#ifdef __ANDROID__
    // Android doesn't emit the standard output streams by default, so instead we will
    // pump the logging through the Android Logcat system. If you enter 'a-simple-triangle'
    // as the Logcat filter you will see only these logging statements.
    __android_log_print(ANDROID_LOG_DEBUG, "a-simple-triangle", "%s: %s", tag.c_str(), message.c_str());
#else
    // Other platforms can just use the standard output mechanics.
    std::cout << tag << ": " << message << std::endl;
#endif
#endif
}

void ast::log(const std::string& tag, const std::string& message, const std::exception& error)
{
#ifndef NDEBUG
    std::string output = message + " Exception message was: " + std::string{error.what()};
    ast::log(tag, output);
#endif
}
