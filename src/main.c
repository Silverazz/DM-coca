#include <stdio.h>
#include <z3.h>
#include "Solving.h"
#include "Z3Tools.h"
#include <Graph.h>
#include <Parsing.h>
#include <string.h>
#include <assert.h>

int main (int argc, char **argv){

    Z3_context c = makeContext();
    //Z3_ast a = getNodeVariable(c,1,2,3,4);
    Z3_ast f;
    Z3_model model;
    int k; //solution length
    int isSat;
    int size = 1;
    char * fileName; //for the name of the file

    Graph graph[size];
    //graph[0] = getGraphFromFile("graphs/assignment-instance/triangle.dot");
	graph[0] = getGraphFromFile("graphs/assignment-instance/triangle.dot");
    //graph[1] = getGraphFromFile("graphs/generic-instances/positive-instances/Taille10_pos/instance1/G1.dot");

    for (int i =1; i< argc; i++){
        if(!strcmp(argv[i], "-h")){
            printf ("Options:\n"); 
            printf ("-h         Displays this help\n");
            printf ("-v         Activate verbose mode (displays parsed graphs\n");
            printf ("-F         Displays the formula computed\n");
            printf ("-s         Tests separately all formula by depth [if not present: uses the global formula]\n");
            printf ("-d         Only if -s is present. Explore the length in decreasing order. [if not present: in increasing order]\n");
            printf ("-a         Only if -s is present. Computes a result for every length instead of stopping at the first positive result (default behaviour)\n");
            printf ("-t         Displays the paths found on the terminal [if not present, only displays the existence of the path].\n");
            printf ("-f         Writes the result with colors in a .dot file. See next option for the name. These files will be produced in the folder 'sol'.\n");
            printf ("-o NAME    Writes the output in 'NAME-lLENGTH.dot' where LENGTH is the length of the solution. Writes several files in this format if both -s and -a are present. [if not present: 'result-lLENGTH.dot']\n");
    
        }else if (!strcmp (argv[i], "-v" ) && argc > 2){
            for(int j =0; j<size; j++){
                printGraph(graph[i]);
            }
        
      }else if (!strcmp (argv[i], "-F")){
          f = graphsToFullFormula(c,graph,size);
          printf("Path formula: %s\n",Z3_ast_to_string(c,f));

      }else if (!strcmp (argv[i], "-s")){
        f = graphsToFullFormula(c,graph,size);
        isSat = isFormulaSat(c,f);
        if(isSat == -1){
            printf("Satisfiable ? : non\n");
        }else if(isSat == 0){
            printf("Satisfiable ? : On ne peut pas savoir\n");
        }else{
             printf("Satisfiable ? : oui\n");
        }

      }else if (!strcmp (argv[i], "-d")){
          if(!strcmp (argv[i-1], "-s") || !strcmp (argv[i+1], "-s")){
            f = graphsToFullFormula(c,graph,size);
            model = getModelFromSatFormula(c,f);
            k = getSolutionLengthFromModel(c,model,graph);
          }

      }else if (!strcmp (argv[i], "-a")){

      }else if (!strcmp (argv[i], "-t")){
        f = graphsToFullFormula(c,graph,size);
        model = getModelFromSatFormula(c,f);
        k = getSolutionLengthFromModel(c,model,graph);
        printPathsFromModel(c,model,graph,size,k);

      }else if (!strcmp (argv[i], "-f")){
        if(!strcmp (argv[i+1], "-o")){
           fileName = argv[i+2];
        }
        f = graphsToFullFormula(c,graph,size);
        model = getModelFromSatFormula(c,f);
        k = getSolutionLengthFromModel(c,model,graph);
        if(fileName == NULL)
            createDotFromModel(c,model,graph,size,k,NULL);
        createDotFromModel(c,model,graph,size,k,fileName);
      }
    }

    Z3_del_context(c);
}
