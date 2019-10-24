#include <fstream>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
using namespace std;

set<int> vertices;
map<int, set<int> > successors;
map<pair<int,int>, double > edgeprob;
map<pair<int,int>, int> edgereward;

int main(int argc, char **argv)
{
	ifstream cin(argv[1]);
	/*string outpath = argv[1];
	outpath = outpath.substr(0, outpath.length()-3);
	outpath+="m";
	ofstream cout(outpath.c_str());*/
	
	int temp, e;
	double lambda;
	cin>>temp>>e>>lambda;
	
	while(e--)
	{
		int u, v;
		double p;
		int r;
		cin>>u>>v>>p>>r;
		
		if(p<1e-9)
			continue;
		
		vertices.insert(u);
		vertices.insert(v);
		successors[u].insert(v);
		edgeprob[make_pair(u,v)] = p;
		edgereward[make_pair(u,v)] = r;
	}
	
	cout<<"syms ";
	
	for(auto v:vertices)
		cout<<"x"<<v<<" ";
	
	cout<<endl;
	
	for(auto v:vertices)
	{
		cout<<"eqn"<<v<<" = x"<<v<<" == ";
		
		if(successors[v].size()==0)
			cout<<"0;";
		else
		{
			cout<<" 0 ";
			for(auto w:successors[v])
				cout<<" + ("<<edgeprob[make_pair(v,w)]<<")*("<<edgereward[make_pair(v,w)]<<") + ("<<edgeprob[make_pair(v,w)]<<") * "<<lambda<<" * x"<<w;
			cout<<";";
		}
		
		cout<<endl;
	}

	cout <<" tTheVar = cputime;"<<endl;
	
	// [A,B] = equationsToMatrix([eqn1, eqn2, eqn3], [x, y, z])

	cout<<endl<<endl;
	cout<<"[ATheVar,BTheVar] = equationsToMatrix([";
	
	bool first=true;
	for(auto v:vertices)
	{
		if(first)
			first=false;
		else
			cout<<", ";
		cout<<"eqn"<<v;
	}
	cout<<"], [";
	
	first=true;
	for(auto v:vertices)
	{
		if(first)
			first=false;
		else
			cout<<", ";
		cout<<"x"<<v;
	}
	
	cout<<"]);"<<endl;
	
	cout<<"XTheVar = ATheVar\\BTheVar;"<<endl;
	
	cout<<"eTheVar = cputime - tTheVar;"<<endl;
	
	cout<<"fprintf(2, \"%6.6f\\n\" ,eTheVar);"<<endl;
	
	cout<<endl<<"clear;"<<endl<<endl<<endl;
	
	return 0;
}