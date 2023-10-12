#include "app.h"


int main(int, char**)
{   
    App app;
    if (!app.init()) { return 1; }

    // Main loop
    while (!app.windowShouldClose()) {
        app.loop();
    }

    app.deinit();

    return 0;
}