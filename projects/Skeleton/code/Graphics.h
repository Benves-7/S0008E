#pragma once

#include "Vector4D.h"
#include "GraphicsNode.h"
#include <vector>
#include <fstream>

#define NAX2_MAGICNUMBER 'NVX2'

namespace CoreGraphics
{
    struct Nvx2Header
    {
        unsigned int magic;
        unsigned int numGroups;
        unsigned int numVertices;
        unsigned int vertexWidth;
        unsigned int numIndices;
        unsigned int numEdges;
        unsigned int vertexComponentMask;
    };
    struct Nvx2Group
    {
        unsigned int firstVertex;
        unsigned int numVertices;
        unsigned int firstTriangle;
        unsigned int numTriangles;
        unsigned int firstEdge;
        unsigned int numEdges;
    };
    struct PrimitiveGroup
    {
        unsigned int numVerticies;
        unsigned int baseIndex;
        unsigned int numIndicies;
        unsigned int primitiveTopology;
    };
    enum SemanticName
    {
        Position = 0,
        Normal = 1,
        TexCoord1 = 2,
        Tangent = 3,
        Binormal = 4,
        Color = 5,
        TexCoord2 = 6,
        SkinWeights = 7,
        SkinJIndices = 8,

        Invalid,
    };
    enum Format
    {
        Float,      //> one-component float, expanded to (float, 0, 0, 1)
        Float2,     //> two-component float, expanded to (float, float, 0, 1)
        Float3,     //> three-component float, expanded to (float, float, float, 1)
        Float4,     //> four-component float
        UByte4,     //> four-component unsigned byte
        Byte4,		//> four-component signed byte
        Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
        Short4,     //> four-component signed short
        UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
        Byte4N,		//> four-component normalized signed byte (value / 127.0f)
        Short2N,    //> two-component normalized signed short (value / 32767.0f)
        Short4N,    //> four-component normalized signed short (value / 32767.0f)
    };
    enum N2VertexComponent
    {
        N2Coord = (1 << 0),      // 3 floats
        N2Normal = (1 << 1),      // 3 floats
        N2NormalB4N = (1 << 2),      // 4 unsigned bytes, normalized
        N2Uv0 = (1 << 3),      // 2 floats
        N2Uv0S2 = (1 << 4),      // 2 shorts, 4.12 fixed point
        N2Uv1 = (1 << 5),      // 2 floats
        N2Uv1S2 = (1 << 6),      // 2 shorts, 4.12 fixed point
        N2Uv2 = (1 << 7),      // 2 floats
        N2Uv2S2 = (1 << 8),      // 2 shorts, 4.12 fixed point
        N2Uv3 = (1 << 9),      // 2 floats
        N2Uv3S2 = (1 << 10),     // 2 shorts, 4.12 fixed point
        N2Color = (1 << 11),     // 4 floats
        N2ColorUB4N = (1 << 12),     // 4 unsigned bytes, normalized
        N2Tangent = (1 << 13),     // 3 floats
        N2TangentB4N = (1 << 14),     // 4 unsigned bytes, normalized
        N2Binormal = (1 << 15),     // 3 floats
        N2BinormalB4N = (1 << 16),     // 4 unsigned bytes, normalized
        N2Weights = (1 << 17),     // 4 floats
        N2WeightsUB4N = (1 << 18),     // 4 unsigned bytes, normalized
        N2JIndices = (1 << 19),     // 4 floats
        N2JIndicesUB4 = (1 << 20),     // 4 unsigned bytes

        N2NumVertexComponents = 21,
        N2AllComponents = ((1 << N2NumVertexComponents) - 1),
    };
} // namespace CoreGraphics

using namespace std; 
using namespace CoreGraphics;
struct vertexComponent {
    vertexComponent(SemanticName n, Format f, int size, uint isN, int sizeA)
    {
        name = n;
        format = f;
        sizeInBytes = size;
        isNormalized = isN;
        sizeArray = sizeA;
    }
    SemanticName name;
    Format format;
    int sizeInBytes;
    unsigned int isNormalized;
    int sizeArray;
    unsigned int getType()
    {
        if (this->format == Format::Float3 ||this->format == Format::Float2)
            return 0x1406;
        else if (this->format == Format::Byte4N)
            return 0x1400;
        else if (this->format == Format::UByte4 || this->format == Format::UByte4N)
            return 0x1401;
    }
};

class Graphics
{
public:
    Graphics() {}
    ~Graphics() {}

    bool setup()
    {
        // Try to load mesh.
        if (!loadMesh("Unit_Footman.nvx2"))
            return false;

        // Setup the mesh variables.
        if (!setupMesh())
            return false;

        return true;
    }

