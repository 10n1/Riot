#include "terrain.h"

#include <memory>
#include <stdlib.h>
#include "renderEngine.h"

struct vert
{
    float pos[3];
    float norm[3];
    float tex[2];
};

int CreateTerrain(int size)
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
            v->pos[1] = 0.0f;
            v->pos[1] = rand() / (float)RAND_MAX;
            //v->pos[1] = xx%2 ? 1.0f : 0.0f;
            v->pos[2] = yy;
            v->norm[0] = 0.0f;
            v->norm[1] = 1.0f;
            v->norm[2] = 0.0f;
            v->tex[0] = xx/(float)size;
            v->tex[1] = yy/(float)size;
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

    return RenderEngine::CreateMesh(vertexCount, indexCount, vertexSize, indexSize, vertices, indices);
}

