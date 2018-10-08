# Overview of the Firmware

The Coal Mine Loader Firmware ("CML Firmware") is written specifically for the Coal Mine Distributed Controller Board ("CMDCB"), an ATmega 328P-powered board that attaches to the Coal Loader on the Eli Whitney Museum's seasonal train display. The Firmware allows interaction with the Coal Loader in the following ways:

+ Offer a single arcade button for the public to press
+ Automatically lower and raise the coal loader's bucket when the arcade button is pressed
+ Power the electromagnet within the coal loader's bucket to [pretend to] pick up objects
+ Offer two under-the-table override buttons for staff to move the motor forward or backwards
+ Display error codes and realtime endstop engagement to staff via several under-the-table status LEDs

The Firmware utilizes an optoisolator endstop along with a high-precision encoder to detect potentially erroneous or destructive motor control. In the event that a malfunction does happen, the Firmware will display an error code on the CMDCB's LED and potentially disable the operation of the motor. The Firmware is generally permissive to errors, to prevent non-critical issues from requiring human input.


# Wiring Connections

[TODO]


# Error Codes

See **Error Codes Documentation** for a list of error codes and their operation.
