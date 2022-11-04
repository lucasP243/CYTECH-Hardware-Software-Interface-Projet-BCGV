/**
 * \brief This file implements a finite state machine to manage the light
 * systems (sidelights, headlights and redlights).
 */
#ifndef FSM_LIGHTS_H
#define FSM_LIGHTS_H

#include "lib/data_dictionary.h"

/**
 * \brief Compute the headlights FSM with the current application data and
 * update them.
 */
void compute_headlights();

/**
 * \brief Compute the sidelights FSM with the current application data and
 * update them.
 */
void compute_sidelights();

/**
 * \brief Compute the redlights FSM with the current application data and
 * update them.
 */
void compute_redlights();

#endif // FSM_LIGHTS_H
