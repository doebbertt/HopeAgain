/********************************************************
 * Author: SUK & Thomas
 * Date: 03/20/2016
 * sensor.c (sensor library source file)
 ********************************************************/


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "sensor.h"



//--------------------------------------------------------------
// global variables
//--------------------------------------------------------------
uint8_t ds_data_read[9]; // to save the sensor data



//--------------------------------------------------------------
// internal functions
//--------------------------------------------------------------
float getTempSensorVal();
float getHumiditySensorVal();
float getLightSensorVal();


//--------------------------------------------------------------
// reading the temperature from the sensor
// return_val : temperature in degrees of celcius [+125.0 to -55.0]
// if an error occurs "Sensor_TEMP_ERR_CODE" will be returned
//--------------------------------------------------------------
float getTempSensorVal()
{
  float ret_val=22.1;

  return(ret_val);
}

//--------------------------------------------------------------
// reading the humidity from the sensor
// return_val : humidity in percent [0 to 100]
// if an error occurs "Sensor_HUM_ERR_CODE" will be returned
//--------------------------------------------------------------
float getHumSensorVal()
{
  float ret_val=50;

  return(ret_val);
}

//--------------------------------------------------------------
// reading the light values from the sensor
// return_val : light SI lux units or percent [0 to 100]
// if an error occurs "Sensor_LIGHT_ERR_CODE" will be returned
//--------------------------------------------------------------
float getLightSensorVal()
{
  float ret_val=20;

  return(ret_val);
}


//--------------------------------------------
// converts a float value into a string value
//--------------------------------------------
#ifdef USE_TEMP2STR
void Sensor_TempToStr(float val, char *ptr)
{
  int16_t precomma;
  uint16_t pastcomma;
  float rest;

  precomma=(int16_t)(val);
  if(val>=0) {
    rest = val-(float)(precomma);
  }
  else {
    rest = (float)(precomma)-val;
  }
  pastcomma = (uint16_t)(rest*(float)(1000)+0.5);

  sprintf(ptr,"%d.%03d",precomma,pastcomma);
}
#endif

//--------------------------------------------
// converts a float value into a string value
//--------------------------------------------
#ifdef USE_HUM2STR
void Sensor_HumToStr(float val, char *ptr)
{
  int16_t precomma;
  uint16_t pastcomma;
  float rest;

  precomma=(int16_t)(val);
  if(val>=0) {
    rest = val-(float)(precomma);
  }
  else {
    rest = (float)(precomma)-val;
  }
  pastcomma = (uint16_t)(rest*(float)(1000)+0.5);

  sprintf(ptr,"%d.%03d",precomma,pastcomma);
}
#endif


//--------------------------------------------
// converts a float value into a string value
//--------------------------------------------
#ifdef USE_LIGHT2STR
void Sensor_LightToStr(float val, char *ptr)
{
  int16_t precomma;
  uint16_t pastcomma;
  float rest;

  precomma=(int16_t)(val);
  if(val>=0) {
    rest = val-(float)(precomma);
  }
  else {
    rest = (float)(precomma)-val;
  }
  pastcomma = (uint16_t)(rest*(float)(1000)+0.5);

  sprintf(ptr,"%d.%03d",precomma,pastcomma);
}
#endif





