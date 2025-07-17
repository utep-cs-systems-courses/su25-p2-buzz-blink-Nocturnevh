#include <msp430.h>
#include "led.h"
#include "switches.h"

void led_init()
{
    P1DIR |= LEDS;
    P1OUT &= ~LED_GREEN;
    P1OUT |= LED_RED;
}

void led_off(){
    P1OUT &= ~LEDS;
}

int secondCount = 0;
void alternate_leds(){
    secondCount ++;
    if (secondCount >= 250) { 	/* once each sec... */
      secondCount = 0;		    /* reset count */
      P1OUT ^= LEDS;            /* toggle LEDS */
    }
}

