/*
 * JTDec: A tool for tree decompositions in SOOT
 * Please read the related paper and its appendix for more information on how to use JTDec.
 *  http://pub.ist.ac.at/~akafshda/JTDec
    Copyright (C) 2016  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Please send suggestions and bug reports to goharshady@ist.ac.at

 */

/*
 * JTDec is a tool for obtaining and manipulating Tree Decompositions of Control Flow graphs of methods in Jimple
 * Public methods include:
-> process
-> getUnitsOf
-> createTreeDec
-> normalizeTreeDec
-> createBalancedTree
 */

import java.util.Iterator;
import java.util.HashMap;
import java.util.List;
import java.util.Set;
import java.util.HashSet;
import java.util.ArrayList;
import soot.SootMethod;
import soot.Unit;
import soot.toolkits.graph.BriefUnitGraph;

public class JTDec {
	static String prefix="[JTDec] "; //a prefix to all messages created by this class
	/*
	 * Gets a SootMethod method, creates a tree decomposition of it and balances it with parameter lambda
	 * The last parameter in each case defines whether or not the function should write its log to STDERR
	 */
	public static JTDecTree process(SootMethod method, int lambda,boolean writeLog)
	{
	    long startTime = System.currentTimeMillis();
		String cprefix=prefix+"[Main process] ";
		if(writeLog)
		{
			System.err.println(cprefix+"Process started");
			System.err.println(cprefix+"Creating a tree decomposition from Jimple code");
		}
		JTDecTree T = createTreeDec(method,writeLog);
		if(writeLog)
			System.err.println(cprefix+"Normalizing the tree decomposition");
		JTDecTree NT = normalizeTreeDec(T);
		if(writeLog)
		{
			System.err.println(cprefix+"Obtained the following normalized tree decomposition:\n"+NT);
			System.err.println(cprefix+"Balancing the tree decomposition");
		}
		JTDecTree TP = createBalancedTree(NT,lambda,writeLog);
		if(writeLog)
		{
			System.err.println(cprefix+"Obtained the following balanced tree decomposition with lambda ="+lambda+":\n"+TP);
			System.err.println(cprefix+"Process finished");
			long stopTime = System.currentTimeMillis(	);
			long elapsedTime = stopTime - startTime;
			System.err.println(cprefix+"The process ran for "+elapsedTime+ " milliseconds.");
		}
	    return TP;
	}
	public static JTDecTree process(SootMethod method, int lambda)
	{
		return process(method,lambda,false);
	}
	/*
	 * Another version of process, with lambda set to 2 (this what one would normally use)
	 */
	public static JTDecTree process(SootMethod method)
	{
		return process(method,2);
	}
	public static JTDecTree process(SootMethod method, boolean writeLog)
	{
		return process(method, 2, writeLog);
	}
	/*
	 * Returns a map that maps each integer used by JTDec on this method to its respective Unit.
	 * The Units are numbered starting from 0 and in the normal iterator order of the getActiveBody() function, but one may prefer to have them like this
	 */
	public static HashMap<Integer,Unit> getUnitsOf(SootMethod method)
	{
		//get the CFG
		BriefUnitGraph cfg = new BriefUnitGraph(method.getActiveBody());		
		//hash maps for converting units to line numbers and vice-versa
		HashMap<Unit,Integer> lineNumber = new HashMap<Unit,Integer>();
		HashMap<Integer,Unit> unitOf = new HashMap<Integer,Unit>();
		
		//print the method
		{
			Iterator<Unit> i=cfg.iterator();
			int cnt=0;
			while(i.hasNext())
			{
				cnt++;
				Unit u=(Unit) i.next();
				lineNumber.put(u,cnt);
				unitOf.put(cnt,u);
			}
		}
		return unitOf;
	}
	/*
	 * Gets a method and returns a tree decomposition of its control flow graph
	 * You can use getUnitsOf to get the units corresponding to numbers in the output of this function
	 */
	public static JTDecTree createTreeDec(SootMethod method)
	{
		return createTreeDec(method,false);
	}
	public static JTDecTree createTreeDec(SootMethod method, boolean writeLog)
	{
		String cprefix=prefix+"[createTreeDec] ";
		int numberOfLines;
		//get the CFG
		if(writeLog)
			System.err.println(cprefix+"Obtaining CFG of the method");
		BriefUnitGraph cfg = new BriefUnitGraph(method.getActiveBody());
		if(writeLog)
			System.err.println(cprefix+"CFG obtained");
		
		//hash maps for converting units to line numbers and vice-versa
		HashMap<Unit,Integer> lineNumber = new HashMap<Unit,Integer>();
		HashMap<Integer,Unit> unitOf = new HashMap<Integer,Unit>();
		
		//print the method
		if(writeLog)
			System.err.println(cprefix+"The lines were numbered as follows:");
		{
			Iterator<Unit> i=cfg.iterator();
			int cnt=0;
			while(i.hasNext())
			{
				cnt++;
				Unit u=(Unit) i.next();
				if(writeLog)
					System.err.println(cprefix+cnt+":\t"+u);
				lineNumber.put(u,cnt);
				unitOf.put(cnt,u);
			}
			numberOfLines=cnt;
		}
		//The sets J (=edges) and S=(symmetric expansion of J)
		Set<JTDecPair<Integer,Integer> > J = new HashSet<JTDecPair<Integer,Integer> >();
		Set<JTDecPair<Integer,Integer> > S = new HashSet<JTDecPair<Integer,Integer> >();
		
		//find the successors of each node
		if(writeLog)
			System.err.println(cprefix+"Finding successors of each node");
		Iterator<Unit> iter=cfg.iterator();
		HashMap<Integer,List<Integer> > successorsOf = new HashMap<Integer, List<Integer> >(); //successors of each node
		HashMap<Integer, HashSet<Integer> > neighborsOf = new HashMap<Integer, HashSet<Integer> >(); //neighbors of each node
		while(iter.hasNext())
		{
			Unit u=(Unit) iter.next();
			List<Unit> succ=cfg.getSuccsOf(u);
			List<Integer> integerSucc = new ArrayList<Integer>();
			int here=lineNumber.get(u);
			for(int j=0;j<succ.size();++j)
			{
				int next=lineNumber.get(succ.get(j));
				integerSucc.add(next);
				JTDecPair current=new JTDecPair<Integer,Integer>(here,next);
				J.add(current);
				S.add(current);
				S.add(current.Inverse());
			}
			successorsOf.put(here, integerSucc);
		}
		
		//create neighborsOf (successorsOf without direction)
		if(writeLog)
			System.err.println(cprefix+"Creating a list of neighbors of each node");
		for(int i=1;i<=numberOfLines;++i)
		{
			HashSet<Integer> emptySet = new HashSet<Integer>();
			neighborsOf.put(i, emptySet);
		}
		for(int i=1;i<=numberOfLines;++i)
		{
			List<Integer> proc=successorsOf.get(i);
			if(proc==null)
				continue;
			for(int j:proc)
			{
				HashSet<Integer> a = neighborsOf.get(i);
				HashSet<Integer> b = neighborsOf.get(j);
				a.add(j);
				b.add(i);
				neighborsOf.put(i, a);
				neighborsOf.put(j, b);
			}
		}
		
		//print them
		if(writeLog){
		System.err.println(cprefix+"successors are: "+successorsOf);
		System.err.println(cprefix+"neighbors are:"+neighborsOf);
		System.err.println(cprefix+"J is:"+J);
		System.err.println(cprefix+"S is:"+S);
		}
		System.out.println(cprefix+"number of lines:"+numberOfLines);
		System.out.println(cprefix+"successors are:"+successorsOf);
		//get the maximal chains
		
		Set<JTDecPair<Integer,Integer> > JChains = getChains(J,numberOfLines);
		Set<JTDecPair<Integer,Integer> > SChains = getChains(S,numberOfLines);

		//create reverse_maps reverse_map[j]=i <=> there is a chain from i to j
		HashMap<Integer,Integer> reverseMapJ = reverseMap(JChains,numberOfLines);
		HashMap<Integer,Integer> reverseMapS = reverseMap(SChains,numberOfLines);
		
		//create the listing (algorithm D)
		if(writeLog)
			System.err.println(cprefix+"Creating the listing");
		List<Integer> listing = new ArrayList<Integer>();
		int mark[] = new int[numberOfLines+3];
		for(int i=0;i<=numberOfLines+1;++i)
			mark[i]=0;
		{//create a block for this part
			int i =1;
			for(int j=numberOfLines;j>=1;--j)
			{
				if(mark[j]==0)
				{
					mark[j]=i;
					i++;
					listing.add(j);
				}
				if(reverseMapS.get(j)!=-1)
				{
					int k=reverseMapS.get(j);
					if(mark[k]==0)
					{
						mark[k]=i;
						i++;
						listing.add(k);
					}
				}
				if(reverseMapJ.get(j)!=-1)
				{
					int k=reverseMapJ.get(j);
					if(mark[k]==0)
					{
						mark[k]=i;
						i++;
						listing.add(k);
					}
				}
			}//end of for
		}
		if(writeLog)
		{
			System.err.println(cprefix+"Created the following listing:");
			System.err.println(cprefix+listing);
		}
		//convert the listing to a tree-decomposition
		//create a DSU data structure
		JTDecDSU DSU = new JTDecDSU(numberOfLines);
		//calculate separators
		//add all the backward edges
		for(int i:listing)
		{
			HashSet<Integer> lst=neighborsOf.get(i);
			if(lst==null)
				continue;
			for(int j:lst)
			{
				if(mark[i]<mark[j])
					DSU.addBackCon(i,j);
				else if(mark[j]<mark[i])
					DSU.addBackCon(j,i);
			}
		}//int i
		//calculate Separators
		if(writeLog)
			System.err.println(cprefix+"Finding separators");
		HashMap<Integer, HashSet<Integer> > separator = new HashMap<Integer, HashSet<Integer> >(); //separators of vertices
		for(int i=1;i<=numberOfLines;++i)
		{
			HashSet<Integer> emptySet = new HashSet<Integer>();
			separator.put(i, emptySet);
		}
		for(int ii=listing.size()-1;ii>=0;--ii)
		{
			int i=listing.get(ii);
			HashSet<Integer> lst=neighborsOf.get(i);
			for(int j: lst)
				if(mark[j]>mark[i]) //merge them
					DSU.merge(i,j);
			DSU.removeBackCon(i); //delete this vertex from DSU Backward connections
			HashSet<Integer> backCons = DSU.getBackCons(i);
			HashSet<Integer> cp = new HashSet<Integer>();
			for(int k:backCons)
				cp.add(k);
			separator.put(i, cp);
		}
		//make it connected
		for(int ii=listing.size()-1;ii>0;--ii)
		{
			int i = listing.get(ii);
			HashSet<Integer> sep = separator.get(i);
			if(sep.size()==0)
				sep.add(listing.get(0));
			separator.put(i,sep);
		}
		if(writeLog){
		System.err.println(cprefix+"Separators are:");
		System.err.println(cprefix+separator);
		}
		//initialize variables for the tree decomposition
		JTDecTree result = new JTDecTree();
		HashSet<JTDecPair<Integer,Integer> > treeDecEdges = new HashSet<JTDecPair<Integer,Integer> >();
		HashMap<Integer,HashSet<Integer> > treeDecNodes = new HashMap<Integer,HashSet<Integer> > ();
		for(int i=1;i<=numberOfLines;++i)
		{
			HashSet<Integer> emptySet = new HashSet<Integer>();
			treeDecNodes.put(i,emptySet);
			result.addVertex(i);
		}
		
		//create the tree decomposition according to Thorup's algorithm
		//initial case (v_1)
		HashSet<Integer> oneSet = new HashSet<Integer>();
		oneSet.add(listing.get(0));
		treeDecNodes.put(1, oneSet);
		
		for(int iii:oneSet)
			result.addToBag(1, iii);
		
		//the rest
		for(int i=2;i<=numberOfLines;++i)
		{
			int vi=listing.get(i-1);
			int h=getMax(separator.get(vi),mark);
			if(h!=-1)
			{
				int vh=listing.get(h-1);
				JTDecPair<Integer,Integer> newEdge = new JTDecPair(h,i);
				treeDecEdges.add(newEdge);
				result.changeParent(i,h);
			}
			HashSet<Integer> Wi = separator.get(vi);
			Wi.add(vi);
			treeDecNodes.put(i,Wi);
			for(int iii:Wi)
				result.addToBag(i, iii);
		}
		result.traverseTree();
		if(writeLog){
		System.err.println(cprefix+"Nodes of the tree decomposition are: "+treeDecNodes);
		System.err.println(cprefix+"Edges of the tree decomposition are:"+treeDecEdges);
		System.err.println(cprefix+"Created this JTDecTree:");
		System.err.println(result);
		System.err.println(cprefix+"Process finished");
		}
		return result;
	}

