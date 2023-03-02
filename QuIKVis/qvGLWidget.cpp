/*=========================================================================

  Name:        qvGLWidget.cpp

  Author:      David Borland

  Description: Handles drawing to an OpenGL buffer.

=========================================================================*/


#include "qvGLWidget.h"

#include <QtGui>
#include <QGLShader>

#include <iostream>
#include <fstream>


#define FORPAPER


qvGLWidget::qvGLWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    // Create the QuIK class
    ik = new QuIK();


    // Create some bones
    generateBonesType = EvenLengths;
    generateBonesNumber = 7;
#ifdef FORPAPER    
    generateBonesLength = 5.0;
#else
    generateBonesLength = 2.0;
#endif
    generateBonesSeed = 1;
    GenerateBones();


    // Solve initial IK
    ik->SetTargetJoint(generateBonesNumber - 1);
    ik->SetTarget(Vec3(generateBonesLength * generateBonesNumber, 0.0, 0.0));
    ik->SolveIK();


    // Set defaults
    jointColorsType = RGBRepeatColors;
    randomColorsSeed = 1;
#ifdef FORPAPER
    backgroundColor.setRgbF(1.0, 1.0, 1.0);
#else
    backgroundColor.setRgbF(0.95, 0.95, 0.9);
//    backgroundColor.setRgbF(0.9, 0.9, 0.8);
#endif
    ComputeColors();

    worldWidth = 100.0;
    worldCenter.MakeIdentity();

    showOverlap = true;
    showBones = true;
    showEndJoint = true;

    centersType = InitialCenters;
    targetOnlyOverlap = false;

#ifdef FORPAPER
    overlapLineWidth = 5.0f;
    overlapSmoothWidth = 1.5f;
    overlapIntersectionAlpha = 0.5f;
    overlapCircleAlpha = 0.1f;
    overlapMinCircleDashSize = 0.2f;

    boneUseColor = true;
    boneLineWidth = overlapLineWidth;
    boneSmoothWidth = overlapSmoothWidth;
    boneJointRadius = 10.0;
#else
    overlapLineWidth = 1.0f;
    overlapSmoothWidth = 1.5f;
    overlapIntersectionAlpha = 0.5f;
    overlapCircleAlpha = 0.1f;
    overlapMinCircleDashSize = 0.2f;

    boneUseColor = true;
    boneLineWidth = overlapLineWidth;
    boneSmoothWidth = overlapSmoothWidth;
    boneJointRadius = 4.0;
#endif

    lockTargetType = LockTargetNone;

    // QPainter settings
    setAutoFillBackground(false);
    setAutoBufferSwap(true);
}

qvGLWidget::~qvGLWidget() {
    // Clean up
    delete ik;

    delete lineShader;
    delete circleShader;
    delete circleFilledShader;
    delete overlapShader;
}


bool qvGLWidget::GetShowOverlap() {
    return showOverlap;
}

void qvGLWidget::SetShowOverlap(bool show) {
    showOverlap = show;

    // Update graphics
    updateGL();
}

void qvGLWidget::SetShowBones(bool show) {
    showBones = show;

    // Update graphics
    updateGL();
}

bool qvGLWidget::GetShowBones() {
    return showBones;
}


void qvGLWidget::SetShowEndJoint(bool show) {
    showEndJoint = show;

    // Update graphics
    updateGL();
}

bool qvGLWidget::GetShowEndJoint() {
    return showEndJoint;
}


qvGLWidget::CentersType qvGLWidget::GetCentersType() {
    return centersType;
}

void qvGLWidget::SetCentersType(CentersType type) {
    centersType = type;
    
    // Update graphics
    updateGL();
}


bool qvGLWidget::GetTargetOnlyOverlap() {
    return targetOnlyOverlap;
}

void qvGLWidget::SetTargetOnlyOverlap(bool targetOnly) {
    targetOnlyOverlap = targetOnly;

    // Update graphics
    updateGL();
}


void qvGLWidget::SetOverlapLineWidth(double width) {
    // Set the new overlap line width
    overlapLineWidth = width;

    // Set shader variable
    overlapShader->bind();
    overlapShader->setUniformValue("lineWidth", overlapLineWidth);
    overlapShader->release();
    
    // Update graphics
    updateGL();
}

void qvGLWidget::SetOverlapSmoothWidth(double width) {
    // Set the new overlap smooth width
    overlapSmoothWidth = width;

    // Set shader variable
    overlapShader->bind();
    overlapShader->setUniformValue("smoothWidth", overlapSmoothWidth);
    overlapShader->release();
    
    // Update graphics
    updateGL();
}

