#ifndef _dfb_extra_h
#define _dfb_extra_h

#define HOLDA_DALIGN_LOW    0x00
#define HOLDA_DALIGN_HIGH   0x04

#define HOLDB_DALIGN_LOW    0x00
#define HOLDB_DALIGN_HIGH   0x08

#define STAGEA_DALIGN_LOW   0x00
#define STAGEA_DALIGN_HIGH  0x01

#define STAGEB_DALIGN_LOW   0x00
#define STAGEB_DALIGN_HIGH  0x02

#define HOLDA_KEY_MASK      0x30
#define HOLDA_KEY_LOW       0x00
#define HOLDA_KEY_MID       0x10
#define HOLDA_KEY_HIGH      0x20

#define HOLDB_KEY_MASK      0xC0
#define HOLDB_KEY_LOW       0x00
#define HOLDB_KEY_MID       0x40
#define HOLDB_KEY_HIGH      0x80

#define STAGEA_KEY_MASK     0x03
#define STAGEA_KEY_LOW      0x00
#define STAGEA_KEY_MID      0x01
#define STAGEA_KEY_HIGH     0x02

#define STAGEB_KEY_MASK     0x0C
#define STAGEB_KEY_LOW      0x00
#define STAGEB_KEY_MID      0x04
#define STAGEB_KEY_HIGH     0x08

#endif

