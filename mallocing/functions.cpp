// Program to redirect the allocation functions of C++ to the
// respective ones that we have created in the custom_allocator.c file

#include <iostream>

void* operator new(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        throw std::bad_alloc(); 
    }
    return ptr;
}

void* operator new[](size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        throw std::bad_alloc(); 
    }
    return ptr;
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}


