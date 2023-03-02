/*=========================================================================

  Name:        qvMainWindow.cpp

  Author:      David Borland

  Description: The main window for the QuIKVis program.

=========================================================================*/


#include "qvMainWindow.h"

#include "qvGLWidget.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>

#include <iostream>


// Constructor
qvMainWindow::qvMainWindow(QWidget* parent) : QMainWindow(parent) {
    // Create the GUI from the Qt Designer file
    setupUi(this);

    // Add generate bones radio buttons to same group
    QButtonGroup* generateBonesGroup = new QButtonGroup(this);
    generateBonesGroup->addButton(evenLengthsRadioButton);
    generateBonesGroup->addButton(alternatingLengthsRadioButton);
    generateBonesGroup->addButton(decreasingLengthsRadioButton);
    generateBonesGroup->addButton(increasingLengthsRadioButton);
    generateBonesGroup->addButton(randomLengthsRadioButton);

    // Set up the OpenGL widget
    glWidget = new qvGLWidget(this);
    glWidgetLayout->addWidget(glWidget);

    // Set the GUI from the OpenGL widget settings
    RefreshGUI();
}

// Destructor
qvMainWindow::~qvMainWindow() {
}


// Menu
void qvMainWindow::on_actionSaveScreenShot_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Screen Shot", "", "PNG Images (*.png)");

    QImage image = glWidget->grabFrameBuffer();
    image.save(fileName, "PNG", 100);
}


// Appearance 


// Buttons
void qvMainWindow::on_showOverlapCheckBox_toggled(bool checked) {
    glWidget->SetShowOverlap(checked);
}


void qvMainWindow::on_initialCentersRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetCentersType(qvGLWidget::InitialCenters);
    }
}

void qvMainWindow::on_progressiveCentersRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetCentersType(qvGLWidget::ProgressiveCenters);
    }
}

void qvMainWindow::on_targetOnlyCheckBox_toggled(bool checked) {
    glWidget->SetTargetOnlyOverlap(checked);
}


void qvMainWindow::on_showBonesCheckBox_toggled(bool checked) {
    glWidget->SetShowBones(checked);
}

void qvMainWindow::on_showEndJointCheckBox_toggled(bool checked) {
    glWidget->SetShowEndJoint(checked);
}

void qvMainWindow::on_boneUseColorCheckBox_toggled(bool checked) {
    glWidget->SetBoneUseColor(checked);
}


void qvMainWindow::on_rgbRepeatRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointColorsType(qvGLWidget::RGBRepeatColors);
    }
}

void qvMainWindow::on_rainbowRepeatRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointColorsType(qvGLWidget::RainbowRepeatColors);
    }
}

void qvMainWindow::on_blackBodyRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointColorsType(qvGLWidget::BlackBodyColors);
    }
}

void qvMainWindow::on_greyScaleRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointColorsType(qvGLWidget::GreyScaleColors);
    }
}

void qvMainWindow::on_randomColorsRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointColorsType(qvGLWidget::RandomColors);
    }

    randomColorsButton->setEnabled(checked);
}


void qvMainWindow::on_randomColorsButton_clicked() {
    glWidget->SetJointColorsType(qvGLWidget::RandomColors);
}


void qvMainWindow::on_backgroundColorButton_clicked() {
    QColor color = QColorDialog::getColor(glWidget->GetBackgroundColor(), this, "Select Background Color");

    if (color.isValid()) {
        glWidget->SetBackgroundColor(color);
        setBackgroundColor(color);
    }
}


// Sliders
void qvMainWindow::on_overlapLineWidthSlider_valueChanged(int value) {
    glWidget->SetOverlapLineWidth((double)value / 10.0);
}

void qvMainWindow::on_overlapSmoothWidthSlider_valueChanged(int value) {
    glWidget->SetOverlapSmoothWidth((double)value / 10.0);
}

