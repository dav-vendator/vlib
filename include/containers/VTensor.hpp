/*
Name: VMatrix
Description: A dyamical tensor container
Author: Dav Vendator
*/

#ifndef VLIB_MATRIX_HPP
#define VLIB_MATRIX_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <numerics>

namespace Vlib{
    template<typename T>
    class VTensor {
    private:
        T* data_;
        std::vector<std::size_t> shape_;
        std::vector<std::size_t> strides_;
        std::size_t size_;

        void stride() noexcept;

        void index(std::vector<std::size_t>&) noexcept const;

    public:
        VTensor();
        explicit VTensor(const std::vector<std::size_t>& shape);
        VTensor(std::initializer_list<std::size_t> shape);

        ~VTensor();

        VTensor(const VTensor& other);
        VTensor& operator=(const Tensor& other);

        VTensor(VTensor&& other) noexcept;
        VTensor& operator=(VTensor&& other) noexcept;

        std::size_t size() const noexcept;
        std::size_t rank() const noexcept;
        const std::vector<std::size_t>& shape() const noexcept;
        const std::vector<std::size_t>& strides() const noexcept;

        T* data() noexcept;
        const T* data() const noexcept;

        T& operator()(std::initializer_list<std::size_t> indices);
        const T& operator()(std::initializer_list<std::size_t> indices) const;

        void fill(const T& value);

        void reshape(const std::vector<std::size_t>& new_shape);
        void reshape(std::initializer_list<std::size_t> new_shape);

        void swap(VTensor& other) noexcept;
    };

    template<typename T>
    VTensor<T>::VTensor(): data_(nullptr),size_(0)
    {}

    template<typename T>
    VTensor<T>::VTensor(const std::vector<std::size_t>& shape){

    }
}

#endif