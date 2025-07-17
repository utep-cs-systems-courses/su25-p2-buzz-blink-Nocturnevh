#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "buzzer.h"
#include "switches.h"

typedef enum {
    STATE_ALT_LED,
    STATE_BLINK_LED,
    STATE_SONG,
    STATE_OFF
} State;

State current_state = STATE_OFF;

void state_machine() {
    switch (current_state) {
        case STATE_ALT_LED:
            buzzer_off();
            alternate_leds();
            break;
       /*case STATE_BLINK_LED:
            blink_led();
            buzzer_off();
            break; */
        case STATE_SONG:
            led_off();
            play_song();
            break;
        case STATE_OFF:
            led_off();
            buzzer_off();
            break;
    }
}

void main(void) 
{  
  configureClocks();
  switch_init();
  led_init();
  buzzer_init();

  enableWDTInterrupts();

  or_sr(0x18);  // CPU off, GIE on
} 

/* Switch on P2 (S2) */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */

    if(sw1_state_down) {
      current_state = STATE_ALT_LED;
    }
    else if(sw2_state_down) {
      current_state = STATE_BLINK_LED;
    }
    else if(sw3_state_down) {
      current_state = STATE_SONG;
    }
    else if(sw4_state_down) {
      current_state = STATE_OFF;
    }
    state_machine();
  }
}

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  if(current_state == STATE_ALT_LED){
    alternate_leds();
  }
} 
