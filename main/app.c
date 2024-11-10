#include "string.h"

#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_sleep.h"

#ifdef CONFIG_PM_ENABLE
#include "esp_pm.h"
#include "esp_private/esp_clk.h"
#include "esp_sleep.h"
#endif

#include "ha/esp_zigbee_ha_standard.h"
#include "esp_zigbee_core.h"
#include "esp_zigbee_cluster.h"
#include "esp_zigbee_endpoint.h"


#include "app.h"
#include "zigbee_driver.h"
#include "zboss_api.h"

static const char *TAG = "APP";
TaskHandle_t xZigbeeTask = NULL;

void set_binary_input_state(uint8_t *state){

    static bool dumbbool = true ;
    dumbbool = !dumbbool;

    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_status_t status = esp_zb_zcl_set_attribute_val(
        HA_GARAGE_DOOR_BINARY_INPUT_EP_ID,
        ZB_ZCL_CLUSTER_ID_BINARY_INPUT,
        ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        ESP_ZB_ZCL_ATTR_BINARY_INPUT_PRESENT_VALUE_ID,
        &dumbbool,
        true);
    esp_zb_lock_release() ;

    ESP_LOGD(TAG, "Set Attr Status: 0x%02X", status);
}


void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
        ESP_LOGI(TAG, "Starting ZigBee");


    // PLATFORM
    esp_zb_platform_config_t config = {
        .radio_config = {
            .radio_mode = ZB_RADIO_MODE_NATIVE
        },
        .host_config = {
            .host_connection_mode = ZB_HOST_CONNECTION_MODE_NONE
        }
    };
    
    esp_err_t rc = ESP_OK;
#ifdef CONFIG_PM_ENABLE
    int cur_cpu_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ;
    esp_pm_config_t pm_config = {
        .max_freq_mhz = cur_cpu_freq_mhz,
        .min_freq_mhz = cur_cpu_freq_mhz,
#if CONFIG_FREERTOS_USE_TICKLESS_IDLE
        .light_sleep_enable = true
#endif
    };
    rc = esp_pm_configure(&pm_config);
