/*
CSE6140 Project 
This is a experiment of Minimum Vertices Cover(MVC) algorithm.
Algorithm supposrted:
1. Brand and Bound
2. Construction Heuristics
3. Local Search
*/

#include "util.h"
#include "brandAndBound.h"
#include "constructionHeuristics.h"
#include "localSearch.h"

int main(int argc, char *argv[]) {

    /*
     * Getting Parameter
     * */
	if (argc != 8) {
		cout << "Exec prarmeter error" << endl;
		return 1;
	}

	if (argv[1] != "-inst" || argv[3] != "-alg" || argv[5] != "-time" || argv[7] != "-seed") {
		cout << "Exec prarmeter error" << endl;
		return 1;
	}

    //init function parameter
	string graph_file_name = argv[2];
	string algorithm = argv[4];
    int cutOffTime = atoi(argv[6]);
    int seed = atoi(argv[8]);

    //init ouput files.
    string output_file_name = "./Solutions/" + graph_file_name + "_" + algorithm + "_" + argv[6];
    if(algorithm != BnB)
        output_file_name+= "_" + argv[8];
    string solution_file += ".sol";
    string trace_file += ".trace";

	ofstream output_solution, output_trace;
	output_solution.open(solution_file);
	output_trace.open(trace_file);

#ifdef DEBUG
	ofstream coutFile;
	coutFile.open("out", ios::app);
#endif

    /*
     * Get graph file input
     * */
	Graph G = inputGraph("./Data/" + graph_file_name + ".graph");

#ifdef DEBUG
    outputGraph(coutfile, G);
#endif

    /*
     * Start calculation
     * */
    VCTYPE vc;

	//Run your MVC function on graph G and collect as output the total weight of the MST
    switch(algorithm) {
        case BnB:
            branchAndBound(G, vc); break;
        case APPROX:
            constructionHeuristics(G, vc, output_trace, cutoff, seed); break;
        case LS1:
            localSearch1(G, vc, output_trace, cutoff, seed); break;
        case LS2:
            localSearch2(G, vc, output_trace, cutoff, seed); break;
        case default:
            cout << "Algorithm Error" << endl;
    }

    /*
     * output result
     * */
    output_solution << vc.size() << endl;
    while(vc.size() != 0) {
        size_t vec = vc.top();
        vc.pop();
        output_solution << vec << "\t";
    }

	output_solution.close();
	output_trace.close();
	coutFile << "\n";
	coutFile.close();
	
	return 0;
}
