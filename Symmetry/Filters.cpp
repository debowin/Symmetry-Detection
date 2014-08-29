#include "Filters.h"
#include<math.h>
#include<algorithm>
#include<conio.h>

using namespace std;


void Filters::computeTerminalBridge()
{	
	//Find terminal and branch nodes
	for(int i=0;i<skel->branches.size();i++)
	{
		int last_index = skel->branches[i].curve.size()-1;
		CVertex& current_first = skel->branches[i].curve[0];
		CVertex& current_last = skel->branches[i].curve[last_index];
		bool first_not_terminal = false;
		bool last_not_terminal = false;
		for(int j=0;j<skel->branches.size();j++)
		{
			if(j==i)
				continue;
			last_index = skel->branches[j].curve.size()-1;
			if(current_first==skel->branches[j].curve[0] || current_first==skel->branches[j].curve[last_index])
			{
				vector<CVertex>::iterator it;
				it = find(bridge_nodes.begin(),bridge_nodes.end(),current_first);
				if(it==bridge_nodes.end())
				{
					bridge_nodes.push_back(current_first);
					branch_connect[i][j] = bridge_nodes.capacity()-1;
					branch_connect[j][i] = bridge_nodes.capacity()-1;
				}
				else
				{
					branch_connect[i][j] = distance(bridge_nodes.begin(), it);
					branch_connect[j][i] = distance(bridge_nodes.begin(), it);
				}
				first_not_terminal = true;
			}
			if(current_last==skel->branches[j].curve[0] || current_last==skel->branches[j].curve[last_index])
			{
				vector<CVertex>::iterator it;
				it = find(bridge_nodes.begin(),bridge_nodes.end(),current_last);
				if(it==bridge_nodes.end())
				{
					bridge_nodes.push_back(current_last);
					branch_connect[i][j] = bridge_nodes.capacity()-1;
					branch_connect[j][i] = bridge_nodes.capacity()-1;
				}
				else
				{
					branch_connect[i][j] = distance(bridge_nodes.begin(), it);
					branch_connect[j][i] = distance(bridge_nodes.begin(), it);
				}
				last_not_terminal = true;
			}
		}
		if(first_not_terminal==false)
			terminal_nodes.push_back(current_first);
		if(last_not_terminal==false)
			terminal_nodes.push_back(current_last);
	}
}

void Filters::exportSkeleton(string filename)
{
	fstream out;
	out.open(filename,ios::out);
	for(int i=0;i<skel->branches.size();i++)
	{
		for(int j=0;j<skel->branches[i].curve.size();j++)
		{
			CVertex& thisv = skel->branches[i].curve[j];
			if(find(bridge_nodes.begin(),bridge_nodes.end(),thisv)!=bridge_nodes.end())
				out<<"#bridge\nv "<<thisv.P()[0]<<" "<<thisv.P()[1]<<" "<<thisv.P()[2]<<" 0 255 0\n";
			else if(find(terminal_nodes.begin(),terminal_nodes.end(),thisv)!=terminal_nodes.end())
				out<<"#terminal\nv "<<thisv.P()[0]<<" "<<thisv.P()[1]<<" "<<thisv.P()[2]<<" 255 0 0\n";
			else
				out<<"v "<<thisv.P()[0]<<" "<<thisv.P()[1]<<" "<<thisv.P()[2]<<"\n";
		}
	}
	out.close();
}

double Filters::geoDistance(CVertex src,CVertex dst)
{
	vector<int> src_br,dst_br;
	vector<int> src_in,dst_in;
	if(src==dst)
		return 0.0f;
	traversal.clear();
	best_traversal.clear();
	memset(visited, 0, sizeof(visited));
	bool found = find_path(src,dst);
	if(!found)
		return 0;
	//cout<<"Path from source to dest stored in path.obj...\n";
	fstream pathfile;
	pathfile.open("path.obj",ios::out);
	for(int i=0;i<best_traversal.size();i++)
	{
		pathfile<<"v "<<best_traversal[i][0]<<" "<<best_traversal[i][1]<<" "<<best_traversal[i][2]<<" 0 0 255\n";
	}
	return path_distance();
}

