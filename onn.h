/* Defines an ONN of harmonic oscillators */

#ifndef ONN_H
#define ONN_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_errno.h>
#include <stdio.h>
#include <math.h>

typedef struct node * Node;
typedef struct onn * ONN;

/* ==== Constants ==== */
double DEFAULT_NUM_EVALS_PARAM = 1;
double DEFAULT_T_STEP_PARAM = 0.2;

/* ==== Method stubs ==== */
Node newNode(gsl_vector *, ONN);
ONN newONN(gsl_matrix, gsl_matrix, gsl_matrix, int, int, double, double);
void * safeMalloc(size_t);

#endif
