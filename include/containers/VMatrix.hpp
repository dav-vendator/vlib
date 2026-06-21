/*
Name: VMatrix
Description: A dyamical matrix container
Ordering: row-major order
Author: Dav Vendator
*/

#ifndef VLIB_MATRIX_HPP
#define VLIB_MATRIX_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <limits>
#include <ostream>

namespace Vlib{

    template<typename T>
    class Matrix{
            std::size_t rows_;
            std::size_t cols_;
            T* data_;

            std::size_t index(std::size_t row, std::size_t col) const noexcept;

            void swap(Matrix&) noexcept;

        public:
            Matrix();
            Matrix(std::size_t, std::size_t);
            ~Matrix();

            void clear() noexcept;

            Matrix(const Matrix&);
            Matrix(Matrix&&) noexcept;

            Matrix& operator=(Matrix);
           // Matrix& operator=(Matrix&&) noexcept;

            std::size_t rows() const noexcept;
            std::size_t cols() const noexcept;
            bool empty() const noexcept;
            std::size_t size() const noexcept;

            T& operator()(std::size_t,std::size_t);
              
            const T& operator()(std::size_t,std::size_t) const;

            T& at(std::size_t,std::size_t);
              
            const T& at(std::size_t,std::size_t) const;

            void fill(const T&);

            T* data() noexcept;
            const T* data() const noexcept;

            T* begin() noexcept;
            T* end() noexcept;

            const T* begin() const noexcept;
            const T* end() const noexcept;

            const T* cbegin() const noexcept;
            const T* cend() const noexcept;

            Matrix operator+(const Matrix&) const;
            Matrix operator-(const Matrix&) const;

            Matrix& operator+=(const Matrix&);
            Matrix& operator-=(const Matrix&);

            Matrix operator*(const Matrix&) const;   // matrix multiplication
            Matrix operator*(const T&) const;       // scalar multiplication

            Matrix transpose() const;

            static Matrix identity(std::size_t);
    };

    template<typename T>
    Matrix<T>::Matrix():rows_(0),
                        cols_(0),
                        data_(nullptr)
    {}

    template<typename T>
    Matrix<T>::Matrix(std::size_t rows, std::size_t cols):rows_(rows), 
                                                          cols_(cols), 
                                                          data_(nullptr){
        
        if (this->rows_ == 0 || this->cols_ == 0)
            return;

        if (this->rows_ > std::numeric_limits<std::size_t>::max() / this->cols_)
            throw std::length_error("Matrix size overflow");

        this->data_ = new T[this->rows_ * this->cols_]();
    }

    template<typename T>
    Matrix<T>::~Matrix(){
        delete [] this->data_;
    }

    template<typename T>
    void Matrix<T>::clear() noexcept {
        delete [] this->data_;
        this->cols_ = this->rows_ = 0;
        this->data_ = nullptr;
    }

    template<typename T>
    Matrix<T>::Matrix(const Matrix<T>& other):rows_(other.rows_), 
                                              cols_(other.cols_), 
                                              data_(nullptr){
        //copy constructor
        if (this->rows_ == 0 || this->cols_ == 0)
            return;
        
        std::size_t n = this->rows_ * this->cols_;

        this->data_ =  new T[n]();
        
        for (std::size_t i=0; i < n; i++)
            this->data_[i] = other.data_[i];
    }


