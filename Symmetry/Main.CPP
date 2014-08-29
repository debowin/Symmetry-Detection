//Include Debo's dll files
#include<iostream>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include "Filters.h"
#include "rapidxml.hpp"
#include "DataMgr.h"   // for sampling
#include "Algorithm/Skeleton.h"   
#include "Algorithm/Skeletonization.h"   
#define MAX_SKELETON_ITERATE 500


using namespace std;

int main()
{
	SkeleParameterSet *para = new SkeleParameterSet();
	//Default Parameter Values
		para->setDouble("Down Sample Num",2000);
		para->setDouble("Init Radius Para",1.0);
		para->setDouble("Grow Search Radius",0.15);
		para->setDouble("CGrid Radius",0.2);

		para->setDouble("Repulsion Power", 1.0);
		para->setDouble("Average Power",2.0);
		para->setDouble("H Gaussian Para", 4);
		para->setDouble("Current Movement Error", 0.0);

		para->setDouble("Max Iterate Time", 55);
		para->setDouble("Stop And Grow Error", 0.0005);
		para->setDouble("Initial Radius", 0.1202);
		para->setDouble("Radius Update Speed", 0.5);

		para->setDouble("Repulsion Mu", 0.35);
		para->setDouble("Repulsion Mu2", 0.15);
		para->setDouble("Follow Sample Radius", 0.33);
		para->setDouble("Follow Sample Max Angle", 80);

		para->setDouble("Save Virtual Angle", 30);
		para->setDouble("Grow Accept Sigma", 0.8);
		para->setDouble("Bad Virtual Angle", 101);
		para->setDouble("Combine Too Close Threshold", 0.01);

		para->setDouble("Sigma KNN", 6);
		para->setDouble("Eigen Feature Identification Threshold", 0.901);
		para->setDouble("Branches Search Angle", 25);
		para->setDouble("Virtual Head Accept Angle", 25);

		para->setDouble("Snake Search Max Dist Blue", 0.4);
		para->setDouble("Accept Branch Size", 6);
		para->setDouble("Branch Search Max Dist Yellow", 0.1);
		para->setDouble("Branches Merge Max Dist", 0.08);

		para->setDouble("Branch Search KNN", 12);
		para->setDouble("Combine Similar Angle", 140);
		para->setDouble("Grow Search Radius", 0.15);
		para->setDouble("Add Accept Branch Size", 1);

		para->setDouble("Local Density Radius",0.16);
		para->setDouble("Clean Near Branches Dist", 0.05);
		para->setDouble("Fix Original Weight", 0.91);
		para->setDouble("Curve Segment Length", 0.051);
		para->setDouble("Max Stop Radius", 1.99);

		para->setInt("Fix Original Mode", 4);

		para->setBool("Run ALL Segment", false); //Check
		para->setBool("Need Segment Right Away", true);
		para->setBool("Use Nearby Combine Strategy", true);
		para->setBool("Use Clean Points When Following Strategy", true);
		para->setBool("Use Plus Perpendicular Dist Strategy", false);
		para->setBool("Use Kill Too Close Strategy", false);
		para->setBool("Use Compute Eigen Ignore Branch Strategy", true);
		para->setBool("Use Virtual Group Merge Strategy", false);
		para->setBool("Use Virtual Near Body Stop Strategy", true);
		para->setBool("Need Compute Density", true); 
		para->setBool("Run Auto Wlop One Step", false); //Check
		para->setBool("Run Auto Wlop One Stage", false); //Check
		para->setBool("The Skeletonization Process Should Stop", false);
		para->setBool("Step1 Detect Skeleton Feature", false); //Check
		para->setBool("Step2 Search New Branches", false); //Check
		para->setBool("Step3 Clean And Update Radius", false); //Check
		
	//.skel load
	bool load_skel;
	cout<<"Load existing .skel file?(1/0)"<<endl;
	cin>>load_skel;
	string skelfile;
	if(load_skel)
	{
		DataMgr *sdata = new DataMgr();
		cout<<"Enter .skel filename: ";
		cin>>skelfile;
		sdata->loadSkeletonFromSkel(skelfile + "_skeleton.skel");
		Filters filt(sdata);
		cout<<"Bridge Nodes:\n";
		filt.showBridges();
		cout<<"Terminal Nodes:\n";
		filt.showTerminals();
		cout<<"Connectivity Info:\n";
		filt.showConnect();
		//cout<<"Importing neighbouring points information...\n";
		//filt.import_neighbors(skelfile+"_neigh.obj");
		cout<<"Performing Filters...\n";
		filt.perform_filters();
		cout<<"Electors voting...\n";
		filt.voting_mapping();
		cout<<"Symmetry Correspondence Matrix calculation...\n";
		filt.scm();
		//filt.testgeod();
		/*int br1,in1,br2,in2;
		while(1){cout<<"Geodesic Distance\nEnter 1st vertex branch and index: ";
		cin>>br1>>in1;
		cout<<"Enter 2nd vertex branch and index: ";
		cin>>br2>>in2;
		filt.testgeod(br1,in1,br2,in2);
		}*/
		_getch();
		return 0;
	}
	string fname,ext;
	cout<<"Enter mesh filename: "; //Temporary filename
	cin>>fname;
	ext = fname.substr(fname.length()-3,3);
	if(ext=="obj")
		cout<<"Extension: "<<ext<<endl;
	else
	{
		cout<<"\nError : Please use .obj files only!\n";
		exit(1);
	}
	bool willNormalize;
	cout<<"Do you want to normalize the mesh?(1/0)\n";
	cin>>willNormalize;
	//Include Debo's file here...
	try
    {
		DataMgr *data = new DataMgr(para);
		Skeletonization sklz(para);
		data->loadMeshToSample(fname);
		cout<<"Mesh loaded to Samples\n";
		if(willNormalize)
			data->normalizeAllMesh();
		data->downSamplesByNum();
		cout<<"Down Sampled\n";
		sklz.setFirstIterate();
		cout<<"Input provided to Skeletonization Phase\n";

		for (int i = 0; i < MAX_SKELETON_ITERATE; i++)
		{
			para->setBool("Run Auto Wlop One Step", true);
			sklz.setInput(data);
			sklz.run();
			sklz.clear();
			if(para->getBool("The Skeletonization Process Should Stop"))
			{
				break;
			}
		}
		cout<<"Skeletonization Complete!\n";
		data->saveSkeletonAsSkel(fname.substr(0,fname.size()-4) + "_skeleton.skel");
		cout<<"Operation details saved as "+fname.substr(0,fname.size()-4)+"_skeleton.skel\n";
		data->eraseRemovedSamples();
		data->loadMeshToSample("samples.obj");
		data->saveObj(fname.substr(0,fname.size()-4) + "_samples.obj",*data->getCurrentSamples());
		cout<<"Modified samples exported as "<<fname.substr(0,fname.size()-4) + "_samples.obj\n";
		//Filters begin here
		Filters filter(data);
		filter.exportSkeleton(fname.substr(0,fname.size()-4) + "_skeleton.obj");
		cout<<"Skeleton exported as "<<fname.substr(0,fname.size()-4) + "_skeleton.obj\n";
		filter.showBridges();
		filter.showTerminals();
		filter.showConnect();
		filter.saveNeighbors(fname.substr(0,fname.size()-4) + "_neigh.obj");
		cout<<"Neighbouring point information saved in "<<fname.substr(0,fname.size()-4) + "_neigh.obj\n";
    }
    catch (const invalid_argument &e)
    {
        cout << "Caught exception: " << e.what() << endl;
    }
	_getch();
    return 0;
}