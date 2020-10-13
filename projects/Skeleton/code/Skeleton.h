//
// Created by benves-7 on 9/25/19.
//

#ifndef SKELETON_H
#define SKELETON_H

#include "tinyxml.h"

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif


struct Joint
{
    std::string name;
    bool changed = false;
    int index, parent;
    vector<int> children;
    Vector4D position, rotation, scale;
    Matrix4D mPos, mRot, mSca;
    Matrix4D transform, worldspaceTransform;
};

class Skeleton
{
public:
    inline void worldSpaceConvertion()
    {
        for (int i = 0; i < joints.size(); i++)
        {
            if (joints.at(i).parent!=-1)
            {
                joints.at(i).worldspaceTransform = joints.at(joints.at(i).parent).worldspaceTransform * joints.at(i).transform;
            }
        }
    }
    inline void loadSkeleton(const char* filename)
    {
        TiXmlDocument doc(filename);
        if (!doc.LoadFile())
        {
            printf("Failed to load file \"%s\"\n", filename);
            return;
        }

        TiXmlElement *eJoint = doc.FirstChildElement("Nebula3")->FirstChildElement("Model")->FirstChildElement("CharacterNodes")->FirstChildElement("CharacterNode")->FirstChildElement("Joint");
        while (eJoint)
        {
            Joint joint;
            joint.name = eJoint->Attribute("name");
            joint.index = atoi(eJoint->Attribute("index"));
            joint.parent = atoi(eJoint->Attribute("parent"));

            if (joint.parent != -1)
            {
                joints.at(joint.parent).children.push_back(joint.index);
            }

            float temp[4];
            sscanf(eJoint->Attribute("position"), "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            Vector4D position(temp);
            Matrix4D p = Matrix4D::GetPositionMatrix(position);
            sscanf(eJoint->Attribute("rotation"), "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            Vector4D rotation(temp);
            Matrix4D r = Matrix4D::GetRotationFromQuaternian(rotation);
            sscanf(eJoint->Attribute("scale"),    "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            Vector4D scale(temp);
            Matrix4D s = Matrix4D::GetScaleMatrix(scale);

            joint.transform = p*r*s;

            joints.push_back(joint);
            eJoint = eJoint->NextSiblingElement("Joint");
        }
        worldSpaceConvertion();
    }
    inline void update(int index)
    {
        if (joints.at(index).parent == -1)
        {
            joints.at(index).worldspaceTransform = joints.at(index).transform;
        }
        else
        {
            joints.at(index).worldspaceTransform = joints.at(joints.at(index).parent).worldspaceTransform * joints.at(index).transform;
        }
        for (int i = 0; i < joints.at(index).children.size(); i++)
        {
            update(joints.at(index).children.at(i));
        }
    }
    vector<Joint> joints;
};


#endif //SKELETON_H
