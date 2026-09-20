/*
 * sensores-data.c
 *
 *  Created on: 13 de ago. de 2026
 *      Author: eduardo-reichert
 */

#include "sensor-data.h"

void Sensores_Data_Init(Sensores_Data_t *sd, BME_Data_t *bme_data, BNO085_Data_t *bno_data, GPS_Data_t *gps_data)
{
    sd->bme = bme_data;
    sd->bno = bno_data;
    sd->gps = gps_data;

    sd->bme->temperatura = 0;
    sd->bme->pressao = 0;
    sd->bme->humidade = 0;

    sd->bno->qx = 0;
    sd->bno->qy = 0;
    sd->bno->qz = 0;
    sd->bno->qw = 0;

    sd->bno->accel_x = 0;
    sd->bno->accel_y = 0;
    sd->bno->accel_z = 0;

    sd->bno->gyro_x = 0;
    sd->bno->gyro_y = 0;
    sd->bno->gyro_z = 0;

    sd->gps->latitude = 0;
    sd->gps->longitude = 0;
    sd->gps->altitude = 0;
}