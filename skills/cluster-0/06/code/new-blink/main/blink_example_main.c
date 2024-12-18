/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

static uint8_t s_led_state = 0;

#ifdef CONFIG_BLINK_LED_STRIP

//lights defintions
static const uint8_t SS    = 2;
static const uint8_t MOSI  = 18;
static const uint8_t MISO  = 19;
static const uint8_t SCK   = 5;
// mapping to match other feathers and also in order
static const uint8_t A0 = 26;
static const uint8_t A1 = 25;
static const uint8_t A2 = 34;
static const uint8_t A3 = 39;
static const uint8_t A4 = 36;
static const uint8_t A5 = 4;

static led_strip_handle_t led_strip;

static void blink_led(void)
{
    /* If the addressable LED is enabled */
    if (s_led_state) {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        led_strip_set_pixel(led_strip, 0, 16, 16, 16);
        /* Refresh the strip to send data */
        led_strip_refresh(led_strip);
    } else {
        /* Set all LED off to clear all pixels */
        led_strip_clear(led_strip);
    }
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
#if CONFIG_BLINK_LED_STRIP_BACKEND_RMT
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
#elif CONFIG_BLINK_LED_STRIP_BACKEND_SPI
    led_strip_spi_config_t spi_config = {
        .spi_bus = SPI2_HOST,
        .flags.with_dma = true,
    };
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
#else
#error "unsupported LED strip backend"
#endif
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

#elif CONFIG_BLINK_LED_GPIO

static void blink_led(int gpin)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(gpin, s_led_state);
}

static void configure_led(int gpin)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(gpin);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(gpin, GPIO_MODE_OUTPUT);
}

#else
#error "unsupported LED type"
#endif

void app_main(void)
{
    /* Configure the peripheral according to the LED type */
    configure_led(BLINK_GPIO);
    configure_led(5);
    configure_led(26);
    configure_led(25);
    // configure_led(36);
    configure_led(4);

    while (1) {
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led(BLINK_GPIO);
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, s_led_state);

        /* Make a blink-type program that illuminates the LEDs as follows (LED positions 4, 3, 2, 1)
            Period is 1s
            All LEDs off
            LED 1 on (1 s)
            LED 1,2 on
            LED 1,2,3 on
            LED 1,2,3,4 on
            Binary countdown, 1s each, back to all LEDs off
            Repeat */
        gpio_set_level(25, 1);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(26, 1);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(5, 1);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 1);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        
        
        // Binary countdown
        gpio_set_level(4, 0);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 1);
        gpio_set_level(5, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 1);
        gpio_set_level(5, 1);
        gpio_set_level(26, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 1);
        gpio_set_level(5, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 1);
        gpio_set_level(5, 1);
        gpio_set_level(26, 1);
        gpio_set_level(25, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 1);
        gpio_set_level(5, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 1);
        gpio_set_level(5, 1);
        gpio_set_level(26, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 1);
        gpio_set_level(5, 0);

        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        gpio_set_level(4, 0);
    }
}