    bool loadMesh(char* filename)
    {
        // Open file by filename.
        std::ifstream file;
        file.open(filename, std::ifstream::in | std::ifstream::binary);

        // Check if file could open.
        if (!file)
        {
            printf("ERROR: Failed to open file %s\n", filename);
            return false;
        }

        // Setup pointers.
        file.seekg(0, file.end);
        unsigned int length = file.tellg();
        file.seekg(0, file.beg);
        char* ptr = new char[length];
        file.read(ptr, length);
        file.close();

        // Set header.
        header = (Nvx2Header*)ptr;

        // Check header to see that magic number is a match (correction check).
        if(header->magic != NAX2_MAGICNUMBER)
            return false;

        // EXTRACT info (magic)
        header->numIndices *= 3;

        numGroups = header->numGroups;
        numVertices = header->numVertices;
        vertexWidth = header->vertexWidth;
        numIndices = header->numIndices;
        numEdges = header->numEdges;
        vertexComponentMask = header->vertexComponentMask;
        groupDataSize = 6 * sizeof(uint) * numGroups;
        vertexDataSize = numVertices * vertexWidth * sizeof(GLfloat);
        indexDataSize = sizeof(int) * numIndices;
        groupDataPtr = header + 1;
        vertexDataPtr = ((char*)groupDataPtr) + groupDataSize;
        indexDataPtr = ((char*)vertexDataPtr) + vertexDataSize;

        //Load vertices
        for (int i = 0; i < 21; i++)
        {
            SemanticName vertexType;
            Format format;
            int sizeInBytes, isNormalized, sizeA;
            if (vertexComponentMask & (1<<i))
            {
                switch (1<<i)
                {
                    case N2Coord:       vertexType = SemanticName::Position;      format = Format::Float3;    sizeInBytes = 12;   isNormalized = 0; sizeA = 3; break;
                    case N2Normal:      vertexType = SemanticName::Normal;        format = Format::Float3;    sizeInBytes = 12;   isNormalized = 0; sizeA = 3; break;
                    case N2NormalB4N:   vertexType = SemanticName::Normal;        format = Format::Byte4N;    sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                    case N2Uv0:         vertexType = SemanticName::TexCoord1;     format = Format::Float2;    sizeInBytes = 8;    isNormalized = 0; sizeA = 2; break;
                    case N2Uv0S2:       vertexType = SemanticName::TexCoord1;     format = Format::Short2;    sizeInBytes = 4;    isNormalized = 0; sizeA = 2; break;
                    case N2Uv1:         vertexType = SemanticName::TexCoord2;     format = Format::Float2;    sizeInBytes = 8;    isNormalized = 0; sizeA = 2; break;
                    case N2Uv1S2:       vertexType = SemanticName::TexCoord2;     format = Format::Short2;    sizeInBytes = 4;    isNormalized = 0; sizeA = 2; break;
                    case N2Color:       vertexType = SemanticName::Color;         format = Format::Float4;    sizeInBytes = 16;   isNormalized = 0; sizeA = 4; break;
                    case N2ColorUB4N:   vertexType = SemanticName::Color;         format = Format::UByte4N;   sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                    case N2Tangent:     vertexType = SemanticName::Tangent;       format = Format::Float3;    sizeInBytes = 12;   isNormalized = 0; sizeA = 3; break;
                    case N2TangentB4N:  vertexType = SemanticName::Tangent;       format = Format::Byte4N;    sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                    case N2Binormal:    vertexType = SemanticName::Binormal;      format = Format::Float3;    sizeInBytes = 12;   isNormalized = 0; sizeA = 3; break;
                    case N2BinormalB4N: vertexType = SemanticName::Binormal;      format = Format::Byte4N;    sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                    case N2Weights:     vertexType = SemanticName::SkinWeights;   format = Format::Float4;    sizeInBytes = 16;   isNormalized = 0; sizeA = 4; break;
                    case N2WeightsUB4N: vertexType = SemanticName::SkinWeights;   format = Format::UByte4N;   sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                    case N2JIndices:    vertexType = SemanticName::SkinJIndices;  format = Format::Float4;    sizeInBytes = 16;   isNormalized = 0; sizeA = 4; break;
                    case N2JIndicesUB4: vertexType = SemanticName::SkinJIndices;  format = Format::UByte4;    sizeInBytes = 4;    isNormalized = 0; sizeA = 4; break;

                    default:
                        break;
                }
                this->vertexComponents.push_back(vertexComponent(vertexType, format, sizeInBytes, isNormalized, sizeA));
            }
        }
        return true;
    }

