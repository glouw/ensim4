struct engine_s
{
    struct node_s* node;
    size_t size;
};

#define set_engine(nodes) (struct engine_s) { nodes, len(nodes) }

static void
normalize_engine(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        normalize_node(&self->node[i]);
    }
}
