/**
 * \brief This file implements a finite state machine to manage the light
 * systems (sidelights, headlights and redlights).
 *
 * \author Lucas Pinard
 */
#ifndef FSM_LIGHTS_H
#define FSM_LIGHTS_H

#include "lib/data_dictionary.h"

/**
 * \brief The different states of the lights FSM.
 */
typedef enum fsm_lights_state_t {
  FSM_LIGHTS_OFF = 0,
  FSM_LIGHTS_ON = 1,
  FSM_LIGHTS_ACKNOWLEDGED = 2,
  FSM_LIGHTS_ERROR = -1
} fsm_lights_state_t;

/**
 * \brief The different events triggering state changes in the FSM.
 */
typedef enum fsm_lights_event_t {
  FSM_LIGHTS_EVENT_ANY = 0,
  FSM_LIGHTS_EVENT_COMMAND_ON = 1,
  FSM_LIGHTS_EVENT_COMMAND_OFF = 2,
  FSM_LIGHTS_EVENT_ACK_RECEIVED = 3,
  FSM_LIGHTS_EVENT_ACK_MISSED = -1,
} fsm_lights_event_t;

/**
 * \brief Tick a FSM with an event, changing its state if a corresponding
 * transition exists.
 *
 * \param[in,out]   fsm_lights_t        Pointer to the FSM to tick.
 * \param[in]       fsm_lights_event_t  The event to tick the FSM with.
 * \param[in,out]   fsm_timer_t         Pointer to the FSM timer.
 */
__attribute__((unused)) void fsm_lights_tick(fsm_lights_t *, fsm_lights_event_t,
                                             fsm_timer_t *);

/**
 * \brief Computes the event corresponding to the current values of the
 * application data for the headlights.
 *
 * @return The event.
 */
__attribute__((unused)) fsm_lights_event_t compute_headlights_event();

/**
 * \brief Computes the event corresponding to the current values of the
 * application data for the sidelights.
 *
 * @return The event.
 */
__attribute__((unused)) fsm_lights_event_t compute_sidelights_event();

/**
 * \brief Computes the event corresponding to the current values of the
 * application data for the redlights.
 *
 * @return The event.
 */
__attribute__((unused)) fsm_lights_event_t compute_redlights_event();

#endif // FSM_LIGHTS_H
