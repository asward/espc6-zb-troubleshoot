/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 * Zigbee HA_on_off_switch Example
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */

#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ha/esp_zigbee_ha_standard.h"
#include "esp_zigbee_core.h"
#include "zigbee_driver.h"
#include "zboss_api.h"
#include "esp_sleep.h"
#include "esp_timer.h"


#if !defined ZB_ED_ROLE
#error Define ZB_ED_ROLE in idf.py menuconfig.
#endif
static const char *TAG = "ESP_ZB";

esp_err_t action_callback_handler(esp_zb_core_action_callback_id_t callback_id, const void *message)
{
    ESP_LOGD(TAG,"CALLBACK 0x%04X", callback_id );

    switch (callback_id) {
        case ESP_ZB_CORE_SET_ATTR_VALUE_CB_ID:
            // Handle Set attribute value
            break;
            
        case ESP_ZB_CORE_SCENES_STORE_SCENE_CB_ID:
            // Handle Store scene
            break;
            
        case ESP_ZB_CORE_SCENES_RECALL_SCENE_CB_ID:
            // Handle Recall scene
            break;
            
        case ESP_ZB_CORE_IAS_ZONE_ENROLL_RESPONSE_VALUE_CB_ID:
            // Handle IAS Zone enroll response
            break;
            
        case ESP_ZB_CORE_OTA_UPGRADE_VALUE_CB_ID:
            // Handle OTA Upgrade
            break;
            
        case ESP_ZB_CORE_OTA_UPGRADE_SRV_STATUS_CB_ID:
            // Handle OTA Server status
            break;
            
        case ESP_ZB_CORE_OTA_UPGRADE_SRV_QUERY_IMAGE_CB_ID:
            // Handle OTA Server query image
            break;
            
        case ESP_ZB_CORE_THERMOSTAT_VALUE_CB_ID:
            // Handle Thermostat value
            break;
            
        case ESP_ZB_CORE_METERING_GET_PROFILE_CB_ID:
            // Handle Metering get profile
            break;
            
        case ESP_ZB_CORE_METERING_GET_PROFILE_RESP_CB_ID:
            // Handle Metering get profile response
            break;
            
        case ESP_ZB_CORE_METERING_REQ_FAST_POLL_MODE_CB_ID:
            // Handle Metering request fast poll mode
            break;
            
        case ESP_ZB_CORE_METERING_REQ_FAST_POLL_MODE_RESP_CB_ID:
            // Handle Metering request fast poll mode response
            break;
            
        case ESP_ZB_CORE_METERING_GET_SNAPSHOT_CB_ID:
            // Handle Metering get snapshot
            break;
            
        case ESP_ZB_CORE_METERING_PUBLISH_SNAPSHOT_CB_ID:
            // Handle Metering publish snapshot
            break;
            
        case ESP_ZB_CORE_METERING_GET_SAMPLED_DATA_CB_ID:
            // Handle Metering get sampled data
            break;
            
        case ESP_ZB_CORE_METERING_GET_SAMPLED_DATA_RESP_CB_ID:
            // Handle Metering get sampled data response
            break;
            
        case ESP_ZB_CORE_DOOR_LOCK_LOCK_DOOR_CB_ID:
            // Handle Lock/unlock door request
            break;
            
        case ESP_ZB_CORE_DOOR_LOCK_LOCK_DOOR_RESP_CB_ID:
            // Handle Lock/unlock door response
            break;
            
        case ESP_ZB_CORE_IDENTIFY_EFFECT_CB_ID:
            // Handle Identify effect request
            break;
            
        case ESP_ZB_CORE_BASIC_RESET_TO_FACTORY_RESET_CB_ID:
            // Handle Reset to factory default
            break;
            
        case ESP_ZB_CORE_PRICE_GET_CURRENT_PRICE_CB_ID:
            // Handle Get current price
            break;
            
        case ESP_ZB_CORE_PRICE_GET_SCHEDULED_PRICES_CB_ID:
            // Handle Get scheduled prices
            break;
            
        case ESP_ZB_CORE_PRICE_GET_TIER_LABELS_CB_ID:
            // Handle Get tier labels
            break;
            
        case ESP_ZB_CORE_PRICE_PUBLISH_PRICE_CB_ID:
            // Handle Publish price
            break;
            
        case ESP_ZB_CORE_PRICE_PUBLISH_TIER_LABELS_CB_ID:
            // Handle Publish tier labels
            break;
            
        case ESP_ZB_CORE_PRICE_PRICE_ACK_CB_ID:
            // Handle Price acknowledgment
            break;
            
        case ESP_ZB_CORE_COMM_RESTART_DEVICE_CB_ID:
            // Handle Commissioning restart device
            break;
            
        case ESP_ZB_CORE_COMM_OPERATE_STARTUP_PARAMS_CB_ID:
            // Handle Operate startup parameters
            break;
            
        case ESP_ZB_CORE_COMM_COMMAND_RESP_CB_ID:
            // Handle Commissioning command response
            break;
            
        case ESP_ZB_CORE_IAS_WD_START_WARNING_CB_ID:
            // Handle IAS WD Start Warning command
            break;
            
        case ESP_ZB_CORE_IAS_WD_SQUAWK_CB_ID:
            // Handle IAS WD Squawk command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_ARM_CB_ID:
            // Handle IAS ACE Arm command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_BYPASS_CB_ID:
            // Handle IAS ACE Bypass command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_EMERGENCY_CB_ID:
            // Handle IAS ACE Emergency command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_FIRE_CB_ID:
            // Handle IAS ACE Fire command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_PANIC_CB_ID:
            // Handle IAS ACE Panic command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_GET_PANEL_STATUS_CB_ID:
            // Handle Get Panel Status command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_GET_BYPASSED_ZONE_LIST_CB_ID:
            // Handle Get Bypass Zone List command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_GET_ZONE_STATUS_CB_ID:
            // Handle Get Zone Status command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_ARM_RESP_CB_ID:
            // Handle IAS ACE Arm response
            break;
            
        case ESP_ZB_CORE_IAS_ACE_GET_ZONE_ID_MAP_RESP_CB_ID:
            // Handle Get Zone ID Map response
            break;
            
        case ESP_ZB_CORE_IAS_ACE_GET_ZONE_INFO_RESP_CB_ID:
            // Handle Get Zone Information response
            break;
            
        case ESP_ZB_CORE_IAS_ACE_ZONE_STATUS_CHANGED_CB_ID:
            // Handle IAS ACE Zone Status Changed command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_PANEL_STATUS_CHANGED_CB_ID:
            // Handle IAS ACE Panel Status Changed command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_GET_PANEL_STATUS_RESP_CB_ID:
            // Handle Get Panel Status response
            break;
            
        case ESP_ZB_CORE_IAS_ACE_SET_BYPASSED_ZONE_LIST_CB_ID:
            // Handle Set Bypassed Zone List command
            break;
            
        case ESP_ZB_CORE_IAS_ACE_BYPASS_RESP_CB_ID:
            // Handle Bypass command response
            break;
            
        case ESP_ZB_CORE_IAS_ACE_GET_ZONE_STATUS_RESP_CB_ID:
            // Handle Get Zone Status response
            break;
            
        case ESP_ZB_CORE_WINDOW_COVERING_MOVEMENT_CB_ID:
            // Handle Window covering movement command
            break;
            
        case ESP_ZB_CORE_OTA_UPGRADE_QUERY_IMAGE_RESP_CB_ID:
            // Handle OTA upgrade query image response
            break;
            
        case ESP_ZB_CORE_CMD_READ_ATTR_RESP_CB_ID:
            ESP_LOGD(TAG,"ESP_ZB_CORE_CMD_READ_ATTR_RESP_CB_ID");
            // Handle Read attribute response
            break;
            
        case ESP_ZB_CORE_CMD_WRITE_ATTR_RESP_CB_ID:
            ESP_LOGD(TAG,"ESP_ZB_CORE_CMD_WRITE_ATTR_RESP_CB_ID");
            // Handle Write attribute response
            break;
            
        case ESP_ZB_CORE_CMD_REPORT_CONFIG_RESP_CB_ID:
            ESP_LOGD(TAG,"ESP_ZB_CORE_CMD_REPORT_CONFIG_RESP_CB_ID");
            // Handle Configure report response
            break;
            
        case ESP_ZB_CORE_CMD_READ_REPORT_CFG_RESP_CB_ID:
            ESP_LOGD(TAG,"ESP_ZB_CORE_CMD_READ_REPORT_CFG_RESP_CB_ID");
            // Handle Read report configuration response
            break;
            
        case ESP_ZB_CORE_CMD_DISC_ATTR_RESP_CB_ID:
            // Handle Discover attributes response
            break;
            
        case ESP_ZB_CORE_CMD_DEFAULT_RESP_CB_ID:
            // Handle Default response
            break;
            
        case ESP_ZB_CORE_CMD_OPERATE_GROUP_RESP_CB_ID:
            // Handle Group operation response
            break;
            
        case ESP_ZB_CORE_CMD_VIEW_GROUP_RESP_CB_ID:
            // Handle Group view response
            break;
            
        case ESP_ZB_CORE_CMD_GET_GROUP_MEMBERSHIP_RESP_CB_ID:
            // Handle Get Group Membership response
            break;
            
        case ESP_ZB_CORE_CMD_OPERATE_SCENE_RESP_CB_ID:
            // Handle Scene operation response
            break;
            
        case ESP_ZB_CORE_CMD_VIEW_SCENE_RESP_CB_ID:
            // Handle Scene view response
            break;
            
        case ESP_ZB_CORE_CMD_GET_SCENE_MEMBERSHIP_RESP_CB_ID:
            // Handle Scene Membership response
            break;
            
        case ESP_ZB_CORE_CMD_IAS_ZONE_ZONE_ENROLL_REQUEST_ID:
            // Handle IAS Zone enroll request
            break;
            
        case ESP_ZB_CORE_CMD_IAS_ZONE_ZONE_STATUS_CHANGE_NOT_ID:
            // Handle IAS Zone status change notification
            break;
            
        case ESP_ZB_CORE_CMD_CUSTOM_CLUSTER_REQ_CB_ID:
            // Handle Custom Cluster request
            break;
            
        case ESP_ZB_CORE_CMD_CUSTOM_CLUSTER_RESP_CB_ID:
            // Handle Custom Cluster response
            break;
            
        case ESP_ZB_CORE_CMD_PRIVILEGE_COMMAND_REQ_CB_ID:
            // Handle Privilege command request
            break;
            
        case ESP_ZB_CORE_CMD_PRIVILEGE_COMMAND_RESP_CB_ID:
            // Handle Privilege command response
            break;
            
        case ESP_ZB_CORE_CMD_TOUCHLINK_GET_GROUP_ID_RESP_CB_ID:
            // Handle Touchlink get group ID response
            break;
            
        case ESP_ZB_CORE_CMD_TOUCHLINK_GET_ENDPOINT_LIST_RESP_CB_ID:
            // Handle Touchlink get endpoint list response
            break;
            
        case ESP_ZB_CORE_CMD_GREEN_POWER_RECV_CB_ID:
            // Handle Green power command receiving
            break;
            
        case ESP_ZB_CORE_REPORT_ATTR_CB_ID:
            // Handle Attribute report
            ESP_LOGD(TAG,"ESP_ZB_CORE_REPORT_ATTR_CB_ID");
            break;

        default:
            // Handle unknown callback
            break;
    }
    return ESP_OK ;
}