void qvGLWidget::SetOverlapIntersectionAlpha(double alpha) {
    // Set the new overlap intersection alpha
    overlapIntersectionAlpha = alpha;

    // Set shader variable
    overlapShader->bind();
    overlapShader->setUniformValue("intersectionAlpha", overlapIntersectionAlpha);
    overlapShader->release();
    
    // Update graphics
    updateGL();
}

void qvGLWidget::SetOverlapCircleAlpha(double alpha) {
    // Set the new overlap circle alpha
    overlapCircleAlpha = alpha;

    // Set shader variable
    overlapShader->bind();
    overlapShader->setUniformValue("circleAlpha", overlapCircleAlpha);
    overlapShader->release();
    
    // Update graphics
    updateGL();
}

void qvGLWidget::SetOverlapMinCircleDashSize(double size) {
    // Set the new overlap min circle dash size
    overlapMinCircleDashSize = size;

    // Set shader variable
    overlapShader->bind();
    overlapShader->setUniformValue("minCircleDashSize", overlapMinCircleDashSize);
    overlapShader->release();
    
    // Update graphics
    updateGL();
}


void qvGLWidget::SetBoneLineWidth(double width) {
    // Set the new overlap line width
    boneLineWidth = width;

    // Set the OpenGL line width
    glLineWidth(boneLineWidth);

    // Set shader variables
    lineShader->bind();
    lineShader->setUniformValue("lineWidth", boneLineWidth);
    lineShader->release();

    circleShader->bind();
    circleShader->setUniformValue("lineWidth", boneLineWidth);
    circleShader->release();

    circleFilledShader->bind();
    circleFilledShader->setUniformValue("lineWidth", boneLineWidth);
    circleFilledShader->release();
    
    // Update graphics
    updateGL();
}

void qvGLWidget::SetBoneSmoothWidth(double width) {
    // Set the new overlap smooth width
    boneSmoothWidth = width;

    // Set shader variable
    lineShader->bind();
    lineShader->setUniformValue("smoothWidth", boneSmoothWidth);
    lineShader->release();

    circleShader->bind();
    circleShader->setUniformValue("smoothWidth", boneSmoothWidth);
    circleShader->release();

    circleFilledShader->bind();
    circleFilledShader->setUniformValue("smoothWidth", boneSmoothWidth);
    circleFilledShader->release();
    
    // Update graphics
    updateGL();
}

void qvGLWidget::SetBoneJointRadius(double radius) {
    // Set the new bone joint radius
    boneJointRadius = radius;

    // Set shader variable
    circleShader->bind();
    circleShader->setUniformValue("r", boneJointRadius);
    circleShader->release();

    circleFilledShader->bind();
    circleFilledShader->setUniformValue("r", boneJointRadius);
    circleFilledShader->release();
    
    // Update graphics
    updateGL();
}


double qvGLWidget::GetOverlapLineWidth() {
    return overlapLineWidth;
}

double qvGLWidget::GetOverlapSmoothWidth() {
    return overlapSmoothWidth;
}

double qvGLWidget::GetOverlapIntersectionAlpha() {
    return overlapIntersectionAlpha;
}

double qvGLWidget::GetOverlapCircleAlpha() {
    return overlapCircleAlpha;
}

double qvGLWidget::GetOverlapMinCircleDashSize() {
    return overlapMinCircleDashSize;
}


void qvGLWidget::SetBoneUseColor(bool use) {
    boneUseColor = use;

    // Update graphics
    updateGL();
}

bool qvGLWidget::GetBoneUseColor() {
    return boneUseColor;
}


double qvGLWidget::GetBoneLineWidth() {
    return boneLineWidth;
}

double qvGLWidget::GetBoneSmoothWidth() {
    return boneSmoothWidth;
}

double qvGLWidget::GetBoneJointRadius() {
    return boneJointRadius;
}


void qvGLWidget::SetBackgroundColor(const QColor& color) {
    backgroundColor = color;
    
    glClearColor(backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF(), 1.0f);

    ComputeColors();
}

const QColor& qvGLWidget::GetBackgroundColor() {
    return backgroundColor;
}


qvGLWidget::JointColorsType qvGLWidget::GetJointColorsType() {
    return jointColorsType;
}

void qvGLWidget::SetJointColorsType(qvGLWidget::JointColorsType type) {
    jointColorsType = type;

    if (jointColorsType == RandomColors) {
        // Create a new ordering
        srand(randomColorsSeed);
        randomColorsSeed = rand();
    }

    // Update color map
    ComputeColors();

    // Update graphics
    updateGL();
}


qvGLWidget::GenerateBonesType qvGLWidget::GetGenerateBonesType() {
    return generateBonesType;
}

