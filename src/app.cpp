#include "app.h"

void App::setup() {

}

void App::loop() {
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
}
