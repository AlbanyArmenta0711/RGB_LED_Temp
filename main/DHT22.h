/* 

	DHT22 temperature sensor driver

*/

#ifndef DHT22_H_  
#define DHT22_H_

#define DHT_OK 0
#define DHT_CHECKSUM_ERROR -1
#define DHT_TIMEOUT_ERROR -2

#define DHT_GPIO			25
#define DHT22_TASK_STACK_SIZE 4096
#define DHT22_TASK_PRIORITY 5
#define DHT22_TASK_CORE_ID 0

/**
 * Starts DHT22 sensor task
 */
void DHT22_task_start(void);

// == function prototypes =======================================

void 	setDHTgpio(int gpio);
void 	errorHandler(int response);
int 	readDHT();
float 	getHumidity();
float 	getTemperature();
int 	getSignalLevel( int usTimeOut, bool state );

#endif