bool Filters::find_path(CVertex src,CVertex dst)
{
	stack<CVertex> vstack;
	stack<CVertex> tstack;
	vstack.push(src);
	CVertex current;
	vector<int> cur_branch,cur_index;
	while(!vstack.empty())
	{
		current = vstack.top();
		if(current==dst)
		{
			traversal.push_back(current.P());
			if(traversal.size()<best_traversal.size() || best_traversal.size()==0)
				best_traversal = traversal;
			if(best_traversal.size()==2)
				break;
			//Backtrack to last checkpoint
			vstack.pop();
			traversal.pop_back();
			while(vstack.top().P()!=traversal[traversal.size()-1])
			{
				tstack.push(vstack.top());
				vstack.pop();
			}
			vstack.pop();
			while(!tstack.empty())
			{
				vstack.push(tstack.top());
				tstack.pop();
			}
			traversal.pop_back();
			continue;
		}
		cur_branch.clear();
		cur_index.clear();
		//Find out branches and indices of current node
		for(int i=0;i<skel->branches.size();i++)
		{
			vector<CVertex>::iterator it = find(skel->branches[i].curve.begin(),skel->branches[i].curve.end(),current);
			if(it!=skel->branches[i].curve.end())
			{
				visited[i][distance(skel->branches[i].curve.begin(),it)] = 1;
				cur_index.push_back(distance(skel->branches[i].curve.begin(),it));
				cur_branch.push_back(i);
			}
		}
		if(traversal.size()==0 || traversal[traversal.size()-1]!=current.P())
			traversal.push_back(current.P());
		//Search everywhere
		bool useless = true;
		for(int i=0;i<cur_branch.size();i++)
		{
			if((cur_index[i]!=skel->branches[cur_branch[i]].curve.size()-1) && (visited[cur_branch[i]][cur_index[i]+1]==false)) //If right node not visited
			{
				useless = false;
				vstack.push(skel->branches[cur_branch[i]].curve[cur_index[i]+1]);
			}
			if((cur_index[i]!=0) && (visited[cur_branch[i]][cur_index[i]-1]==false)) //If left node not visited
			{
				useless = false;
				vstack.push(skel->branches[cur_branch[i]].curve[cur_index[i]-1]);
			}
		}
		if(useless==true)
		{
			vstack.pop();
			traversal.pop_back();
		}
	}
	if(best_traversal.size()!=0)
		return true;
	return false;
}

double Filters::path_distance()
{
	Point3f current = best_traversal[0];
	double distance = 0.0;
	for(int i=1;i<best_traversal.size();i++)
	{
		distance += sqrt(pow(current[0]-best_traversal[i][0],2)+pow(current[1]-best_traversal[i][1],2)+pow(current[2]-best_traversal[i][2],2));
		current = best_traversal[i];
	}
	return distance;
}

void Filters::filter1_threshold()
{
	cout<<"Calculating threshold for Similarity Filter...\n";
	max_sdf = -999.0;
	for(int i=0;i<skel->branches.size();i++)
	{
		for(int j=0;j<skel->branches[i].curve.size();j++)
		{
			if(max_sdf<skel->branches[i].curve[j].skel_radius)
				max_sdf = skel->branches[i].curve[j].skel_radius;
		}
	}
	cout<<max_sdf<<endl;
}

bool Filters::similarity_filter(CVertex first, CVertex second)
{
	if(abs(first.skel_radius-second.skel_radius)<0.05*max_sdf)
		return true;
	else
		return false;
}

bool Filters::distance_profile_filter(CVertex first, CVertex second)
{
	vector<double> firstdp,seconddp,difference;
	double l2 = 0.0;
	for(int i=0;i<terminal_nodes.size();i++)
	{
		firstdp.push_back(geoDistance(first,terminal_nodes[i]));
		seconddp.push_back(geoDistance(second,terminal_nodes[i]));
	}
	//Sort distance profiles
	sort(firstdp.begin(),firstdp.end());
	sort(seconddp.begin(),seconddp.end());
	for(int i=0;i<firstdp.size();i++)
	{
		difference.push_back(abs(firstdp[i] - seconddp[i]));
		l2+=pow(difference[i],2);
	}
	if(l2<0.1*max_l2)
		return true;
	else
		return false;
}

