/*
Name: VVector
Description: A dyamical vector
Author: Dav Vendator
*/

#ifndef VLIB_VECTOR_HPP
#define VLIB_VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <initializer_list>

namespace Vlib{
    template<typename T>
    class Vector{
            T* d_array_;
            std::size_t size_; 
            std::size_t capacity_;

            void reallocate();
        public:
            Vector() noexcept;  

            Vector(const T*, std::size_t); //initialize from array pointer

            Vector(std::initializer_list<T>);
            Vector(const Vector<T>&);
            Vector(Vector<T>&&) noexcept ;   


            Vector& operator = (const Vector<T>&); 
            Vector& operator = (Vector<T>&&) noexcept ;

            ~Vector();

            std::size_t size() const noexcept;
            std::size_t capacity() const noexcept;

            void push_back(const T&);

            T& operator[](std::size_t);
            const T& operator[](std::size_t) const;

            T& at(std::size_t);
            const T& at(std::size_t) const;

            void pop_back();
            void clear() noexcept;
            void clear_and_release();

            void reserve(std::size_t);

            bool empty() const noexcept;
            void shrink_to_fit();

            const T& front() const;
            const T& back() const;

            T& front();
            T& back();

            T* data() noexcept;
            const T* data() const noexcept;

            T* begin() noexcept;
            T* end() noexcept;
            const T* begin() const noexcept;
            const T* end() const noexcept;
            
            
    };

    template<typename T>
    void Vector<T>::reallocate(){
        //increases the size of the vector
        std::size_t cap_ = (this->capacity_ == 0) ? 2 : this->capacity_*2;
        T * arr = new T[cap_]();

        for (std::size_t i = 0; i < this->size_; i++)
            arr[i] = this->d_array_[i];
        
        delete  [] this->d_array_;

        this->d_array_ = arr;
        this->capacity_ = cap_;
        
    }

    template<typename T>
    Vector<T>::Vector() noexcept :d_array_(nullptr), size_(0), capacity_(0)
    {}

    template<typename T>
    Vector<T>::~Vector(){
        delete [] this->d_array_;
        this->d_array_ = nullptr;
        this->capacity_ = 0;
        this->size_ = 0;
    }

    template<typename T>
    Vector<T>::Vector(const T* array, std::size_t sz){
        //if its empty return
        if (sz == 0){
            this->capacity_ = this->size_ = 0;
            this->d_array_ = nullptr;
            return;
        }

        if (!array){
            throw std::invalid_argument("array pointer is null");
        }

        this->d_array_ = new T[sz*2]();
        this->capacity_ = sz*2;
        this->size_ = sz;

        for (std::size_t i=0; i < sz; i++){
            this->d_array_[i] = array[i];
        }

    }

    template<typename T>
    void Vector<T>::clear() noexcept {
        this->size_ = 0;
    }

    template<typename T>
    Vector<T>::Vector(std::initializer_list<T> lst){
        std::size_t sz = lst.size();

        if (sz == 0){
            this->capacity_ = 0;
            this->size_ = 0;
            this->d_array_ = nullptr;
            return;
        }

        this->capacity_ = sz*2;
        this->size_ = sz;
        this->d_array_ = new T[this->capacity_]();

        std::size_t i = 0;
        for (const T& value : lst)
            d_array_[i++] = value;
    }

    template<typename T>
    void Vector<T>::clear_and_release(){
        //clear the vector
        delete [] this->d_array_;
        this->capacity_ = this->size_ = 0;
        this->d_array_ = nullptr;
    }

    template<typename T>
    Vector<T>::Vector(const Vector<T>& other){
        //creates this vector from other vector
        this->capacity_ = other.capacity_;
        this->size_ = other.size_;
        this->d_array_ = nullptr;

        if (this->capacity_ == 0)
            return;

        this->d_array_ = new T[this->capacity_]();

        for (std::size_t i = 0; i < this->size_; i++)
            this->d_array_[i] = other.d_array_[i];
    }

