import matplotlib.pyplot as plt
import numpy as np
from math import *

orders = { # only for the case that computation for all benchmarks is complete
"xml-apis": 0, "xml-apis-1.3.04": 1, "dacapo-luindex": 2, "dacapo-digest": 3, 
"dacapo-xalan": 4, "dacapo-tomcat": 5, "dacapo-lusearch": 6, 
"dacapo-lusearch-fix": 7, "daytrader": 8, "commons-daemon": 9, 
"commons-logging-1.0.4": 10, "tomcat-juli": 11, "constantine": 12, 
"bootstrap": 13, "dacapo-h2": 14, "avalon-framework-4.2.0": 15, 
"jnr-posix": 16, "commons-logging": 17, "jline-0.9.95-SNAPSHOT": 18, 
"asm-commons-3.1": 19, "commons-codec": 20, "commons-io-1.3.1": 21, 
"asm-3.1": 22, "junit-3.8.1": 23, "lucene-demos-2.4": 24, 
"jaxen-1.1.1": 25, "jaffl": 26, "serializer-2.7.0": 27, 
"crimson-1.1.3": 28, "commons-httpclient": 29, "serializer": 30, 
"janino-2.5.15": 31, "xmlgraphics-commons-1.3.1": 32, "lucene-core-2.4": 33, 
"xalan-2.6.0": 34, "derbyclient": 35, "pmd-4.2.5": 36, 
"avrora-cvs-20091224": 37, "eclipse": 38, "xerces_2_5_0": 39 
}

def tsplot(ax, path, label=None, **kw):
	try:
		with open(path, 'r') as f:
			lines = f.readlines()
			runtimes = np.zeros(len(lines), dtype=np.float)
			for i, line in enumerate(lines):
				if len(lines) == len(orders.keys()):
					runtimes[orders[line.split()[0]]] = float(line.split()[1])
				else:
					runtimes[i] = float(line.split()[1])
	except FileNotFoundError:
		print(f"File {path} not found.")
		return
	ax.scatter(np.arange(len(lines)), runtimes)
	ax.plot(np.arange(len(lines)), runtimes, label=label)
	ax.set_xticks(np.arange(len(lines)))
	ax.set_xticklabels( [i+1 for i in range(len(lines))], rotation=45 )
	ax.legend()

fig = plt.figure(figsize=(24.0, 16.0))

ax1 = plt.subplot(2, 2, 1)
ax2 = plt.subplot(2, 2, 2)
ax3 = plt.subplot(2, 2, 3)
ax4 = plt.subplot(2, 2, 4)

tsplot(ax1, "mc_hitprob/ours/results.txt", "ours")
tsplot(ax1, "mc_hitprob/gauss/results.txt", "gauss")
tsplot(ax1, "mc_hitprob/prism/results.txt", "prism")
tsplot(ax1, "mc_hitprob/storm/results.txt", "storm")

tsplot(ax2, "mc_discsum/ours/results.txt", "ours")
tsplot(ax2, "mc_discsum/gauss/results.txt", "gauss")

tsplot(ax3, "mdp_hitprob/ours/results.txt", "ours")
tsplot(ax3, "mdp_hitprob/lpsolve/results.txt", "lpsolve")
tsplot(ax3, "mdp_hitprob/prism/results.txt", "prism")
tsplot(ax3, "mdp_hitprob/storm/results.txt", "storm")
tsplot(ax3, "mdp_hitprob/strategy-iteration/results.txt", "SI")
tsplot(ax3, "mdp_hitprob/value-iteration/results.txt", "VI")

tsplot(ax4, "mdp_discsum/ours/results.txt", "ours")
tsplot(ax4, "mdp_discsum/lpsolve/results.txt", "lpsolve")
tsplot(ax4, "mdp_discsum/strategy-iteration/results.txt", "SI")
tsplot(ax4, "mdp_discsum/value-iteration/results.txt", "VI")

ax1.set_yscale("log")
ax2.set_yscale("log")
ax3.set_yscale("log")
ax4.set_yscale("log")

ax1.set_xlabel('Experimental Results for Computing Hitting Probabilities in MCs')
ax2.set_xlabel('Experimental Results for Computing Expected Discounted Sums in MCs')
ax3.set_xlabel('Experimental Results for Computing Hitting Probabilities in MDPs')
ax4.set_xlabel('Experimental Results for Computing Expected Discounted Sums in MDPs')

ax1.set_ylabel('Runtimes in log10 scale')
ax2.set_ylabel('Runtimes in log10 scale')
ax3.set_ylabel('Runtimes in log10 scale')
ax4.set_ylabel('Runtimes in log10 scale')

fig.savefig("results.png")
# plt.show()