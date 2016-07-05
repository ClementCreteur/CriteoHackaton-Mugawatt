// This #include statement was automatically added by the Particle IDE.
#include "SparkFunMAX17043/SparkFunMAX17043.h"

class Charge
{
    bool isFullyCharged;
    bool hasLowBattery;

public:
    Charge();
    void setup();
    float update(float dt);

private:
    void updateCharge(float charge);
};