	/*
	 * For internal use only. Finds the right-most element in a separator
	 */
	static int getMax(HashSet<Integer> separator, int[] mark)
	{
		int ans=-1;
		for(int i:separator)
			ans = mark[i]>ans?mark[i]:ans;
		return ans;
	}
	/*
	 * For internal use only. creates a reverse_map out of a set, used in createListing
	 */
	static HashMap<Integer,Integer> reverseMap(Set<JTDecPair<Integer,Integer> > CH,int n)
	{
		HashMap<Integer,Integer> ret=new HashMap<Integer,Integer>();
		for(int i=1;i<=n;++i)
			ret.put(i,-1);
		Iterator iter=CH.iterator();
		while(iter.hasNext())
		{
			JTDecPair<Integer,Integer> current=(JTDecPair<Integer,Integer>)(iter.next());
			ret.put(current.Second(), current.First());
		}
		return ret;
	}
	/*
	 * For internal use only. Finds maximal chains of I, n is the maximal line number.
	 */
	static HashSet<JTDecPair<Integer,Integer> > getChains(Set<JTDecPair<Integer,Integer> > I,int n) 
	{
		//return value
		HashSet<JTDecPair<Integer,Integer> > M = new HashSet<JTDecPair<Integer,Integer> >();
		
		//definitions
		int[] ii = new int[n+3];
		int[] jj = new int[n+3];
		int s=0;
		ii[0]=0;
		jj[0]=n+1;
		
		HashMap<Integer,Integer> maxj = new HashMap<Integer,Integer>(); //max {j | (i,j) \in I}
		for(int i1=1;i1<=n;++i1)
			maxj.put(i1,-1);
		Iterator setIter = I.iterator();
		while(setIter.hasNext())
		{
			JTDecPair<Integer,Integer> current=(JTDecPair<Integer,Integer>) setIter.next();
			maxj.put(current.First(),current.Second()>=maxj.get(current.First())?current.Second():maxj.get(current.First()));
		}
		//the main algorithm (E)
		for(int ip=1;ip<=n;++ip)
			if(maxj.get(ip)>ip)
			{
				int i=ip;
				int j=maxj.get(i);
				while(jj[s]<=i)
				{
					JTDecPair<Integer,Integer> current_p = new JTDecPair<Integer,Integer> (ii[s],jj[s]);
					M.add(current_p);
					s--;
				}
				while(j>=jj[s] && jj[s]>i)
				{
					i=ii[s];
					s=s-1;
				}
				s++;
				ii[s]=i;
				jj[s]=j;
			}
		return M;
	}
	
