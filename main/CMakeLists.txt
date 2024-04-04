idf_component_register(
    SRCS "DHT22.c" "http_server.c" "wifi_app.c" "rgb_led.c" "main.c"
    INCLUDE_DIRS "."
    EMBED_FILES webpage/app.js webpage/index.html webpage/jquery-3.3.1.min.js webpage/favicon.ico
)