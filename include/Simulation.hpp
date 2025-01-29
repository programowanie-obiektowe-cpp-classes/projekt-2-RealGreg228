#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "Car.hpp"
#include <vector>

class Simulation{
    private:
        Car car;
        bool is_running;
        float throttle;
        float brake;
        float fuel_to_add;
        float oil_to_add;

        struct ErrorMessage {
            std::string message; // Treść błędu
            double displayStartTime; // Czas dodania błędu
            float displayDuration; // Jak długo wyświetlać błąd (w sekundach)
            int id;
            ErrorMessage(const std::string& msg, double startTime, float duration, int id)
                : message{msg}, displayStartTime{startTime}, displayDuration{duration}, id{id} {}
        };
        std::vector<ErrorMessage> errorMessages;
    public:
        Simulation();
        void run();
        void addError(const std::string& message, float duration, int id);
};

#endif // SIMULATION_HPP