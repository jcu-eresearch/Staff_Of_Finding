//
// Created by nigelb on 2/09/2019.
//

#include "Corella.h"

Corella::Corella(Stream *port, Stream* debug) {

    debug->println("Corella Init");
    this->port = port;
    this->port->println("AT+VERSION");
    this->debug = debug;

    String hw_ver = port->readStringUntil('\n');
    String fw_ver = port->readStringUntil('\n');
    String ok = port->readStringUntil('\n');

    debug->println(hw_ver);
    debug->println(fw_ver);
    debug->println(ok.startsWith("OK"));
}

size_t Corella::read_response(char *buf, size_t buf_size, corella_response_e * result, unsigned long time_out) {
    size_t count = 0;
    *result = corella_response_undefined;
    memset(buf, 0, buf_size);
    this->debug->println("JJJJJ");
    unsigned long start = millis();
    const char* ok = "OK";
    const char* error = "ERROR";

    int ok_len = strlen(ok);
    int error_len = strlen(error);

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
                this->debug->println("Found OK");
                break;
            }
            if(strncmp(&buf[count - error_len], error, buf_size) == 0)
            {
                *result = corella_response_error;
                this->debug->println("Found ERROR");
                break;
            }
        }
    }
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
    mdot_response response;
    read_response(rvc_buf, sizeof(rvc_buf), &response);
    return response == corella_response_ok;
};

void Corella::clear_buf() {
    while(this->port->available())
    {
        this->port->read();
    }
}




