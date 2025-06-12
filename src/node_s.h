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

static size_t
count_selected_nodes(struct node_s* nodes, size_t size)
{
    size_t selected = 0;
    for(size_t i = 0; i < size; i++)
    {
        if(nodes[i].is_selected)
        {
            selected++;
        }
    }
    return selected;
}

static void
select_next(struct node_s* nodes, size_t size)
{
    if(count_selected_nodes(nodes, size) == 1)
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
}

#undef TYPES
