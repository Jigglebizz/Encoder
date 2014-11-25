#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

typedef struct state {
    uint8_t mask;
    struct state* next;
    struct state* prev;
} State;

class Encoder {
  private:
    int pin_a, pin_b;               // pin values
    static State* state_wheel;      // State wheel data structure
    static int state_wheel_set_up;  // If we've set up the state wheel
    volatile State* current_state;  // This encoder's state
    volatile int position;          // position value

    static void setupStateWheel();
  public:
    int num_values;                 // Number of values this encoder can express

    Encoder(int, int);
    ~Encoder();
    void setPins(int, int);
    void readPins();
    int getPosition();
    void setPosition(int);
};




#endif