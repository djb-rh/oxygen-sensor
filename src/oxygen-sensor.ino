/*
 * Project oxygen-sensor
 * Description: Simple o2 sensor application using an LCD and some buttons
 * Author: Donnie Barnes <djb@donniebarnes.com>
 * Date: May 19, 2020
 *
 * License
 * -------
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <clickButton.h>
#include <LiquidCrystal_I2C_Spark.h>

Adafruit_ADS1015 ads; // 12 bit ADC object

// wiring
int calibrateButton = D2;

double calibrationv; // calibrated value
int sensorcheck=0; // check sensor health. if 0 it works, if 1 it isn't working
int Sensor_lowrange=58; // working sensor reads 58 on low
int Sensor_highrange=106; // working sensor reads 106 on high
int localAlt=600; // local calibration factor

ClickButton buttonCalibrate(calibrateButton, LOW, CLICKBTN_PULLUP);
int buttonCalibrateClicks = 0;

int calibrateButtonClicks = 0;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

char publishString[40];

void setup() {

    Serial.begin(115200);
    pinMode(calibrateButton, INPUT_PULLUP);

    buttonCalibrate.debounceTime = 20;
    buttonCalibrate.multiclickTime = 250;
    buttonCalibrate.longClickTime = 1000;

    lcd.init();
    lcd.backlight();

    // start up the 12 bit ADC
    ads.setGain(GAIN_SIXTEEN); // 16x gain
    ads.begin();
    
    calibrationv = calibrate(); // calibrate at startup, or don't! we don't know what altitude this thing is at!

    if ((calibrationv > Sensor_highrange) || (calibrationv < Sensor_lowrange))
        {
            sensorcheck = 1;
            need_calibrating();
        }

}

void need_calibrating()
{
    lcd.setCursor(0,0);
    lcd.print("Sensor out of range");
}

// take 20 readings and average
int calibrate()
{
    int16_t adc0=0;
    int result;
    for(int i=0; i<=19; i++) adc0=adc0+ads.readADC_SingleEnded(0);
    result=adc0/20;
    return result;
}

void loop() {

    double modr;
    int16_t adc0=0;
    double result;
    double currentmv;
    double altitude;

    buttonCalibrate.Update();

    if(buttonCalibrate.clicks != 0) buttonCalibrateClicks = buttonCalibrate.clicks;

    if (buttonCalibrateClicks == 1)
    {
        Serial.println("button pressed");
        calibrationv=calibrate();
        if ((calibrationv > Sensor_highrange) || (calibrationv < Sensor_lowrange))
         {
             need_calibrating();
         }
         else {
            lcd.setCursor(0,0);
            lcd.print("Calibrated");
        }
    }


    for(int i=0; i<=19; i++) adc0=adc0+ads.readADC_SingleEnded(0);
    currentmv = adc0/20;
    sprintf(publishString, "%3.4f", currentmv);
    // Serial.println(publishString);
    lcd.setCursor(0,3);
    lcd.print(publishString);
    result=((currentmv/calibrationv)*20.9)-.5;
    altitude=81539*(1.783-pow(result, 0.190254));
    lcd.setCursor(0,2);
    sprintf(publishString, "approx alt: %5.0f", altitude);
    lcd.print(publishString);

    lcd.setCursor(0,1);
    sprintf(publishString, "02: %3.1f", result);
    lcd.print(publishString);

    buttonCalibrateClicks = 0;

    // delay(100);
}
