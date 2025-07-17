#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "switches.h"

void main(void) 
{  
  configureClocks();
  switch_init();
  led_init();

  or_sr(0x18);  // CPU off, GIE on
} 

/* Switch on P2 (S2) */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_light();	/* single handler for all switches */
  }
}
