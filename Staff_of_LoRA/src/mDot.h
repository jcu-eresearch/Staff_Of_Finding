/*
    Staff_Of_Lora sends GPS positions via a Multitech mDot to be used to
    perform site surveys.
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
