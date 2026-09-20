#ifndef NMEA_H
#define NMEA_H

#include <stdint.h>

typedef struct {
    uint8_t  valid;        // 1 = last fix valid (status A)
    double   latitude;     // decimal degrees,  +N / -S
    double   longitude;    // decimal degrees,  +E / -W
    float    speed_knots;  // speed over ground
    float    course_deg;   // track angle
    uint8_t  hour, minute, second;
    uint8_t  day, month;
    uint16_t year;
    uint8_t  fix_quality;  // 0=invalid,1=GPS,2=DGPS
    uint8_t  sat_count;    // satellites in use
    float    altitude_m;   // altitude above MSL
    uint8_t  fix_mode;     // 2=2D, 3=3D (from GSA)
    float    pdop;         // position dilution of precision
    float    hdop;         // horizontal DOP
    float    vdop;         // vertical DOP
} nmea_data_t;

extern nmea_data_t nmea_data;

// Feed one byte from the UART ISR. Returns 1 when a complete
// sentence was just received and processed.
uint8_t nmea_process_byte(uint8_t b);

const char *nmea_get_buf(void);
uint8_t     nmea_get_idx(void);

#endif
