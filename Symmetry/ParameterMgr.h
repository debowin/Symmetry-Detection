#pragma once
#include<string>

using namespace std;

class SkeleParameterSet
{
	bool runAutoWlop1Step;
	bool runAutoWlop1Stage;
	bool step1;
	bool step2;
	bool step3;
	bool runAllSeg;
	bool skeleStop;
	bool needSeg;
	bool cleanPoints;
	bool needDensity;
	bool useCEIBS;
	bool usePlusPDS;
	bool useKillTooCloseS;
	bool useVirtGrpMergeS;
	bool useNearbyCombineS;
	bool useVNearBodyStopS;

	int fixOrigMode;

	double stopAndGrowError;
	double initRadiusPara;
	double initRadius;
	double downSampleNum;
	double maxIterateTime;
	double maxStopRadius;
	double cGridRadius;
	double acceptBranchSize;
	double currMovError;
	double avgPower;
	double fixOrigWt;
	double hGaussian;
	double repPower;
	double repMu2;
	double repMu;
	double sigmaKNN;
	double eigenFeatureIdT;
	double branchKNN;
	double addSize;
	double snakeSearchMaxDist;
	double branchSearchMaxDist;
	double combineTooCloseT;
	double branchSearchAngle;
	double growSearchRadius;
	double growAcceptSigma;
	double virtHeadAccAngle;
	double saveVirtualAngle;
	double branchMergeMaxD;
	double combineSimAngle;
	double clNearBranchDist;
	double localDensityRadius;
	double radiusUpdateSpeed;
	double followSampleRadius;
	double followSampleMaxAngle;
	double badVirtualAngle;
	double curveSegmentLength;

public:
	__declspec(dllexport) SkeleParameterSet();
	__declspec(dllexport) bool getBool(string pname);
	__declspec(dllexport) int getInt(string pname);
	__declspec(dllexport) double getDouble(string pname);
	__declspec(dllexport) void setDouble(string pname,double val);
	__declspec(dllexport) void setBool(string pname,bool val);
	__declspec(dllexport) void setInt(string pname,int val);
};