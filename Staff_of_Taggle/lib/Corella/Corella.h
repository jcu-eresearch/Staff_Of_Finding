/*
    Staff_Of_Taggle sends GPS positions via a Taggle Corella to be used to
    calibrate the Taggle Multilateration.
    Copyright (C) 2019  Nigel Bajema eResearch Center James Cook University

    This program is free software: you can redistribute it and/or modify
            it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
            but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
    corella_response_wait,
    corella_response_error_data_size_to_large,
};

class Corella
{
private:
    Stream *port, *debug;
    size_t read_response(char* buf, size_t buf_size, corella_response_e * result, unsigned long time_out=5000);

public:
    Corella(Stream *port, Stream* debug);
    corella_response_e send_data(uint8_t packet_type, uint8_t* buf, size_t buf_size);

    void clear_buf();
};

#endif //DRO_LORA_TEST_MDOT_H
