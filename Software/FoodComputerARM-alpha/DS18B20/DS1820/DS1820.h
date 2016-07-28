/* mbed DS1820 Library, for the Dallas (Maxim) 1-Wire Digital Thermometer
 * Copyright (c) 2010, Michael Hagberg Michael@RedBoxCode.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MBED_DS1820_H
#define MBED_DS1820_H

#include "mbed.h"

// ****** THIS GLOBAL VARIABLES MUST BE DEFINED IN main.cpp

// Global variables shared between all DS1820 objects
//bool DS1820_done_flag;
//int  DS1820_last_descrepancy;
//char DS1820_search_ROM[8];

/** DS1820 Dallas 1-Wire Temperature Probe
 *
 * Example:
 * @code
 * #include "mbed.h"
 *
 * #include "TextLCD.h"
 * #include "DS1820.h"
 *
 * TextLCD lcd(p25, p26, p21, p22, p23, p24, TextLCD::LCD16x2); // rs, e, d0-d3, layout
 *
 * const int MAX_PROBES = 16;
 * DS1820* probe[MAX_PROBES];
 *
 * int main() {
 *     int i;
 *     int devices_found=0;
 *     // Initialize the probe array to DS1820 objects
 *     for (i = 0; i < MAX_PROBES; i++)
 *         probe[i] = new DS1820(p27);
 *     // Initialize global state variables
 *     probe[0]->search_ROM_setup();
 *     // Loop to find all devices on the data line
 *     while (probe[devices_found]->search_ROM() and devices_found<MAX_PROBES-1)
 *         devices_found++;
 *     // If maximum number of probes are found, 
 *     // bump the counter to include the last array entry
 *     if (probe[devices_found]->ROM[0] != 0xFF)
 *         devices_found++;
 * 
 *     lcd.cls();
 *     if (devices_found==0)
 *         lcd.printf("No devices found");
 *     else {
 *         while (true) {
 *             probe[0]->convert_temperature(DS1820::all_devices);
 *             lcd.cls();
 *             for (i=0; i<devices_found; i++) {
 *                 lcd.printf("%3.1f ",probe[i]->temperature('f'));
 *             }
 *         }
 *     }
 * }
 * @endcode
 */
 
class DS1820 {
public:
    enum devices{
        this_device,     // command applies to only this device
        all_devices };   // command applies to all devices

    /** Create a probe object connected to the specified pins
     *
     * @param data_pin DigitalInOut pin for the data bus
     * @param power_pin DigitalOut pin to control the power MOSFET
     */
    DS1820(PinName data_pin, PinName power_pin); // Constructor with parasite power pin

    /** Create a probe object connected to the specified pin
     *  this is used when all probes are externally powered
     *
     * @param data_pin DigitalInOut pin for the data bus
     */
    DS1820(PinName data_pin);

    /** ROM is a copy of the internal DS1820's ROM
      * It's created during the search_ROM() or search_alarm() commands
      *
      * ROM[0] is the Dallas Family Code
      * ROM[1] thru ROM[6] is the 48-bit unique serial number
      * ROM[7] is the device xCRC
      */
    char ROM[8];
    #define FAMILY_CODE ROM[0]
    #define FAMILY_CODE_DS1820 0x10
    #define FAMILY_CODE_DS18S20 0x10
    #define FAMILY_CODE_DS18B20 0x28
    
    /** RAM is a copy of the internal DS1820's RAM
      * It's updated during the read_RAM() command
      * which is automaticaly called from any function
      * using the RAM values.
      */
    char RAM[9];
    
    /* This function copies the DS1820's RAM into the object's
     * RAM[].
     */
    void read_RAM();

    /** This routine initializes the global variables used in
      * the search_ROM() and search_alarm() funtions. It should
      * be called once before looping to find devices.
      */
    void search_ROM_setup();

    /** This routine will search for an unidentified device
      * on the bus. It uses the variables in search_ROM_setup
      * to remember the pervious ROM address found.
      * It will return FALSE if there were no new devices
      * discovered on the bus.
      */
    bool search_ROM();

