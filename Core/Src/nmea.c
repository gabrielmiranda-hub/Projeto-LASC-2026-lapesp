#include "nmea.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NMEA_BUF_LEN 96
#define NMEA_MAX_FIELDS 24

static char    nmea_buf[NMEA_BUF_LEN];
static uint8_t nmea_idx = 0;
nmea_data_t    nmea_data = {0}; /* zero-initialize everything */

static double nmea_to_deg(const char *val, char hemi)
{
    if (!val || val[0] == '\0') return 0.0;
    double raw = strtod(val, NULL);
    int    deg = (int)(raw / 100.0);
    double min = raw - (deg * 100.0);
    double dec = deg + min / 60.0;
    if (hemi == 'S' || hemi == 'W') dec = -dec;
    return dec;
}

/* Return pointer to first character after optional leading $ or ! */
static const char *nmea_payload(const char *s)
{
    if (s[0] == '$' || s[0] == '!') return s + 1;
    return s;
}

static uint8_t nmea_checksum_ok(const char *s)
{
    const char *star = strrchr(s, '*');
    if (!star) return 0;

    const char *p = nmea_payload(s);
    uint8_t cs = 0;
    for (; p < star; p++) cs ^= (uint8_t)*p;

    unsigned int rcv = 0;
    if (sscanf(star + 1, "%2X", &rcv) != 1) return 0;
    return cs == (uint8_t)rcv;
}

/* Tokenise a writable copy of the payload in-place.
 * fields[] points into the provided buffer. Returns field count. */
static int nmea_tokenise(char *buf, char *fields[], int max_fields)
{
    int n = 0;
    char *p = buf;
    while (n < max_fields) {
        fields[n] = p;
        char *end = p;
        while (*end && *end != ',' && *end != '*') end++;
        if (*end == '\0') {
            n++;
            break;
        }
        *end = '\0';
        p = end + 1;
        n++;
    }
    return n;
}

static void parse_rmc(char *s)
{
    if (!nmea_checksum_ok(s)) return;

    char *fields[NMEA_MAX_FIELDS] = {0};
    char  tmp[NMEA_BUF_LEN];
    strncpy(tmp, nmea_payload(s), sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    int n = nmea_tokenise(tmp, fields, NMEA_MAX_FIELDS);
    /* RMC fields: 0=talker+msg, 1=time, 2=status, 3=lat, 4=NS, 5=lon, 6=EW,
     * 7=spd, 8=crs, 9=date */
    if (n < 10) return;

    char status = fields[2][0];
    nmea_data.valid = (status == 'A');

    nmea_data.latitude  = nmea_to_deg(fields[3], fields[4][0]);
    nmea_data.longitude = nmea_to_deg(fields[5], fields[6][0]);
    nmea_data.speed_knots = (fields[7][0] != '\0') ? strtof(fields[7], NULL) : 0.0f;
    nmea_data.course_deg  = (fields[8][0] != '\0') ? strtof(fields[8], NULL) : 0.0f;

    const char *time_s = fields[1];
    if (strlen(time_s) >= 6) {
        unsigned int h, m, s;
        sscanf(time_s, "%2u%2u%2u", &h, &m, &s);
        nmea_data.hour   = (uint8_t)h;
        nmea_data.minute = (uint8_t)m;
        nmea_data.second = (uint8_t)s;
    }

    const char *date_s = fields[9];
    if (strlen(date_s) == 6) {
        unsigned int d, mo, y;
        sscanf(date_s, "%2u%2u%2u", &d, &mo, &y);
        nmea_data.day   = (uint8_t)d;
        nmea_data.month = (uint8_t)mo;
        nmea_data.year  = (uint16_t)y;
    }
    if (nmea_data.year < 100) nmea_data.year += 2000;
}

static void parse_gga(char *s)
{
    if (!nmea_checksum_ok(s)) return;

    char *fields[NMEA_MAX_FIELDS] = {0};
    char  tmp[NMEA_BUF_LEN];
    strncpy(tmp, nmea_payload(s), sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    int n = nmea_tokenise(tmp, fields, NMEA_MAX_FIELDS);
    /* GGA fields: 0=talker+msg, 1=time, 2=lat, 3=NS, 4=lon, 5=EW,
     * 6=quality, 7=sats, 8=hdop, 9=alt, 10=alt units ... */
    if (n < 11) return;

    nmea_data.fix_quality = (uint8_t)atoi(fields[6]);
    nmea_data.sat_count   = (uint8_t)atoi(fields[7]);
    nmea_data.altitude_m  = (fields[9][0] != '\0') ? strtof(fields[9], NULL) : 0.0f;

    nmea_data.latitude  = nmea_to_deg(fields[2], fields[3][0]);
    nmea_data.longitude = nmea_to_deg(fields[4], fields[5][0]);

    const char *time_s = fields[1];
    if (strlen(time_s) >= 6) {
        unsigned int h, m, s;
        sscanf(time_s, "%2u%2u%2u", &h, &m, &s);
        nmea_data.hour   = (uint8_t)h;
        nmea_data.minute = (uint8_t)m;
        nmea_data.second = (uint8_t)s;
    }
}

static void parse_gsa(char *s)
{
    if (!nmea_checksum_ok(s)) return;

    /* Only parse the GPS constellation (trailing ,1 or legacy with no sys id) */
    char *star = strrchr(s, '*');
    if (!star) return;
    char *sys = strrchr(s, ',');
    if (sys && sys > star) sys = NULL; /* comma after * is not ours */
    if (sys && sys[1] != '1' && sys[1] != '\0') return;

    char *fields[NMEA_MAX_FIELDS] = {0};
    char  tmp[NMEA_BUF_LEN];
    strncpy(tmp, nmea_payload(s), sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    int n = nmea_tokenise(tmp, fields, NMEA_MAX_FIELDS);
    /* GSA fields: 0=talker+msg, 1=auto_mode, 2=fix_mode, 3..14=PRNs,
     * 15=PDOP, 16=HDOP, 17=VDOP, [18=sys_id] */
    if (n < 18) return;

    nmea_data.fix_mode = (uint8_t)atoi(fields[2]);
    nmea_data.pdop = (fields[15][0] != '\0') ? strtof(fields[15], NULL) : 0.0f;
    nmea_data.hdop = (fields[16][0] != '\0') ? strtof(fields[16], NULL) : 0.0f;
    nmea_data.vdop = (fields[17][0] != '\0') ? strtof(fields[17], NULL) : 0.0f;
}


uint8_t nmea_process_byte(uint8_t b)
{
    if (b == '$' || b == '!') {
        nmea_idx = 0;
    }

    if (nmea_idx < NMEA_BUF_LEN - 1) {
        nmea_buf[nmea_idx++] = (char)b;
    }
    nmea_buf[nmea_idx] = '\0';

    if (b != '\n') return 0;

    if (strstr(nmea_buf, "RMC"))      parse_rmc(nmea_buf);
    else if (strstr(nmea_buf, "GGA")) parse_gga(nmea_buf);
    else if (strstr(nmea_buf, "GSA")) parse_gsa(nmea_buf);

    nmea_idx = 0;
    nmea_buf[0] = '\0';
    return 1;
}

const char *nmea_get_buf(void) { return nmea_buf; }
uint8_t     nmea_get_idx(void) { return nmea_idx; }
