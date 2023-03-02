/*=========================================================================

  Name:        QuIK.cpp

  Author:      David Borland

  Description: Class to implement quantum inverse kinematics.

=========================================================================*/


#include "QuIK.h"

#include "SphereInterior.h"
#include "SphereExterior.h"

#include <math.h>


QuIK::QuIK() {
    jointOrderType = IncreasingJointOrder;
    jointPlacementType = TriangulationJointPlacement;
}


void QuIK::SetBones(const std::vector<double>& boneLengths) {    
    // Copy the bone lengths
    int numBones = boneLengths.size();

    lengths.resize(numBones); 
    for (int i = 0; i < numBones; i++) {
        lengths[i] = boneLengths[i];
    }


    // Create initial positions
    int numJoints = numBones + 1;

    positions.resize(numJoints);
    for (int i = 1; i < numJoints; i++) {
        positions[i] = positions[i - 1] + Vec3(lengths[i - 1], 0.0, 0.0);
    }


    // Set the initial target joint
    targetJoint = numJoints - 1;


    // Create radii data structures
    CreateRadii();
}

void QuIK::SetJoints(const std::vector<Vec3>& jointPositions) {
    // Copy the joint positions
    int numJoints = jointPositions.size();

    positions.resize(numJoints);
    for (int i = 0; i < numJoints; i++) {
        positions[i] = jointPositions[i];
    }


    // Compute the bone lengths
    int numBones = numJoints - 1;

    lengths.resize(numBones);
    for (int i = 0; i < numBones; i++) {
        lengths[i] = positions[i].Distance(positions[i + 1]);
    }


    // Set the initial target joint
    targetJoint = numJoints - 1;


    // Create radii data structures
    CreateRadii();
}


void QuIK::AddJoint(int jointIndex, Vec3 position) {
    // Get vector from previous joint to the new joint
    Vec3 v = position - positions[jointIndex - 1];

    // Add the joint
    positions.insert(positions.begin() + jointIndex, position);

    // Add the vector to all joints in front of this one
    for (int i = jointIndex + 1; i < (int)positions.size(); i++) {
        positions[i] += v;
    }

    // Add a bone
    lengths.push_back(1.0);

    // Update bone lengths
    for (int i = 0; i < (int)lengths.size(); i++) {
        lengths[i] = positions[i].Distance(positions[i + 1]);
    }

    // Update radii
    CreateRadii();
}

void QuIK::DeleteJoint(int jointIndex) {
    // Get vector from next joint to this joint
    Vec3 v = positions[jointIndex] - positions[jointIndex + 1];

    // Delete the joint
    positions.erase(positions.begin() + jointIndex);

    // Add the vector to all joints in front of this one
    for (int i = jointIndex; i < (int)positions.size(); i++) {
        positions[i] += v;
    }

    // Delete a bone
    lengths.pop_back();

    // Update bone lengths
    for (int i = 0; i < (int)lengths.size(); i++) {
        lengths[i] = positions[i].Distance(positions[i + 1]);
    }

    // Update radii
    CreateRadii();
}

void QuIK::SetJointPosition(int jointIndex, Vec3 position) {
    // Get vector from current to new position
    Vec3 v = position - positions[jointIndex];

    // Update the position
    positions[jointIndex] = position;

    // Add the vector to all joints in front of this one
    for (int i = jointIndex + 1; i < (int)positions.size(); i++) {
        positions[i] += v;
    }

    // Update bone lengths
    for (int i = 0; i < (int)lengths.size(); i++) {
        lengths[i] = positions[i].Distance(positions[i + 1]);
    }

    // Update radii
    CreateRadii();
}


int QuIK::GetTargetJoint() {
    return targetJoint;
}

void QuIK::SetTargetJoint(int targetJointIndex) {
    targetJoint = targetJointIndex;;

    SetTarget(positions[targetJoint]);
}

const Vec3& QuIK::GetTarget() {
    return target;
}

void QuIK::SetTarget(const Vec3& targetPosition) {
    // Set the new target position
    target = targetPosition;
}