void qvGLWidget::SetGenerateBonesType(qvGLWidget::GenerateBonesType type) {
    generateBonesType = type;

    if (generateBonesType == RandomLengths) {
        srand(generateBonesSeed);
        generateBonesSeed = rand();
    }

    // Generate bones
    GenerateBones();

    // Update graphics
    updateGL();
}


int qvGLWidget::GetGenerateBonesNumber() {
    return generateBonesNumber;
}

void qvGLWidget::SetGenerateBonesNumber(int number) {
    generateBonesNumber = number;

    // Generate bones
    GenerateBones();

    // Update graphics
    updateGL();
}


double qvGLWidget::GetGenerateBonesLength() {
    return generateBonesLength;
}

void qvGLWidget::SetGenerateBonesLength(double length) {
    generateBonesLength = length;

    // Generate bones
    GenerateBones();

    // Update graphics
    updateGL();
}


QuIK::JointOrderType qvGLWidget::GetJointOrderType() {
    return ik->GetJointOrderType();
}

void qvGLWidget::SetJointOrderType(QuIK::JointOrderType type) {
    ik->SetJointOrderType(type);

    // Solve IK
    ik->SolveIK();

    // Update graphics
    updateGL();
}


QuIK::JointPlacementType qvGLWidget::GetJointPlacementType() {
    return ik->GetJointPlacementType();
}

void qvGLWidget::SetJointPlacementType(QuIK::JointPlacementType type) {
    ik->SetJointPlacementType(type);

    // Solve IK
    ik->SolveIK();

    // Update graphics
    updateGL();
}


qvGLWidget::LockTargetType qvGLWidget::GetLockTargetType() {
    return lockTargetType;
}

void qvGLWidget::SetLockTargetType(qvGLWidget::LockTargetType type) {
    lockTargetType = type;

    Vec3 target = ik->GetTarget();
    if (lockTargetType == LockTargetX) target.x() = 0.0;
    else if (lockTargetType == LockTargetY) target.y() = 0.0;

    ik->SetTarget(target);

    // Solve IK
    ik->SolveIK();

    // Update graphics
    updateGL();
}


void qvGLWidget::initializeGL() {
    // Set clear color
    glClearColor(backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF(), 1.0f);

    // No depth test or lighting
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // Turn on blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Set line width
    glLineWidth(boneLineWidth);

    // Load shaders
    lineShader = new QGLShaderProgram();
    lineShader->addShaderFromSourceFile(QGLShader::Vertex, "../../QuIKVis/line_vp.glsl");
    lineShader->addShaderFromSourceFile(QGLShader::Fragment, "../../QuIKVis/line_fp.glsl");
    lineShader->link();

    circleShader = new QGLShaderProgram();
    circleShader->addShaderFromSourceFile(QGLShader::Vertex, "../../QuIKVis/circle_vp.glsl");
    circleShader->addShaderFromSourceFile(QGLShader::Fragment, "../../QuIKVis/circle_fp.glsl");
    circleShader->link();

    circleFilledShader = new QGLShaderProgram();
    circleFilledShader->addShaderFromSourceFile(QGLShader::Vertex, "../../QuIKVis/circleFilled_vp.glsl");
    circleFilledShader->addShaderFromSourceFile(QGLShader::Fragment, "../../QuIKVis/circleFilled_fp.glsl");
    circleFilledShader->link();

    overlapShader = new QGLShaderProgram();
    overlapShader->addShaderFromSourceFile(QGLShader::Vertex, "../../QuIKVis/overlap_vp.glsl");
    overlapShader->addShaderFromSourceFile(QGLShader::Fragment, "../../QuIKVis/overlap_fp.glsl");
    overlapShader->link();

    // Set uniform variables
    lineShader->bind();
    lineShader->setUniformValue("lineWidth", boneLineWidth);
    lineShader->setUniformValue("smoothWidth", boneSmoothWidth);
    lineShader->release();

    circleShader->bind();
    circleShader->setUniformValue("lineWidth", boneLineWidth);
    circleShader->setUniformValue("smoothWidth", boneSmoothWidth);
    circleShader->release();

    circleFilledShader->bind();
    circleFilledShader->setUniformValue("lineWidth", boneLineWidth);
    circleFilledShader->setUniformValue("smoothWidth", boneSmoothWidth);
    circleFilledShader->release();

    overlapShader->bind();
    overlapShader->setUniformValue("lineWidth", overlapLineWidth);
    overlapShader->setUniformValue("smoothWidth", overlapSmoothWidth);
    overlapShader->setUniformValue("intersectionAlpha", overlapIntersectionAlpha);
    overlapShader->setUniformValue("circleAlpha", overlapCircleAlpha);
    overlapShader->setUniformValue("minCircleDashSize", overlapMinCircleDashSize);
    overlapShader->release();
}

