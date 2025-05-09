#include <iostream>
#include <GL/glut.h>
using namespace std;

const int MAX_POINTS = 20;
int wxmin = 200, wxmax = 500, wymin = 100, wymax = 350;  // Clipping window bounds
int points[MAX_POINTS][2]; // Stores polygon vertices
int edge;  // Number of edges (vertices)

// Function to initialize OpenGL settings
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);  // Set 2D coordinate system
}

// Function to draw a polygon
void drawPolygon() {
    glColor3f(0.2, 0.2, 1);  // Blue polygon
    glBegin(GL_POLYGON);
    for (int i = 0; i < edge; i++) {
        glVertex2i(points[i][0], points[i][1]);
    }
    glEnd();
}

// Function to draw the clipping window
void drawClippingWindow() {
    glColor3f(0, 1, 0);  // Green clipping window
    glBegin(GL_LINE_LOOP);
    glVertex2i(wxmin, wymin);
    glVertex2i(wxmax, wymin);
    glVertex2i(wxmax, wymax);
    glVertex2i(wxmin, wymax);
    glEnd();
}

// Function to display the polygon and clipping window
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawPolygon();
    drawClippingWindow();
    glFlush();
}

// Function to compute intersection of polygon edge with a boundary
void computeIntersection(int x1, int y1, int x2, int y2, int &ix, int &iy, int boundary, int edgeType) {
    float m = (x2 - x1) != 0 ? (float)(y2 - y1) / (x2 - x1) : 0;

    switch (edgeType) {
        case 1:  // Left (wxmin)
            ix = wxmin;
            iy = y1 + m * (wxmin - x1);
            break;
        case 2:  // Right (wxmax)
            ix = wxmax;
            iy = y1 + m * (wxmax - x1);
            break;
        case 3:  // Bottom (wymin)
            iy = wymin;
            ix = x1 + (1 / m) * (wymin - y1);
            break;
        case 4:  // Top (wymax)
            iy = wymax;
            ix = x1 + (1 / m) * (wymax - y1);
            break;
    }
}

// Function to clip the polygon against a given boundary
void clipPolygon(int boundary, int edgeType) {
    int temp[MAX_POINTS][2];  // Temporary array for storing clipped vertices
    int newEdge = 0;

    for (int i = 0; i < edge; i++) {
        int x1 = points[i][0], y1 = points[i][1];
        int x2 = points[(i + 1) % edge][0], y2 = points[(i + 1) % edge][1];

        bool inside1, inside2;

        switch (edgeType) {
            case 1: inside1 = x1 >= wxmin; inside2 = x2 >= wxmin; break;
            case 2: inside1 = x1 <= wxmax; inside2 = x2 <= wxmax; break;
            case 3: inside1 = y1 >= wymin; inside2 = y2 >= wymin; break;
            case 4: inside1 = y1 <= wymax; inside2 = y2 <= wymax; break;
        }

        if (inside1 && inside2) {  // Both points inside
            temp[newEdge][0] = x2;
            temp[newEdge][1] = y2;
            newEdge++;
        } else if (inside1 && !inside2) {  // Going outside
            int ix, iy;
            computeIntersection(x1, y1, x2, y2, ix, iy, boundary, edgeType);
            temp[newEdge][0] = ix;
            temp[newEdge][1] = iy;
            newEdge++;
        } else if (!inside1 && inside2) {  // Coming inside
            int ix, iy;
            computeIntersection(x1, y1, x2, y2, ix, iy, boundary, edgeType);
            temp[newEdge][0] = ix;
            temp[newEdge][1] = iy;
            newEdge++;

            temp[newEdge][0] = x2;
            temp[newEdge][1] = y2;
            newEdge++;
        }
    }

    // Update polygon with new clipped vertices
    edge = newEdge;
    for (int i = 0; i < edge; i++) {
        points[i][0] = temp[i][0];
        points[i][1] = temp[i][1];
    }

    glutPostRedisplay();
}

// Function to handle menu interaction
void menu(int choice) {
    switch (choice) {
        case 1: clipPolygon(wxmin, 1); break;  // Left Clipping
        case 2: clipPolygon(wxmax, 2); break;  // Right Clipping
        case 3: clipPolygon(wymin, 3); break;  // Bottom Clipping
        case 4: clipPolygon(wymax, 4); break;  // Top Clipping
    }
}

// Main function
int main(int argc, char **argv) {
    cout << "Enter number of edges of polygon: ";
    cin >> edge;

    if (edge < 3 || edge > MAX_POINTS) {
        cout << "Invalid number of edges! Must be between 3 and " << MAX_POINTS << endl;
        return 1;
    }

    cout << "Enter the coordinates (x y) of each vertex:\n";
    for (int i = 0; i < edge; i++) {
        cout << "Vertex " << i + 1 << ": ";
        cin >> points[i][0] >> points[i][1];
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Polygon Clipping - Sutherland Hodgman Algorithm");

    init();

    glutCreateMenu(menu);
    glutAddMenuEntry("Left Clipping", 1);
    glutAddMenuEntry("Right Clipping", 2);
    glutAddMenuEntry("Bottom Clipping", 3);
    glutAddMenuEntry("Top Clipping", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