void QuIK::SolveIK() {
    // Set the target joint to the target
    if (targetJoint == positions.size() - 1) {
        // Set the last joint position as close as possible to the target
        SphereInterior maxSphere(positions[0], maxRadii[0][positions.size() - 1]);
        SphereExterior minSphere(positions[0], minRadii[0][positions.size() - 1]);

        Vec3 p1 = maxSphere.ClosestPoint(target);
        Vec3 p2 = minSphere.ClosestPoint(target);

        if (positions.back().Distance(p1) < positions.back().Distance(p2) &&
            positions[0].Distance(p1) >= minRadii[0][positions.size() - 1]) {
            positions.back() = p1;
        }
        else {
            positions.back() = p2;
        }
    }
    else {
        positions[targetJoint] = target;
    }


    // Set the priorities
    tempPriorities.resize(priorities.size());
    bool foundTargetJoint = false;
    for (int i = 0; i < (int)priorities.size(); i++) {
        tempPriorities[i] = priorities[i];

        if (tempPriorities[i] == targetJoint) foundTargetJoint = true;
    }
    
    // Add target joint, if necessary
    if (!foundTargetJoint) {
        tempPriorities.push_back(targetJoint);
    }


    // Clear the joint order vectors
    startOrder.clear();
    currentOrder.clear();
    endOrder.clear();


    // Update the IK
    SolveIK(0, positions.size() - 1);
//    SolveIK(0, targetJoint);
//    SolveIK(targetJoint, positions.size() - 1);
}


QuIK::JointOrderType QuIK::GetJointOrderType() {
    return jointOrderType;
}

void QuIK::SetJointOrderType(QuIK::JointOrderType type) {
    jointOrderType = type;
}


QuIK::JointPlacementType QuIK::GetJointPlacementType() {
    return jointPlacementType;
}

void QuIK::SetJointPlacementType(QuIK::JointPlacementType type) {
    jointPlacementType = type;
}


void  QuIK::ClearPriorities() {
    priorities.clear();
}

void  QuIK::AddPriority(int jointIndex) {
    // Don't accept first or last indeces
    if (jointIndex <= 0 || jointIndex >= (int)positions.size() - 1) return;

    // Make sure there is no duplicate
    for (int i = 0; i < (int)priorities.size(); i++) {
        if (priorities[i] == jointIndex) return;
    }

    // Add the joint index
    priorities.push_back(jointIndex);
}

void  QuIK::TogglePriority(int jointIndex) {
    // Don't accept first or last indeces
    if (jointIndex <= 0 || jointIndex >= (int)positions.size() - 1) return;

    // If there, remove
    for (int i = 0; i < (int)priorities.size(); i++) {
        if (priorities[i] == jointIndex) {   
            priorities.erase(priorities.begin() + i);

            return;
        }
    }

    // If here, it was not found, so add the joint index
    priorities.push_back(jointIndex);
}


int QuIK::GetNumBones() {
    return lengths.size();
}

int QuIK::GetNumJoints() {
    return positions.size();
}


const std::vector<double>& QuIK::GetLengths() {
    return lengths;
}

const std::vector<Vec3>& QuIK::GetPositions() {
    return positions;
}


const std::vector<int>& QuIK::GetPriorities() {
    return priorities;
}


const std::vector<std::vector<double>>& QuIK::GetMaxRadii() {
    return maxRadii;
}

const std::vector<std::vector<double>>& QuIK::GetMinRadii() {
    return minRadii;
}


const std::vector<int>& QuIK::GetStartOrder() {
    return startOrder;
}

const std::vector<int>& QuIK::GetCurrentOrder() {
    return currentOrder;
}

const std::vector<int>& QuIK::GetEndOrder() {
    return endOrder;
}


