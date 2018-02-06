/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef QLABELEDSLIDER_H
#define QLABELEDSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>

// Simple wrapper around a QSlider that also has a QSpinbox showing its numerical value
class QLabeledSlider : public QWidget {
public:
    // Either Horizontal or Vertical orientation
    QLabeledSlider(Qt::Orientation orientation = Qt::Horizontal, QWidget* parent = Q_NULLPTR);
    QSlider& GetSlider() const;
    void SetRange(int min, int max);
    void SetValue(int value);
    int GetValue() const;
private:
    QSlider* slider_;
    QSpinBox* label_;
};

#endif // QLABELEDSLIDER_H
