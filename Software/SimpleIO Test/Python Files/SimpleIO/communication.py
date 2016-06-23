import serial

class MCUcomm:
    mcu = serial

    def openMCU(self, comportinuse):
        self.mcu = serial.Serial(
            port=comportinuse,
            baudrate=9600,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=3,
            xonxoff=False,
            rtscts=False,
            writeTimeout=None,
            dsrdtr=False,
            interCharTimeout=None)

    def closeMCU(self):
        self.mcu.close()  # close com port

    def getAllSensor(self):
        self.mcu.write('#') # send '#' to get all sensor readings from MCU
        rx = self.mcu.readline()
        print "The received data is: " + rx
        return rx

    def mcuStatus(self):
        return self.mcu.isOpen()  # see if port is open

    ###<relay control>###

    ###<DC relays>### //simple relay control
    def ventilationFanON(self):
        self.mcu.write('A\r')

    def ventilationFanOFF(self):
        self.mcu.write('B\r')

    def circulationFanON(self):
        self.mcu.write('C\r')

    def circulationFanOFF(self):
        self.mcu.write('D\r')

    def panelLightON(self):
        self.mcu.write('E\r')

    def panelLightOFF(self):
        self.mcu.write('F\r')

    def humidifierON(self):
        self.mcu.write('G\r')

    def humidifierOFF(self):
        self.mcu.write('H\r')
    ###</DC relays>###

    ###<AC relays>###
    def growLightON(self):
        self.mcu.write('I\r')

    def growLightOFF(self):
        self.mcu.write('J\r')

    def heaterON(self):
        self.mcu.write('K\r')

    def heaterOFF(self):
        self.mcu.write('L\r')

    def waterPumpON(self):
        self.mcu.write('M\r')

    def waterPumpOFF(self):
        self.mcu.write('N\r')

    def airPumpON(self):
        self.mcu.write('O\r')

    def airPumpOFF(self):
        self.mcu.write('P\r')
    ###</AC relays>###

    ###</relay control>###