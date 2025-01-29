#include "CarComponents.hpp"
#include <vector>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include <algorithm>

// Pojedyńczy bieg
Gear::Gear(int num, double ratio) : gear_number{num}, ratio{ratio} {}

// Skrzynia biegów
Transmission::Transmission() : finalDriveRatio{4.2}, gear_index{1}{
    gears.emplace_back(-1, -3.3); // Wsteczny
    gears.emplace_back(0, 0.0);  // Luz
    gears.emplace_back(1, 3.3);  // Pierwszy bieg
    gears.emplace_back(2, 2.1);  // Drugi bieg
    gears.emplace_back(3, 1.5);  // Trzeci bieg
    gears.emplace_back(4, 1.0);  // Czwarty bieg
    gears.emplace_back(5, 0.5);  // Piąty bieg
}

void Transmission::shiftUp(){
    if (gear_index < static_cast<int>(gears.size()) - 1) {
        gear_index++;
    } else {
        throw std::runtime_error("Cannot shift up: already in the highest gear!");
    }
}

void Transmission::shiftDown(){
    if (gear_index > 0) {
        gear_index--;
    } else {
        throw std::runtime_error("Cannot shift down: already in the lowest gear!");
    }
}

double Transmission::getEffectiveGearRatio() const {
    return gears[gear_index].ratio * finalDriveRatio;
}

int Transmission::getGearIndex() const { return gear_index; }
double Transmission::getGearRatioForGear(int gear_number) const {
    auto it = std::find_if(gears.begin(), gears.end(), 
        [gear_number](const Gear& gear) {
            return gear.gear_number == gear_number;
        });
    return it->ratio;
}
// Silnik
Engine::Engine() : rpm{0}, is_running{false}, oil_level{5.0} {
    torque_curve = {
        {0, 50}, {500, 100}, {1000, 150}, {1500, 200}, {2000, 250}, {2500, 300},
        {3000, 350}, {3500, 400}, {4000, 450}, {4500, 400}, {5000, 350}};
}

void Engine::start(){ 
    is_running = true; 
    rpm = 600;          // Bieg jałowy
}

void Engine::stop(){ is_running = false; }

void Engine::setRPM(double new_rpm){
    rpm = new_rpm; 
}

double Engine::getTorque() const {
    // Jeśli RPM jest poza zakresem, zwróć 0.0 
    if (rpm < torque_curve.front().first || rpm > torque_curve.back().first) {
        return 0.0;
    }
    // Znajdź pierwszy element, który nie jest mniejszy niż RPM
    auto it = std::lower_bound(torque_curve.begin(), torque_curve.end(), std::make_pair(rpm, 0.0),
                               [](const std::pair<double, double>& a, const std::pair<double, double>& b) {
                                   return a.first < b.first; // Porównaj po wartości RPM
                               });
    // Jeśli znaleziono dokładnie punkt odpowiadający RPM, zwróć jego wartość
    if (it->first == rpm) { return it->second; }
    // Interpolacja liniowa pomiędzy poprzednim a bieżącym punktem
    auto prev = std::prev(it); // Poprzedni punkt
    //double r1 = prev->first, t1 = prev->second;
    //double r2 = it->first, t2 = it->second;

    return prev->second + (rpm - prev->first) * (it->second - prev->second) / 
        (it->first - prev->first);
}

double Engine::getRPM() const { return rpm; }

bool Engine::isEngineRunning() const { return is_running; }

void Engine::consumeOil(double distance){
    double consumed = (distance / 100.0) * 42.0;
    oil_level -= consumed;

    if (oil_level <= 0) {
        oil_level = 0;
        throw std::runtime_error("Out of oil!");
    }
}

bool Engine::isSeized() const {
    return oil_level <= 0;
}

double Engine::getOilLevel() const {
    return oil_level;
}

void Engine::refillOil(double amount) {
    oil_level += amount;
}

// System paliwa
FuelSystem::FuelSystem() : fuel_level(50.0) {}

void FuelSystem::consumeFuel(double distance) {
    double consumed = (distance / 100.0) * 120.0;
    fuel_level -= consumed;

    if (fuel_level <= 0) {
        fuel_level = 0;
        throw std::runtime_error("Out of fuel!");
    }
}

double FuelSystem::getFuelLevel() const {
    return fuel_level;
}

void FuelSystem::refuel(double amount) {
    fuel_level += amount;
}