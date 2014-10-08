#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

// constants for comparison
#define ENC_PIN_A 0
#define ENC_PIN_B 1

// Circular buffer to compare values against.
// Looks like |11|10|00|01| in binary
#define ENC_BUFF  0xE1

// Mask for buffer values
// looks like 00000011 in binary
#define ENC_MASK  0x03

class Encoder {
  private:
    volatile uint8_t ptr;

    int pin_a, pin_b;
  public:
    volatile int position;
    int num_values;

    Encoder(int, int);
    ~Encoder();
    void setPins(int, int);

    void readPins();
};




#endif