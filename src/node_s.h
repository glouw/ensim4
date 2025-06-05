static constexpr size_t g_nodes_node_children = 16;

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

static const char* g_node_name_string[] = {
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
    uint8_t next[g_nodes_node_children];
};

static struct node_s g_node_8_cylinder[] = {
    [0] = {
        .type = is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001,
            },
        },
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
        .next = {4, 5, 6, 7, 8, 9, 10, 11}
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
        .next = {12}
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
        .next = {13}
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
        .next = {14}
    },
    [7] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {15}
    },
    [8] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {16}
    },
    [9] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {17}
    },
    [10] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {18}
    },
    [11] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {19}
    },
    [12] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (0.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {20}
    },
    [13] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (1.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {21}
    },
    [14] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (2.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {22}
    },
    [15] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (3.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {23}
    },
    [16] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (4.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {24}
    },
    [17] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (5.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {25}
    },
    [18] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (6.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {26}
    },
    [19] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (7.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient = 0.1,
        },
        .next = {27}
    },
    [20] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [21] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [22] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [23] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [24] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [25] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [26] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [27] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [28] = {
        .type = is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {29}
    },
    [29] = {
        .type = is_exhaust,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {30}
    },
    [30] = {
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

static void
select_next(struct node_s* nodes, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        struct node_s* node = &nodes[i];
        if(node->is_selected)
        {
            size_t edges = 0;
            size_t next;
            while((next = node->next[edges]))
            {
                nodes[next].is_selected = true;
                edges++;
            }
            break;
        }
    }
}

#undef TYPES