void qvGLWidget::paintGL() {
    // Set some OpenGL state for rendering
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (showOverlap) {
        RenderOverlap();
    }

    if (showBones) {
        RenderBones();
    }
}

void qvGLWidget::resizeGL(int width, int height) {
    double aspect = (double)width / (double)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(worldCenter.x() - worldWidth / 2.0,             worldCenter.x() + worldWidth / 2.0, 
            worldCenter.y() - worldWidth / aspect / 2.0,    worldCenter.y() + worldWidth / aspect / 2.0, 
            -1.0,                                           1.0);

    glMatrixMode(GL_MODELVIEW);

    // Update shader pixel size
    pixelSize = (GLfloat)(worldWidth / width);

    lineShader->bind();
    lineShader->setUniformValue("pixelSize", pixelSize);
    lineShader->release();

    circleShader->bind();
    circleShader->setUniformValue("pixelSize", pixelSize);
    circleShader->release();

    circleFilledShader->bind();
    circleFilledShader->setUniformValue("pixelSize", pixelSize);
    circleFilledShader->release();

    overlapShader->bind();
    overlapShader->setUniformValue("pixelSize", pixelSize);
    overlapShader->release();
}


void qvGLWidget::mousePressEvent(QMouseEvent* event) {
    switch (event->button()) {

        case Qt::LeftButton: {
            
            // Try to pick a joint
            int jointIndex = PickJoint(WindowToWorld(event->posF()));

            if (jointIndex > 0) {
                // Set the new target joint
                ik->SetTargetJoint(jointIndex);

                // Update the IK
                ik->SolveIK();

                // Render
                updateGL();
            }

            // Move current target
            worldClickPos = ik->GetTarget();
            clickPos.setX(event->posF().x());
            clickPos.setY(height() - event->y());

            break;
        }

        case Qt::RightButton: {

            // Try to pick a joint
            int jointIndex = PickJoint(WindowToWorld(event->posF()));

            if (jointIndex > 0) {
                // Set the new target joint
                ik->SetTargetJoint(jointIndex);

                // UPdate the IK
                ik->SolveIK();

                // Render
                updateGL();
            }

            // Move current target
            worldClickPos = ik->GetTarget();
            clickPos.setX(event->posF().x());
            clickPos.setY(height() - event->y());

            break;
        }

        case Qt::MidButton:

            worldClickPos = worldCenter;
            clickPos.setX(event->posF().x());
            clickPos.setY(height() - event->y());

            break;
    }
}

void qvGLWidget::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        // Left button, move target
        Vec3 pos = worldClickPos + WorldMouseDifference(event->posF());

        if (lockTargetType == LockTargetX) pos.x() = 0.0;
        else if (lockTargetType == LockTargetY) pos.y() = 0.0;
        
        ik->SetTarget(pos);
        ik->SolveIK();

        updateGL();
    }
    else if (event->buttons() & Qt::RightButton) {
        // Right button, move joint
        Vec3 pos = worldClickPos + WorldMouseDifference(event->posF());

        ik->SetJointPosition(ik->GetTargetJoint(), pos);
        ik->SetTarget(pos);
        ik->SolveIK();

        updateGL();
    }
    else if (event->buttons() & Qt::MidButton) {
        // Middle button, move world
        worldCenter = worldClickPos - WorldMouseDifference(event->posF());
        resizeGL(width(), height());

        updateGL();
    }
}

void qvGLWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    switch (event->button()) {

        case Qt::LeftButton: {

            // Pick joint
            int jointIndex = PickJoint(WindowToWorld(event->posF()));

            if (jointIndex > 0 && jointIndex < ik->GetNumJoints() - 1) {
                // Toggle priority
                ik->TogglePriority(jointIndex);
            }
            else if (jointIndex == -1) {
                // Clear priorities
                ik->ClearPriorities();
            }

            // Render
            updateGL();

            break;
        }

        case Qt::RightButton: {

            // Pick joint
            Vec3 pos = WindowToWorld(event->posF());
            int jointIndex = PickJoint(pos);

            if (jointIndex > 0 && jointIndex < ik->GetNumJoints() - 1) {
                // Delete joint
                ik->DeleteJoint(jointIndex);
            }
            else if (jointIndex == -1) {
                // Find closest joint
                jointIndex = ClosestJoint(pos);

                // Add new joint
                ik->AddJoint(jointIndex + 1, pos);

                // Set new joint as target joint
                ik->SetTargetJoint(jointIndex + 1);

                // Set target
                ik->SetTarget(pos);
            }

            // Update IK
            ik->SolveIK();

            // Update color map
            ComputeColors();

            // Render
            updateGL();

            break;
        }

        case Qt::MidButton:

            // Reset view
            worldWidth = 100.0;
            worldCenter.MakeIdentity();

            resizeGL(width(), height());

            // Render
            updateGL();

            break;
    }
}

