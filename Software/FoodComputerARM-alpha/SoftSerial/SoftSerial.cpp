#include "SoftSerial.h"

SoftSerial::SoftSerial(PinName TX, PinName RX, const char* name) {
    tx_en = rx_en = false;
    read_buffer = 0;
    if (TX != NC) {
        tx = new DigitalOut(TX);
        tx_en = true;
        tx->write(1);
        tx_bit = -1;
        txticker.attach(this, &SoftSerial::tx_handler);
    }
    if (RX != NC) {
        rx = new InterruptIn(RX);
        rx_en = true;
        out_valid = false;
        rxticker.attach(this, &SoftSerial::rx_handler);
        rx->fall(this, &SoftSerial::rx_gpio_irq_handler);
    }
    
    baud(9600);
    format();
}

SoftSerial::~SoftSerial() {
    if (tx_en)
        delete(tx);
    if (rx_en)
        delete(rx);
}

void SoftSerial::baud(int baudrate) {
    bit_period = 1000000 / baudrate;
}

void SoftSerial::format(int bits, Parity parity, int stop_bits) {
    _bits = bits;
    _parity = parity;
    _stop_bits = stop_bits;
    _total_bits = 1 + _bits + _stop_bits + (bool)_parity;
}
