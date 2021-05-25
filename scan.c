// gcc -Wall -Werror -Wno-format -pedantic -std=c99 -g3 -o scan getregs.S scan.c
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define ESP 0

void *i = (void *) 0x51089f337b473c7c;
void *j;
static void *k = (void *) 0x51089f337b473c7c;
static void *l;

// Start of initialized data segment.
extern void *__data_start;
// End of the intialize data segment and start of the
// uninitialized data segment.
extern void *_edata;
// End of the uninitialized data segment.
extern void *_end;
// Storage for the following callee saved registers:
//   rsp, rbx, rbp, r12 - r15
// The registers will be stored in the bss segment
// and, if they contain the target value, they will
// get picked up in the bss scan.
static void *regs[7];
extern void get_regs(void *regs);

static void *stack_bot(void)
{
    FILE *fp;
    unsigned long bot;

    fp = fopen("/proc/self/stat", "r");
    if (fp == NULL)
    {
        return NULL;
    }
    fscanf(fp,"%*d %*s %*c%*d%*d%*d%*d%*d%*u%*lu%*lu%*lu%*lu"
           "%*lu%*lu%*ld%*ld%*ld%*ld%*ld%*ld%*llu%*lu%*ld%*lu"
           "%*lu%*lu%lu", &bot);
    fclose(fp);
    return (void *) bot;
}

static size_t scan_mem(void *target, void **begin, void **end)
{
    size_t num = 0;
    
    for (void **i = begin; i < end; i++)
    {
        if (*i == target)
        {
            num++;
        }
    }
    return num;
}

void scan(void *target, size_t *num_data, size_t *num_bss, size_t *num_stack)
{
    void *bot;

    bot = stack_bot();
    get_regs(regs);
    *num_data = scan_mem(target, &__data_start, &_edata);
    *num_bss = scan_mem(target, &_edata, &_end);
    *num_stack = scan_mem(target, regs[ESP], bot);
}

int main(void)
{
    size_t num_data;
    size_t num_bss;
    size_t num_stack;

    j = (void *) 0x51089f337b473c7c;
    (void) k;
    l = (void *) 0x51089f337b473c7c;
    scan((void *) 0x51089f337b473c7c, &num_data, &num_bss, &num_stack);
    printf("0x51089f337b473c7c data: %zu, bss: %zu, stack: %zu\n",
           num_data, num_bss, num_stack);
    return EXIT_SUCCESS;
}
