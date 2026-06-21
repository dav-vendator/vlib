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
            VUniquePointer() noexcept;
            explicit VUniquePointer(T* ptr) noexcept;

            ~VUniquePointer();

            VUniquePointer(const VUniquePointer&) = delete;
            VUniquePointer& operator=(const VUniquePointer&) = delete;

            VUniquePointer(VUniquePointer&& other) noexcept;
            VUniquePointer& operator=(VUniquePointer&& other) noexcept;

            T* get() const noexcept;

            T& operator*() const;
            T* operator->() const noexcept;

            explicit operator bool() const noexcept;

            T* release() noexcept;
            void reset(T* ptr) noexcept;
            void swap(VUniquePointer& other) noexcept;

            bool operator==(std::nullptr_t) const noexcept;

    };

    template<typename T>
    VUniquePointer<T>::VUniquePointer() noexcept: ptr_(nullptr)
    {}

    template<typename T>
    VUniquePointer<T>::VUniquePointer(T* ptr) noexcept: ptr_(ptr)
    {}

    template<typename T>
    VUniquePointer<T>::~VUniquePointer(){
        delete this->ptr_;
    }

    template<typename T>
    VUniquePointer<T>::VUniquePointer(VUniquePointer&& other) noexcept : ptr_(other.ptr_){
        other.ptr_ = nullptr;
    }
    
    template<typename T>
    VUniquePointer<T>& VUniquePointer<T>::operator=(VUniquePointer&& other) noexcept{
        if (this == &other)
            return *this;

        delete this->ptr_;
        this->ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }

    template<typename T>
    T* VUniquePointer<T>::get() const noexcept{
        return this->ptr_;
    }

    template<typename T>
    T& VUniquePointer<T>::operator*() const{
        return *(this->ptr_);
    }

    template<typename T>
    T* VUniquePointer<T>::operator->() const noexcept{
        return this->ptr_;
    }

    template<typename T>
    VUniquePointer<T>::operator bool() const noexcept{
        return this->ptr_ != nullptr;
    }

    template<typename T>
    T* VUniquePointer<T>::release() noexcept{
        T* temp = this->ptr_;
        this->ptr_ = nullptr;
        return  temp;
    }

    template<typename T>
    void VUniquePointer<T>::reset(T* ptr) noexcept{
        if (this->ptr_ == ptr)
            return;

        delete this->ptr_;
        this->ptr_ = ptr;
    }

    template<typename T>
    void VUniquePointer<T>::swap(VUniquePointer& other) noexcept{
        std::swap(this->ptr_, other.ptr_);
    }

    template<typename T>
    bool VUniquePointer<T>::operator==(std::nullptr_t) const noexcept{
        return this->ptr_ == nullptr;
    }

    template<typename T, typename... Args>
    VUniquePointer<T> make_vunique(Args&&... args){
        return VUniquePointer<T>(
            new T(std::forward<Args>(args)...)
        );
    }
}

#endif