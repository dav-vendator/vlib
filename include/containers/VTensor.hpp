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

namespace Vlib{
    template<typename T>
    class VTensor {
            T* data_;
            std::vector<std::size_t> shape_;
            std::vector<std::size_t> strides_;
            std::size_t size_;

            void build_strides();
            void build_size() noexcept;

            std::size_t offset(const std::vector<std::size_t>&) const noexcept;

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
    void VTensor<T>::build_size() noexcept {  
        this->size_ = 1;
        for (auto& val : this->shape_)
            this->size_ *= val;
    }

    template<typename T>
    void VTensor<T>::build_strides(){
        this->strides_.resize(this->shape_.size());
        std::size_t running = 1;
        for (std::size_t i = this->shape_.size(); i-- > 0; ) {
            this->strides_[i] = running;
            running *= this->shape_[i];
        }
    }

    template<typename T>
    std::size_t VTensor<T>::offset(const std::vector<std::size_t>& indices) const noexcept{
        std::size_t offset = 1;
        for (std::size_t i = 0; i < indices.size(); i++)
            offset += (this->strides_[i] * indices[[i]]);
        return offset;
    }

    template<typename T>
    VTensor<T>::VTensor(): data_(nullptr), size_(0)
    {}

    template<typename T>
    VTensor<T>::VTensor(const std::vector<std::size_t>& shape){

    }
}

#endif