#include "MOSI_protocol.h"
#include "../sim/mreq.h"
#include "../sim/sim.h"
#include "../sim/hash_table.h"

extern Simulator *Sim;

/*************************
 * Constructor/Destructor.
 *************************/
MOSI_protocol::MOSI_protocol (Hash_table *my_table, Hash_entry *my_entry)
    : Protocol (my_table, my_entry)
{
    state = MOSI_CACHE_I;
}

MOSI_protocol::~MOSI_protocol ()
{    
}

void MOSI_protocol::dump (void)
{
    const char *block_states[10] = {"X","I","S","O","M", "IS", "IM", "SM", "OM", "OOM"};
    fprintf (stderr, "MOSI_protocol - state: %s\n", block_states[state]);
}

void MOSI_protocol::process_cache_request (Mreq *request)
{
    switch (state) {
    case MOSI_CACHE_I:   do_cache_I   (request); break;
    case MOSI_CACHE_S:   do_cache_S   (request); break;
    case MOSI_CACHE_O:   do_cache_O   (request); break;
    case MOSI_CACHE_M:   do_cache_M   (request); break;
    case MOSI_CACHE_IS:  do_cache_IS  (request); break;
    case MOSI_CACHE_IM:  do_cache_IM  (request); break;
    case MOSI_CACHE_SM:  do_cache_SM  (request); break;
    case MOSI_CACHE_OM:  do_cache_OM  (request); break;
    case MOSI_CACHE_OOM: do_cache_OOM (request); break;
    default:
        fatal_error ("Invalid Cache State for MOSI Protocol\n");
    }
}

void MOSI_protocol::process_snoop_request (Mreq *request)
{
    switch (state) {
    case MOSI_CACHE_I:   do_snoop_I   (request); break;
    case MOSI_CACHE_S:   do_snoop_S   (request); break;
    case MOSI_CACHE_O:   do_snoop_O   (request); break;
    case MOSI_CACHE_M:   do_snoop_M   (request); break;
    case MOSI_CACHE_IS:  do_snoop_IS  (request); break;
    case MOSI_CACHE_IM:  do_snoop_IM  (request); break;
    case MOSI_CACHE_SM:  do_snoop_SM  (request); break;
    case MOSI_CACHE_OM:  do_snoop_OM  (request); break;
    case MOSI_CACHE_OOM: do_snoop_OOM (request); break;
    default:
        fatal_error ("Invalid Cache State for MOSI Protocol\n");
    }
}

inline void MOSI_protocol::do_cache_I (Mreq *request)
{
    switch (request->msg) {
    case LOAD:

        // GetS then wait for data on bus
        send_GETS(request->addr);
        state = MOSI_CACHE_IS;
        Sim->cache_misses++;
        break;

    case STORE:

        // GetM then wait for data on bus
        send_GETM(request->addr);
        state = MOSI_CACHE_IM;
        Sim->cache_misses++;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_cache_S (Mreq *request)
{
    switch (request->msg) {
    case LOAD:

        // data is in cache so give to processor
        send_DATA_to_proc(request->addr);
        break;

    case STORE:

        // GetM then wait for data on bus
        send_GETM(request->addr);
        state = MOSI_CACHE_SM;
        Sim->cache_misses++;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_cache_O (Mreq *request)
{
    switch (request->msg) {
    case LOAD:

        // data is in cache so give to processor
        send_DATA_to_proc(request->addr);
        break;

    case STORE:

        // GetM then wait for data on bus
        send_GETM(request->addr);
        state = MOSI_CACHE_OM;
        Sim->cache_misses++;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: E state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_cache_M (Mreq *request)
{
    switch (request->msg) {
    case LOAD:

        // data is in cache so give to processor
        send_DATA_to_proc(request->addr);
        break;

    case STORE:

        // data is in cache so give to processor
        send_DATA_to_proc(request->addr);
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: M state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_cache_IS (Mreq *request)
{
    switch (request->msg) {
    // waiting on data from the bus so this request should not happen
    case LOAD:
    case STORE:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error("Should only have one outstanding request per processor!");
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_cache_IM (Mreq *request)
{
    switch (request->msg) {
    // waiting on data from the bus so this request should not happen
    case LOAD:
    case STORE:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error("Should only have one outstanding request per processor!");
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_cache_SM (Mreq *request)
{
    switch (request->msg) {
    // waiting on data from the bus so this request should not happen
    case LOAD:
    case STORE:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error("Should only have one outstanding request per processor!");
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_cache_OM (Mreq *request)
{
    switch (request->msg) {
    // waiting on data from the bus so this request should not happen
    case LOAD:
    case STORE:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error("Should only have one outstanding request per processor!");
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_cache_OOM (Mreq *request)
{
    switch (request->msg) {
    // waiting on data from the bus so this request should not happen
    case LOAD:
    case STORE:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error("Should only have one outstanding request per processor!");
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_snoop_I (Mreq *request)
{
    switch (request->msg) {
    case GETS:
    case GETM:
    case DATA:

        // cannot supply data since data is invalid
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_snoop_S (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have clean data but state S cant supply it
        break;

    case GETM:

        // other cache entering state M, so invalidate
        state = MOSI_CACHE_I;
        break;

    case DATA:

        // should not be receiving data
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_snoop_O (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have data and are responsible for supplying it
        send_DATA_on_bus(request->addr,request->src_mid);
        break;

    case GETM:

        // we have data and are responsible for supplying it
        send_DATA_on_bus(request->addr,request->src_mid);
        // other cache entering state M, so invalidate
        state = MOSI_CACHE_I;
        break;

    case DATA:

        // should not be receiving data
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_snoop_M (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have data so supply it, then transition to state O
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOSI_CACHE_O;
        break;

    case GETM:

        // we have data so supply it, then invalidate
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOSI_CACHE_I;
        break;

    case DATA:

        // should not be receiving data
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_snoop_IS (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // should see our own GetS on the bus
        break;

    case GETM:
        
        // should not be seeing GetM on the bus
        break;

    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state S
        state = MOSI_CACHE_S;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_snoop_IM (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // should not be seeing GetS on the bus
        break;

    case GETM:
        
        // should see our own GetM on the bus
        break;
        
    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state M
        state = MOSI_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_snoop_SM (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // should not be seeing GetS on the bus
        break;

    case GETM:

        // should see our own GetM on the bus
        break;
        
    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state M
        state = MOSI_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_snoop_OM (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have data and are responsible for supplying it
        send_DATA_on_bus(request->addr,request->src_mid);
        break;

    case GETM:

        // should see our own GetM on the bus
        // we are responsibe for supplying it
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOSI_CACHE_OOM;
        break;
        
    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state M
        state = MOSI_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOSI_protocol::do_snoop_OOM (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // should not be seeing GetS on the bus
        break;

    case GETM:

        // should not be seeing GetS on the bus
        break;
        
    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state M
        state = MOSI_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

