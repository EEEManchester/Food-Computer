#include "TSL2561_I2C.h"

TSL2561_I2C::TSL2561_I2C( PinName sda, PinName scl ) : i2c( sda, scl ){   
    i2c.frequency(100000);
    enablePower();
}

int TSL2561_I2C::writeSingleRegister( char address, char data ){
    char tx[2] = { address | 160, data }; //0d160 = 0b10100000
    int ack = i2c.write( TSL_SLAVE_ADDRESS << 1, tx, 2 );
    return ack;
}

int TSL2561_I2C::writeMultipleRegisters( char address, char* data, int quantity ){
    char tx[ quantity + 1 ];
    tx[0] = address | 160;
    for ( int i = 1; i <= quantity; i++ ){
        tx[ i ] = data[ i - 1 ];
    }
    int ack = i2c.write( TSL_SLAVE_ADDRESS << 1, tx, quantity + 1 );
    return ack;
}

char TSL2561_I2C::readSingleRegister( char address ){
    char output = 255;
    char command = address | 160; //0d160 = 0b10100000
    i2c.write( TSL_SLAVE_ADDRESS << 1, &command, 1, true );
    i2c.read( TSL_SLAVE_ADDRESS << 1, &output, 1 );
    return output;
}

int TSL2561_I2C::readMultipleRegisters( char address, char* output, int quantity ){
    char command = address | 160; //0d160 = 0b10100000
    i2c.write( TSL_SLAVE_ADDRESS << 1, &command, 1, true );
    int ack = i2c.read( TSL_SLAVE_ADDRESS << 1, output, quantity );
    return ack;
}

int TSL2561_I2C::getVisibleAndIR(){
    char buffer[2] = { 0 };
    readMultipleRegisters( TSL_DATA0LOW, buffer, 2 );
    int reading = (int)buffer[1] << 8 | (int)buffer[0];
    return reading;
}

int TSL2561_I2C::getIROnly(){
    char buffer[2] = { 0 };
    readMultipleRegisters( TSL_DATA1LOW, buffer, 2 );
    int reading = (int)buffer[1] << 8 | (int)buffer[0];
    return reading;
}

float TSL2561_I2C::getLux(){
    float lux = 0;
    int ch0 = getVisibleAndIR();
    int ch1 = getIROnly();
    
    // Determine if either sensor saturated (0xFFFF)
    // If so, abandon ship (calculation will not be accurate)
    if( (ch0 == 0xFFFF) || (ch1 == 0xFFFF) ){
        return -1;
    }
    
    // Convert from unsigned integer to floating point
    float d0 = ch0;
    float d1 = ch1;

    // We will need the ratio for subsequent calculations
    double ratio = d1 / d0;
    
    // Normalize for integration time
    int itime = readIntegrationTime();
    d0 *= (402.0/itime);
    d1 *= (402.0/itime);
    
    // Normalize for gain
    int gain = readGain();
        d0 /= gain;
        d1 /= gain;
    
    // Determine lux per datasheet equations:

    if (ratio < 0.5)
    {
        lux = 0.0304 * d0 - 0.062 * d0 * pow(ratio,1.4);
    }
    else if (ratio < 0.61)
    {
        lux = 0.0224 * d0 - 0.031 * d1;
    }
    else if (ratio < 0.80)
    {
        lux = 0.0128 * d0 - 0.0153 * d1;
    }
    else if (ratio < 1.30)
    {
        lux = 0.00146 * d0 - 0.00112 * d1;
    }
    
    return lux;
}

int TSL2561_I2C::enablePower(){
    int ack = writeSingleRegister( TSL_CONTROL, 3 );
    return ack;
}

int TSL2561_I2C::disablePower(){
    int ack = writeSingleRegister( TSL_CONTROL, 0 );
    return ack;
}

bool TSL2561_I2C::isPowerEnabled(){
    char control = readSingleRegister( TSL_CONTROL );
    bool power = 0;
    if( control == 3 ){
        power = 1;
    }
    return power;
}

int TSL2561_I2C::readGain(){
    char timing = readSingleRegister( TSL_TIMING );
    char gain_bit = ( timing << 3 ) >> 7; // keep only bit 4
    int gain;
    switch (gain_bit) {
        case 0:
            gain = 1;
            break;
        case 1:
            gain = 16;
            break;
        default:
            gain = 0;
            break;
    }
    return gain;
}

int TSL2561_I2C::setGain( const int gain ){
    char timing_old = readSingleRegister( TSL_TIMING );
    char timing_new = 0;
    int ack = 0;
    switch (gain){
        case 1:
            timing_new = timing_old & 239; // sets bit 4 to 0
            break;
        case 16:
            timing_new = timing_old | 16; // sets bit 4 to 1
            break;
        default:
            ack = 2; // 2 used to indicate invalid entry
            break;
    }
    
    if ( ack != 2 ){
        ack = writeSingleRegister( TSL_TIMING, timing_new );
    }
    return ack;
}

