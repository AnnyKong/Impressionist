/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef SCATTERCIRCLEBRUSH_H
#define SCATTERCIRCLEBRUSH_H

#include <brushes/brush.h>
#include <qlabeledslider.h>

class ScatterCircleBrush : public Brush {
public:
    ScatterCircleBrush(const std::string& name);

    virtual void SetRadious(unsigned int rad);
    virtual void SetDensity(unsigned int density);
    virtual void BrushBegin(const glm::vec2 pos) override;
    virtual void BrushMove(const glm::vec2 pos) override;
    virtual void BrushEnd(const glm::vec2 pos) override;

protected:
    QLabeledSlider* radious_slider_;
    QLabeledSlider* density_slider_;
    unsigned int GetRadious() const;
    unsigned int GetDensity() const;
};

#endif // SCATTERCIRCLEBRUSH_H
