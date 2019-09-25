//
// Created by nigelb on 5/12/2018.
//

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
