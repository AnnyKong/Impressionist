/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <brushes/brush.h>
#include <filters/filter.h>
#include <assert.h>
#include <QScrollArea>
#include <QOffscreenSurface>
#include <QMouseEvent>
#include <QFileDialog>
#include <QDebug>
#include <QTimer>
#include <math.h>
#include <inttypes.h>




QString MainWindow::LastPath = QDir::currentPath();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    left_view_(nullptr),
    right_view_(nullptr),
    brush_dialog_(nullptr),
    filter_kernel_dialog_(nullptr),
    bilat_mean_dialog_(nullptr),
    bilat_gauss_dialog_(nullptr),
    mouse_buttons_(0),
    reference_image_(nullptr),
    reference_image_width_(0),
    reference_image_height_(0),
    marker_brush_("Marker Brush"),
    angle_indicator_brush_("Angle Indicator Brush")
{
    ui->setupUi(this);
    setWindowTitle(tr("Impressionist"));

    InitializeContext();
    // Must be initialized after InitializeContext
    left_view_ = new PaintView(this);
    right_view_ = new PaintView(this);
    // Must be initialized after PaintViews are constructed
    brush_dialog_ = new BrushDialog(this);
    filter_kernel_dialog_ = new FilterKernelDialog(*right_view_, this);
    bilat_mean_dialog_ = new BilateralMeanDialog(*right_view_, this);
    bilat_gauss_dialog_ = new BilateralGaussDialog(*right_view_, this);

    CreateActions();
    CreateMenus();

    QScrollArea* scroll_area_left_ = new QScrollArea;
    scroll_area_left_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    scroll_area_left_->setBackgroundRole(QPalette::Dark);
    scroll_area_left_->setWidget(left_view_);

    QScrollArea* scroll_area_right_ = new QScrollArea;
    scroll_area_right_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    scroll_area_right_->setBackgroundRole(QPalette::Dark);
    scroll_area_right_->setWidget(right_view_);

    ui->horizontalLayout->setSpacing(CANVAS_SPACING);
    ui->horizontalLayout->addWidget(scroll_area_left_);
    ui->horizontalLayout->addWidget(scroll_area_right_);

    // Default Size
    ResizeCanvases(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);

    // Connect mouse events
    connect(right_view_, &PaintView::MousePress, this, &MainWindow::CanvasMousePressed);
    connect(right_view_, &PaintView::MouseMove, this, &MainWindow::CanvasMouseMoved);
    connect(right_view_, &PaintView::MouseRelease, this, &MainWindow::CanvasMouseReleased);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::InitializeContext() {
    QOpenGLContext* context = new QOpenGLContext();
    context->setShareContext(QOpenGLContext::globalShareContext());
    assert(context->create());

    QOffscreenSurface* surface = new QOffscreenSurface();
    surface->create();
    assert(surface->isValid());
    context->makeCurrent(surface);

    glewInit();
}

void MainWindow::CreateActions() {
    // Load reference image
    connect(ui->load_ref_action, &QAction::triggered, this, [this](){
	
        // if reference image is loaded, prompt user to save or discard current image
        if (reference_image_ != nullptr) {

            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Image may have been modified.");
            msgBox.setInformativeText("Do you want to save changes before opening new reference image?");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Save);

            switch(msgBox.exec()) {
            case QMessageBox::Save: {
                // Save image to file, then open new image file
                QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), MainWindow::LastPath, "Image Files (*.jpg | *.jpeg | *.png | *.bmp)");
                if (!filename.isNull() && !filename.isEmpty()) {
                    MainWindow::LastPath = QFileInfo(filename).path();
                    auto snapshot = right_view_->GetSnapshot();
                    QImage image(snapshot->Bytes, snapshot->Width, snapshot->Height, QImage::Format_RGBA8888);
                    image.mirrored().save(filename);
                }
                break;
            }
            case QMessageBox::Discard:
                // No save, just discard any changes and open new image file
                break;
            case QMessageBox::Cancel:
                // do nothing
                return;
            }
        }

        QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), MainWindow::LastPath, "Image Files (*.jpg | *.jpeg | *.png | *.bmp)");
        if (!filename.isNull() && !filename.isEmpty()) {
            MainWindow::LastPath = QFileInfo(filename).path();

            // Load the image from file into RGBA32 format
            QImage unconverted(filename);
            QImage image = unconverted.convertToFormat(QImage::Format_RGBA8888);
            unsigned int width = image.width();
            unsigned int height = image.height();

            // Make sure we were able to load the image
            if (image.isNull()) {
                qDebug() << "Failed to import image \"" << filename << "\"";
                return;
            }

            // Keep a copy of the reference image
            if (reference_image_ != nullptr) delete[] reference_image_;
            reference_image_ = new unsigned char[image.byteCount()];
            memcpy(reference_image_, image.bits(), image.byteCount());
            reference_image_width_ = width;
            reference_image_height_ = height;

            // Construct the left and right hand side views
            left_view_->Setup(width, height);
            left_view_->CreateLayer(PaintView::BASE_LAYER);
            left_view_->CreateLayer(PaintView::OVERLAY_LAYER);

            right_view_->Setup(width, height);
            right_view_->CreateLayer(PaintView::BASE_LAYER);
            right_view_->CreateLayer(PaintView::OVERLAY_LAYER);

            ResizeCanvases(width, height);

            left_view_->SetCurrentLayer(PaintView::BASE_LAYER);
            left_view_->DrawImage(image.bits(), width, height, true);

            right_view_->SetCurrentLayer(PaintView::BASE_LAYER);
            right_view_->Clear(PaintView::RGBA_WHITE);

            // Enable the UI
            ui->save_canvas_action->setEnabled(true);
            ui->clear_canvas_action->setEnabled(true);
            ui->copy_ref_action->setEnabled(true);
            ui->select_brush_action->setEnabled(true);
            ui->filter_kernel_action->setEnabled(true);
            ui->gaussian_blur_action->setEnabled(true);
            ui->bilat_mean_action->setEnabled(true);
            ui->bilat_gauss_action->setEnabled(true);
        }
    });

    // Save Canvas
    connect(ui->save_canvas_action, &QAction::triggered, this, [this](){
        QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), MainWindow::LastPath, "Image Files (*.jpg | *.jpeg | *.png | *.bmp)");
        if (!filename.isNull() && !filename.isEmpty()) {
            MainWindow::LastPath = QFileInfo(filename).path();
            auto snapshot = right_view_->GetSnapshot();
            QImage image(snapshot->Bytes, snapshot->Width, snapshot->Height, QImage::Format_RGBA8888);
            image.mirrored().save(filename);
        }
    });

    // Clear Canvas
    connect(ui->clear_canvas_action, &QAction::triggered, this, [this](){
        right_view_->SetCurrentLayer(PaintView::BASE_LAYER);
        right_view_->Clear(PaintView::RGBA_WHITE);
        right_view_->update();
    });

    // Copy Reference image to Canvas
    connect(ui->copy_ref_action, &QAction::triggered, this, [this](){
        right_view_->SetCurrentLayer(PaintView::BASE_LAYER);
        right_view_->DrawImage(reference_image_, reference_image_width_, reference_image_height_, true);
        right_view_->update();
    });

    // Brushes Dialog
    connect(ui->select_brush_action, &QAction::triggered, this, [this]() {
        brush_dialog_->show();
    });

    // Filters
    connect(ui->filter_kernel_action, &QAction::triggered, this, [this](){
        filter_kernel_dialog_->exec();
    });

    connect(ui->gaussian_blur_action, &QAction::triggered, this, [this](){
        auto snapshot = right_view_->GetSnapshot();
        // Allocate space for the filtered image
        unsigned int width = right_view_->GetWidth();
        unsigned int height = right_view_->GetHeight();
        RGBABuffer filtered(width, height);
        // Apply the Filter
        Filter::ApplyGaussianBlur(snapshot->Bytes, filtered.Bytes, width, height);
        right_view_->DrawImage(filtered.Bytes, width, height);
    });

    connect(ui->bilat_mean_action, &QAction::triggered, this, [this](){
        bilat_mean_dialog_->exec();
    });

    connect(ui->bilat_gauss_action, &QAction::triggered, this, [this](){
        bilat_gauss_dialog_->exec();
    });
}

