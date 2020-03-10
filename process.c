#include "3140_concur.h"
#include <stdlib.h>
#include <MK64F12.h>

struct process_state {
    unsigned int* stack_pointer;
	struct process_state* next_process;
};

void push_process(process_t* to_push){
    // if there is nothing to_push
    if(to_push == NULL){
        return;
    }
    // if there is nothing in process_queue, then just add process
    if(process_queue == NULL){
        process_queue = to_push;
    }
    // if there is, then add to the end
    else{
        process_t* tail = process_queue;
        while(tail->next_process != NULL){
            tail = tail->next_process;
        }
        tail->next_process = to_push;
    }
    to_push->next_process = NULL;
}

void pop_process(process_t* to_pop){
    .// if empty, return NULL
    if(process_queue == NULL){
        to_pop = NULL;
    }
    // if there is, pop from the front 
    else{
        to_pop = process_queue;
        process_queue = process_queue->next_process; 
        to_pop->next_process = NULL;
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
        // allocate space for new_process and push it on to process_queue
        process_t* new_process = malloc(sizeof(process_t));
        new_process->stack_pointer = new_stack_pointer;
        push_process(new_process);
        return 0;
    }
}

void process_start (void){
    // set current_process to NULL
    process_queue = NULL;
    current_process = process_queue;

    // setup interrupt
    SIM->SCGC6 = SIM_SCGC6_PIT_MASK;    // Enable clock to PIT module
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;     // Disables standard PIT timers
    PIT->CHANNEL[0].LDVAL = 2250000;    // Set load value of zeroth PIT to 2.25 million for 1 second counts
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;  // enables Timer 0 interrupts
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;  // start Timer 0
    NVIC_EnableIRQ(PIT0_IRQn);    // Enable PIT0 Interrupts

    // call process_begin()
    process_begin();
}

unsigned int * process_select(unsigned int* cursp){
    // if there is a current_process, pop it from queue
    if(current_process != NULL){
        process_t* temp = NULL;
        pop_process(temp);
        // if the stack pointer is not NULL, then process isn't done
        if(cursp != NULL){
            // so push it back on process_queue
            push_process(temp);

        else{
            // otherwise free the space
            free(temp);
        }
    }
    
    // retrieve next process
    current_process = process_queue;

    // if its empty, then return NULL, otherwise return stack pointer
    return current_process == NULL ? NULL : current_process->stack_pointer;
}