static void bdb_start_top_level_commissioning_cb(uint8_t mode_mask)
{
    ESP_ERROR_CHECK(esp_zb_bdb_start_top_level_commissioning(mode_mask));
}

void esp_zb_app_signal_handler(esp_zb_app_signal_t *signal_struct)
{
    uint32_t *p_sg_p       = signal_struct->p_app_signal;
    esp_err_t err_status = signal_struct->esp_err_status;
    esp_zb_app_signal_type_t sig_type = *p_sg_p;
    ESP_LOGD(TAG,"CALLBACK 0x%02X", sig_type );

      switch (sig_type) {
        case ESP_ZB_ZDO_SIGNAL_DEFAULT_START:
            // Handle default start signal
            break;

        case ESP_ZB_ZDO_SIGNAL_SKIP_STARTUP:
            // Handle skip startup signal
            ESP_LOGI(TAG, "Initialize Zigbee stack");
            esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_INITIALIZATION);
            break;
            break;

        case ESP_ZB_ZDO_SIGNAL_DEVICE_ANNCE:
            // Handle device announce signal
            break;

        case ESP_ZB_ZDO_SIGNAL_LEAVE:
            // Handle leave signal
            break;

        case ESP_ZB_ZDO_SIGNAL_ERROR:
            // Handle error signal
            break;

        case ESP_ZB_BDB_SIGNAL_DEVICE_FIRST_START:
            // Handle first device start signal
        case ESP_ZB_BDB_SIGNAL_DEVICE_REBOOT:
            // Handle device reboot signal
            if (err_status == ESP_OK) {
                // ESP_LOGI(TAG, "Deferred driver initialization %s", deferred_driver_init() ? "failed" : "successful");
                ESP_LOGI(TAG, "Device started up in %s factory-reset mode", esp_zb_bdb_is_factory_new() ? "" : "non");
                if (esp_zb_bdb_is_factory_new()) {
                    ESP_LOGI(TAG, "Start network steering");
                    esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_NETWORK_STEERING);
                } else {
                    ESP_LOGI(TAG, "Device rebooted");
                }
            } else {
                ESP_LOGE(TAG, "Failed to initialize Zigbee stack (status: %s)", esp_err_to_name(err_status));
                esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_NETWORK_STEERING);
            }
            break;

        case ESP_ZB_BDB_SIGNAL_TOUCHLINK_NWK_STARTED:
            // Handle Touchlink network started signal
            break;

        case ESP_ZB_BDB_SIGNAL_TOUCHLINK_NWK_JOINED_ROUTER:
            // Handle Touchlink network joined router signal
            break;

        case ESP_ZB_BDB_SIGNAL_TOUCHLINK:
            // Handle Touchlink signal
            break;

        case ESP_ZB_BDB_SIGNAL_STEERING:
            // Handle steering signal
                    if (err_status == ESP_OK) {
            esp_zb_ieee_addr_t extended_pan_id;
            esp_zb_get_extended_pan_id(extended_pan_id);
            ESP_LOGI(TAG,
                     "Joined network successfully (Extended PAN ID: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x, PAN ID: 0x%04hx, Channel:%d, Short "
                     "Address: 0x%04hx)",
                     extended_pan_id[7], extended_pan_id[6], extended_pan_id[5], extended_pan_id[4], extended_pan_id[3], extended_pan_id[2],
                     extended_pan_id[1], extended_pan_id[0], esp_zb_get_pan_id(), esp_zb_get_current_channel(), esp_zb_get_short_address());
            } else {
                ESP_LOGI(TAG, "Network steering was not successful (status: %s)", esp_err_to_name(err_status));
                esp_zb_scheduler_alarm((esp_zb_callback_t)bdb_start_top_level_commissioning_cb, ESP_ZB_BDB_MODE_NETWORK_STEERING, 5000);
            }
            break;
    
        case ESP_ZB_BDB_SIGNAL_FORMATION:
            // Handle formation signal
            break;

        case ESP_ZB_BDB_SIGNAL_FINDING_AND_BINDING_TARGET_FINISHED:
            // Handle finding and binding target finished signal
            break;

        case ESP_ZB_BDB_SIGNAL_FINDING_AND_BINDING_INITIATOR_FINISHED:
            // Handle finding and binding initiator finished signal
            break;

        case ESP_ZB_BDB_SIGNAL_TOUCHLINK_TARGET:
            // Handle Touchlink target signal
            break;

        case ESP_ZB_BDB_SIGNAL_TOUCHLINK_NWK:
            // Handle Touchlink network signal
            break;

        case ESP_ZB_BDB_SIGNAL_TOUCHLINK_TARGET_FINISHED:
            // Handle Touchlink target finished signal
            break;

        case ESP_ZB_BDB_SIGNAL_TOUCHLINK_ADD_DEVICE_TO_NWK:
            // Handle Touchlink add device to network signal
            break;

        case ESP_ZB_NWK_SIGNAL_DEVICE_ASSOCIATED:
            // Handle device associated signal
            break;

        case ESP_ZB_ZDO_SIGNAL_LEAVE_INDICATION:
            // Handle leave indication signal
            break;

        case ESP_ZB_BDB_SIGNAL_WWAH_REJOIN_STARTED:
            // Handle WWAH rejoin started signal
            break;

        case ESP_ZB_ZGP_SIGNAL_COMMISSIONING:
            // Handle ZGP commissioning signal
            break;

        case ESP_ZB_COMMON_SIGNAL_CAN_SLEEP:
            //Notifies the application that the MCU can sleep for the specified period of time.
            // ESP_LOGI(TAG, "Zigbee can sleep");
            #ifdef CONFIG_LOG_MASTER_LEVEL
            esp_log_set_level_master(ESP_LOG_NONE) ;
            #endif
            int64_t t_before_us = esp_timer_get_time();
            printf("%s Sleeping\n",TAG);
            esp_zb_sleep_now();
            printf("%s Waking\n",TAG);
            int64_t t_after_us = esp_timer_get_time();
            const char* wakeup_reason;
            esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
            switch (cause) {
                case ESP_SLEEP_WAKEUP_TIMER:
                    wakeup_reason = "timer";
                    break;
                case ESP_SLEEP_WAKEUP_GPIO:
                    wakeup_reason = "pin";
                    break;
                case ESP_SLEEP_WAKEUP_UART:
                    wakeup_reason = "uart";
                    /* Hang-up for a while to switch and execuse the uart task
                    * Otherwise the chip may fall sleep again before running uart task */
                    vTaskDelay(1);
                    break;
                default:
                    wakeup_reason = "other";
                    break;
            }
            printf("Returned from light sleep, reason: %s (%d), t=%lld ms, slept for %lld ms\n",
                    wakeup_reason, cause, t_after_us / 1000, (t_after_us - t_before_us) / 1000);
            #ifdef CONFIG_LOG_MASTER_LEVEL
            esp_log_set_level_master(ESP_LOG_DEBUG) ;
            #endif
            break;
        case ESP_ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY:
            // Handle production config ready signal
            break;

        case ESP_ZB_NWK_SIGNAL_NO_ACTIVE_LINKS_LEFT:
            // Handle no active links left signal
            break;

        case ESP_ZB_SE_SIGNAL_SKIP_JOIN:
            // Handle skip join signal
            break;

        case ESP_ZB_SE_SIGNAL_REJOIN:
            // Handle rejoin signal
            break;

        case ESP_ZB_SE_SIGNAL_CHILD_REJOIN:
            // Handle child rejoin signal
            break;

        case ESP_ZB_SE_TC_SIGNAL_CHILD_JOIN_CBKE:
            // Handle child join CBKE signal
            break;

        case ESP_ZB_SE_TC_SIGNAL_CHILD_JOIN_NON_CBKE:
            // Handle child join non-CBKE signal
            break;

        case ESP_ZB_SE_SIGNAL_CBKE_FAILED:
            // Handle CBKE failed signal
            break;

        case ESP_ZB_SE_SIGNAL_CBKE_OK:
            // Handle CBKE OK signal
            break;

        case ESP_ZB_SE_SIGNAL_SERVICE_DISCOVERY_START:
            // Handle service discovery start signal
            break;

        case ESP_ZB_SE_SIGNAL_SERVICE_DISCOVERY_DO_BIND:
            // Handle service discovery do bind signal
            break;

        case ESP_ZB_SE_SIGNAL_SERVICE_DISCOVERY_BIND_OK:
            // Handle service discovery bind OK signal
            break;

        case ESP_ZB_SE_SIGNAL_SERVICE_DISCOVERY_BIND_FAILED:
            // Handle service discovery bind failed signal
            break;

        case ESP_ZB_SE_SIGNAL_SERVICE_DISCOVERY_BIND_INDICATION:
            // Handle service discovery bind indication signal
            break;

        case ESP_ZB_SE_SIGNAL_SERVICE_DISCOVERY_OK:
            // Handle service discovery OK signal
            break;

        case ESP_ZB_SE_SIGNAL_SERVICE_DISCOVERY_FAILED:
            // Handle service discovery failed signal
            break;

        case ESP_ZB_SE_SIGNAL_APS_KEY_READY:
            // Handle APS key ready signal
            break;

        case ESP_ZB_SE_SIGNAL_APS_KEY_FAIL:
            // Handle APS key fail signal
            break;

        case ESP_ZB_SIGNAL_SUBGHZ_SUSPEND:
            // Handle SubGHz suspend signal
            break;

        case ESP_ZB_SIGNAL_SUBGHZ_RESUME:
            // Handle SubGHz resume signal
            break;

        case ESP_ZB_MACSPLIT_DEVICE_BOOT:
            // Handle MAC split device boot signal
            break;

        case ESP_ZB_MACSPLIT_DEVICE_READY_FOR_UPGRADE:
            // Handle MAC split device ready for upgrade signal
            break;

        case ESP_ZB_MACSPLIT_DEVICE_FW_UPGRADE_EVENT:
            // Handle MAC split device firmware upgrade event signal
            break;

        case ESP_ZB_SIGNAL_NWK_INIT_DONE:
            // Handle network init done signal
            break;

        case ESP_ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED:
            // Handle device authorized signal
            break;

        case ESP_ZB_ZDO_SIGNAL_DEVICE_UPDATE:
            // Handle device update signal
            break;

        case ESP_ZB_NWK_SIGNAL_PANID_CONFLICT_DETECTED:
            // Handle PAN ID conflict detected signal
            break;

        case ESP_ZB_NLME_STATUS_INDICATION:
            // Handle NLME status indication signal
            break;

        case ESP_ZB_TCSWAP_DB_BACKUP_REQUIRED_SIGNAL:
            // Handle TC swap database backup required signal
            break;

        case ESP_ZB_TC_SWAPPED_SIGNAL:
            // Handle TC swapped signal
            break;

        case ESP_ZB_BDB_SIGNAL_TC_REJOIN_DONE:
            // Handle TC rejoin done signal
            break;

        case ESP_ZB_NWK_SIGNAL_PERMIT_JOIN_STATUS:
            // Handle permit join status signal
            break;

        case ESP_ZB_BDB_SIGNAL_STEERING_CANCELLED:
            // Handle steering cancelled signal
            break;

        case ESP_ZB_BDB_SIGNAL_FORMATION_CANCELLED:
            // Handle formation cancelled signal
            break;

        case ESP_ZB_SIGNAL_READY_TO_SHUT:
            // Handle ready to shut signal
            break;

        case ESP_ZB_SIGNAL_INTERPAN_PREINIT:
            // Handle InterPAN pre-init signal
            break;

        case ESP_ZB_ZGP_SIGNAL_MODE_CHANGE:
            // Handle ZGP mode change signal
            break;

        case ESP_ZB_ZDO_DEVICE_UNAVAILABLE:
            // Handle device unavailable signal
            break;

        case ESP_ZB_ZGP_SIGNAL_APPROVE_COMMISSIONING:
            // Handle approve commissioning signal
            break;

        case ESP_ZB_SIGNAL_END:
            // Handle end signal
            break;

        default:
            // Handle unknown or unhandled signal
            ESP_LOGI(TAG, "ZDO signal: %s (0x%x), status: %s", esp_zb_zdo_signal_to_string(sig_type), sig_type, esp_err_to_name(err_status));
            break;
    }
}