float TSL2561_I2C::readIntegrationTime(){
    char timing = readSingleRegister( TSL_TIMING );
    char integ = ( timing << 6 ) >> 6; // keep bits 0 & 1
    int itime;
    switch (integ) {
        case 0:
            itime = 13.7;
            break;
        case 1:
            itime = 101;
            break;
        case 2:
            itime = 402;
            break;
        default:
            itime = 0;
            break;
    }
    return itime;
}

int TSL2561_I2C::setIntegrationTime( const float itime ){
    char timing_old = readSingleRegister( TSL_TIMING );
    char timing_new = 0;
    int ack = 0;
    if( abs( itime - 13.7 ) <= 0.001 ){
        timing_new = timing_old & 252; // set bits 0 & 1 (INTEG) to 00
    }
    else if( abs( itime - 101 ) <= 0.001 ){
        timing_new = timing_old | 1;   // sets bit 0 to 1
        timing_new = timing_new & 253; // sets bit 1 to 0
    }
    else if( abs( itime - 402 ) <= 0.001 ){
        timing_new = timing_old | 3; // sets bits 0 & 1 (INTEG) to 11
    }
    else {
        ack = 2; // indicates invalid entry
    }
    if ( ack != 2 ){
        ack = writeSingleRegister( TSL_TIMING, timing_new );
    }
    return ack;
}

int TSL2561_I2C::readLowInterruptThreshold(){
    char buffer[2] = { 0 };
    readMultipleRegisters( TSL_THRESHLOWLOW, buffer, 2 );
    int reading = (int)buffer[1] << 8 | (int)buffer[0];
    return reading;
}

int TSL2561_I2C::readHighInterruptThreshold(){
    char buffer[2] = { 0 };
    readMultipleRegisters( TSL_THRESHHIGHLOW, buffer, 2 );
    int reading = (int)buffer[1] << 8 | (int)buffer[0];
    return reading;
}

int TSL2561_I2C::setLowInterruptThreshold( const int threshold ){
    char threshold_bytes[2];
    threshold_bytes[0] = threshold; // take lowest 8 bits of threshold
    threshold_bytes[1] = threshold >> 8; // take highest 8 bits of threshold
    int ack = writeMultipleRegisters( TSL_THRESHLOWLOW, threshold_bytes, 2 );
    return ack;
}

int TSL2561_I2C::setHighInterruptThreshold( const int threshold ){
    char threshold_bytes[2];
    threshold_bytes[0] = threshold;
    threshold_bytes[1] = threshold >> 8;
    int ack = writeMultipleRegisters( TSL_THRESHHIGHLOW, threshold_bytes, 2 );
    return ack;
}

int TSL2561_I2C::readInterruptPersistence(){
    char interrupt = readSingleRegister( TSL_INTERRUPT );
    char persist = ( interrupt << 4 ) >> 4; // discard bits 4 to 7, keep only bits 0 to 3 
    return (int)persist;
}

int TSL2561_I2C::setInterruptPersistence( const int persistence ){
    char interrupt_old = readSingleRegister( TSL_INTERRUPT );
    char interrupt_new = interrupt_old | (char)persistence; // sets bits 1 to 3 (PERSIST) to the value of persistence
    int ack = writeSingleRegister( TSL_INTERRUPT, interrupt_new );
    return ack;
}

int TSL2561_I2C::readInterruptControl(){
    char interrupt = readSingleRegister( TSL_INTERRUPT );
    char control = ( interrupt << 2 ) >> 6; // keep only bits 4 & 5 
    return (int)control;
}

int TSL2561_I2C::setInterruptControl( const int control ){
    char interrupt_old = readSingleRegister( TSL_INTERRUPT );
    char interrupt_new = interrupt_old | (char)( control << 4 ); // sets bits 4 and 5 (INTR) to the value of control
    int ack = writeSingleRegister( TSL_INTERRUPT, interrupt_new );
    return ack;
}

int TSL2561_I2C::clearInterrupt(){
    char tx = 192;
    int ack = i2c.write( TSL_SLAVE_ADDRESS << 1, &tx, 1 ); // writes 0b11000000 to command register to clear interrupt
    return ack;
}

int TSL2561_I2C::getPartNumber(){
    char id = readSingleRegister( TSL_ID );
    char partno = id >> 4; // keep upper 4 bits
    return (int)partno;
}

int TSL2561_I2C::getRevisionNumber(){
    char id = readSingleRegister( TSL_ID );
    char revno = ( id << 4 ) >> 4; // keep lower 4 bits
    return (int)revno;
}