#include <iostream>
#include <GL/glut.h>
#include <cmath>

using namespace std;

float paddleX = 300; // Paddle's x position
float paddleY = 20;  // Paddle's y position
float ballX = 320;   // Ball's x position
float ballY = 240;   // Ball's y position
float ballDirectionX = 2; // Ball's x direction
float ballDirectionY = 2; // Ball's y direction
const float paddleWidth = 80; // Paddle width
const float paddleHeight = 20; // Paddle height
const float ballRadius = 10; // Ball radius

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

void drawPaddle() {
    glColor3f(0, 0, 1); // Blue color for the paddle
    glBegin(GL_POLYGON);
    glVertex2i(paddleX, paddleY);
    glVertex2i(paddleX + paddleWidth, paddleY);
    glVertex2i(paddleX + paddleWidth, paddleY + paddleHeight);
    glVertex2i(paddleX, paddleY + paddleHeight);
    glEnd();
}

void drawBall() {
    glColor3f(1, 0, 0); // Red color for the ball
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * 3.14159 / 180;
        glVertex2f(ballX + ballRadius * cos(angle), ballY + ballRadius * sin(angle));
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawPaddle();
    drawBall();
    glutSwapBuffers();
}

void timer(int) {
    // Move the ball
    ballX += ballDirectionX;
    ballY += ballDirectionY;

    // Check for collision with the walls
    if (ballX - ballRadius < 0 || ballX + ballRadius > 640) {
        ballDirectionX = -ballDirectionX; // Reverse x direction
    }
    if (ballY + ballRadius > 480) {
        ballDirectionY = -ballDirectionY; // Reverse y direction
    }
    
    // Check if the ball touches the bottom wall
    if (ballY - ballRadius <= 0) {
        cout << "Game Over! The ball touched the bottom wall." << endl;
        exit(0); // Exit the game
    }

    // Check for collision with the paddle
    if (ballY - ballRadius <= paddleY + paddleHeight && 
        ballX >= paddleX && ballX <= paddleX + paddleWidth) {
        ballDirectionY = -ballDirectionY; // Reverse y direction
        ballY = paddleY + paddleHeight + ballRadius; // Position the ball above the paddle
    }

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

void keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        paddleX -= 20; // Move left
        if (paddleX < 0) paddleX = 0; // Prevent going out of bounds
    }
    if (key == GLUT_KEY_RIGHT) {
        paddleX += 20; // Move right
        if (paddleX + paddleWidth > 640) paddleX = 640 - paddleWidth; // Prevent going out of bounds
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Block Blaster Game");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutSpecialFunc(keyboard); // Use special keys for paddle movement
    glutMainLoop();
    return 0;
}
