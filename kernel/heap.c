#include "heap.h"
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

static void chunk_alloc(chunk_header* p, size_t size);
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
    root = chunk_create(_heap_start_,
    _heap_end_ - _heap_start_ - sizeof(chunk_header),
    NULL,NULL);
}

void* malloc(size_t size){
    //align 4 bytes
    size = (size + 4 - 1) / 4 * 4;
    return size > 0 ? chunk_find_free(size) : NULL;
}

void* calloc(size_t num, size_t size){
    void* ptr = malloc(num*size);
    if(ptr)
        memset(ptr, 0, num*size);
    return ptr;
}

void free(void* ptr){
    if(ptr == NULL)
        return;
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
        //create new header
        chunk->next = chunk_create((chunk_header*)((char*)chunk + new_size + sizeof(chunk_header)),
         chunk->size - new_size - sizeof(chunk_header), chunk->next, chunk);
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
                chunk->next = chunk_create((char*)chunk + sizeof(chunk_header) + new_size,
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

static void chunk_alloc(chunk_header* hdr, size_t size){
    //create new chunk on the next position
    if (hdr->size > size + sizeof(chunk_header))
        hdr->next = chunk_create(((char*)hdr + size + sizeof(chunk_header)),
     hdr->size - size - sizeof(chunk_header), hdr->next, hdr);

    hdr->is_allocated = 1;
    hdr->size = size;
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
        if ((ptr->prev && ((uint32_t)((char*)ptr - (char*)ptr->prev) != (sizeof(chunk_header) + ptr->prev->size) )) ||
            (ptr->next && ((uint32_t)((char*)ptr->next - (char*)ptr)) != (sizeof(chunk_header) + ptr->size)))
            PANIC("Chunk headers corruption!!!\n");
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

    chunk_header* temp = p->next;
    size_t size = p->size;
    // if there are free chunks on the left
    while (p->prev && !p->prev->is_allocated) {
        p = p->prev;
        size += sizeof(chunk_header) + p->size;
    }
    //merge chunks on the right
    while (temp && !temp->is_allocated){
        size += sizeof(chunk_header) + temp->size;
        temp = temp->next;
    }
    p->next = temp;
    if(temp)
        temp->prev = p;
    p->size = size;
}

static chunk_header* chunk_create(void* mem, size_t size, chunk_header* next, chunk_header* prev){
    chunk_header* ptr = (chunk_header*)mem;
    ptr->is_allocated = 0;
    ptr->next = next;
    ptr->prev = prev;
    ptr->size = size;

    if(next)
        next->prev = ptr;
    if(prev)
        prev->next = ptr;
    return ptr;
}

void heap_basic_test(){
    printf("=== Test: Basic malloc/free ===\n");

    void* ptr1 = malloc(100);
    printf("malloc(100) -> %p\n", ptr1);
    heap_debug();

    free(ptr1);
    printf("free(ptr1)\n");
    heap_debug();

    void* ptr2 = malloc(100);
    printf("malloc(100) -> %p (should reuse freed block)\n", ptr2);
    heap_debug();
    free(ptr2);
}

void heap_multiple_test(){
    printf("=== Test: Multiple allocations ===\n");

    void* ptrs[5];
    for (int i = 0; i < 5; i++) {
        ptrs[i] = malloc(50);
        printf("malloc(50) -> %p\n", ptrs[i]);
    }
    heap_debug();

    free(ptrs[1]); ptrs[1] = NULL;
    free(ptrs[2]); ptrs[2] = NULL;
    free(ptrs[3]); ptrs[3] = NULL;
    printf("Freed ptrs[1-3]\n");
    heap_debug();

    void* new_ptr = malloc(50);
    printf("malloc(50) -> %p (should fill gaps)\n", new_ptr);
    heap_debug();
    free(new_ptr);

    for (int i = 0; i < 5; i++) {
        if (ptrs[i] != NULL) free(ptrs[i]);
    }
    heap_debug();
}

void heap_calloc_test(){
    printf("=== Test: calloc ===\n");
    int* arr = (int*)calloc(10, sizeof(int));
    printf("calloc(10, sizeof(int)) -> %p\n", arr);

    for (int i = 0; i < 10; i++) {
        if (arr[i] != 0) {
            printf("ERROR: calloc did not zero memory!\n");
            break;
        }
    }
    heap_debug();
    free(arr);
}

void heap_realloc_test(){
    printf("=== Test: realloc ===\n");

    int* ptr = (int*)malloc(5 * sizeof(int));
    printf("malloc(5 * sizeof(int)) -> %p\n", ptr);

    for (int i = 0; i < 5; i++) ptr[i] = i;
    heap_debug();

    ptr = (int*)realloc(ptr, 10 * sizeof(int));
    printf("realloc(ptr, 10 * sizeof(int)) -> %p\n", ptr);

    for (int i = 0; i < 5; i++) {
        if (ptr[i] != i) {
            printf("ERROR: realloc corrupted data!\n");
            break;
        }
    }

    heap_debug();
    free(ptr);
}

void heap_edge_cases_test(){
    printf("=== Test: Edge cases ===\n");

    void* ptr1 = malloc(0);
    printf("malloc(0) -> %p\n", ptr1);

    free(NULL);
    printf("free(NULL) good\n");

    void* huge_ptr = malloc(1024 * 1024);
    printf("malloc(1MB) -> %p\n", huge_ptr);
    heap_debug();
    free(ptr1);
    free(huge_ptr);
}