void MainWindow::CreateMenus() {

}

void MainWindow::ResizeCanvases(unsigned int width, unsigned int height) {
    left_view_->setFixedSize(width, height);
    left_view_->update();
    right_view_->setFixedSize(width, height);
    right_view_->update();
    resize(left_view_->frameSize().width() + right_view_->frameSize().width() + CANVAS_MARGIN + CANVAS_SPACING,
           left_view_->frameSize().height() + CANVAS_MARGIN + ui->menu_bar->frameSize().height());
}

void MainWindow::CanvasMousePressed(QMouseEvent *event) {
    float pos_x = event->pos().x();
    float pos_y = event->pos().y();
    glm::vec2 pos = glm::vec2(pos_x, pos_y);
    mouse_buttons_ = event->buttons();
    Brush& current_brush = brush_dialog_->GetCurrentBrush();
    if (mouse_buttons_.testFlag(Qt::LeftButton)) {
        // Sample the Color from the reference image       
        current_brush.SetColorMode(ColorMode::Sample);
        current_brush.SetColorImage(reference_image_, reference_image_width_, reference_image_height_);
        right_view_->SetCurrentLayer(PaintView::BASE_LAYER);
        current_brush.SetAngleMode(brush_dialog_->GetCurrentAngleControl());
        if(current_brush.getAngleMode() == AngleMode::CursorMovement){
            //current_brush.previous_angle = current_brush.GetAngle();
            current_brush.SetAngle(180);
        }else if(current_brush.getAngleMode() == AngleMode::Gradient){
            int angle = GradientAngleSet(pos.x, pos.y);
            current_brush.SetAngle(angle);
        }
        right_view_->DrawBegin(current_brush, pos);
        // REQUIREMENT: Set brush angle if needed.
        current_brush.SetMousePre(pos);
    } else if (mouse_buttons_.testFlag(Qt::RightButton)) {
        right_view_->SetCurrentLayer(PaintView::OVERLAY_LAYER);
        angle_indicator_brush_.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
        right_view_->DrawBegin(angle_indicator_brush_, pos);
        current_brush.SetAngleStart(pos);
    }
}

