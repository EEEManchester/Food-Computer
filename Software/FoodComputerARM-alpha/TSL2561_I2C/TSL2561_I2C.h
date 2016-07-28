#ifndef TSL2561_I2C_H
#define TSL2561_I2C_H
#include "mbed.h"

//Defines 
#define TSL_SLAVE_ADDRESS       0x39

#define TSL_CONTROL             0x00
#define TSL_TIMING              0x01
#define TSL_THRESHLOWLOW        0x02
#define TSL_THRESHHIGHLOW       0x04
#define TSL_INTERRUPT           0x06
#define TSL_ID                  0x0A
#define TSL_DATA0LOW            0x0C
#define TSL_DATA1LOW            0x0E

/** TSL2561_I2C class.
 *  Abstraction for TAOS TSL2561 Light-To-Digital Converter.
 *
 * Example:
 * @code
 * #include "mbed.h"
 * #include "TSL2561_I2C.h"
 *
 * TSL2561_I2C lum_sensor( p9, p10 );
 * 
 * int main() {
 *     lum_sensor.enablePower();
 *
 *     int rgb_readings[4];
 *     while(1) {
 *         printf( "Luminosity: %4.2f\n", lum_sensor.getLux() );
 *         wait_ms( 100 );
 *     }
 * }
 * @endcode
 */
class TSL2561_I2C {
public:
    /** Create TSL2561_I2C instance
     *
     * @param sda sda pin for I2C
     * @param scl scl pin for I2C
     */
    TSL2561_I2C( PinName sda, PinName scl );
    
    /** Read broadband photodiode (visible plus infrared)
     *
     * @returns
     *     Irradiance measured
     */
    int getVisibleAndIR();
    
    /** Read infrared-responding photodiode
     *
     * @returns
     *     Irradiance measured
     */
    int getIROnly();
    
    /** Read sensors and calculate Illuminance in lux
     *
     * @returns
     *     Illuminance (lux)
     */
    float getLux();
    
    /** Power up the device.
     *
     * @returns
     *     1 if successful
     *     0 if otherwise
     */
    int enablePower();
    
    /** Disable power to the device.
     *
     * @returns
     *     1 if successful
     *     0 if otherwise
     */
    int disablePower();
    
    /** Check if power to the device is enabled.
     *
     * @returns
     *     1 if power ON
     *     0 if power OFF
     */
    bool isPowerEnabled();
 
    /** Return present gain value
     *
     * @returns
     *     1 (low gain mode)
     *     16 (high gain mode)
     */
    int readGain();
    
    /** Set gain
     *
     * @param gain Gain must be either 1 or 16  
     *
     * @returns
     *     1 if successful
     *     0 if otherwise
     */
    int setGain( const int gain );
    
    /** Read the current integration time.
     *
     * @returns
     *     Integration time in milliseconds
     */
    float readIntegrationTime();
    
    /** Set integration time.
     *
     * @param itime Integration time to set in milliseconds. Should be 13.7, 101 or 402.
     *
     * @returns
     *     1 if successful
     *     0 if otherwise
     */
    int setIntegrationTime( const float itime );
    
    /** Read the low trigger point for the comparison function for interrupt generation.
     *
     * @returns
     *     Low threshold value
     */
    int readLowInterruptThreshold();
    
    /** Read the high trigger point for the comparison function for interrupt generation.
     *
     * @returns
     *     High threshold value
     */
    int readHighInterruptThreshold();
    
    /** Set the low trigger point for the comparison function for interrupt generation.
     *
     * @param threshold Low threshold value
     *
     * @returns
     *     1 if successful
     *     0 if otherwise
     */
    int setLowInterruptThreshold( const int threshold );
    
    /** Set the high trigger point for the comparison function for interrupt generation.
     *
     * @param threshold High threshold value
     *
     * @returns
     *     1 if successful
     *     0 if otherwise
     */
    int setHighInterruptThreshold( const int threshold );
    
    /** Return the number of consecutive values out of range (set by the low and high thresholds) required to trigger an interrupt
     *
     * @returns
     *     interrput persistence
     */
    int readInterruptPersistence();
    
    /** Set the number of consecutive values out of range (set by the low and high thresholds) required to trigger an interrupt
     *
     * @param persistence Value to set. 0: interrupt every ADC cycle, 1-15: corresponding number of cycles until interrupt
     *
     * @returns
     *     1 if successful
     *     0 if otherwise
     */
    int setInterruptPersistence( const int persistence );
    
    /** Check the interrupt function's operation mode
     *
     * @returns
     *     0: Interrupt output disabled
     *     1: Level Interrupt
     *     2: SMBAlert compliant
     *     3: Test Mode
     */
    int readInterruptControl();
    
    /** Set the interrupt function's operation mode
     *
     * @param control Value to set. 0: interrupt every ADC cycle, 1-15: corresponding number of cycles until interrupt
     *
     * @returns
     *     1 if successful
     *     0 if otherwise
     */
    int setInterruptControl( const int control );
    
    /** Clear the interrupt, allowing normal operation to resume.
     *  (writes 0b11000000 to command register to clear interrupt)
     *
     * @returns
     *     1 if successful
     *     0 if otherwise
     */
    int clearInterrupt();
    
    /** Get integer indicating part number
     *
     * @returns
     *     0: TSL2560
     *     1: TSL2561
     */
    int getPartNumber();
    
    /** Get revision number
     *
     * @returns
     *     Revision number
     */
    int getRevisionNumber();

private:
    I2C i2c;
    
    int writeSingleRegister( char address, char data );
    int writeMultipleRegisters( char address, char* data, int quantity );
    char readSingleRegister( char address );
    int readMultipleRegisters( char address, char* output, int quantity );
};

#endif