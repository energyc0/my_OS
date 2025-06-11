#include "heap.h"
#include "io.h"
#include "utils.h"

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

void heap_init(){
    root = (chunk_header*)_heap_start_;
    root->is_allocated = 0;
    root->next = root->prev = NULL;
    root->size = _heap_end_ - _heap_start_ - sizeof(chunk_header);
}

void* malloc(size_t size){
    void* ptr = chunk_find_free(size);
    if(ptr == NULL)
        PANIC("Failed to allocate %d bytes!\n", size);
    return ptr;
}

void free(void* ptr){
    chunk_header* chunk = (chunk_header*)((char*)ptr - sizeof(chunk_header));
    if(!chunk_is_valid((void*)chunk))
        PANIC("Invalid pointer for free()!\n");
    chunk_dealloc(chunk);
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
        printf("Header at %p with size %d, next = %p, prev %p, %s\n",
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