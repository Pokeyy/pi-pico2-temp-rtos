#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "dht.h"

#define CMD_BUF_LEN 64

volatile bool use_fahrenheit = false;

// change this to match your setup
static const dht_model_t DHT_MODEL = DHT11;
static const uint DATA_PIN = 0;
static const uint LED_PIN = PICO_DEFAULT_LED_PIN;

static float celsius_to_fahrenheit(float temperature) {
    return temperature * (9.0f / 5) + 32;
}

/* Hardware Setup */
void prvSetupHardware(void)
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void process_commands(const char *cmd) {
    if (strcmp(cmd, "celsius") == 0) {
        use_fahrenheit = false;
        printf("Displaying in Celsius.\n");
    }
    else if(strcmp(cmd, "fahrenheit") == 0) {
        use_fahrenheit = true;
        printf("Displaying in Fahrenheit.\n");
    }
    else {
        printf("Unknown command: %s\n", cmd);
    }
}

void usb_rx_tx_task(void *pvParameters) {
    static char cmd_buffer[CMD_BUF_LEN];
    static int cmd_index = 0;
    while(true) { 
        // if (cmd_index == 0) {
        //     printf("[DEBUG] Waiting for input...\n");
        // }
        if(stdio_usb_connected()) {
            int chars = getchar_timeout_us(0); // waits for input
            if (chars != PICO_ERROR_TIMEOUT) {
                //printf("Received char: '%c' (%d)\n", (char)chars, chars); debug to show serial input per char
                if(chars == ';') {
                    // end of command (serial monitor doesn't account for escape characters, so end command with ;)

                    if(cmd_index > 0) {
                        cmd_buffer[cmd_index] = '\0';
                        printf("[DEBUG] Received command: \"%s\"\n", cmd_buffer);
                        //printf("[DEBUG] cmd_index = %d\n", cmd_index);
                        // for (int i = 0; i < cmd_index; i++) {
                        //     printf("cmd_buffer[%d] = '%c' (%d)\n", i, cmd_buffer[i], cmd_buffer[i]);
                        // }
                        process_commands(cmd_buffer);
                    }
                    else {
                        printf("[DEBUG] Skipping empty command.\n");
                    }
                    cmd_index = 0;
                    memset(cmd_buffer, 0, CMD_BUF_LEN);
                }
                else if(chars != 0 && cmd_index < CMD_BUF_LEN - 1) {
                    cmd_buffer[cmd_index++] = (char)chars;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void temperature_task(void *pvParameters) {
    puts("\nDHT test");
    dht_t dht;
    dht_init(&dht, DHT_MODEL, pio0, DATA_PIN, true /* pull_up */);
    while(true) {
        float humidity;
        float temperature_c;

        dht_start_measurement(&dht);
        dht_result_t result = dht_finish_measurement_blocking(&dht, &humidity, &temperature_c);
        if (result == DHT_RESULT_OK) {
            float display_temperature = use_fahrenheit ? celsius_to_fahrenheit(temperature_c) : temperature_c;
            printf("Temp: %.1f %s | Humidity: %.1f%%\n", display_temperature, use_fahrenheit ? "F" : "C", humidity);
        } 
        else if (result == DHT_RESULT_TIMEOUT) {
            puts("DHT sensor not responding. Please check your wiring.");
        } 
        else {
            assert(result == DHT_RESULT_BAD_CHECKSUM);
            puts("Bad checksum");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    TaskHandle_t xHandle_0, xHandle_1;
    UBaseType_t uxCoreAffinityMask_0, uxCoreAffinityMask_1;
    prvSetupHardware();
    xTaskCreate(temperature_task, "TEMP_TASK", 1024, NULL, 1, &(xHandle_0));
    xTaskCreate(usb_rx_tx_task, "USB_RX_TX_TASK", 1024, NULL, 1, &(xHandle_1));

    uxCoreAffinityMask_0 = (1 << 0); // mask so temperature task only runs on core 0
    uxCoreAffinityMask_1 = (1 << 1); // mask so usb task only runs on core 1
    vTaskCoreAffinitySet(xHandle_0, uxCoreAffinityMask_0);
    vTaskCoreAffinitySet(xHandle_1, uxCoreAffinityMask_1);

    vTaskStartScheduler();

    while(true);
}