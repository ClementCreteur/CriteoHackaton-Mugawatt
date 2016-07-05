#include "accelerometer.h"
#include <math.h>

const float G = 9.8f;
const float deadzone = 0.1f;
const int samplings = 100;

void Accelerometer::setup()
{
    accel.begin(SCALE_2G, ODR_100);
    calibrating = true;
    nbsamples = 0;
}

bool Accelerometer::update(float dt)
{
    bool has_been_flipped = false;
    if (calibrating) {
        calibrate();
    } else {
        if (accel.available()) accel.read();

        if (compute_acceleration() && has_moved()) {
            has_been_flipped = is_flipped();
            String log = (has_been_flipped ? "FLIPPED" : "MOVED");
            log += " (" + String(acc.x) + ", " + String(acc.y) + ", " + String(acc.z) + ")";
            if (has_been_flipped)
            {
                Particle.publish("flip", log);
            }
        }
    }
    return has_been_flipped;
}

void Accelerometer::calibrate()
{
    if (nbsamples < samplings) {
        accel.read();
        ground.x += accel.cx;
        ground.y += accel.cy;
        ground.z += accel.cz;
        ++nbsamples;
    } else {
        ground.x = ground.x / nbsamples;
        ground.y = ground.y / nbsamples;
        ground.z = ground.z / nbsamples - 1;
        nbsamples = 0;
        calibrating = false;
        String orient = "calibrated to (" + String(ground.x) + ", " + String(ground.y) + ", " + String(ground.z) + ")";
        Particle.publish("calibrate", orient);
    }
}

bool Accelerometer::compute_acceleration()
{
    if (nbsamples < samplings) {
        newacc.x += (accel.cx - ground.x);
        newacc.y += (accel.cy - ground.y);
        newacc.z += (accel.cz - ground.z);
        ++nbsamples;
        return false;
    } else {
        newacc.x *= G / nbsamples;
        newacc.y *= G / nbsamples;
        newacc.z *= G / nbsamples;
        
        oldacc = acc;
        acc = newacc;
        newacc.x = newacc.y = newacc.z = 0;
        nbsamples = 0;
        return true;
    }
}

bool Accelerometer::has_moved()
{
    return fabs(acc.x - oldacc.x) > deadzone
        || fabs(acc.y - oldacc.y) > deadzone
        || fabs(acc.z - oldacc.z) > deadzone;
    
}

bool Accelerometer::is_flipped()
{
    return (fabs(acc.x) > 5 || fabs(acc.y) > 5);
}

