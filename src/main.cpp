#include "Application.h"


int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    Application app("data");
    app.run();
    return 0;
}