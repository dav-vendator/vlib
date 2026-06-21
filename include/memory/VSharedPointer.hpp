/*
Name: VSharedPointer
Description: A smart reference counted pointer
Author: Dav Vendator
*/

#ifndef VLIB_VSharedPointer_HPP
#define VLIB_VSharedPointer_HPP

#include <utility>
#include <cstddef>

namespace Vlib
{
    template <typename T>
    class VSharedPointer{
        T *pointer;
        std::size_t *reference_count;

        void release();

    public:
        VSharedPointer() noexcept;

        VSharedPointer(T*);

        // copy constructor
        VSharedPointer(const VSharedPointer<T>&);

        // move constructor
        VSharedPointer(VSharedPointer<T>&&);

        // Copy assignment
        VSharedPointer &operator=(const VSharedPointer<T>&);

        // Move assignment
        VSharedPointer &operator=(VSharedPointer<T>&&);

        ~VSharedPointer();

        T &operator*() const;

        T *operator->() const;

        T *get() const;
        std::size_t use_count() const;

        explicit operator bool() const;
        void reset();

        void reset(T*);

        bool operator==(const VSharedPointer<T>&) const;
        bool operator!=(const VSharedPointer<T>&) const;
        void swap(VSharedPointer<T>&) noexcept;
    };

    template<typename T>
    void VSharedPointer<T>::release(){
        if (this->reference_count){
            if (*this->reference_count > 1){
                --(*this->reference_count);
                this->pointer = nullptr;
            }
            else if (*this->reference_count == 1){
                delete this->pointer;
                delete this->reference_count;
            }
        }
        this->reference_count = nullptr;
        this->pointer = nullptr;
    }

    template<typename T>
    VSharedPointer<T>::VSharedPointer() noexcept :  pointer(nullptr), reference_count(nullptr)
            {}

    template<typename T>
    VSharedPointer<T>::VSharedPointer(T *ptr) : pointer(ptr){
        // intialising the reference counter
        this->reference_count = ptr? new std::size_t(1) : nullptr;
    }

    // copy constructor
    template<typename T>
    VSharedPointer<T>::VSharedPointer(const VSharedPointer<T> &other){
        // this is for creation
        this->pointer = other.pointer;
        this->reference_count = other.reference_count;
        if (other.reference_count){
            ++(*this->reference_count);
        }
    }

    // move constructor
    template<typename T>
    VSharedPointer<T>::VSharedPointer(VSharedPointer<T> &&other) : pointer(other.pointer), reference_count(other.reference_count){
        other.reference_count = nullptr;
        other.pointer = nullptr;
    }

    // Copy assignment
    template<typename T>
    VSharedPointer<T>& VSharedPointer<T>::operator=(const VSharedPointer<T> &other){
        // p1 = p2
        if (this == &other)
            return *this; // if same then return
        this->release();
        this->pointer = other.pointer;
        this->reference_count = other.reference_count;
        if (other.reference_count){
            ++(*this->reference_count);
        }
        return *this;
    }

    // Move assignment
    template<typename T>
    VSharedPointer<T>& VSharedPointer<T>::operator=(VSharedPointer<T> &&other){
        if (this == &other){
            return *this;
        }
        this->release(); 
        this->pointer = other.pointer;
        this->reference_count = other.reference_count;
        other.pointer = nullptr;
        other.reference_count = nullptr;
        return *this;
    }

    template<typename T>
    VSharedPointer<T>::~VSharedPointer(){
        this->release();
    }

    template<typename T>
    T& VSharedPointer<T>::operator*() const{
        return *this->pointer;
    }

    template<typename T>
    T* VSharedPointer<T>::operator->() const{
        return this->pointer;
    }

    template<typename T>
    T* VSharedPointer<T>::get() const{
        return this->pointer;
    }

    template<typename T>
    std::size_t VSharedPointer<T>::use_count() const{
        return this->reference_count ? *this->reference_count:0;
    }

    template<typename T>
    VSharedPointer<T>::operator bool() const{
            return this->pointer != nullptr;
    }

    template<typename T>
    void VSharedPointer<T>::reset(){
        this->release();
    }

    template<typename T>
    void VSharedPointer<T>::reset(T *new_ptr){
        this->release();
        this->pointer = new_ptr;
        this->reference_count = new_ptr? new std::size_t(1)  : nullptr;
    }

    template<typename T>
    bool VSharedPointer<T>::operator==(const VSharedPointer<T>& other) const{
        return this->pointer == other.pointer;
    }

    template<typename T>
    bool VSharedPointer<T>::operator!=(const VSharedPointer<T>& other) const{
        return this->pointer != other.pointer;
    }

    template<typename T>
    void VSharedPointer<T>::swap(VSharedPointer<T>& other) noexcept{
        std::swap(this->pointer, other.pointer);
        std::swap(this->reference_count, other.reference_count);
    }
}

#endif