void qvGLWidget::wheelEvent(QWheelEvent* event) {
    if (event->delta() > 0.0) {
        worldWidth *= 1.0 / 0.9;
    }
    else {
        worldWidth *= 0.9;
    }

    resizeGL(width(), height());

    updateGL();
}


void qvGLWidget::GenerateBones() {
    // Create the bones
    std::vector<double> bones(generateBonesNumber);


    // Set the lengths
    double totalLength = generateBonesLength * generateBonesNumber;
    switch (generateBonesType) {

        case EvenLengths:

            for (int i = 0; i < generateBonesNumber; i++) {
                bones[i] = generateBonesLength;
            }

            break;

        case AlternatingLengths: {

            double sum = 0.0;
            for (int i = 0; i < generateBonesNumber; i++) {
                if (i % 4 == 0) {
                    bones[i] = generateBonesLength;
                }
                else if (i % 4 == 1) {
                    bones[i] = generateBonesLength / 2.0;
                }
                else if (i % 4 == 2) {
                    bones[i] = generateBonesLength / 2.0;
                }
                else if (i % 4 == 3) {
                    bones[i] = generateBonesLength;
                }

                sum += bones[i];
            }

            // Rescale
            for (int i = 0; i < generateBonesNumber; i++) {
                bones[i] *= totalLength / sum;
            }

            break;
        }

        case DecreasingLengths: {

            double length = totalLength / (double)(pow(2.0, generateBonesNumber) - 1);
            for (int i = generateBonesNumber - 1; i >= 0; i--) {
                bones[i] = length;
                length *= 2.0;
            }

            break;
        }

        case IncreasingLengths: {

            double length = totalLength / (double)(pow(2.0, generateBonesNumber) - 1);
            for (int i = 0; i < generateBonesNumber; i++) {
                bones[i] = length;
                length *= 2.0;
            }

            break;
        }

        case RandomLengths: {

            srand(generateBonesSeed);

            double sum = 0.0;
            for (int i = 0; i < generateBonesNumber; i++) {
                bones[i] = (double)rand() / (double)RAND_MAX;
                sum += bones[i];
            }

            // Rescale
            for (int i = 0; i < generateBonesNumber; i++) {
                bones[i] *= totalLength / sum;
            }

            break;
        }
    }

    ik->SetBones(bones);

    // Solve the new IK
    ik->SolveIK();

    // Update color map
    ComputeColors();
}


Vec3 qvGLWidget::WindowToWorld(const QPointF& windowPos) {
    // Transform from window to OpenGL coordinates
    Vec3 pos(windowPos.x(), height() - windowPos.y(), 0.0);
    pos -= Vec3((double)width() / 2.0, (double)height() / 2.0, 0.0);
    pos *= worldWidth / width();
    pos += Vec3(worldCenter.x(), worldCenter.y(), 0.0);

    return pos;
}

QPointF qvGLWidget::WorldToWindow(const Vec3& worldPos) {
    return QPointF(0, 0);
}


Vec3 qvGLWidget::WorldMouseDifference(const QPointF& windowPos) {
    QPointF pos(windowPos.x(), height() - windowPos.y());
    pos = (pos - clickPos) * worldWidth / width();

    return Vec3(pos.x(), pos.y(), 0.0);
}


int qvGLWidget::PickJoint(const Vec3& worldPos) {
    // Try to pick joint
    for (int i = 0; i < ik->GetNumJoints(); i++) {
        if (worldPos.Distance(ik->GetPositions()[i]) <= boneJointRadius * pixelSize + boneLineWidth * pixelSize * 0.5 + boneSmoothWidth * pixelSize * 0.5) {
            return i;
        }
    }

    return -1;
}

int qvGLWidget::ClosestJoint(const Vec3& worldPos) {
    // Find closest joint
    int index = 1;
    double distance = ik->GetPositions()[index].Distance(worldPos);
    for (int i = index + 1; i < (int)ik->GetNumJoints(); i++) {
        double d = ik->GetPositions()[i].Distance(worldPos);
        if (d < distance) {
            distance = d;
            index = i;
        }
    }

    return index;
}