void QuIK::CreateRadii() {
    // One more joint than bones
    int numJoints = positions.size();

    // One set of radii per joint
    maxRadii.resize(numJoints);
    minRadii.resize(numJoints);

    // Create radii data structures
    for (int i = 0; i < numJoints; i++) {
        // Set the number of joints
        maxRadii[i].resize(numJoints);
        minRadii[i].resize(numJoints);

        // Initialize this joint
        maxRadii[i][i] = 0.0;
        minRadii[i][i] = 0.0;

        // Initialize to the left, which must be a fixed distance
        if (i > 0) {
            maxRadii[i][i - 1] = lengths[i - 1];
            minRadii[i][i - 1] = lengths[i - 1];
        }

        // Initialize to the right, which must be a fixed distance
        if (i < numJoints - 1) {
            maxRadii[i][i + 1] = lengths[i];
            minRadii[i][i + 1] = lengths[i];
        }

        // Do the rest
        for (int j = i - 2; j >= 0; j--) {
            maxRadii[i][j] = maxRadii[i][j + 1] + lengths[j];
            minRadii[i][j] = minRadii[i][j + 1] - lengths[j];
        }
        for (int j = i + 2; j < numJoints; j++) {
            maxRadii[i][j] = maxRadii[i][j - 1] + lengths[j - 1];
            minRadii[i][j] = minRadii[i][j - 1] - lengths[j - 1];
        }
    }
/*
    for (int i = 0; i < numJoints; i++) {
        for (int j = 0; j < numJoints; j++) {
            std::cout << maxRadii[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < numJoints; i++) {
        for (int j = 0; j < numJoints; j++) {
            std::cout << minRadii[i][j] << " ";
        }
        std::cout << std::endl;
    }
*/
}


void QuIK::SolveIK(int start, int end) {
    // Pick the current joint to place
    int current = -1;
    for (int i = 0; i < (int)tempPriorities.size(); i++) {
        if (tempPriorities[i] > start && tempPriorities[i] < end) {
            // Use this joint for current
            current = tempPriorities[i];

            // Remove from temp priorities
            tempPriorities.erase(tempPriorities.begin() + i);

            break;
        }
    }    
    if (current == -1) {
        // Pick current some other way
        switch (jointOrderType) {

            case IncreasingJointOrder:

                current = start + 1;

                break;

            case DecreasingJointOrder:

                current = end - 1;

                break;

            case DividingJointOrder:
        
                current = (start + end) / 2;

                break;
        }
    }    
    

    // Save the order 
    startOrder.push_back(start);
    currentOrder.push_back(current);
    endOrder.push_back(end);


    // Place the joint
    switch (jointPlacementType) {

        case TriangulationJointPlacement:

            PlaceJointTriangulation(start, current, end);

            break;

        case InertialJointPlacement:

            PlaceJointInertial(start, current, end);

            break;
    }


    // Recurse
    if (end - start <= 2) {
        return;
    }
    else {
        SolveIK(start, current);
        SolveIK(current, end);
    }
}

void QuIK::PlaceJointTriangulation(int start, int current, int end) {
    // Create max radii spheres
    Sphere s1(positions[start], maxRadii[start][current]);
    Sphere s2(positions[end], maxRadii[end][current]);

    // Perform sphere-sphere intersection
    Vec3 c;
    double r;
    Vec3 n;
    Sphere::SSI_Type i = s1.Intersection(s2, c, r, n);

    // Set the position based on the sphere-sphere intersection
    positions[current] = HandleSphereSphereIntersection(positions[current], i, c, r, n);
}

