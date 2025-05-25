static constexpr size_t nodes_node_children = 16;

#define TYPES   \
    X(chamber)  \
    X(source)   \
    X(filter)   \
    X(throttle) \
    X(iplenum)  \
    X(irunner)  \
    X(piston)   \
    X(erunner)  \
    X(eplenum)  \
    X(exhaust)  \
    X(sink)

enum node_type_e
{
    #define X(type) is_##type,
    TYPES
    #undef X
};

static const char* node_name_string[] = {
    #define X(type) #type,
    TYPES
    #undef X
};

struct node_s
{
    enum node_type_e type;
    union
    {
        #define X(type) struct type##_s type;
        TYPES
        #undef X
    }
    as;
    bool is_selected;
    uint8_t next[nodes_node_children];
};

static struct node_s node_debug_stationary[] = {
    [0] = {
        .type = is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = 1e9,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001,
            },
        },
        .next = {1}
    },
    [1] = {
        .type = is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = 1e9,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {}
    },
};

static struct node_s node_debug_cylinder[] = {
    [0] = {
        .type = is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = 1e9,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001,
            },
        },
        .next = {1}
    },
    [1] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {2}
    },
    [2] = {
        .type = is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = 1e9,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {}
    },
};

static struct node_s node_three_cylinder[] = {
    [0] = {
        .type = is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001,
            },
        },
        .is_selected = true,
        .next = {1}
    },
    [1] = {
        .type = is_filter,
        .as.filter = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .is_selected = true,
        .next = {2}
    },
    [2] = {
        .type = is_throttle,
        .as.throttle = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .is_selected = true,
        .next = {3}
    },
    [3] = {
        .type = is_iplenum,
        .as.iplenum = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .is_selected = true,
        .next = {4, 5, 6}
    },
    [4] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {7}
    },
    [5] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {8}
    },
    [6] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {9}
    },
    [7] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (0.0 / 3.0) * std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {10}
    },
    [8] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (1.0 / 3.0) * std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {11}
    },
    [9] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (2.0 / 3.0) * std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {12}
    },
    [10] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {13}
    },
    [11] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {13}
    },
    [12] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {13}
    },
    [13] = {
        .type = is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {14}
    },
    [14] = {
        .type = is_exhaust,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {15}
    },
    [15] = {
        .type = is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {}
    },
};

static void
normalize_node(struct node_s* self)
{
    normalize_chamber(&self->as.chamber);
}

static size_t
count_node_edges(const struct node_s* self)
{
    size_t edges = 0;
    while(self->next[edges])
    {
        edges++;
    }
    return edges;
}

static size_t
count_many_node_edges(const struct node_s* nodes, size_t size)
{
    size_t edges = 0;
    for(size_t i = 0; i < size; i++)
    {
        edges += count_node_edges(&nodes[i]);
    }
    return edges;
}

static void
deselect_all_nodes(struct node_s* nodes, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        nodes[i].is_selected = false;
    }
}

static void
select_nodes(struct node_s* nodes, size_t size, enum node_type_e type)
{
    for(size_t i = 0; i < size; i++)
    {
        struct node_s* node = &nodes[i];
        if(node->type == type)
        {
            node->is_selected = true;
        }
    }
}

#undef TYPES