void qvGLWidget::RenderOverlap() {    
    // Get data from the IK solver
    int numJoints = ik->GetNumJoints();

    Vec3 target = ik->GetTarget();

    std::vector<Vec3> positions = ik->GetPositions();
    
    std::vector<std::vector<double>> maxRadii = ik->GetMaxRadii();
    std::vector<std::vector<double>> minRadii = ik->GetMinRadii();
    

    // Set up for rendering overlapping regions
    std::vector<int> startOrder;
    std::vector<int> currentOrder;
    std::vector<int> endOrder;

/*
for (int i = 0; i < (int)ik->GetStartOrder().size(); i++) {
    std::cout << ik->GetStartOrder()[i] << " " << ik->GetCurrentOrder()[i] << " " << ik->GetEndOrder()[i] << std::endl;
}
std::cout << std::endl;
*/

    switch (centersType) {

        case InitialCenters:

            if (targetOnlyOverlap) {
                // Only use the target joint
                for (int i = 0; i < (int)ik->GetCurrentOrder().size(); i++) {
                    if (ik->GetCurrentOrder()[i] == ik->GetTargetJoint()) {
                        // Use first and last joint for all centers
                        startOrder.push_back(0);
                        currentOrder.push_back(ik->GetCurrentOrder()[i]);
                        endOrder.push_back(numJoints - 1);

                        // XXX: Need to look to see why same joint is used more than once...
                        break;
                    } 
                }
            }
            else {
                // Use the first joint for all circle 1 centers
                startOrder.assign(numJoints - 1, 0);

                // Just go in order for the radii 
                currentOrder.resize(numJoints - 1);
                for (int i = 0; i < numJoints - 1; i++) {
                    currentOrder[i] = i;
                }

                // Use the last joint for all circle 2 centers
                endOrder.assign(numJoints - 1, numJoints - 1);
            }

            break;

        case ProgressiveCenters:

            if (targetOnlyOverlap) {
                // Only use the target joint
                for (int i = 0; i < (int)ik->GetCurrentOrder().size(); i++) {
                    if (ik->GetCurrentOrder()[i] == ik->GetTargetJoint()) {
                        // Use the orders from the IK solver
                        startOrder.push_back(ik->GetStartOrder()[i]);
                        currentOrder.push_back(ik->GetCurrentOrder()[i]);
                        endOrder.push_back(ik->GetEndOrder()[i]);

                        // XXX: Need to look to see why same joint is used more than once...
                    } 
                }
            }
            else {
                // Use the orders from the IK solver
                startOrder = ik->GetStartOrder();
                currentOrder = ik->GetCurrentOrder();
                endOrder = ik->GetEndOrder();
            }

            break;
    }

    
    // Use the target as the center if the last joint is the target joint
    bool useTargetCenter = ik->GetTargetJoint() == ik->GetNumJoints() - 1;


    // Render the overlapping areas
    overlapShader->bind();

//for (int i = (int)startOrder.size() - 1; i >= 1; i--) {
    for (int i = 0; i < (int)startOrder.size(); i++) {
        // Get the start and end sphere centers
        Vec3 c1 = positions[startOrder[i]];
        Vec3 c2 = positions[endOrder[i]];

        if (useTargetCenter) {
            if (endOrder[i] == numJoints - 1) {
                c2.x() = target.x();
                c2.y() = target.y();
            }
        }


        // Set color
        QColor color = colorMap[currentOrder[i]];
        glColor3f(color.redF(), color.greenF(), color.blueF());


        // Draw overlap
        DrawOverlap(c1, c2, 
                    maxRadii[startOrder[i]][currentOrder[i]], minRadii[startOrder[i]][currentOrder[i]],
                    maxRadii[endOrder[i]][currentOrder[i]], minRadii[endOrder[i]][currentOrder[i]]);
    }

// This draws a circle for the last joint
/*
        glColor3f(0.0, 0.0, 0.0);
    int s = positions.size() - 1;
        DrawOverlap(positions[0], positions[0], 
                    maxRadii[0][s], maxRadii[0][s],
                    0.0, 0.0);
*/

    overlapShader->release();
}

