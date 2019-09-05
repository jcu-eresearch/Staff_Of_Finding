//
// Created by nigelb on 5/12/2018.
//

#ifndef DRO_LORA_TEST_MDOT_H
#define DRO_LORA_TEST_MDOT_H

#include <Stream.h>
#include "Arduino.h"
#include "parser.h"

enum mdot_response
{
    mdot_response_undefined,
    mdot_response_ok,
    mdot_response_error,
};

class mDot
{
private:
    Stream *port, *debug;
    size_t read_response(char* buf, size_t buf_size, mdot_response * result, unsigned long time_out=50000);

public:
    mDot(Stream *port, Stream* debug);
    bool join();
    bool send_data(uint8_t* buf, size_t buf_size);
    bool send_b_data(uint8_t* buf, size_t buf_size);
    bool set_ack(uint8_t ack_count);
    bool get_rssi(int *last, int *min, int *max, int *avg);
    bool get_dr();
    void clear_buf();
};

#endif //DRO_LORA_TEST_MDOT_H
