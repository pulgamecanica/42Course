#include <vector>

// Interaface
class LinkablePart {
public:
    virtual void execute(float p_pressure) = 0;
};

// Wheel
class Wheel {
public:
    void executeRotation(float p_force);
};

// Gear
class Gear {
public:
    int demultiplier;
};


// GearLever => Aggregation with the Gear, a Gear can exist on it's own, and has it's own lifespan.
//              Gear does not depend on the GearLever, Gear can exist on it's own
//              GearLever is not made out a Gear
class GearLever : public Singleton<GearLever> {
public:
    Gear* gearArray;
    int level;
    void change();
    Gear* activeGear();
};

// Transmission =>  Aggregation with Wheel, a wheel is independent of the transmission
// 0..*             Transmission can exist on it's own, is not made out of Wheels
//                  May not work as intended withoutany wheels but still can exist
class Transmission {
public:
    std::vector<Wheel*> wheelVector;
    void activate(float p_force);
};

// Crankshaft =>    Aggregation with Transmission, a crankshaft is independent of the Transmission
// 0..1            Crankshaft has it's own lifespan, as well as the Transmission, both can work withput another
class Crankshaft {
public:
    Transmission* transmissionPointer;
    void receiveForce(float p_volume);
};

// ExplosionChamber => Aggregation with a Crankshaft, it's existence does not depend on a Crankshaft
// 1..1                Both can exists without another, or not exist without another
class ExplosionChamber {
public:
    Crankshaft* crankshaftPointer;
    void fill(float p_volume);
};

// Injector => Aggregation with an Explosion chamber
class Injector : public LinkablePart {
public:
    ExplosionChamber* explosionChamberPointer;
    void execute(float p_pressure);
};

// Motor => Composition with an Injector, Injector is a part of the Motor, motor can't be complete without an Injector
// 1..1     
//       => Composition with an ExplosionChamber, ExplosionChamber is a part of the Motor, motor can't be complete without an ExplosionChamber
// 1..1
//       => Composition with an Crankshaft, Crankshaft is a part of the Motor, motor can't be complete without an Crankshaft
// 1..1
class Motor {
public:
    Injector injector;
    ExplosionChamber explosionChamber;
    Crankshaft crankshaft;
    void connectToTransmission(Transmission* p_transmission);
};

// Pedal => Association with a Linkable Part, the pedal makes use of a linkablePart, but it's not made out of linkableParts
// 1..1
class Pedal {
public:
    LinkablePart* target;
    void setTarget(LinkablePart* p_part);
    void use(float p_pressure);
};

// Direction => Aggregation with Wheels
class Direction {
public:
    std::vector<Wheel*> wheelArray;
    void turn(float p_angle);
};

class DAE {
public:
    Direction* directionPointer;
    float force;
    void use(float p_angle);
};

class SteerWheel {
public:
    DAE* daePointer;
    void turn(float p_angle);
};

class Brake {
public:
    Wheel* wheelPointer;
    void execute(float p_force);
    void attackWheel(Wheel* p_wheel);
};

class BrakeController : public LinkablePart {
public:
    std::vector<Brake> brakeArray;
    void execute(float p_pressure);
};

class Cockpit {
public:
    Pedal pedal;
    SteerWheel steerWheel;
    GearLever gearLever;
    // Methods to manage these objects
};

class Electronics {
public:
    DAE dae;
};

class Car {
public:
    BrakeController brakeController;
    Direction direction;
    Transmission transmission;
    Motor motor;
    Electronics electronics;
    Cockpit cockpit;
};
