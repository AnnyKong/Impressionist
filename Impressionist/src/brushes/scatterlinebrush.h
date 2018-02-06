/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef SCATTERLINEBRUSH_H
#define SCATTERLINEBRUSH_H

#include <brushes/brush.h>

class ScatterLineBrush : public Brush {
public:
    ScatterLineBrush(const std::string& name);

    virtual void SetRadious(unsigned int rad);
    virtual void SetDensity(unsigned int density);
    virtual void SetThickness(unsigned int thickness);
    virtual void BrushBegin(const glm::vec2 pos) override;
    virtual void BrushMove(const glm::vec2 pos) override;
    virtual void BrushEnd(const glm::vec2 pos) override;

protected:
    QLabeledSlider* radious_slider_;
    QLabeledSlider* density_slider_;
    QLabeledSlider* thickness_slider_;
    unsigned int GetRadious() const;
    unsigned int GetDensity() const;
    unsigned int GetThickness() const;
};

#endif // SCATTERLINEBRUSH_H
