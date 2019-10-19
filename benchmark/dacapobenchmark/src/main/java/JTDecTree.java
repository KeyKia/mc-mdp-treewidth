/*
 * Implements the tree structure needed to store tree decompositions obtained by JTDec
 * Note: some functions allow you to change the data so that it does not correspond to a real tree, in this case many operations are not
 * well-defined and will cause undefined behavior and/or different sorts of exceptions
 * Note 2: For the global numbers (height, root, maxWidth, etc.) to work, you need to run a traverseTree first. All processes update these automatically
 * except for the local changes, i.mcE. addVertex, addToBag, changeParent.
 * Please send suggestions and bug reports to goharshady@ist.ac.at
 */

import java.util.HashSet;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.List;
import java.util.ArrayList;

public class JTDecTree {
	int numberOfVertices; //number of vertices in this tree
	int maxNode; //the maximum integer that has been used to label a vertex of this tree
	int root; //the root, if well-defined
	int height; //height of the tree, well-defined only if connected
	int maxWidth; //maximum width among all mcBags
	HashMap<Integer,Integer> parent; //parents of nodes
	HashMap<Integer,HashSet<Integer>> children; //children of nodes
	HashMap<Integer,HashSet<Integer>> bag; //mcBags
	HashMap<Integer,Integer> rootBag; //rootBags of nodes of the original graph G
	HashMap<Integer,Integer> level; //levels of vertices in the tree
	void fillWithNull() //initializes everything to null
	{
		numberOfVertices=0;
		root=-1;
		height=0;
		parent = new HashMap<Integer,Integer>();
		children = new HashMap<Integer, HashSet<Integer>>();
		bag = new HashMap<Integer,HashSet<Integer>>();
		level = new HashMap<Integer,Integer>();
		rootBag = new HashMap<Integer,Integer>();
		maxNode=0;
		maxWidth=0;
	}
	public JTDecTree()
	{
		fillWithNull();
	}
	public JTDecTree(JTDecTree T) //copy constructor
	{
		fillWithNull();
		numberOfVertices = T.numberOfVertices;
		root = T.root;
		height = T.height;
		maxNode = T.maxNode;
		maxWidth = T.maxWidth;
		
		for(Entry<Integer,Integer> i:T.parent.entrySet())
			parent.put(i.getKey(), i.getValue());
		
		for(Entry<Integer,Integer> i:T.rootBag.entrySet())
			rootBag.put(i.getKey(), i.getValue());
		
		for(Entry<Integer,Integer> i:T.level.entrySet())
			level.put(i.getKey(),i.getValue());
		
		for(Entry<Integer,HashSet<Integer>> i:T.children.entrySet())
		{
			HashSet<Integer> c = new HashSet<Integer>(i.getValue());
			children.put(i.getKey(), c);
		}
		
		for(Entry<Integer,HashSet<Integer>> i:T.bag.entrySet())
		{
			HashSet<Integer> c = new HashSet<Integer>(i.getValue());
			bag.put(i.getKey(),c);
		}
		
	}
	public String toString(boolean Traverse)//true works only if it's a connected tree
	{
		String ans;
		if(Traverse)
			traverseTree();
		ans = "------JTDecTree------\n"+
			"width: "+ (maxWidth -1)+"\n"+
				"numberOfVertices: "+numberOfVertices+"\n"+"root: "+root+"\n"
				+"height: "+height+"\n";
		ans = ans
				+"parents: "+parent.toString()+"\n";
		ans = ans
				+"children: "+children.toString()+"\n";
		ans = ans
				+"mcBags: "+bag.toString()+"\n";
		ans = ans
				+"rootBags: "+rootBag.toString()+"\n";
		ans = ans
				+"levels: "+level.toString()+"\n"
				+"---End of JTDecTree---";
		return ans;
	}
	public String toString() //works only if it's a connected and well-defined tree
	{
		return toString(true);
	}
	void traverseRecursively(int u)//sets rootbags and height. called in traverseTree
	{
		maxNode = maxNode >= u? maxNode : u;
		maxWidth = maxWidth >= bag.get(u).size() ? maxWidth : bag.get(u).size();
		if(parent.get(u)==u)//added in package 6
			level.put(u,0);
		else
			level.put(u,level.get(parent.get(u))+1);
		height = level.get(u)>height? level.get(u):height;
		//update rootbags
		HashSet<Integer> cbag = bag.get(u);
		for(int i:cbag)
		{
			if(rootBag.get(i)==null)
				rootBag.put(i,u);
		}
		//recurse on all children
		HashSet<Integer> ch = children.get(u);
		for(int i:ch)
			if(i!=u)
				traverseRecursively(i);
	}
	public void traverseTree() //finds root, rootbags and height, the tree must be connected for this to work
	{
		height=0;
		maxNode = 0;
		maxWidth = 0;
		//find the root
		while(parent.get(root)!=root)
			root=parent.get(root);
		rootBag = new HashMap<Integer,Integer>();
		//recurse down
		traverseRecursively(root);
	}
	public void addVertex(int n) //adds a vertex to the tree, n is the identifier of the vertex to be added
	{
		maxNode = n >= maxNode ? n : maxNode;
		if(numberOfVertices==0)
		{
			root=n;
			height=0;
		}
		numberOfVertices++;
		parent.put(n,n);
		HashSet<Integer> ch = new HashSet<Integer>();
		ch.add(n);
		children.put(n,ch);
		HashSet<Integer> emptyBag = new HashSet<Integer>();
		bag.put(n, emptyBag);
		level.put(n,0);
	}
	public void changeParent(int vertex, int newParent) //changes the parent of a vertex. May cause the tree to become a forest in which case several other operations are not well-defined. 
	{
		//remove it from its old parent
		int oldParent = parent.get(vertex);
		HashSet<Integer> oldParentChildren = children.get(oldParent);
		oldParentChildren.remove(vertex);
		children.put(oldParent,oldParentChildren);
		
		//update it
		parent.put(vertex, newParent);
		level.put(vertex, level.get(newParent)+1);
		if(newParent == vertex)
			level.put(vertex,0);
		
		//add it to its newParent
		HashSet<Integer> newParentChildren = children.get(newParent);
		newParentChildren.add(vertex);
		children.put(newParent, newParentChildren);
	}
	public void addToBag(int bagNo, int node) //adds node to bag # bagNo
	{
		HashSet<Integer> B=bag.get(bagNo);
		B.add(node);
		bag.put(bagNo, B);
		maxWidth = maxWidth >= bag.get(bagNo).size() ? maxWidth : bag.get(bagNo).size();
	}
	public boolean hasVertex(int x) //whether there is a vertex with identifier x
	{
		return (parent.get(x)!=null);
	}
	public int size() //number of vertices
	{
		return numberOfVertices;
	}
	public int getHeight() //height of the tree.
	{
		return height;
	}
	public HashSet<Integer> getBag(int x) //contents of the bag x.
	{
		return new HashSet<Integer>(bag.get(x));
	}
	public int getRoot() //root of the tree.
	{
		return root;
	}
	public int getParent(int x) //returns the parent of vertex x.
	{
		return parent.get(x);
	}
	public HashSet<Integer> getChildren(int x) //returns a set of children of x.
	{
		return new HashSet<Integer>(children.get(x));
	}
	public int getRootBag(int x) //x is a node in the main graph G. Returns root bag of x.
	{
		return rootBag.get(x);
	}
	public int getDegree(int x) //Returns degree of the vertex x.
	{
		if(parent.get(x)==x) //x is a root
			return children.get(x).size() - 1; //we should remove itself from the list of its children
		else //x is not a root
			return children.get(x).size() + 1; //we should add its parent
	}
	public int getLevel(int x) //added in revision 7
	{
		return level.get(x);
	}
	public List<JTDecTree> breakTree(int placeOfCut) //cuts the tree from the vertex placeOfCut into several trees and returns a list of them
	{
		List<JTDecTree> answer = new ArrayList<JTDecTree>();
		HashSet<Integer> ch = children.get(placeOfCut);
		for(int i:ch)
			if(i!=placeOfCut)
				answer.add(subTreeRootedAt(i));
		JTDecTree upperComponent = new JTDecTree(this);
		upperComponent.removeSubTreeAt(placeOfCut);
		if(upperComponent.size()!=0)
		upperComponent.traverseTree();
		if(upperComponent.size()!=0)
			answer.add(upperComponent);
		
		return answer;
	}
	public JTDecTree subTreeRootedAt(int vertex) //returns the subtree rooted at vertex
	{
		JTDecTree answer = new JTDecTree();
		
		//create a list of vertices in this subtree in BFS order
		List<Integer> vertices = new ArrayList<Integer>();
		vertices.add(vertex);
		for(int i=0;i<vertices.size();++i)
		{
			int currentVertex=vertices.get(i);
			HashSet<Integer> currentChildren = children.get(currentVertex);
			for(int child:currentChildren)
				if(child!=currentVertex)
					vertices.add(child);
		}
		
		//add vertices to answer
		for(int i=0;i<vertices.size();++i)
		{
			int currentVertex=vertices.get(i);
			answer.addVertex(currentVertex);
			//set the parent
			answer.changeParent(currentVertex, (currentVertex==vertex)?vertex:parent.get(currentVertex));
			//set the bag
			HashSet<Integer> currentBag = bag.get(currentVertex);
			for(int node:currentBag)
				answer.addToBag(currentVertex, node);
		}
		
		answer.traverseTree();
		return answer;
	}
	public void removeSubTreeAt(int vertex) //removes the subtree rooted at vertex
	{
		//remove children
		HashSet<Integer> ch = new HashSet<Integer> (children.get(vertex));
		for(int child:ch)
			if(child!=vertex)
				removeSubTreeAt(child);
		//remove this one
		numberOfVertices--;
		HashSet<Integer> parentsChildren = (children.get(parent.get(vertex)));
		parentsChildren.remove(vertex);
		children.put(parent.get(vertex), parentsChildren);
		parent.remove(vertex);
		children.remove(vertex);
	}
	public ArrayList<Integer> allVerticesInBFSOrder() //returns a list of all vertices in BFS order starting from the root
	{
		ArrayList<Integer> vertices = new ArrayList<Integer>();
		vertices.add(this.getRoot());
		for(int i=0;i<vertices.size();++i)
		{
			int currentVertex=vertices.get(i);
			HashSet<Integer> children=this.getChildren(currentVertex);
			for(int j:children)
				if(j!=currentVertex)
					vertices.add(j);
		}
		return vertices;
	}
	public int getMaxNodeNumber() //returns the maximum number used as an identifier in this tree
	{
		return maxNode;
	}
	public int getTreeWidth()
	{
		return maxWidth - 1;
	}
	
}
	