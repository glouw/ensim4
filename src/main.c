#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "gamma.h"
#include "gas_t.h"
#include "chamber_t.h"
#include "piston_t.h"
#include "node_t.h"
#include "engine_t.h"
#include "visualize.h"

void
help()
{
    size_t estimated_l1_cache_bytes = 32768;
    size_t node_bytes = sizeof(struct node_t);
    size_t estimated_nodes_per_l1 = estimated_l1_cache_bytes / node_bytes;
    printf("cache line size (bytes): %lu\n", cache_line_bytes);
    printf("estimated l1 cache size (bytes): %lu\n", estimated_l1_cache_bytes);
    printf("node size (bytes): %lu\n", node_bytes);
    printf("estimated nodes per L1: %lu\n", estimated_nodes_per_l1);
}

int
main()
{
    help();
    visualize_gamma();
    visualize_chamber_t();
}

/* [1] B. Mcbride, M. Zehe, and S. Gordon, “NASA Glenn Coefficients for Calculating Thermodynamic Properties of Individual Species”, 2002.
 *     Available: https://ntrs.nasa.gov/api/citations/20020085330/downloads/20020085330.pdf
 *
 */