void QuIK::PlaceJointInertial(int start, int current, int end) {
    Vec3 p = positions[current];

    Vec3 c1 = positions[start];
    Vec3 c2 = positions[end];

    Vec3 v1 = p - c1;
    Vec3 v2 = p - c2;
    double d1 = v1.Magnitude();
    double d2 = v2.Magnitude();

    double rMax1 = maxRadii[start][current];
    double rMin1 = minRadii[start][current];

    double rMax2 = maxRadii[end][current];
    double rMin2 = minRadii[end][current];

    if (d1 <= rMax1 && d1 >= rMin1 &&
        d2 <= rMax2 && d2 >= rMin2) {
        // Already in a good spot, leave it alone
    }
    else {
        // Find closest legitimate point
        SphereInterior sMax1(c1, rMax1);
        SphereExterior sMin1(c1, rMin1);
        SphereInterior sMax2(c2, rMax2);
        SphereExterior sMin2(c2, rMin2);

        Vec3 points[4];
        points[0] = sMax1.ClosestPoint(p);
        points[1] = sMin1.ClosestPoint(p);
        points[2] = sMax2.ClosestPoint(p);
        points[3] = sMin2.ClosestPoint(p);

        double closestDistance;
        int closest = -1;
        double epsilon = 1e-10;
        for (int i = 0; i < 4; i++) {
            if ((points[i].Distance(c1) <= rMax1 + epsilon) &&
                (points[i].Distance(c1) >= rMin1 - epsilon) &&
                (points[i].Distance(c2) <= rMax2 + epsilon) &&
                (points[i].Distance(c2) >= rMin2 - epsilon)) {
                // Compute the distance
                double d = p.Distance(points[i]);

                // Check if closest
                if (closest == -1) {
                    closestDistance = d;
                    closest = i;
                }
                else if (d < closestDistance) {
                    closestDistance = d;
                    closest = i;
                }
            }
        }

        if (closest != -1) {
            // At least one of the points worked, use the closest
            positions[current] = points[closest];  
        }
        else {       
            // Four spheres
            Sphere s1Max(positions[start], maxRadii[start][current]);
            Sphere s1Min(positions[start], minRadii[start][current]);
            Sphere s2Max(positions[end], maxRadii[end][current]);
            Sphere s2Min(positions[end], minRadii[end][current]);

            // Compute intersections
            Sphere::SSI_Type iType[4];
            Vec3 c[4];
            double r[4];
            Vec3 n[4];

            iType[0] = s1Max.Intersection(s2Max, c[0], r[0], n[0]);
            iType[1] = s1Max.Intersection(s2Min, c[1], r[1], n[1]);
            iType[2] = s2Max.Intersection(s1Min, c[2], r[2], n[2]);
            iType[3] = s1Min.Intersection(s2Min, c[3], r[3], n[3]);

            // Handle the intersections
            Vec3 points[4];
            for (int i = 0; i < 4; i++) {
                points[i] = HandleSphereSphereIntersection(positions[current], iType[i], c[i], r[i], n[i]);
            }

            // Choose the closest intersection point
            double closestDistance;
            int closest = -1;
            double epsilon = 1e-10;
            for (int i = 0; i < 4; i++) {
                if ((points[i].Distance(c1) <= rMax1 + epsilon) &&
                    (points[i].Distance(c1) >= rMin1 - epsilon) &&
                    (points[i].Distance(c2) <= rMax2 + epsilon) &&
                    (points[i].Distance(c2) >= rMin2 - epsilon)) {
                    // Compute the distance
                    double d = p.Distance(points[i]);

                    // Check if closest
                    if (closest == -1) {
                        closestDistance = d;
                        closest = i;
                    }
                    else if (d < closestDistance) {
                        closestDistance = d;
                        closest = i;
                    }
                }
            }

            
            // Set the position
            positions[current] = points[closest];
        }
    }
}


Vec3 QuIK::HandleSphereSphereIntersection(const Vec3& p, Sphere::SSI_Type i, const Vec3& c, double r, const Vec3& n) {
    switch (i) {

        case Sphere::SSI_EmptyOutside:
        case Sphere::SSI_EmptyInside:
        case Sphere::SSI_Point:

            // Use the result from the intersection
            return c + n * r;

        case Sphere::SSI_Circle: {

            // Pick the closest of the two possible points
            Vec3 p1 = c + Vec3(-n.y(), n.x(), n.z()) * r;
            Vec3 p2 = c + Vec3(n.y(), -n.x(), n.z()) * r;
            if (p.Distance(p1) < p.Distance(p2)) {
                return p1;
            }
            else {
                return p2;
            }
        }

        case Sphere::SSI_Sphere: {

            // Pick the closest point on the sphere
            SphereInterior s(c, r);

            return s.ClosestPoint(p);
        }

        default:

            // Should never be here
            return p;
    }
}

int QuIK::ClosestValidPoint(const Vec3& p1, const std::vector<Vec3> p2, const std::vector<Sphere*> s) {
    // XXX: STUB!
    return -1;
}