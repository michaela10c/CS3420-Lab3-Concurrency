#include "3140_concur.h"
#include <stdlib.h>
#include <MK64F12.h>

struct process_state {
    unsigned int * stack_pointer;
	struct process_state* next_process;
};

bool

void push_process(process_t* to_push){
    // if there is nothing in process_queue, then just add process
    if(process_queue == NULL){
        process_queue = to_push;
    }
    // if there is, then add to the end
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
    .// if empty, return NULL
    if(process_queue == NULL){
        return NULL;
    }
    // if there is, pop from the front 
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
        // allocate space for new_process and push it on to process_queue
        process_t * new_process = malloc(sizeof(process_t));
        new_process->stack_pointer = new_stack_pointer;
        push_process(new_process);
        return 0;
    }
}

void process_start (void){
    // set current_process to NULL
    current_process = NULL;

    // setup interrupt
    SIM->SCGC6 = SIM_SCGC6_PIT_MASK;    // Enable clock to PIT module
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;     // Disables standard PIT timers
    PIT->CHANNEL[0].LDVAL = 2250000;    // Set load value of zeroth PIT to 2.25 million for 1 second counts
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;  // enables Timer 0 interrupts
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;  // start Timer 0
    NVIC_EnableIRQ(PIT0_IRQHandler);    // Enable PIT0 Interrupts

    // call process_begin()
    process_begin();
}

unsigned int * process_select(unsigned int * cursp){
    // TODO: implement process_select
    return NULL:
}
