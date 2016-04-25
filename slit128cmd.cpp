#include <cstdio>
#include <fstream>
#include <iostream>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

// #include "Slit128Packet.h"

using namespace std;

int read_4_bytes(FILE *fp, unsigned char *buf)
{
    int n;
    n = fread(buf, 1, 4, fp);
    if (n == 0) {
        if (ferror(fp)) {
            warn("fread error");
            return -1;
        }
        if (feof(fp)) {
            return 0;
        }
    }

    return n;
}

int fill_event_buf(FILE *fp, unsigned char *event_buf)
{
    int n;
    static bool first_read = true;
    static unsigned char tmpbuf[4];
    int event_data_len = 0;

    if (first_read) {
        n = read_4_bytes(fp, tmpbuf);
        if (n <= 0) {
            return -1;
        }
        first_read = false;
    }
    event_data_len += 4; /* first time: read from file. after second time stored in tmpbuf */

    n = read_4_bytes(fp, tmpbuf);
    if (n <= 0) {
        return -1;
    }
    event_data_len += 4;

    n = read_4_bytes(fp, tmpbuf);
    if (n <= 0) {
        return -1;
    }
    event_data_len += 4;
    
    for ( ; ; ) {
        n = read_4_bytes(fp, tmpbuf); /* (***) */
        if (n <= 0) {
            return -1;
        }
        if ( (tmpbuf[0] & 0x80) == 0x00) {
            fprintf(stderr, "fill_event_buf done %d bytes\n", event_data_len);
            return 0;
        }
        else {
            event_data_len += 4; /* increment by above (***) read_4_bytes() */

            n = read_4_bytes(fp, tmpbuf);
            event_data_len += 4;
            if (n <= 0) {
                return -1;
            }
        }
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    unsigned char event_buf[1024*1024];
    int n;

    FILE *fp;
    char *data_filename;

    if (argc == 1) {
        fp = stdin;
    }
    else {
        data_filename = argv[1];
        fp = fopen(data_filename, "r");
        if (fp == NULL) {
            err(EXIT_FAILURE, "fopen");
        }
    }

    for ( ; ; ) {
        n = fill_event_buf(fp, event_buf);
        if (n < 0) {
            break;
        }
    }

    return 0;
}
