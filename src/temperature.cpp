#include "temperature.h"

Temperature::Temperature()
{
    calibrated = false;
    //frequencyShootChargeInfoInMs = 10 * 1000;
    temperatureAnalogPin = 1;
    hotBeverageDetected = false;
    //nbLoopIterationBeforeShooting = 0;
}

void Temperature::calibrate(float temperatureInCelcius)
{
    maxTemperature = temperatureInCelcius;
    previousTemperatureInCelcius = temperatureInCelcius;
    temperatureThresholdToDetectHotBeverage = temperatureInCelcius + 7;
    Particle.publish("calibrate", "threshold temperature: " + String(temperatureThresholdToDetectHotBeverage));
    calibrated = true;
}

float Temperature::computeVoltage(long value)
{
    return value * 3.3 / 4095.0;
}

float Temperature::convertToTemperature(long analogValue)
{
    float voltage = computeVoltage(analogValue);
    float temperature = ((voltage *1000) - 500) / 10;
    return temperature;
}

float Temperature::readTemperature()
{
    long analogValueTemp = analogRead(temperatureAnalogPin);
    return convertToTemperature(analogValueTemp);
}

bool Temperature::detectHotBeverage(float temperatureInCelcius)
{
    bool hasSuccendlyIncreased = false;
    /*float tempDelta = (temperatureInCelcius - previousTemperatureInCelcius) / previousTemperatureInCelcius;
    hasSuccendlyIncreased = tempDelta > 0.15;
    if (hasSuccendlyIncreased) {
        Particle.publish("info",
            "delta temperature: " + String(tempDelta) +
            " previous : " + String(previousTemperatureInCelcius) +
            " current : " + String(temperatureInCelcius));
    }*/
    
    previousTemperatureInCelcius = temperatureInCelcius;
    return temperatureInCelcius > temperatureThresholdToDetectHotBeverage || hasSuccendlyIncreased;
}

void Temperature::setup()
{
}

void Temperature::updateTemperature(float temperatureInCelcius)
{
    if (temperatureInCelcius > maxTemperature)
    {
        maxTemperature = temperatureInCelcius;
    }
    
    if (detectHotBeverage(temperatureInCelcius))
    {
        if (!hotBeverageDetected)
        {
            Particle.publish("temp", "The cup contains a hot beverage");
            hotBeverageDetected = true;
        }
    }
    else if (temperatureInCelcius < temperatureThresholdToDetectHotBeverage - 1)
    {
        if (hotBeverageDetected) {
            Particle.publish("temp", "The cup is empty");
        }
        hotBeverageDetected = false;
    }
}

bool Temperature::update(float dt)
{
    float temperatureInCelcius = readTemperature();
    
    if (!calibrated) {
        calibrate(temperatureInCelcius);
    }

    updateTemperature(temperatureInCelcius);

    //if (nbLoopIterationBeforeShooting == 0)
    //{
        Particle.publish("info",
            " temperature: " + String(temperatureInCelcius) +
            " previous : " + String(previousTemperatureInCelcius) +
            " max : " + String(maxTemperature) +
            " threshold temperature: " + String(temperatureThresholdToDetectHotBeverage));
        //nbLoopIterationBeforeShooting = (int)frequencyShootChargeInfoInMs / dt;
    //}
    //else 
    //{
        //nbLoopIterationBeforeShooting--;
    //}
    return hotBeverageDetected;
}