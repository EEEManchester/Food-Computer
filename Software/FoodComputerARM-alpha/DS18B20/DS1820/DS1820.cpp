#include "DS1820.h"
#include "mbed.h"

// Global variables shared between all DS1820 objects
bool DS1820_done_flag;
int  DS1820_last_descrepancy;
char DS1820_search_ROM[8];


DS1820::DS1820 (PinName data_pin, PinName power_pin) : _datapin(data_pin), _parasitepin(power_pin) {
    int byte_counter;

    _parasite_power = true;
    for(byte_counter=0;byte_counter<8;byte_counter++)
        ROM[byte_counter] = 0xFF;
    for(byte_counter=0;byte_counter<9;byte_counter++)
        RAM[byte_counter] = 0x00;
}
DS1820::DS1820 (PinName data_pin) : _datapin(data_pin), _parasitepin(NC) {
    int byte_counter;
    _parasite_power = false;
    for(byte_counter=0;byte_counter<8;byte_counter++)
        ROM[byte_counter] = 0xFF;
    for(byte_counter=0;byte_counter<9;byte_counter++)
        RAM[byte_counter] = 0x00; 
}

bool DS1820::onewire_reset() {
// This will return false if no devices are present on the data bus
    bool presence=false;
    _datapin.output();
    _datapin.mode(PullUp);
    _datapin = 0;           // bring low for 500 us
    wait_us(500);
    _datapin.input();       // let the data line float high
    _datapin.mode(PullUp);
    wait_us(90);            // wait 90us
    if (_datapin.read()==0) // see if any devices are pulling the data line low
        presence=true;
    wait_us(410);
    return presence;
}

void DS1820::onewire_bit_out (bool bit_data) {
    _datapin.output();
    _datapin = 0;
    wait_us(5);
    if (bit_data) {
        _datapin.input(); // bring data line high
        _datapin.mode(PullUp);
        wait_us(55);
    } else {
        wait_us(55);            // keep data line low
        _datapin.input();
        _datapin.mode(PullUp);
    }
}

void DS1820::onewire_byte_out(char data) { // output data character (least sig bit first).
    int n;
    for (n=0; n<8; n++) {
        onewire_bit_out(data & 0x01);
        data = data >> 1; // now the next bit is in the least sig bit position.
    }
}

bool DS1820::onewire_bit_in() {
    bool answer;
    _datapin.output();
    _datapin = 0;
    wait_us(5);
    _datapin.input();
    _datapin.mode(PullUp);
    wait_us(5);
    answer = _datapin.read();
    wait_us(50);
    return answer;
}

char DS1820::onewire_byte_in() { // read byte, least sig byte first
    char answer = 0x00;
    int i;
    for (i=0; i<8; i++) {
        answer = answer >> 1; // shift over to make room for the next bit
        if (onewire_bit_in())
            answer = answer | 0x80; // if the data port is high, make this bit a 1
    }
    return answer;
}

bool DS1820::search_ROM() {
    return search_ROM_routine(0xF0);    // Search ROM command
}

bool DS1820::search_alarm() {
    return search_ROM_routine(0xEC);    // Search Alarm command
}

bool DS1820::search_ROM_routine(char command) {
    extern bool DS1820_done_flag;
    extern int DS1820_last_descrepancy;
    extern char DS1820_search_ROM[8];
    int descrepancy_marker, ROM_bit_index;
    bool return_value, Bit_A, Bit_B;
    char byte_counter, bit_mask;

    return_value=false;
    if (!DS1820_done_flag) {
        if (!onewire_reset()) {
            DS1820_last_descrepancy = 0; // no devices present
        } else {
            ROM_bit_index=1;
            descrepancy_marker=0;
            onewire_byte_out(command); // Search ROM command or Search Alarm command
            byte_counter = 0;
            bit_mask = 0x01;
            while (ROM_bit_index<=64) {
                Bit_A = onewire_bit_in();
                Bit_B = onewire_bit_in();
                if (Bit_A & Bit_B) {
                    descrepancy_marker = 0; // data read error, this should never happen
                    ROM_bit_index = 0xFF;
                } else {
                    if (Bit_A | Bit_B) {
                        // Set ROM bit to Bit_A
                        if (Bit_A) {
                            DS1820_search_ROM[byte_counter] = DS1820_search_ROM[byte_counter] | bit_mask; // Set ROM bit to one
                        } else {
                            DS1820_search_ROM[byte_counter] = DS1820_search_ROM[byte_counter] & ~bit_mask; // Set ROM bit to zero
                        }
                    } else {
                        // both bits A and B are low, so there are two or more devices present
                        if ( ROM_bit_index == DS1820_last_descrepancy ) {
                            DS1820_search_ROM[byte_counter] = DS1820_search_ROM[byte_counter] | bit_mask; // Set ROM bit to one
                        } else {
                            if ( ROM_bit_index > DS1820_last_descrepancy ) {
                                DS1820_search_ROM[byte_counter] = DS1820_search_ROM[byte_counter] & ~bit_mask; // Set ROM bit to zero
                                descrepancy_marker = ROM_bit_index;
                            } else {
                                if (( DS1820_search_ROM[byte_counter] & bit_mask) == 0x00 )
                                    descrepancy_marker = ROM_bit_index;
                            }
                        }
                    }
                    onewire_bit_out (DS1820_search_ROM[byte_counter] & bit_mask);
                    ROM_bit_index++;
                    if (bit_mask & 0x80) {
                        byte_counter++;
                        bit_mask = 0x01;
                    } else {
                        bit_mask = bit_mask << 1;
                    }
                }
            }
            DS1820_last_descrepancy = descrepancy_marker;
            if (ROM_bit_index != 0xFF) {
                for(byte_counter=0;byte_counter<8;byte_counter++)
                    ROM[byte_counter] = DS1820_search_ROM[byte_counter];
                return_value = true;
            }
        }
        if (DS1820_last_descrepancy == 0)
            DS1820_done_flag = true;
    }
    return return_value;
}

