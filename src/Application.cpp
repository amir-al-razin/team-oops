#include "Application.h"
#include <iostream>

Application::Application(std::string dataDir)
    : dm(), menu(dm), dataDir(std::move(dataDir)) {}

void Application::run() {
    std::cout << R"(

          ██████╗  █████╗ ███████╗███████╗███╗   ███╗███████╗███╗   ██╗████████╗
          ██╔══██╗██╔══██╗██╔════╝██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝
          ██████╔╝███████║███████╗█████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   
          ██╔══██╗██╔══██║╚════██║██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   
          ██████╔╝██║  ██║███████║███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   
          ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   

          ██████╗ ██╗    ██╗███████╗██╗     ██╗     ███████╗██████╗ ███████╗
          ██╔══██╗██║    ██║██╔════╝██║     ██║     ██╔════╝██╔══██╗██╔════╝
          ██║  ██║██║ █╗ ██║█████╗  ██║     ██║     █████╗  ██████╔╝███████╗
          ██║  ██║██║███╗██║██╔══╝  ██║     ██║     ██╔══╝  ██╔══██╗╚════██║
          ██████╔╝╚███╔███╔╝███████╗███████╗███████╗███████╗██║  ██║███████║
          ╚═════╝  ╚══╝╚══╝ ╚══════╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝

          ======================================================================
                            BASEMENT DWELLERS ERP SYSTEM v1.0
          ======================================================================

)";
    std::cout << "                             \033[36mDeveloped by Basement Dwellers Team\033[0m\n";
    std::cout << "                        \033[33mCSE 4301 - Object Oriented Programming Project\033[0m\n\n";

    bool safeToSave = false; // THE SAFETY LOCK

    try {
        dm.loadAll(dataDir);
        safeToSave = true; // Only unlocks if loading finishes without exceptions
        std::cout << "Loaded data from: " << dataDir << "\n";
    } catch (const std::exception& e) {
        std::cout << "\nCRITICAL WARNING: Could not load data (" << e.what() << ").\n";
        std::cout << "Starting with empty data. Changes will NOT be saved to prevent data loss.\n\n";
    }

    menu.mainLoop();

    // Only save if it's safe!
    if (safeToSave) {
        try {
            dm.saveAll(dataDir);
            std::cout << "Saved data to: " << dataDir << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error: Could not save data (" << e.what() << ").\n";
        }
    } else {
        std::cout << "Skipped saving to protect your original files from being overwritten.\n";
    }
}