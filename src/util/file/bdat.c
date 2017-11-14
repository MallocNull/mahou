#include "bdat.h"

bdat_t* bdat_open_read(char *file) {
    bdat_t *bdat = (bdat_t*)malloc(sizeof(bdat_t));
    bdat->fp = fopen(file, "rb");
    bdat->mode = BDAT_MODE_READ;
    return bdat;
}

bdat_t* bdat_open_write(char *file) {
    bdat_t *bdat = (bdat_t*)malloc(sizeof(bdat_t));
    bdat->fp = fopen(file, "wb+");
    bdat->mode = BDAT_MODE_WRITE;
    return bdat;
}

uint8_t bdat_get_mode(bdat_t *bdat) {
    return bdat->mode;
}

void* bdat_get_raw(bdat_t *bdat, void *data, int length) {
    if(bdat->mode != BDAT_MODE_READ)
        return NULL;
    
    fread(data, length, 1, bdat->fp);
}

uint8_t bdat_get_uint8(bdat_t *bdat) {
    if(bdat->mode != BDAT_MODE_READ)
        return 0;
    
    uint8_t data;
    fread(&data, 1, 1, bdat->fp);
    return data;
}

uint16_t bdat_get_uint16(bdat_t *bdat) {
    if(bdat->mode != BDAT_MODE_READ)
        return 0;
    
    uint16_t data;
    fread(&data, sizeof(data), 1, bdat->fp);
    return data;
}

uint32_t bdat_get_uint32(bdat_t *bdat) {
    if(bdat->mode != BDAT_MODE_READ)
        return 0;
    
    uint32_t data;
    fread(&data, sizeof(data), 1, bdat->fp);
    return data;
}

uint64_t bdat_get_uint64(bdat_t *bdat) {
    if(bdat->mode != BDAT_MODE_READ)
        return 0;
    
    uint64_t data;
    fread(&data, sizeof(data), 1, bdat->fp);
    return data;
}

float bdat_get_float(bdat_t *bdat) {
    if(bdat->mode != BDAT_MODE_READ)
        return 0;
    
    float data;
    fread(&data, sizeof(data), 1, bdat->fp);
    return data;
}

double bdat_get_double(bdat_t *bdat) {
    if(bdat->mode != BDAT_MODE_READ)
        return 0;
    
    double data;
    fread(&data, sizeof(data), 1, bdat->fp);
    return data;
}

void bdat_write_raw(bdat_t *bdat, void *data, int length) {
    if(bdat->mode != BDAT_MODE_WRITE)
        return;
    
    fwrite(data, length, 1, bdat->fp);
}

void bdat_write_uint8(bdat_t *bdat, uint8_t data) {
    bdat_write_raw(bdat, (void*)(&data), 1);
}

void bdat_write_uint16(bdat_t *bdat, uint16_t data) {
    bdat_write_raw(bdat, (void*)(&data), sizeof(data));
}

void bdat_write_uint32(bdat_t *bdat, uint32_t data) {
    bdat_write_raw(bdat, (void*)(&data), sizeof(data));
}

void bdat_write_uint64(bdat_t *bdat, uint64_t data) {
    bdat_write_raw(bdat, (void*)(&data), sizeof(data));
}

void bdat_write_float(bdat_t *bdat, float data) {
    bdat_write_raw(bdat, (void*)(&data), sizeof(data));
}

void bdat_write_double(bdat_t *bdat, double data) {
    bdat_write_raw(bdat, (void*)(&data), sizeof(data));
}

void bdat_close(bdat_t *bdat) {
    fclose(bdat->fp);
    free(bdat);
}