/*=========================================================================

  Name:        QuIK.h

  Author:      David Borland

  Description: Class to implement quantum inverse kinematics.

=========================================================================*/


#include "Vec3.h"
#include "Sphere.h"

#include <vector>


class QuIK {
public:
    // Constructor
    QuIK();

    // Set bones or joints
    void SetBones(const std::vector<double>& boneLengths);
    void SetJoints(const std::vector<Vec3>& jointPositions);

    // Manipulate joints
    void AddJoint(int jointIndex, Vec3 position);
    void DeleteJoint(int jointIndex);
    void SetJointPosition(int jointIndex, Vec3 position);

    // Get/set target and target joint
    int GetTargetJoint();
    void SetTargetJoint(int targetJointIndex);
    const Vec3& GetTarget();
    void SetTarget(const Vec3& targetPosition);

    // Solve
    void SolveIK();

    // Joint order
    enum JointOrderType {
        IncreasingJointOrder,
        DecreasingJointOrder,
        DividingJointOrder
    };
    JointOrderType GetJointOrderType();
    void SetJointOrderType(JointOrderType type);

    // Joint placement
    enum JointPlacementType {
        TriangulationJointPlacement,
        InertialJointPlacement
    };
    JointPlacementType GetJointPlacementType();
    void SetJointPlacementType(JointPlacementType type);

    // Joint priorities
    void ClearPriorities();
    void AddPriority(int jointIndex);
    void TogglePriority(int jointIndex);

    // Get number of bones and joints
    int GetNumBones();
    int GetNumJoints();

    // Get bone lengths and joint positions
    const std::vector<double>& GetLengths();
    const std::vector<Vec3>& GetPositions();

    // Get joint priorities
    const std::vector<int>& GetPriorities();

    // Get radii data structures
    const std::vector<std::vector<double>>& GetMaxRadii();
    const std::vector<std::vector<double>>& GetMinRadii();

    // Get order of joint placement during most recent solution
    const std::vector<int>& GetStartOrder();
    const std::vector<int>& GetCurrentOrder();
    const std::vector<int>& GetEndOrder();

protected:
    // Create the radii data structures
    void CreateRadii();

    // Solve the IK
    void SolveIK(int start, int end);
    void PlaceJointTriangulation(int start, int current, int end);
    void PlaceJointInertial(int start, int current, int end);

    // Handle result of sphere-sphere intersection
    Vec3 HandleSphereSphereIntersection(const Vec3& p, Sphere::SSI_Type i, const Vec3& c, double r, const Vec3& n);

    // Find the closest point in p2 to the given point p1 that is valid for the given spheres
    int ClosestValidPoint(const Vec3& p1, const std::vector<Vec3> p2, const std::vector<Sphere*> s);


    // Bone chain
    std::vector<double> lengths;
    std::vector<Vec3> positions;
    
    std::vector<int> priorities;
    std::vector<int> tempPriorities;
    
    // Radius data structures
    std::vector<std::vector<double>> maxRadii;
    std::vector<std::vector<double>> minRadii;

    // Target Position
    int targetJoint;
    Vec3 target;

    // Algorithm parameters
    JointOrderType jointOrderType;
    JointPlacementType jointPlacementType;

    // Description of most recent IK solution
    std::vector<int> startOrder;
    std::vector<int> currentOrder;
    std::vector<int> endOrder;
};