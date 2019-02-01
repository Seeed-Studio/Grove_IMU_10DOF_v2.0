#ifndef _BMP280_H_
#define _BMP280_H_

#include <Arduino.h>
#include <Wire.h>

#define BMP280_ADDRESS   0x77

#define BMP280_REG_DIG_T1    0x88
#define BMP280_REG_DIG_T2    0x8A
#define BMP280_REG_DIG_T3    0x8C

#define BMP280_REG_DIG_P1    0x8E
#define BMP280_REG_DIG_P2    0x90
#define BMP280_REG_DIG_P3    0x92
#define BMP280_REG_DIG_P4    0x94
#define BMP280_REG_DIG_P5    0x96
#define BMP280_REG_DIG_P6    0x98
#define BMP280_REG_DIG_P7    0x9A
#define BMP280_REG_DIG_P8    0x9C
#define BMP280_REG_DIG_P9    0x9E

#define BMP280_REG_CHIPID          0xD0
#define BMP280_REG_VERSION         0xD1
#define BMP280_REG_SOFTRESET       0xE0

#define BMP280_REG_CONTROL         0xF4
#define BMP280_REG_CONFIG          0xF5
#define BMP280_REG_PRESSUREDATA    0xF7
#define BMP280_REG_TEMPDATA        0xFA

class BMP280
{
public:
  enum sensor_sampling {
            SAMPLING_NONE = 0x00,
            SAMPLING_X1   = 0x01,
            SAMPLING_X2   = 0x02,
            SAMPLING_X4   = 0x03,
            SAMPLING_X8   = 0x04,
            SAMPLING_X16  = 0x05
        };

        enum sensor_mode {
            MODE_SLEEP  = 0x00,
            MODE_FORCED = 0x01, //And 0x02 ?
            MODE_NORMAL = 0x03,
            MODE_SOFT_RESET_CODE = 0xB6
        };

        enum sensor_filter {
            FILTER_OFF = 0x00,
            FILTER_X2  = 0x01,
            FILTER_X4  = 0x02,
            FILTER_X8  = 0x03,
            FILTER_X16 = 0x04
        };

        // standby durations in ms 
        enum standby_duration {
            STANDBY_MS_1      = 0x00, // 0.5ms
            STANDBY_MS_63     = 0x01, //62.5ms
            STANDBY_MS_125    = 0x02, //125.ms
            STANDBY_MS_250    = 0x03,
            STANDBY_MS_500    = 0x04,
            STANDBY_MS_1000   = 0x05,
            STANDBY_MS_2000   = 0x06,
            STANDBY_MS_4000   = 0x07
        };

  bool init(void);
  float getTemperature(void);
  uint32_t getPressure(void);
  float calcAltitude(float pressure);
  
  void setSampling(sensor_mode mode  = MODE_NORMAL,
			 sensor_sampling tempSampling  = SAMPLING_X16,
			 sensor_sampling pressSampling = SAMPLING_X16,
			 sensor_filter filter          = FILTER_OFF,
			 standby_duration duration     = STANDBY_MS_1
			 );

private:
  // Calibration data
  uint16_t dig_T1;
  int16_t dig_T2;
  int16_t dig_T3;

  uint16_t dig_P1;
  int16_t dig_P2;
  int16_t dig_P3;
  int16_t dig_P4;
  int16_t dig_P5;
  int16_t dig_P6;
  int16_t dig_P7;
  int16_t dig_P8;
  int16_t dig_P9;

  int32_t t_fine;

  uint8_t bmp280Read8(uint8_t reg);
  uint16_t bmp280Read16(uint8_t reg);
  uint16_t bmp280Read16LE(uint8_t reg);
  int16_t bmp280ReadS16(uint8_t reg);
  int16_t bmp280ReadS16LE(uint8_t reg);
  uint32_t bmp280Read24(uint8_t reg);
  void writeRegister(uint8_t reg, uint8_t val);
	
	// config register
    struct config {
        // inactive duration (standby time) in normal mode
        unsigned int t_sb : 3;

        // filter settings
        unsigned int filter : 3;

        // unused - don't set
        unsigned int none : 1;
        unsigned int spi3w_en : 1;

        unsigned int get() {
            return (t_sb << 5) | (filter << 3) | spi3w_en;
        }
    }; config _configReg;

    // ctrl_meas register
    struct ctrl_meas {
        // temperature oversampling
        unsigned int osrs_t : 3;

        // pressure oversampling
        unsigned int osrs_p : 3;

        // device mode
        unsigned int mode : 2;

        unsigned int get() {
            return (osrs_t << 5) | (osrs_p << 3) | mode;
        }
    }; ctrl_meas _measReg;

};

#endif
