#include "Arduino.h"
#include "Encoder.h"

State* Encoder::state_wheel;
int Encoder::state_wheel_set_up = 0;

// Create a new Encoder
// Provide pins a and b
Encoder::Encoder(int a, int b) {
    num_values = 128;
    position = 0;

    if (!state_wheel_set_up) {
        setupStateWheel();
    }

    setPins(a, b);
}

// Delete Encoder. Nothing in here
Encoder::~Encoder() {

}

// Sets the pins and runs initialization (reads pin values)
void Encoder::setPins(int a, int b) {
    noInterrupts();
    pin_a = a;
    pin_b = b;

    pinMode(pin_a, INPUT_PULLUP);
    pinMode(pin_b, INPUT_PULLUP);

    uint8_t signal_state = digitalRead(pin_a) | (digitalRead(pin_b) << 1);

    State* s = state_wheel;
    while (signal_state != s->mask) {
        s = s->next;
    }

    current_state = s;

    interrupts();
}

void Encoder::setupStateWheel() {
    State *a, *b, *c, *d;

    a = (State*)malloc(sizeof(State));
    b = (State*)malloc(sizeof(State));
    c = (State*)malloc(sizeof(State));
    d = (State*)malloc(sizeof(State));

    a->mask = 0x0; b->mask = 0x01; c->mask = 0x3, d->mask = 0x2;

    a->next = b; a->prev = d;
    b->next = c; b->prev = a;
    c->next = d; c->prev = b;
    d->next = a; d->prev = c;

    state_wheel = a;

    state_wheel_set_up = 1;
}

// Gets the state of the pins and determines relative location.
// This should be referenced by your interrupt service routines.
void Encoder::readPins() {
    // Get pin values
    uint8_t signal_state = digitalRead(pin_a) | (digitalRead(pin_b) << 1);

    if (current_state->next->mask == signal_state) {
        current_state = current_state->next;
        if (position < num_values) position++;
    } else if (current_state->prev->mask == signal_state) {
        current_state = current_state->prev;
        if (position > 0) position--;
    }
}

int Encoder::getPosition() {
    return position;
}