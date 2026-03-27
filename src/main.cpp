#include "Application.h"
#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>

namespace {
Application* g_appInstance = nullptr;

void quickShutdown() {
    if (g_appInstance != nullptr) {
        g_appInstance->shutdown();
        g_appInstance = nullptr;
    }
}

void handleSigInt(int signalCode) {
    if (signalCode == SIGINT) {
        std::quick_exit(130);
    }
}
} // namespace


int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    try {
        std::at_quick_exit(quickShutdown);
        std::signal(SIGINT, handleSigInt);

        Application app("data");
        g_appInstance = &app;

        app.initialize();
        app.run();
        app.shutdown();
        g_appInstance = nullptr;
        return 0;
    } catch (const std::exception& e) {
        quickShutdown();
        std::cerr << "Fatal error: " << e.what() << "\n";
    } catch (...) {
        quickShutdown();
        std::cerr << "Fatal error: unknown exception\n";
    }

    return 1;
}