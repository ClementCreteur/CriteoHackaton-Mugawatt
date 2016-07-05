#include "HttpClient/HttpClient.h"
#include "webservice.h"
#include "temperature.h"
#include "charge.h"
#include "accelerometer.h"

Temperature temperature;
Accelerometer accelerometer;
Charge battery;
WebService http;
unsigned long refreshDelay = 10;
unsigned long lastTime = 0;
int nbLoopIterationsBeforeTempUpdate = 0;
int frequencyTempUpdateInMs = 10 * 1000;
int nbLoopIterationsBeforeChargeUpdate = 0;
int frequencyChargeUpdateInMs = 30 * 1000;
bool hasFlipped = false;

void setup() {
    temperature.setup();
    battery.setup();
    accelerometer.setup();
    lipo.begin();
}

void loop() {
    unsigned long dt = millis() - lastTime;
    if (dt > refreshDelay) {
        
        bool newlyFlipped = accelerometer.update(dt);
        hasFlipped = hasFlipped || newlyFlipped;
        
        if (nbLoopIterationsBeforeTempUpdate == 0) {
            bool isHot = temperature.update(dt);
            if (isHot && hasFlipped)
            {
                Particle.publish("sip", "Sending drink");
                http.sendDrink();
            }
            if (!isHot)
            {
                http.sendDrinkIsCold();
            }
            nbLoopIterationsBeforeTempUpdate = (int)frequencyTempUpdateInMs / dt;
            hasFlipped = false;
        }
        else
        {
            nbLoopIterationsBeforeTempUpdate--;
        }
        
        if (nbLoopIterationsBeforeChargeUpdate == 0)
        {
             float charge = battery.update(dt);
             Particle.publish("info", "charge: " + String(charge));
             http.sendBattery(charge);
             nbLoopIterationsBeforeChargeUpdate = (int)frequencyChargeUpdateInMs / dt;
        }
        else
        {
            nbLoopIterationsBeforeChargeUpdate--;
        }
        
        lastTime = millis();
    }
}