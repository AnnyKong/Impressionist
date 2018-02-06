/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef BILATERALGAUSSDIALOG_H
#define BILATERALGAUSSDIALOG_H

#include <rgbabuffer.h>
#include <memory>
#include <QDialog>
#include <QTimer>

class PaintView;

namespace Ui {
    class BilateralGaussDialog;
}

class BilateralGaussDialog : public QDialog {
    Q_OBJECT
public:
    explicit BilateralGaussDialog(PaintView& paint_view, QWidget *parent = 0);
    ~BilateralGaussDialog();
    virtual int exec() override;

private:
    Ui::BilateralGaussDialog *ui;
    PaintView* paint_view_;
    std::unique_ptr<RGBABuffer> original_image_;
    QTimer preview_timer_;

    // Applies the filter to the paint view
    void Preview();

    // Resets the UI controls to their default state
    void Reset();
};

#endif // BILATERALGAUSSDIALOG_H
