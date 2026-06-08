#define _USE_MATH_DEFINES
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <GL/gl.h>
#include "Libs/noise/noise.h"
#include "Libs/noise/module/module.h"
#include "Libs/noise/module/perlin.h"
#include <cmath>
#include <ctime>
const int WIDTH = 800;
const int HEIGHT = 800;
const int scl = 4;
float sunx = 300, suny = 0, sunz = -400;
struct Col {
    float r;
    float g;
    float b;
    float alfa;
};
float getAngle(float x, float y, float z, float a1, float b1, float c1,
    float a2, float b2, float c2, float a3, float b3, float c3,
    float a4, float b4, float c4)
{
    float normal_vector[3];
    float v1[3] = { a2 - a1, b2 - b1, c2 - c1 };
    float v2[3] = { a3 - a1, b3 - b1, c3 - c1 };
    normal_vector[0] = v1[1] * v2[2] - v1[2] * v2[1];
    normal_vector[1] = v1[2] * v2[0] - v1[0] * v2[2];
    normal_vector[2] = v1[0] * v2[1] - v1[1] * v2[0];

    float point_vector[3] = { x - a1, y - b1, z - c1 };

    float dot_product = point_vector[0] * normal_vector[0]
        + point_vector[1] * normal_vector[1]
        + point_vector[2] * normal_vector[2];
    float point_vector_length = sqrt(point_vector[0] * point_vector[0]
        + point_vector[1] * point_vector[1]
        + point_vector[2] * point_vector[2]);
    float normal_vector_length = sqrt(normal_vector[0] * normal_vector[0]
        + normal_vector[1] * normal_vector[1]
        + normal_vector[2] * normal_vector[2]);
    float angle = acos(dot_product / (point_vector_length * normal_vector_length));

    return angle * 180.0 / M_PI;
}

float map(float v, float x1, float y1, float x2, float y2) {
    return x2 + (y2 - x2) * ((v - x1) / (y1 - x1));
}
void rect(float x, float y, float a, float b, Col col) {
    glColor4f(col.r, col.g, col.b, col.alfa);
    glBegin(GL_QUADS);             
    glVertex2f(x, y);               
    glVertex2f(x + a, y);           
    glVertex2f(x + a, y + b);      
    glVertex2f(x, y + b);          
    glEnd();
}
GLfloat light_position[] = { 1.0f, 0.0f, 1.0f, 0.0f }; 
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 


int main() {

    if (!glfwInit()) {
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "halne", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -2500, 2500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.1f, 0.25f, 0.7f, 1.0f);
    float h = 35;
    float xoff = 800;
    float yoff = 800;
    float z = 0;
    float s = 0;
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        noise::module::Perlin perlin = noise::module::Perlin::Perlin();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE_LOOP);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTranslatef(0, HEIGHT / 2, 0);
        glRotatef(70, 1.0f, 0.0f, 0.0f);
        glTranslatef(0, -HEIGHT / 2, 0);
      /*  glPointSize(30);
        glBegin(GL_POINTS);
        glVertex3f(sunx, suny, sunz);
        glEnd();*/
        z += 0.01;

        for (float i = 0; i < WIDTH; i += scl) {
            for (float j = -HEIGHT; j < HEIGHT; j += scl) {
                float h1 = map(perlin.GetValue(i / xoff, j / yoff, z), 0, 1, -h, h);
                float h2 = map(perlin.GetValue(i / xoff, (j + scl) / yoff, z), 0, 1, -h, h);
                float h3 = map(perlin.GetValue((i + scl) / xoff, (j + scl) / yoff, z), 0, 1, -h, h);
                float h4 = map(perlin.GetValue((i + scl) / xoff, j / yoff, z), 0, 1, -h, h);
                float angle = getAngle(sunx, suny, sunz,
                    i, j, h1,
                    i, j + scl, h2,
                    i + scl, j+ scl, h3,
                    i + scl, j, h4
                );
                float alpha = 1 - abs(angle) / 120;
                glColor4f(0.2f * alpha, 0.2f * alpha, 1.0f * alpha, 1.0f);
                glBegin(GL_TRIANGLE_FAN);
                glVertex3f(i, j, h1);
                glVertex3f(i, j + scl,h2);
                glVertex3f(i + scl, j+ scl, h3);
                glVertex3f(i + scl, j, h4);
                glEnd();
            }
        }


        glPopMatrix();

        Col col = { 1,1,1,0.5 };

     //   rect(0, 0, 300, 500, col);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}