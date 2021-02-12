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

#ifndef DRO_LORA_TEST_MAIN_H
#define DRO_LORA_TEST_MAIN_H

#endif //DRO_LORA_TEST_MAIN_H

#include <Arduino.h>
#include "Corella.h"
#include <SoftwareSerial.h>

#include <TinyGPS.h>


enum last_ack_e{
    last_ack_undefined = 0,
    last_ack_OK = 1,
    last_ack_FAIL = 2
};

struct reading_t
{
    long latitude, longitude;
    unsigned long prec;
} __attribute__((packed));

#define TAGGLE_SESSION_REGISTRATION_PACKET_ID 9
#define TAGGLE_SESSION_REGISTRATION_MAGIC_NUMBER 0xABCD
#define TAGGLE_GPS_PACKET_ID 2

enum taggle_session_e
{
    taggle_session_none = 0,
    taggle_session_gps_dms = 1,
    taggle_session_gps_dec = 2,
    taggle_session_ceres_tag_esn_2 = 3,
};

struct session_registration_t
{
    uint16_t magic_number;
    enum taggle_session_e session_type;
    uint64_t p2;
} __attribute__((packed));
