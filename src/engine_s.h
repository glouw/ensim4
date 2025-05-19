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

static void
flow_engine(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* x = &self->node[i];
        for(size_t next, j = 0; (next = x->next[j]); j++)
        {
            struct node_s* y = &self->node[next];
            struct nozzle_flow_s nozzle_flow = flow(&x->as.chamber, &y->as.chamber);
            // sum these three from nozzle_flow_s if a parent has more than 1 child:
            //   mach
            //   velocity_m_per_s
            //   mass_flow_rate_kg_per_s
        }
    }
}

static void
flow_engine_by(struct engine_s* self, size_t count)
{
    for(size_t i = 0; i < count; i++)
    {
        flow_engine(self);
    }
}
