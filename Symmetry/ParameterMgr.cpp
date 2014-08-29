#include "ParameterMgr.h"
#include<iostream>
using namespace std;

SkeleParameterSet::SkeleParameterSet()
{
	
}

bool SkeleParameterSet::getBool(string name)
{
	//string name(pname);
	if(name == "Run Auto Wlop One Step")
		return runAutoWlop1Step;
	else if(name == "Run Auto Wlop One Stage")
		return runAutoWlop1Stage;
	else if(name == "Step1 Detect Skeleton Feature")
		return step1;
	else if(name == "Step2 Search New Branches")
		return step2;
	else if(name == "Step3 Clean And Update Radius")
		return step3;
	else if(name == "Run ALL Segment")
		return runAllSeg;
	else if(name == "The Skeletonization Process Should Stop")
		return skeleStop;
	else if(name == "Need Segment Right Away")
		return needSeg;
	else if(name == "Use Clean Points When Following Strategy")
		return cleanPoints;
	else if(name == "Need Compute Density")
		return needDensity;
	else if(name=="Use Compute Eigen Ignore Branch Strategy")
		return useCEIBS;
	else if(name == "Use Plus Perpendicular Dist Strategy")
		return usePlusPDS;
	else if(name == "Use Kill Too Close Strategy")
		return useKillTooCloseS;
	else if(name == "Use Virtual Group Merge Strategy")
		return useVirtGrpMergeS;
	else if(name == "Use Nearby Combine Strategy")
		return useNearbyCombineS;
	else if(name == "Use Virtual Near Body Stop Strategy")
		return useVNearBodyStopS;
}

int SkeleParameterSet::getInt(string name)
{
	//string name(pname);
	if(name=="Fix Original Mode")
		return fixOrigMode;
}

double SkeleParameterSet::getDouble(string name)
{
	//string name(pname);
	if(name=="Stop And Grow Error")
		return stopAndGrowError;
	else if(name=="Max Iterate Time")
		return maxIterateTime;
	else if(name=="Max Stop Radius")
		return maxStopRadius;
	else if(name=="CGrid Radius")
		return cGridRadius;
	else if(name=="Accept Branch Size")
		return acceptBranchSize;
	else if(name=="Average Power")
		return avgPower;
	else if(name=="Fix Original Weight")
		return fixOrigWt;
	else if(name=="H Gaussian Para")
		return hGaussian;
	else if(name=="Repulsion Power")
		return repPower;
	else if(name=="Repulsion Mu")
		return repMu;
	else if(name=="Repulsion Mu2")
		return repMu2;
	else if(name=="Combine Too Close Threshold")
		return combineTooCloseT;
	else if(name=="Current Movement Error")
		return currMovError;
	else if(name=="Sigma KNN")
		return sigmaKNN;
	else if(name=="Eigen Feature Identification Threshold")
		return eigenFeatureIdT;
	else if(name=="Branch Search KNN")
		return branchKNN;
	else if(name=="Add Accept Branch Size")
		return addSize;
	else if(name=="Initial Radius")
		return initRadius;
	else if(name=="Snake Search Max Dist Blue")
		return snakeSearchMaxDist;
	else if(name=="Branch Search Max Dist Yellow")
		return branchSearchMaxDist;
	else if(name=="Branches Search Angle")
		return branchSearchAngle;
	else if(name=="Grow Search Radius")
		return growSearchRadius;
	else if(name=="Grow Accept Sigma")
		return growAcceptSigma;
	else if(name=="Virtual Head Accept Angle")
		return virtHeadAccAngle;
	else if(name=="Save Virtual Angle")
		return saveVirtualAngle;
	else if(name=="Branches Merge Max Dist")
		return branchMergeMaxD;
	else if(name=="Combine Similar Angle")
		return combineSimAngle;
	else if(name=="Clean Near Branches Dist")
		return clNearBranchDist;
	else if(name=="Local Density Radius")
		return localDensityRadius;
	else if(name=="Radius Update Speed")
		return radiusUpdateSpeed;
	else if(name=="Follow Sample Radius")
		return followSampleRadius;
	else if(name=="Follow Sample Max Angle")
		return followSampleMaxAngle;
	else if(name=="Bad Virtual Angle")
		return badVirtualAngle;
	else if(name=="Curve Segment Length")
		return curveSegmentLength;
	else if(name=="Init Radius Para")
		return initRadiusPara;
	else if(name=="Down Sample Num")
		return downSampleNum;
}