bool raw_command_handler(uint8_t bufid){
    // ESP_LOGD(TAG, "RAW COMMAND");
    uint8_t buf[zb_buf_len(bufid)];
    zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(bufid, zb_zcl_parsed_hdr_t);
    printf("cluster id: 0x%x, command id: %d\n", cmd_info->cluster_id, cmd_info->cmd_id);
    memcpy(buf, zb_buf_begin(bufid), sizeof(buf));
    ESP_LOGI("RAW", "bufid: %d size: %d", bufid, sizeof(buf));
    for (int i = 0; i < sizeof(buf); ++i) {
        printf("0x%02X ", buf[i]);
    }
    printf("\n");

    zb_zcl_send_default_handler(bufid,cmd_info, ZB_ZCL_STATUS_SUCCESS);
    return true;
}
void zcl_command_status_handler(esp_zb_zcl_command_send_status_message_t message){
    ESP_LOGD(TAG, "STATUS");

}

void endpoint_task(void *pvParameters)
{
    /* initialize Zigbee stack */
    ESP_LOGI(TAG, "Endpoint Task Started");

    esp_zb_core_action_handler_register(action_callback_handler) ;
    // esp_zb_device_cb_id_handler_register() ;
    esp_zb_raw_command_handler_register(raw_command_handler) ;
    // esp_zb_cli_resp_handler_register() ;
    esp_zb_zcl_command_send_status_handler_register(zcl_command_status_handler) ;
    // esp_zb_identify_notify_handler_register () ;

    // START/LOOP
    ESP_ERROR_CHECK(esp_zb_start(false));
    esp_zb_stack_main_loop();
}