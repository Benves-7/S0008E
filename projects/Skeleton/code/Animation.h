#pragma once
#include "Vector4D.h"

namespace CoreAnimation
{
#pragma pack(push, 1)

#define NEBULA3_NAX3_MAGICNUMBER 'NA01'

    //------------------------------------------------------------------------------
    /**
        NAX3 file format structs.
        NOTE: keep all header-structs 4-byte aligned!
    */
    struct Nax3Header
    {
        unsigned int magic;
        unsigned int numClips;
        unsigned int numKeys;
    };

    struct Nax3Clip
    {
        unsigned short numCurves;
        unsigned short startKeyIndex;
        unsigned short numKeys;
        unsigned short keyStride;
        unsigned short keyDuration;
        unsigned char preInfinityType;          // CoreAnimation::InfinityType::Code
        unsigned char postInfinityType;         // CoreAnimation::InfinityType::Code
        unsigned short numEvents;
        char name[50];                          // add up to 64 bytes size for Nax3Clip
    };

    struct Nax3AnimEvent
    {
        char name[47];
        char category[15];
        unsigned short keyIndex;
    };

    struct Nax3Curve
    {
        unsigned int firstKeyIndex;
        unsigned char isActive;                 // 0 or 1
        unsigned char isStatic;                 // 0 or 1
        unsigned char curveType;                // CoreAnimation::CurveType::Code
        unsigned char _padding;                 // padding byte
        float staticKeyX;
        float staticKeyY;
        float staticKeyZ;
        float staticKeyW;

    };

#pragma pack(pop)
} // namespace CoreAnimation

class Curve
{
public:
    unsigned int firstKeyIndex;
    bool isActive;
    bool isStatic;
    unsigned int curveType;
    Vector4D staticKey;
};

class Clip
{
public:
    unsigned short numCurves;
    unsigned short startKeyIndex;
    unsigned short numKeys;
    unsigned short keyStride;
    unsigned short keyDuration;
    unsigned char preInfinityType;
    unsigned char postInfinityType;
    char* name;
    Curve* curves;
};
typedef Vector4D* keys;

class Animation
{
public:
	Animation() { }
	~Animation() { }
	Vector4D getKey(unsigned int clipIndex, float i, unsigned int CurveIndex, unsigned int type)
	{
        int flooredI = (int)floor(i);
        float rem = i - flooredI;

        return Vector4D();
	}
	void loadAnimations(char* filename);

    Clip* clips;
    keys keyBuffer;

private:

};