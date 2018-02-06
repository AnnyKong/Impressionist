/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <glinclude.h>
#include <QMainWindow>
#include <QMessageBox>
#include <paintview.h>
#include <forms/filterkerneldialog.h>
#include <forms/bilateralmeandialog.h>
#include <forms/bilateralgaussdialog.h>
#include <forms/brushdialog.h>
#include <brushes/pointbrush.h>
#include <brushes/linesegmentbrush.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    const unsigned int DEFAULT_CANVAS_WIDTH = 500;
    const unsigned int DEFAULT_CANVAS_HEIGHT = 500;
    const unsigned int CANVAS_MARGIN = 50;
    const unsigned int CANVAS_SPACING = 6;

    static QString LastPath; // Last path accessed in the file dialog
    Ui::MainWindow *ui;

    PaintView* left_view_; // Reference View to sample from
    PaintView* right_view_; // Canvas to draw on

    // Dialogs
    BrushDialog* brush_dialog_;
    FilterKernelDialog* filter_kernel_dialog_;
    BilateralMeanDialog* bilat_mean_dialog_;
    BilateralGaussDialog* bilat_gauss_dialog_;

    // Mouse tracking
    Qt::MouseButtons mouse_buttons_;

    // Copy of the reference image
    unsigned char* reference_image_;
    unsigned int reference_image_width_;
    unsigned int reference_image_height_;

    // Overlay Brushes
    PointBrush marker_brush_;
    LineSegmentBrush angle_indicator_brush_;

    // Single-shot initialization
    void InitializeContext();
    void CreateActions();
    void CreateMenus();

    // Physically resizes the two paintview widgets and MainWindow to contain them
    void ResizeCanvases(unsigned int width, unsigned int height);

    void CanvasMousePressed(QMouseEvent* event);
    void CanvasMouseMoved(QMouseEvent* event);
    void CanvasMouseReleased(QMouseEvent* event);

    int GradientAngleSet(float x, float y);
};

#endif // MAINWINDOW_H
