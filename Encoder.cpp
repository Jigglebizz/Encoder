#include "Arduino.h"
#include "Encoder.h"

// Create a new Encoder
Encoder::Encoder(int a, int b) {
    num_values = 128;
    position = 0;
    setPins(a, b);
}

Encoder::~Encoder() {

}

void Encoder::setPins(int a, int b) {
    pin_a = a;
    pin_b = b;

    pinMode(pin_a, INPUT_PULLUP);
    pinMode(pin_b, INPUT_PULLUP);

    uint8_t signal_state = digitalRead(pin_a) | (digitalRead(pin_b) << 1);
    ptr = 0;

    while ( (ENC_BUFF >> ptr) & ENC_MASK != signal_state) {
        ptr += 2;
    }
}

void Encoder::readPins() {
    uint8_t signal_state = digitalRead(pin_a) | (digitalRead(pin_b) << 1);
    uint8_t next_ptr = ptr + 2;

    if (next_ptr >= 8) next_ptr = 0;
    if ((ENC_BUFF >> next_ptr) & ENC_MASK == signal_state) {
        ptr = next_ptr;
        if (position < num_values - 1) position++;
    }

    next_ptr = ptr - 2;
    if (next_ptr < 0) next_ptr = 6;
    
    if ((ENC_BUFF >> next_ptr) & ENC_MASK == signal_state) {
        ptr = next_ptr;
        if (position > 0) position--;
    }
}