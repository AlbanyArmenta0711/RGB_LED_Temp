#include "esp_http_server.h"
#include "esp_log.h"
#include "http_server.h"
#include "sys/param.h"
#include "rgb_led.h"
#include "DHT22.h"

static const char TAG[] = "http_server";
 
//Embedded files: JQuery, index.html, and app.js files
extern const uint8_t jquery_3_3_1_min_js_start[] asm("_binary_jquery_3_3_1_min_js_start");
extern const uint8_t jquery_3_3_1_min_js_end[] asm("_binary_jquery_3_3_1_min_js_end");
extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");
extern const uint8_t app_js_start[] asm("_binary_app_js_start");
extern const uint8_t app_js_end[] asm("_binary_app_js_end");
extern const uint8_t favicon_ico_start[] asm("_binary_favicon_ico_start");
extern const uint8_t favicon_ico_end[] asm("_binary_favicon_ico_end");


//Jquery request handler
static esp_err_t jquery_handler(httpd_req_t * req){
    ESP_LOGI(TAG, "jquery requested");
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char * )jquery_3_3_1_min_js_start, jquery_3_3_1_min_js_end - jquery_3_3_1_min_js_start );
    return ESP_OK; 
}

//Index.html request handler 
static esp_err_t index_html_handler(httpd_req_t * req){
    ESP_LOGI(TAG, "index.html requested");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char * )index_html_start, index_html_end - index_html_start );
    return ESP_OK; 
}

//app.js request handler 
static esp_err_t app_js_handler(httpd_req_t * req){
    ESP_LOGI(TAG, "app.js requested");
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char * )app_js_start, app_js_end - app_js_start );
    return ESP_OK; 
}

//favicon request handler
static esp_err_t http_server_favicon_ico_handler(httpd_req_t *req){
	ESP_LOGI(TAG, "favicon.ico requested");
	httpd_resp_set_type(req, "image/x-icon");
	httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_end - favicon_ico_start);
	return ESP_OK;
}

//DHT22 sensor reading handler
static esp_err_t http_server_dht_handler(httpd_req_t *req){
    ESP_LOGI(TAG, "dhtSensor.json requested");
    char dhtSensorJSON[100];
    sprintf(dhtSensorJSON, "{\"temp\":\"%.2f\",\"humidity\":\"%.2f\"}",getTemperature(), getHumidity());
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, dhtSensorJSON, strlen(dhtSensorJSON));
    return ESP_OK;
}

static esp_err_t http_server_rgb_change_handler(httpd_req_t * req){
    char color_received [12];
    ESP_LOGI(TAG, "RGBchange posted");
    size_t recv_size = MIN(req->content_len, sizeof(color_received));
    //Read content of the request
    int ret = httpd_req_recv(req, color_received, recv_size);
    ESP_LOGI(TAG, "new color: %s", color_received);
    update_color(color_received);
    /* Send a simple response */
    const char resp[] = "Color changed!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

//Function for starting webserver
httpd_handle_t start_webserver(void){
    httpd_handle_t server = NULL;
    //Default configuration
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    //Start the httpd server
    if(httpd_start(&server, &config) == ESP_OK){
        //Register uri handlers
        //jquery handler
        httpd_uri_t jquery_uri = {
            .uri = "/jquery-3.3.1.min.js",
            .method = HTTP_GET,
            .handler = jquery_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &jquery_uri);

        //index.html handler
        httpd_uri_t index_html_uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = index_html_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &index_html_uri);

        //app.js handler
        httpd_uri_t app_js_uri = {
            .uri = "/app.js",
            .method = HTTP_GET,
            .handler = app_js_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &app_js_uri);

        //register favicon handler
		httpd_uri_t favicon_ico = {
				.uri = "/favicon.ico",
				.method = HTTP_GET,
				.handler = http_server_favicon_ico_handler,
				.user_ctx = NULL
		};
		httpd_register_uri_handler(server, &favicon_ico);

        //RGBChange handler
        httpd_uri_t rgb_change_uri = {
            .uri = "/RGBchange",
            .method = HTTP_POST,
            .handler = http_server_rgb_change_handler,
            .user_ctx = NULL
        };
		httpd_register_uri_handler(server, &rgb_change_uri);

        //dhtSensor.json handler
        httpd_uri_t dht_sensor_json_uri = {
            .uri = "/dhtSensor.json",
            .method = HTTP_GET,
            .handler = http_server_dht_handler,
            .user_ctx = NULL
        };
		httpd_register_uri_handler(server, &dht_sensor_json_uri);
    }

    return server;
}

void http_start_server(void){
    //Create an instance of HTTP server based on initial configuration and handlers
    httpd_handle_t http_webserver = start_webserver(); 
}