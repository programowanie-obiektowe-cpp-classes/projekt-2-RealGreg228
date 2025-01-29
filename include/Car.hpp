#ifndef CAR_HPP
#define CAR_HPP

#include "CarComponents.hpp"
#include <cmath>
/* 

Mocno inspirowane: 
https://www.reddit.com/r/gamedev/comments/1320er/math_for_simulating_a_car_transmission_xpost_rmath/

*/

class Car{
    private:
        // Parametry pojazdu
        const double mass = 2500;                                     // kg
        const double wheel_radius = 0.34;                             // m
        const double wheel_circumference = 2 * M_PI * wheel_radius;   // m
        // Elementy pojazdu
        Transmission transmission;
        Engine engine;
        FuelSystem fuel_system;
        // Zmienne Pojazdu
        double speed;           // m/s
        double acceleration;    // m/s^2
        double distance;        // m
        
    public:
        Car();
        void startEngine();
        void stopEngine();
        void shiftGearUp();
        void shiftGearDown();
        void update(double throttle, double brake, float dt);
        bool isEngineRunning() const;
        double getSpeed() const;
        double getAcceleration() const;
        double getDistanceTraveled() const;
        void updateDistanceTraveled(float dt);
        double getEngineRPM() const;
        int getCurrentGear() const;
        double getGearRatioForGear(int gear_number) const;
        double getFuelLevel() const;
        double getOilLevel() const;
        void refuel(float ammount);
        void refillOil(float ammount);
};

#endif // CAR_HPP