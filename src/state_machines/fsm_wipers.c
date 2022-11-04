#include <stddef.h>

#include "fsm_wipers.h"

/**
 * \brief The different states of the wipers FSM.
 */
typedef enum fsm_wipers_state_t {
  FSM_WIPERS_OFF = 0,
  FSM_WIPERS_ON = 1,
  FSM_WIPERS_WASH = 2,
  FSM_WIPERS_WAIT = 3
} fsm_wipers_state_t;

/**
 * \brief The different events triggering state changes in the FSM.
 */
typedef enum fsm_wipers_event_t {
  FSM_WIPERS_EVENT_ANY = 0,
  FSM_WIPERS_EVENT_COMMAND_WIPE = 1,
  FSM_WIPERS_EVENT_COMMAND_WASH = 2,
  FSM_WIPERS_EVENT_COMMAND_OFF = 4,
  FSM_WIPERS_EVENT_TIMEOUT = 5
} fsm_wipers_event_t;

#define FSM_WIPERS_TRANSITION_ALIGNMENT 16
/**
 * \brief The list of all possible transitions from one state to another,
 * associated with the corresponding trigger event.
 */
struct __attribute__((aligned(FSM_WIPERS_TRANSITION_ALIGNMENT))) {
  fsm_wipers_state_t current_state;
  fsm_wipers_state_t next_state;
  fsm_wipers_event_t event;
} fsm_wipers_transitions[] = {
    {
        .current_state = FSM_WIPERS_OFF,
        .next_state = FSM_WIPERS_ON,
        .event = FSM_WIPERS_EVENT_COMMAND_WIPE,
    },
    {
        .current_state = FSM_WIPERS_OFF,
        .next_state = FSM_WIPERS_WASH,
        .event = FSM_WIPERS_EVENT_COMMAND_WASH,
    },
    {
        .current_state = FSM_WIPERS_ON,
        .next_state = FSM_WIPERS_OFF,
        .event = FSM_WIPERS_EVENT_COMMAND_OFF,
    },
    {
        .current_state = FSM_WIPERS_ON,
        .next_state = FSM_WIPERS_WASH,
        .event = FSM_WIPERS_EVENT_COMMAND_WASH,
    },
    {
        .current_state = FSM_WIPERS_WASH,
        .next_state = FSM_WIPERS_WAIT,
        .event = FSM_WIPERS_EVENT_COMMAND_OFF,
    },
    {
        // This transition is here to prevent being stuck on FSM_WIPERS_WASH
        // when COMMAND_WIPE is on but COMMAND_WASH is off
        .current_state = FSM_WIPERS_WASH,
        .next_state = FSM_WIPERS_WAIT,
        .event = FSM_WIPERS_EVENT_COMMAND_WIPE,
    },
    {
        .current_state = FSM_WIPERS_WAIT,
        .next_state = FSM_WIPERS_OFF,
        .event = FSM_WIPERS_EVENT_TIMEOUT,
    },
    {
        .current_state = FSM_WIPERS_WAIT,
        .next_state = FSM_WIPERS_WASH,
        .event = FSM_WIPERS_EVENT_COMMAND_WASH,
    },
};

#define FSM_WIPERS_TRANSITIONS_COUNT                                           \
  (sizeof(fsm_wipers_transitions) / sizeof(*fsm_wipers_transitions))

/**
 * \brief Tick a FSM with an event, changing its state if a corresponding
 * transition exists.
 *
 * \param[in,out]   fsm_wipers_t          Pointer to the FSM to tick.
 * \param[in]       fsm_wipers_event_t    The event to tick the FSM with.
 * \param[in,out]   fsm_timer_t             Pointer to the FSM timer.
 */
void fsm_wipers_tick(fsm_wipers_t *fsm_p, fsm_wipers_event_t event_p,
                     fsm_timer_t *timer_p) {
#pragma unroll
  for (size_t i = 0; i < FSM_WIPERS_TRANSITIONS_COUNT; i++) {

    fsm_wipers_state_t *current_state = (fsm_wipers_state_t *)fsm_p;

    if (*current_state == fsm_wipers_transitions[i].current_state) {
      fsm_wipers_event_t event = fsm_wipers_transitions[i].event;

      if ((event_p == event) || (FSM_WIPERS_EVENT_ANY == event)) {
        *current_state = fsm_wipers_transitions[i].next_state;
        *timer_p = 0;
        break;
      }
    }
  }

  *timer_p += 1;
}

#define FSM_WIPERS_WAITING_DELAY 200

void compute_wipers() {

  fsm_wipers_t fsm = get_fsm_wipers();
  fsm_wipers_event_t event;
  fsm_timer_t timer = get_fsm_wipers_timer();

  // Compute event

  if (fsm == FSM_WIPERS_WAIT && timer > FSM_WIPERS_WAITING_DELAY) {
    event = FSM_WIPERS_EVENT_TIMEOUT;
  } else if (get_washer_fluid_in()) {
    event = FSM_WIPERS_EVENT_COMMAND_WASH;
  } else if (get_wipers_in()) {
    event = FSM_WIPERS_EVENT_COMMAND_WIPE;
  } else {
    event = FSM_WIPERS_EVENT_COMMAND_OFF;
  }

  // Tick FSM

  fsm_wipers_tick(&fsm, event, &timer);

  // Update data

  set_fsm_wipers(fsm);
  set_fsm_wipers_timer(timer);

  switch ((fsm_wipers_state_t)fsm) {

  case FSM_WIPERS_OFF:
  case FSM_WIPERS_WAIT:
    set_wipers_out(false);
    set_washer_fluid_out(false);
    break;
  case FSM_WIPERS_ON:
    set_wipers_out(true);
    set_washer_fluid_out(false);
    break;
  case FSM_WIPERS_WASH:
    set_wipers_out(true);
    set_washer_fluid_out(true);
    break;
  }
}