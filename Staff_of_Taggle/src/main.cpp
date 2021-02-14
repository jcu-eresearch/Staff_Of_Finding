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

#include "main.h"

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

TinyGPS gps;

#define MODE_RUN 0
#define MODE_GPS_TEST 1
#define MODE_TAGGLE_TEST 2
#define MODE_LED_TEST 3

#define MODE MODE_RUN
//#define MODE MODE_GPS_TEST
//#define MODE MODE_TAGGLE_TEST
//#define MODE MODE_LED_TEST

//SoftwareSerial gps_port(4, 3);
SoftwareSerial gps_port(4, 9);

#if MODE == MODE_RUN || MODE == MODE_TAGGLE_TEST

//SoftwareSerial _debug(8, 9);
//Stream *debug = &_debug;
Stream *debug = nullptr;

#elif MODE == MODE_GPS_TEST || MODE == MODE_LED_TEST

Stream *debug = nullptr;

#endif





uint8_t button_pin = 2;
uint8_t buzzer_pin = 6;
uint8_t red_led_pin = 13;
uint8_t green_led_pin = 12;
uint8_t blue_led_pin = 11;
uint8_t  POWERBEE_CONTROL_PIN = 5;
volatile unsigned long last;

volatile bool do_send = false;
volatile bool first = true;

void button_press()
{
    if(millis() - last < 100)
    {return;}
    last = millis();
    delay(100);

    if(digitalRead(button_pin) == HIGH) {
        debug->print("Button press: ");
        debug->println(digitalRead(button_pin));
        //digitalWrite(red_led_pin, !digitalRead(red_led_pin));
        do_send = true;
        first = true;
    }

}

void power_up_radio()
{
    pinMode(POWERBEE_CONTROL_PIN, OUTPUT);
    digitalWrite(POWERBEE_CONTROL_PIN, HIGH);
    delay(500);
}

void power_down_radio()
{
    digitalWrite(POWERBEE_CONTROL_PIN, LOW);
}

reading_t reading;
Corella *taggle;

void setup()
{
    Serial.begin(9600);

#if MODE == MODE_RUN
    debug = &gps_port;
    debug->println("Running in mode: RUN");

#elif MODE == MODE_GPS_TEST
    debug = &Serial;
    debug->println("Running in mode: GPS_TEST");
#elif MODE == MODE_TAGGLE_TEST
    _debug.begin(9600);
    debug->println("Running in mode: TAGGLE_TEST");
#elif MODE == MODE_LED_TEST
    debug = &Serial;
    debug->println("Running in mode: GPS_LED_TEST");
#endif

#if MODE == MODE_RUN || MODE == MODE_GPS_TEST
    gps_port.begin(9600);
#endif

    last = millis();

    memset(&reading, 0, sizeof(reading));

    delay(100);

    debug->println("Starting...");
    debug->println();

    debug->print("Compile Date Time: ");
    debug->print(__DATE__);
    debug->print(" ");
    debug->println(__TIME__);

    debug->println();
    debug->print("Simple TinyGPS library v. "); debug->println(TinyGPS::library_version());
    debug->println();

    debug->print("Session Size: ");
    debug->println(sizeof(session_registration_t));

    debug->print("Reading Size: ");
    debug->println(sizeof(reading_t));

    debug->print("GPS GGA: ");
    debug->println(_GPGGA_TERM);

    debug->println("-----------------------------------------------");
    debug->println("Initializing Pins and Attaching Interrupts...");

    pinMode(button_pin, INPUT);
    pinMode(buzzer_pin, OUTPUT);
    pinMode(red_led_pin, OUTPUT);
    pinMode(green_led_pin, OUTPUT);
    pinMode(blue_led_pin, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(button_pin), button_press, RISING);

#if MODE == MODE_RUN || MODE == MODE_TAGGLE_TEST
    debug->println("Initializing Corella");
    taggle = new Corella(&Serial, debug);
    int32_t id;
    taggle->get_at_id(id);
    debug->print("Corella ID: ");
    debug->println(id);
    session_registration_t reg = {0};
    memset(&reg, 0, sizeof(reg));
    reg.magic_number = TAGGLE_SESSION_REGISTRATION_MAGIC_NUMBER;
    reg.session_type = taggle_session_gps_dec;

    size_t count = 0;
    for(; count < 5; count++){
        debug->println("Attempting to send Session Registration Packet");
        corella_response_e response = taggle->send_data(TAGGLE_SESSION_REGISTRATION_PACKET_ID, (uint8_t *) &reg, sizeof(reg));
        if(response == corella_response_ok)
        {
            debug->println("Sent Session Registration Packet");
            break;
        }
        delay(30000);
    }
    if(count >= 5)
    {
        debug->println("Failed to send Registration Packet");
    }
    delay(2000);
#endif

    //Serial.println("ATI");


    debug->println("------------------------------");
    digitalWrite(buzzer_pin, HIGH);
    delay(100);
    digitalWrite(buzzer_pin, LOW);
}




