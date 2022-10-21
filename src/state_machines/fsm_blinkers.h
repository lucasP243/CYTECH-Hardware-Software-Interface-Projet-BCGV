/**
 * \brief This file implements a finite state machine to manage the blinker
 * systems (left blinker, right blinker and both for warnings).
 *
 * \author Lucas Pinard
 */
#ifndef FSM_BLINKERS_H
#define FSM_BLINKERS_H

#include "lib/data_dictionary.h"

/**
 * \brief Compute the left blinker FSM with the current application data and
 * update them.
 */
__attribute__((unused)) void compute_left_blinker();

/**
 * \brief Compute the right blinker FSM with the current application data and
 * update them.
 */
__attribute__((unused)) void compute_right_blinker();

#endif // FSM_BLINKERS_H
