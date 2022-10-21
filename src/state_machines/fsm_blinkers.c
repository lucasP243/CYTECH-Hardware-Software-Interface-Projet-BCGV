#include <stddef.h>

#include "fsm_blinkers.h"

/**
 * \brief The different states of the blinkers FSM.
 */
typedef enum fsm_blinkers_state_t {
  FSM_BLINKERS_OFF = 0,
  FSM_BLINKERS_ACTIVE_ON = 1,
  FSM_BLINKERS_ACTIVE_OFF = 2,
  FSM_BLINKERS_ACTIVE_ON_ACKNOWLEDGED = 3,
  FSM_BLINKERS_ACTIVE_OFF_ACKNOWLEDGED = 4,
  FSM_BLINKERS_ERROR = -1
} fsm_blinkers_state_t;

/**
 * \brief The different events triggering state changes in the FSM.
 */
typedef enum fsm_blinkers_event_t {
  FSM_BLINKERS_EVENT_ANY = 0,
  FSM_BLINKERS_EVENT_COMMAND_ON = 1,
  FSM_BLINKERS_EVENT_COMMAND_OFF = 2,
  FSM_BLINKERS_EVENT_BLINK = 3,
  FSM_BLINKERS_EVENT_ACK_RECEIVED = 4,
  FSM_BLINKERS_EVENT_ACK_MISSED = -1,
} fsm_blinkers_event_t;

#define FSM_BLINKERS_TRANSITION_ALIGNMENT 16
/**
 * \brief The list of all possible transitions from one state to another,
 * associated with the corresponding trigger event.
 */
struct __attribute__((aligned(FSM_BLINKERS_TRANSITION_ALIGNMENT))) {
  fsm_blinkers_state_t current_state;
  fsm_blinkers_state_t next_state;
  fsm_blinkers_event_t event;
} fsm_blinkers_transitions[] = {
    {
        // Technically this transition isn't needed, but it prevents iterating
        // through the whole list when the fsm is in error state
        .current_state = FSM_BLINKERS_ERROR,
        .next_state = FSM_BLINKERS_ERROR,
        .event = FSM_BLINKERS_EVENT_ANY,
    },
    {
        .current_state = FSM_BLINKERS_OFF,
        .next_state = FSM_BLINKERS_ACTIVE_ON,
        .event = FSM_BLINKERS_EVENT_COMMAND_ON,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_ON,
        .next_state = FSM_BLINKERS_OFF,
        .event = FSM_BLINKERS_EVENT_COMMAND_OFF,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_ON,
        .next_state = FSM_BLINKERS_ACTIVE_ON_ACKNOWLEDGED,
        .event = FSM_BLINKERS_EVENT_ACK_RECEIVED,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_ON,
        .next_state = FSM_BLINKERS_ERROR,
        .event = FSM_BLINKERS_EVENT_ACK_MISSED,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_ON_ACKNOWLEDGED,
        .next_state = FSM_BLINKERS_OFF,
        .event = FSM_BLINKERS_EVENT_COMMAND_OFF,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_ON_ACKNOWLEDGED,
        .next_state = FSM_BLINKERS_ACTIVE_OFF,
        .event = FSM_BLINKERS_EVENT_BLINK,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_OFF,
        .next_state = FSM_BLINKERS_OFF,
        .event = FSM_BLINKERS_EVENT_COMMAND_OFF,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_OFF,
        .next_state = FSM_BLINKERS_ACTIVE_OFF_ACKNOWLEDGED,
        .event = FSM_BLINKERS_EVENT_ACK_RECEIVED,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_OFF,
        .next_state = FSM_BLINKERS_ERROR,
        .event = FSM_BLINKERS_EVENT_ACK_MISSED,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_OFF_ACKNOWLEDGED,
        .next_state = FSM_BLINKERS_OFF,
        .event = FSM_BLINKERS_EVENT_COMMAND_OFF,
    },
    {
        .current_state = FSM_BLINKERS_ACTIVE_OFF_ACKNOWLEDGED,
        .next_state = FSM_BLINKERS_ACTIVE_ON,
        .event = FSM_BLINKERS_EVENT_BLINK,
    },
};

#define FSM_BLINKERS_TRANSITIONS_COUNT                                         \
  (sizeof(fsm_blinkers_transitions) / sizeof(*fsm_blinkers_transitions))

/**
 * \brief Tick a FSM with an event, changing its state if a corresponding
 * transition exists.
 *
 * \param[in,out]   fsm_blinkers_t          Pointer to the FSM to tick.
 * \param[in]       fsm_blinkers_event_t    The event to tick the FSM with.
 * \param[in,out]   fsm_timer_t             Pointer to the FSM timer.
 */
