/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "iot_config.h"
#include "iot_main.h"
#include "iot_profile.h"
#include "iot_errno.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifi_connecter.h"
#include "iot_gpio_ex.h"
#include "iot_gpio.h"
#include "iot_watchdog.h"
#include "cjson_init.h"
#include "hi_stdlib.h"
#include "app_demo_iot.h"
#include "hi_gpio.h"
#include "hi_io.h"
#include "hi_adc.h"
#include "hi_errno.h"
#include "aht20.h"

#define STACK_SIZE   (1024)
#define DELAY_US     (100000)
/* attribute initiative to report */
#define TAKE_THE_INITIATIVE_TO_REPORT
/* oc request id */
#define CN_COMMAND_INDEX                    "commands/request_id="
/* oc report HiSpark attribute */
#define TRAFFIC_LIGHT_CMD_CONTROL_MODE      "ControlModule"
#define TRAFFIC_LIGHT_RED_ON_PAYLOAD        "RED_LED_ON"
#define TRAFFIC_LIGHT_RED_OFF_PAYLOAD        "RED_LED_OFF"

#define TASK_SLEEP_1000MS (1000)

#define GAS_SENSOR_CHAN_NAME 5
// #define GAS_SENSOR_PIN_NAME WIFI_IOT_IO_NAME_GPIO_11

#define AHT20_BAUDRATE (400 * 1000)
#define AHT20_I2C_IDX  0

#define ADC_RESOLUTION 2048
#define STACK_SIZE     4096
#define DELAY_500MS    500000
#define IDX_0          0
#define IDX_1          1
#define IDX_2          2
#define IDX_3          3
#define IDX_4          4
#define IDX_5          5
#define IDX_6          6
#define VOLTAGE_5V     (5.0)
#define EPS            (1e-7)

int g_lightStatus = -1;

char ch_value[];

float Temp_value = 36.05578f;
float Humi_value = 0.0f;
float Gas_value = 0.0f;
hi_u16 TDS_value = 0;

char bufTemp_value[16] = {0};
char bufHumi_value[16] = {0};
char bufGas_value[16] = {0};
char bufTDS_value[16] = {0};

void AdcGpioTask(void)
{
        if (hi_adc_read(HI_ADC_CHANNEL_4, &TDS_value,
            HI_ADC_EQU_MODEL_4, HI_ADC_CUR_BAIS_DEFAULT, 0) != HI_ERR_SUCCESS) {
            printf("ADC read error!\n");
        } else {
            printf("ADC_VALUE = %u\n", (unsigned int)TDS_value);
        }
}

void getTempHumi(void)
{
    AHT20_Calibrate();
    AHT20_StartMeasure();
    AHT20_GetMeasureResult(&Temp_value, &Humi_value);
}
float ConvertToVoltage(unsigned short data)
{
    return (float)data * 1.8 * 4 / 4096;
}
void getGas(void)
{
    unsigned short data = 0;
    int flag = -1;
    flag = hi_adc_read(GAS_SENSOR_CHAN_NAME, &data, HI_ADC_EQU_MODEL_4, HI_ADC_CUR_BAIS_DEFAULT, 0);
    if (flag == IOT_SUCCESS) {
    float Vx = ConvertToVoltage(data);
             // Vcc            ADC            GND
             //  |    ______   |     ______   |
             //  +---| MG-2 |---+---| 1kom |---+
             //       ------         ------
             // 查阅原理图，ADC 引脚位于 1K 电阻和燃气传感器之间，燃气传感器另一端接在 5V 电源正极上
             // 串联电路电压和阻止成正比：
             // Vx / 5 == 1kom / (1kom + Rx)
             //   => Rx + 1 == 5/Vx
             //   =>  Rx = 5/Vx - 1
        if (abs(Vx) > EPS) {
            Gas_value = VOLTAGE_5V / Vx - 1;
            printf("\r\n hi_adc_read ok, data=%hu, vx=%f, gasSensorResistance=%f", data, Vx, Gas_value);
            } 
    }
    else {
        printf("\r\n hi_adc_read fail");
    }
    
}
/* traffic light:1.control module */
void IotPublishSample_1(void)
{
    IoTProfileService service;
    IoTProfileKV property;
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "Temp_ControlModule";
    getTempHumi();
    sprintf(bufTemp_value, "%f", Temp_value);
    property.value = bufTemp_value;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "monitor";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
}

void IotPublishSample_2(void)
{
    IoTProfileService service;
    IoTProfileKV property;
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "Humid_ControlModule";
    getTempHumi();
    sprintf(bufHumi_value, "%f", Humi_value);
    property.value = bufHumi_value;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "monitor";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
}
void IotPublishSample_3(void)
{
    IoTProfileService service;
    IoTProfileKV property;
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "Gas_ControlModule";
    getGas();
    sprintf(bufGas_value, "%f", Gas_value);
    property.value = bufGas_value;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "monitor";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
}
void IotPublishSample_4(void)
{
    IoTProfileService service;
    IoTProfileKV property;
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "TDS_monitor";
    AdcGpioTask();
    sprintf(bufTDS_value, "%f", (unsigned int)TDS_value);
    property.value = bufTDS_value;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "monitor";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
}
// /< this is the demo main task entry,here we will set the wifi/cjson/mqtt ready ,and
// /< wait if any work to do in the while
static void DemoEntry(void)
{
    ConnectToHotspot();
    CJsonInit();
    IoTMain();
    hi_gpio_init();
    hi_io_set_func(HI_GPIO_IDX_9, HI_IO_FUNC_GPIO_9_GPIO);
    hi_gpio_set_dir(HI_GPIO_IDX_9, HI_GPIO_DIR_IN);
    OledInit();
    OledFillScreen(0);
    IoTI2cInit(AHT20_I2C_IDX, AHT20_BAUDRATE);

    // IoTSetMsgCallback(DemoMsgRcvCallBack);
    TaskMsleep(30000); // 30000 = 3s连接华为云
/* 主动上报 */
    while (1) {
        // here you could add your own works here--we report the data to the IoTplatform
        TaskMsleep(TASK_SLEEP_1000MS);
        // know we report the data to the iot platform
        IotPublishSample_1();
        IotPublishSample_2();
        IotPublishSample_3();
        IotPublishSample_4();
    }
}

// This is the demo entry, we create a task here, and all the works has been done in the demo_entry
#define CN_IOT_TASK_STACKSIZE  0x1000
#define CN_IOT_TASK_PRIOR 28
#define CN_IOT_TASK_NAME "IOTDEMO"
static void AppDemoIot(void)
{
    osThreadAttr_t attr;
    IoTWatchDogDisable();
    attr.name = "IOTDEMO";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = CN_IOT_TASK_STACKSIZE;
    attr.priority = CN_IOT_TASK_PRIOR;

    if (osThreadNew((osThreadFunc_t)DemoEntry, NULL, &attr) == NULL) {
        printf("[TrafficLight] Failed to create IOTDEMO!\n");
    }
}

SYS_RUN(AppDemoIot);