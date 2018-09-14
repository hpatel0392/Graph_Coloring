# Graph Coloring with CLIPS and Graphviz

Contents:
-----------
1) readme.txt - this file
2) Makefile - Has 4 scripts to build the program and graphs
	      * make all - will build 2 excecutables named
		"graphGen" and "graphColor"
	      * make clean - removes the exe's and accessory files
	      * make jpg - used to build the .dot file into a jpg (requires graphviz)
 	      * make png - used to build the .dot file into a png (rquires graphviz)
3) main.c - the main driver, will create the CLIPS file, environment and also 
	    a .dot file if a coloring was found
4) clipsGen.* - Reads in graph data supplied in the proper format and creates a .clips
                file for the main driver to use.
5) graphGen.c - creates a random graph of N nodes and outputs into file, where N >= 2. 
		**Note: see report, but graphs with N > 20 are strongly discouraged**
6) Sample_Images - Contains the results of several sample graph tests that were run
7) Sample_Graphs - Contains the .txt files for the graphs that correspond to the images
                   in "Sample_Images"

8) Clips_Core - contains the clips source and static library required for this project
	***NOTE: I am not distributing this source with this project, please download it from 
	https://sourceforge.net/projects/clipsrules/files/CLIPS/6.30/
	

9) harshp-report_takehome1.pdf - the accompanying report for this project

** 
   As report suggests graphviz is required to render the .dot files, 
   1) if not installed: sudo apt-get install graphviz
   2) this package is installed on ECE lab machines already
   3) alternatively, go to www.webgraphviz.com and copy the contents of the .dot file
      into the text box and hit generate graph
