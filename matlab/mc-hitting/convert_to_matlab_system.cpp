#include <fstream>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

set<int> vertices;
map<int, set<int> > successors;
map<pair<int,int>, double > edgeprob;

int main(int argc, char **argv)
{
	ifstream cin(argv[1]);
	string outpath = argv[1];
	outpath = outpath.substr(0, outpath.length()-3);
	outpath+="m";
	ofstream cout(outpath.c_str());
	
	int temp, e, target;
	cin>>temp>>e>>target;
	
	while(e--)
	{
		int u, v;
		double p;
		cin>>u>>v>>p;
		
		if(p<1e-9)
			continue;
		
		vertices.insert(u);
		vertices.insert(v);
		successors[u].insert(v);
		edgeprob[make_pair(u,v)] = p;
	}
	
	cout<<"syms ";
	
	for(auto v:vertices)
		cout<<"x"<<v<<" ";
	
	cout<<endl;
	
	for(auto v:vertices)
	{
		cout<<"eqn"<<v<<" = x"<<v<<" == ";
		
		if(v==target)
			cout<<"1;";
		else if(successors[v].size()==0)
			cout<<"0;";
		else
		{
			cout<<" 0 ";
			for(auto w:successors[v])
				cout<<" + ("<<edgeprob[make_pair(v, w)]<<") * x"<<w<<" ";
			cout<<";";
		}
		
		cout<<endl;
	}

	cout <<" t = cputime;"<<endl;
	
	// [A,B] = equationsToMatrix([eqn1, eqn2, eqn3], [x, y, z])

	cout<<endl<<endl;
	cout<<"[A,B] = equationsToMatrix([";
	
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
	
	cout<<"X = A\\B;"<<endl;
	
	cout<<"e = cputime - t;"<<endl;
	
	cout<<"fprintf(2, \" %6.6f \" ,e);"<<endl;
	
	return 0;
}