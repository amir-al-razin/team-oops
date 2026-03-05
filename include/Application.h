#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include "DataManager.h"
#include "MenuSystem.h"

class Application {
private:
    DataManager dm;
    MenuSystem menu;
    std::string dataDir;

public:
    explicit Application(std::string dataDir = "data");
    void run();
};

#endif