    template<typename T>
    Matrix<T>::Matrix(Matrix<T>&& other) noexcept:rows_(other.rows_), 
                                                  cols_(other.cols_), 
                                                  data_(other.data_){
        other.cols_ = other.rows_ = 0;
        other.data_ = nullptr;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator=(Matrix<T> other){ 
        this->swap(other);

        return *this;
    }
    
    // template<typename T>
    // Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept{
    //     if (this == &other)
    //         return *this;

    //     this->cols_ = other.cols_;
    //     this->rows_ = other.rows_;

    //     delete [] this->data_;

    //     this->data_ = other.data_;

    //     other.data_ = nullptr;
    //     other.rows_ = 0;
    //     other.cols_ = 0;

    //     return *this;
    // }

    template<typename T>
    std::size_t Matrix<T>::rows() const noexcept{
        return this->rows_;
    }
    
    template<typename T>
    std::size_t Matrix<T>::cols() const noexcept{
        return this->cols_;
    }
            
    template<typename T>
    bool Matrix<T>::empty() const noexcept{
        return (this->rows_ == 0 || this->cols_ == 0);
    }
    
    template<typename T>
    std::size_t Matrix<T>::size() const noexcept{
        return this->cols_*this->rows_;
    }

    template<typename T>
    std::size_t Matrix<T>::index(std::size_t row, 
                                 std::size_t col) const noexcept{
        //this is for row-major order
        return row * this->cols_ + col;
    }

    template<typename T>
    void Matrix<T>::swap(Matrix<T>& other) noexcept {
        std::swap(rows_, other.rows_);
        std::swap(cols_, other.cols_);
        std::swap(data_, other.data_);
    }

    template<typename T>
    T& Matrix<T>::operator()(std::size_t row,
              std::size_t col){
        return this->data_[this->index(row, col)];
    }
    
    template<typename T>
    const T& Matrix<T>::operator()(std::size_t row,
                    std::size_t col) const{
        return this->data_[this->index(row, col)];
    }

    template<typename T>
    T& Matrix<T>::at(std::size_t row,
              std::size_t col){
        if ((row >= this->rows_) || (col >= this->cols_))
            throw std::out_of_range("Matrix index out of range");
        return this->data_[this->index(row, col)];
    }
    
    template<typename T>
    const T& Matrix<T>::at(std::size_t row,
                    std::size_t col) const{
        if ((row >= this->rows_) ||  (col >= this->cols_))
            throw std::out_of_range("Matrix index out of range");
        return this->data_[this->index(row,col)];
    }

    template<typename T>
    void Matrix<T>::fill(const T& value){
        
        for (std::size_t i=0; i < this->size(); i++)
            this->data_[i] = value;
    }

    template<typename T>
    T* Matrix<T>::data() noexcept{
        return this->data_;
    }

    template<typename T>
    const T* Matrix<T>::data() const noexcept{
        return this->data_;
    }
    
    template<typename T>
    T* Matrix<T>::begin() noexcept{
        return this->data_;
    }

    template<typename T>
    T* Matrix<T>::end() noexcept{
        return this->data_ + this->size();
    }

    template<typename T>
    const T* Matrix<T>::begin() const noexcept{
        return this->data_;
    }

    template<typename T>
    const T* Matrix<T>::end() const noexcept{
        return this->data_ + this->size();
    }

    template<typename T>
    const T* Matrix<T>::cbegin() const noexcept{
        return this->data_;
    }

    template<typename T>
    const T* Matrix<T>::cend() const noexcept{
        return this->data_ + this->size();
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const{
        Matrix<T> new_m(*this);
        new_m += other;
        return new_m;
    }
    
    template<typename T>
    Matrix<T> Matrix<T>::operator-(const Matrix<T>& other)  const{
        Matrix<T> new_m(*this);
        new_m -= other;
        return new_m;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other){
       if ((this->rows_ != other.rows_) || (this->cols_ != other.cols_))
            throw std::invalid_argument("Matrix shape is incompatible.");
        
        for  (std::size_t i = 0; i < this->size(); i++)
            this->data_[i] += other.data_[i];

        return *this;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& other){
        if ((this->rows_ != other.rows_) || (this->cols_ != other.cols_))
            throw std::invalid_argument("Matrix shape is incompatible.");
        
        for  (std::size_t i = 0; i < this->size(); i++)
            this->data_[i] -= other.data_[i];

        return *this;
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const{

        if (this->cols_ != other.rows_)
            throw std::invalid_argument("Matrix is incompatible");

        Matrix<T> result(this->rows_, other.cols_);
        
        for (std::size_t row = 0; row < this->rows_; row++){
            for (std::size_t col = 0; col < other.cols_; col++){
                for (std::size_t k = 0; k < this->cols_; k++){
                    result.data_[result.index(row, col)] += 
                                    this->data_[this->index(row, k)]*other.data_[other.index(k, col)];
                }
            }
        }

        return result;
    }   // matrix multiplication

    template<typename T>
    Matrix<T> Matrix<T>::operator*(const T& value) const{
        Matrix<T> result(*this);

        for (std::size_t i=0; i < this->size(); i++)
            result.data_[i] *= value;

        return result;
    }      // scalar multiplication

    template<typename T>
    Matrix<T> Matrix<T>::transpose() const{
        Matrix<T> temp(*this);
        for (std::size_t i=0; i < temp.rows_; i++)
            for (std::size_t j=0; j < temp.cols_; j++)
                temp(i,j) = (*this)(j,i);
        return temp;
    }

    template<typename T>
    Matrix<T> Matrix<T>::identity(std::size_t sz){
        Matrix<T> result(sz, sz);

        for (std::size_t i=0; i < sz; i++)
            result(i,i) = T{1};
        
        return result;
    }

    template<typename T>
    std::ostream& operator << (std::ostream& os, const Matrix<T>& mat){
        for (std::size_t row = 0; row < mat.rows(); ++row) {
            for (std::size_t col = 0; col < mat.cols(); ++col) {
                os << mat(row, col) << ' ';
            }
            os << '\n';
        }
        return os;
    }
}

#endif