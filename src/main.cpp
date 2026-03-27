#include "Application.h"
#include <exception>
#include <iostream>


int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    try {
        Application app("data");
        app.initialize();
        app.run();
        app.shutdown();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
    } catch (...) {
        std::cerr << "Fatal error: unknown exception\n";
    }

    return 1;
}