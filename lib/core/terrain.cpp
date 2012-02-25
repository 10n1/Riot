#include "terrain.h"

#include <memory>
#include <stdlib.h>
#include "renderEngine.h"
#include "perlin.h"

struct vert
{
    float pos[3];
    float norm[3];
    float tex[2];
};

float* GenerateTerrain(int size)
{
    Perlin perlin(4,2,100,100);
    float* heights = new float[size*size];

    for(int xx=0; xx<size; ++xx)
    {
        for(int yy=0; yy<size; ++yy)
        {
            float height = rand() / (float)RAND_MAX;
            heights[yy*size + xx] = perlin.Get(xx/(float)size,yy/(float)size);
        }
    }

    return heights;
}

int GenerateTerrainMesh(float* heights, int size)
{
    size_t vertexSize = sizeof(vert);
    size_t indexSize = sizeof(unsigned int);
    size_t vertexCount = size*size;
    size_t indexCount = vertexCount*6;

    vert* vertices = new vert[vertexCount];
    unsigned int* indices = new unsigned int[indexCount];

    // Vertices
    for(int xx=0; xx<size; ++xx)
    {
        for(int yy=0; yy<size; ++yy)
        {
            vert* v = &vertices[yy*size + xx];
            v->pos[0] = xx;
            v->pos[1] = heights[yy*size + xx];
            v->pos[2] = yy;
            v->norm[0] = 0.0f;
            v->norm[1] = 1.0f;
            v->norm[2] = 0.0f;
            float textureResolution = 4;
            v->tex[0] = xx/(size/textureResolution) * textureResolution;
            v->tex[1] = yy/(size/textureResolution) * textureResolution;
        }
    }

    // Indices
    unsigned int nIndex = 0;
    for(unsigned int xx = 0; xx < size; ++xx )
    {
        if(xx == size-1)
            continue;
        for(unsigned int yy = 0; yy < size; ++yy )
        {
            unsigned int nStart = xx * size;
            if(yy == size-1)
                continue;

            indices[ nIndex++ ] = xx + yy*size + 1;
            indices[ nIndex++ ] = xx + yy*size;
            indices[ nIndex++ ] = xx + yy*size + size;
            indices[ nIndex++ ] = xx + yy*size + 1;
            indices[ nIndex++ ] = xx + yy*size + size;
            indices[ nIndex++ ] = xx + yy*size + size + 1;
        }
    }

    int mesh = RenderEngine::CreateMesh(vertexCount, indexCount, vertexSize, indexSize, vertices, indices);
    delete [] vertices;
    delete [] indices;
    return mesh;
}

