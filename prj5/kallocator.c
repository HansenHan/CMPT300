#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "kallocator.h"
#include "list.h"

struct KAllocator {
    enum allocation_algorithm aalgorithm;
    int size;
    void* memory;
    // Some other data members you want, 
    // such as lists to record allocated/free memory
    struct nodeStruct* allocate_memory;
    struct nodeStruct* free_memory;
    int **array;
};

struct KAllocator kallocator;


void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm) {
    assert(_size > 0);
    kallocator.aalgorithm = _aalgorithm;
    kallocator.size = _size;
    kallocator.memory = malloc((size_t)kallocator.size);

    // Add some other initialization 
    kallocator.allocate_memory = NULL;  // set record list to null
    kallocator.free_memory = List_createNode(_size);//create node
    kallocator.array = malloc(sizeof(int*) * _size);
    for (int i = 0; i < _size; ++i){
        array[i] = kallocator.memory + i;
    }
}

void destroy_allocator() {
    free(kallocator.memory);

    // free other dynamic allocated memory to avoid memory leak
}

void* kalloc(int _size) {
    void* ptr = NULL;

    // Allocate memory from kallocator.memory 
    // ptr = address of allocated memory

    if(_size <= 0 || _size > available_memory() || kallocator.size < _size){
        return ptr;
    }

    struct nodeStruct* store = NULL;
    struct nodeStruct* curr = kallocator.free_memory;

    if(kallocator.aalgorithm == FIRST_FIT){
        while(curr != NULL && _size > curr->item){
            curr = curr ->next;
        }
        if(_size < curr->item){
            store = curr;
        }
    }
    else if(kallocator.aalgorithm == BEST_FIT){
        int min = 101;
        while(curr != NULL){
            if(curr->item - _size >= 0 && curr->item - _size < min){
                min = curr->item - _size;
                store = curr;
            }
            curr = curr->next;
        }
    }
    else if(kallocator.aalgorithm == WORST_FIT){
        int max = -1;
        while(curr != NULL){
            if(curr->item - _size >= 0 && curr->item - _size > max){
                max = curr->item - _size;
                store = curr;
            }
            curr = curr->next
        }
    }

    if(store == NULL){
        return ptr;
    }

    // int iter = 0;
    if(store->item == _size){
        List_insertTail(&kallocator.allocate_memory, store);
        ptr = store->add;
        List_deleteNode(&kallocator.free_memory, store);
    }
    else{
        struct nodeStruct *newnode = malloc(sizeof(struct nodeStruct));
        newnode->item = store->item - _size;
        store->item = _size;
        List_insertTail(&kallocator.allocate_memory, store);
        List_deleteNode(&kallocator.free_memory, store);
        List_insertTail(&kallocator.free_memory, newnode);
    }

    return ptr;
}

void kfree(void* _ptr) {
    assert(_ptr != NULL);

    
}

int compact_allocation(void** _before, void** _after) {
    int compacted_size = 0;

    // compact allocated memory
    // update _before, _after and compacted_size

    return compacted_size;
}

int available_memory() {
    int available_memory_size = 0;
    // Calculate available memory size
    struct nodeStruct *current = kallocator.free_memory;  // go through memory size and sum everything up
    while(current != NULL){
        available_memory_size = available_memory_size + current->item;
        current = current->next;
    }
    return available_memory_size;
}

void print_statistics() {

    int allocated_size = 0;
    struct nodeStruct *current = kallocator.allocate_memory;  // go through memory size and sum everything up
    while(current != NULL){
        allocated_size = allocated_size + current->item;
        current = current->next;
    }

    int allocated_chunks = List_countNodes(kallocator.allocate_memory);
    int free_size = available_memory();
    int free_chunks = List_countNodes(kallocator.free_memory);


    struct nodeStruct *current = free_memory;
    struct nodeStruct *min = free_memory;
    while(current != NULL){
        if(current->item < min->item){
            min = current;
        }
        current = current->next;
    }
    int smallest_free_chunk_size = min->item;



    struct nodeStruct *current = kallocator.free_memory;
    struct nodeStruct *max = kallocator.free_memory;
    while(current != NULL){
        if(current->item > max->item){
            max = current;
        }
        current = current->next;
    }
    int largest_free_chunk_size = max->item;

    // Calculate the statistics

    printf("Allocated size = %d\n", allocated_size);
    printf("Allocated chunks = %d\n", allocated_chunks);
    printf("Free size = %d\n", free_size);
    printf("Free chunks = %d\n", free_chunks);
    printf("Largest free chunk size = %d\n", largest_free_chunk_size);
    printf("Smallest free chunk size = %d\n", smallest_free_chunk_size);
}