void Filters::filter3_threshold()
{
	cout<<"Calculating threshold for Distance Profile Filter...\n";
	cout<<"[";
	max_l2 = -999.0;
	double l2 = 0.0;
	vector<double> dp1,dp2,diff;
	for(int i=0;i<skel->branches.size();i++)
	{
		cout<<"=";
		for(int j=0;j<skel->branches[i].curve.size();j++)
		{
			for(int p=i;p<skel->branches.size();p++)
			{
				for(int q=j+1;q<skel->branches[p].curve.size();q++)
				{
					CVertex first = skel->branches[i].curve[j];
					CVertex second = skel->branches[p].curve[q];
					if(first==second)
						continue;
					dp1.clear();
					dp2.clear();
					diff.clear();
					l2 = 0.0;
					for(int x=0;x<terminal_nodes.size();x++)
					{
						dp1.push_back(geoDistance(first,terminal_nodes[x]));
						dp2.push_back(geoDistance(second,terminal_nodes[x]));
					}
					//Sort distance profiles
					sort(dp1.begin(),dp1.end());
					sort(dp2.begin(),dp2.end());
					for(int x=0;x<dp1.size();x++)
					{
						diff.push_back(abs(dp1[x] - dp2[x]));
						l2+=pow(diff[x],2);
					}
					if(max_l2<l2)
						max_l2 = l2;
				}
			}
		}
	}
	cout<<"]\n"<<max_l2<<endl;
}

bool Filters::symmetry_support_filter(CVertex first, CVertex second)
{
	Electors el; //an object of the electors class created 
	el.clear(); //no pairs right now
	el.addpair(first,second); //Initialize current elector with values of pair
	for(int i=0;i<skel->branches.size()-1;i++) //i-j  &  p-q
	{
		for(int j=0;j<skel->branches[i].curve.size();j++)
		{
			for(int p=i+1;p<skel->branches.size();p++)
			{
				for(int q=0;q<skel->branches[p].curve.size();q++)
				{
					CVertex third = skel->branches[i].curve[j];
					CVertex fourth = skel->branches[p].curve[q];
					if((first==third && second==fourth) || (first==fourth && second==third))
						continue;
					//cout<<"Symmetry Support Check: "<<node_index(i,j)<<" "<<node_index(p,q)<<endl;
					double geod1 = geoDistance(first,third);
					double geod2 = geoDistance(second,fourth);
					double geod3 = geoDistance(first,fourth);
					double geod4 = geoDistance(second,third);
					if((abs(geod1-geod2)<0.02*max(geod1,geod2)) && (abs(geod3-geod4)<0.02*max(geod3,geod4)))
					{
						//Add pair of third and fourth to current elector
						el.addpair(third,fourth);
					}
				}
			}
		}
	}
	if(el.size()>0.0001*nc2)
	{
		elect.push_back(el);
		return true;
	}
	else
		return false;
}

void Filters::filter4_threshold()
{
	cout<<"Calculating threshold for Symmetry Support Filter...\n";
	int n = 0;
	for(int i=0;i<skel->branches.size();i++)
		n+=skel->branches[i].curve.size();
	nc2 = (n*(n-1))/2;
	cout<<nc2<<endl;
}

