// File to to be compiled into a shared library and preloaded to an executable.
// This library is used to  perform monitoring of the allocation patterns
// of the target executable. All objects are allocated on DRAM, but the allocated bytes
// and the active allocated objects over the execution are monitored and saved in a log file.
// The logging is performed via a background timer thread, which is triggered every x seconds.
// compile with : g++ -shared -fPIC -o libmon.so monitor.c new.cpp -lmemkind

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <unistd.h>
#include <memkind.h>
#include <pthread.h>

int err = 0, initialized = 0, dram = 1;
pthread_t timer_thread;
void *timer_threadproc(void *arg);   
int done = 0;  
float seconds_elapsed = 0.0;
unsigned long  allocated_objects = 0;
unsigned long  allocated_bytes = 0;
pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
FILE *log_file;


// Constructor Function to initialize our library. The log file
// is created and the background thread is initialized.
__attribute__((constructor)) void init_memkind() {
    initialized = 1;
    done = 0;  
    log_file = fopen("logfile.log", "w");
    if (log_file == NULL) {
        perror("Failed to open logfile");
    }
    if (pthread_create(&timer_thread, NULL, &timer_threadproc, NULL) != 0) {
        fprintf(stderr, "Failed to create timer thread\n");
    }
}


// Destructor Function, sets initialization flag to 0, shuts down the background thread
// and closes the log file.
__attribute__((destructor)) void del(void) {
    initialized = 0;
    done = 1;  
    pthread_join(timer_thread, NULL);   
    fclose(log_file);
}


// Function triggered by the background thread. This function is called every x seconds, where x
// is user defined (0.25s in this case). It logs the allocated_bytes and active_objects information
// every x seconds.
// The active_objects is the number of active_objects FROM the beginning of the execution
// UNTIL the current time stamp.
// The allocated_bytes is the number of allocated bytes IN the current x second time stamp.
void *timer_threadproc(void *arg) {
    while (!done) {
        usleep(250000);             // Change the value of the usec to monitor at your desired frequency
        seconds_elapsed += 0.25;    // also change the elapsed seconds value
        fprintf(log_file, "Allocated: %lu bytes (second %f) (objects alive = %lu)\n", 
                allocated_bytes, seconds_elapsed, allocated_objects);
                
        pthread_mutex_lock(&alloc_mutex);
        allocated_bytes = 0;
        pthread_mutex_unlock(&alloc_mutex);
    }
    return NULL;
}


// Function to override default malloc, allocates memory on DRAM and monitors the allocation information.
// The active objects are increased by 1 for each allocation, and the allocated bytes are increased based
// on the size of the allocation.
// Mutex locks are also used to make sure that the active_objects and allocated_bytes
// variables are updated in a thread-safe way.
void *malloc(size_t size) {
    
    void *p = memkind_malloc(MEMKIND_DEFAULT, size);
    if (initialized == 1) {
        pthread_mutex_lock(&alloc_mutex);
        allocated_bytes += size;
        allocated_objects += 1;
        pthread_mutex_unlock(&alloc_mutex);
    }
    return p;
    
}


// Function to override default calloc, and monitor the aforementioned information, similar to malloc.
void *calloc(size_t num, size_t size) {
    
    void *p = memkind_calloc(MEMKIND_DEFAULT, num, size);
    if (initialized == 1) {
        pthread_mutex_lock(&alloc_mutex);
        allocated_bytes += num * size;
        allocated_objects += 1;
        pthread_mutex_unlock(&alloc_mutex);
    }
    return p;
    
}


// Function to override default realloc, and monitor the aforementioned information, similar to malloc.
void *realloc(void *ptr, size_t size) {
    
    void *p = memkind_realloc(MEMKIND_DEFAULT, ptr, size);
    if (initialized == 1) {
        pthread_mutex_lock(&alloc_mutex);
        allocated_bytes += size;
        allocated_objects += 1;
        pthread_mutex_unlock(&alloc_mutex);
    }
    return p;
    
}


// Function to override default free. It decreases the allocated_objects by 1, each time it is called.
void free(void *ptr) {
    if (ptr == NULL)
        return;
    
    if (initialized == 1) {
        pthread_mutex_lock(&alloc_mutex);
        allocated_objects -= 1;
        pthread_mutex_unlock(&alloc_mutex);
    }
    memkind_free(MEMKIND_DEFAULT, ptr);
    
}

#ifdef __cplusplus
}
#endif
