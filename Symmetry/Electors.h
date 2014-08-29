#include<vector>
#include"CMesh.h"
using namespace std;

class Electors
{
public:
	void addpair(CVertex one,CVertex two)
	{
		first.push_back(one);
		second.push_back(two);
	}
	vector<CVertex> first;
	vector<CVertex> second;
	int size()
	{
		return first.size();
	}
	void clear()
	{
		first.clear();
		second.clear();
	}
};