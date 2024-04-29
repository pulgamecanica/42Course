//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CAR_HPP__
# define __CAR_HPP__

#include <iostream>
#include <vector>

#include "ex00.inc"

namespace ex00 {

    // In charge of the engine, turns engine on and off
    class Engine {
    public:
        void start() {
            is_on_ = true;
            if (DEBUG)
                std::cout << "Engine started.\n";
        }

        void stop() {
            is_on_ = false;
            if (DEBUG)
                std::cout << "Engine stopped.\n";
        }

        bool get_status() const {
            return is_on_;
        }

    private:
        bool is_on_;
    };

    std::ostream& operator<<(std::ostream& s, const Engine& c) {
        s << "Engine[" << (c.get_status() ? "on" : "off") << "]";
        return s;
    }

    // Transmission class responsible for shifting gears and reversing
    class Transmission {
    public:
        Transmission(): current_gear(PARKING) {
            gears.push_back("R");
            gears.push_back("P");
            gears.push_back("1st");
            gears.push_back("2nd");
            gears.push_back("3th");
            gears.push_back("4th");
            gears.push_back("5th");
        }
        void shift_up() {
            if (current_gear < gears.size())
                current_gear++;
            if (DEBUG)
                std::cout << "Shifted gears up.\n";
        }

        void shift_down() {
            if (current_gear > 0)
                current_gear--;
            if (DEBUG)
                std::cout << "Shifted gears down.\n";
        }

        void reverse() {
            current_gear = REVERSE;
            if (DEBUG)
                std::cout << "Transmission in reverse.\n";
        }

        const std::string get_current_gear() const {
            return gears[current_gear];
        }
    private:
        enum gears {
            PARKING,
            REVERSE,
            GEAR1,
            GEAR2,
            GEAR3,
            GEAR4,
            GEAR5
        };
        unsigned current_gear;
        std::vector<std::string> gears;
    };

    std::ostream& operator<<(std::ostream& s, const Transmission& t) {
        s << t.get_current_gear() << " Gear";
        return s;
    }

    // Steering class responsible for turning the wheels
    class Steering {
    public:
        Steering(): angle(90) {
            ;
        }

        void turn(float angle) {
            this->angle += angle;
            if (DEBUG)
                std::cout << "Turning the wheels by " << angle << " degrees.\n";
        }

        void straighten() {
            angle = 90;
            if (DEBUG)
                std::cout << "Wheels straightened.\n";
        }

        float get_angle() const {
            return angle;
        }
    private:
        float angle;
    };

    std::ostream& operator<<(std::ostream& s, const Steering& st) {
        s << st.get_angle() << "°";
        return s;
    }

    // In chrge of controlling the speed
    class CarSpeedController {
    public:
        CarSpeedController(): MAX_SPEED(42) {
            ;
        }

        void input(float force) {
            speed += force;
            if (speed > MAX_SPEED)
                speed = MAX_SPEED;
            if (speed < 0)
                speed = 0;
        }

        int get_speed() const {
            return speed;
        }
    private:
        const unsigned MAX_SPEED;
        float speed;
    };

    std::ostream& operator<<(std::ostream& s, const CarSpeedController& csc) {
        s << csc.get_speed() << "/kph";
        return s;
    }

    // Brakes class responsible for applying force on brakes
    class Brakes {
    public:
        Brakes(CarSpeedController *csc): csc(csc), MAX_FORCE(42) {
            ;
        }

        void apply_force(float force = 42) {
            if (force > MAX_FORCE)
                force = MAX_FORCE;
            csc->input(-force);
            if (DEBUG)
                std::cout << "Applying " << force << " units of force on the brakes.\n";
        }

        void apply_emergency() {
            apply_force(MAX_FORCE);
            if (DEBUG)
                std::cout << "Applying emergency brakes with maximum force.\n";
        }
    private:
        CarSpeedController *csc;
        const unsigned MAX_FORCE;
    };

    // In charge of accelerating the car
    class Accelerator {
    public:
        Accelerator(CarSpeedController *csc): csc(csc) {
            ;
        }

        void accelerate(float speed) {
            csc->input(speed);
            if (DEBUG)
                std::cout << "Accelerating by " << speed << " km/h.\n";                
        }
    private:
        CarSpeedController *csc;
    };

    class car {
    public:
        car(): LIMIT_SPEED(200), brakes(&csc), accelerator(&csc) {
        }

        ~car() {
            ;
        }

        void start() {
            engine.start();
        }

        void stop() {
            brakes.apply_emergency();
            engine.stop();
        }

        void accelerate(int speed) {
            accelerator.accelerate(speed);
        }

        void shift_gears_up() {
            transmission.shift_up();
        }

        void shift_gears_down() {
            transmission.shift_down();
        }

        void reverse() {
            transmission.reverse();
        }

        void turn_wheel(int angle) {
            steering.turn(angle);
        }

        void straighten_wheels() {
            steering.straighten();
        }

        void apply_force_on_brakes(int force) {
            brakes.apply_force(force);
        }

        void apply_emergency_brakes() {
            brakes.apply_force();
        }

        std::ostream& get_car_str(std::ostream& s) const {
            s << engine << " " << csc << " " << transmission << " " << steering;
            return s;
        }
    private:
        const unsigned LIMIT_SPEED;
        Engine engine;
        Transmission transmission;
        Steering steering;
        Brakes brakes;
        Accelerator accelerator;
        CarSpeedController csc;
    };

    std::ostream& operator<<(std::ostream& s, const car& c) {
        return (c.get_car_str(s));
    }
}
#endif
