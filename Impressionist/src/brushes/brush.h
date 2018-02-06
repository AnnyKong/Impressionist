/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef BRUSH_H
#define BRUSH_H

#include <glinclude.h>
#include <string>
#include <vectors.h>

class QWidget;
class QFormLayout;
class QLabeledSlider;
class QComboBox;

// Different types of brushes
enum class Brushes {
    Point,
    Line,
    Circle,
    ScatterPoint,
    ScatterLine,
    ScatterCircle,
    Star,
    TripleHex,
    WindWheel
};

// How to control the angle of the brush
enum class AngleMode {
    Direct,
    CursorMovement,
    Gradient
};

// How to get the color to draw with
enum class ColorMode {
    Solid,
    Sample
};

class Brush {
public:

    Brush(const std::string& name);
    glm::vec2 MousePre;
    glm::vec2 MouseCur;
    int previous_angle;
    glm::vec2 angle_control_start;
    glm::vec2 angle_control_end;
    QWidget* GetWidget() const;
    std::string GetName() const;
    int getOpacity();
    u_int previousAngle;
    virtual void SetSize(unsigned int size);
    virtual void SetO(unsigned int O);
    virtual void SetAngleMode(AngleMode angle_mode);
    virtual void SetColor(const glm::vec3& color);
    virtual void SetColorImage(const unsigned char* color_image, unsigned int width, unsigned int height);
    virtual void SetColorMode(ColorMode color_mode);
    virtual void SetAngleStart(glm::vec2 pos);
    virtual void SetAngleEnd(glm::vec2 pos);
    virtual void SetMousePre(glm::vec2 pos);
    virtual void SetMouseCur(glm::vec2 pos);
    // Must be called before drawing
    virtual void SetColorLocation(GLint color_location);
    virtual AngleMode getAngleMode();
    glm::vec4 GetColor(glm::ivec2 position = glm::ivec2(0, 0));
    virtual void SetAngle(int angle);
    // Called for drawing
    virtual void BrushBegin(const glm::vec2 pos) = 0;
    virtual void BrushMove(const glm::vec2 pos) = 0;
    virtual void BrushEnd(const glm::vec2 pos) = 0;


protected:
    QWidget* widget_;
    QFormLayout* layout_;
    std::string name_;
    QLabeledSlider* size_slider_;
    QLabeledSlider* o_slider_;
    QLabeledSlider* angle_slider_;
    AngleMode angle_mode_;
    ColorMode color_mode_;
    GLint color_location_;
    glm::vec3 color_;


    // Image to sample colors from
    const unsigned char* color_image_;
    unsigned int color_image_width_;
    unsigned int color_image_height_;

    // Called inside the BrushBegin/BrushMove/BrushEnd methods
    unsigned int GetSize() const;
    unsigned int GetAngle();

    void UseColor(const glm::vec4& color);
};

#endif // BRUSH_H
