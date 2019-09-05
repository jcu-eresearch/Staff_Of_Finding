//
// Created by nigelb on 5/12/2018.
//

#include "main.h"
#include "mDot.h"


/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

TinyGPS gps;
SoftwareSerial ss(4, 3);
//SoftwareSerial ss(4, 9);
SoftwareSerial _debug(8, 9);
SoftwareSerial *debug = &_debug;
//SoftwareSerial *debug = &ss;
mDot *dot;

uint8_t button_pin = 2;
uint8_t buzzer_pin = 6;
uint8_t red_led_pin = 13;
uint8_t green_led_pin = 12;
uint8_t  POWERBEE_CONTROL_PIN = 5;
long last = millis();

volatile bool do_send = false;

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

void setup()
{
    Serial.begin(9600);
//    debug->begin(9600);
    ss.begin(9600);


    memset(&reading, 0, sizeof(reading));

    debug->print("Simple TinyGPS library v. "); debug->println(TinyGPS::library_version());
    debug->println();

    delay(100);
    debug->println("Starting...");
    dot = new mDot(&Serial, debug);

    pinMode(button_pin, INPUT);
    pinMode(buzzer_pin, OUTPUT);
    pinMode(red_led_pin, OUTPUT);
    pinMode(green_led_pin, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(button_pin), button_press, RISING);
    //Serial.println("ATI");

    bool joined = false;
    digitalWrite(red_led_pin, HIGH);
    dot->set_ack(3);
    do{
        debug->println("Joining.....");
        joined = dot->join();
        if(!joined){delay(5000);}

    }while(!joined);
    debug->println("Joined");
    digitalWrite(red_led_pin, LOW);
    digitalWrite(green_led_pin, HIGH);
    delay(100);
    digitalWrite(green_led_pin, LOW);
    delay(100);
    digitalWrite(green_led_pin, HIGH);
    delay(100);
    digitalWrite(green_led_pin, LOW);

    int last, min, max, avg;

    dot->get_rssi(&last, &min, &max, &avg);

    debug->print(last);
    debug->print(" ");
    debug->print(min);
    debug->print(" ");
    debug->print(max);
    debug->print(" ");
    debug->print(avg);
    debug->println("");

    dot->get_dr();

    debug->println("------------------------------");
}




void loop()
{
    if(do_send)
    {

        digitalWrite(buzzer_pin, HIGH);
        delay(500);
        digitalWrite(buzzer_pin, LOW);



        gps.get_position(&reading.latitude, &reading.longitude, &reading.age);
        reading.sats = gps.satellites();
        reading.prec = gps.hdop();
        gps.crack_datetime(&reading.year, &reading.month, &reading.day, &reading.hour, &reading.minute, &reading.second, &reading.hundy, &reading.dage);
        dot->get_rssi(&reading.rssi_last, &reading.rssi_min, &reading.rssi_max, &reading.rssi_avg);

        /*
        size_t raw_buffer_size = sizeof(reading) * 2;

        uint8_t hex_buf[(sizeof(reading) * 2) + 1];
        memset(hex_buf, 0, sizeof(hex_buf));
        uint8_t *ptr = (uint8_t*)&reading;


        for(size_t i = 0; i < raw_buffer_size; i++)
        {
            sprintf((char*)hex_buf, "%s%02X", hex_buf, ptr[i]);
        }
         */

        debug->print("Reading Size: ");
        debug->println(sizeof(reading));
        if(dot->send_b_data((uint8_t*)&reading, sizeof(reading)))
        {
            digitalWrite(green_led_pin, HIGH);
            delay(100);
            digitalWrite(green_led_pin, LOW);
            delay(100);
            digitalWrite(green_led_pin, HIGH);
            delay(100);
            digitalWrite(green_led_pin, LOW);
            reading.acked = last_ack_OK;
        }else
        {
            digitalWrite(red_led_pin, HIGH);
            delay(100);
            digitalWrite(red_led_pin, LOW);
            delay(100);
            digitalWrite(red_led_pin, HIGH);
            delay(100);
            digitalWrite(red_led_pin, LOW);
            reading.acked = last_ack_FAIL;
        }
        do_send = false;
        reading.counter++;

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