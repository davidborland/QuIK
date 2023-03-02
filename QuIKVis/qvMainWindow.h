/*=========================================================================

  Name:        qvMainWindow.h

  Author:      David Borland

  Description: The main window for the QuIKVis program.

=========================================================================*/


#ifndef QVMAINWINDOW_H
#define QVMAINWINDOW_H


#include <qmainwindow.h>

#include "ui_qvMainWindow.h"

class qvGLWidget;


class qvMainWindow : public QMainWindow, private Ui_MainWindow {
    Q_OBJECT

public:
    // Constructor/destructor
    qvMainWindow(QWidget* parent = NULL);
    virtual ~qvMainWindow();

public slots:
    // Use Qt's auto-connect magic to tie GUI widgets to slots,
    // removing the need to call connect() explicitly.
    // Names of the methods must follow the naming convention
    // on_<widget name>_<signal name>(<signal parameters>).


    // Menu
    virtual void on_actionSaveScreenShot_triggered();


    // Appearance Tab


    // Buttons
    virtual void on_showOverlapCheckBox_toggled(bool checked);

    virtual void on_initialCentersRadioButton_toggled(bool checked);
    virtual void on_progressiveCentersRadioButton_toggled(bool checked);

    virtual void on_targetOnlyCheckBox_toggled(bool checked);

    virtual void on_showBonesCheckBox_toggled(bool checked);
    virtual void on_showEndJointCheckBox_toggled(bool checked);
    virtual void on_boneUseColorCheckBox_toggled(bool checked);

    virtual void on_rgbRepeatRadioButton_toggled(bool checked);
    virtual void on_rainbowRepeatRadioButton_toggled(bool checked);
    virtual void on_blackBodyRadioButton_toggled(bool checked);
    virtual void on_greyScaleRadioButton_toggled(bool checked);
    virtual void on_randomColorsRadioButton_toggled(bool checked);

    virtual void on_randomColorsButton_clicked();

    virtual void on_backgroundColorButton_clicked();

    // Sliders
    virtual void on_overlapLineWidthSlider_valueChanged(int value);
    virtual void on_overlapSmoothWidthSlider_valueChanged(int value);
    virtual void on_overlapIntersectionAlphaSlider_valueChanged(int value);
    virtual void on_overlapCircleAlphaSlider_valueChanged(int value);
    virtual void on_overlapMinCircleDashSizeSlider_valueChanged(int value);

    virtual void on_boneLineWidthSlider_valueChanged(int value);
    virtual void on_boneSmoothWidthSlider_valueChanged(int value);
    virtual void on_boneJointRadiusSlider_valueChanged(int value);


    // Algorithm Tab


    // Buttons
    virtual void on_evenLengthsRadioButton_toggled(bool checked);
    virtual void on_alternatingLengthsRadioButton_toggled(bool checked);
    virtual void on_decreasingLengthsRadioButton_toggled(bool checked);
    virtual void on_increasingLengthsRadioButton_toggled(bool checked);
    virtual void on_randomLengthsRadioButton_toggled(bool checked);

    virtual void on_evenLengthsButton_clicked();
    virtual void on_alternatingLengthsButton_clicked();
    virtual void on_decreasingLengthsButton_clicked();
    virtual void on_increasingLengthsButton_clicked();
    virtual void on_randomLengthsButton_clicked();

    virtual void on_jointOrderIncreasingRadioButton_toggled(bool checked);
    virtual void on_jointOrderDecreasingRadioButton_toggled(bool checked);
    virtual void on_jointOrderDividingRadioButton_toggled(bool checked);

    virtual void on_jointPlacementTriangulationRadioButton_toggled(bool checked);
    virtual void on_jointPlacementInertialRadioButton_toggled(bool checked);

    virtual void on_lockTargetNoneRadioButton_toggled(bool checked);
    virtual void on_lockTargetXRadioButton_toggled(bool checked);
    virtual void on_lockTargetYRadioButton_toggled(bool checked);

    // Sliders
    virtual void on_numBonesSlider_valueChanged(int value);
    virtual void on_boneLengthSlider_valueChanged(int value);

protected:
    void RefreshGUI();

    qvGLWidget* glWidget;
};


#endif
