#ifndef BDAT_H
#define BDAT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BDAT_MODE_READ  0
#define BDAT_MODE_WRITE 1

typedef struct bdat_t bdat_t;
struct bdat_t {
    FILE *fp;
    uint8_t mode;
};

bdat_t* bdat_open_read(char*);
bdat_t* bdat_open_write(char*);

uint8_t bdat_get_mode(bdat_t*);

void*    bdat_get_raw    (bdat_t*, void*, int);
uint8_t  bdat_get_uint8  (bdat_t*);
uint16_t bdat_get_uint16 (bdat_t*);
uint32_t bdat_get_uint32 (bdat_t*);
uint64_t bdat_get_uint64 (bdat_t*);
float    bdat_get_float  (bdat_t*);
double   bdat_get_double (bdat_t*);

void bdat_write_raw    (bdat_t*, void*, int);
void bdat_write_uint8  (bdat_t*, uint8_t);
void bdat_write_uint16 (bdat_t*, uint16_t);
void bdat_write_uint32 (bdat_t*, uint32_t);
void bdat_write_uint64 (bdat_t*, uint64_t);
void bdat_write_float  (bdat_t*, float);
void bdat_write_double (bdat_t*, double);

void bdat_close(bdat_t*);

#endif