void Filters::perform_filters()
{
	//calculates threshold values for comparison in all filters
	filter1_threshold();
	filter3_threshold();
	filter4_threshold();
	fstream f;
	
	f.open("filter_results.txt",ios::out); //Dump results of filter in file
	for(int i=0;i<skel->branches.size()-1;i++)
	{
		for(int j=0;j<skel->branches[i].curve.size();j++)
		{
			for(int p=i+1;p<skel->branches.size();p++)
			{
				for(int q=0;q<skel->branches[p].curve.size();q++)
				{
					CVertex node1 = skel->branches[i].curve[j];
					CVertex node2 = skel->branches[p].curve[q];
					if(node1==node2) //skip bridge nodes
						continue;
					//Siya removed this
					//cout<<"Filtering "<<node_index(i,j)<<" "<<node_index(p,q)<<endl;
					cout<<".";
					if(similarity_filter(node1,node2))
					{
						f<<"Similarity Filter passed: "<<node_index(i,j)<<" "<<node_index(p,q)<<endl;
						if(distance_profile_filter(node1,node2))
						{
							f<<"Distance Profile Filter passed: "<<node_index(i,j)<<" "<<node_index(p,q)<<endl;
							if(symmetry_support_filter(node1,node2))
							{
								f<<"Symmetry Support Filter passed: "<<node_index(i,j)<<" "<<node_index(p,q)<<endl;
							}
						}
					}
				}
			}
		}
	}
	cout<<"Filters completed successfully\nListing electors...\n";
	int j;
	for(int i=0;i<elect.size();i++)
	{
		cout<<"Elector "<<i<<":\n";
		for(j=0;j<elect[i].size();j++)
		{
			cout<<elect[i].first[j].P()[0]<<" "<<elect[i].first[j].P()[1]<<" "<<elect[i].first[j].P()[2]<<"\t"<<elect[i].second[j].P()[0]<<" "<<elect[i].second[j].P()[1]<<" "<<elect[i].second[j].P()[2]<<endl;			
		}
		num_pairs_in_electors[i]=j;
	}

	/****SIYA****/
	//For electors vote matrix
	cout<<"\nCalculating intrinsic deviation...";

	int neic2;
	double total_elector_dist=0.0;
	double final_dist;
	int no_of_electors = elect.size();
	cout<<"\nNumber of electors : "<<no_of_electors;
	/*Printing the number of node pairs for each elector
	for(int k=0;k<no_of_electors;k++)
	{
		cout<<"\nelector no "<<k<<" : "<<num_pairs_in_electors[k];
	}*/
	for(int i=0;i<elect.size();i++)
	{
		for(j=0;j<elect[i].size();j++)
		{
			for(int k=0;k<elect[i].size();k++)
			{
				if(k==j)
				{
					break;
				} 
				else //compare with every other node
				{
					double dist1;
					double dist2;
					dist1 = geoDistance(elect[i].first[j],elect[i].first[k]);
					dist2 = geoDistance(elect[i].second[j],elect[i].second[k]);
					double diff1 = dist1 - dist2;
					//cout<<diff1<<" ";

					double dist3;
					double dist4;
					dist3 = geoDistance(elect[i].first[j],elect[i].second[k]);
					dist4 = geoDistance(elect[i].second[j],elect[i].first[k]);
					double diff2 = dist3 - dist4;
					//cout<<diff2<<" ";

					final_dist = max(diff1,diff2);
					//cout<<" Dist "<<j<<" :"<<final_dist<<endl;
				} //end else
				total_elector_dist = total_elector_dist + final_dist;
			}//k loop
		}//j loop
		if(num_pairs_in_electors[i]==1)
		{ neic2 = 1;}
		else
		{ neic2 = num_pairs_in_electors[i]*(num_pairs_in_electors[i]-1)/2;}
		//cout<<"pairs"<<num_pairs_in_electors[i];
		//cout<<"boo-->  "<<neic2<<endl;
		elector_id[i]=total_elector_dist / neic2;
	} //elector for loop

	for(int b=0;b< no_of_electors;b++)
		cout<<"\nIntrinsic deviation : "<<elector_id[b];
	
}//end function

