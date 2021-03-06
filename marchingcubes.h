#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include "mpVector.h"
#include "MCTable.h"

class TRIANGLE{
public:
    MpVector p[3];
    MpVector norm;
} ;

MpVector LinearInterp(mp4Vector p1, mp4Vector p2, float value);

typedef bool (*OUTSIDE)(MpVector);

typedef MpVector (*INTERSECTION)(mp4Vector, mp4Vector, float);

typedef float (*FORMULA)(MpVector);

TRIANGLE* MarchingCubes(int ncellsX, int ncellsY, int ncellsZ, float minValue, mp4Vector * points,
                                    INTERSECTION intersection, int &numTriangles);

TRIANGLE* MarchingCubesLinear(int ncellsX, int ncellsY, int ncellsZ, float minValue,
                                    mp4Vector * points, int &numTriangles);


TRIANGLE* MarchingCubes(float mcMinX, float mcMaxX, float mcMinY, float mcMaxY, float mcMinZ, float mcMaxZ,
                                    int ncellsX, int ncellsY, int ncellsZ, float minValue,
                                    FORMULA formula, INTERSECTION intersection, int &numTriangles);
TRIANGLE* MarchingCubesLinear(float mcMinX, float mcMaxX, float mcMinY, float mcMaxY, float mcMinZ, float mcMaxZ,
                                    int ncellsX, int ncellsY, int ncellsZ, float minValue,
                                    FORMULA formula, int &numTriangles);

#endif // MARCHINGCUBES_H
