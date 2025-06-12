#include "heap.h"
#include "io.h"
#include "utils.h"
#include <stdint.h>

extern char _heap_start_[];
extern char _heap_end_[];

typedef struct chunk_header{
    int is_allocated;
    size_t size;
    struct chunk_header* prev;
    struct chunk_header* next;
}chunk_header;

static chunk_header* root;

static void* chunk_find_free(size_t size);
//return next node of the allocated chunk
static chunk_header* chunk_alloc(chunk_header* p, size_t size);
static inline void chunk_remove(chunk_header* p);
//check for free() if chunk is valid
static inline int chunk_is_valid(chunk_header* p);
//call in free()
static void chunk_dealloc(chunk_header* p);
//merge neighbours if they are not allocated
static void chunk_merge(chunk_header* p);

//initalize new chunk header
static chunk_header* chunk_create(void* mem, size_t size, chunk_header* next, chunk_header* prev);

void heap_init(){
    root = (chunk_header*)_heap_start_;
    root->is_allocated = 0;
    root->next = root->prev = NULL;
    root->size = _heap_end_ - _heap_start_ - sizeof(chunk_header);
}

void* malloc(size_t size){
    //align 4 bytes
    size = (size + 4 - 1) / 4 * 4;
    void* ptr = chunk_find_free(size);
    if(ptr == NULL)
        PANIC("Failed to allocate %d bytes!\n", size);
    return ptr;
}

void* calloc(size_t num, size_t size){
    void* ptr = malloc(num*size);
    if(ptr)
        memset(ptr, 0, num*size);
    return ptr;
}

void free(void* ptr){
    chunk_header* chunk = (chunk_header*)((char*)ptr - sizeof(chunk_header));
    if(!chunk_is_valid((void*)chunk))
        PANIC("Invalid pointer for free()!\n");
    chunk_dealloc(chunk);
}

void* realloc(void* ptr, size_t new_size){
    chunk_header* chunk = (chunk_header*)((char*)ptr - sizeof(chunk_header));
    if(!chunk_is_valid(chunk))
        PANIC("Invalid pointer for realloc()!\n");
    if(chunk->size > new_size + sizeof(chunk_header)){
        chunk_header* temp = chunk->next;

        //create new header
        chunk->next = (chunk_header*)((char*)chunk + new_size + sizeof(chunk_header));
        chunk->next->is_allocated = 0;
        chunk->next->next = temp;
        chunk->next->prev = chunk;
        chunk->next->size = chunk->size - new_size - sizeof(chunk_header);
        //update size
        chunk->size = new_size;
    }else if(chunk->size < new_size){
        size_t cur_sz = chunk->size;
        chunk_header* temp = chunk->next;
        //try to find more space on the right
        for(;cur_sz < new_size && temp && !temp->is_allocated; temp = temp->next) {
            cur_sz += sizeof(chunk_header) + temp->size;
        }

        //if found more space on the right
        if(cur_sz >= new_size){
            //try to emplace a new header
            if(cur_sz > new_size + sizeof(chunk_header)){
                chunk->next = chunk_create(chunk + sizeof(chunk_header) + new_size,
                 cur_sz - new_size - sizeof(chunk_header),
                  temp, chunk);
                  chunk->size = new_size;
            }else{
                 chunk->next = temp;
                 chunk->size = cur_sz;
            }
        }else{ 
            //else deallocate this chunk and search for more space
            chunk_dealloc(chunk);
            ptr = malloc(new_size);
            //if not enough space
            if(!ptr)
                return NULL;
            //and then copy the data
            memcpy(ptr, (char*)chunk + sizeof(chunk_header), chunk->size);
        }
    }
    return ptr;
}

static void* chunk_find_free(size_t size){
    chunk_header* p = root;
    for(;p!=NULL; p = p->next){
        if(!p->is_allocated && p->size >= size){
            break;
        }
    }

    if(p){
        chunk_alloc(p, size);
        return (void*)((char*)p+sizeof(chunk_header));
    }
    return NULL;
}

static chunk_header* chunk_alloc(chunk_header* hdr, size_t size){
    //create new chunk on the next position
    if (hdr->size > size + sizeof(chunk_header)) {
        chunk_header* temp = hdr->next;
        hdr->next = (chunk_header*)((char*)hdr + size + sizeof(chunk_header));

        hdr->next->size = hdr->size - size - sizeof(chunk_header);
        hdr->next->next = temp;
        hdr->next->prev = hdr;
        hdr->next->is_allocated = 0;
    }
    hdr->is_allocated = 1;
    hdr->size = size;
    return hdr->next;
}

static inline void chunk_remove(chunk_header* p){
    if(p->next)
        p->next->prev = p->prev;
    if (p->prev)
        p->prev->next = p->next;
}

void heap_debug(){
    printf("Heap start: %p\n", _heap_start_);
    printf("Heap end: %p\n", _heap_end_);
    for(chunk_header* ptr = root; ptr != NULL; ptr = ptr->next){
        printf("Header at %p with size %X, next = %p, prev %p, %s\n",
             ptr, ptr->size, ptr->next, ptr->prev, ptr->is_allocated ? "allocated" : "not allocated");
    }
}

static inline int chunk_is_valid(chunk_header* chunk){
    for (chunk_header* ptr = root; ptr!=NULL; ptr = ptr->next) {
        if(ptr == chunk)
            return 1;
    }
    return 0;
}

static void chunk_dealloc(chunk_header* p){
    p->is_allocated = 0;
    chunk_merge(p);
}

static void chunk_merge(chunk_header* p){
    if(p->is_allocated)
        return;
    // if there are free chunks on the left
    if (p->prev && !p->prev->is_allocated)
        chunk_merge(p->prev);
    else{ //merge chunks on the right
        chunk_header* temp = p->next;
        size_t size = p->size;
        while (temp && !temp->is_allocated){
            size += sizeof(chunk_header) + temp->size;
            temp = temp->next;
        }
        p->next = temp;
        p->size = size;
    }
}

static chunk_header* chunk_create(void* mem, size_t size, chunk_header* next, chunk_header* prev){
    chunk_header* ptr = (chunk_header*)mem;
    ptr->is_allocated = 0;
    ptr->next = next;
    ptr->prev = prev;
    ptr->size = size;
    return ptr;
}