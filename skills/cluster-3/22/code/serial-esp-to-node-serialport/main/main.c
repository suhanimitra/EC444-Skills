//Leyandra Burke, Suhani Mitra, Margherita Piana, Kyla Wilson 09/20/2024

/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "math.h"
#include <string.h>
#include <time.h>

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_channel_t channel2 = ADC_CHANNEL_9;
static const adc_atten_t atten = ADC_ATTEN_DB_11;        //0dB attenuation fix for ADC scale
static const adc_unit_t unit = ADC_UNIT_1;

#define RED 15
#define BLUE 27
#define GREEN 33
#define TILT 21
#define BUTTON 12
#define  LUX_CONVERSION_CONSTANT 1940000
//hardware interrupt defines
#define GPIO_INPUT_IO_1       4
#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1 // casting GPIO input to bitmap


static void check_efuse(void)
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

void app_main(void)
{
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel, atten);
        adc2_config_channel_atten(channel2, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }


    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);


    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0;
        uint32_t adc_reading2 = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel);
                int raw;
                adc2_get_raw((adc2_channel_t)channel2, ADC_WIDTH_BIT_12, &raw);
                adc_reading2 += raw;
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
        }
        adc_reading /= NO_OF_SAMPLES;
        adc_reading2 /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);  //photocell
        uint32_t voltage2 = esp_adc_cal_raw_to_voltage(adc_reading2, adc_chars);    //thermistor

        //using code from the following source: https://www.e-tinkers.com/2019/10/using-a-thermistor-with-arduino-and-unexpected-esp32-adc-non-linearity/
        float Ro = 10000.0;
        float beta = 3435;
        
        //Finding celcius
        float thermistor_r = Ro * (3300.0 / voltage2 - 1.0);

        float To = 298.15; 
        float kelvin = 1.0/(1.0/To + log(thermistor_r/Ro)/beta);
        float celcius = kelvin - 273.15;

        //Conversion code: ttps://forums.adafruit.com/viewtopic.php?t=48551 
        //Finding lux
        //float lux = 27.565 * pow(10, voltage/1000);
        uint32_t voltage_toresistance;
        float Rt;
        float R1=10000.0; //voltage divider resistor value
         //voltage_toresistance = (voltage/1000) * 3300/1023.0; //3.3 supplied voltage and 1023 is adc max
        // Rt = R1 * ((3300- voltage)/voltage);
        Rt=R1*(3300.0/voltage-1.0);
         //T = 1/(1/To + log(Rt/Ro)/BCOEFFICIENT);  // Temperature in Kelvin
         //Tc = T - 273.15;                 // Celsius
        float lux = LUX_CONVERSION_CONSTANT / Rt;
        //Finding tilt

        //Display output
        // printf("Time: %d s, Temp: %lf C, Light: %f Lux, Battery1 (photo) %ld mV, Battery2 (temp) %ld mV, Tilt: %s\n", time_taken, celcius, lux, voltage, voltage2, tilt);

        //print temp and lux
        printf("Temp: %lf C, Light: %f Lux\n", celcius, lux);

        vTaskDelay(pdMS_TO_TICKS(1000));        //report every one seconds

    }
}


//we expect Vs = (3.3/{10000 + 10000 (there are resistor / thermistor resistance values)}) * 10000 (r2) = 1.65 Volts
