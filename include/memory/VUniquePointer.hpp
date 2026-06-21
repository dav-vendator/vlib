/*
Name: VUniquePointer
Description: A smart unique pointer
Author: Dav Vendator
*/

#ifndef VLIB_VUniquePointer_HPP
#define VLIB_VUniquePointer_HPP

#include <utility>
#include <cstddef>

namespace Vlib{
    template<typename T>
    class VUniquePointer{

            T* ptr_;
            
        public:
            VUniquePtr();
            explicit VUniquePtr(T* ptr);

            ~VUniquePtr();

            VUniquePtr(const VUniquePtr&) = delete;
            VUniquePtr& operator=(const VUniquePtr&) = delete;

            VUniquePtr(VUniquePtr&& other) noexcept;
            VUniquePtr& operator=(VUniquePtr&& other) noexcept;

            T* get() const noexcept;

            T& operator*() const;
            T* operator->() const noexcept;

            explicit operator bool() const noexcept;
    };
}

#endif