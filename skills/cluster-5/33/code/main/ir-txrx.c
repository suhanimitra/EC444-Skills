
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

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

#define LED_RED_PIN 15
#define LED_GREEN_PIN 14
#define LED_BLUE_PIN 32

#define DEFAULT_VREF    1100
#define NO_OF_SAMPLES   64
#define DT_MS           1000
#define KP              1.0f
#define KI              0.5f
#define KD              0.1f
#define SETPOINT        11.224489f  // we want the object to be 11.224489 cm away from the sensor

static float error, integral, derivative, previous_error = 0;

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_6;
static const adc_unit_t unit = ADC_UNIT_1;

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

void led_init() {
    gpio_reset_pin(LED_RED_PIN);
    gpio_reset_pin(LED_GREEN_PIN);
    gpio_reset_pin(LED_BLUE_PIN);

    gpio_set_direction(LED_RED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GREEN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_BLUE_PIN, GPIO_MODE_OUTPUT);
}

void update_leds(float error) {
    if (error < 0) {
        printf("Object is too far\n");
        gpio_set_level(LED_RED_PIN, 1);
        gpio_set_level(LED_GREEN_PIN, 0);
        gpio_set_level(LED_BLUE_PIN, 0);
    } else if (error == 0) {
        printf("Object is at the correct distance\n");
        gpio_set_level(LED_RED_PIN, 0);
        gpio_set_level(LED_GREEN_PIN, 1);
        gpio_set_level(LED_BLUE_PIN, 0);
    } else {
        printf("Object is too close\n");
        gpio_set_level(LED_RED_PIN, 0);
        gpio_set_level(LED_GREEN_PIN, 0);
        gpio_set_level(LED_BLUE_PIN, 1);
    }
}

void pid_task(void *arg) {
    while (1) {
        uint32_t adc_reading = 0;
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel);
            }
        }
        adc_reading /= NO_OF_SAMPLES;

        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
        printf("Raw ADC Reading: %ld\tVoltage: %ld mV\n", adc_reading, voltage);

        float measured_value = voltage / 9.8f;

        error = SETPOINT - measured_value;
        integral += error * (DT_MS / 1000.0f);
        derivative = (error - previous_error) / (DT_MS / 1000.0f);

        float output = (KP * error) + (KI * integral) + (KD * derivative);
        previous_error = error;

        printf("Measured Value: %.6f cm, Error: %.6f, Output: %.2f\n", measured_value, error, output);
        
        update_leds(error);

        vTaskDelay(pdMS_TO_TICKS(DT_MS));
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
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    led_init();

    xTaskCreate(pid_task, "pid_task", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
}

