#include "Application.h"
#include <iostream>

Application::Application(std::string dataDir)
    : dm(), menu(dm), dataDir(std::move(dataDir)) {}

void Application::run() {
    try {
        dm.loadAll(dataDir);
        std::cout << "Loaded data from: " << dataDir << "\n";
    } catch (const std::exception& e) {
        std::cout << "Warning: Could not load data (" << e.what()
                  << "). Starting with empty data.\n";
    }

    menu.mainLoop();

    try {
        dm.saveAll(dataDir);
        std::cout << "Saved data to: " << dataDir << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Could not save data (" << e.what() << ").\n";
    }
}