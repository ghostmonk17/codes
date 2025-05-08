#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
using namespace std;

float x1, x2, y2, y3;
int ch;

int sign(float a) {
    if (a < 0) {
        return -1;
    }
    if (a > 0) {
        return 1;
    }
    return 0;
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.6, 0.9);
    glBegin(GL_LINES);
    glVertex2i(-640, 0);
    glVertex2i(640, 0);
    glVertex2i(0, 480);
    glVertex2i(0, -480);
    glEnd();

    float dy, dx, x, y, delx, dely, length;
    int i;
    dx = x2 - x1;
    dy = y3 - y2;

    if (abs(dx) >= abs(dy)) {
        length = abs(dx);
    } else {
        length = abs(dy);
    }

    delx = dx / length;
    dely = dy / length;

    x = x1 + 0.5 * sign(delx);
    y = y2 + 0.5 * sign(dely);

    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();

    switch (ch) {
        case 1: { // Simple Line
            glColor3f(0.0, 0.4, 0.2);
            for (i = 1; i <= length; i++) {
                x = x + delx;
                y = y + dely;
                glBegin(GL_POINTS);
                glVertex2i(x, y);
                glEnd();
            }
            break;
        }
        case 2: { // Dashed Line
            glColor3f(0.0, 0.4, 0.2);
            for (i = 1; i <= length; i++) {
                x = x + delx;
                y = y + dely;
                if (i % 16 <= 8) {
                    glBegin(GL_POINTS);
                    glVertex2i(x, y);
                    glEnd();
                }
            }
            break;
        }
        case 3: { // Dotted Line
            glColor3f(0.0, 0.6, 0.8);
            for (i = 1; i <= length; i++) {
                x = x + delx;
                y = y + dely;
                if (i % 4 == 0) {
                    glBegin(GL_POINTS);
                    glVertex2i(x, y);
                    glEnd();
                }
            }
            break;
        }
    }

    glFlush();
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-640, 640, -480, 480);
}

int main(int argc, char** argv) {
    cout << "Enter x1: ";
    cin >> x1;
    cout << "Enter y1: ";
    cin >> y2;
    cout << "Enter x2: ";
    cin >> x2;
    cout << "Enter y2: ";
    cin >> y3;

    cout << "1. Simple Line  2. Dashed Line  3. Dotted Line" << endl;
    cout << "Enter your choice: ";
    cin >> ch;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 150);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Line Drawing");

    init();

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
