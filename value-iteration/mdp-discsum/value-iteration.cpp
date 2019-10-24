#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono; 


#define abs(x) (((x)>=0)?(x):(-(x)))
#define edge pair<int,int>
#define mp make_pair

set<int> choose_vertices;
set<int> probability_vertices;
map<int, set<int> > successors;
map<edge, int> edge_reward;
map<edge, long double> edge_prob;

map<int, long double> V[2];

int main(int argc, char **argv)
{
	
	ifstream cin(argv[1]);
	
	int cnt_choose, cnt_probability, cnt_edge;
	long double lambda;
	
	cin>>cnt_choose>>cnt_probability>>cnt_edge>>lambda;
	
	while(cnt_choose--)
	{
		int v;
		cin>>v;
		choose_vertices.insert(v);
	}
	
	while(cnt_probability--)
	{
		int v;
		cin>>v;
		probability_vertices.insert(v);
	}
	
	while(cnt_edge--)
	{
		int u,v;
		long double p;
		int r;
		int type;
		cin>>u>>v>>p>>r>>type;
		if(p<1e-9)
			continue;
		edge e = mp(u,v);
		successors[u].insert(v);
		//cout<<u<<" --> "<<v<<endl;
		if(type==0) //prob vertex
			edge_prob[e]=p;
		edge_reward[e]=r;
	}
	
	
	auto start = high_resolution_clock::now(); 
	
	//start of value iteration
	for(auto u:choose_vertices)
		V[0][u]=0;
	for(auto u:probability_vertices)
		V[0][u]=0;
	
	//iterate
	bool converged=false;
	for(int iter=0;!converged;iter++)
	{
		iter%=2;
		int previter = 1-iter;
		converged = true;
		
		for(auto u:probability_vertices)
		{
			V[iter][u] = 0;
			for(auto v:successors[u])
				V[iter][u] += edge_prob[mp(u, v)] * ( edge_reward[mp(u, v)] + lambda * V[previter][v] );
				
			if(abs(V[iter][u]-V[previter][u])>1e-8)
				converged = false;
		}
		
		for(auto u:choose_vertices)
		{
			V[iter][u]=-1e10;
			if(successors[u].size()==0)
				V[iter][u]=0;
			for(auto v:successors[u])
				V[iter][u] = max(V[iter][u], edge_reward[mp(u, v)] + lambda * V[previter][v]);
				
			if(abs(V[iter][u]-V[previter][u])>1e-8)
				converged = false;
		}
		
		/*if(1==1)
		{
			cout<<"NEW ITERATION ==============="<<endl;
			cout<<"The values are:"<<endl;
			for(auto u:choose_vertices)
				cout<<"V["<<u<<"]="<<V[iter][u]<<endl;
			for(auto u:probability_vertices)
				cout<<"V["<<u<<"]="<<V[iter][u]<<endl;
		}*/
		
	}
	
	auto stop = high_resolution_clock::now(); 
	
	auto duration = duration_cast<milliseconds>(stop - start); 
	cout<<fixed<<setprecision(6)<<duration.count()/double(1000)<<endl;
	
	return 0;
}