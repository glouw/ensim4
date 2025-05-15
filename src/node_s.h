constexpr size_t max_edges_per_node = 16;

enum type_e
{
    is_nothing,
    is_chamber,
    is_piston,
};

struct node_s
{
    enum type_e type;
    union
    {
        struct chamber_s chamber;
        struct piston_s piston;
    }
    as;
    uint8_t connects_to[max_edges_per_node];
};

struct node_s debug_pipeline[] = {
    [0] = {
        .type = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
        .connects_to = {1, 2, 3, 4}
    },
    [1] = {
        .type = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
    },
    [2] = {
        .type = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
    },
    [3] = {
        .type = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
    },
    [4] = {
        .type = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
    },
    [5] = {
        .type = is_nothing
    }
};

static void
normalize_node(struct node_s* self)
{
    normalize_chamber(&self->as.chamber);
}

static void
normalize_nodes(struct node_s* self)
{
    for(struct node_s* node = self; node->type != is_nothing; node++)
        normalize_node(node);
}
