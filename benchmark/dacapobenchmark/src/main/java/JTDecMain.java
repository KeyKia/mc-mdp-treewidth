import java.io.IOException;
import java.util.Arrays;
import java.util.Calendar;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import soot.PackManager;
import soot.Scene;
import soot.SceneTransformer;
import soot.SootMethod;
import soot.Transform;
import soot.options.Options;


public class JTDecMain extends SceneTransformer{

	static boolean writeLog;
	static int lambda;
	
	public static void main(String[] args) 
	{
		try{
		writeLog = Boolean.parseBoolean(args[0]);
		lambda = Integer.parseInt(args[1]);
		String[] argsnovo = new String[args.length-2];
		
		for(int i=2;i<args.length;++i)
			argsnovo[i-2]=args[i];
		sootSetup(argsnovo);
		} 
		catch (ArrayIndexOutOfBoundsException x)
		{
			System.err.println("Missing parameters for JTDec Analysis.");
		}
		catch (Exception x)
		{
			System.err.println("An error occured. Details:");
			x.printStackTrace();
		}
	}


	//Set up soot options and call
	public static void sootSetup(String[] args){


		PackManager.v().getPack("wjtp").add(new Transform("wjtp.main", new JTDecMain()));
		List<String>  sootArgs = new LinkedList<String>(Arrays.asList(args));
		/*soot.options.Options.v().set_whole_program(true);
		soot.options.Options.v().set_omit_excepting_unit_edges(true);
		soot.options.Options.v().set_throw_analysis(Options.throw_analysis_unit);
		Scene.v().loadNecessaryClasses();*/
		

			
		sootArgs.add("-keep-line-number");	//Keep java line numbers
		sootArgs.add("-w");
		sootArgs.add("-W");
		
	/*	sootArgs.add("--trim-cfgs");


		sootArgs.add("-p");
		sootArgs.add("wjop");
		sootArgs.add("enabled:false");
*/
		
		sootArgs.add("-p");
		sootArgs.add("wjtp");
		sootArgs.add("enabled:true");

		/*
		sootArgs.add("-p");
		sootArgs.add("cg");
		sootArgs.add("enabled:true,all-reachable:false,verbose:false,safe-forname:false,safe-newinstance:false");

		sootArgs.add("-p");
		sootArgs.add("cg.spark");
		sootArgs.add("enabled:true,on-fly-cg:true");

		
		sootArgs.add("-f");
		sootArgs.add("n");
		
		sootArgs.add("-i");
		sootArgs.add("java.");

		sootArgs.add("-allow-phantom-refs");
*/
		
		//System.out.println(sootArgs);
		//Call soot
		String[] argsArray = sootArgs.toArray(new String[0]);
		//System.out.println("Soot is called with arguments:"+argsArray);
		soot.Main.main(argsArray);
	}

	//Method called by soot to build the Pushdown Graph
	public void internalTransform(String phaseName, Map options)
	{
		Scene.v().loadBasicClasses();

		try {
			run();
		} catch (IOException mcE) {
			// TODO Auto-generated catch block
			mcE.printStackTrace();
		} catch (InterruptedException mcE) {
			// TODO Auto-generated catch block
			mcE.printStackTrace();
		}
		
		
	}



	
	
	public void run() throws IOException, InterruptedException{

		System.out.println("Running JTDec...");
		long time = Calendar.getInstance().get(Calendar.MILLISECOND);
		List<SootMethod> entryMethods = Scene.v().getEntryPoints();
		List<SootMethod> applicationMains = soot.EntryPoints.v().mainsOfApplicationClasses();
		
		List<SootMethod> applicationMethods = soot.EntryPoints.v().methodsOfApplicationClasses();

		entryMethods.retainAll(applicationMains);

		////////////JTDec Analysis
		for(SootMethod method:applicationMethods)
		{
			JTDecTree result = JTDec.process(method, lambda, writeLog);
			System.out.println("Method:"+method.getName());
			System.out.println("Resulting Tree decomposition:");
			System.out.println(result);
		}
		
		long time2 = Calendar.getInstance().get(Calendar.MILLISECOND);
		System.out.println("JTDec Execution Finished.");
		System.out.println("Total runtime of JTDec: "+(time2-time)+" milliseconds.");
	}
	
	

}