    /** This routine will search for an unidentified device
      * which has the temperature alarm bit set. It uses the 
      * variables in search_ROM_setup to remember the pervious 
      * ROM address found. It will return FALSE if there were 
      * no new devices with alarms discovered on the bus.
      */
    bool search_alarm();

    /** This routine will read the ROM (Family code, serial number
      * and Checksum) from a dedicated device on the bus.
      *
      * NOTE: This command can only be used when there is only one 
      *       DS1820 on the bus. If this command is used when there 
      *       is more than one slave present on the bus, a data 
      *       collision will occur when all the DS1820s attempt to 
      *       respond at the same time.
      */
    void read_ROM();

    /** This routine will initiate the temperature conversion within
      * a DS1820. There is a built in 750ms delay to allow the 
      * conversion to complete.
      *
      * To update all probes on the bus, use a statement such as this:
      * probe[0]->convert_temperature(DS1820::all_devices);
      *
      * @param allows the fnction to apply to a specific device or
      * to all devices on the 1-Wire bus.
      */
    void convert_temperature(devices device=this_device);

    /** This function will return the probe temperature. This function
      * uses the count remainding values to interpolate the temperature
      * to about 1/150th of a degree. Whereas the probe is not spec to
      * that precision. It does seem to give a smooth reading to the
      * tenth of a degree.
      *
      * @param scale, may be either 'c' or 'f'
      * @returns temperature for that scale
      */
    float temperature(char scale='c');
    
    /** This function calculates the ROM checksum and compares it to the
      * xCRC value stored in ROM[7].
      *
      * @returns true if the checksum matches, otherwise false.
      */
    bool ROM_checksum_error();

    /** This function calculates the RAM checksum and compares it to the
      * xCRC value stored in RAM[8].
      *
      * @returns true if the checksum matches, otherwise false.
      */
    bool RAM_checksum_error();

    /** This function returns the values stored in the temperature
      * alarm registers. 
      *
      * @returns a 16 bit integer of TH (upper byte) and TL (lower byte).
      */
    bool set_configuration_bits(unsigned int resolution);
    
    /** This function sets the temperature resolution for the DS18B20
      * in the configuration register.
      *
      * @param a number between 9 and 12 to specify the resolution
      * @returns true if successful
      */
    int read_scratchpad();
    
    /** This function will store the passed data into the DS1820's RAM.
      * Note: It does NOT save the data to the EEPROM for retention
      * during cycling the power off and on.
      *
      * @param a 16 bit integer of TH (upper byte) and TL (lower byte).
      */
    void write_scratchpad(int data);
    
    /** This function will transfer the TH and TL registers from the
      * DS1820's RAM into the EEPROM.
      * Note: There is a built in 10ms delay to allow for the
      * completion of the EEPROM write cycle.
      *
      * @param allows the fnction to apply to a specific device or
      * to all devices on the 1-Wire bus.
      */
    void store_scratchpad(devices device=this_device);

    /** This function will copy the stored values from the EEPROM
      * into the DS1820's RAM locations for TH and TL.
      *
      * @param allows the function to apply to a specific device or
      * to all devices on the 1-Wire bus.
      */
    int recall_scratchpad(devices device=this_device);

    /** This function will return the type of power supply for
      * a specific device. It can also be used to query all devices
      * looking for any device that is parasite powered.
      *
      * @returns true if the device (or all devices) are Vcc powered,
      * returns false if the device (or ANY device) is parasite powered.
      */
    bool read_power_supply(devices device=this_device);
private:
    bool _parasite_power;
    char CRC_byte (char xCRC, char byte );
    bool onewire_reset();
    void match_ROM();
    void skip_ROM();
    bool search_ROM_routine(char command);
    void onewire_bit_out (bool bit_data);
    void onewire_byte_out(char data);
    bool onewire_bit_in();
    char onewire_byte_in();

protected:
    DigitalInOut _datapin;
    DigitalOut _parasitepin;
};


#endif
