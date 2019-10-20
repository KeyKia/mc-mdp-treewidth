/*
 * This class is mainly created to be used in other classes of the JTDec package. It is very similar to the C++ pair.
 * Please send bug reports and suggestions to goharshady@ist.ac.at
 */
public class JTDecPair<L,R> {
	private final L left;
	private final R right;
	
	public JTDecPair(L left, R right)
	{
		this.left=left;
		this.right=right;
	}
	
	public JTDecPair<R,L> Inverse()
	{
		JTDecPair<R,L> ret=new JTDecPair(right,left);
		return ret;
	}
	
	public L First()
	{
		return left;
	}
	public R Second()
	{
		return right;
	}
	
	public int hashCode()
	{
		return left.hashCode()^right.hashCode();
	}
	
	public boolean equals(Object obj)
	{
		if(!(obj instanceof JTDecPair))
			return false;
		JTDecPair given=(JTDecPair)obj;
		return this.left.equals(given.First()) && this.right.equals(given.Second());
	}
	
	public String toString()
	{
		return "("+left.toString()+", "+right.toString()+")";
	}

}