void DS1820::search_ROM_setup() {
    extern bool DS1820_done_flag;
    extern int DS1820_last_descrepancy;
    extern char DS1820_search_ROM[8];
    DS1820_done_flag = false;
    DS1820_last_descrepancy = 0;
    int i;
    for (i=0; i<8; i++)
        DS1820_search_ROM[i]=0x00;
}

void DS1820::read_ROM() {
    // NOTE: This command can only be used when there is one DS1820 on the bus. If this command
    // is used when there is more than one slave present on the bus, a data collision will occur
    // when all the DS1820s attempt to respond at the same time.
    int i;
    onewire_reset();
    onewire_byte_out(0x33);   // Read ROM id
    for (i=0; i<8; i++)
        ROM[i]=onewire_byte_in();
}

void DS1820::match_ROM() {
// Used to select a specific device
    int i;
    onewire_reset();
    onewire_byte_out( 0x55);  //Match ROM command
    for (i=0;i<8;i++)
        onewire_byte_out(ROM[i]);
}

void DS1820::skip_ROM() {
    onewire_reset();
    onewire_byte_out(0xCC);   // Skip ROM command
}

bool DS1820::ROM_checksum_error() {
    char xCRC=0x00;
    int i;
    for(i=0;i<7;i++) // Only going to shift the lower 7 bytes
        xCRC = CRC_byte(xCRC, ROM[i]);
    // After 7 bytes CRC should equal the 8th byte (ROM CRC)
    return (xCRC!=ROM[7]); // will return true if there is a CRC checksum error         
}

bool DS1820::RAM_checksum_error() {
    char xCRC=0x00;
    int i;
    read_RAM();
    for(i=0;i<8;i++) // Only going to shift the lower 8 bytes
        xCRC = CRC_byte(xCRC, RAM[i]);
    // After 8 bytes CRC should equal the 9th byte (RAM CRC)
    return (xCRC!=RAM[8]); // will return true if there is a CRC checksum error         
}

char DS1820::CRC_byte (char xCRC, char byte ) {
    int j;
    for(j=0;j<8;j++) {
        if ((byte & 0x01 ) ^ (xCRC & 0x01)) {
            // DATA ^ LSB CRC = 1
            xCRC = xCRC>>1;
            // Set the MSB to 1
            xCRC = xCRC | 0x80;
            // Check bit 3
            if (xCRC & 0x04) {
                xCRC = xCRC & 0xFB; // Bit 3 is set, so clear it
            } else {
                xCRC = xCRC | 0x04; // Bit 3 is clear, so set it
            }
            // Check bit 4
            if (xCRC & 0x08) {
                xCRC = xCRC & 0xF7; // Bit 4 is set, so clear it
            } else {
                xCRC = xCRC | 0x08; // Bit 4 is clear, so set it
            }
        } else {
            // DATA ^ LSB xCRC = 0
            xCRC = xCRC>>1;
            // clear MSB
            xCRC = xCRC & 0x7F;
            // No need to check bits, with DATA ^ LSB xCRC = 0, they will remain unchanged
        }
        byte = byte>>1;
    }
return xCRC;
}