void qvGLWidget::RenderBones() {
    // Get data from the IK solver
    int numJoints = ik->GetNumJoints();

    Vec3 target = ik->GetTarget();

// XXX: HACK FOR PAPER
//target = ik->GetPositions()[ik->GetNumJoints() - 1];


    std::vector<Vec3> positions = ik->GetPositions();
    

    // Set the color for the bones
    glColor3f(colorMap[0].redF(), colorMap[0].greenF(), colorMap[0].blueF());

    // Render the bones
    lineShader->bind();

    int endJoint = showEndJoint ? numJoints - 1 : numJoints - 2;
    for (int i = 1; i <= endJoint; i++) {
        Vec3 c1 = positions[i - 1];
        Vec3 c2 = positions[i];

        DrawLine(c1, c2);
    }

    lineShader->release();

    
    // Render the first joint
    circleFilledShader->bind();

    DrawCircle(positions[0], boneJointRadius, boneLineWidth, circleFilledShader);

    circleFilledShader->release();


    // Render the other joints
    circleShader->bind();

    for (int i = 1; i <= endJoint; i++) {
        // Set radius and line width
        GLfloat radius = boneJointRadius;
        GLfloat lineWidth = boneLineWidth;
// XXX: HACK FOR PAPER
        if (i == ik->GetTargetJoint()) {
//if (i == endJoint) {

            // Render larger
            radius *= 1.5;
        }
//        else {
            // Check for priority
            for (int j = 0; j < (int)ik->GetPriorities().size(); j++) {            
                if (i == ik->GetPriorities()[j]) {
                    // Render thicker line
                    lineWidth = boneJointRadius;

                    break;
                }
            } 
//        }


        // Compute color
        glColor3f(backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF());
        if (boneUseColor && i < numJoints - 1) {
            glColor3f(colorMap[i].redF(), colorMap[i].greenF(), colorMap[i].blueF());
        }


        // Draw circle
        DrawCircle(positions[i], radius, lineWidth, circleShader);
    }

    circleShader->release();


    // Render the target    
    glColor3f(colorMap[0].redF(), colorMap[0].greenF(), colorMap[0].blueF());
    
    Vec3 p[4];
    double d = boneJointRadius * 1.5 * pixelSize * sqrt(2.0) / 2.0;
    p[0] = Vec3(target.x() - d, target.y() - d, 0.0);
    p[1] = Vec3(target.x() + d, target.y() + d, 0.0);
    p[2] = Vec3(target.x() - d, target.y() + d, 0.0);
    p[3] = Vec3(target.x() + d, target.y() - d, 0.0);

    lineShader->bind();

    DrawLine(p[0], p[1]);
    DrawLine(p[2], p[3]);

    lineShader->release();

/*
    // Render a line from the target to the target joint
    glColor4f(0.0, 0.0, 0.0, 0.5);
    Vec3 c = positions[ik->GetTargetJoint()];
    glBegin(GL_LINES);
        glVertex2f(c.x(), c.y());
        glVertex2f(target.x(), target.y());
    glEnd();
*/
}


void qvGLWidget::DrawLine(const Vec3& p1, const Vec3& p2) {
    lineShader->setUniformValue("p1", p1.x(), p1.y());
    lineShader->setUniformValue("p2", p2.x(), p2.y());

    Vec3 v1 = p2 - p1;
    Vec3 v2(-v1.y(), v1.x(), v1.z());
    v2.Normalize();
    v2 *= boneLineWidth * pixelSize * 0.5 + boneSmoothWidth * pixelSize * 0.5;

    Vec3 pp1 = p1 + v2;
    Vec3 pp2 = p1 - v2;
    Vec3 pp3 = p2 - v2;
    Vec3 pp4 = p2 + v2;

    glBegin(GL_QUADS);
        glVertex2f(pp1.x(), pp1.y());
        glVertex2f(pp2.x(), pp2.y());
        glVertex2f(pp3.x(), pp3.y());
        glVertex2f(pp4.x(), pp4.y());
    glEnd();
}

void qvGLWidget::DrawCircle(const Vec3& c, double r, double lineWidth, QGLShaderProgram* shader) {
    // Stay same size regardless of zoom
    r *= pixelSize;

    // Padding for rendering the quad
    double rPad = r + lineWidth * pixelSize * 0.5 + boneSmoothWidth * pixelSize * 0.5;

    // Set shader parameters
    shader->setUniformValue("r", (GLfloat)r);
    shader->setUniformValue("c", c.x(), c.y());
    shader->setUniformValue("lineWidth", (GLfloat)lineWidth);

    // Draw bounding quad
    glBegin(GL_QUADS);
        glVertex2f(c.x() - rPad, c.y() - rPad);
        glVertex2f(c.x() + rPad, c.y() - rPad);
        glVertex2f(c.x() + rPad, c.y() + rPad);
        glVertex2f(c.x() - rPad, c.y() + rPad);
    glEnd();
}

