constexpr size_t max_edges_per_node = 16;

enum tag_e
{
    is_nothing,
    is_chamber,
    is_piston,
};

struct node_s
{
    alignas(cache_line_bytes)
    struct
    {
        enum tag_e tag;
        union
        {
            struct chamber_s chamber;
            struct piston_s piston;
        }
        as;
        uint8_t connects_to[max_edges_per_node];
    };
};

struct node_s debug_pipeline[] = {
    [0] = {
        .tag = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
        .connects_to = {1, 2, 3, 4}
    },
    [1] = {
        .tag = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
    },
    [2] = {
        .tag = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
    },
    [3] = {
        .tag = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
    },
    [4] = {
        .tag = is_chamber,
        .as.chamber = {
            .gas = ambient_gas_air,
            .volume_m3 = 1.0,
            .nozzle_max_flow_area_m2 = 0.01
        },
    },
    [5] = {
        .tag = is_nothing
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
    for(struct node_s* node = self; node->tag != is_nothing; node++)
        normalize_node(node);
}
