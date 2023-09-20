/**============================================================================
Name        : DVector.h
Created on  : 15.09.2023
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : DVector.h
============================================================================**/

#ifndef CPPPROJECTS_DVECTOR_H
#define CPPPROJECTS_DVECTOR_H

#include <memory>
#include <algorithm>
#include <format>

namespace DVector
{
    template<typename _Ty>
    struct Allocator: std::allocator<_Ty>
    {
        _Ty* allocate(size_t size)
        {
            return new _Ty[size];
        }

        void deallocate(_Ty* ptr, size_t)
        {
            delete[] ptr;
        }
    };



    template<typename Type,
            typename Allocator = Allocator<Type>>
    class DVector
    {
        using object_type = Type;
        using pointer = object_type*;
        using size_type = size_t;

        static_assert(!std::is_same_v<object_type, void>,
                      "Type of the Objects in the pool can not be void");

        static constexpr size_type initialCapacity { 10 };
        static constexpr size_type growthFactor { 4 };

    private:
        /** Elements collection block: **/
        pointer data { nullptr };

        /** Capacity: **/
        size_type capacity { 0 };

        /** Index of the element **/
        size_type left { 0 };
        size_type right { 0 };

        /** The allocator to use for allocating and deallocating chunks: **/
        Allocator allocator;

    private:

        void growVector()
        {
            // std::cout << "* * * * ReAlloc (" << capacity << " ==> " << capacity * growthFactor << ") * * * * \n";

            const size_type size = right - left - 1;
            const size_type left_old = left, left_center_dist = capacity / 2 - left - 1;

            capacity *= growthFactor;
            left = capacity / 2 - left_center_dist  - 1;
            right = left + size + 1;

            pointer newData { allocator.allocate(capacity) };
            std::uninitialized_move_n(data + left_old + 1, size, newData + left + 1);
            std::swap(data, newData);

            std::destroy_n(newData + left_old + 1, size);
            allocator.deallocate(newData, capacity);
        }

        void destroy()
        {
            const size_type size = right - left - 1;

            /** Invoke destructors for all contained objects: **/
            std::destroy_n(data + left + 1, size);
        }

    public:

        explicit DVector(const size_type s = initialCapacity)
        {
            capacity = s > 0 ? s : initialCapacity;
            data = allocator.allocate(capacity);

            right = capacity / 2;
            left = right - 1;   // TODO: check right > 1 ??
        }

        ~DVector()
        {
            if (0 == capacity)
                return;

            /** Invoke destructors for all contained objects: **/
            destroy();

            /** Deallocate all memory: **/
            allocator.deallocate(data, capacity);
        }

        DVector(const DVector<object_type, Allocator>& other):
                capacity { other.capacity }, left { other.left } , right { other.right }
        {
            data = allocator.allocate(other.capacity);
            std::copy_n(other.data + left + 1, right - left - 1, data + left + 1);
        }

        DVector(DVector<object_type, Allocator>&& other) noexcept:
                data { std::exchange(other.data, nullptr) },
                capacity { std::exchange(other.capacity, 0) },
                left { std::exchange(other.left, 0) },
                right { std::exchange(other.right, 0) } {
            /** **/
        }

        DVector<object_type, Allocator>& operator=(const DVector<object_type, Allocator>& other)
        {
            if (&other != this) {
                DVector localCopy(other);
                DVector::swap(localCopy, *this);
            }
            return *this;
        }

        DVector<object_type, Allocator>& operator=(DVector<object_type, Allocator>&& other) noexcept
        {
            if (&other != this)
            {
                data = std::exchange(other.data, nullptr);
                capacity = std::exchange(other.capacity, 0);
                left = std::exchange(other.left, 0);
                right = std::exchange(other.right, 0);
            }
            return *this;
        }

    public:

        [[nodiscard]]
        object_type& Front() const noexcept {
            return this->data[left + 1];
        }

        [[nodiscard]]
        object_type& Back() const noexcept {
            return this->data[right - 1];
        }

        [[nodiscard]]
        object_type& operator[] (size_type index) const {
            return this->data[index + left + 1];
        }

        [[nodiscard]]
        object_type& at(size_type index) const {
            if (index >= (right - left - 1))
                throw std::out_of_range(std::format("{} index is out of range", index));
            return this->data[index + left + 1];
        }

        [[nodiscard]]
        inline size_type Size() const noexcept {
            return 0 != capacity ? right - left - 1 : 0;
        }

        [[nodiscard]]
        inline size_type Capacity() const noexcept {
            return capacity;
        }

        [[nodiscard]]
        inline size_type FrontCapacity() const noexcept {
            return left + 1;
        }

        [[nodiscard]]
        inline size_type BackCapacity() const noexcept {
            return capacity - right;
        }

        [[nodiscard]]
        inline bool Empty() const noexcept {
            return 0 == capacity || 1 == (right - left);
        }

        [[nodiscard]]
        inline pointer Data() const noexcept {
            return data + left + 1;
        }

        inline void Clear() noexcept
        {
            /** Invoke destructors for all contained objects: **/
            destroy();

            right = capacity / 2;
            left = right - 1;
        }

        object_type& push_back(const object_type& v)
        {
            if (right >= capacity)
                growVector();
            this->data[right] = v;
            return data[right++];
        }

        object_type& push_back(object_type&& v)
        {
            if (right >= capacity)
                growVector();
            this->data[right] = std::move(v);
            return data[right++];
        }

        object_type& push_front(const object_type& v)
        {
            if (0 >= left)
                growVector();
            this->data[left] = v;
            return data[left--];
        }

        object_type& push_front(object_type&& v)
        {
            if (0 >= left)
                growVector();
            this->data[left] = std::move(v);
            return data[left--];
        }

        void pop_back()
        {
            data[--right].~object_type();
        }

        void pop_front()
        {
            data[++left].~object_type();
        }

        template<typename ... Args>
        object_type& emplace_back(Args&&... params)
        {
            if (right >= capacity)
                growVector();

            // Construct element in place:
            new (data + right) object_type { std::forward<Args>(params)... };
            return data[right++];
        }

        template<typename ... Args>
        object_type& emplace_front(Args&&... params)
        {
            if (0 >= left)
                growVector();

            // Construct element in place:
            new (data + left) object_type { std::forward<Args>(params)... };
            return data[left--];
        }

        void swap(DVector<object_type, Allocator> &other) noexcept
        {
            std::swap(this->data, other.data);
            std::swap(this->left, other.left);
            std::swap(this->right, other.right);
            std::swap(this->capacity, other.capacity);
        }

        static void swap(DVector<object_type, Allocator> &first,
                         DVector<object_type, Allocator> &second) noexcept
        {
            std::swap(first.data, second.data);
            std::swap(first.left, second.left);
            std::swap(first.right, second.right);
            std::swap(first.capacity, second.capacity);
        }

    public:  /** Debug methods: **/

        void printInfo()
        {
            const size_type size = right - left - 1;
            std::cout << "[ capacity = " << capacity << ", size = " << size << " ] "
                      << "[ left: " << left << ", right: " << right << " ]\n";

            for (size_type idx = 0; idx < capacity; ++idx)
                std::cout << '[' << idx << "] = " << data[idx] << std::endl;
        }

        void callGrowVector()
        {
            growVector();
        }
    };
}

#endif //CPPPROJECTS_DVECTOR_H
