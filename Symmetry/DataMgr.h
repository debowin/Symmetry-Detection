#pragma once
#include "CMesh.h"
//#include "Parameter.h"
#include "GlobalFunction.h"
#include "Algorithm/Skeleton.h"



#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export.h>

#include <sstream>
#include <fstream>
#include <set>

using namespace vcg;
using namespace std;
using namespace tri;


class DataMgr
{
public:
	__declspec(dllexport) DataMgr(SkeleParameterSet* _para);
	__declspec(dllexport) DataMgr();
	__declspec(dllexport) ~DataMgr(void);

	__declspec(dllexport) void loadMeshToOriginal(string fileName);
	__declspec(dllexport) void loadMeshToSample(string fileName);
	__declspec(dllexport) void saveObj(string fileName,CMesh&);
	__declspec(dllexport) void loadImage(string fileName);
  __declspec(dllexport) void loadXYZN(string fileName);

	__declspec(dllexport) bool isSamplesEmpty();
	__declspec(dllexport) bool isOriginalEmpty();
  __declspec(dllexport) bool isSkeletonEmpty();

	__declspec(dllexport) CMesh* getCurrentSamples();
	__declspec(dllexport) CMesh* getCurrentOriginal();
	__declspec(dllexport) Skeleton* getCurrentSkeleton();

	__declspec(dllexport) void recomputeBox();
	__declspec(dllexport) double getInitRadiuse();

	__declspec(dllexport) void downSamplesByNum(bool use_random_downsample = true);
	__declspec(dllexport) void subSamples();

	__declspec(dllexport) void normalizeROSA_Mesh(CMesh& mesh);
	__declspec(dllexport) Box3f normalizeAllMesh();

	__declspec(dllexport) void eraseRemovedSamples();
	__declspec(dllexport) void clearData();
	__declspec(dllexport) void recomputeQuad();

	__declspec(dllexport) void loadSkeletonFromSkel(string fileName);
	__declspec(dllexport) void saveSkeletonAsSkel(string fileName);


private:
	__declspec(dllexport) void clearCMesh(CMesh& mesh);

public:
	CMesh original;
	CMesh samples;
	Skeleton skeleton;
	//cv::Mat image;

	SkeleParameterSet* para;
	double init_radius;
	string curr_file_name;
};