//maps the voting matrix
void Filters ::voting_mapping()
{
	for(int i=0;i<skel->branches.size();i++)
	{
		for(int j=0;j<skel->branches[i].curve.size();j++)
		{
			CVertex& vert = skel->branches[i].curve[j];
			ref.push_back(vert);
			//cout<<endl<<vert.P()[0]<<" "<<vert.P()[1]<<" "<<vert.P()[2];
		}
	}
	cout<<endl;
	vote_matrix[ref.size()][ref.size()]; //matrix initialized with the right size

	cout<<"\nSize of n * n matrix:::"<<ref.size();

	//initializing the matrix with zero values
	for(int i=0;i<ref.size();i++)
	{
		for(int j=0;j<ref.size();j++)
		{
			vote_matrix[i][j]=0;
			node_id[i][j]=0;
		}
	}
		cout<<endl;
	/*printing the vector
	 for(int i = 0; i < ref.size(); i++)
    {
		cout << ref[i].skel_radius << " "<<endl;
    }*/
	int row,col;
	//Voting procedure -- first=row & second=column
	for(int i=0;i<elect.size();i++)
	{
		for(int j=0;j<elect[i].size();j++)
		{
			row=col=-1;
				for(int k=0;k<ref.size();k++)
				{
					if(elect[i].first[j]==ref[k])
					{
						row=k;
						
					}

					if(elect[i].second[j]==ref[k])
					{
						col=k; 
						
					}
				}//end for loop
				//Bad cases
				if (row!=-1 && col!=-1)
				{
					cout<<"\n Row "<<row;
					cout<<" Col "<<col<<endl;
				}
	
				//Putting the vote value
				//****************Use Intrinsic Deviation values of each elector to decide its vote value(Talk to Sir about it)******************//
				if (row!=-1 && col!=-1)
				{
					    
					  if(node_id[row][col]==0) //this is its first elector
					   {
						   node_id[row][col]=elector_id[i];
					   }
					   else
					   {
						   if(elector_id[i] < node_id[row][col])
							   node_id[row][col]=elector_id[i];
					   }

						vote_matrix[row][col]+=1;
						vote_matrix[col][row]+=1;
				}
		
		}
	}

	int temp=vote_matrix[0][0];
	double scm_temp=node_id[0][0]; //highest value of node_id
	cout<<"\n Vote Matrix : \n";
	for(int i=0;i<ref.size();i++)
	{
		for(int j=0;j<ref.size();j++)
		{
			if(vote_matrix[i][j]>=temp)
				temp=vote_matrix[i][j];
			if(node_id[i][j]>=scm_temp)
				scm_temp=node_id[i][j];
			//cout<<vote_matrix[i][j]<<"\t";
		}
		//cout<<"\n";
	}

	cout<<endl<<"Highest value : "<<temp;
	
	
	float threshold=temp*0.1;
	cout<<endl<<"Threshold value : "<<threshold<<endl;
	
	float val;
	int count=0;
	for(int i=0;i<ref.size();i++)
	{
		for(int j=0;j<ref.size();j++)
		{
			//val=(float)vote_matrix[i][j];
			if(vote_matrix[i][j]>=threshold)
			{
				cout<<vote_matrix[i][j]<<"\t\t";
				count++;
			}
			else
			{
				vote_matrix[i][j]=0;
				cout<<vote_matrix[i][j]<<"\t\t";
			}

		}
		cout<<"\n";
	}
	cout<<endl<<"Count of voters matrix:"<<count/2;


	cout<<endl<<endl<<"SCM matrix";
	cout<<endl<<"Highest value in the scm wala (ID): "<<scm_temp;
	double threshold_scm=scm_temp*0.1;
	cout<<endl<<"Threshold value for scm wala (ID) : "<<threshold_scm<<endl;
	
	//For printing SCM
	for(int i=0;i<ref.size();i++)
	{
		for(int j=0;j<ref.size();j++)
		{
			//val=(float)vote_matrix[i][j];
			
			cout<<node_id[i][j]<<"\t\t";
		}
		cout<<"\n";
	}
	
	//******************Used the threshold value to shortlist which pairs of nodes have sufficient votes to be considered symmetric****************//
}//end function

//Get neighbors of sample points and store in a file
void Filters::saveNeighbors(string filename)
{
	ofstream neighfile;
	neighfile.open(filename.c_str());

	CVertex& thisv = skel->branches[0].curve[0];

	for(int i=0;i<skel->branches.size();i++)
	{
		for(int j=0;j<skel->branches[i].curve.size();j++)
		{
			CVertex& thisv = skel->branches[i].curve[j];
			for(int k=0;k<thisv.original_neighbors.size();k++)
			{
				neighfile<<"v "<<original->vert[thisv.original_neighbors[k]].P()[0]<<" "<<original->vert[thisv.original_neighbors[k]].P()[1]<<" "<<original->vert[thisv.original_neighbors[k]].P()[2]<<"\n";
			}
			neighfile<<"end\n";
		}
	}
}