void qvGLWidget::DrawOverlap(const Vec3& c1, const Vec3& c2, double rMax1, double rMin1, double rMax2, double rMin2) {
        // Set the sphere center shader parameters
        overlapShader->setUniformValue("c1", c1.x(), c1.y());
        overlapShader->setUniformValue("c2", c2.x(), c2.y());

        // Set the radii shader parameters
        overlapShader->setUniformValue("rMax1", (GLfloat)rMax1);
        overlapShader->setUniformValue("rMin1", (GLfloat)rMin1);
        overlapShader->setUniformValue("rMax2", (GLfloat)rMax2);
        overlapShader->setUniformValue("rMin2", (GLfloat)rMin2);

        // Compute padded radii for bounding box
        GLfloat rMax1Pad = rMax1 + overlapLineWidth * pixelSize * 0.5 + overlapSmoothWidth * pixelSize * 0.5;
        GLfloat rMax2Pad = rMax2 + overlapLineWidth * pixelSize * 0.5 + overlapSmoothWidth * pixelSize * 0.5;

        // Compute bounding box
        // XXX: Could use a bounding box aligned with the vector between the sphere centers...
        double bb[4];
        bb[0] = min(c1.x() - rMax1Pad, c2.x() - rMax2Pad);
        bb[1] = min(c1.y() - rMax1Pad, c2.y() - rMax2Pad);
        bb[2] = max(c1.x() + rMax1Pad, c2.x() + rMax2Pad);
        bb[3] = max(c1.y() + rMax1Pad, c2.y() + rMax2Pad);
       
        // Draw
        glBegin(GL_QUADS);
            glVertex2f(bb[0], bb[1]);
            glVertex2f(bb[2], bb[1]);
            glVertex2f(bb[2], bb[3]);
            glVertex2f(bb[0], bb[3]);
        glEnd();
}


void qvGLWidget::ComputeColors() {
    // Resize the color map
    int numJoints = ik->GetNumJoints();
    colorMap.resize(numJoints);

    // Set the colors
    switch (jointColorsType) {

        case RGBRepeatColors:

            // Set the first joint color
            colorMap[0].setRgbF(0.0, 0.0, 0.0);

            // Generated RGB, repeated
            for (int i = 1; i < numJoints; i++) {
                if ((i - 1) % 3 == 0) {
                    colorMap[i] = Qt::red;
                }
                else if ((i - 1) % 3 == 1) {
                    colorMap[i] = Qt::green;
                }
                else {
                    colorMap[i] = Qt::blue;
                }
            }

            break;

        case RainbowRepeatColors:

            // Set the first joint color
            colorMap[0].setRgbF(0.0, 0.0, 0.0);

            // Generate 6 repeated colors
            for (int i = 1; i < numJoints; i++) {
                if ((i - 1) % 6 == 0) {
                    colorMap[i].setRgbF(1.0, 0.0, 1.0);
                }
                else if ((i - 1) % 6 == 1) {
                    colorMap[i].setRgbF(0.0, 0.0, 1.0);
                }
                else if ((i - 1) % 6 == 2) {
                    colorMap[i].setRgbF(0.0, 1.0, 1.0);
                }            
                else if ((i - 1) % 6 == 3) {
                    colorMap[i].setRgbF(0.0, 1.0, 0.0);
                }            
                else if ((i - 1) % 6 == 4) {
                    colorMap[i].setRgbF(1.0, 1.0, 0.0);
                }            
                else {
                    colorMap[i].setRgbF(1.0, 0.0, 0.0);
                }
            }

            break;

        case BlackBodyColors: {
            
            //double scale = 0.75;
            double scale = 1.0;

            // Black-body radiation
            for (int i = 0; i < numJoints; i++) {
                double c = (double)(i) / (double)(numJoints) * 3.0;
                if (c <= 1.0 ) {
                    colorMap[i].setRgbF(c * scale, 0.0, 0.0);
                }
                else if (c <= 2.0) {
                    colorMap[i].setRgbF(scale, (c - 1.0) * scale, 0.0);
                }
                else {
                    colorMap[i].setRgbF(scale, scale, (c - 2.0) * scale);
                }
            }

            break;
        }

        case GreyScaleColors: {

            // A grey scale, using the background color
            for (int i = 0; i < numJoints; i++) {
                double scale = (double)(i) / (double)(numJoints);
                colorMap[i].setRgbF(backgroundColor.redF() * scale,
                                    backgroundColor.greenF() * scale,
                                    backgroundColor.blueF() * scale);
            }

            break;
        }

        case RandomColors:

            // Set first joint color
            colorMap[0].setRgbF(0.0, 0.0, 0.0);

            // Seed the random number generator
            srand(randomColorsSeed);

            // Set random colors
            for (int i = 1; i < numJoints - 1; i++) {
                colorMap[i].setRgbF((double)rand() / (double)RAND_MAX,
                                    (double)rand() / (double)RAND_MAX,
                                    (double)rand() / (double)RAND_MAX);
            }

            // Set last joint color
            colorMap[numJoints - 1].setRgbF(backgroundColor.redF(),
                                            backgroundColor.greenF(),
                                            backgroundColor.blueF());

            break;
    }
}