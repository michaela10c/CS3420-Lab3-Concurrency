#include "3140_concur.h"
#include "utils.h"

/* *
 * This test has three processes. A short process called short_process that
 * blinks an LED red after 1 delay, a medium process called medium_process
 * that blinks an LED blue after 10 delays, and a long processed called 
 * long_process that blinks an LED green after 100 delays. Since, they are
 * pushed into the queue from longest to shortest, the longest will be processed,
 * but since this uses round-robin scheduling, the shortest will be done first and
 * then the medium one, and finally the longest. The end result is the LED blinks
 * red, then blue, and finally green.
 * */

void short_process(void){
	int i;
	for (i=0; i < 1; i++) {
		delay();
	}
    LEDRed_Toggle();
}

void medium_process(void){
	int i;
	for (i=0; i < 10; i++) {
		delay();
	}
    LEDBlue_Toggle();
}

void long_process(void){
	int i;
	for (i=0; i < 100; i++) {
		delay ();
	}
    LEDGreen_Toggle();
}

int main (void){
    // initialize clock and LED
    int clock = SystemCoreClock;
    LED_Initialize();

    // create 1 long process
    if(process_create(long_process,32) < 0){
        return -1;
    }

    // create 1 medium process
    if(process_create(medium_process,32) < 0){
        return -1;
    }

    // create 1 short process
    if(process_create(short_process,32) < 0){
        return -1;
    }

    // start process
    process_start ();
    while (1);
    return 0;
}
