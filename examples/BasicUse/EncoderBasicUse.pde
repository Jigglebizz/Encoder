///////////////////////////////////////////////////////////////////////////////
//                         Encoder Libraray Example                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// This is, yet again, another Encoder library. I'm hoping that this one is  //
// lightweight enough to justify its existence. No velocity sensitivity or   //
// other bells and whistles, just a fast library that uses minimal CPU.      //
// This was achieved by storing each state of grey-code in a circular list.  //
//                                                                           //
// I created this because I foresee encoders being used widely, especially   //
// by Arduino Due users. Encoders are prevalent in many consumer electronic  //
// devices because they offer excellent interaction control. Paired with an  //
// LCD or OLED, many creative UX experiences can be explored with relative   //
// ease, thanks to the Arduino community.                                    //
//                                                                           //
// I wrote this entirely in C++ (no asm) in order to accomidate non-AVR uC's.//
// The Arduino Due allows all of its pins to be used as interrupts, so you   //
// can use up to 27 encoders at once.                                        //
///////////////////////////////////////////////////////////////////////////////


#include <Encoder.h>

#define enc_pin_a   22
#define enc_pin_b   23
Encoder* enc;

void encoder_interrupt() { enc->readPins(); }

void setup() {
    enc = new Encoder(enc_pin_a, enc_pin_b); 

    attachInterrupt(enc_pin_a, encoder_interrupt, CHANGE);
    attachInterrupt(enc_pin_b, encoder_interrupt, CHANGE);
    Serial.begin(9600);

}

void loop() {
    Serial.println(enc->getPosition()); 
}
