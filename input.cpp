#include "input.h"

#define SWITCH_PIN 3
#define DOUBLECLICK_INTERVAL 300
#define LONGPRESS_INTERVAL 1000



ButtonState bstate;

void reset_button_state(){
  bstate.uptime = 0;
  bstate.downtime = 0;
  bstate.clicks = 0;
  bstate.is_clicking = false;
  bstate.timing_out = false;
  bstate.longpressing = false;
}

/* Parses the button state-stream into interprettable click/double click/long-press */
byte get_input(){
  unsigned long t = millis();
  byte switchposition = digitalRead(SWITCH_PIN);
  if (switchposition == LOW) {
    // pressed
    if (!bstate.is_clicking){
      bstate.downtime = t;
      bstate.is_clicking = true;
      bstate.timing_out = false;
      bstate.clicks++;
    } else { 
      if (t - bstate.downtime > LONGPRESS_INTERVAL && !bstate.longpressing){
        bstate.longpressing = true;
        return longpress;
      }
    }
  } else {
    // up
    if (bstate.is_clicking){
      // released
      bstate.uptime = t;
      bstate.is_clicking = false;
      bstate.timing_out = true;
    } else {
      // up
      if (bstate.timing_out){
        // In the timeout period
        if (t - bstate.uptime > DOUBLECLICK_INTERVAL){
          byte input;
          // Exceeded doubleclick interval, interpret as is
          if (bstate.longpressing){
            input = NO_INPUT;
          } else {
            input = bstate.clicks;
          }
          reset_button_state();
          return input;
        } 
      }
    }
  }
  return NO_INPUT;
}
