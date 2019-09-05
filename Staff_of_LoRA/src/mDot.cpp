//
// Created by nigelb on 5/12/2018.
//

#include "mDot.h"

mDot::mDot(Stream *port, Stream* debug) {

    debug->println("mDot Init");
    this->port = port;
    this->port->println("ATI");
    this->debug = debug;

    String ati = port->readStringUntil('\n');
    String mt = port->readStringUntil('\n');
    String firm = port->readStringUntil('\n');
    String hdw = port->readStringUntil('\n');
    String a = port->readStringUntil('\n');
    String b = port->readStringUntil('\n');
    String ok = port->readStringUntil('\n');

    debug->println(ati);
    debug->println(mt);
    debug->println(firm);
    debug->println(hdw);
    debug->println(a);
    debug->println(b);
    debug->println(ok.startsWith("OK"));

}

bool mDot::join() {
    this->port->println("");
    delay(100);
    clear_buf();
    this->port->println("AT+JOIN");
    char buf[128];
    mdot_response result;
    read_response(buf, sizeof(buf), &result);
    return result == mdot_response_ok;

}

bool mDot::send_b_data(uint8_t *buf, size_t buf_size) {
    this->port->println("");
    delay(50);
    clear_buf();
    this->port->print("AT+SENDB=");

    char h_buf[4];


    for(size_t i = 0; i < buf_size; i++)
    {
        memset(h_buf, 0, sizeof(h_buf));
        snprintf(h_buf, sizeof(h_buf), "%02x", buf[i]);
        this->port->print(h_buf);
    }

    this->port->println("");

    char rvc_buf[128];
    mdot_response response;
    read_response(rvc_buf, sizeof(rvc_buf), &response);
    return response == mdot_response_ok;
}

bool mDot::set_ack(uint8_t ack_count){
    this->port->println("");
    delay(50);
    clear_buf();
    this->port->print("AT+ACK=");
    this->port->println(ack_count);

    char rvc_buf[128];
    mdot_response response;
    read_response(rvc_buf, sizeof(rvc_buf), &response);
    return response == mdot_response_ok;
}

void mDot::clear_buf() {
    while(this->port->available())
    {
        this->port->read();
    }
}

size_t mDot::read_response(char *buf, size_t buf_size, mdot_response * result, unsigned long time_out) {
    size_t count = 0;
    *result = mdot_response_undefined;
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
                *result = mdot_response_ok;
                this->debug->println("Found OK");
                break;
            }
            if(strncmp(&buf[count - error_len], error, buf_size) == 0)
            {
                *result = mdot_response_error;
                this->debug->println("Found ERROR");
                break;
            }
        }
    }
    return count;
}

bool mDot::get_rssi(int *last, int *min, int *max, int *avg) {
    this->port->println("");
    delay(50);
    clear_buf();
    this->port->println("AT+RSSI");

    char rvc_buf[128];
    mdot_response response;
    read_response(rvc_buf, sizeof(rvc_buf), &response);

    if(response == mdot_response_ok) {
        size_t lines_argc = count_tokens(rvc_buf, "\n");
        char *lines_argv[lines_argc];
        parse_string(rvc_buf, "\n", lines_argc, lines_argv);

        if(lines_argc >= 2) {
            size_t rssi_argc = count_tokens(lines_argv[1], ",");
            char *rssi_argv[rssi_argc];
            parse_string(lines_argv[1], ",", rssi_argc, rssi_argv);
            if(rssi_argc >= 4)
            {
                *last = atoi(rssi_argv[0]);
                *min  = atoi(rssi_argv[1]);
                *max  = atoi(rssi_argv[2]);
                *avg  = atoi(rssi_argv[3]);
            }
        }
    }

    return response == mdot_response_ok;

}

bool mDot::get_dr() {
    this->port->println("");
    delay(50);
    clear_buf();
    this->port->println("AT+TXDR?");

    char rvc_buf[128];
    mdot_response response;
    read_response(rvc_buf, sizeof(rvc_buf), &response);
    return response == mdot_response_ok;
}

bool mDot::send_data(uint8_t *buf, size_t buf_size) {
    this->port->println("");
    delay(50);
    clear_buf();
    this->port->print("AT+SEND=");
    for(size_t i =0; i < buf_size; i++)
    {
        this->port->write(buf[i]);
    }
    this->port->println("");

    char rvc_buf[128];
    mdot_response response;
    read_response(rvc_buf, sizeof(rvc_buf), &response);
    return response == mdot_response_ok;
}
