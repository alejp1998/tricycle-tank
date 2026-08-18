/*
 * tests/test_fsm.c — hardware-free unit tests for the FSM engine (fsm.c).
 * Build & run with: make test
 */
#include <assert.h>
#include <stdio.h>

#include "fsm.h"

/* A tiny two-state machine to exercise the engine */
enum { S_A, S_B, S_C };

static int input_go(fsm_t *this) {
    (void)this;
    return 1;
}

static int input_none(fsm_t *this) {
    (void)this;
    return 0;
}

static int output_count = 0;

static void output_inc(fsm_t *this) {
    (void)this;
    output_count++;
}

static void output_noop(fsm_t *this) {
    (void)this;
}

int main(void) {
    fsm_trans_t tt[] = {
        {S_A, input_go, S_B, output_inc},   /* A -> B fires once */
        {S_B, input_go, S_C, output_noop},  /* B -> C fires once  */
        {S_C, input_none, S_A, NULL},       /* C -> A never fires */
        {-1, NULL, -1, NULL},
    };

    /* fsm_new starts in the given state */
    fsm_t *f = fsm_new(S_A, tt, NULL);
    assert(f != NULL);
    assert(f->current_state == S_A);

    /* First fire: A -> B, output runs */
    fsm_fire(f);
    assert(f->current_state == S_B);
    assert(output_count == 1);

    /* Second fire: B -> C */
    fsm_fire(f);
    assert(f->current_state == S_C);

    /* C's only transition needs input_none (returns 0) -> stays in C */
    fsm_fire(f);
    assert(f->current_state == S_C);

    /* Unknown state: engine must not crash and must stay put */
    f->current_state = 99;
    fsm_fire(f);
    assert(f->current_state == 99);

    /* fsm_init resets a reused object */
    fsm_init(f, S_A, tt, NULL);
    assert(f->current_state == S_A);
    fsm_fire(f);
    assert(f->current_state == S_B);

    /* fsm_destroy frees the object */
    fsm_destroy(f);

    printf("FSM tests passed (%d transitions executed)\n", output_count + 1);
    return 0;
}
