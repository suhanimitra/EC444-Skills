#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#ifdef CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN
#include "addr_from_stdin.h"
#endif

#if defined(CONFIG_EXAMPLE_IPV4)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#elif defined(CONFIG_EXAMPLE_IPV6)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#else
#define HOST_IP_ADDR ""
#endif


#define PORT 1111 // Port number for the UDP server
#define LED_GPIO_PIN 13 // GPIO pin number for LED

static const char *TAG_UDP = "udp_server";


// Function to handle UDP server tasks
static void udp_server_task(void *pvParameters)
{
    char rx_buffer[128];
    char addr_str[128];
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);

    int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (sock < 0) {
        ESP_LOGE(TAG_UDP, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG_UDP, "Socket created");

    int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAG_UDP, "Socket unable to bind: errno %d", errno);
        close(sock);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG_UDP, "Socket bound, port %d", PORT);

    while (1) {
        ESP_LOGI(TAG_UDP, "Waiting for data");
        struct sockaddr_in source_addr;
        socklen_t socklen = sizeof(source_addr);
        int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

        // Error occurred during receiving
        if (len < 0) {
            ESP_LOGE(TAG_UDP, "recvfrom failed: errno %d", errno);
            break;
        }
        // Data received
        else {
            rx_buffer[len] = 0; // Null-terminate received data
            ESP_LOGI(TAG_UDP, "Received %d bytes from %s:", len, addr_str);
            ESP_LOGI(TAG_UDP, "%s", rx_buffer);

            // Check if the message is to turn on the LED
            if (strncmp(rx_buffer, "TURN ON LED", len) == 0) {
                gpio_set_level(LED_GPIO_PIN, 1); // Turn on the LED
                ESP_LOGI(TAG_UDP, "LED turned ON");

                // Send acknowledgment back to the client
                const char *ack_msg = "LED ON ACK";
                sendto(sock, ack_msg, strlen(ack_msg), 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
            } else if (strncmp(rx_buffer, "TURN OFF LED", len) == 0) {
                gpio_set_level(LED_GPIO_PIN, 0); // Turn off the LED
                ESP_LOGI(TAG_UDP, "LED turned OFF");

                // Send acknowledgment back to the client
                const char *ack_msg = "LED OFF ACK";
                sendto(sock, ack_msg, strlen(ack_msg), 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
            }
        }
    }

    if (sock != -1) {
        ESP_LOGE(TAG_UDP, "Shutting down socket and restarting...");
        shutdown(sock, 0);
        close(sock);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    // Initialize the NVS
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect()); // Connect to WiFi

    // Configure LED GPIO
    esp_rom_gpio_pad_select_gpio(LED_GPIO_PIN);
    gpio_set_direction(LED_GPIO_PIN, GPIO_MODE_OUTPUT);

    xTaskCreate(udp_server_task, "udp_server", 4096, NULL, 5, NULL);
}
