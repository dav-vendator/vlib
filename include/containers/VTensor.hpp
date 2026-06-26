/*
Name: VTensor
Description: A dyamical tensor container
Author: Dav Vendator
*/

#ifndef VLIB_TENSOR_HPP
#define VLIB_TENSOR_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <initializer_list>
#include <utility>

namespace Vlib{

    //forward declaration
    template<typename T>
    class VTensor;

    template<typename T>
    class VTensorView;

    template<typename T>
    class VTensor {
            T* data_;
            std::vector<std::size_t> shape_;
            std::vector<std::size_t> strides_;
            std::size_t size_;

            static std::vector<std::size_t> build_strides(const std::vector<std::size_t>&);

            static std::size_t build_size(const std::vector<std::size_t>&) noexcept;

            std::size_t offset(const std::vector<std::size_t>&) const;
            std::size_t offset(const std::initializer_list<std::size_t>) const;

            friend class VTensorView<T>;

        public:
            VTensor();
            explicit VTensor(const std::vector<std::size_t>&);
            VTensor(std::initializer_list<std::size_t>);

            ~VTensor();

            VTensor(const VTensor&);
            VTensor& operator=(const VTensor&);

            VTensor(VTensor&&) noexcept;
            VTensor& operator=(VTensor&&) noexcept;

            std::size_t size() const noexcept;
            std::size_t rank() const noexcept;
            const std::vector<std::size_t>& shape() const noexcept;
            const std::vector<std::size_t>& strides() const noexcept;

            T* data() noexcept;
            const T* data() const noexcept;

            T& operator()(std::initializer_list<std::size_t>);
            const T& operator()(std::initializer_list<std::size_t>) const;

            void fill(const T&);

            void reshape(const std::vector<std::size_t>& );
            void reshape(std::initializer_list<std::size_t> );

            void swap(VTensor& ) noexcept;
    };

    template<typename T>
    std::size_t build_size(const std::vector<std::size_t>& shape) noexcept {  
        std::size_t sz = 1;
        for (auto& val : shape)
            sz *= val;
        return sz;
    }

    template<typename T>
    std::vector<std::size_t>
    build_strides(const std::vector<std::size_t>& shape)
    {
        std::vector<std::size_t> strides(shape.size()); 
        std::size_t running = 1;    
        for (std::size_t i = shape.size(); i-- > 0; ) {
            strides[i] = running;
            running *= shape[i];
        }   
        return strides;
    }

    template<typename T>
    std::size_t VTensor<T>::offset(const std::vector<std::size_t>& indices) const {
        if (indices.size() != this->shape_.size())
            throw std::invalid_argument("Indices are not equal to shape.");

        std::size_t off = 0;

        for (std::size_t i = 0; i < indices.size(); i++){
            if (indices[i] >= this->shape_[i])
                throw std::out_of_range("Tensor index out of range");
            off += (indices[i] * this->strides_[i]);
        }

        return off;   
    }

    template<typename T>
    std::size_t VTensor<T>::offset(std::initializer_list<std::size_t> indices) const {
        if (indices.size() != this->shape_.size())
            throw std::invalid_argument("Indices are not equal to shape.");

        std::size_t off = 0;
        std::size_t dim = 0;

        for (auto index : indices) {
            if (index >= this->shape_[dim])
                throw std::out_of_range("Tensor index out of range");

            off += index * this->strides_[dim];
            ++dim;
        }

        return off;
    }

    template<typename T>
    VTensor<T>::VTensor(): data_(nullptr), size_(0)
    {}

    template<typename T>
    VTensor<T>::VTensor(std::initializer_list<std::size_t> shape): VTensor(std::vector<std::size_t>(shape))
    {}

    template<typename T>
    VTensor<T>::VTensor(const std::vector<std::size_t>& shape): data_(nullptr),
                                                                shape_(shape),
                                                                size_(build_size(shape)){
       
        this->strides_ = this->build_strides(this->shape_);
        if (size_ == 0)
            return;
        data_ = new T[size_]();
    }

    template<typename T>
    VTensor<T>::~VTensor(){
        delete [] this->data_;
    }

    template<typename T>
    VTensor<T>::VTensor(const VTensor<T>& other){

        this->size_ = other.size_;
        this->shape_ = other.shape_;
        this->strides_ = other.strides_;

        if (this->size_ == 0){
            this->data_ = nullptr;
            return;
        }

        this->data_ = new T[this->size_]();
        
        for (std::size_t i = 0; i < this->size_; i++)
            this->data_[i] = other.data_[i];

    }
    
    template<typename T>
    VTensor<T>& VTensor<T>::operator=(const VTensor<T>& other){
        if (this == &other)
            return *this;

        VTensor<T> temp(other);

        this->swap(temp);

        return *this;
        
    }

