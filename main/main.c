/*
 * @file : main.c
 * @author : Avinashee Tech
 * @brief : main project source file. simple demonstration to receive data from 
            1-wire based DS18B20 temperature sensor with ESP32.  
 */ 


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "ds18b20.h"

#define UNUSED(x)         (void)(x)
#define DETECTED          1
#define NOT_DETECTED      0  


/***********************************************************************
 *@brief : main function 
 *@param : None
 *@retval : None
 *@note : checks device presence. if detected, traansmits command sequence 
 and decodes temperature received. if not detected, keeps looking for device 
 every 1 second.
************************************************************************/
void app_main(void)
{
    
    vTaskDelay(100); //small delay after cold start

    uint8_t received_data[9] = {0}; //buffer to store received data
    int device_presence = Init();  //reset and check for device presence
    ESP_LOGI(TAG,"device presence : %s",device_presence?"detected":"not detected");

    while(1){
        if(device_presence==DETECTED){
            GetData(received_data);
            float temp = DecodeTemp(received_data[0],received_data[1],DS18B20_RESOLUTION_12_BIT); 
            ESP_LOGI(TAG,"temperature:%.2f",temp);
            vTaskDelay(400);
        }else{
            ESP_LOGI(TAG,"device not found...re-checking....");
            device_presence = Init();
            ESP_LOGI(TAG,"device presence : %s",device_presence?"detected":"not detected");
            vTaskDelay(1000);
        }
        
    }
   
}
