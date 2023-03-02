/*=========================================================================

  Name:        qvGLWidget.h

  Author:      David Borland

  Description: Handles drawing to an OpenGL buffer.

=========================================================================*/


#ifndef QVGLWIDGET_H
#define QVGLWIDGET_H


#include <QGLWidget>

#include "Vec3.h"
#include "QuIK.h"

class QGLShaderProgram;


class qvGLWidget : public QGLWidget {
    Q_OBJECT

public:
    qvGLWidget(QWidget* parent);
    virtual ~qvGLWidget();

    // Show overlap and bones
    bool GetShowOverlap();
    void SetShowOverlap(bool show);

    bool GetShowBones();
    void SetShowBones(bool show);

    bool GetShowEndJoint();
    void SetShowEndJoint(bool show);

    // Centers type
    enum CentersType {
        InitialCenters,
        ProgressiveCenters
    };
    CentersType GetCentersType();
    void SetCentersType(CentersType type);

    bool GetTargetOnlyOverlap();
    void SetTargetOnlyOverlap(bool justTarget);

    // Overlap appearance
    double GetOverlapLineWidth();
    void SetOverlapLineWidth(double width); 

    double GetOverlapSmoothWidth();
    void SetOverlapSmoothWidth(double width);

    void SetOverlapIntersectionAlpha(double alpha); 
    double GetOverlapIntersectionAlpha();   

    void SetOverlapCircleAlpha(double alpha);
    double GetOverlapCircleAlpha();

    void SetOverlapMinCircleDashSize(double size);
    double GetOverlapMinCircleDashSize();

    // Bone appearance
    bool GetBoneUseColor();
    void SetBoneUseColor(bool use);

    double GetBoneLineWidth();
    void SetBoneLineWidth(double width);

    double GetBoneSmoothWidth();
    void SetBoneSmoothWidth(double width);

    double GetBoneJointRadius();
    void SetBoneJointRadius(double radius);

    // Colors
    void SetBackgroundColor(const QColor& color);
    const QColor& GetBackgroundColor();

    enum JointColorsType {
        RGBRepeatColors,
        RainbowRepeatColors,
        BlackBodyColors,
        GreyScaleColors,
        RandomColors
    };
    JointColorsType GetJointColorsType();
    void SetJointColorsType(JointColorsType type);

    // Generate bones
    enum GenerateBonesType {
        EvenLengths,
        AlternatingLengths,
        DecreasingLengths,
        IncreasingLengths,
        RandomLengths
    };
    GenerateBonesType GetGenerateBonesType();
    void SetGenerateBonesType(GenerateBonesType type);

    int GetGenerateBonesNumber();
    void SetGenerateBonesNumber(int number);

    double GetGenerateBonesLength();
    void SetGenerateBonesLength(double length);

    // Joint order
    QuIK::JointOrderType GetJointOrderType();
    void SetJointOrderType(QuIK::JointOrderType type);

    // Joint placement
    QuIK::JointPlacementType GetJointPlacementType();
    void SetJointPlacementType(QuIK::JointPlacementType type);

    // Lock target
    enum LockTargetType {
        LockTargetNone,
        LockTargetX,
        LockTargetY
    };
    LockTargetType GetLockTargetType();
    void SetLockTargetType(LockTargetType type);

public slots:

protected:
    // Reimplement parent members for OpenGL
    virtual void initializeGL();
    virtual void paintGL(); 
    virtual void resizeGL(int width, int height);

    // Mouse events
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    // Generate bones
    void GenerateBones();

    // Coordinate conversion
    Vec3 WindowToWorld(const QPointF& windowPos);
    QPointF WorldToWindow(const Vec3& worldPos);
    Vec3 WorldMouseDifference(const QPointF& windowPos);

    // Pick a joint
    int PickJoint(const Vec3& worldPos);

    // Pick closest joint
    int ClosestJoint(const Vec3& worldPos);

    // Render
    void RenderOverlap();
    void RenderBones();

    void DrawLine(const Vec3& p1, const Vec3& p2);
    void DrawCircle(const Vec3& c, double r, double lineWidth, QGLShaderProgram* shader);
    void DrawOverlap(const Vec3& c1, const Vec3& c2, double rMax1, double rMin1, double rMax2, double rMin2);

    // Compute colors for rendering
    void ComputeColors();

    // Shaders
    QGLShaderProgram* lineShader;
    QGLShaderProgram* circleShader;
    QGLShaderProgram* circleFilledShader;
    QGLShaderProgram* overlapShader;

    // Appearance
    double worldWidth;
    Vec3 worldCenter;

    bool showOverlap;
    bool showBones;
    bool showEndJoint;

    CentersType centersType;
    bool targetOnlyOverlap;

    GLfloat pixelSize;

    GLfloat overlapLineWidth;
    GLfloat overlapSmoothWidth;    
    GLfloat overlapIntersectionAlpha;
    GLfloat overlapCircleAlpha;
    GLfloat overlapMinCircleDashSize;

    bool boneUseColor;
    GLfloat boneLineWidth;
    GLfloat boneSmoothWidth;
    GLfloat boneJointRadius;

    JointColorsType jointColorsType;
    std::vector<QColor> colorMap;
    unsigned int randomColorsSeed;
    QColor backgroundColor;

    // The IK solver
    QuIK* ik;

    GenerateBonesType generateBonesType;
    int generateBonesNumber;
    double generateBonesLength;
    unsigned int generateBonesSeed;

    LockTargetType lockTargetType;

    // For mouse interaction
    Vec3 worldClickPos;
    QPointF clickPos;
};


#endif