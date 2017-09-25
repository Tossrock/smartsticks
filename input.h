#ifndef _INPUT_h
#define _INPUT_h

#if defined(ARDUINO) && ARDUINO >=100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#define NO_INPUT 255

const typedef enum {
  longpress, singleclick, doubleclick, tripleclick
} ClickType;

byte get_input();
void reset_button_state();

struct ButtonState {
    unsigned int downtime;
    unsigned int uptime;
    byte clicks;
    bool is_clicking;
    bool timing_out;
    bool longpressing;
};

extern ButtonState bstate;
#endif
