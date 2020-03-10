#include "3140_concur.h"
#include "utils.h"

/**
 * This test has three processes. Two short process called short_process that
 * blinks an LED red 2 times and long process called long_process that 
 * add blinks a LED blue 10000 times. Since the two short processes are pushed
 * into the process queue first, they will execute first and then the long process
 * will execute.
**/
void short_process(void){
	int i;
	for (i=0; i < 2; i++) {
		delay();
        LEDRed_Toggle();
	}
}

void long_process(void){
	int i;
	for (i=0; i < 10000; i++) {
		delay ();
        LEDBlue_Toggle();
	}
}

int main (void){
    // initialize clock and LED
    int clock = SystemCoreClock;
    LED_Initialize();

    // create 2 short processes
    int i;
    for(i = 0; i < 2; i++){
        if(process_create(short_process,32) < 0){
            return -1;
        }
    }

    // create 1 long process
    if(process_create(long_process,32) < 0){
        return -1;
    }

    // start process
    process_start ();
    while (1);
    return 0;
}
