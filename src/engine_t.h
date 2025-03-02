constexpr size_t nodes_per_engine = 16;

struct engine_t
{
    struct node_t nodes[nodes_per_engine];
};
