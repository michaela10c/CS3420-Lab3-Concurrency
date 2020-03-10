#include "3140_concur.h"
#include <stdlib.h>
#include <MK64F12.h>

struct process_state {
    unsigned int * stack_pointer;
	struct process_state* next_process;
};

void push_process(process_t* to_push){
    // if there is nothing in queue, then queue pointer points to process pointer
    if(process_queue == NULL){
        process_queue = to_push;
    }
    // if there is, then queue's tail points to process pointer
    else{
        process_t *tail = process_queue;
        while(tail->next_process != NULL){
            tail = tail->next_process;
        }
        tail->next_process = to_push;
    }
    toAdd->next_process = NULL;
}

process_t* pop_process(){
    .// if there is nothing to pop return NULL
    if(process_queue == NULL){
        return NULL;
    }
    // if there, pop from the front 
    else{
        process_t* to_pop = process_queue;
        process_queue = process_queue->next_process;
        return to_pop;
    }
}

int process_create (void (*f)(void), int n){
    // initialize process stack
    unsigned int *new_stack_pointer = process_stack_init(f, n);

    // if cannot allocate stack space, return -1s
    if(new_stack_pointer == NULL){
        return -1;
    }
    else{
        // allocate space for a process and push it on to process queue
        process_t * new_process = malloc(sizeof(process_t));
        new_process->stack_pointer = new_stack_pointer;
        push_process(new_process);
        return 0;
    }
}