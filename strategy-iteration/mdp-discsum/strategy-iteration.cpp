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

map<int, long double> V;
map<int, int> sigma;

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
	
	//start of strategy iteration
	for(auto u:choose_vertices)
	{
		if(successors[u].size()==0)
		{
			successors[u].insert(u);
			edge_reward[mp(u,u)]=0;
			edge_prob[mp(u,u)]=0;
		}
		V[u]=0;
		sigma[u]=*successors[u].begin();
	}
	for(auto u:probability_vertices)
	{
		if(successors[u].size()==0)
		{
			successors[u].insert(u);
			edge_reward[mp(u,u)]=0;
			edge_prob[mp(u,u)]=0;
		}
		V[u]=0;
	}
	
	//iterate
	bool converged=false;
	while(!converged)
	{
		//cerr<<"NEW ITERATION"<<endl;
		converged = true;
		
		//update the policy
		for(auto u:choose_vertices)
			for(auto v:successors[u])
				if(edge_reward[mp(u,v)]+lambda*V[v]>edge_reward[mp(u, sigma[u])]+lambda*V[sigma[u]]+1e-9)
				{
					converged = false;
					sigma[u]=v;
					//cerr<<"sigma["<<u<<"]="<<v<<endl;
				}
		
		//update the values until convergence
		bool val_converged = false;
		while(!val_converged)
		{
			val_converged = true;
			
			for(auto u:choose_vertices)
			{
				auto prev = V[u];
				V[u] = edge_reward[mp(u, sigma[u])] + lambda * V[sigma[u]];
				if(abs(V[u]-prev)>1e-9)
					val_converged=false;
			}
			
			for(auto u:probability_vertices)
			{
				auto prev = V[u];
				V[u]=0;
				for(auto v:successors[u])
					V[u]+= edge_prob[mp(u,v)] * (edge_reward[mp(u,v)] + lambda * V[v]);
				if(abs(V[u]-prev)>1e-9)
					val_converged = false;
			}
		}
	}
	
	auto stop = high_resolution_clock::now(); 
	
	auto duration = duration_cast<microseconds>(stop - start); 
	cout<<fixed<<setprecision(6)<<duration.count()/double(1e6)<<endl;
	
	return 0;
}