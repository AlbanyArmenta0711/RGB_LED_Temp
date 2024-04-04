#include <string.h>
#include "wifi_app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "http_server.h"

static const char * TAG = "Wi-Fi softAP";

esp_netif_t * esp_netif_ap = NULL;

/*
 * Wi-Fi application event handler
 * @param arg_data, aside from event data, that is passed to the handler when it is called
 * @param event_base the base id of the event to register the handler for
 * @param event_id the if of the event to register the handler for
 * @param event_data event data
 */
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
    wifi_event_ap_staconnected_t * conn_event;
    wifi_event_ap_stadisconnected_t * disconn_event;

    if(event_base == WIFI_EVENT){
        switch(event_id){
            //Station connected to access point
            case WIFI_EVENT_AP_STACONNECTED:
                conn_event  = (wifi_event_ap_staconnected_t*) event_data;
                ESP_LOGI(TAG, "station "MACSTR" join, AID=%d", MAC2STR(conn_event->mac), conn_event->aid);
                break;
            case WIFI_EVENT_AP_STADISCONNECTED:
                disconn_event  = (wifi_event_ap_stadisconnected_t*) event_data;
                ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d", MAC2STR(disconn_event->mac), disconn_event->aid);
                break; 
        }
    }
}

/*
 * 1.- INIT PHASE
 */
//Function to initialize Wi-Fi access point according to Espressif steps
static void wifi_init_softap(void){
    //Initialize the TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    //Create system Event task and initialize an application event's callback function
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    //Register an instance of event handler to the default loop
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_event_handler, //Function callback
        NULL,
        NULL
    ));

    //Create default network instance
    esp_netif_ap = esp_netif_create_default_wifi_ap();
    //Create the Wifi driver task with default configuration
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT(); 
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    ESP_LOGI(TAG, "Wi-Fi init phase finished");
}

/*
 * 2.- CONFIGURE PHASE
 */
//Function to configure the Wi-Fi driver once it is initialized
static void  wifi_config_softap(void){
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .ssid_len = strlen(WIFI_SSID),
            .channel = WIFI_CHANNEL,
            .authmode = WIFI_AUTH_WPA2_PSK, 
            .ssid_hidden = 0, //broadcast the SSID
            .max_connection = WIFI_MAX_STA_CONN,
            .beacon_interval = 100 //default value
        }
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_LOGI(TAG, "Wi-Fi configuration finished. SSID: %s", WIFI_SSID);
}


void wifi_start_app(void){
    //Call init phase function
    wifi_init_softap();
    //Call configure phase function
    wifi_config_softap();
    //Start Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Wi-Fi app started ");
    
    //Start http server 
    http_start_server();
}