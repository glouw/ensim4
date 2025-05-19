/* ------- + block_deck_surface_m
 *         | head_clearance_height_m
 * ------- +
 * |     | | head_compression_height_m
 * |  o  | + pin_x_m, pin_y_m
 * |     | |
 * |-----| |
 *   | |   |
 *   | |   | connecting_rod_length_m
 *   | |   |
 *   | |   |
 *   | |   |
 *   |o|   + bearing_x_m, bearing_y_m
 *    |    |
 *    |    | crank_throw_length_m
 *    |    |
 *    o    + origin
 */

struct piston_s
{
    struct chamber_s chamber;
    double pin_x_m;
    double pin_y_m;
    double bearing_x_m;
    double bearing_y_m;
    double crankshaft_offset_theta_r;
    double crank_throw_length_m4;
    double connecting_rod_length_m3;
    double connecting_rod_mass_kg;
    double head_mass_density_kg_per_m3;
    double head_compression_height_m;
    double head_clearance_height_m;
};