#endif

    ESP_ERROR_CHECK(esp_zb_platform_config(&config));
    // ESP_ERROR_CHECK(esp_zb_sleep_set_threshold(1000));

    // zb_zdo_pim_set_long_poll_interval(7500);

    esp_zb_sleep_enable(true);

    // DEVICE
    esp_zb_cfg_t zb_device_cfg = ESP_ZB_ZED_CONFIG();
    esp_zb_init(&zb_device_cfg);

    esp_zb_set_primary_network_channel_set(ESP_ZB_PRIMARY_CHANNEL_MASK);

    gpio_wakeup_enable(GPIO_NUM_2,GPIO_INTR_LOW_LEVEL );
    gpio_sleep_set_pull_mode(GPIO_NUM_2, GPIO_PULLUP_ENABLE);
    gpio_sleep_set_direction(GPIO_NUM_2, GPIO_MODE_INPUT);
    ESP_ERROR_CHECK(esp_sleep_enable_gpio_wakeup());

    // ZIGBEE BINARY INPUT
    ESP_LOGI(TAG, "Registering") ;

    // BASIC

    esp_zb_basic_cluster_cfg_t basic_cfg = {
        .power_source = ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE,
        .zcl_version = 4
    };
    esp_zb_attribute_list_t *basic_attr_list = esp_zb_basic_cluster_create(&basic_cfg) ;
    // esp_zb_basic_cluster_add_attr(basic_attr_list, ESP_ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, (void *)&ManufacturerName);
    // esp_zb_basic_cluster_add_attr(basic_attr_list, ESP_ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, (void *)&ModelIdentifier);
    // esp_zb_basic_cluster_add_attr(basic_attr_list, ESP_ZB_ZCL_ATTR_BASIC_DATE_CODE_ID, (void *)&DateCode);

    // IDENTITY
    esp_zb_identify_cluster_cfg_t identity_cfg  = {
        .identify_time = 4
    } ;
    esp_zb_attribute_list_t *identity_attr_list = esp_zb_identify_cluster_create(&identity_cfg) ;

    //ON OFF
    esp_zb_binary_input_cluster_cfg_t binary_cfg = {
        .out_of_service = true,
        .status_flags = 0x00
    } ;
    zb_bool_t init_state = false ;

    esp_zb_attribute_list_t *door_binary_input_attr_list = esp_zb_binary_input_cluster_create(&binary_cfg);
    esp_zb_cluster_add_attr(door_binary_input_attr_list,
            ZB_ZCL_CLUSTER_ID_BINARY_INPUT,
            ZB_ZCL_ATTR_BINARY_INPUT_PRESENT_VALUE_ID,
            ESP_ZB_ZCL_ATTR_TYPE_BOOL,
            ESP_ZB_ZCL_ATTR_ACCESS_REPORTING | ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE,
            &init_state);

    // CLUSTER
    esp_zb_cluster_list_t *door_binary_input_cluster_list = esp_zb_zcl_cluster_list_create();
    ESP_ERROR_CHECK(esp_zb_cluster_list_add_basic_cluster(door_binary_input_cluster_list, basic_attr_list, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE ));
    ESP_ERROR_CHECK(esp_zb_cluster_list_add_identify_cluster(door_binary_input_cluster_list, identity_attr_list, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE )) ;
    ESP_ERROR_CHECK(esp_zb_cluster_list_add_identify_cluster(door_binary_input_cluster_list, esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_IDENTIFY), ESP_ZB_ZCL_CLUSTER_CLIENT_ROLE)) ;
    ESP_ERROR_CHECK(esp_zb_cluster_list_add_binary_input_cluster(door_binary_input_cluster_list, door_binary_input_attr_list, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE )) ;

    // EP
    esp_zb_ep_list_t *door_lock_ep_list = esp_zb_ep_list_create() ;
    esp_zb_endpoint_config_t endpoint_cfg = {
        .app_device_id = ESP_ZB_HA_ON_OFF_OUTPUT_DEVICE_ID,
        .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID,
        .endpoint = HA_GARAGE_DOOR_BINARY_INPUT_EP_ID
    };
    esp_zb_ep_list_add_ep(door_lock_ep_list,door_binary_input_cluster_list,endpoint_cfg) ;
    
    // REG
    esp_zb_device_register(door_lock_ep_list) ;

    esp_zb_zcl_attr_location_info_t filter;
    esp_zb_zcl_reporting_info_t *report;

    filter.endpoint_id=HA_GARAGE_DOOR_BINARY_INPUT_EP_ID;
    filter.cluster_id=ZB_ZCL_CLUSTER_ID_BINARY_INPUT ;
    filter.attr_id=ESP_ZB_ZCL_ATTR_BINARY_INPUT_PRESENT_VALUE_ID;
    filter.cluster_role=ESP_ZB_ZCL_CLUSTER_SERVER_ROLE;
    filter.manuf_code=ZB_ZCL_NON_MANUFACTURER_SPECIFIC;
    report=esp_zb_zcl_find_reporting_info(filter);
    if(report!=NULL){
        report->u.send_info.def_max_interval=0x00;
        report->u.send_info.def_min_interval=0x00;
        report->u.send_info.max_interval=0x00;
        report->u.send_info.min_interval=0x00;
        report->u.send_info.delta.u8=1;
        if(esp_zb_zcl_update_reporting_info(report)!=ESP_OK){
            ESP_LOGE(TAG, "error conf update");
        } else {
            ESP_LOGI(TAG, "Reporting Conf Updated");
        }
    }else ESP_LOGE(TAG, "error filter");

    // TASK
    BaseType_t xReturned;
    xReturned = xTaskCreate(endpoint_task, "endpoint_task",4096, NULL, 5, &xZigbeeTask);

    if( xReturned == pdPASS )
    {
        ESP_LOGI(TAG, "Task Completed");
    }  else{
        ESP_LOGI(TAG, "Task Failed");
    }
}
