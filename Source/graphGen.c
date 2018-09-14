#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define INVALID -1
#define ITERATIONS 10000

int main(int argc, char* argv[]){
  
   if(argc != 3){
      printf("Error: usage ./graphGen <numNodes> <output_file>\n");
      exit(-1);
   }

   FILE * out = fopen(argv[2], "w");
   if(out == NULL){
      printf("Error: can't open file %s\n", argv[2]);
      exit(-1);
   }
   
   int numNodes = atoi(argv[1]);
   int minEdges = (numNodes > 10)?2:1;
   if(numNodes <= 1){
      printf("Error: numNodes must be > 1\n");
      exit(-1);
   } else if (numNodes > 20){
      printf("Warning: It is strongly recommended that numNodes be <= 20\n");
      printf("         For better results run again with a smaller number\n");
   }

   int adj[numNodes][numNodes];

   srand(time(NULL));
   
   int i, j, numAdjs = 0;
   int temp, randNode = rand() % (numNodes*numNodes);
   int * curr1 = NULL;
   int * curr2 = NULL;


   // Clear the matrix
   for(i = 0; i < numNodes; i++){
      for(j = 0; j < numNodes; j++){
         if(i == j){
            adj[i][j] = INVALID;
         } else {
            adj[i][j] = FALSE;
         }
      }
   }


   // randomly add adjacencies
   for(i = 0; i < ITERATIONS; i++){
      
      temp = rand() % (numNodes*numNodes);
      while(temp == randNode){
         randNode = rand() %(numNodes*numNodes);
      }
  
      curr1 = &adj[randNode/numNodes][randNode%numNodes];
      curr2 = &adj[randNode%numNodes][randNode/numNodes];
      if(*curr1 == FALSE){
         *curr1 = TRUE;
         *curr2 = TRUE;
         numAdjs++;
      } else if(*curr1 == TRUE){
         *curr1 = FALSE;
         *curr2 = FALSE;
         numAdjs--;
      }
   }    
   

   // Make sure all nodes have at least "minEdges"
   int rowCount;
   for(i = 0; i < numNodes; i++){
      rowCount = 0;
      for(j = 0; j < numNodes; j++){
         if(adj[i][j] == TRUE){
            rowCount++;
         }
      }
      while(rowCount < minEdges){
          do{
             randNode = rand() % numNodes;
          }while(randNode == i);
          
          if(adj[i][randNode] == FALSE){
             adj[i][randNode] = TRUE;
             adj[randNode][i] = TRUE;
             numAdjs++;
             rowCount++;
          }
      }
   }

   // write to file
   fprintf(out,"%d %d\n", numNodes, numAdjs);
   for(i = 0; i < numNodes; i++){
      for(j = i; j < numNodes; j++){
         if(adj[i][j] == TRUE)
            fprintf(out, "%d--%d\n", i+1, j+1);
      }
   }
   fclose(out);

   return 0; 
}

