// This implementation relies on a 'state wheel' data structure. The state wheel
// contains 4 states - one for each possible value in grey code, linked together
// in a cicular pattern. The following ascii drawing shows visually what the 
// structure is:
//
//           --> [ 11 ] <--
//          |              |
//          V              V
//       [ 01 ]          [ 10 ]
//          ^              ^
//          |              |
//           --> [ 00 ] <--
//
// This structure is static, and therefor common among all Encoder objects you
// may instantiate. Each encoder points to one of these states, and whenever
// an interrupt is recieved it checks to the left and right of that state
// To see which way it's moving. It then increments or decrements its value
// accordingly.
//
// This implementation is very fast. Besides from constructing the signal
// state, this requires no math. The remainder of the algorithm only contains
// instructions to load from memory and compare values.

#include "Arduino.h"
#include "Encoder.h"

State* Encoder::state_wheel;
int Encoder::state_wheel_set_up = 0;

/**
 * Encoder::Encoder - creates a new Encoder object
 * @a - pin A
 * @b - pin B
 */
Encoder::Encoder(int a, int b) {
    num_values = 128;
    position = 0;
    resolution = 1;

    if (!state_wheel_set_up) {
        setupStateWheel();
    }

    setPins(a, b);
}

/**
 * Encoder::~Encoder - deletes the encoder
 */
Encoder::~Encoder() {
    // Nothing to put here
}

/**
 * Encoder::setPins - Sets the pins and runs initialization (reads pin values)
 * @a - pin A
 * @b - pin B
 */
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

/**
 * Encoder::setupStateWheel - Sets up the static state wheel to which all
 *                            encoder objects refer. This should only be
 *                            run once per solution.
 */
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

/**
 *  Encoder::readPins - Gets the state of the pins and determines relative
 *                      location. This should be referenced by your interrupt
 *                      service routines (ISR's).
 */
void Encoder::readPins() {
    // Get pin values
    uint8_t signal_state = digitalRead(pin_a) | (digitalRead(pin_b) << 1);

    int nominal_position = position * resolution;
    if (current_state->next->mask == signal_state) {
        current_state = current_state->next;
        if (nominal_position < num_values) position++;
    } else if (current_state->prev->mask == signal_state) {
        current_state = current_state->prev;
        if (position > 0) position--;
    }
}

/**
 * Encoder::getPosition - Returns the position of the encoder
 */
int Encoder::getPosition() {
    return position * resolution;
}

/**
 * Encoder::setPosition - Sets the position of the encoders
 * @position - Position to set it to
 */
void Encoder::setPosition(int position) {
    this->position = position / resolution;
}