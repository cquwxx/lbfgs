/*
 * File:        backtracking_wolfe.c
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 01-Sep-2012.
 */

#include "include/backtracking_wolfe.h"

#include "include/linesearch.h"
#include "../exmath/include/exmath.h"

static void
backtracking_wolfe_parameter(
    linesearch_parameter *parameter
);

int
backtracking_wolfe(
    nlp_float *work,
    const nlp_float *x,
    const nlp_float *g,
    const nlp_float *d,
    int n,
    evaluate_object *eval_obj,
    nlp_component *component
)
{
    int iter, i;
    nlp_float width, beta, fx, gd, gd_next, *x_next, *g_next;
    linesearch_parameter parameter;

    x_next = work;
    g_next = x_next + n;

    backtracking_wolfe_parameter(&parameter);
    width = parameter.width;
    beta = parameter.beta;
    if (NLP_VALUE_NAN == eval_obj->func(x, n, component))
        return LINESEARCH_VALUE_NAN;
    fx = component->fx;
    gd = 0.;
    for (i = 0; i < n; ++i)
        gd += g[i] * d[i];
    for (iter = 1; iter <= parameter.maxit; ++iter)
    {
        for (i = 0; i < n; ++i)
            x_next[i] = x[i] + beta * d[i];
        if (NLP_VALUE_NAN == eval_obj->func(x_next, n, component))
            return LINESEARCH_VALUE_NAN;
        if (component->fx <= fx + parameter.xi * beta * gd)
        {
            if (NLP_VALUE_NAN == eval_obj->grad(g_next, x_next, n, component))
                return LINESEARCH_VALUE_NAN;
            gd_next = 0.;
            for (i = 0; i < n; ++i)
                gd_next += g_next[i] * d[i];
            if (parameter.sigma * gd <= gd_next)
            {
                component->alpha = beta;
                return LINESEARCH_SATISFIED;
            }
            else
            {
                width = parameter.inc;
            }
        }
        else
        {
            width = parameter.dec;
        }
        beta *= width;
        if (beta < parameter.minw || beta > parameter.maxw)
            return LINESEARCH_STEPWIDTH_FAILED;
    }
    return LINESEARCH_FAILED;
}

static void
backtracking_wolfe_parameter(
    linesearch_parameter *parameter
)
{
    parameter->maxit = 500;
    parameter->minw = 0.;
    parameter->maxw = 50.;
    parameter->width = .5;
    parameter->beta = .8;
    parameter->xi = .001;
    parameter->sigma = .5;
    parameter->dec = .5;
    parameter->inc = 2.1;
}
