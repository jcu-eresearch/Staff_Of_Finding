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

#ifndef DRO_LORA_TEST_MAIN_H
#define DRO_LORA_TEST_MAIN_H

#endif //DRO_LORA_TEST_MAIN_H

#include "Arduino.h"

#include <SoftwareSerial.h>

#include <TinyGPS.h>


enum last_ack_e{
    last_ack_undefined = 0,
    last_ack_OK = 1,
    last_ack_FAIL = 2
};

struct reading_t
{
    int year;
    byte month, day, hour, minute, second, hundy;
    unsigned long dage;
    int rssi_last, rssi_min, rssi_max, rssi_avg;
    uint32_t counter = 0;
    last_ack_e acked = last_ack_undefined;
    long latitude, longitude;
    unsigned long age;
    unsigned short sats;
    unsigned long prec;
} __attribute__((packed));