void MainWindow::CanvasMouseMoved(QMouseEvent *event) {
    float pos_x = event->pos().x();
    float pos_y = event->pos().y();
    glm::vec2 pos = glm::vec2(pos_x, pos_y);
    mouse_buttons_ = event->buttons();

    if (mouse_buttons_.testFlag(Qt::LeftButton)) {
        // Sample the Color from the reference image
        Brush& current_brush = brush_dialog_->GetCurrentBrush();
        current_brush.SetColorMode(ColorMode::Sample);
        current_brush.SetColorImage(reference_image_, reference_image_width_, reference_image_height_);
        right_view_->SetCurrentLayer(PaintView::BASE_LAYER);
       // right_view_->DrawMove(current_brush, pos);
        // REQUIREMENT: Set brush angle if needed.
        current_brush.SetMouseCur(pos);
        current_brush.SetAngleMode(brush_dialog_->GetCurrentAngleControl());
        if(current_brush.getAngleMode() == AngleMode::CursorMovement){
            float preX = current_brush.MousePre.x;
            float preY = current_brush.MousePre.y;
            int preAngle = current_brush.previous_angle;
            GLfloat radianAngle = atan2(pos.y - preY, pos.x - preX);
            radianAngle += M_PI;
            int angleChange = (int)((radianAngle*180/M_PI)- preAngle);
            if(abs(angleChange)>180) {
                if(angleChange >0){
                    angleChange = angleChange - 360;
                } else {
                    angleChange = 360 + angleChange;
                }
            }
            preAngle += (int)(0.3 * angleChange);
            //std::cout << radianAngle << " " << angle << " " << angleChange << std::endl;
            preAngle = preAngle > 0 ? preAngle % 360 : preAngle + 360;
            current_brush.previous_angle = preAngle;
            current_brush.SetAngle(preAngle);
        } else if (current_brush.getAngleMode() == AngleMode::Gradient) {
            int angle = GradientAngleSet(pos.x, pos.y);
            current_brush.SetAngle(angle);
        }
        right_view_->DrawMove(current_brush, pos);
        current_brush.SetMousePre(pos);

    } else if (mouse_buttons_.testFlag(Qt::RightButton)) {
        right_view_->SetCurrentLayer(PaintView::OVERLAY_LAYER);
        right_view_->Clear(PaintView::RGBA_TRANSPARENT);
        angle_indicator_brush_.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
        right_view_->DrawMove(angle_indicator_brush_, pos);
    }

    // EXTRA CREDIT: Draw an overlay marker on the left view
}

void MainWindow::CanvasMouseReleased(QMouseEvent *event) {
    float pos_x = event->pos().x();
    float pos_y = event->pos().y();
    glm::vec2 pos = glm::vec2(pos_x, pos_y);
    Brush& current_brush = brush_dialog_->GetCurrentBrush();
    if (mouse_buttons_.testFlag(Qt::LeftButton)) {
        // Sample the Color from the reference image

        current_brush.SetColorMode(ColorMode::Sample);
        current_brush.SetColorImage(reference_image_, reference_image_width_, reference_image_height_);
        right_view_->SetCurrentLayer(PaintView::BASE_LAYER);
        right_view_->DrawEnd(current_brush, pos);
    }

    if (mouse_buttons_.testFlag(Qt::RightButton)) {
        right_view_->SetCurrentLayer(PaintView::OVERLAY_LAYER);
        right_view_->Clear(PaintView::RGBA_TRANSPARENT);
        // REQUIREMENT: Set brush angle if needed.
        current_brush.SetAngleEnd(pos);
    }

    // Reset mouse buttons state
    mouse_buttons_ = 0;
}

int MainWindow::GradientAngleSet(float pos_x, float pos_y) {
    Brush& current_brush = brush_dialog_->GetCurrentBrush();

    const int sobel_x[3][3] = {{-1, 0, 1},
                               {-2, 0, 2},
                               {-1, 0, 1}};
    const int sobel_y[3][3] = {{-1, -2, -1},
                               {0, 0, 0},
                               {1, 2, 1}};
    float color_grid[3][3];
    for(int row = 0; row < 3; row++){
        for(int col = 0; col < 3; col++){
            u_int x = pos_x + (col - 1);
            u_int y = pos_y + (row - 1);
            glm::vec2 xy = glm::vec2(x, y);
            glm::vec4 center_color = current_brush.GetColor(xy);
            color_grid[row][col] = center_color.r * 0.299 + center_color.g * 0.587 + center_color.b * 0.114;
        }
    }
    float gx = 0;
    float gy = 0;
    for(int row = 0; row < 3; row++) {
        for(int col = 0; col <3; col++) {
            gx += (sobel_x[row][col]) * color_grid[row][col];
            gy += (sobel_y[row][col]) * color_grid[row][col];
        }
    }
    float radianangle = atan2(gy,gx);
    int angle = (int)((radianangle + 1.5 * M_PI)*180/M_PI);
    return angle;
}