    template<typename T>
    Vector<T>::Vector(Vector<T>&& other) noexcept {
        this->capacity_ = other.capacity_;
        this->size_ = other.size_;
        this->d_array_ = other.d_array_;

        other.capacity_ = other.size_ = 0;
        other.d_array_ = nullptr;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector<T>& other){
        if (this == &other)
            return *this;
        
        delete [] this->d_array_;

        this->size_ = other.size_;
        this->capacity_ = other.capacity_;
        this->d_array_ = nullptr;

        if (this->capacity_ != 0){
            this->d_array_ = new T[this->capacity_]();

            for (std::size_t i = 0; i < this->size_; i++)
                this->d_array_[i] = other[i];
        }

        return *this;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
        if (this == &other)
            return *this;

        delete [] this->d_array_;

        this->capacity_ = other.capacity_;
        this->size_ = other.size_;
        this->d_array_ = other.d_array_;

        other.capacity_ = other.size_ = 0;
        other.d_array_ = nullptr;

        return *this;
    }

    template<typename T>
    std::size_t Vector<T>::size() const noexcept{
        return this->size_;
    }

    template<typename T>
    std::size_t Vector<T>::capacity() const noexcept{
        return this->capacity_;
    }

    template<typename T>
    void Vector<T>::push_back(const T& value){
        if (this->capacity_ == this->size_)
            this->reallocate();
        this->d_array_[this->size_++] = value;
    }

    template<typename T>
    T& Vector<T>::operator[](std::size_t index){
        return this->d_array_[index];
    }
    
    template<typename T>
    const T& Vector<T>::operator[](std::size_t index) const{
        return this->d_array_[index];
    }

    template<typename T>
    T& Vector<T>::at(std::size_t index){
        if (index >= this->size_)
            throw std::out_of_range("index is greater than the array size.");
        return this->d_array_[index];
    }

    template<typename T>
    const T& Vector<T>::at(std::size_t index) const {
        if (index >= this->size_)
            throw std::out_of_range("index is greater than the array size.");
        return this->d_array_[index];
    }

    template<typename T>
    void Vector<T>::pop_back(){
        if (this->size_ > 0)
            --this->size_;
    }

    template<typename T>
    void Vector<T>::reserve(std::size_t new_capacity){
        if (new_capacity <= this->capacity_)
            return;
        this->capacity_ = new_capacity;
        T* new_ = new T[this->capacity_];
        for (std::size_t i=0; i < this->size_; i++)
            new_[i] = this->d_array_[i];
        delete [] this->d_array_;
        this->d_array_ = new_;
    }
    
    template<typename T>
    bool Vector<T>::empty() const noexcept {
        return this->size_ == 0;
    }

    template<typename T>
    void Vector<T>::shrink_to_fit(){
        if (this->size_ == this->capacity_)
            return;
        T* new_array = new T[this->size_]();
        for (std::size_t i=0; i < this->size_; i++)
            new_array[i] = this->d_array_[i];
        delete [] this->d_array_;
        this->d_array_ = new_array;
        this->capacity_ = this->size_;
    }
    
    template<typename T>
    const T& Vector<T>::front() const{
        if (this->size_ == 0)
            throw std::underflow_error("Can't access element from empty vector");
        return this->d_array_[0];
    }
    
    template<typename T>
    const T& Vector<T>::back() const{
        if (this->size_ == 0)
            throw std::underflow_error("Can't access element from empty vector");
        return this->d_array_[this->size_-1];
    }

    template<typename T>
    T& Vector<T>::front(){
        if (this->size_ == 0)
            throw std::underflow_error("Can't access element from empty vector");
        return this->d_array_[0];
    }
    
    template<typename T>
    T& Vector<T>::back(){
        if (this->size_ == 0)
            throw std::underflow_error("Can't access element from empty vector");
        return this->d_array_[this->size_-1];
    }

    template<typename T>
    T* Vector<T>::data() noexcept{
        return this->d_array_;
    }
    
    template<typename T>
    const T* Vector<T>::data() const noexcept{
        return this->d_array_;
    }

    template<typename T>
    T* Vector<T>::begin() noexcept{
        return d_array_;
    }

    template<typename T>
    T* Vector<T>::end() noexcept{
        return d_array_ + size_;
    }

    template<typename T>
    const T* Vector<T>::begin() const noexcept{
        return d_array_;
    }

    template<typename T>
    const T* Vector<T>::end() const noexcept{
        return d_array_ + size_;
    }
}

#endif