	/*
	 * Normalizes a given Tree decomposition. A tree decomposition is normal if each vertex has at most one node that appears in its bag but not in its parent bag
	 */
	public static JTDecTree normalizeTreeDec(JTDecTree inputT) //normalize a tree decomposition
	{
		JTDecTree answer = new JTDecTree(inputT);
		recursivelyNormalizeTreeDec(answer,answer.getRoot());
		answer.traverseTree();
		return answer;
	}
	
	/*
	 * For internal use only. The recursive function called by normalizeTreeDec.
	 */
	static void recursivelyNormalizeTreeDec(JTDecTree T, int currentVertex)
	{
		//call recursively first
		List<Integer> children = new ArrayList<Integer> (T.getChildren(currentVertex));
		for(int child:children)
			if(child!=currentVertex)
				recursivelyNormalizeTreeDec(T,child);
		//process this vertex
		int currentParent = T.getParent(currentVertex);
		HashSet<Integer> parentBag = T.getBag(currentParent);
		HashSet<Integer> thisBag = T.getBag(currentVertex);
		ArrayList<Integer> newInt = new ArrayList<Integer>();
		HashSet<Integer> common = new HashSet<Integer>();
		for(int i:thisBag)
			if(parentBag.contains(i))
				common.add(i);
			else
				newInt.add(i);
		if(newInt.size()<=1)
			return;
		
		//add vertices in between
		int lastVertex = currentParent;
		for(int k=1;k<newInt.size();++k)
		{
			int no=T.getMaxNodeNumber()+1;
			T.addVertex(no);
			//set its parent and child
			T.changeParent(no, lastVertex);
			T.changeParent(currentVertex, no);
			//set its bag
			//add the commons
			for(int i:common)
				T.addToBag(no, i);
			for(int i=0;i<k;++i)
				T.addToBag(no, newInt.get(i));
			
			lastVertex=no;
		}
		
	}
	
