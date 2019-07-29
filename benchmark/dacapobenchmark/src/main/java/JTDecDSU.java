/*
 * This class provides support for Disjoint Set operations as needed by functions in other classes of the JTDec Package.
 * Due to its specialized features this is not a multi-purpose class and its use is discouraged elsewhere. 
 * Please send suggestions and bug reports to goharshady@ist.ac.at
 */

import java.util.HashMap;
import java.util.List;
import java.util.HashSet;
import java.util.ArrayList;

public class JTDecDSU { //implements a DSU data structure
 int[] parent; //stores parents of nodes
 int[] numberOfChildren; //stores number of descendants of each node
 HashMap<Integer, HashSet<Integer> > backCons; //Backward connections as in JTDec algorithms
 public JTDecDSU(int n) 
 {
	 parent = new int[n+3];
	 numberOfChildren = new int[n+3];
	 backCons = new HashMap<Integer, HashSet<Integer> >();
	 for(int i=0;i<=n;++i)
	 {
		 //initialize backCons
		 HashSet<Integer> thisBackCons = new HashSet<Integer>();
		 backCons.put(i,thisBackCons);
		 //initialize parent
		 parent[i]=i;
		 //initialize children
		 numberOfChildren[i]=1;
	 }
 }
 void pathCompression(int root, List<Integer> children) //connect all children directly to root
 {
	 for(int i:children)
		 parent[i]=root;
 }
 int getRoot(int vertex) //get the root of the current vertex
 {
	 List<Integer> path = new ArrayList<Integer>();
	 while(parent[vertex]!=vertex)
	 {
		 path.add(vertex);
		 vertex=parent[vertex];
	 }
	 pathCompression(vertex,path);
	 return vertex;
 }
 void merge(int a,int b) //merge two components. a and be can be any member of each of the components.
 {
	 a=getRoot(a);
	 b=getRoot(b);
	 if(a==b)
		 return;
	 if(numberOfChildren[a]<numberOfChildren[b])
	 {
		 //swap them
		 int c=b;
		 b=a;
		 a=c;
	 }
	 parent[b]=a;
	 numberOfChildren[a]+=numberOfChildren[b];
	 HashSet<Integer> currentBack = backCons.get(a);
	 HashSet<Integer> otherBack= backCons.get(b);
	 for(int i:otherBack)
		 currentBack.add(i);
	 backCons.put(a,currentBack);
 }
 void addBackCon(int left, int right) //add a backward connection
 {
	 List<Integer> path = new ArrayList<Integer>(); 
	 while(true)
	 {
		 path.add(right);
		 HashSet<Integer> currentBack = backCons.get(right);
		 currentBack.add(left);
		 backCons.put(right,currentBack);
		 
		 if(right==parent[right]) //got to the root
			 break;
		 right=parent[right]; //go up
	 }
	 pathCompression(right,path);
 }
 void removeBackCon(int left) //remove a backward connection
 {
	 int right=left;
	 List<Integer> path = new ArrayList<Integer>(); 
	 while(true)
	 {
		 path.add(right);
		 HashSet<Integer> currentBack = backCons.get(right);
		 currentBack.remove(left);
		 backCons.put(right,currentBack);
		 
		 if(right==parent[right]) //got to the root
			 break;
		 right=parent[right]; //go up
	 }
	 pathCompression(right,path);
 }
 HashSet<Integer> getBackCons(int root) //returns back connections
 {
	 root=getRoot(root);
	 return backCons.get(root);
 }
}