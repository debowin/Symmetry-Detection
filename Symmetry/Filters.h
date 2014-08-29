#pragma once
#include "CMesh.h"
#include "Electors.h"
#include "DataMgr.h"
#include "Algorithm/Skeleton.h"
#include "GlobalFunction.h"
#include <algorithm>
#include <fstream>
#include <sstream>

#define MAX_BRANCH 50
#define MAX_INDEX 100
#define MAX_NODES 500
#define SIZE 100

class Filters
{
public:
	Filters(DataMgr *pdata)
	{
		//if(!pdata->isSamplesEmpty() && !pdata->isOriginalEmpty())
		//{
			CMesh* _samples = pdata->getCurrentSamples();
			CMesh* _original = pdata->getCurrentOriginal();
			Skeleton* _skeleton = pdata->getCurrentSkeleton();

			samples = _samples;
			original = _original;
			skel = _skeleton;
		//}
		//else
			//cout << "ERROR: DataSet empty!!" << endl;
		memset(visited, 0, sizeof(visited));
		memset(branch_connect, -1, sizeof(branch_connect));
		computeTerminalBridge();
		max_l2 = 15.0799;
		nc2 = 4950;
		max_sdf = 0.484472;
	}
	~Filters()
	{
		cout<<"Filters destroyed!!\n";
	}
	void voting_mapping();
	void scm();
	void saveNeighbors(string);
	void import_neighbors(string);
	void computeTerminalBridge();
	void exportSkeleton(string);
	void showTerminals()
	{
		for(int i=0;i<terminal_nodes.size();i++)
			cout<<terminal_nodes[i].P()[0]<<" "<<terminal_nodes[i].P()[1]<<" "<<terminal_nodes[i].P()[2]<<" "<<endl;
	}
	void showBridges()
	{
		for(int i=0;i<bridge_nodes.size();i++)
			cout<<bridge_nodes[i].P()[0]<<" "<<bridge_nodes[i].P()[1]<<" "<<bridge_nodes[i].P()[2]<<" "<<endl;
	}
	void showConnect()
	{
		for(int i=0;i<skel->branches.size();i++)
		{
			for(int j=0;j<skel->branches.size();j++)
			{
				cout<<branch_connect[i][j]<<"\t";
			}
			cout<<"\n";
		}
	}
	double geoDistance(CVertex,CVertex);
	void testgeod(int a,int b,int c,int d)
	{
		cout<<"Sample Distance\n"<<geoDistance(skel->branches[a].curve[b],skel->branches[c].curve[d])<<endl;
	}
	void testgeod()
	{
		for(int i=0;i<skel->branches.size();i++)
		{
			for(int j=0;j<skel->branches[i].curve.size();j++)
			{
				for(int p=0;p<skel->branches.size();p++)
				{
					for(int q=0;q<skel->branches[p].curve.size();q++)
					{
						cout<<"Sample Distance b/w Node "<<node_index(i,j)<<" and Node "<<node_index(p,q)<<"\n"<<geoDistance(skel->branches[i].curve[j],skel->branches[p].curve[q])<<endl;
					}
				}
			}
		}
	}
	bool find_path(CVertex,CVertex);
	double path_distance();
	bool similarity_filter(CVertex,CVertex);
	void filter1_threshold(); //Calculate threshold for Filter 1
	//bool branch_filter(CVertex,CVertex); Not needed, done implicitly
	bool distance_profile_filter(CVertex,CVertex); //3rd filter
	void filter3_threshold(); //Calculate threshold for Filter 3
	bool symmetry_support_filter(CVertex,CVertex); //4th filter
	void filter4_threshold(); //Calculate threshold for Filter 4
	void perform_filters(); //The main function that performs all 4 filters
	int node_index(int,int);
private:
		Skeleton *skel;
		CMesh* samples;
		CMesh* original;

		int branch_connect[MAX_BRANCH][MAX_BRANCH]; //Connectivity Info
		//For Geodesic Distance
		vector<Point3f> traversal;
		vector<Point3f> best_traversal;
		bool visited[MAX_BRANCH][MAX_INDEX];

		vector<CVertex> terminal_nodes;
		vector<CVertex> bridge_nodes;
		vector<CVertex> neighbors[MAX_NODES];

		vector<Electors> elect;

		double max_l2;
		double max_sdf;
		int nc2;
		int num_pairs_in_electors[50];
		double elector_id[50]; // stores intrinsic deviation
		double node_id[SIZE][SIZE];
		double distance_nodes(CVertex,CVertex);

		vector<CVertex> ref;//stores all the nodes
		int vote_matrix[SIZE][SIZE];
};

class triple {
public:
    vector<CVertex> x;
    vector<CVertex> y;
    float z;
    bool operator<(const triple &other) const {
        return 1;
	}
};

class triple2 {
public:
    vector<CVertex> x;
    vector<CVertex> y;  
    float z;
   bool operator<(const triple2 &other) const {
        return 1;
   }
};

class triple3 {
public:
    vector<CVertex> x;
    vector<CVertex> y;
    float z;
    bool operator<(const triple3 &other) const {
        return 1;
}
};