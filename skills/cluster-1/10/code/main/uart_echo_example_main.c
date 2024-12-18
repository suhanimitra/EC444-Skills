/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include <string.h>

/**
 * This is an example which echos any data it receives on configured UART back to the sender,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: configured UART
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below (See Kconfig)
 */

#define ECHO_TEST_TXD (CONFIG_EXAMPLE_UART_TXD)
#define ECHO_TEST_RXD (CONFIG_EXAMPLE_UART_RXD)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM      (0)        ///set to zero for huzzah board
#define ECHO_TASK_STACK_SIZE    (CONFIG_EXAMPLE_TASK_STACK_SIZE)

static const char *TAG = "UART TEST";
static uint8_t s_led_state = 0;

#define BUF_SIZE (1024)

enum states {stateOne = 1, stateTwo = 2, stateThree = 3};
static enum states currState = stateOne;        //initialize

static void configure_led(int gpin)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(gpin);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(gpin, GPIO_MODE_OUTPUT);
}

static void blink_led(int gpin)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(gpin, s_led_state);
}

static void echo_task(void *arg)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
        // printf("Len: %d\n", len);
        // Write data back to the UART
        //uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) data, len);
        if (len > 0) {
            data[len] = '\0';
            //uart_write_bytes(UART_NUM_0, (const char *) data, len);
            //ESP_LOGI(TAG, "Recv str: %s", (char *) data);

            blink_led(13);
            s_led_state = !s_led_state;

            //if in put is s go to next mode:
            if (data[0] == 's') {
                currState ++;
                if (currState > 3) {
                    currState = 1; 
                }
                printf("State: %d\n", currState);
                if (currState == stateOne) {
                printf("Toggle Mode\n");
            }
            else if (currState == stateTwo) {
                printf("Echo Mode\n");
            }
            else if (currState == stateThree) {
                printf("Echo Decimal to Hex Mode\n");
            }
            }    

            else {
                switch(currState){
                    case stateOne:
                        //Toggle the onboard LED based on key input 't'
                        if (data[0] == 't') {
                            printf("Read: %s", (const char * ) data);
                            blink_led(13);
                            gpio_set_level(GPIO_NUM_2, 1);
                            vTaskDelay(1000 / portTICK_PERIOD_MS);
                            gpio_set_level(GPIO_NUM_2, 0);
                        }
                        break;
                    case stateTwo:
                        //Echo input (2 or more characters) from the keyboard to the console
                        if (len >= 2) {
                            printf("Echo: %s", data);
                        }
                        break;
                    case stateThree:
                        //Echo a decimal number input as a hexadecimal
                        printf("Enter an integer: \n");
                        int num = atoi((const char *)data);
                        printf("Hex: 0x%X\n", num);
                        break;
                    default:
                        printf("Error\n");
                        break;
                }
            } 
        }
    }
}

void app_main(void)
{
    xTaskCreate(echo_task, "uart_echo_task", 2048, NULL, 10, NULL);
    configure_led(13);
}