	/*
	 * Creates a balanced tree decomposition with parameter lambda
	 * Input is an unbalanced tree decomposition inputT
	 */
	public static JTDecTree createBalancedTree(JTDecTree inputT, int lambda)
	{
		return createBalancedTree(inputT, lambda,false);
	}
	public static JTDecTree createBalancedTree(JTDecTree inputT, int lambda, boolean writeLog) 
	{
		String cprefix = prefix+"[createBalancedTree] ";
		//take a copy
		JTDecTree T = new JTDecTree(inputT);
		T.traverseTree();
		
		//balance it
		JTDecTree rankTree = new JTDecTree();
		createRankTree(T, T, lambda, 1, rankTree, 1);
		if(writeLog)
			System.err.println(cprefix+"The following rank tree was created:\n"+rankTree);
		
		//convert rankTree to a tree decomposition
		JTDecTree finalT=new JTDecTree(rankTree);
		ArrayList<Integer> allVertices = rankTree.allVerticesInBFSOrder();
		
		for(int thisVertex:allVertices)
		{
			HashSet<Integer> neighbors = rankTree.getChildren(thisVertex);
			neighbors.add(rankTree.getParent(thisVertex));
			
			for(int neighbor:neighbors)
			{
				HashSet<Integer> neighborBag = rankTree.getBag(neighbor);
				for(int node:neighborBag)
					finalT.addToBag(thisVertex, node);
			}
		}
		finalT.traverseTree(); //added in package 6
		
		//return it
		return finalT;
	}
	/*
	 * Creates a rank tree recursively. For internal use in createBalancedTree only.
	 */
	static void createRankTree(JTDecTree originalT, JTDecTree componentC, int lambda, int depth, JTDecTree rankTree, int placeOfAdditionInRankTree)//creates a rank-tree recursively, 
	{
		componentC.traverseTree();
		int placeOfCut=-1;
		HashSet<Integer> bagOfCut = new HashSet<Integer>();
		if(componentC.size() <= 2)
		{
			//base case
			int r=componentC.getRoot();
			HashSet<Integer> allVertices = componentC.getChildren(r);
			for(int vertex:allVertices)
			{
				HashSet<Integer> vertexBag = componentC.getBag(vertex);
				for(int i:vertexBag)
					bagOfCut.add(i);
			}
			//add bagOfCut to rankTree
			int no = rankTree.size()+1;
			rankTree.addVertex(no);
			rankTree.changeParent(no, placeOfAdditionInRankTree);
			for(int cutb:bagOfCut)
				rankTree.addToBag(no, cutb);
			return;
		}
		//sort vertices in BFS order
		List<Integer> vertices = componentC.allVerticesInBFSOrder();
		
		if(depth==0) //cut based on Nh
		{
			//process form bottom up
			HashMap<Integer,Integer> Nh = new HashMap<Integer,Integer>();
			for(int i=vertices.size()-1;i>=0;--i)
			{
				int thisVertex=vertices.get(i);
				Nh.put(thisVertex, originalT.getDegree(thisVertex) - componentC.getDegree(thisVertex));
				HashSet<Integer> thisChildren = componentC.getChildren(thisVertex);
				for(int child:thisChildren)
					if(child!=thisVertex)
						Nh.put(thisVertex, Nh.get(thisVertex)+Nh.get(child));
			}
			
			//find the placeOfCut
			for(int i=vertices.size()-1;i>=0;--i)
			{
				int thisVertex=vertices.get(i);
				if(Nh.get(thisVertex)*2>=Nh.get(componentC.getRoot()))
				{
					placeOfCut=thisVertex;
					break;
				}
					
			}
			//do the cut 
			//place of cut is found
		}
		else //cut based on graph size
		{
			//calculate number of descendants from bottom up
			HashMap<Integer,Integer> descendants = new HashMap<Integer,Integer>();
			for(int i=vertices.size()-1;i>=0;--i)
			{
				int currentVertex = vertices.get(i);
				descendants.put(currentVertex, 1);
				HashSet<Integer> children = componentC.getChildren(currentVertex);
				for(int child:children)
					if(child!=currentVertex)
						descendants.put(currentVertex,descendants.get(currentVertex)+descendants.get(child));
				if(descendants.get(currentVertex)*2>=componentC.size())
				{
					placeOfCut=currentVertex;
					break;
				}
			}
			//place of cut is found
		}
		//create bagOfCut
		bagOfCut = new HashSet<Integer>(componentC.getBag(placeOfCut));
		//add bagofCut to rankTree
		int no=rankTree.size()+1;
		rankTree.addVertex(no);
		rankTree.changeParent(no,placeOfAdditionInRankTree);
		for(int cutV:bagOfCut)
			rankTree.addToBag(no, cutV);
		//recurse
		List<JTDecTree> recurseComponents = componentC.breakTree(placeOfCut);
		for(JTDecTree newComponent:recurseComponents)
			createRankTree(originalT, newComponent, lambda, (depth+1)%lambda, rankTree, no);
	}
	
}