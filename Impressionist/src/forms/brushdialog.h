/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef BRUSHDIALOG_H
#define BRUSHDIALOG_H

#include <brushes/brush.h>
#include <map>
#include <memory>
#include <QDialog>

namespace Ui {
    class BrushDialog;
}

class BrushDialog : public QDialog {
    Q_OBJECT

public:
    explicit BrushDialog(QWidget *parent = 0);
    ~BrushDialog();

    Brush& GetCurrentBrush();
    AngleMode GetCurrentAngleControl();

private:
    Ui::BrushDialog *ui;
    std::map<Brushes, std::unique_ptr<Brush>> brushes_;
    std::vector<Brushes> brush_choices_;
    std::vector<AngleMode> angle_choices_;
    std::map<Brushes, QWidget*> brush_widgets_;
    unsigned int current_brush_choice_;
    unsigned int current_angle_choice_;
};

#endif // BRUSHDIALOG_H
