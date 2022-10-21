#include <stddef.h>

#include "src/fsm_lights.h"

#define FSM_LIGHTS_TRANSITION_ALIGNMENT 16
/**
 * \brief The list of all possible transitions from one state to another,
 * associated with the corresponding trigger event.
 */
struct __attribute__((aligned(FSM_LIGHTS_TRANSITION_ALIGNMENT))) {
  fsm_lights_state_t current_state;
  fsm_lights_state_t next_state;
  fsm_lights_event_t event;
} fsm_lights_transitions[] = {
    {
        // Technically this transition isn't needed, but it prevents iterating
        // through the whole list when the fsm is in error state
        .current_state = FSM_LIGHTS_ERROR,
        .event = FSM_LIGHTS_EVENT_ANY,
        .next_state = FSM_LIGHTS_ERROR,
    },
    {
        .current_state = FSM_LIGHTS_OFF,
        .event = FSM_LIGHTS_EVENT_COMMAND_ON,
        .next_state = FSM_LIGHTS_ON,
    },
    {
        .current_state = FSM_LIGHTS_ON,
        .event = FSM_LIGHTS_EVENT_COMMAND_OFF,
        .next_state = FSM_LIGHTS_OFF,
    },
    {
        .current_state = FSM_LIGHTS_ON,
        .event = FSM_LIGHTS_EVENT_ACK_RECEIVED,
        .next_state = FSM_LIGHTS_ACKNOWLEDGED,
    },
    {
        .current_state = FSM_LIGHTS_ON,
        .event = FSM_LIGHTS_EVENT_ACK_MISSED,
        .next_state = FSM_LIGHTS_ERROR,
    },
    {
        .current_state = FSM_LIGHTS_ACKNOWLEDGED,
        .event = FSM_LIGHTS_EVENT_COMMAND_OFF,
        .next_state = FSM_LIGHTS_OFF,
    },
};

#define FSM_LIGHTS_TRANSITIONS_COUNT                                           \
  (sizeof(fsm_lights_transitions) / sizeof(*fsm_lights_transitions))

__attribute__((unused)) void fsm_lights_tick(fsm_lights_t *fsm_p,
                                             fsm_lights_event_t event_p,
                                             fsm_timer_t *timer_p) {
#pragma unroll
  for (size_t i = 0; i < FSM_LIGHTS_TRANSITIONS_COUNT; i++) {

    fsm_lights_state_t *current_state = fsm_p;

    if (*current_state == fsm_lights_transitions[i].current_state) {
      fsm_lights_event_t event = fsm_lights_transitions[i].event;

      if ((event_p == event) || (FSM_LIGHTS_EVENT_ANY == event)) {
        *current_state = fsm_lights_transitions[i].next_state;
        *timer_p = 0;
        break;
      }
    }
  }

  *timer_p += 1;
}

#define FSM_LIGHTS_ACKNOWLEDGEMENT_DELAY 200

__attribute__((unused)) fsm_lights_event_t compute_headlights_event() {

  fsm_lights_state_t state = get_fsm_headlights();
  fsm_lights_event_t event = FSM_LIGHTS_EVENT_NONE;
  fsm_timer_t timer = get_fsm_headlights_timer();

  if (get_headlights_in()) {

    if (state == FSM_LIGHTS_ON) {
      if (get_headlights_acknowledgement()) {
        event = FSM_LIGHTS_EVENT_ACK_RECEIVED;
      } else if (timer > FSM_LIGHTS_ACKNOWLEDGEMENT_DELAY) {
        event = FSM_LIGHTS_EVENT_ACK_MISSED;
      }
    }

  } else {
    event = FSM_LIGHTS_EVENT_COMMAND_OFF;
  }

  return event;
}

__attribute__((unused)) fsm_lights_event_t compute_sidelights_event() {

  fsm_lights_state_t state = get_fsm_sidelights();
  fsm_lights_event_t event = FSM_LIGHTS_EVENT_NONE;
  fsm_timer_t timer = get_fsm_sidelights_timer();

  if (get_sidelights_in()) {

    if (state == FSM_LIGHTS_ON) {
      if (get_sidelights_acknowledgement()) {
        event = FSM_LIGHTS_EVENT_ACK_RECEIVED;
      } else if (timer > FSM_LIGHTS_ACKNOWLEDGEMENT_DELAY) {
        event = FSM_LIGHTS_EVENT_ACK_MISSED;
      }
    }

  } else {
    event = FSM_LIGHTS_EVENT_COMMAND_OFF;
  }

  return event;
}

__attribute__((unused)) fsm_lights_event_t compute_redlights_event() {

  fsm_lights_state_t state = get_fsm_redlights();
  fsm_lights_event_t event = FSM_LIGHTS_EVENT_NONE;
  fsm_timer_t timer = get_fsm_redlights_timer();

  if (get_redlights_in()) {

    if (state == FSM_LIGHTS_ON) {
      if (get_redlights_acknowledgement()) {
        event = FSM_LIGHTS_EVENT_ACK_RECEIVED;
      } else if (timer > FSM_LIGHTS_ACKNOWLEDGEMENT_DELAY) {
        event = FSM_LIGHTS_EVENT_ACK_MISSED;
      }
    }

  } else {
    event = FSM_LIGHTS_EVENT_COMMAND_OFF;
  }

  return event;
}
