//
// Created by domonion on 12.09.18.
//

#ifndef EXAM_FIXED_CAPACITY_VECTOR_H
#define EXAM_FIXED_CAPACITY_VECTOR_H

#include <cstddef>
#include <type_traits>
#include <iterator>
#include <cassert>

template<typename T, size_t SIZE>
class fixed_vector {
private:
    size_t size_;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data_[SIZE];

public:
    template<typename Type>
    class Iterator : public std::iterator<std::random_access_iterator_tag, Type> {
    private:
        Type *ptr_;
    public:
        using difference_type = typename std::iterator<std::random_access_iterator_tag, Type>::difference_type;

        Iterator() : ptr_(nullptr) {}

        explicit Iterator(Type *rhs) : ptr_(rhs) {}

        Iterator(const Iterator &rhs) : ptr_(rhs.ptr_) {}

        Iterator &operator=(Type *rhs) {
            ptr_ = rhs;
            return *this;
        }

        Iterator &operator=(const Iterator &rhs) {
            ptr_ = rhs.ptr_;
            return *this;
        }

        Iterator &operator+=(difference_type rhs) {
            ptr_ += rhs;
            return *this;
        }

        Iterator &operator-=(difference_type rhs) {
            ptr_ -= rhs;
            return *this;
        }

        Type &operator*() const { return *ptr_; }

        Type *operator->() const { return ptr_; }

        Type &operator[](difference_type rhs) const { return ptr_[rhs]; }

        Iterator &operator++() {
            ++ptr_;
            return *this;
        }

        Iterator &operator--() {
            --ptr_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp(*this);
            ++ptr_;
            return tmp;
        }

        Iterator operator--(int) {
            Iterator tmp(*this);
            --ptr_;
            return tmp;
        }

        difference_type operator-(const Iterator &rhs) const { return ptr_ - rhs.ptr_; }

        Iterator operator+(difference_type rhs) const { return Iterator(ptr_ + rhs); }

        Iterator operator-(difference_type rhs) const { return Iterator(ptr_ - rhs); }

        friend Iterator operator+(difference_type lhs, const Iterator &rhs) { return Iterator(lhs + rhs.ptr_); }

        friend Iterator operator-(difference_type lhs, const Iterator &rhs) { return Iterator(lhs - rhs.ptr_); }

        bool operator==(const Iterator &rhs) const { return ptr_ == rhs.ptr_; }

        bool operator!=(const Iterator &rhs) const { return ptr_ != rhs.ptr_; }

        bool operator>(const Iterator &rhs) const { return ptr_ > rhs.ptr_; }

        bool operator<(const Iterator &rhs) const { return ptr_ < rhs.ptr_; }

        bool operator>=(const Iterator &rhs) const { return ptr_ >= rhs.ptr_; }

        bool operator<=(const Iterator &rhs) const { return ptr_ <= rhs.ptr_; }
    };

    fixed_vector() : size_(0) {}

    fixed_vector(fixed_vector const &rhs) : size_(rhs.size_) {
        for (size_t i = 0; i < size_; i++)
            new(data_ + i)T(*reinterpret_cast<T *>(rhs.data_ + i));
    }

    fixed_vector &operator=(fixed_vector rhs) {
        swap(*this, rhs);
        return *this;
    }

    void push_back(T const &value) {
        assert(size() < SIZE);
        new(data_ + size_++) T(value);
    }

    void pop_back() {
        assert(0 < size());
        reinterpret_cast<T *>(data_ + --size_)->~T();
    }

    void push_front(T const &value) {
        assert(size() < SIZE);
        for (size_t i = size_; i > 0; i--) {
            new(data_ + i)T(*reinterpret_cast<T *>(data_ + i - 1));
            reinterpret_cast<T *>(data_ + i - 1)->~T();
        }
        new(data_)T(value);
        size_++;
    }

    void pop_front() {
        assert(0 < size());
        reinterpret_cast<T *>(data_)->~T();
        size_--;
        for (size_t i = 0; i < size_; i++) {
            new(data_ + i)T(*reinterpret_cast<T *>(data_ + i + 1));
            reinterpret_cast<T *>(data_ + i + 1)->~T();
        }
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return SIZE;
    }

    size_t max_size() const {
        return SIZE;
    }

    bool empty() const {
        return size_ == 0;
    }

    T operator[](size_t ind) const {
        assert(0 <= ind && ind < size_);
        return *reinterpret_cast<T *>(data_ + ind);
    }

    T &operator[](size_t ind) {
        assert(0 <= ind && ind < size_);
        return *reinterpret_cast<T *>(data_ + ind);
    }

    friend void swap(fixed_vector<T, SIZE> &lhs, fixed_vector<T, SIZE> &rhs) {
        std::swap(lhs.size_, rhs.size_);
        std::swap(lhs.data_, rhs.data_);
    }

    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        return iterator(reinterpret_cast<T *>(data_));
    }

    iterator end() {
        return iterator(reinterpret_cast<T *>(data_ + size_));
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_iterator begin() const {
        return const_iterator(reinterpret_cast<const T *>(data_));
    }

    const_iterator end() const {
        return const_iterator(reinterpret_cast<const T *>(data_ + size_));
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    iterator insert(iterator pos, T const &value) {
        assert(size() < SIZE);
        assert(begin() <= pos && pos <= end());
        for (size_t i = size_; i > pos - begin(); i--) {
            new(data_ + i)T(*reinterpret_cast<T *>(data_ + i - 1));
            reinterpret_cast<T *>(data_ + i - 1)->~T();
        }
        new(data_ + (pos - begin()))T(value);
        size_++;
        return pos;
    }

    iterator erase(iterator pos) {
        assert(begin() <= pos && pos < end());
        assert(size() > 0);
        reinterpret_cast<T *>(data_ + (pos - begin()))->~T();
        --size_;
        for (size_t i = pos - begin(); i < size_; i++) {
            new(data_ + i)T(*reinterpret_cast<T *>(data_ + i + 1));
            reinterpret_cast<T *>(data_ + i + 1)->~T();
        }
        return pos;
    }

    T front(){
        assert(size_ > 0);
        return *reinterpret_cast<T *>(data_);
    }
    T back(){
        assert(size_ > 0);
        return *reinterpret_cast<T *>(data_ + size_ - 1);
    }
    void clear(){
        for(; size_ > 0;){
            pop_back();
        }
    }
};


#endif //EXAM_FIXED_CAPACITY_VECTOR_H
