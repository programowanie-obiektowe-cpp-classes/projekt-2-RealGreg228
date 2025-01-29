#include "Simulation.hpp"
#include "imgui.h"
#include <chrono>
#include <thread>
#include <vector>

Simulation::Simulation() : throttle{0.0f}, fuel_to_add{0.0f},
        oil_to_add{0.0f}, brake{0.0f} {}

void Simulation::addError(const std::string& message, float duration, int id){
    for (const auto& error : errorMessages) {
        if (error.id == id && ImGui::GetTime() - error.displayStartTime < error.displayDuration) {
            return; 
        }
    }
    errorMessages.push_back(ErrorMessage(message, ImGui::GetTime(), duration, id));
}

void Simulation::run(){
    static std::vector<float> speedHistory; // Historia prędkości
    static int maxHistorySize = 100;

    // Podstawowe okno
    ImGui::Begin("Car Simulation");
    ImGui::Text("Speed: %.2f km/h", car.getSpeed()*3.6);
    ImGui::Text("Acceleration: %.2f m/s^2", car.getAcceleration());
    ImGui::Text("Distance Traveled: %.2f km", car.getDistanceTraveled()/1000);
    ImGui::Text("Current Gear: %d", car.getCurrentGear()-1);
    ImGui::Text("Engine RPM: %.2f", car.getEngineRPM());
    ImGui::Text("Fuel Level: %.2f L", car.getFuelLevel());
    ImGui::Text("Oil Level: %.2f L", car.getOilLevel());

    if (ImGui::Button(car.isEngineRunning() ? "Stop Engine" : "Start Engine")) {
        if (car.isEngineRunning())
            car.stopEngine();
        else
            car.startEngine();
    }

    ImGui::SliderFloat("Gas", &throttle, 0.0f, 1.0f); // Zakres przepustnicy <0,1>
    ImGui::SliderFloat("Brake", &brake, 0.0f, 1.0f); // Identycznie hamulec

    try {
        car.update(throttle, brake, ImGui::GetIO().DeltaTime);
    } catch (const std::exception &ex) {
        addError(ex.what(), 5.0f, 1);
    }

    if (speedHistory.size() >= maxHistorySize) {
        speedHistory.erase(speedHistory.begin()); // Usuwamy najstarszy element
    }
    speedHistory.push_back(car.getSpeed() * 3.6);
    // Rysowanie wykresu prędkości
    ImGui::Separator();
    ImGui::Text("Speed Chart:");
    ImGui::PlotLines("Speed (km/h)", speedHistory.data(), speedHistory.size(), 0, nullptr, -50.0f, 170.0f, ImVec2(0, 150));

    ImGui::End();

    // Zmiana biegów
    ImGui::Begin("Transmission Control");
    ImGui::Text("Current Gear: %d", car.getCurrentGear()-1);

    if (ImGui::Button("Shift Up")) {
        try {
            car.shiftGearUp();
        } catch (const std::exception &ex) {
            addError(ex.what(), 3.0f, 2);
        }
    }
    if (ImGui::Button("Shift Down")) {
        try {
            car.shiftGearDown();
        } catch (const std::exception &ex) {
            addError(ex.what(), 3.0f, 3);
        }
    }

    ImGui::Separator();
    ImGui::Text("Available Gears:");
    for (int i = -1; i <= 5; ++i) {
        ImGui::Text("Gear %d: %.2f ratio", i, car.getGearRatioForGear(i));
    }
    ImGui::End();

    // Erorr log
    ImGui::Begin("Error log");
    for (auto it = errorMessages.begin(); it != errorMessages.end(); ) {
        float currentTime = ImGui::GetTime();
        if (currentTime - it->displayStartTime > it->displayDuration) {
            it = errorMessages.erase(it);
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error: %s", it->message.c_str());
            ++it;
        }
    }
    ImGui::End();

    // Stacja Paliw
    ImGui::Begin("Fuel Station");

    ImGui::Text("Fuel Station:");

    // Suwak do wybierania ilości paliwa
    ImGui::SliderFloat("Fuel to Add (l)", &fuel_to_add, 0.0f, 50.0f);
    if (ImGui::Button("Refuel")) {
        try{
            car.refuel(fuel_to_add);
        } catch(const std::exception &ex) {
            addError(ex.what(), 3.0f, 4);
        } 
        fuel_to_add = 0.0f;      
    }

    // Suwak do wybierania ilości oleju
    ImGui::SliderFloat("Oil to Add (l)", &oil_to_add, 0.0f, 5.0f);
    if (ImGui::Button("Refill Oil")) {
        try{
            car.refillOil(oil_to_add);
        } catch(const std::exception &ex) {
            addError(ex.what(), 3.0f, 5);
        } 
        oil_to_add = 0.0f;         
    }

    ImGui::End();
}