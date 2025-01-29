#ifndef CAR_COMPONENTS_HPP
#define CAR_COMPONENTS_HPP

#include <vector>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include <algorithm>

// Pojedyńczy bieg
struct Gear{
    const int gear_number;
    const double ratio;
    Gear(int num, double ratio);
};

// Skrzynia biegów
class Transmission{
    private:
        std::vector<Gear> gears;
        int gear_index;
        const double finalDriveRatio;
    public:
        Transmission();
        void shiftUp();
        void shiftDown();
        double getEffectiveGearRatio() const;
        int getGearIndex() const;
        double getGearRatioForGear(int gear_number) const;
};

//Silnik
class Engine{
    private:
        double rpm;
        std::vector<std::pair<double, double>> torque_curve;
        bool is_running;
        double oil_level;
    public:
        Engine();
        void start();
        void stop();
        double getTorque() const;
        double getRPM() const;
        void setRPM(double new_rpm);
        bool isEngineRunning() const;
        void consumeOil(double distance);
        bool isSeized() const;
        double getOilLevel() const;
        void refillOil(double amount);
};

// System paliwa
class FuelSystem {
private:
    double fuel_level;
public:
    FuelSystem();
    void consumeFuel(double distance);
    double getFuelLevel() const;
    void refuel(double amount);
};

#endif // CAR_COMPONENTS_HPP