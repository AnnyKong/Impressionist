/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef GLINCLUDE_H
#define GLINCLUDE_H

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
    #include <GL/glew.h>
#endif

// Qt5 OpenGL
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <QOpenGLFramebufferObject>

bool _GLCheckError(const char *file, int line);
#define GLCheckError() _GLCheckError(__FILE__,__LINE__)

// Wrapper around glewInit
inline void GLEWInit() {
#ifndef __APPLE__
    // Force GLEW to use modern OpenGL method for checking function availability
    glewExperimental = GL_TRUE;
    // Initialize GLEW (must be called after OpenGL context is created and made current!)
    glewInit();
#endif
}

// Replace calls to glewInit with calls to GLEWInit
#undef glewInit
#define glewInit GLEWInit

#endif // GLINCLUDE_H
