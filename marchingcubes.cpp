#include "MarchingCubes.h"
#include <QVector>
#include<QDebug>

MpVector LinearInterp(mp4Vector p1, mp4Vector p2, float value)
{
    MpVector p;
    if(p1.val != p2.val)
        p = (MpVector)p1 + ((MpVector)p2 - (MpVector)p1)/(p2.val - p1.val)*(value - p1.val);
    else
        p = (MpVector)p1;
    return p;
}

TRIANGLE* MarchingCubes(int ncellsX, int ncellsY, int ncellsZ, float minValue, mp4Vector * points,
                                        INTERSECTION intersection, int &numTriangles)
{
    TRIANGLE * triangles = new TRIANGLE[3*ncellsX*ncellsY*ncellsZ];
    numTriangles = int(0);

    int YtimeZ = (ncellsY+1)*(ncellsZ+1);
    for(int i=0; i < ncellsX; i++)
        for(int j=0; j < ncellsY; j++)
            for(int k=0; k < ncellsZ; k++)
            {
                mp4Vector verts[8];
                int ind = i*YtimeZ + j*(ncellsZ+1) + k;
                verts[0] = points[ind];
                verts[1] = points[ind + YtimeZ];
                verts[2] = points[ind + YtimeZ + 1];
                verts[3] = points[ind + 1];
                verts[4] = points[ind + (ncellsZ+1)];
                verts[5] = points[ind + YtimeZ + (ncellsZ+1)];
                verts[6] = points[ind + YtimeZ + (ncellsZ+1) + 1];
                verts[7] = points[ind + (ncellsZ+1) + 1];

                int cubeIndex = int(0);
                for(int n=0; n < 8; n++)
                    if(verts[n].val <= minValue) cubeIndex |= (1 << n);

                    if(!edgeTable[cubeIndex]) continue;

                MpVector intVerts[12];
                if(edgeTable[cubeIndex] & 1) intVerts[0] = intersection(verts[0], verts[1], minValue);
                if(edgeTable[cubeIndex] & 2) intVerts[1] = intersection(verts[1], verts[2], minValue);
                if(edgeTable[cubeIndex] & 4) intVerts[2] = intersection(verts[2], verts[3], minValue);
                if(edgeTable[cubeIndex] & 8) intVerts[3] = intersection(verts[3], verts[0], minValue);
                if(edgeTable[cubeIndex] & 16) intVerts[4] = intersection(verts[4], verts[5], minValue);
                if(edgeTable[cubeIndex] & 32) intVerts[5] = intersection(verts[5], verts[6], minValue);
                if(edgeTable[cubeIndex] & 64) intVerts[6] = intersection(verts[6], verts[7], minValue);
                if(edgeTable[cubeIndex] & 128) intVerts[7] = intersection(verts[7], verts[4], minValue);
                if(edgeTable[cubeIndex] & 256) intVerts[8] = intersection(verts[0], verts[4], minValue);
                if(edgeTable[cubeIndex] & 512) intVerts[9] = intersection(verts[1], verts[5], minValue);
                if(edgeTable[cubeIndex] & 1024) intVerts[10] = intersection(verts[2], verts[6], minValue);
                if(edgeTable[cubeIndex] & 2048) intVerts[11] = intersection(verts[3], verts[7], minValue);

                for (int n=0; triTable[cubeIndex][n] != -1; n+=3) {
                    triangles[numTriangles].p[0] = intVerts[triTable[cubeIndex][n+2]];
                    triangles[numTriangles].p[1] = intVerts[triTable[cubeIndex][n+1]];
                    triangles[numTriangles].p[2] = intVerts[triTable[cubeIndex][n]];
                    triangles[numTriangles].norm = ((triangles[numTriangles].p[1] -
                        triangles[numTriangles].p[0]).Cross(triangles[numTriangles].p[2] -
                        triangles[numTriangles].p[0])).Normalize();
                    numTriangles++;
                }
            }
        TRIANGLE *retTriangles = new TRIANGLE[numTriangles];
        for(int i=0; i < numTriangles; i++)
            retTriangles[i] = triangles[i];
        delete [] triangles;

    return retTriangles;
}


TRIANGLE* MarchingCubesLinear(int ncellsX, int ncellsY, int ncellsZ, float minValue,
                                    mp4Vector * points, int &numTriangles)
{
    return MarchingCubes(ncellsX, ncellsY, ncellsZ, minValue, points, LinearInterp, numTriangles);
}


TRIANGLE* MarchingCubes(float mcMinX, float mcMaxX, float mcMinY, float mcMaxY, float mcMinZ, float mcMaxZ,
                            int ncellsX, int ncellsY, int ncellsZ, float minValue,
                            FORMULA formula, INTERSECTION intersection, int &numTriangles)
{
    mp4Vector * mcDataPoints = new mp4Vector[(ncellsX+1)*(ncellsY+1)*(ncellsZ+1)];
    MpVector stepSize((mcMaxX-mcMinX)/ncellsX, (mcMaxY-mcMinY)/ncellsY, (mcMaxZ-mcMinZ)/ncellsZ);

    int YtimesZ = (ncellsY+1)*(ncellsZ+1);
    for(int i=0; i < ncellsX+1; i++) {
        int ni = i*YtimesZ;
        float vertX = mcMinX + i*stepSize.x;
        for(int j=0; j < ncellsY+1; j++) {
            int nj = j*(ncellsZ+1);
            float vertY = mcMinY + j*stepSize.y;
            for(int k=0; k < ncellsZ+1; k++) {
                mp4Vector vert(vertX, vertY, mcMinZ + k*stepSize.z, 0);
                vert.val = formula((MpVector)vert);
                mcDataPoints[ni + nj + k] = vert;
            }
        }
    }
    return MarchingCubes(ncellsX, ncellsY, ncellsZ, minValue, mcDataPoints, intersection, numTriangles);
}

TRIANGLE* MarchingCubesLinear(float mcMinX, float mcMaxX, float mcMinY, float mcMaxY, float mcMinZ, float mcMaxZ,
                                int ncellsX, int ncellsY, int ncellsZ, float minValue,
                                FORMULA formula, int &numTriangles)
{
    return MarchingCubes(mcMinX, mcMaxX, mcMinY, mcMaxY, mcMinZ, mcMaxZ, ncellsX, ncellsY, ncellsZ, minValue,
        formula, LinearInterp, numTriangles);
}
