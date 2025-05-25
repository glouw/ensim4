struct gas_mail_s
{
    struct gas_s gas;
    struct chamber_s* x;
    struct chamber_s* y;
};

static void
mail_gas_mail(struct gas_mail_s* self)
{
    remove_gas(self->x, &self->gas);
    mix_in_gas(&self->y->gas, &self->gas);
    clamp_momentum(&self->x->gas);
    clamp_momentum(&self->y->gas);
    self->x->flow_cycles++;
}
