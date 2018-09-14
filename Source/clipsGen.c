#include <stdio.h>
#include <stdlib.h>
#include "clipsGen.h"

FILE* out;
                                                   
int makeClips(int argc, char* argv[]) {
   if(argc != 3){
      printf("Error: usage ./graphColor <input_file_name> <number_colors>\n");
      return -1;
   }
   
   int numColors = atoi(argv[2]);
   if(numColors < 2 || numColors > 4){
       printf("Error: num_colors must be between 2 and 4\n");
       return -1;
   }

   FILE* fp = fopen(argv[1], "r");
   if(fp == NULL){
      printf("Error: Can't open file %s\n", argv[1]);
      return -1;
   }

   out = fopen("color.clips", "w");
   int numNodes;
   int numAdjs;
   if (fscanf(fp,"%d %d\n", &numNodes, &numAdjs) != 2){
      printf("Error: first line of input file should be");
      printf("\"<num Vertices> <num Adjacencies>\"");
      return -1;
   }
   
   char** adjs = (char**)malloc(sizeof(char*)*numAdjs);
   
   int i;
   for(i = 0; i < numAdjs; i++){
      adjs[i] = (char*)malloc(8);
      fscanf(fp, "%s\n", adjs[i]);
   }

   writeDefTemps(numNodes);
   writeRules(numNodes, numAdjs, adjs);
   writeInitFacts(numColors);

   fclose(fp);
   fclose(out);
   for(i = 0; i < numAdjs; i++){
      free(adjs[i]);
   }
   free(adjs);
   return numNodes;

}


void writeDefTemps(int numVerts){
   
   fputs("\n(deftemplate adj-to (slot node1) (slot node2))\n", out);
   fputs("\n(deftemplate coloring\n", out);
   int i;
   for(i = 1; i <= numVerts; i++){
      fprintf(out, "   (slot col%d)\n", i);
   }
   fputs(")\n", out);

}

void writeRules(int numVerts, int numAdjs, char** adjs){
 
  
   fputs("\n(defrule rulesym\n", out);
   fputs("   (adj-to (node1 ?X) (node2 ?Y))\n", out);
   fputs(" =>\n", out);
   fputs("   (assert (adj-to (node1 ?Y) (node2 ?X)))\n)\n", out);
   
   fputs("\n(defrule rulecol\n", out);
   int i, n1, n2;
   for( i = 0; i < numAdjs; i++){
      sscanf(adjs[i], "%d--%d", &n1, &n2);
      fprintf(out, "   (adj-to (node1 ?R%d) (node2 ?R%d))\n", n1, n2);
   }
   fputs(" =>\n", out);
   fputs("   (assert\n", out);
   fputs("      (coloring\n", out);

   for(i = 1; i <= numVerts; i++){
      fprintf(out,  "         (col%d ?R%d)\n", i, i);
   }
   fputs("         )\n", out);
   fputs("      )\n", out);
   fputs(")\n", out); 
}


void writeInitFacts(int numColors){

   char* colors[] = {"red", "green", "blue", "yellow"};   
   fputs("\n(deffacts startup \n", out);
   
   int i, j;
   for(i = 0; i < numColors; i++){
      for(j = i+1; j < numColors; j++){
         fprintf(out,"   (adj-to (node1 %s) (node2 %s))\n", colors[i], colors[j]);
      }
   }
   fputs(")\n", out);
}


