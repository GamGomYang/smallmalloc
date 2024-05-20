// homwork2 
//2022041052_GeumyoungKim 
//2022041046_JeayunByeon

// smalloc.h -> #include <stddef.h> #include <stdint.h> include

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stddef.h>
#include "smalloc.h"

#define PAGE_SIZE getpagesize()

smheader_ptr smlist = NULL;
int initialized = 0;
void smalloc_initialized(void);

void *smalloc(size_t s)
{

    smheader_ptr itr = smlist, previous = NULL;
    if (smlist == NULL)
    {
        smalloc_initialized();
    }

    while (itr != NULL)
    {
        if (!itr->used && itr->size >= s + sizeof(struct _smheader))
        {
            size_t total_size_needed = s + sizeof(struct _smheader);
            if (itr->size > total_size_needed)
            {
                smheader_ptr block_add = (smheader_ptr)(((char *)itr) + total_size_needed);
                block_add->size = itr->size - total_size_needed;
                block_add->used = 0;
                block_add->next = itr->next;
                itr->next = block_add;
                itr->size = s;
            }
            itr->used = 1;
            return (void *)((char *)itr + sizeof(struct _smheader));
        }
        previous = itr;
        itr = itr->next;
    }

    void *ptr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap fail!");
        exit(EXIT_FAILURE);
    }

    smheader_ptr new_page = (smheader_ptr)ptr;
    new_page->size = PAGE_SIZE - sizeof(struct _smheader);
    new_page->used = 0;
    new_page->next = NULL;

    if (previous != NULL)
    {
        previous->next = new_page;
    }
    else
    {
        smlist = new_page;
    }

    return smalloc(s);
}

void sfree(void *p)
{
    if (p == NULL)
        return;
    smheader_ptr header = (smheader_ptr)((char *)p - sizeof(struct _smheader));
    header->used = 0;
}

// realloc() -> if memory value less than  allocated memory is allocated, few memory will be deleted
// srealloc() -> memory value less than allocated memory sfree() -> salloc() reallocate memory -> make other problem?

/*코드를 자세히 보니까 할당된 메모리 일부를 줄여야하는데
현재 메모리를 free시키고 다시 할당하고 있음
만약에 이렇게 된다면 같은 페이지에 할당된 다른 메모리를두고
 새로운 페이지를 만들어 2개의 페이지가 됨*/

void *srealloc(void *p, size_t s)
{
    if (p == NULL)
    {

        // printf("요청 크기: %zu\n", s);
        return smalloc(s);
    }

    smheader_ptr header = (smheader_ptr)((char *)p - sizeof(struct _smheader));
    size_t old_size = header->size;

    // printf("srealloc 호출됨: 주소=%p, 요청된 크기=%zu, 기존 크기=%zu\n", p, s, old_size);

    if (s <= old_size)
    {
        size_t remaining_size = old_size - s - sizeof(struct _smheader);
        // printf(" 남는 크기=%zu\n", remaining_size);

        if (remaining_size > sizeof(struct _smheader))
        {
            smheader_ptr block_add = (smheader_ptr)(((char *)header) + sizeof(struct _smheader) + s);
            block_add->size = remaining_size;
            block_add->used = 0;
            block_add->next = header->next;
            header->next = block_add;

            // printf("새로운 빈 블록 생성됨: 주소=%p, 크기=%zu\n", block_add, block_add->size);

            smcoalesce();
        }

        header->size = s;
        return (void *)((char *)header + sizeof(struct _smheader));
    }
    else
    {
        void *new_ptr = smalloc(s);
        if (new_ptr == NULL)
        {
            return NULL;
        }
        memcpy(new_ptr, p, old_size);
        sfree(p);
    }
}

void smcoalesce()
{
    smheader_ptr itr = smlist;
    while (itr != NULL && itr->next != NULL)
    {
        if (!itr->used && !itr->next->used)
        {
            uintptr_t current_start = (uintptr_t)itr;
            uintptr_t next_start = (uintptr_t)itr->next;
            if ((current_start / PAGE_SIZE) == (next_start / PAGE_SIZE))
            {
                itr->size += (itr->next->size + sizeof(struct _smheader));
                itr->next = itr->next->next;
            }
            else
            {
                itr = itr->next;
            }
        }
        else
        {
            itr = itr->next;
        }
    }
}

// test4 realloc 4000
// getpagesize() must use

void smalloc_initialized()
{
    void *ptr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        perror("fail!");
        exit(EXIT_FAILURE);
    }
    smlist = (smheader_ptr)ptr;
    smlist->size = PAGE_SIZE - sizeof(struct _smheader);
    smlist->used = 0;
    smlist->next = NULL;
    initialized = 1;
}

void *smalloc_mode(size_t s, smmode mode)
{
    smheader_ptr optimal_state = NULL;
    smheader_ptr *best_previous = NULL;
    smheader_ptr current = smlist;
    size_t result_size_diff = SIZE_MAX;

    while (current != NULL)
    {

        if (!current->used && current->size >= s)
        {
            size_t current_size_diff = current->size;

            // problem point1. size - (s+sizeof(struct _smheader) -> underflow
            // problem point2. sfree() -> not merge adjacent unused area

            switch (mode)
            {
            case firstfit:
                current->used = 1;
                return ((char *)current) + sizeof(struct _smheader);
            case bestfit:
                if (optimal_state == NULL || current_size_diff < result_size_diff)
                {
                    optimal_state = current;
                    result_size_diff = current_size_diff;
                }
                break;
            case worstfit:
                if (optimal_state == NULL || current_size_diff > result_size_diff)
                {
                    optimal_state = current;
                    result_size_diff = current_size_diff;
                }
                break;
            }
        }
        current = current->next;
    }

    if (optimal_state)
    {
        optimal_state->used = 1;
        return ((char *)optimal_state) + sizeof(struct _smheader);
    }

    return smalloc(s);
}

void smdump()
{
    smheader_ptr itr;
    printf("==================== used memory slots ====================\n");
    int i = 0;
    for (itr = smlist; itr != NULL; itr = itr->next)
    {
        if (itr->used == 0)
            continue;
        printf("%3d:%p:%8d:", i, ((void *)itr) + sizeof(struct _smheader), (int)itr->size);
        int j;
        char *s = ((char *)itr) + sizeof(struct _smheader);
        for (j = 0; j < (itr->size >= 8 ? 8 : itr->size); j++)
        {
            printf("%02x ", s[j]);
        }
        printf("\n");
        i++;
    }
    printf("\n");

    printf("==================== unused memory slots ====================\n");
    i = 0;
    for (itr = smlist; itr != NULL; itr = itr->next)
    {
        if (itr->used == 1)
            continue;
        printf("%3d:%p:%8d:", i, ((void *)itr) + sizeof(struct _smheader), (int)itr->size);
        int j;
        char *s = ((char *)itr) + sizeof(struct _smheader);
        for (j = 0; j < (itr->size >= 8 ? 8 : itr->size); j++)
        {
            printf("%02x ", s[j]);
        }
        printf("\n");
        i++;
    }
    printf("\n");
}
