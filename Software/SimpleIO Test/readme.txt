This folder (SimpleIO) contains simple Arduino sketch and Python based GUI
to test all UART communications, sensors, and actuators operations.
Both drafts are far from perfect, it is meant for quick hardware/software test.
Might be full of bugs. Definitely needs improvement.


For Arduino:
-Just upload the sketch normally.

To run the GUI:
-Install Python 2.7
-Install PyQt 4
-Install pySerial
-run main.py
-optional: install pyCharm

Sensors used:
(software uart) http://www.co2meter.com/products/cozir-0-2-co2-sensor
(DHT22) https://www.seeedstudio.com/item_detail.html?p_id=838
(I2C) https://www.seeedstudio.com/item_detail.html?p_id=1281
(ADC) http://www.dfrobot.com/index.php?route=product/product&product_id=1025#.V2vjyPkrKHs
(ADC and onewire) http://www.dfrobot.com/index.php?route=product/product&product_id=1123#.V2vrN_krKHu

The Arduino sketch designed so that it can be run on either uno or mega (cost optimisation).

The Python GUI designed in PyQt Designer (auto generated).
I learn it from here:
https://www.youtube.com/watch?v=Dmo8eZG5I2w&list=PLQVvvaa0QuDdVpDFNq4FwY9APZPGSUyR4&index=16