void loop()
{

    if(do_send)
    {   if(first){
            digitalWrite(buzzer_pin, HIGH);
            delay(10);
            digitalWrite(buzzer_pin, LOW);
        }
        first = false;
        if(millis() - last > 10000) {

            digitalWrite(buzzer_pin, HIGH);
            delay(10);
            digitalWrite(buzzer_pin, LOW);
            debug->println("Wait Over");

            long lat, lng;
            unsigned long age;
            gps.get_position(&lat, &lng, &age);
            memset(&reading, 0, sizeof(reading));
            reading.latitude = lat;
            reading.longitude = lng;
            reading.prec = gps.hdop();

//        Lat: -19329748
//        Long: 146759923

            debug->println(sizeof(long));
            debug->print("Lat: ");
            debug->println(reading.latitude);
            debug->print("Long: ");
            debug->println(reading.longitude);

            corella_response_e response = corella_response_undefined;

#if MODE == MODE_RUN || MODE == MODE_TAGGLE_TEST
            debug->println("Sending Data...");
            response = taggle->send_data(TAGGLE_GPS_PACKET_ID, (uint8_t*)&reading, sizeof(reading));
#endif

            if(response == corella_response_ok)
            {
                debug->println("Data Sent");
                digitalWrite(buzzer_pin, HIGH);
                digitalWrite(green_led_pin, HIGH);
                delay(100);
                digitalWrite(green_led_pin, LOW);
                digitalWrite(buzzer_pin, LOW);
                delay(100);
                digitalWrite(green_led_pin, HIGH);
                digitalWrite(buzzer_pin, HIGH);
                delay(100);
                digitalWrite(green_led_pin, LOW);
                digitalWrite(buzzer_pin, LOW);

            }else if(response == corella_response_wait)
            {
                debug->println("WAIT recieved from Corella");
                for(int count = 0; count < 10; count++)
                {
                    digitalWrite(buzzer_pin, HIGH);
                    digitalWrite(green_led_pin, HIGH);
                    delay(10);
                    digitalWrite(green_led_pin, LOW);
                    digitalWrite(buzzer_pin, LOW);
                    delay(10);
                }
            }
            else
            {
                debug->println("ERROR recieved from Corella.");
                digitalWrite(buzzer_pin, HIGH);
                digitalWrite(red_led_pin, HIGH);
                delay(100);
                digitalWrite(red_led_pin, LOW);
                delay(100);
                digitalWrite(red_led_pin, HIGH);
                delay(100);
                digitalWrite(red_led_pin, LOW);
                delay(200);
                digitalWrite(buzzer_pin, LOW);
            }
            debug->println("Finished");
            do_send = false;
        }

    }


    while (gps_port.available())
    {
        char c = (char)gps_port.read();

#if MODE == MODE_GPS_TEST
        Serial.write(c); // uncomment this line if you want to see the GPS data flowing
#endif

        if (gps.encode(c)){
            //debug->println("GPS");
            //digitalWrite(red_led_pin, !digitalRead(red_led_pin));
        }
    }

#if MODE == MODE_LED_TEST
    digitalWrite(red_led_pin, HIGH);
    delay(500);
    digitalWrite(red_led_pin, LOW);
    digitalWrite(green_led_pin, HIGH);
    delay(500);
    digitalWrite(green_led_pin, LOW);
    digitalWrite(blue_led_pin, HIGH);
    delay(500);
    digitalWrite(blue_led_pin, LOW);
    delay(500);

#endif

}