static constexpr size_t edges_per_node = 16;

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

const char* node_name[] = {
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
    uint8_t next[edges_per_node];
};

static struct node_s three_cylinder[] = {
    [0] = {
        .type = is_source,
        .as.source = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01,
            },
        },
        .next = {1}
    },
    [1] = {
        .type = is_filter,
        .as.filter = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {2}
    },
    [2] = {
        .type = is_throttle,
        .as.throttle = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {3}
    },
    [3] = {
        .type = is_iplenum,
        .as.iplenum = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {4, 5, 6}
    },
    [4] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {7}
    },
    [5] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {8}
    },
    [6] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {9}
    },
    [7] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {10}
    },
    [8] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {11}
    },
    [9] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {12}
    },
    [10] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {13}
    },
    [11] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {13}
    },
    [12] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {13}
    },
    [13] = {
        .type = is_eplenum,
        .as.eplenum = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {14}
    },
    [14] = {
        .type = is_exhaust,
        .as.eplenum = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
            },
        },
        .next = {15}
    },
    [15] = {
        .type = is_sink,
        .as.sink = {
            .chamber = {
                .gas = ambient_gas_air,
                .volume_m3 = 1.0,
                .nozzle_max_flow_area_m2 = 0.01
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
