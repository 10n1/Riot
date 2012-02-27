#ifndef __TERRAIN_H_
#define __TERRAIN_H_

float* GenerateTerrain(int size);
int GenerateTerrainMesh(float* heights, int size, void** outVertices = 0);

#define TERRAIN_INDEX(x,z) _terrainHeights[x+z*terrainSize]

#endif
