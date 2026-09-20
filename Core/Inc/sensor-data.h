/*
 * sensor-data.h
 *
 *  Created on: 12 de ago. de 2026
 *      Author: eduardo-reichert
 */

#ifndef INC_SENSOR_DATA_H_
#define INC_SENSOR_DATA_H_

// Estrutura para dados do BME280
typedef struct {
    float temperatura;   // °C
    float pressao;       // Pa ou hPa
    float humidade;      // %
} BME_Data_t;

// Estrutura para dados do BNO085 (IMU)
typedef struct {
    // Quatérnio (Orientação Absoluta)
    float qx, qy, qz, qw;
    // Aceleração (m/s²)
    float accel_x, accel_y, accel_z;
    // Velocidade Angular (rad/s)
    float gyro_x, gyro_y, gyro_z;
} BNO085_Data_t;

// Estrutura para dados do GPS
typedef struct {
    float latitude;      // graus decimais
    float longitude;     // graus decimais
    float altitude;      // metros
    //float speed;         // nós ou km/h
} GPS_Data_t;

typedef struct{
	BME_Data_t *bme;
	BNO085_Data_t *bno;
	GPS_Data_t *gps;
}Sensores_Data_t;

void Sensores_Data_Init(Sensores_Data_t *sd, BME_Data_t *bme_data, BNO085_Data_t *bno_data, GPS_Data_t *gps_data);

#endif /* INC_SENSOR_DATA_H_ */