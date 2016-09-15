Alpha version; tested only on ARM mbed Nucleo F401RE board.
Can be found here: 
https://developer.mbed.org/users/AfdhalAtiffTan/code/FoodComputerARM-alpha/

It meant to replace Arduino boards and use ARM processor instead.
It serves the same purpose as "Food_Computer_Draft_V2" (with compatible pinout).

Differences:
-It uses MODBUS RTU instead of non-standard uart protocol.
-Analogue pins are 3.3V, make sure to divide the voltage first.
-Has three debugging LEDs.
-Some of the sensors data were scaled by 100;

Logs:
-Tried to use RTOS, but failed because the software-serial library can't handle it (took a long time to figure this out).

-Because RTOS failed, it uses Ticker instead.

-Most of the libraries are download from mbed community.

-The modbus library was a modified Arduino library.

-Programming using ST-Link achieves more consistent results.

-Sometimes, the online compiler gives peculiar result, try to compile using Keil (by exporting it to local pc). In other words, sometimes, both online and Keil compiler can give very different behaviour (such as failed I2C etc.). 

-If uses mbed to upload the program, don't forget to manually reset the ARM board by pushing the reset button before every each upload. Not too sure why but it was discovered that it gives more predictable result.

-Reconnecting the USB cable and Clicking on "Compile" then "Compile All" multiple times sometimes helps.

-If errors were found, sometimes clicking the "Update" and "Update All" helps. 

-All of the communications tested only on oscilloscope, not on the actual sensors.

-The program will not start without CO2 sensor because of the blocking-call used.

-Don't request data faster than 1 second because the update rate is 100ms (not including sensors).

-Searching library/program on the mbed IDE gives incomplete results. Better to use mbed website or Google.

Notes:
-Program here was exported from mbed environment.
-The program is far from perfect, bugs might exists. 
-It was not written to look proper, yet.
-The code may look ugly as it was coded using the glue method.
-If the Raspberry Pi appears not to be stuck, add a 5 seconds delay after relay initialisation. Fault from: Modbus library.


To-dos:
-Proper documentation and Doxygen.
-Format the code so that it looks more professional and manageable.
-Test all communication on all sensors.