void DS1820::convert_temperature(devices device) {
    // Convert temperature into scratchpad RAM for all devices at once
    int delay_time = 750; // Default delay time
    char resolution;
    if (device==all_devices)
        skip_ROM();          // Skip ROM command, will convert for ALL devices
    else {
        match_ROM();
        if (FAMILY_CODE == FAMILY_CODE_DS18B20 ) {
            resolution = RAM[4] & 0x60;
            if (resolution == 0x00) // 9 bits
                delay_time = 94;
            if (resolution == 0x20) // 10 bits
                delay_time = 188;
            if (resolution == 0x40) // 11 bits. Note 12bits uses the 750ms default
                delay_time = 375;
        }
    }
    onewire_byte_out( 0x44);  // perform temperature conversion
    if (_parasite_power)
        _parasitepin = 1;       // Parasite power strong pullup
    wait_ms(delay_time);
    if (_parasite_power)
        _parasitepin = 0;
}

void DS1820::read_RAM() {
    // This will copy the DS1820's 9 bytes of RAM data
    // into the objects RAM array. Functions that use
    // RAM values will automaticly call this procedure.
    int i;
    match_ROM();             // Select this device
    onewire_byte_out( 0xBE);   //Read Scratchpad command
    for(i=0;i<9;i++) {
        RAM[i] = onewire_byte_in();
    }
}

bool DS1820::set_configuration_bits(unsigned int resolution) {
    bool answer = false;
    resolution = resolution - 9;
    if (resolution < 4) {
        resolution = resolution<<5; // align the bits
        RAM[4] = (RAM[4] & 0x60) | resolution; // mask out old data, insert new
        write_scratchpad ((RAM[2]<<8) + RAM[3]);
//        store_scratchpad (DS1820::this_device); // Need to test if this is required
        answer = true;
    }
    return answer;
}

int DS1820::read_scratchpad() {
    int answer;
    read_RAM();
    answer = (RAM[2]<<8) + RAM[3];
    return answer;
}

void DS1820::write_scratchpad(int data) {
    RAM[3] = data;
    RAM[2] = data>>8;
    match_ROM();
    onewire_byte_out(0x4E);   // Copy scratchpad into DS1820 ram memory
    onewire_byte_out(RAM[2]); // T(H)
    onewire_byte_out(RAM[3]); // T(L)
    if ( FAMILY_CODE == FAMILY_CODE_DS18B20 ) {
        onewire_byte_out(RAM[4]); // Configuration register
    }
}

void DS1820::store_scratchpad(devices device) {
    if (device==all_devices)
        skip_ROM();          // Skip ROM command, will store for ALL devices
    else
        match_ROM();
    onewire_byte_out(0x48);   // Write scratchpad into E2 command
    if (_parasite_power)
        _parasitepin=1;
    wait_ms(10);            // Parasite power strong pullup for 10ms
    if (_parasite_power)
        _parasitepin=0;
}

int DS1820::recall_scratchpad(devices device) {
// This copies the E2 values into the DS1820's memory.
// If you specify all_devices this will return zero, otherwise
// it will return the value of the scratchpad memory.
    int answer=0;
    if (device==all_devices)
        skip_ROM();          // Skip ROM command, will recall for ALL devices
    else
        match_ROM();
    onewire_byte_out(0xB8);   // Recall E2 data to scratchpad command
    wait_ms(10); // not sure I like polling for completion
                 // it could cause an infinite loop
    if (device==DS1820::this_device) {
        read_RAM();
        answer = read_scratchpad();
    }
    return answer;
}    

float DS1820::temperature(char scale) {
// The data specs state that count_per_degree should be 0x10 (16), I found my devices
// to have a count_per_degree of 0x4B (75). With the standard resolution of 1/2 deg C
// this allowed an expanded resolution of 1/150th of a deg C. I wouldn't rely on this
// being super acurate, but it does allow for a smooth display in the 1/10ths of a
// deg C or F scales.
    float answer, remaining_count, count_per_degree;
    int reading;
    read_RAM();
    reading = (RAM[1] << 8) + RAM[0];
    if (reading & 0x8000) { // negative degrees C
        reading = 0-((reading ^ 0xffff) + 1); // 2's comp then convert to signed int
    }
    answer = reading +0.0; // convert to floating point
    if ( FAMILY_CODE == FAMILY_CODE_DS18B20 ) {
        answer = answer / 8.0;
    }
    else {
        remaining_count = RAM[6];
        count_per_degree = RAM[7];
        answer = answer - 0.25 + (count_per_degree - remaining_count) / count_per_degree;
    }
    if (scale=='C' or scale=='c')
        answer = answer / 2.0;
    else
        // Convert to deg F
        answer = answer * 9.0 / 10.0 + 32.0;
    return answer;
}

bool DS1820::read_power_supply(devices device) {
// This will return true if the device (or all devices) are Vcc powered
// This will return false if the device (or ANY device) is parasite powered
    if (device==all_devices)
        skip_ROM();          // Skip ROM command, will poll for any device using parasite power
    else
        match_ROM();
    onewire_byte_out(0xB4);   // Read power supply command
    return onewire_bit_in();
}
