This Draft V1 is meant for backup purposes. It is far from ideal and may contains a lot of bugs.

It should be able to control arduino (or any capable microcontroller) via serial communication (onboard usb-ttl).
 
It has:
-simple relay control
-simple relay timer
-non-standard serial communication (simple one-character ascii commands)
-charts for sensors (polled every 10 seconds)
-gauges for sensor (polled every 10 seconds)
-simple webcam interface (just live stream, no capture yet)
-simple close-loop control (bang-bang controller) for lights, humidity and temperature
-simple MQTT client
-basic system control (Pi reboot and shutdown)


To use it, simply install nodered by using [Peter Scargill]'s scripts:

https://bitbucket.org/snippets/scargill/ayAG6/include-for-jessie-script-call-it

https://bitbucket.org/snippets/scargill/L8zjE/raspbian-jessie-and-node-red-script-for

For installation explanation by [Peter Oakes] (near minute 9:30):
https://www.youtube.com/watch?v=uyx2KZNxhe4

The installation should include all required libraries. If not, they can be found on:
http://flows.nodered.org/ or
https://www.npmjs.com/

Optional: "Motion" package must be installed on the Pi in order to use the usb webcam:
https://pimylifeup.com/raspberry-pi-webcam-server/

After installation, node-red can be access via:
main interface: http://localhost:1880
web UI: http://localhost:1880/ui


Then, import the json file (flows_FoodComputerPi.json) by copying and paste it into (localhost:1880)>(Menu)>(Import)>(Clipboard)

Optional: Port forwarding, to allow external access.


Future version:
-Should include modbus-rtu. Credit: [Sam Walsh].