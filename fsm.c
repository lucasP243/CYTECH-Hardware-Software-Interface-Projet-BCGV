// includes

/* States */
#define ST_ANY                             -1
#define ST_INIT                             0
#define ST_CONNECTION_REQUESTED             1
#define ST_DISCONNECTION_REQUESTED          2
#define ST_DISCONNECTED                     3
#define ST_OPERATIONNAL                     4
#define ST_CONNECTED                        5
#define ST_TERM                           255

/* Events */
#define EV_NONE                             0
#define EV_ANY                             -1
#define EV_CONNECTION_REQUEST_LOOP       5000
#define EV_CONNECTION_TO_OPERATIONNEL    5001

/* Callback functions called on transitions */
static int ConnectionRequested (void) { ... };
static int Operational (void) { ... };
static int Connected (void) { ... };
static int Disconnected (void) { ... };
static int Disconnected (void) { ... };
static int FsmError(void) { ... };

/* Transition structure */
typedef struct {
    int state;
    int event;
    int (*callback)(void);
    int next_state;
} tTransition;

/* Transition table */
tTransition trans[] = {
    { ST_CONNECTION_REQUESTED, EV_CONNECTION_REQUEST_LOOP, &ConnectionRequested, ST_CONNECTION_REQUESTED},
    { ST_ANY, EV_ANY, &FsmError, ST_TERM}
};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))

int GetNextEvent(int current_state)
{
    int event = EV_NONE;

    /* Here, you can get the parameters of your FSM */

    /* Build all the events */
    switch (current_state) {

        /* switch case on states */
        case ST_CONNECTION_REQUESTED :
            /*
            if (PARAM1 == ...) {
                event = 
            }
            */
            event = EV_CONNECTION_REQUEST_LOOP;
        break;
    }
    return event;
}

int main(void)
{
    int i = 0;
    int ret = 0; 
    int event = EV_NONE;
    int state = ST_INIT;
    
    /* While FSM hasn't reach end state */
    while (state != ST_TERM) {
        
        /* Get event */
        event = GetNextEvent(state);
        
        /* For each transitions */
        for (i = 0; i < TRANS_COUNT; i++) {
            /* If State is current state OR The transition applies to all states ...*/
            if ((state == trans[i].state) || (ST_ANY == trans[i].state)) {
                /* If event is the transition event OR the event applies to all */
                if ((event == trans[i].event) || (EV_ANY == trans[i].event)) {
                    /* Apply the new state */
                    state = trans[i].next_state;
                    /* Call the state function */
                    ret = (trans[i].callback)();
                    break;
                }
            }
        }
    }

    return 0;
}
