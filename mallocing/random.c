// File to to be compiled into a shared library and preloaded to an executable.
// This library places allocation objects randomly either on DRAM or PMEM.
// compile with : g++ -shared -fPIC -o librandom.so random.c new.cpp -lmemkind


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <unistd.h>
#include <memkind.h>
#include <stdlib.h> 
#include <time.h>
struct memkind *pmem_kind=NULL;
int err=0, initialized=0;


// Constructor Function to initialize PMEM by creating a file-backed kind of memory.
// Sets initialization flag to 1, so that placement decisions are trigerred.
__attribute__((constructor)) void init_memkind() {    
    err = memkind_create_pmem("/mnt/pmem0", 0, &pmem_kind);   // use the mount location of your own configuration
    if(err){ 
        return; 
    }
    initialized = 1;
    srand(time(NULL));
}

// Destructor Function, sets initialization flag to 0.
static void __attribute__((destructor)) del(void){
    initialized = 0;
}


// Function to override default malloc, allocates memory either on PMEM or DRAM
// randomly, according to the value of the dram variable.
void *malloc(size_t size){
    if(initialized == 0) {
        return memkind_malloc(MEMKIND_DEFAULT, size);
    }

    else{
        int dram = rand() % 2; // set dram to either 0 or 1 randomly
        if(dram == 1){
            return memkind_malloc(MEMKIND_DEFAULT, size);
        }
        else{
            return memkind_malloc(pmem_kind, size);
        }
    }
}


// Function to override default calloc, and randomly make allocations, similar to malloc.
void *calloc(size_t num, size_t size) {
    if(initialized == 0){
        return memkind_calloc(MEMKIND_DEFAULT, num, size);
    }

    else {
        int dram = rand() % 2;
        if(dram == 1){
            return memkind_calloc(MEMKIND_DEFAULT, num, size);
        }
        else{
            return memkind_calloc(pmem_kind, num, size);
        }
    }

}



// Function to override default realloc, checks if the pointer belongs to PMEM or DRAM 
// and calls the respective realloc function.
void *realloc(void *ptr,size_t size){
    if(ptr == NULL) return malloc(size);

    memkind_t kind = memkind_detect_kind(ptr);
    if(kind == MEMKIND_DEFAULT)  return  memkind_realloc(MEMKIND_DEFAULT,ptr,size);
    else  return memkind_realloc(pmem_kind,ptr,size);
    
}


// Function to override default free, checks if the pointer belongs to PMEM or DRAM 
// and frees from the respective memory type.
void free(void *ptr){
    if(ptr == NULL) return;

    memkind_t kind = memkind_detect_kind(ptr);      
    if(kind == MEMKIND_DEFAULT)  memkind_free(MEMKIND_DEFAULT,ptr);
    else memkind_free(pmem_kind,ptr);

    
}


#ifdef __cplusplus
}
#endif
