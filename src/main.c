#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include "util.h"
#include "gamma.h"
#include "gas_s.h"
#include "chamber_s.h"
#include "nozzle_flow_s.h"
#include "piston_s.h"
#include "node_s.h"
#include "visualize.h"

int
main()
{
    printf("%lu\n", sizeof(debug_pipeline));
    visualize_gamma();
    visualize_chamber_s();
}

/* [1] B. Mcbride, M. Zehe, and S. Gordon, “NASA Glenn Coefficients for Calculating Thermodynamic Properties of Individual Species”, 2002.
 *     Available: https://ntrs.nasa.gov/api/citations/20020085330/downloads/20020085330.pdf
 *
 */
