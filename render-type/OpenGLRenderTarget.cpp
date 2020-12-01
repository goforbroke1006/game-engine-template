//
// Created by GOFORBROKE on 01.12.2020.
//

#include "OpenGLRenderTarget.h"

#include <GL/freeglut.h>


BaseRenderTarget *OpenGLRenderTarget::create(int w, int h, int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(w, h);
//    glutInitWindowPosition(100, 100);
    glutCreateWindow("hello");

    {
        /* установим черный фон */
        glClearColor(0.0, 0.0, 0.0, 0.0);

        /* инициализация viewing values */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    }

    glutDisplayFunc(display);

    return new OpenGLRenderTarget(w, h);
}

void OpenGLRenderTarget::Release() {
    BaseRenderTarget::Release();
}

void OpenGLRenderTarget::loop(GEngTmp::Scene * scene) {
    glutMainLoop();
}

OpenGLRenderTarget::OpenGLRenderTarget(int w, int h) : BaseRenderTarget(w, h) {}

void OpenGLRenderTarget::display() {
    /* стираем */
    glClear(GL_COLOR_BUFFER_BIT);

    /* рисуем белый прямоугольник
    * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
    */
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.25, 0.25, 0.0);
    glVertex3f(0.75, 0.25, 0.0);
    glVertex3f(0.75, 0.75, 0.0);
    glVertex3f(0.25, 0.75, 0.0);
    glEnd();

    /* поехали ! */
    glFlush();
}
