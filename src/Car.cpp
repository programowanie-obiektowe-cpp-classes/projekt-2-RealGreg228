#include "Car.hpp"
#include <cmath>

Car::Car() : speed{0}, distance{0}, acceleration{0} {}

void Car::startEngine() { engine.start(); }
void Car::stopEngine() { engine.stop(); }
void Car::shiftGearUp() { transmission.shiftUp(); }
void Car::shiftGearDown() { transmission.shiftDown(); }

void Car::update(double throttle, double brake, float dt){
    if(brake!=0){
        engine.setRPM(0);
        if(speed > 0) {  acceleration = -brake*25; speed += acceleration*dt; }
        else if(speed < -0.5) { acceleration = brake*25; speed += acceleration*dt; } 
        else { speed = 0.0; acceleration = 0.0; }
    } else if(engine.isSeized() || fuel_system.getFuelLevel() <= 0 || 
        !engine.isEngineRunning()){
        engine.setRPM(0);
        if(speed > 0) { acceleration = -0.5; speed += acceleration*dt; }
        else if(speed < -0.5){ acceleration = 0.5; speed += acceleration*dt; }
        else { speed = 0.0; acceleration = 0.0; }
    } else if(speed == 0 && transmission.getGearIndex() > 2){
        throw std::runtime_error("Car cannot start form gear higher than 1!");
    } else {
        double new_rpm = (speed/(wheel_circumference/60)) * 
            transmission.getEffectiveGearRatio();
        engine.setRPM(new_rpm);

        double wheel_force = (engine.getTorque() * throttle * 
            transmission.getEffectiveGearRatio()) / wheel_radius;
        double drag_force = 0.404 * speed * speed;
        double net_force = wheel_force - drag_force;

        acceleration = net_force/mass;
        speed += acceleration*dt;

        fuel_system.consumeFuel(std::abs(speed*dt / 3600));
        engine.consumeOil(std::abs(speed*dt / 3600));
    }

    updateDistanceTraveled(dt);
}

bool Car::isEngineRunning() const { return engine.isEngineRunning(); }
double Car::getSpeed() const { return speed; }
double Car::getAcceleration() const { return acceleration; }
double Car::getDistanceTraveled() const { return distance; }
double Car::getEngineRPM() const { return engine.getRPM(); }
int Car::getCurrentGear() const { return transmission.getGearIndex(); }
double Car::getGearRatioForGear(int gear_number) const { return transmission.getGearRatioForGear(gear_number); }
double Car::getFuelLevel() const { return fuel_system.getFuelLevel(); }
double Car::getOilLevel() const { return engine.getOilLevel(); }

void Car::refuel(float ammount){
    if(speed != 0){
        throw std::runtime_error("Can't refuel while moving!");
    } else if( fuel_system.getFuelLevel() == 50){
        throw std::runtime_error("Tank is already full");
    } else if(50 - fuel_system.getFuelLevel() < ammount){
        throw std::runtime_error("Can't refuel above 50l!");
    } else {
        fuel_system.refuel(ammount);
    }
}
void Car::refillOil(float ammount){
    if(speed != 0){
        throw std::runtime_error("Can't refill oil while moving!");
    } else if( engine.getOilLevel() == 5){
        throw std::runtime_error("Oil reserve is already full");
    } else if(5 - engine.getOilLevel() < ammount){
        throw std::runtime_error("Can't refill oil above 5l!");
    } else {
        engine.refillOil(ammount);
    }
}

void Car::updateDistanceTraveled(float dt){
    distance += std::abs(speed)*dt;
}