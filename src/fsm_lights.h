#ifndef FSM_LIGHTS_H
#define FSM_LIGHTS_H

#include "lib/data_dictionary.h"

typedef enum {
  FSM_LIGHTS_EVENT_NONE = 0,
  FSM_LIGHTS_EVENT_ANY = 1,
  FSM_LIGHTS_EVENT_COMMAND_ON = 2,
  FSM_LIGHTS_EVENT_COMMAND_OFF = 3,
  FSM_LIGHTS_EVENT_ACQUITTAL_RECEIVED = 4,
  FSM_LIGHTS_EVENT_ACQUITTAL_MISSED = -1,
} fsm_lights_events_t; /* TODO should move to types.h */

void process_state(fsm_lights_t *, fsm_timer_t *);

#endif // FSM_LIGHTS_H
