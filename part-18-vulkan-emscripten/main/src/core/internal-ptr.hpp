#pragma once

#include <memory>

/*
 * This utility allows us to adopt a very basic form of the PIMPL (Pointer to IMPLementation)
 * pattern in C++, based of the information discussed at this blog post:
 *
 * http://oliora.github.io/2015/12/29/pimpl-and-rule-of-zero.html
 *
 * It allows us to have an 'internal' object that forms the private implementation of a class
 * allowing us to hide all the private guts of a class from its public API. Because it is
 * based on the std::unique_ptr, you can be sure it will destroy itself automatically when it
 * goes out of scope. Be aware though, that because it is based on the std::unique_ptr type,
 * only move semantics are permitted. For the purposes of this code base, this design decision
 * is acceptable and doesn't necessarily feel like a bad thing.
 *
 * Here is a basic example of how it is used.
 *
 * // example.hpp
 *
 * #include "internal-ptr.hpp"
 *
 * struct Example
 * {
 *    Example();
 *
 *    void sayHello();
 *
 * private:
 *    struct Internal;
 *    ast::internal_ptr<Internal> internal;
 * };
 *
 * // example.cpp
 *
 * struct Example::Internal
 * {
 *    void sayHello()
 *    {
 *       // Hello!
 *    }
 * };
 *
 * Example::Example() : internal(ast::make_internal_ptr<Internal>()) {}
 *
 * void Example::sayHello()
 * {
 *    internal->sayHello();
 * }
 *
 */

namespace ast
{
    namespace internal_ptr_deleter
    {
        /*
         * The custom deleter template is needed to ensure that an 'internal_ptr'
         * based object will self destroy upon going out of scope.
         */
        template <class T>
        void deleter(T* victim)
        {
            delete victim;
        }
    } // namespace internal_ptr_deleter

    /*
     * This is the template that gives us the 'internal_ptr' name and maps it
     * to be a std::unique_ptr with a custom deleter.
     */
    template <class T, class Deleter = void (*)(T*)>
    using internal_ptr = std::unique_ptr<T, Deleter>;

    /*
     * This factory method will take any required arguments and forward them into a
     * new instance of the 'internal_ptr' type and wires in the custom deleter on the way.
     */
    template <class T, class... Args>
    inline internal_ptr<T> make_internal_ptr(Args&&... args)
    {
        return internal_ptr<T>(new T(std::forward<Args>(args)...), &internal_ptr_deleter::deleter<T>);
    }
} // namespace ast