void SkeleParameterSet::setBool(string name, bool val)
{
	//string name(pname);
	if(name == "Run Auto Wlop One Step")
		runAutoWlop1Step = val;
	else if(name == "Run Auto Wlop One Stage")
		runAutoWlop1Stage = val;
	else if(name == "Step1 Detect Skeleton Feature")
		step1 = val;
	else if(name == "Step2 Search New Branches")
		step2 = val;
	else if(name == "Step3 Clean And Update Radius")
		step3 = val;
	else if(name == "Run ALL Segment")
		runAllSeg = val;
	else if(name == "The Skeletonization Process Should Stop")
		skeleStop = val;
	else if(name == "Max Iterate Time")
		maxIterateTime = val;
	else if(name == "Max Stop Radius")
		maxStopRadius = val;
	else if(name == "Need Segment Right Away")
		needSeg = val;
	else if(name == "Use Clean Points When Following Strategy")
		cleanPoints = val;
	else if(name == "Need Compute Density")
		needDensity = val;
	else if(name=="Use Compute Eigen Ignore Branch Strategy")
		useCEIBS = val;
	else if(name == "Use Plus Perpendicular Dist Strategy")
		usePlusPDS = val;
	else if(name == "Use Kill Too Close Strategy")
		useKillTooCloseS = val;
	else if(name == "Use Virtual Group Merge Strategy")
		useVirtGrpMergeS = val;
	else if(name == "Use Nearby Combine Strategy")
		useNearbyCombineS = val;
	else if(name == "Use Virtual Near Body Stop Strategy")
		useVNearBodyStopS = val;
	else
		cout<<"Unneeded Parameter: "<<name<<endl;
}

void SkeleParameterSet::setDouble(string name, double val)
{
	//string name(pname);
	if(name=="Stop And Grow Error")
		stopAndGrowError = val;
	else if(name=="Max Iterate Time")
		maxIterateTime = val;
	else if(name=="Max Stop Radius")
		maxStopRadius = val;
	else if(name=="CGrid Radius")
		cGridRadius = val;
	else if(name=="Accept Branch Size")
		acceptBranchSize = val;
	else if(name=="Average Power")
		avgPower = val;
	else if(name=="Fix Original Weight")
		fixOrigWt = val;
	else if(name=="H Gaussian Para")
		hGaussian = val;
	else if(name=="Repulsion Power")
		repPower = val;
	else if(name=="Repulsion Mu")
		repMu = val;
	else if(name=="Repulsion Mu2")
		repMu2 = val;
	else if(name=="Combine Too Close Threshold")
		combineTooCloseT = val;
	else if(name=="Current Movement Error")
		currMovError = val;
	else if(name=="Sigma KNN")
		sigmaKNN = val;
	else if(name=="Eigen Feature Identification Threshold")
		eigenFeatureIdT = val;
	else if(name=="Branch Search KNN")
		branchKNN = val;
	else if(name=="Add Accept Branch Size")
		addSize = val;
	else if(name=="Initial Radius")
		initRadius = val;
	else if(name=="Snake Search Max Dist Blue")
		snakeSearchMaxDist = val;
	else if(name=="Branch Search Max Dist Yellow")
		branchSearchMaxDist = val;
	else if(name=="Branches Search Angle")
		branchSearchAngle = val;
	else if(name=="Grow Search Radius")
		growSearchRadius = val;
	else if(name=="Grow Accept Sigma")
		growAcceptSigma = val;
	else if(name=="Virtual Head Accept Angle")
		virtHeadAccAngle = val;
	else if(name=="Save Virtual Angle")
		saveVirtualAngle = val;
	else if(name=="Branches Merge Max Dist")
		branchMergeMaxD = val;
	else if(name=="Combine Similar Angle")
		combineSimAngle = val;
	else if(name=="Clean Near Branches Dist")
		clNearBranchDist = val;
	else if(name=="Local Density Radius")
		localDensityRadius = val;
	else if(name=="Radius Update Speed")
		radiusUpdateSpeed = val;
	else if(name=="Follow Sample Radius")
		followSampleRadius = val;
	else if(name=="Follow Sample Max Angle")
		followSampleMaxAngle = val;
	else if(name=="Bad Virtual Angle")
		badVirtualAngle = val;
	else if(name=="Curve Segment Length")
		curveSegmentLength = val;
	else if(name=="Init Radius Para")
		initRadiusPara = val;
	else if(name=="Down Sample Num")
		downSampleNum = val;
	else
		cout<<"Unneeded Parameter: "<<name<<endl;
}

void SkeleParameterSet::setInt(string name,int val)
{
	//string name(pname);
	if(name=="Fix Original Mode")
		fixOrigMode = val;
}