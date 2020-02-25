# carnaval

Two external libraries are used in this project. To compile with Arduino editor or Visual studio code you need to install these libraries trough the library manager.

The libraries are:
JC_Button 2.1.1 and FastLED 3.3.2

The looper contains the setup and continuous loop. There are several states the loop can be in. Menu and Program are the most common. Other states are button presses during program and towards other states. There is a Sleep state for standby effects when you are in the menu (ripples). Also an Error state is added that enables when buttons seems to be defective. 

The leddriver contains the actual programs and controlling of the leds. The led pins are on ports 9, 10 and 11 for the 3 seperate strips. The buttons are on port 2 and 4 as configured in the looper. 