//Reading 
void Filters::import_neighbors(string filename)
{
	ifstream neighfile;
	neighfile.open(filename.c_str());
	stringstream sem; 
	sem << neighfile.rdbuf(); 
    int k=0,l=0;
	string str;

	CVertex& thisv = skel->branches[0].curve[0];

	for(int i=0;i<skel->branches.size();i++)
	{
		for(int j=0;j<skel->branches[i].curve.size();j++)
		{
			CVertex neighbor;
			CVertex& thisv = skel->branches[i].curve[j];
			while(1)
			{
				sem >> str;
				if(str=="end")
					break;
				sem >> neighbor.P()[0] >> neighbor.P()[1] >> neighbor.P()[2];
				neighbors[k].push_back(neighbor);
			}
			k++;
		}
	}
}

//Distance function
double Filters::distance_nodes(CVertex a,CVertex b)
{
	float sum;
	sum= pow((a.P()[0]-b.P()[0]),2)+pow((a.P()[1]-b.P()[1]),2)+pow((a.P()[1]-b.P()[1]),2);
	return sqrt(sum);
}


//Symmetry correspondence Matrix starts here
//Input: Voter's matrix and the neighboring sample points data.
//Output gives the symmetry pair of sample points in sparse matrix form.
void Filters :: scm()
{
    std::map<triple,float> data;
	std::map<triple2,float> data2;
	std::map<triple3,float> data3;
    triple point;
	triple2 point2;
	triple3 point3;
    int i,j,k;
	float bw,temp1,temp3,temp4,maxd,dmax;
	vector<float> distance;
	vector<float> temp2;


	for(i=0;i<ref.size();i++){
    for (j = 0; j < ref.size(); j++) {
        point.x = neighbors[i];
        point.y = neighbors[j];
        point.z = node_id[i][j];
        //printf("%d %d %d %d\n", i, point.x, point.y, point.z);
		for(k=0;k<min(point.x.size(),point.y.size());k++)
		{
		if(distance_nodes(neighbors[i][k],ref[i]) < distance_nodes(neighbors[j][k],ref[i]))
		{
		if(distance_nodes(neighbors[i][k],ref[i]) > distance_nodes(neighbors[j][k],ref[j])) //Distance from sample to skel node
			distance.push_back(distance_nodes(neighbors[i][k],ref[i])); 
		else
			distance.push_back(distance_nodes(neighbors[j][k],ref[j]));
		}
		else
		{
			if(distance_nodes(neighbors[j][k],ref[i]) > distance_nodes(neighbors[i][k],ref[j])) //Distance from sample to skel node
				distance.push_back(distance_nodes(neighbors[j][k],ref[i]));
		else
			distance.push_back(distance_nodes(neighbors[i][k],ref[j]));
		}
		}
		if(distance[distance.size()-1] > maxd)
			maxd = distance[distance.size()-1];
		
		//printf("%d %d %d %d\n", i, point.x, point.y, point.z);
        data[point] = i;
    }
}
		cout<<"Distance:"<<maxd;
		_getch();
	for(i=0;i<ref.size();i++){
    for (j = 0; j < ref.size(); j++) {
        point.x = neighbors[i];
        point.y = neighbors[j];
        point.z = node_id[i][j];
       // printf("%d %d %d %d\n", i, point2.x, point2.y, point2.z);
		for(k=0;k<min(point.x.size(),point.y.size());k++)
		{
		temp1 = 1 - (distance[k]/maxd);
		bw = temp1 * temp1;
		temp2.push_back(bw * point2.z);
		if (temp2[temp2.size()-1]>dmax)
			dmax=temp2[temp2.size()-1];
		//printf("%d %d %d %d\n", i, point2.x, point2.y, point2.z);
        data2[point2] = i;
    }
	}
	}
	cout<<"DMAX:"<<dmax;
	_getch();
	for(i=0;i<ref.size();i++){
    for (j = 0; j < ref.size(); j++) {
        point.x = neighbors[i];
        point.y = neighbors[j];
        point.z = node_id[i][j];
		for(k=0;k<min(point.x.size(),point.y.size());k++)
		{
       // printf("%d %d %d %d\n", i, point3.x, point3.y, point3.z);
		temp3=sqr(temp2[k]/(0.01*dmax));
		temp4=-temp3;
		point3.z=exp(temp4);
		printf(" %f \n", point3.z);
        data3[point3] = i;
    }
}	
}
}

int Filters::node_index(int a,int b)
{
	for(int i=0;i<a;i++)
	{
		b+=skel->branches[i].curve.size();
	}
	return b;
}

