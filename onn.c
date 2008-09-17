/* C implementation of oscillator neural network.
 * Harmonic oscillator equations are used here.
 * This file describes an entire ONN object.
 */

#include "onn.h"

/* ==== Internal Structs ==== */

struct node {
  int layer_num;
  gsl_matrix * states_matrix;
  gsl_vector * input_sum_terms;
};

struct onn {
  int num_outputs;
  int num_inputs;
  int curr_step;
  double num_steps;
  double t_step;
  double curr_time;
  Node * nodes;
  gsl_matrix * connections;
};

/* ==== Initialization Methods ==== */

Node newNode(gsl_vector * initial_state, ONN net_ref){
  Node new = (Node) safeMalloc(sizeof(Node));
  gsl_matrix * states_matrix = gsl_matrix_calloc(net_ref->num_steps, initial_state->size);
  int i;
  for(i = 0; i < initial_state->size - 1; i++){
    gsl_matrix_set(states_matrix, 0, (size_t) i, gsl_vector_get(initial_state, (size_t) i));
  }
  new->states_matrix = states_matrix;
  new->layer_num = gsl_vector_get(initial_state, initial_state->size - 1);
  new->input_sum_terms = NULL;
  return new;
}

ONN newONN(gsl_matrix input_list, gsl_matrix node_data, gsl_matrix connections, 
           int num_inputs, int num_outputs, double num_evals_param, double t_step_param){

  int i;
  int row;
  int entry;
  gsl_matrix * conn_dest;
  Node * node_list;
  gsl_vector * a_vals;
  ONN new = (ONN) safeMalloc(sizeof(struct onn));
  gsl_vector * ones_vec;
  gsl_vector  * two_pi_vec;
  size_t num_a_vals = node_data.size1;
  double min_quotient;
  double max_period;

  new->num_inputs = num_inputs;
  new->num_outputs = num_outputs;
  new->curr_time = 0.0;
  new->curr_step = 0;

  /* copy and store connections */

  conn_dest = gsl_matrix_alloc(connections.size1,connections.size2);
  gsl_matrix_memcpy(conn_dest, &connections);
  new->connections = conn_dest;

  /* Calculate time step, number of steps */

  a_vals = gsl_vector_alloc(num_a_vals);
  for(row = 0; row < node_data.size1; row++){
    gsl_vector_set(a_vals, (size_t) row, gsl_matrix_get(&node_data, (size_t) row, 1)); 
  }

  for(entry = 0; entry < num_a_vals; entry++){
    gsl_vector_set(a_vals, (size_t) entry, sqrt(gsl_vector_get(a_vals, (size_t) entry)));
  }

  two_pi_vec = gsl_vector_alloc(num_a_vals);
  gsl_vector_set_all(two_pi_vec, M_PI_2);
  gsl_vector_div(a_vals,two_pi_vec);

  ones_vec = gsl_vector_alloc(num_a_vals);
  gsl_vector_set_all(ones_vec, 1);
  gsl_vector_div(ones_vec,a_vals);

  min_quotient = gsl_vector_min(ones_vec);
  gsl_vector_scale(ones_vec,2);
  max_period = gsl_vector_max(ones_vec);

  new->t_step = t_step_param * min_quotient;
  new->num_steps = round(max_period*num_evals_param);

  /* Create node list */

  node_list = (Node *) safeMalloc(sizeof(Node)*node_data.size1);

  for(i = 0; i < node_data.size1; i++){
    Node * next_node = node_list++;
    Node new_node;
    gsl_vector * vec = gsl_vector_alloc(node_data.size2);
    gsl_matrix_get_row(vec, &node_data, (size_t) i);
    new_node = newNode(vec,new);
    next_node = &new_node;
  }

  new->nodes = node_list;

  return new;
}

/* ==== Accessor methods ==== */

int get_current_step(ONN net){
  return net->curr_step;
}

double get_current_time(ONN net){
  return net->curr_time;
}

double get_step_size(ONN net){
  return net->t_step;
}

int get_num_steps(ONN net){
  return net->num_steps;
}

/* ==== Utility Methods ==== */

void * safeMalloc(size_t size){
  void * ret_val = malloc(size);
  if(ret_val != NULL)
    return ret_val;
  else
    exit(101);
}


