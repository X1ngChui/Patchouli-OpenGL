#pragma once
#include <atomic>
#include <type_traits>
#include <utility>

namespace Pache
{
    // Default deleter for releasing memory
    template <typename T>
    struct DefaultDeleter
    {
        void operator()(T* ptr) const
        {
            delete ptr;
        }
    };

    // Default deleter specialization for arrays
    template <class T>
    struct DefaultDeleter<T[]>
    {
        void operator()(T* ptr) const
        {
            delete[] ptr;
        }
    };

    // Base class implementing reference counting
    class RefCounted
    {
    public:
        RefCounted() = default;
        virtual ~RefCounted() = default;

        // Increase reference count
        void incRefCount() const
        {
            ++count;
        }

        // Decrease reference count
        void decRefCount() const
        {
            --count;
        }

        // Get reference count
        uint32_t getRefCount() const
        {
            return count.load();
        }
    private:
        template <typename T, typename Deleter>
        friend class IntrusivePtr;
        
        mutable std::atomic<size_t> count = 0;  // Atomic variable for reference count
    };

    // Reference-counted smart pointer
    template <typename T, typename Deleter = DefaultDeleter<T>>
    class IntrusivePtr
    {
        static_assert(std::is_base_of_v<RefCounted, T>, "The type must be derived from RefCounted");
    public:
        IntrusivePtr() : ptr(nullptr)
        {
        }
        
        // Allow nullptr construction
        IntrusivePtr(std::nullptr_t)
            : ptr(nullptr)
        {
        }

        // Explicit constructor, takes a raw pointer
        explicit IntrusivePtr(T* ptr) : ptr(ptr)
        {
            incRefCount();
        }

        // Copy constructor
        IntrusivePtr(const IntrusivePtr& other) : ptr(other.ptr)
        {
            incRefCount();
        }

        // Copy assignment operator
        IntrusivePtr& operator=(const IntrusivePtr& other)
        {
            ptr = other.ptr;
            incRefCount();
            return *this;
        }

        // Move constructor
        IntrusivePtr(IntrusivePtr&& other) noexcept
            : ptr(std::exchange(other.ptr, nullptr))
        {
        }

        // Move assignment operator
        IntrusivePtr& operator=(IntrusivePtr&& other) noexcept
        {
            if (this != &other)
            {
                ptr = std::exchange(other.ptr, nullptr);
            }
            return *this;
        }

        // Conversion constructor from derived class, requires convertible condition
        template <typename U, typename OtherDeleter, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
        IntrusivePtr(IntrusivePtr<U, OtherDeleter>& other)
            : ptr(other.ptr)
        {
            incRefCount();
        }

        // Move constructor from derived class
        template <typename U, typename OtherDeleter, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
        IntrusivePtr(IntrusivePtr<U, OtherDeleter>&& other)
            : ptr(std::exchange(other.ptr, nullptr))
        {
        }

        // Destructor
        ~IntrusivePtr()
        {
            decRefCount();
        }

        // Get the raw pointer
        T* get() const
        {
            return ptr;
        }

        // Release ownership and return the raw pointer
        T* release() const
        {
            return std::exchange(ptr, nullptr);
        }

        // Reset the pointer, possibly with a new raw pointer
        void reset(T* ptr = nullptr)
        {
            decRefCount();
            this->ptr = ptr;
            incRefCount();
        }

        // Overload dereference operator
        T& operator*() const
        {
            return *ptr;
        }

        // Overload arrow operator
        T* operator->() const
        {
            return ptr;
        }

    private:
        // Increase reference count if the raw pointer is not null
        void incRefCount() const
        {
            if (ptr) {
                ptr->incRefCount();
            }
        }

        // Decrease reference count if the raw pointer is not null
        void decRefCount() const
        {
            if (ptr) {
                ptr->decRefCount();
                if (ptr->getRefCount() == 0) {
                    Deleter()(ptr);
                }
            }
        }

        // Friend declaration to allow access to private members from other IntrusivePtr types
        template <typename U, typename OtherDeleter>
        friend class IntrusivePtr;

        T* ptr;  // Store the raw pointer
    };

    // Specialization of IntrusivePtr for arrays, inherits from IntrusivePtr<T, Deleter>
    template <typename T, typename Deleter>
    class IntrusivePtr<T[], Deleter> : public IntrusivePtr<T, Deleter>
    {
    };

    // Factory function to create a IntrusivePtr instance
    template <typename T, typename... Args>
    IntrusivePtr<T> makeIntrusive(Args&&... args)
    {
        return IntrusivePtr<T>(new T(std::forward<Args>(args)...));
    }

    template <typename T, typename Deleter = DefaultDeleter<T>>
    class UniquePtr
    {
    public:
        UniquePtr()
            : ptr(nullptr)
        {
        }

        // Allow nullptr construction
        UniquePtr(std::nullptr_t)
            : ptr(nullptr)
        {
        }

        // Explicit constructor, takes a raw pointer
        explicit UniquePtr(T* ptr)
            : ptr(ptr)
        {
        }

        // Copy constructor
        UniquePtr(const UniquePtr& other) = delete;

        // Copy assignment operator
        UniquePtr operator=(const UniquePtr& other) = delete;

        // Move constructor
        UniquePtr(UniquePtr&& other)
            : ptr(std::exchange(other.ptr, nullptr))
        {
        }

        // Move assignment operator
        UniquePtr& operator=(UniquePtr&& other)
        {
            if (this != &other)
            {
                ptr = std::exchange(other.ptr, nullptr);
            }
            return *this;
        }

        // Conversion constructor from derived class, requires convertible condition
        template <typename U, typename OtherDeleter, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
        UniquePtr(UniquePtr<U, OtherDeleter>& other)
            : ptr(other.ptr)
        {
        }

        // Move constructor from derived class
        template <typename U, typename OtherDeleter, typename = std::enable_if_t<std::convertible_to<U*, T*>>>
        UniquePtr(UniquePtr<U, OtherDeleter>&& other)
            : ptr(std::exchange(other.ptr, nullptr))
        {
        }

        // Destructor
        ~UniquePtr()
        {
            if (ptr)
            {
                Deleter()(ptr);
            }
        }

        // Get the raw pointer
        T* get() const
        {
            return ptr;
        }

        // Release ownership and return the raw pointer
        T* release() const
        {
            return std::exchange(ptr, nullptr);
        }

        // Reset the pointer, possibly with a new raw pointer
        void reset(T* ptr = nullptr)
        {
            if (this->ptr)
            {
                Deleter()(this->ptr);
            }
            this->ptr = ptr;
        }

        // Overload dereference operator
        T& operator*() const
        {
            return *ptr;
        }

        // Overload arrow operator
        T* operator->() const
        {
            return ptr;
        }
    private:
        // Friend declaration to allow access to private members from other UniquePtr types
        template <typename U, typename OtherDeleter>
        friend class UniquePtr;

        T* ptr;  // Store the raw pointer
    };

    // Specialization of UniquePtr for arrays, inherits from UniquePtr<T, Deleter>
    template <typename T, typename Deleter>
    class UniquePtr<T[], Deleter> : public IntrusivePtr<T, Deleter>
    {
    };

    // Factory function to create a UniquePtr instance
    template <typename T, typename... Args>
    UniquePtr<T> makeUnique(Args&&... args)
    {
        return UniquePtr<T>(new T(std::forward<Args>(args)...));
    }
}
