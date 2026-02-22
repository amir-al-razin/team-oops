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
    // Added spaces before the ANSI color codes so they center perfectly under the logo
    std::cout << "                             \033[36mDeveloped by Basement Dwellers Team\033[0m\n";
    std::cout << "                        \033[33mCSE 4301 - Object Oriented Programming Project\033[0m\n\n";


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