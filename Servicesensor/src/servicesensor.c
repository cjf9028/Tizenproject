#include <tizen.h>
#include <service_app.h>
#include <sensor.h>
#include <curl/curl.h>
#include <net_connection.h>
#include "servicesensor.h"
#define LOG_TAG "test"
#define LOG_TAG2 "test2"
struct last{
	double gyroX;
	double gyroY;
	double gyroZ;
	double heartR;
}lastData;
typedef struct _sensor_info
{
	sensor_h sensor; /**< Sensor handle */
	sensor_listener_h sensor_listener;
} sensorinfo;
static sensorinfo sensor_info;

void send_data() {
   CURL *curl;
   CURLcode res;
   char buff[60];
   sprintf(buff,"gyrox=%f&gyroy=%f&gyroz=%f&hrm=%f",lastData.gyroX,lastData.gyroY,lastData.gyroZ,lastData.heartR);
   dlog_print(DLOG_INFO,"test", buff);
   curl_global_init(CURL_GLOBAL_ALL);
   curl = curl_easy_init();
   if(curl) {
	   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	   curl_easy_setopt(curl, CURLOPT_URL, "http://172.31.99.102/test2.php");
	   curl_easy_setopt(curl, CURLOPT_POST, 1);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS,buff);
	   res = curl_easy_perform(curl);
	   long statLong;
	   curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &statLong);
	   dlog_print(DLOG_ERROR, "test",statLong);

    //  Perform the request, res will get the return code
     // Check for errors
     //if(res != CURLE_OK){
    	// dlog_print(DLOG_ERROR, "test", "CurlFail", res);
      //elm_exit();
     //}
     // always cleanup
	   curl_easy_cleanup(curl);
	   curl_global_cleanup() ;
   }
 }



static void show_is_supported()
{
	char buf[30];
	bool is_supported = false;
	bool is_supported2 = false;
	sensor_is_supported(SENSOR_HRM, &is_supported2);
	sensor_is_supported(SENSOR_ACCELEROMETER, &is_supported);
	sprintf(buf, "HEART BEAT sensor is %s", is_supported2 ? "support" : "not support");
	dlog_print(DLOG_INFO, LOG_TAG, buf);
	sprintf(buf, "Acceleration Sensor is %s", is_supported ? "support" : "not support");
	dlog_print(DLOG_INFO, LOG_TAG, buf);

}
static void _new_sensor_value(sensor_h sensor, sensor_event_s *sensor_data, void*user_data)
{
	sensor_type_e type;
	sensor_get_type(sensor, &type);
	char buff[100];
	if(lastData.gyroX!=0 &&lastData.gyroY!=0&&lastData.gyroZ!=0&&lastData.heartR>0)
			send_data();
	switch (type) {
	    case SENSOR_ACCELEROMETER:
	    	lastData.gyroX=sensor_data->values[0];
	    	lastData.gyroY=sensor_data->values[1];
	    	lastData.gyroZ=sensor_data->values[2];
	    	sprintf(buff,"x= %f y=%f z=%f",sensor_data->values[0],sensor_data->values[1],sensor_data->values[2]);
	    	dlog_print(DLOG_INFO, LOG_TAG, buff);
	    	char buff[40];
	    	sprintf(buff,"x= %f y=%f hrm=%f",lastData.gyroY,lastData.gyroZ,lastData.heartR);
	    	dlog_print(DLOG_INFO,"test", buff);
	    	break;
	    case SENSOR_HRM:
	    	if(sensor_data->values[0]>0)
	    	{
	    		lastData.heartR=sensor_data->values[0];
	    		sprintf(buff,"heart beat = %f",sensor_data->values[0]);
	    		dlog_print(DLOG_INFO, LOG_TAG2, buff);
	    	}
	   	    break;
	    default:
	        dlog_print(DLOG_ERROR, LOG_TAG, "Not an HRM event");
	  }
}

static void
start_hrm_sensor_cb(void *ad)
{
	dlog_print(DLOG_INFO, LOG_TAG, "listen to my heartbeat");
	sensor_error_e err = SENSOR_ERROR_NONE;
	sensor_get_default_sensor(SENSOR_HRM, &sensor_info.sensor);
	err = sensor_create_listener(sensor_info.sensor, &sensor_info.sensor_listener);
	sensor_listener_set_event_cb(sensor_info.sensor_listener, 100, _new_sensor_value, ad);
	sensor_listener_start(sensor_info.sensor_listener);
}
static void
start_acceleration_sensor_cb(void *ad)
{
	sensor_error_e err = SENSOR_ERROR_NONE;
	sensor_get_default_sensor(SENSOR_ACCELEROMETER, &sensor_info.sensor);
	err = sensor_create_listener(sensor_info.sensor, &sensor_info.sensor_listener);
	sensor_listener_set_event_cb(sensor_info.sensor_listener,100, _new_sensor_value, ad);
	sensor_listener_start(sensor_info.sensor_listener);
}
bool service_app_create(void *data)
{
    // Todo: add your code here.
    return true;
}

void service_app_terminate(void *data)
{
    // Todo: add your code here.
    return;
}

void service_app_control(app_control_h app_control, void *data)
{
    // Todo: add your code here.
    return;
}

static void
service_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void
service_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
service_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
service_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
    char ad[50] = {0,};
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);
	show_is_supported(ad);
	start_hrm_sensor_cb(ad);
	start_acceleration_sensor_cb(ad);
	return service_app_main(argc, argv, &event_callback, ad);
}
