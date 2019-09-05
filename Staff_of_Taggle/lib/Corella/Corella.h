//
// Created by nigelb on 5/12/2018.
//

#ifndef TAGGLE_CORELLA_H
#define TAGGLE_CORELLA_H

#include <Stream.h>
#include "Arduino.h"
#include "parser.h"

struct corella_version_t
{
    char hardware_version[10];
    char firmware_version[10];
};

enum corella_response_e
{
    corella_response_undefined,
    corella_response_ok,
    corella_response_error,
    corella_response_error_data_size_to_large,
};

class Corella
{
private:
    Stream *port, *debug;
    size_t read_response(char* buf, size_t buf_size, corella_response_e * result, unsigned long time_out=50000);

public:
    Corella(Stream *port, Stream* debug);
    corella_response_e send_data(uint8_t packet_type, uint8_t* buf, size_t buf_size);

    void clear_buf();
};

#endif //DRO_LORA_TEST_MDOT_H
