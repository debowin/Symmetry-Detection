#pragma once

#include "../DataMgr.h"
#include "../ParameterMgr.h"



class PointCloudAlgorithm
{
public:
	PointCloudAlgorithm(SkeleParameterSet _para){}
	virtual ~PointCloudAlgorithm(){}

	virtual void setInput(DataMgr* pData) = 0;
	virtual void setSkeleParameterSet(SkeleParameterSet* _para) = 0;
	virtual SkeleParameterSet* getSkeleParameterSet() = 0;
	virtual void run() = 0;
	virtual void clear() = 0;

protected:
	PointCloudAlgorithm(){}

private:
	SkeleParameterSet * para;
};