void qvMainWindow::on_overlapIntersectionAlphaSlider_valueChanged(int value) {
    glWidget->SetOverlapIntersectionAlpha((double)value / 100.0);
}

void qvMainWindow::on_overlapCircleAlphaSlider_valueChanged(int value) {
    glWidget->SetOverlapCircleAlpha((double)value / 100.0);
}

void qvMainWindow::on_overlapMinCircleDashSizeSlider_valueChanged(int value) {
    glWidget->SetOverlapMinCircleDashSize(10.0 / (double)value);
}


void qvMainWindow::on_boneLineWidthSlider_valueChanged(int value) {
    glWidget->SetBoneLineWidth((double)value / 10.0);
}

void qvMainWindow::on_boneSmoothWidthSlider_valueChanged(int value) {
    glWidget->SetBoneSmoothWidth((double)value / 10.0);
}

void qvMainWindow::on_boneJointRadiusSlider_valueChanged(int value) {
    glWidget->SetBoneJointRadius((double)value / 10.0);
}


// Algorithm


// Buttons
void qvMainWindow::on_evenLengthsRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetGenerateBonesType(qvGLWidget::EvenLengths);
    }

    evenLengthsButton->setEnabled(checked);
}

void qvMainWindow::on_alternatingLengthsRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetGenerateBonesType(qvGLWidget::AlternatingLengths);
    }

    alternatingLengthsButton->setEnabled(checked);
}

void qvMainWindow::on_decreasingLengthsRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetGenerateBonesType(qvGLWidget::DecreasingLengths);
    }

    decreasingLengthsButton->setEnabled(checked);
}

void qvMainWindow::on_increasingLengthsRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetGenerateBonesType(qvGLWidget::IncreasingLengths);
    }

    increasingLengthsButton->setEnabled(checked);
}

void qvMainWindow::on_randomLengthsRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetGenerateBonesType(qvGLWidget::RandomLengths);
    }

    randomLengthsButton->setEnabled(checked);
}


void qvMainWindow::on_evenLengthsButton_clicked() {
    glWidget->SetGenerateBonesType(qvGLWidget::EvenLengths);
}

void qvMainWindow::on_alternatingLengthsButton_clicked() {
    glWidget->SetGenerateBonesType(qvGLWidget::AlternatingLengths);
}

void qvMainWindow::on_decreasingLengthsButton_clicked() {
    glWidget->SetGenerateBonesType(qvGLWidget::DecreasingLengths);
}

void qvMainWindow::on_increasingLengthsButton_clicked() {
    glWidget->SetGenerateBonesType(qvGLWidget::IncreasingLengths);
}

void qvMainWindow::on_randomLengthsButton_clicked() {
    glWidget->SetGenerateBonesType(qvGLWidget::RandomLengths);
}


void qvMainWindow::on_jointOrderIncreasingRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointOrderType(QuIK::IncreasingJointOrder);
    }
}

void qvMainWindow::on_jointOrderDecreasingRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointOrderType(QuIK::DecreasingJointOrder);
    }
}

void qvMainWindow::on_jointOrderDividingRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointOrderType(QuIK::DividingJointOrder);
    }
}


void qvMainWindow::on_jointPlacementTriangulationRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointPlacementType(QuIK::TriangulationJointPlacement);
    }
}

void qvMainWindow::on_jointPlacementInertialRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetJointPlacementType(QuIK::InertialJointPlacement);
    }
}


void qvMainWindow::on_lockTargetNoneRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetLockTargetType(qvGLWidget::LockTargetNone);
    }
}

void qvMainWindow::on_lockTargetXRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetLockTargetType(qvGLWidget::LockTargetX);
    }
}

void qvMainWindow::on_lockTargetYRadioButton_toggled(bool checked) {
    if (checked) {
        glWidget->SetLockTargetType(qvGLWidget::LockTargetY);
    }
}


