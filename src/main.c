#include <SDL2/SDL.h>
#include "const.h"
#include "gamma.h"
#include "gas_t.h"
#include "chamber_t.h"
#include "piston_t.h"
#include "node_t.h"
#include "engine_t.h"
#include "visualize.h"

int
main(void)
{
    visualize_gamma();
    visualize_chamber_t();
    printf("node size (bytes): %lu\n", sizeof(struct node_t));
}

/* [1] B. Mcbride, M. Zehe, and S. Gordon, “NASA Glenn Coefficients for Calculating Thermodynamic Properties of Individual Species”, 2002.
 *     Available: https://ntrs.nasa.gov/api/citations/20020085330/downloads/20020085330.pdf
 *
 */
