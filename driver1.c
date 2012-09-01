/*
 * File:        driver1.c
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 01-Sep-2012.
 *
 * Problem:
 *     minimize f(x) = (x1 - x2^2)^2 / 2 + (x2 - 2)^2 / 2
 *
 * Function Info:
 *     gf(x) =  [ x1 - x2^2                    ]
 *              [ -2 * x2(x1 - x2^2) + x2 - 2  ]
 *
 *     f(x*) = 0 (x* = [ 4, 2 ]^T)
 */

#include "lbfgs/include/lbfgs.h"

#include <stdlib.h>

#include "lbfgs/include/nlp_component.h"
#include "lbfgs/linesearch/include/linesearch.h"
#include "lbfgs/linesearch/include/backtracking_wolfe.h"
#include "lbfgs/exmath/include/exmath.h"

static nlp_float
func(
    const nlp_float *x,
    int n
);

static void
grad(
    nlp_float *g,
    const nlp_float *x,
    int n
);

int
main(int argc, char* argv[])
{
    int i, n;
    nlp_float *x;
    function_object func_obj;

#if 0
    linesearchparameter line_search_parameter;
    quasinewtonparameter quasi_newton_parameter;
    default_armijo_parameter(&line_search_parameter);
    quasi_newton_parameter.formula = 'b';
    quasi_newton_parameter.tolerance = 1.e-8;
    // quasi_newton_parameter.upper_iter = 5000;

    /* int
     * quasi_newton(
     *     nlp_float *x,
     *     nlp_float **b,
     *     int n,
     *     FunctionObject *function_object,
     *     line_search_t line_search,
     *     LineSearchParameter *line_search_parameter,
     *     QuasiNewtonParameter *quasi_newton_parameter
     * );
     */
#endif
    n = 2;

    x = (nlp_float *)malloc(n * sizeof(nlp_float));
    for (i = 0; i < n; ++i)
        x[i] = 0.;

    func_obj.func = func;
    func_obj.grad = grad;

    lbfgs(
        x,
        n,
        &func_obj,
        backtracking_wolfe,
        NULL
    );

    free_vec(x);
    return 0;
}

static nlp_float
func(
    const nlp_float *x,
    int n
)
{
    return (x[0] - x[1] * x[1]) * (x[0] - x[1] * x[1]) + (x[1] - 2.) * (x[1] - 2.) / 2.;
}


static void
grad(
    nlp_float *g,
    const nlp_float *x,
    int n
)
{
    g[0] = x[0] - x[1] * x[1];
    g[1] = -2. * x[1] * (x[0] - x[1] * x[1]) + x[1] - 2.;
}
