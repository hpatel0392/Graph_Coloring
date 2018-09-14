#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clipsGen.h"
#include "clips.h"

void writeDotFile(char buf[], char* fName);
void getColoring(int numNodes, char buf[], char** coloring);

int main (int argc, char *argv[]) {

   printf("\n\t Creating file \"color.clips\"...\n");
   int numNodes = makeClips(argc, argv);
   if(numNodes == -1){
      exit(-1);
   }

   char *filename = "color.clips";

   // Prepare the CLIPS environment and load in the .clips file
   void *theEnv;
   theEnv = CreateEnvironment (); 

   EnvLoad (theEnv, filename);
   EnvReset (theEnv);

   // Obtain  pointer to the template named color
   void * deftemplatePtr = EnvFindDeftemplate(theEnv, "coloring"); 
   
   // Fire one rule
   long long numRun = EnvRun (theEnv, 1);

   void * factPtr = EnvGetNextFactInTemplate(theEnv, deftemplatePtr, NULL); 
   int i = 0;

   const int MAX_RULES = 25;

   /* Fire the rules in the agenda until: 
   *  agenda is empty, i == MAX_RULES : Meaning no coloring found
   *  or the rule with template "coloring" is found
   */
   while(numRun != 0 && i < MAX_RULES && factPtr == NULL){
      numRun = EnvRun (theEnv, 1);
      factPtr = EnvGetNextFactInTemplate(theEnv, deftemplatePtr, NULL);
      i++; 
   }
  
   // Extract the Fact if found
   char buffer[1024];
   if(factPtr != NULL){
      EnvGetFactPPForm(theEnv, buffer, 1024, factPtr);
      printf("\t Coloring found! \n");
      printf("\t Creating file \"coloring.dot\"....\n\n");        
   } else {
      printf("\tNo Coloring found for this graph!\n\n");
      DestroyEnvironment (theEnv);
      return 0;
   }
  
   // Destroy the CLIPS Environment
   DestroyEnvironment (theEnv);

   // Write the resulting .dot file with the colored graph
   writeDotFile(buffer, argv[1]);
   return 0;
}

/**
* Write the .dot file
* @Param buf - contains the fact with the coloring found by CLIPS
* @Param fName - name of the graph Data file, its needed again here
*/
void writeDotFile(char buf[], char* fName){
   
   FILE * in = fopen(fName, "r");

   int i, j;
   int numNodes, numAdjs;
   fscanf(in,"%d %d\n", &numNodes, &numAdjs); 
   char** adjs = (char**)malloc(sizeof(char*)*numAdjs);
   char** coloring = (char**)malloc(sizeof(char*)*numNodes);
    
   getColoring(numNodes, buf, coloring);

   for(i = 0; i < numAdjs; i++){
      adjs[i] = (char*)malloc(8);
      fscanf(in, "%s\n", adjs[i]);
      while(adjs[i][strlen(adjs[i])-1] == '\n'){
            adjs[i][strlen(adjs[i])-1] = '\0';
      }
   }

   fclose(in);

   FILE* out = fopen("coloring.dot", "w");
  
   fputs("graph G{\n", out);
   for(i = 0; i < numAdjs; i++){
      fprintf(out, "\t%s;\n", adjs[i]);
   }
   for(i = 0; i < numNodes; i++){
      fprintf(out, "\t%d[color=%s,style=filled];\n", i+1, coloring[i]);
   }
   fputs("}\n", out);


   fclose(out);
   
   for(i = 0; i < numAdjs; i++){
      free(adjs[i]);
   }
   free(coloring);
   free(adjs);
}


/**
* extracts the color for each node and stores it in coloring
* @Param numNodes - number of nodes in the graph
* @Param buf - the fact with coloring info from CLIPS
* @Param coloring - holds the color for each node
* 	Expected to be malloc'd already 
*/
void getColoring(int numNodes,char buf[], char** coloring){
   
   char* token = strtok(buf, "(");
   token = strtok(NULL, "(");
   int i;
   char* temp;
   for(i = 0; i < numNodes; i++){
      coloring[i] = strchr(strtok(NULL, "("), ' ');
      temp = &coloring[i][strlen(coloring[i])-1]; 
      while( *temp == ')' || *temp == ' ' ){
         *temp = '\0';
         temp--;
      }
   } 
}