    template<typename T>
    VTensor<T>::VTensor(VTensor<T>&& other) noexcept : data_(nullptr), size_(0){
        this->swap(other);
    }
            
    template<typename T>
    VTensor<T>& VTensor<T>::operator=(VTensor<T>&& other) noexcept{
        if (this == &other)
            return *this;
        this->swap(other);
        return *this;
    }

    template<typename T>
    std::size_t VTensor<T>::size() const noexcept{
        return this->size_;
    }

    template<typename T>
    std::size_t VTensor<T>::rank() const noexcept{
        return this->shape_.size();
    }

    template<typename T>
    const std::vector<std::size_t>& VTensor<T>::shape() const noexcept{
        return this->shape_;
    }

    template<typename T>
    const std::vector<std::size_t>& VTensor<T>::strides() const noexcept{
        return this->strides_;
    }

    template<typename T>
    T* VTensor<T>::data() noexcept{
        return this->data_;
    }

    template<typename T>
    const T* VTensor<T>::data() const noexcept{
        return this->data_;
    }

    template<typename T>
    T& VTensor<T>::operator()(std::initializer_list<std::size_t> indices){
        std::size_t index = this->offset(indices);
        return this->data_[index];
    }

    template<typename T>
    const T& VTensor<T>::operator()(std::initializer_list<std::size_t> indices) const{
        std::size_t index = this->offset(indices);
        return this->data_[index];
    }

    template<typename T>
    void VTensor<T>::fill(const T& value){
        if (this->data_ == nullptr)
            return;
        for (std::size_t i = 0; i < this->size_; i++)
            this->data_[i] = value;
    }

    template<typename T>
    void VTensor<T>::reshape(const std::vector<std::size_t>& new_shape){

        if (build_size(new_shape) != this->size_)
            throw std::invalid_argument("New shape is incompatible");
        this->shape_ = new_shape;
        this->strides_ = this->build_strides(this->shape_);
    }

    template<typename T>
    void VTensor<T>::reshape(std::initializer_list<std::size_t> new_shape){
        this->reshape(std::vector<std::size_t>(new_shape));
    }

    template<typename T>
    void VTensor<T>::swap(VTensor<T>& other) noexcept {
        std::swap(this->size_, other.size_);
        std::swap(this->shape_, other.shape_);
        std::swap(this->strides_, other.strides_);
        std::swap(this->data_, other.data_);
    }

// VTensor View -- A view class which is returned when slice or row operations are called on VTensor
    template<typename T>
    class VTensorView {
    private:
        T* data_;
        std::size_t size_;

        std::vector<std::size_t> shape_;
        std::vector<std::size_t> strides_;

        std::size_t offset_;

        VTensorView(
            T*,
            std::vector<std::size_t>,
            std::vector<std::size_t>,
            std::size_t,
            std::size_t
        );

        std::size_t offset(std::initializer_list<std::size_t>) const;

        std::size_t offset(std::vector<std::size_t>&) const;

        bool next_index(std::vector<std::size_t>&, const std::vector<std::size_t>&) const;

        friend class VTensor<T>;

    public:
        VTensorView() = delete;

        T& operator()(std::initializer_list<std::size_t>);
        const T& operator()(std::initializer_list<std::size_t>) const;

        VTensor<T> copy() const;

        VTensorView<T> slice(std::size_t dim, std::size_t index) const;
    };

    template<typename T>
    bool VTensorView<T>::next_index(std::vector<std::size_t>& index, 
                 const std::vector<std::size_t>& shape) const {
        for (std::size_t dim  = shape.size(); dim-- > 0;){
            ++index[dim];
            if (index[dim] < shape[dim])
                return true;
            index[dim] = 0;
        }
        return false;
    }

    template<typename T>
    VTensorView<T>::VTensorView(
        T* data,
        std::vector<std::size_t> shape,
        std::vector<std::size_t> strides,
        std::size_t size,
        std::size_t offset
    ): data_(data),
       shape_(std::move(shape)),
       strides_(std::move(strides)),
       offset_(offset),
       size_(size){
        if (data_ == nullptr)
            throw std::invalid_argument("Data cannot be a null pointer.");

        if (shape_.size() != strides_.size())
            throw std::invalid_argument("Shape and strides rank mismatch.");
    }

    template<typename T>
    VTensor<T> VTensorView<T>::copy() const{
        if (data_ == nullptr)
            throw std::logic_error("TensorView has no valid data.");

        VTensor<T> result(this->shape_);

        if (result.size() == 0)
            return result;

        std::vector<std::size_t> indx(this->shape_.size(), 0);
        std::size_t linear = 0;
        while (true){
            result.data()[linear] = (*this)(indx);
            ++linear;
            if (!next_index(indx, this->shape_))
                break;
        }
        return result;
        
    }
    

}

#endif