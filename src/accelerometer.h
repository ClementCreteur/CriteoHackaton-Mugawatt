// This #include statement was automatically added by the Particle IDE.
#include "SparkFunMMA8452Q/SparkFunMMA8452Q.h"

class Accelerometer
{
    struct Vec3 {
        float x, y, z;
        Vec3() : x(0), y(0), z(0) {}
    };
    
    Vec3 ground, acc, oldacc, newacc;

    MMA8452Q accel;
    bool calibrating;
    int nbsamples;
    
public:
    void setup();
    bool update(float dt);
    
private:
    void calibrate();
    bool compute_acceleration();
    bool has_moved();
    bool is_flipped();
};
