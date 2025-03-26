#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include "dht.h"

/* Example Application Tasks */
void firstTask(void *pvParameters)
{
    while (true) {
        printf("First task\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void secondTask(void *pvParameters)
{
    while (true) {
        printf("Second task\n");
        vTaskDelay(pdMS_TO_TICKS(950));
    }
}
/* End of Example Application Tasks */

/* Hardware Setup */
void prvSetupHardware(void)
{
    stdio_init_all();
}

int main()
{
    /* Perform any hardware setup necessary. */
    prvSetupHardware();

    /* Example Application Tasks */
    auto result1 = xTaskCreate(firstTask, "First Task", 1024, NULL, 1, NULL);
    if (result1 != pdPASS) {
        printf("Failed to create task\n");
        return 1;
    }
    
    auto result = xTaskCreate(secondTask, "Second Task", 1024, NULL, 1, NULL);
    if (result != pdPASS) {
        printf("Failed to create task\n");
        return 1;
    }
    /* End of Example Application Tasks */

    /* Start the created tasks running. */
    vTaskStartScheduler(); // Does not return

    /* Execution will only reach here if there was insufficient heap to start the scheduler. */
    for (;;);
    return 0;
}