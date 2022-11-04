#include <stddef.h>

#include "fsm_lights.h"

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

/**
 * \brief Tick a FSM with an event, changing its state if a corresponding
 * transition exists.
 *
 * \param[in,out]   fsm_lights_t        Pointer to the FSM to tick.
 * \param[in]       fsm_lights_event_t  The event to tick the FSM with.
 * \param[in,out]   fsm_timer_t         Pointer to the FSM timer.
 */
static void fsm_lights_tick(fsm_lights_t *fsm_p, fsm_lights_event_t event_p,
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

#define FSM_LIGHTS_ACKNOWLEDGEMENT_DELAY 100

void compute_headlights() {

  // Compute event

  fsm_lights_t fsm = get_fsm_headlights();
  fsm_lights_event_t event;
  fsm_timer_t timer = get_fsm_headlights_timer();

  if (get_headlights_in()) {

    if (get_headlights_acknowledgement()) {
      event = FSM_LIGHTS_EVENT_ACK_RECEIVED;
    } else if (timer > FSM_LIGHTS_ACKNOWLEDGEMENT_DELAY) {
      event = FSM_LIGHTS_EVENT_ACK_MISSED;
    } else {
      event = FSM_LIGHTS_EVENT_COMMAND_ON;
    }

  } else {
    event = FSM_LIGHTS_EVENT_COMMAND_OFF;
  }

  // Tick FSM

  fsm_lights_tick(&fsm, event, &timer);

  // Update data

  set_fsm_headlights(fsm);
  set_fsm_headlights_timer(timer);
  set_headlights_acknowledgement(false);

  switch ((fsm_lights_state_t)fsm) {

  case FSM_LIGHTS_OFF:
  case FSM_LIGHTS_ERROR:
    set_headlights_out(false);
    set_indicator_headlights(false);
    break;

  case FSM_LIGHTS_ON:
    set_headlights_out(true);
    set_indicator_headlights(false);
    break;

  case FSM_LIGHTS_ACKNOWLEDGED:
    set_headlights_out(true);
    set_indicator_headlights(true);
    break;
  }
}

void compute_sidelights() {

  fsm_lights_t fsm = get_fsm_sidelights();
  fsm_lights_event_t event;
  fsm_timer_t timer = get_fsm_sidelights_timer();

  // Compute event

  if (get_sidelights_in()) {

    if (get_sidelights_acknowledgement()) {
      event = FSM_LIGHTS_EVENT_ACK_RECEIVED;
    } else if (timer > FSM_LIGHTS_ACKNOWLEDGEMENT_DELAY) {
      event = FSM_LIGHTS_EVENT_ACK_MISSED;
    } else {
      event = FSM_LIGHTS_EVENT_COMMAND_ON;
    }

  } else {
    event = FSM_LIGHTS_EVENT_COMMAND_OFF;
  }

  // Tick FSM

  fsm_lights_tick(&fsm, event, &timer);

  // Update data

  set_fsm_sidelights(fsm);
  set_fsm_sidelights_timer(fsm);
  set_sidelights_acknowledgement(false);

  switch ((fsm_lights_state_t)fsm) {

  case FSM_LIGHTS_OFF:
  case FSM_LIGHTS_ERROR:
    set_sidelights_out(false);
    set_indicator_sidelights(false);
    break;

  case FSM_LIGHTS_ON:
    set_sidelights_out(true);
    set_indicator_sidelights(false);
    break;

  case FSM_LIGHTS_ACKNOWLEDGED:
    set_sidelights_out(true);
    set_indicator_sidelights(true);
    break;
  }
}

void compute_redlights() {

  fsm_lights_t fsm = get_fsm_redlights();
  fsm_lights_event_t event;
  fsm_timer_t timer = get_fsm_redlights_timer();

  // Compute event

  if (get_redlights_in()) {

    if (get_redlights_acknowledgement()) {
      event = FSM_LIGHTS_EVENT_ACK_RECEIVED;
    } else if (timer > FSM_LIGHTS_ACKNOWLEDGEMENT_DELAY) {
      event = FSM_LIGHTS_EVENT_ACK_MISSED;
    } else {
      event = FSM_LIGHTS_EVENT_COMMAND_ON;
    }

  } else {
    event = FSM_LIGHTS_EVENT_COMMAND_OFF;
  }

  // Tick FSM

  fsm_lights_tick(&fsm, event, &timer);

  // Update data

  set_fsm_redlights(fsm);
  set_fsm_redlights_timer(timer);
  set_redlights_acknowledgement(false);

  switch ((fsm_lights_state_t)fsm) {

  case FSM_LIGHTS_OFF:
  case FSM_LIGHTS_ERROR:
    set_redlights_out(false);
    set_indicator_redlights(false);
    break;
  case FSM_LIGHTS_ON:
    set_redlights_out(true);
    set_indicator_redlights(false);
    break;
  case FSM_LIGHTS_ACKNOWLEDGED:
    set_redlights_out(true);
    set_indicator_redlights(true);
    break;
  }
}