void fsm_blinkers_tick(fsm_blinkers_t *fsm_p, fsm_blinkers_event_t event_p,
                       fsm_timer_t *timer_p) {
#pragma unroll
  for (size_t i = 0; i < FSM_BLINKERS_TRANSITIONS_COUNT; i++) {

    fsm_blinkers_state_t *current_state = fsm_p;

    if (*current_state == fsm_blinkers_transitions[i].current_state) {
      fsm_blinkers_event_t event = fsm_blinkers_transitions[i].event;

      if ((event_p == event) || (FSM_BLINKERS_EVENT_ANY == event)) {
        *current_state = fsm_blinkers_transitions[i].next_state;
        *timer_p = 0;
        break;
      }
    }
  }

  *timer_p += 1;
}

#define FSM_BLINKERS_ACKNOWLEDGEMENT_DELAY 100
#define FSM_BLINKERS_BLINKING_DELAY 100

__attribute__((unused)) void compute_left_blinker() {

  fsm_blinkers_t fsm = get_fsm_left_blinker();
  fsm_blinkers_event_t event;
  fsm_timer_t timer = get_fsm_left_blinker_timer();

  // Compute event

  if (get_left_blinker_in() || get_warnings_in()) {

    if (get_left_blinker_acknowledgement()) {
      event = FSM_BLINKERS_EVENT_ACK_RECEIVED;
    } else if (timer > FSM_BLINKERS_ACKNOWLEDGEMENT_DELAY) {
      event = FSM_BLINKERS_EVENT_ACK_MISSED;
    } else if (timer > FSM_BLINKERS_BLINKING_DELAY) {
      event = FSM_BLINKERS_EVENT_BLINK;
    } else {
      event = FSM_BLINKERS_EVENT_COMMAND_ON;
    }

  } else {
    event = FSM_BLINKERS_EVENT_COMMAND_OFF;
  }

  // Tick FSM

  fsm_blinkers_tick(&fsm, event, &timer);

  // Update data

  set_fsm_left_blinker(fsm);
  set_fsm_left_blinker_timer(timer);

  switch ((fsm_blinkers_state_t)fsm) {

  case FSM_BLINKERS_OFF:
  case FSM_BLINKERS_ERROR:
  case FSM_BLINKERS_ACTIVE_OFF:
  case FSM_BLINKERS_ACTIVE_OFF_ACKNOWLEDGED:
    set_left_blinker_out(false);
    break;
  case FSM_BLINKERS_ACTIVE_ON:
  case FSM_BLINKERS_ACTIVE_ON_ACKNOWLEDGED:
    set_left_blinker_out(true);
    set_indicator_warnings(get_warnings_in());
    break;
  }
}

__attribute__((unused)) void compute_right_blinker() {

  fsm_blinkers_t fsm = get_fsm_right_blinker();
  fsm_blinkers_event_t event;
  fsm_timer_t timer = get_fsm_right_blinker_timer();

  // Compute event

  if (get_right_blinker_in() || get_warnings_in()) {

    if (get_right_blinker_acknowledgement()) {
      event = FSM_BLINKERS_EVENT_ACK_RECEIVED;
    } else if (timer > FSM_BLINKERS_ACKNOWLEDGEMENT_DELAY) {
      event = FSM_BLINKERS_EVENT_ACK_MISSED;
    } else if (timer > FSM_BLINKERS_BLINKING_DELAY) {
      event = FSM_BLINKERS_EVENT_BLINK;
    } else {
      event = FSM_BLINKERS_EVENT_COMMAND_ON;
    }

  } else {
    event = FSM_BLINKERS_EVENT_COMMAND_OFF;
  }

  // Tick FSM

  fsm_blinkers_tick(&fsm, event, &timer);

  // Update data

  set_fsm_right_blinker(fsm);
  set_fsm_right_blinker_timer(timer);

  switch ((fsm_blinkers_state_t)fsm) {

  case FSM_BLINKERS_OFF:
  case FSM_BLINKERS_ERROR:
  case FSM_BLINKERS_ACTIVE_OFF:
  case FSM_BLINKERS_ACTIVE_OFF_ACKNOWLEDGED:
    set_right_blinker_out(false);
    break;
  case FSM_BLINKERS_ACTIVE_ON:
  case FSM_BLINKERS_ACTIVE_ON_ACKNOWLEDGED:
    set_right_blinker_out(true);
    set_indicator_warnings(get_warnings_in());
    break;
  }
}