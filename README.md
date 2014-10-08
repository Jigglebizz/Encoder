#Encoder Library#

Encoder lets you use encoders with a very low processor footprint. This should minimize the time needed to process encoder usage so that programs execute smoothly.

Two interrupts must be available to use this library with maximum performance. They must be attached to state-change on your encoder pins, and reference your encoder's "readPins" function.

If you do not have two interrupts available, you may simply call readPins() as often as possible. This may offer decent results for low-resolution encoders turned by human hands.

Encoder was created by Michael Hawthorne (Jigglebizz)