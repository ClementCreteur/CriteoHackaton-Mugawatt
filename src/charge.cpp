#include "charge.h"

Charge::Charge()
{
    isFullyCharged = false;
    hasLowBattery = false;
}

void Charge::setup()
{
    lipo.begin();
    lipo.quickStart();
}

void Charge::updateCharge(float charge)
{
    if (charge > 99.0)
    {
        if (!isFullyCharged)
        {
            Particle.publish("charge", "The cup is fully charged");
        }
        isFullyCharged = true;
    }
    else
    {
        isFullyCharged = false;
        if (charge < 10.0)
        {
            if (!hasLowBattery)
            {
                Particle.publish("charge", "The cup has only 10% of battery");
            }
            hasLowBattery = true;
        }
        else
        {
            hasLowBattery = false;
        }
    }
}

float Charge::update(float dt)
{
    float charge = lipo.getSOC();

    updateCharge(charge);

    return charge;
}