    bool setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexDataPtr, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexDataPtr, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, vertexComponents[0].sizeArray, vertexComponents[0].getType(), vertexComponents[0].isNormalized, sizeof(float)* vertexWidth, NULL);

        int amountOfBytes = vertexComponents[0].sizeInBytes;
        for (int i = 1; i < vertexComponents.size(); i++)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, vertexComponents[i].sizeArray, vertexComponents[i].getType(), vertexComponents[i].isNormalized, sizeof(float)* vertexWidth, (void*)(sizeof(char)*amountOfBytes));
            amountOfBytes += vertexComponents[i].sizeInBytes;
        }
        glBindVertexArray(0);

        // Handles for textures
        glGenTextures(1, &Diff);

        // Get diffuse texture
        glBindTexture(GL_TEXTURE_2D, Diff);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        unsigned char* data = stbi_load("Footman_Diffuse.tga", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture (diffuse)" << std::endl;
            return false;
        }
        stbi_image_free(data);

        // Get normal texture
        glGenTextures(1, &Norm);
        glBindTexture(GL_TEXTURE_2D, Norm);
        data = stbi_load("Footman_Normal.tga", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture (normal)" << std::endl;
            return false;
        }
        stbi_image_free(data);

        // Vertex shader
        std::ifstream file;
        file.open("vs.shader");
        if (file.fail()) {
            std::cout << "Failed to load vertexShader" << std::endl;
            return false;
        }
        else {
            std::stringstream tempstream;
            tempstream << file.rdbuf();
            std::string temp = tempstream.str();
            vs = temp.c_str();

            const GLint lengthOfVertexShader = strlen(vs);
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vs, &lengthOfVertexShader);
            glCompileShader(vertexShader);

            // ERROR LOG
            int  success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
            file.close();
        }

        // Fragment shader
        file.open("fs.shader");
        if (file.fail()) {
            std::cout << "Failed to load fragmentShader" << std::endl;
            return false;
        }
        else {
            std::stringstream tempstream;
            tempstream << file.rdbuf();
            std::string temp = tempstream.str();
            fs = temp.c_str();
            const GLint lengthOfPixelShader = strlen(fs);
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fs, &lengthOfPixelShader);
            glCompileShader(fragmentShader);

            // ERROR LOG
            int  success;
            char infoLog[512];
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
            file.close();
        }

        // Program setup.
        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        glUseProgram(program);
        glUniform1i(glGetUniformLocation(program, "diffuseTexture"), 0);
        glUniform1i(glGetUniformLocation(program, "normalMap"), 1);
        glUseProgram(0);

        return true;
    }

    void draw(Matrix4D viewProjection, Matrix4D modelPos, Vector4D cameraPos, vector<Joint*> joints, bool animation)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Diff);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Norm);

        glUseProgram(program);

        unsigned int transformLoc = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_TRUE, viewProjection.getPointer());

        unsigned int transformLoc2 = glGetUniformLocation(program, "cameraPosition");
        glUniform4fv(transformLoc2, 1, cameraPos.getPointer());

        std::vector<float> transformArray;

        for (int i = 0; i < joints.size(); ++i)
        {
            for (int j = 0; j < 16; ++j)
            {
                transformArray.push_back((joints[i]->worldspaceTransform * joints[i]->inverse).getPointer()[j]);
            }
        }
              unsigned int transformLoc3 = glGetUniformLocation(program, "jointTransforms");
        glUniformMatrix4fv(transformLoc3, joints.size(), GL_TRUE, &transformArray[0]);

        unsigned int transformLoc4 = glGetUniformLocation(program, "modelMatrix");
        glUniformMatrix4fv(transformLoc4, 1, GL_TRUE, modelPos.getPointer());

        unsigned int transformLoc5 = glGetUniformLocation(program, "isPlaying");
        glUniform1i(transformLoc5, animation);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, vertexDataSize, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    // ------------------------------------------------------------------------

    // Loader variables.
    Nvx2Header* header;

    vector<PrimitiveGroup> primGroups;
    vector<vertexComponent> vertexComponents;

    void* groupDataPtr;
    void* vertexDataPtr;
    void* indexDataPtr;

    size_t groupDataSize;
    size_t vertexDataSize;
    size_t indexDataSize;

    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;

    // Render variables.
    uint32 EBO;
    uint32 VBO;
    uint32 VAO;

    uint32 Diff;
    uint32 Norm;

    const char* vs;
    const char* fs;
    uint32 vertexShader;
    uint32 fragmentShader;
    uint32 program;

    vector<Matrix4D> jointsTransform;
};