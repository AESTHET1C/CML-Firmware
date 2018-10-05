# Overview of Error Codes

During normal operation, the status LED on the CMDCB remains off. However, if the Firmware detects an error, this LED is used to show the detected error. Error codes are implemented by blinking the LED, where the number of consecutive blinks determines the error code.

If multiple errors are detected, they will be shown in ascending order.

All errors are cleared by using the FORW and/or BACK button, or resetting the CMDCB.

# Error 1 - The endstop engaged early
### Trigger Conditions
+ The endstop engaged while the bucket was retracting and the encoder did not sense an appropriate travel distance

### Potential Causes
+ The endstop was engaged due to an external event

### Action Taken by Firmware
+ The encoder position is homed according to the current motor position

### What To Do
+ Verify the endstop is clear of unwanted obstructions
+ Manually retract the bucket to the proper homed position

# Error 2 - The endstop failed to engage
### Trigger Conditions
+ The endstop did not engage while the bucket was retracting and the encoder sensed an excessive travel distance

### Potential Causes
+ A mechanical failure occurred
+ The endstop failed
+ The motor was not properly homed [see error 1]

### Action Taken by Firmware
+ The encoder position is homed according to the current motor position

### What To Do
+ Verify the string is properly attached to the bucket and winch
+ Manually retract the bucket to the proper homed position
+ Repair the endstop, if required

# Error 3 - The motor jammed
### Trigger Conditions
+ The encoder failed to detect motor movement while the motor was enabled

### Potential Causes
+ The motor was prevented from moving due to an external force
+ The encoder failed

### Action Taken by Firmware
+ The motor is disabled until the error is cleared

### What To Do
+ Verify the encoder wheel is properly attached
+ Clear any mechanical obstructions
+ Clear the error code
