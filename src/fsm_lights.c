/* TODO extract header file */

#include <stddef.h>

#include "src/fsm_lights.h"

static void do_nothing(void);
static void lights_turn_on(void);
static void lights_turn_off(void);
static void lights_turn_indicator_on(void);

#define FSM_TRANSITION_ALIGNMENT 32
struct __attribute__((packed, aligned(FSM_TRANSITION_ALIGNMENT))) {
  fsm_lights_t current_state;
  fsm_lights_t next_state;
  fsm_lights_events_t event;
  void (*callback)(void);
} fsm_lights_transitions[] = {
    {
        // Technically this transition isn't needed, but it prevents iterating
        // through the whole list when the fsm is in error state
        .current_state = FSM_LIGHTS_ERROR,
        .event = FSM_LIGHTS_EVENT_ANY,
        .next_state = FSM_LIGHTS_ERROR,
        .callback = &do_nothing,
    },
    {
        .current_state = FSM_LIGHTS_OFF,
        .event = FSM_LIGHTS_EVENT_COMMAND_ON,
        .next_state = FSM_LIGHTS_ON,
        .callback = &lights_turn_on,
    },
    {
        .current_state = FSM_LIGHTS_ON,
        .event = FSM_LIGHTS_EVENT_COMMAND_OFF,
        .next_state = FSM_LIGHTS_OFF,
        .callback = &lights_turn_off,
    },
    {
        .current_state = FSM_LIGHTS_ON,
        .event = FSM_LIGHTS_EVENT_ACQUITTAL_RECEIVED,
        .next_state = FSM_LIGHTS_ACQUITTED,
        .callback = &lights_turn_indicator_on,
    },
    {
        .current_state = FSM_LIGHTS_ON,
        .event = FSM_LIGHTS_EVENT_ACQUITTAL_MISSED,
        .next_state = FSM_LIGHTS_ERROR,
        .callback = &do_nothing,
    },
    {
        .current_state = FSM_LIGHTS_ACQUITTED,
        .event = FSM_LIGHTS_EVENT_COMMAND_OFF,
        .next_state = FSM_LIGHTS_OFF,
        .callback = &lights_turn_off,
    },
};

#define FSM_LIGHTS_TRANSITIONS_COUNT                                           \
  (sizeof(fsm_lights_transitions) / sizeof(*fsm_lights_transitions))

#define FSM_LIGHTS_ACQUITTAL_DELAY 200

void process_state(fsm_lights_t *state_p, fsm_timer_t *timer_p) {

  fsm_lights_events_t current_event = FSM_LIGHTS_EVENT_NONE;
  // TODO get event according to app data

#pragma unroll
  for (size_t i = 0; i < FSM_LIGHTS_TRANSITIONS_COUNT; i++) {

    if (*state_p == fsm_lights_transitions[i].current_state) {

      fsm_lights_events_t event = fsm_lights_transitions[i].event;
      if ((current_event == event) || (FSM_LIGHTS_EVENT_ANY == event)) {

        *state_p = fsm_lights_transitions[i].next_state;
        (fsm_lights_transitions[i].callback)();
        break;
      }
    }
  }

  *timer_p += 1;
}
