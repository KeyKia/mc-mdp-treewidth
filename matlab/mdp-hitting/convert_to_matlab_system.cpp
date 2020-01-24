#include <fstream>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
using namespace std;

set<int> vertices, prob_vertices, choose_vertices;
map<int, set<int> > successors;
map<pair<int,int>, double > edgeprob;
#define edge_prob edgeprob

int main(int argc, char **argv)
{
	ifstream cin(argv[1]);

	//read MDP input
	int cn, pn, m, target;
	cin>>cn>>pn>>m>>target;

	for(int i=0;i<cn;++i)
	  {
	    int u;
	    cin>>u;
	    choose_vertices.insert(u);
	    vertices.insert(u);
	  }

	for(int i=0;i<pn;++i)
	  {
	    int u;
	    cin>>u;
	    prob_vertices.insert(u);
	    vertices.insert(u);
	  }

	for(int i=0;i<m;++i)
	  {
	    int v, u, flag;
	    double p;
	    cin>>v>>u>>p>>flag;
	    //ignore the flag
	    successors[v].insert(u);
	    if(prob_vertices.find(v)!=prob_vertices.end())
	      edge_prob[make_pair(v, u)] = p;
	  }

	//end of reading MDP input


	//print the system

	//step 1: define the variables
	for(auto v:vertices)
	  cout<<"x"<<v<<" = optimvar('x"<<v<<"', 'LowerBound', 0, 'UpperBound', 1);"<<endl;

	//step 2: define the objective
	cout<<"prob = optimproblem('Objective', 0 ";
	for(auto v:vertices)
	  cout<<"+ x"<<v<<" ";
	cout<<", 'ObjectiveSense', 'min');"<<endl;

	//step 3: define the constraints
	int cnt=1;
	for(auto v:vertices)
	  if(v==target)
	    {
	      //cout<<"#target"<<endl;
	      cout<<"prob.Constraints.c"<<cnt++<<" = x"<<v<<" == 1;"<<endl;
	    }
	  else if(prob_vertices.find(v)!=prob_vertices.end())
	    {
	      //cout<<"#prb vertex "<<v<<endl;
	      cout<<"prob.Constraints.c"<<cnt++<<" = x"<<v<<" == 0 ";
	      for(auto u:successors[v])
		cout<<" + ("<<edge_prob[make_pair(v,u)]<<") * x"<<u<<" ";
	      cout<<";"<<endl;
	    }
	  else //choose vertex
	    {
	      //cout<<"#choose vertex "<<v<<endl;
	      for(auto u:successors[v])
		cout<<"prob.Constraints.c"<<cnt++<<" = x"<<v<<" - x"<<u<<" >= 0;"<<endl;
	    }

	//start timing
	cout <<" tTheVar = cputime;"<<endl;
	
	//step 4: convert problem
	cout<<"problem = prob2struct(prob);"<<endl;

	//step 5: solve it
	cout<<"[sol,fval,exitflag,output] = linprog(problem)"<<endl;

	//compute time
	cout<<"eTheVar = cputime - tTheVar;"<<endl;


	//print it
	cout<<"fprintf(2, \" %6.6f \\n\" ,eTheVar);"<<endl;


	//clear for the next system
	cout<<"clear;"<<endl;
	cout<<endl<<endl<<endl;
	
	return 0;
}
