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
    
    int pin_a, pin_b;
    volatile int position;
    static State* state_wheel;
    static int state_wheel_set_up;
    volatile State* current_state;

  public:
    int num_values;

    Encoder(int, int);
    ~Encoder();
    void setPins(int, int);

    void readPins();
    int getPosition();
    static void setupStateWheel();
};




#endif