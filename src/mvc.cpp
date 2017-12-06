/*
CSE6140 Project 
This is a experiment of Minimum Vertices Cover(MVC) algorithm.
Algorithm supposrted:
1. Brand and Bound
2. Construction Heuristics
3. Local Search
*/

#include "util.h"
#include "branchAndBound.h"
#include "constructionHeuristics.h"
#include "localSearch.h"

int main(int argc, char *argv[]) {

    /*
     * Getting Parameter
     * */
	if (argc != 9) {
		cout << "Exec prarmeter error" << endl;
		return 1;
	}

	if (strcmp(argv[1], "-inst") != 0 || strcmp(argv[3], "-alg") != 0 
            || strcmp(argv[5], "-time") != 0 || strcmp(argv[7], "-seed") != 0) {
		cout << "Exec prarmeter error" << endl;
		return 1;
	}

    //init function parameter
	string graph_file_name = argv[2];
	string algorithm = argv[4];
    int cutoffTime = atoi(argv[6]);
    int seed = atoi(argv[8]);

    //init ouput files.
    string output_file_name = "./Solutions/" + graph_file_name + "_" + algorithm + "_" + argv[6];
    if(algorithm != BnB)
        output_file_name += "_" + string(argv[8]);
    string solution_file = output_file_name + ".sol";
    string trace_file = output_file_name + ".trace";

	ofstream output_solution, output_trace;
	output_solution.open(solution_file);
	output_trace.open(trace_file);

    /*
     * Get graph file input
     * */
	Graph Gra = inputGraph("./Data/" + graph_file_name + ".graph");

#ifdef DEBUG
    outputGraph(Gra, "after Input");
#endif

    /*
     * Start calculation
     * */
    VCTYPE vc;

#ifdef DEBUG
    cout << "start mvc" << endl;
#endif

    clock_t start, end;
    float totalTime = 0.0;

    size_t times = TEST_TIME;
    while(times--) {
        Graph G(Gra);
        clearStack(vc);

        start = clock();
        //Run your MVC function on graph G and collect as output the total weight of the MST
        if(algorithm == BnB) {
            branchAndBound(G, vc, cutoffTime);
        }
        else if (algorithm == APPROX) {
            constructionHeuristics(G, vc, output_trace, cutoffTime, seed, true); 
        }
        else if (algorithm == LS1) {
            localSearch1(G, vc, output_trace, cutoffTime, seed); 
        }
        else if (algorithm == LS2) {
            localSearch2(G, vc, output_trace, cutoffTime, seed); 
        }
        else {
            cout << "Algorithm Error" << endl;
        }
        end = clock();
        totalTime = (end - start) / (float) CLOCKS_PER_SEC;
    }
    cout << totalTime << "\t";

    /*
     * output result
     * */
    output_solution << vc.size() << endl;
    cout << vc.size() << endl;
    while(vc.size() != 0) {
        size_t vec = vc.top();
        vc.pop();
        output_solution << vec+1 << "\t";
    }

	output_solution.close();
	output_trace.close();
#ifdef DEBUG
	cout << "\n";
#endif
	
	return 0;
}
