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


using namespace CoreAnimation;
class Animation
{
public:
	Animation() { }
	~Animation() { }
	Vector4D getKey(unsigned int clipIndex, float i, unsigned int curveIndex, unsigned int type)
	{
        int flooredI = (int)floor(i);
        float rem = i - flooredI;

        Clip clip = clips[clipIndex];
        auto c = clips->curves[curveIndex];
        unsigned int curve = clips->curves[curveIndex].firstKeyIndex;

        if (type == 0)
        {
            auto temp = Vector4D::vLerp(keyBuffer[clips[clipIndex].curves[curveIndex].firstKeyIndex + (flooredI) % clips[clipIndex].numKeys * clips[clipIndex].keyStride],
                keyBuffer[clips[clipIndex].curves[curveIndex].firstKeyIndex + (flooredI + 1) % clips[clipIndex].numKeys * clips[clipIndex].keyStride], rem);
            return temp;
        }
        else if (type == 1)
        {
            auto temp = Vector4D::Slerp(
                keyBuffer[clips[clipIndex].curves[curveIndex].firstKeyIndex + (flooredI + 0) % clips[clipIndex].numKeys * clips[clipIndex].keyStride],
                keyBuffer[clips[clipIndex].curves[curveIndex].firstKeyIndex + (flooredI + 1) % clips[clipIndex].numKeys * clips[clipIndex].keyStride],
                rem);
            return temp;
        }
        auto temp = keyBuffer[clips[clipIndex].curves[curveIndex].firstKeyIndex + ((int)floor(i)) % clips[clipIndex].numKeys * clips[clipIndex].keyStride];
        return temp;
	}

	void loadAnimations(char* filename)
    {
        ifstream file;
        file.open(filename, ifstream::in | ifstream::binary);
        file.seekg(0, file.end);
        unsigned int length = file.tellg();
        file.seekg(0, file.beg);
        char* ptr = new char[length];
        file.read(ptr, length);

        Nax3Header* h = (Nax3Header*)ptr;
        header = h;
        ptr += sizeof(Nax3Header);
        keyBuffer = (Vector4D*)malloc(h->numKeys * sizeof(Vector4D));

        clips = new Clip[h->numClips];
        for (int i = 0; i < h->numClips; i++)
        {
            Nax3Clip* naxClip = (Nax3Clip*)ptr;
            ptr += sizeof(Nax3Clip);
            Clip cl;
            cl.numCurves = naxClip->numCurves;
            cl.startKeyIndex = naxClip->startKeyIndex;
            cl.numKeys = naxClip->numKeys;
            cl.keyStride = naxClip->keyStride;
            cl.keyDuration = naxClip->keyDuration;
            cl.preInfinityType = naxClip->preInfinityType;
            cl.postInfinityType = naxClip->postInfinityType;
            cl.name = naxClip->name;

            for (int j = 0; j < naxClip->numEvents; j++)
            {
                ptr += sizeof(Nax3AnimEvent);
            }

            unsigned  int curveIndex;
            cl.curves = new Curve[cl.numCurves];
            for (curveIndex = 0; curveIndex < cl.numCurves; curveIndex++)
            {
                Nax3Curve* naxCurve = (Nax3Curve*)ptr;
                ptr += sizeof(Nax3Curve);
                Curve cu;
                cu.firstKeyIndex = naxCurve->firstKeyIndex;
                cu.isActive = naxCurve->isActive;
                cu.isStatic = naxCurve->isStatic;
                cu.curveType = naxCurve->curveType;
                cu.staticKey = Vector4D(naxCurve->staticKeyX, naxCurve->staticKeyY, naxCurve->staticKeyZ, naxCurve->staticKeyW);
                cl.curves[curveIndex] = cu;
            }
            clips[i] = cl;
            
            memcpy(keyBuffer, ptr, h->numKeys * sizeof(Vector4D));
        }
    }

    CoreAnimation::Nax3Header* header;

    Clip* clips;
    keys keyBuffer;

private:

};