#include <Arduino.h>
#include <MsTimer2.h>
#include <XBee.h>
#include "define.h"

#define PIN_LED_1 20
#define PIN_LED_2 36
#define PIN_LED_3 37
#define PIN_LED_4 38
#define PIN_DATA_LED 53

XBee xbee = XBee();
ZBRxResponse rx = ZBRxResponse();
XBeeResponse response = XBeeResponse();
int count_10ms = 0; //0;
boolean flag_10ms = false;
boolean flag_20ms = false;
boolean flag_1s = false;
bool flag_100ms = false;
unsigned char count = 0;
bool counter_flag = false;
bool reset_flag = false;
int timer = 500; //5秒間
int sw_data, pre_sw_data;
int get_data0 = 0;
int get_data1 = 0;

void timer_warikomi()
{
    count_10ms++;
    if (count_10ms == 1)
    {
        flag_10ms = true;
        count_10ms = 0;
    }
    static int count_20ms = 0;
    count_20ms++;
    if (count_20ms == 2)
    {
        flag_20ms = !flag_20ms;
        count_20ms = 0;
    }
    static int count_1s = 0;
    count_1s++;
    if (count_1s == 100)
    {
        flag_1s = !flag_1s;
        count_1s = 0;
    }

    if (counter_flag)
    {
        timer--;
    }
    if (reset_flag)
    {
        timer = 100;
    }
    if (timer < 0)
    {

    }
            
    //xbee.readPacket();
}

void setup()
{
    pinMode(PIN_LED_1,OUTPUT);
    pinMode(PIN_LED_2,OUTPUT);
    Serial.begin(115200);
    SERIAL_XBEE.begin(115200);
    xbee.setSerial(SERIAL_XBEE);
    Serial.println("~~~recive~~~");
    MsTimer2::set(10, timer_warikomi); // 10ms period
    MsTimer2::start();
    counter_flag = true;

}

void loop()
{

    if (flag_10ms)
    {

       xbee.readPacket();

        if (xbee.getResponse().isAvailable())
        {
            digitalWrite(PIN_LED_1,HIGH);
            if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
            {
                xbee.getResponse().getZBRxResponse(rx);
                get_data0 = (int)rx.getData(0);
                get_data1 = (int)rx.getData(1);
                analogWrite(PIN_LED_RED,get_data1);
                Serial.println(get_data1);
            }
        }
        else
        {
            digitalWrite(PIN_LED_1,LOW);
        }
        if (flag_1s)
        { //生存確認のLチカ
            digitalWrite(PIN_LED_2,HIGH);
        }
        else
        {
            digitalWrite(PIN_LED_2,LOW);
        }
        flag_10ms = false;
    }
}