enum tag_t
{
    is_chamber, is_piston,
};

struct node_t;

struct node_t
{
#if 1
    alignas(cache_line_bytes)
#endif
    struct
    {
        enum tag_t tag;
        union
        {
            struct chamber_t chamber;
            struct piston_t piston;
        }
        as;
        size_t nodes;
        struct node_t* next[max_edges_per_node];
    };
};
