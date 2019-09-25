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

#include "Corella.h"

Corella::Corella(Stream *port, Stream* debug) {

    debug->println("Corella Init");
    this->debug = debug;
    this->port = port;
    this->port->println("");
    String er = port->readStringUntil('\n');
    this->port->println(er);
    delay(100);
    this->port->println("AT+VERSION?");

    String s = port->readStringUntil('\n');
    String corella = port->readStringUntil('\n');
    String hw_ver = port->readStringUntil('\n');
    String fw_ver = port->readStringUntil('\n');
    String b = port->readStringUntil('\n');
    String ok = port->readStringUntil('\n');

    debug->print("Hardware Version: ");
    debug->println(hw_ver);
    debug->print("Firmware Version: ");
    debug->println(fw_ver);
    debug->println(ok.startsWith("OK"));
    clear_buf();
}

size_t Corella::read_response(char *buf, size_t buf_size, corella_response_e * result, unsigned long time_out) {
    size_t count = 0;
    *result = corella_response_undefined;
    memset(buf, 0, buf_size);
    this->debug->println("JJJJJ");
    unsigned long start = millis();
    const char* ok = "OK";
    const char* error = "ERROR";
    const char* wait = "WAIT";

    int ok_len = strlen(ok);
    int error_len = strlen(error);
    int wait_len = strlen(wait);

    this->debug->print("\r\n\r\n> ");

    while(millis() - start < time_out)
    {
        if(this->port->available())
        {
            //this->debug->println(count);
            buf[count++] = (char)this->port->read();


            this->debug->write((uint8_t)buf[count-1]);
            if(buf[count-1] == '\n'){this->debug->print("> ");}
            if(count == buf_size){
                this->debug->println("BUF SIZE exceded.");
                break;
            }
            //this->debug->print("SUB: ");
            //this->debug->println(&buf[count - strlen(ok)]);
            if(strncmp(&buf[count - ok_len], ok, buf_size) == 0)
            {
                *result = corella_response_ok;
                this->debug->println();
                this->debug->println("Found OK");
                break;
            }
            if(strncmp(&buf[count - error_len], error, buf_size) == 0)
            {
                *result = corella_response_error;
                this->debug->println();
                this->debug->println("Found ERROR");
                break;
            }
            if(strncmp(&buf[count - wait_len], wait, buf_size) == 0)
            {
                *result = corella_response_wait;
                this->debug->println();
                this->debug->println("Found WAIT");
                break;
            }
        }
    }
    this->debug->println();
    return count;
}

corella_response_e Corella::send_data(uint8_t packet_type, uint8_t* buf, size_t buf_size)
{
    if(buf_size > 12)
    {
        return corella_response_error_data_size_to_large;
    }

    this->port->print("AT+SEND=");
    this->port->print(packet_type);
    this->port->print(",");

    for(size_t i =0; i < buf_size; i++)
    {
        this->port->write(buf[i]);
    }
    this->port->println("");


    char rvc_buf[128];
    corella_response_e response;
    read_response(rvc_buf, sizeof(rvc_buf), &response);
    clear_buf();
    return response;
};

void Corella::clear_buf() {
    while(this->port->available())
    {
        this->port->read();
    }
}




