//
// Created by nigelb on 5/12/2018.
//

#include "main.h"

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

TinyGPS gps;
SoftwareSerial ss(4, 3);
//SoftwareSerial ss(4, 9);
SoftwareSerial _debug(8, 9);
Stream *debug = &_debug;
//SoftwareSerial *debug = &ss;


uint8_t button_pin = 2;
uint8_t buzzer_pin = 6;
uint8_t red_led_pin = 13;
uint8_t green_led_pin = 12;
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
    //debug = &Serial;
    //_debug.begin(9600);
    ss.begin(9600);
    last = millis();

    memset(&reading, 0, sizeof(reading));

    debug->print("Simple TinyGPS library v. "); debug->println(TinyGPS::library_version());
    debug->println();

    delay(100);
    debug->println("Starting...");
    taggle = new Corella(&Serial, debug);
    char *bb = "HelloWorld!";
    taggle->send_data(1, (uint8_t*)bb, 12);
    //debug->println("1st Complete");
    //taggle->send_data(1, (uint8_t*)bb, 12);

    debug->println("HelloWorld!");

    pinMode(button_pin, INPUT);
    pinMode(buzzer_pin, OUTPUT);
    pinMode(red_led_pin, OUTPUT);
    pinMode(green_led_pin, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(button_pin), button_press, RISING);
    //Serial.println("ATI");


    debug->println("------------------------------");
    digitalWrite(buzzer_pin, HIGH);
    delay(10);
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


            debug->print("Reading Size: ");
            debug->println(sizeof(reading));

            corella_response_e response = taggle->send_data(1, (uint8_t*)&reading, sizeof(reading));

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


    //debug->println(ss.available());
    while (ss.available())
    {
        char c = ss.read();
        //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)){
            //debug->println("GPS");
            //digitalWrite(red_led_pin, !digitalRead(red_led_pin));
        }

    }
}

/*

void loop()
{

    if(Serial.available())
    {
        uint8_t val = Serial.read();
        debug->write(val);
    }

}
*/

/*

void 1oop()
{
    if(do_send)
    {
        do_send = false;
        digitalWrite(buzzer_pin, HIGH);
        delay(500);
        digitalWrite(buzzer_pin, LOW);


        debug->println("");
        Serial.println("ATI");
        delay(50);
        while(Serial.available())
        {
            uint8_t val = debug->read();
            debug->write(val);
        }
        //power_down_radio();
    }


    //debug->println(ss.available());
    while (ss.available())
    {
        char c = ss.read();
        //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)){
            //debug->println("GPS");
            digitalWrite(red_led_pin, !digitalRead(red_led_pin));


        }

    }

}
 */