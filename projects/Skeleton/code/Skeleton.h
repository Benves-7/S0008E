//
// Created by benves-7 on 9/25/19.
//

#ifndef SKELETON_H
#define SKELETON_H

#include "tinyxml.h"
#include <vector>
#include <sstream>

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif

using namespace std;

struct Joint
{
    Joint(int i, int p, string n)
    {
        index = i;
        parent = p;
        name = n;
    }
    void addMatrixes(Matrix4D pos, Matrix4D rot, Matrix4D sca)
    {
        //
    }
    string name;
    int index, parent;
    vector<int> children;
    bool isRoot = true;

    Matrix4D defaultTransform;

    Matrix4D localTransform, worldspaceTransform, inverse;
};

class Skeleton
{
public:
    Skeleton() {}
    ~Skeleton() {}

    int getJointIndex(int index = 0)
    {
        for (int i = 0; i < joints.size(); ++i)
        {
            if (joints.at(i)->index == index)
                return i;
        }
        return -1;
    }

    void inverseConvertion()
    {
        for (int i = 0; i < joints.size(); ++i)
        {
            if (!joints.at(i)->isRoot)
                joints.at(i)->inverse = Matrix4D::inverse(joints.at(getJointIndex(joints.at(i)->parent))->worldspaceTransform * joints.at(i)->defaultTransform);
        }
    }

    inline void worldSpaceConvertion(int index = 0)
    {
        // Get a reference to the joint.
        int s = joints.size();
        Joint *joint = joints.at(index);

        // If joint is not root
        // WorldspaceTransform is set as parents worldspaceTransform * localTransform.
        // InverseWorldspaceTransform is set as the inverse of worldspaceTransform.
        if (!joint->isRoot)
        {
            joint->worldspaceTransform = joints.at(getJointIndex(joint->parent))->worldspaceTransform * joint->localTransform;
        }
        else
        {
            joint->worldspaceTransform = joint->localTransform;
        }

        // Step through the children and preform the worldspaceconversion.
        for (int i = 0; i < joint->children.size(); ++i)
        {
            worldSpaceConvertion(getJointIndex(joint->children.at(i)));
        }
    }

    inline bool loadSkeleton(const char* filename)
    {
        // Open file by filename.
        TiXmlDocument doc(filename);

        joints = vector<Joint*>();

        // If file can't be found.
        if (!doc.LoadFile())
        {
            printf("ERROR: %s %s\n", doc.ErrorDesc(), filename);
            return false;
        }

        // Create two TiXmlElements for easier access.
        TiXmlElement *eJoint =
                doc.FirstChildElement("Nebula3")
                ->FirstChildElement("Model")
                ->FirstChildElement("CharacterNodes")
                ->FirstChildElement("CharacterNode")
                ->FirstChildElement("Joint");
        TiXmlElement *eOrder =
                doc.FirstChildElement("Nebula3")
                ->FirstChildElement("Model")
                ->FirstChildElement("Skins")
                ->FirstChildElement("Skin")
                ->FirstChildElement("Fragment")
                ->FirstChildElement("Joints");

        // Step through all joint elements.
        while (eJoint)
        {
            // Create a joint and add the name, index, and parent index.
            Joint* joint = new Joint(atoi(eJoint->Attribute("index")), atoi(eJoint->Attribute("parent")), eJoint->Attribute("name"));

            // Check if this joint is not the rootJoint. add this joint to parent's children vector.
            if (joint->parent != -1)
            {
                joints.at(joint->parent)->children.push_back(joint->index);
                joint->isRoot = false;
            }

            // Get all off the attributes from the TiXmlElement.
            float temp[4];
            sscanf(eJoint->Attribute("position"), "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            Vector4D position(temp);
            Matrix4D p = Matrix4D::getPositionMatrix(position);

            sscanf(eJoint->Attribute("rotation"), "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            Vector4D rotation(temp);
            Matrix4D r = Matrix4D::getRotationFromQuaternian(rotation);

            sscanf(eJoint->Attribute("scale"),    "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            Vector4D scale(temp);
            Matrix4D s = Matrix4D::getScaleMatrix(scale);

            // The worldspaceTransform is set to the position * rotation * scale. And the localTransform is set to the same.
            joint->worldspaceTransform = p * r * s;
            joint->defaultTransform = p * r * s;
            joint->localTransform = joint->worldspaceTransform;

            // Then add the joint to the skeletons joint vector.
            joints.push_back(joint);

            // move the TiXmlElement forward to the next joint.
            eJoint = eJoint->NextSiblingElement("Joint");
        }

        // Get the order (as a string) from the TiXmlElement.
        string order = eOrder->GetText();

        // Convert to stringstream.
        stringstream ss(order);

        // Create a string token.
        string token;

        // Step through the stringstream split by ',' and get the token out each time.
        vector<int> orderVector;
        while (getline(ss,token,','))
        {
            // Add token to orderVector as an int.
            orderVector.push_back(atoi(token.c_str()));
        }

        // Step through the orderVector
        vector<Joint*> tempVector;
        for (int i = 0; i < orderVector.size(); ++i)
        {
            // Add the joint from the skeletons joints to the tempVector in order.
            tempVector.push_back(joints[orderVector[i]]);
        }

        defaultArray = joints;
        joints = tempVector;

        worldSpaceConvertion(getJointIndex());
        inverseConvertion();
        return true;
    }

    inline void update()
    {
        // Run worldSpaceConvertion.
        worldSpaceConvertion(getJointIndex());
    }

    vector<Joint*> joints;
    vector<Joint*> defaultArray;
};


#endif //SKELETON_H
