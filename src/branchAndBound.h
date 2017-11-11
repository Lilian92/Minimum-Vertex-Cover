/*
 * Branch and Bound Algorithm for MVC 
 * 1. Using strage that addes a vertex to Minimum Vertices Cover Set if any of its neighbors has degree only be 1.
 * 2. Remove all vertices whose edges can be covered by existing MVC set.
 * 3. Using graph matching algorithm as lower bound to cut search space
 * */
#include "util.h"

void branchAndBound(Graph & g, VCTYPE & vc);
