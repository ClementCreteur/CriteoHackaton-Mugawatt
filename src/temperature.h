// This #include statement was automatically added by the Particle IDE.
#include "SparkFunMAX17043/SparkFunMAX17043.h"

class Temperature
{
    bool calibrated;
    int temperatureAnalogPin;
    bool hotBeverageDetected;
    bool isFullyCharged;
    bool hasLowBattery;
    //int frequencyShootChargeInfoInMs;
    //int nbLoopIterationBeforeShooting;
    float maxTemperature;
    float temperatureThresholdToDetectHotBeverage;
    float previousTemperatureInCelcius;

public:
    Temperature();
    void setup();
    bool update(float dt);

private:
    void calibrate(float temperatureInCelcius);
    void updateTemperature(float temperatureInCelcius);
    void updateCharge(float charge);
    float readTemperature();
    float computeVoltage(long value);
    float convertToTemperature(long analogValue);
    bool detectHotBeverage(float temperatureInCelcius);
};