// Sliders
void qvMainWindow::on_numBonesSlider_valueChanged(int value) {
    glWidget->SetGenerateBonesNumber(value);
}
void qvMainWindow::on_boneLengthSlider_valueChanged(int value) {
    glWidget->SetGenerateBonesLength((double)value / 10.0); 
}


void qvMainWindow::RefreshGUI() {
    // Set background color
    setBackgroundColor(glWidget->GetBackgroundColor());


    // Appearance


    // Set GUI widgets from glWidget state
    showOverlapCheckBox->setChecked(glWidget->GetShowOverlap());
    showBonesCheckBox->setChecked(glWidget->GetShowBones());
    showEndJointCheckBox->setChecked(glWidget->GetShowEndJoint());

    initialCentersRadioButton->setChecked(false);
    progressiveCentersRadioButton->setChecked(false);
    if (glWidget->GetCentersType() == qvGLWidget::InitialCenters) {
        initialCentersRadioButton->setChecked(true);
    }
    else if (glWidget->GetCentersType() == qvGLWidget::ProgressiveCenters) {
        progressiveCentersRadioButton->setChecked(true);
    }


    targetOnlyCheckBox->setChecked(glWidget->GetTargetOnlyOverlap());

    boneUseColorCheckBox->setChecked(glWidget->GetBoneUseColor());


    rgbRepeatRadioButton->setChecked(false);
    rainbowRepeatRadioButton->setChecked(false);
    blackBodyRadioButton->setChecked(false);
    greyScaleRadioButton->setChecked(false);
    randomColorsRadioButton->setChecked(false);
    if (glWidget->GetJointColorsType() == qvGLWidget::RGBRepeatColors) {
        rgbRepeatRadioButton->setChecked(true);
    }    
    else if (glWidget->GetJointColorsType() == qvGLWidget::RainbowRepeatColors) {
        rainbowRepeatRadioButton->setChecked(true);
    }
    else if (glWidget->GetJointColorsType() == qvGLWidget::BlackBodyColors) {
        blackBodyRadioButton->setChecked(true);
    }
    else if (glWidget->GetJointColorsType() == qvGLWidget::GreyScaleColors) {
        greyScaleRadioButton->setChecked(true);
    }
    else if (glWidget->GetJointColorsType() == qvGLWidget::RandomColors) {
        randomColorsRadioButton->setChecked(true);
    }

    randomColorsButton->setEnabled(randomColorsRadioButton->isChecked());


    // Need to block and unblock signals because the shaders in the GLWidget are only
    // created in the initializeGL() function

    overlapLineWidthSlider->blockSignals(true);
    overlapLineWidthSlider->setValue(glWidget->GetOverlapLineWidth() * 10);
    overlapLineWidthSlider->blockSignals(false);

    overlapSmoothWidthSlider->blockSignals(true);
    overlapSmoothWidthSlider->setValue(glWidget->GetOverlapSmoothWidth() * 10);
    overlapSmoothWidthSlider->blockSignals(false);

    overlapIntersectionAlphaSlider->blockSignals(true);
    overlapIntersectionAlphaSlider->setValue(glWidget->GetOverlapIntersectionAlpha() * 100);
    overlapIntersectionAlphaSlider->blockSignals(false);

    overlapCircleAlphaSlider->blockSignals(true);
    overlapCircleAlphaSlider->setValue(glWidget->GetOverlapCircleAlpha() * 100);
    overlapCircleAlphaSlider->blockSignals(false);

    overlapMinCircleDashSizeSlider->blockSignals(true);
    overlapMinCircleDashSizeSlider->setValue(10.0 / glWidget->GetOverlapMinCircleDashSize());
    overlapMinCircleDashSizeSlider->blockSignals(false);


    boneLineWidthSlider->blockSignals(true);
    boneLineWidthSlider->setValue(glWidget->GetBoneLineWidth() * 10);
    boneLineWidthSlider->blockSignals(false);

    boneSmoothWidthSlider->blockSignals(true);
    boneSmoothWidthSlider->setValue(glWidget->GetBoneSmoothWidth() * 10);
    boneSmoothWidthSlider->blockSignals(false);

    boneJointRadiusSlider->blockSignals(true);
    boneJointRadiusSlider->setValue(glWidget->GetBoneJointRadius() * 10);
    boneJointRadiusSlider->blockSignals(false);


    // Algorithm
    evenLengthsRadioButton->setChecked(false);
    alternatingLengthsRadioButton->setChecked(false);
    decreasingLengthsRadioButton->setChecked(false);
    increasingLengthsRadioButton->setChecked(false);
    randomLengthsRadioButton->setChecked(false);
    if (glWidget->GetGenerateBonesType() == qvGLWidget::EvenLengths) {
        evenLengthsRadioButton->setChecked(true);
    }    
    else if (glWidget->GetGenerateBonesType() == qvGLWidget::AlternatingLengths) {
        alternatingLengthsRadioButton->setChecked(true);
    } 
    else if (glWidget->GetGenerateBonesType() == qvGLWidget::DecreasingLengths) {
        decreasingLengthsRadioButton->setChecked(true);
    }    
    else if (glWidget->GetGenerateBonesType() == qvGLWidget::IncreasingLengths) {
        increasingLengthsRadioButton->setChecked(true);
    }    
    else if (glWidget->GetGenerateBonesType() == qvGLWidget::RandomLengths) {
        randomLengthsRadioButton->setChecked(true);
    }

    evenLengthsButton->setEnabled(evenLengthsRadioButton->isChecked());
    alternatingLengthsButton->setEnabled(alternatingLengthsRadioButton->isChecked());
    decreasingLengthsButton->setEnabled(decreasingLengthsRadioButton->isChecked());
    increasingLengthsButton->setEnabled(increasingLengthsRadioButton->isChecked());
    randomLengthsButton->setEnabled(randomLengthsRadioButton->isChecked());


    numBonesSlider->setValue(glWidget->GetGenerateBonesNumber());
    boneLengthSlider->setValue(glWidget->GetGenerateBonesLength() * 10);

    jointOrderIncreasingRadioButton->setChecked(false);
    jointOrderDecreasingRadioButton->setChecked(false);
    jointOrderDividingRadioButton->setChecked(false);
    if (glWidget->GetJointOrderType() == QuIK::IncreasingJointOrder) {
        jointOrderIncreasingRadioButton->setChecked(true);
    }    
    else if (glWidget->GetJointOrderType() == QuIK::DecreasingJointOrder) {
        jointOrderDecreasingRadioButton->setChecked(true);
    } 
    else if (glWidget->GetJointOrderType() == QuIK::DividingJointOrder) {
        jointOrderDividingRadioButton->setChecked(true);
    }

    jointPlacementTriangulationRadioButton->setChecked(false);
    jointPlacementInertialRadioButton->setChecked(false);
    if (glWidget->GetJointOrderType() == QuIK::TriangulationJointPlacement) {
        jointPlacementTriangulationRadioButton->setChecked(true);
    }    
    else if (glWidget->GetJointOrderType() == QuIK::InertialJointPlacement) {
        jointPlacementInertialRadioButton->setChecked(true);
    }

    lockTargetNoneRadioButton->setChecked(false);
    lockTargetXRadioButton->setChecked(false);
    lockTargetYRadioButton->setChecked(false);
    if (glWidget->GetLockTargetType() == qvGLWidget::LockTargetNone) {
        lockTargetNoneRadioButton->setChecked(true);
    }    
    else if (glWidget->GetJointOrderType() == qvGLWidget::LockTargetX) {
        lockTargetXRadioButton->setChecked(true);
    }   
    else if (glWidget->GetJointOrderType() == qvGLWidget::LockTargetY) {
        lockTargetYRadioButton->setChecked(true);
    }
}