/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef FILTERKERNELDIALOG_H
#define FILTERKERNELDIALOG_H

#include <rgbabuffer.h>
#include <memory>
#include <QDialog>
#include <QTimer>
#include <filters/filter.h>

class PaintView;

namespace Ui {
    class FilterKernelDialog;
}

class FilterKernelDialog : public QDialog {
    Q_OBJECT
public:
    explicit FilterKernelDialog(PaintView& paint_view, QWidget *parent = 0);
    ~FilterKernelDialog();
    virtual int exec() override;


private:
    const unsigned int KERNEL_WIDTH = 5;
    const unsigned int KERNEL_HEIGHT = 5;
    Ui::FilterKernelDialog *ui;
    PaintView* paint_view_;
    std::unique_ptr<RGBABuffer> original_image_;
    QTimer preview_timer_;

    // Applies the filter kernel to the paint view
    void Preview();

    // Returns the value of the spinbox in matrix column i, row j
    float GetKernelValue(int i, int j);

    // Resets the UI controls to their default state
    void Reset();
};

#endif // FILTERKERNELDIALOG_H
