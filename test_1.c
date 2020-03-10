#include "3140_concur.h"
#include "utils.h"

/**
 * This test has two processes. A short process called short_process that
 * blinks an LED red 2 times and long process called long_process that 
 * add blinks a LED blue 10000 times. This longer process will pushed to the
 * process queue first and then 10 short processes will be pushed to the process
 * queue. The expected behavior is to see the LED blink blue a couple times and
 * then we should expect the LED blink red until it goes back to blue for a
 * long time. We expect this behavior because the scheduler should allow the
 * long process to start (since it was pushed to the queue first), but not finish
 * (since it is a long process) and instead let the smaller processes finish and
 * then continue with the longer process.
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

    // create 1 long process
    if(process_create(long_process,32) < 0){
        return -1;
    }
    
    // create 10 short processes
    int i;
    for(i = 0; i < 10; i++){
        if(process_create(short_process,32) < 0){
            return -1;
        }
    }

    // start process
    process_start ();
    while (1);
    return 0;
}
