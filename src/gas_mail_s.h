struct gas_mail_s
{
    struct gas_s gas;
    struct chamber_s* x;
    struct chamber_s* y;
    bool is_from_reservoir;
};

static void
mail_gas_mail(struct gas_mail_s* self)
{
    if(self->is_from_reservoir == false)
    {
        remove_gas(self->x, &self->gas);
        clamp_momentum(&self->x->gas);
    }
    mix_in_gas(self->y, &self->gas);
    clamp_momentum(&self->y->gas);
    self->x->flow_cycles++;
}
