#include "MOESIF_protocol.h"
#include "../sim/mreq.h"
#include "../sim/sim.h"
#include "../sim/hash_table.h"

extern Simulator *Sim;

/*************************
 * Constructor/Destructor.
 *************************/
MOESIF_protocol::MOESIF_protocol (Hash_table *my_table, Hash_entry *my_entry)
    : Protocol (my_table, my_entry)
{
    state = MOESIF_CACHE_I;
}

MOESIF_protocol::~MOESIF_protocol ()
{    
}

void MOESIF_protocol::dump (void)
{
    const char *block_states[14] = {"X","I","S","E","O","M","F", "IS", "IM", "SM", "OM", "OOM", "FM", "FFM"};
    fprintf (stderr, "MOESIF_protocol - state: %s\n", block_states[state]);
}

void MOESIF_protocol::process_cache_request (Mreq *request)
{
	switch (state) {
    case MOESIF_CACHE_F:   do_cache_F   (request); break;
    case MOESIF_CACHE_I:   do_cache_I   (request); break;
    case MOESIF_CACHE_S:   do_cache_S   (request); break;
    case MOESIF_CACHE_E:   do_cache_E   (request); break;
    case MOESIF_CACHE_O:   do_cache_O   (request); break;
    case MOESIF_CACHE_M:   do_cache_M   (request); break;
    case MOESIF_CACHE_IS:  do_cache_IS  (request); break;
    case MOESIF_CACHE_IM:  do_cache_IM  (request); break;
    case MOESIF_CACHE_SM:  do_cache_SM  (request); break;
    case MOESIF_CACHE_OM:  do_cache_OM  (request); break;
    case MOESIF_CACHE_OOM: do_cache_OOM (request); break;
    case MOESIF_CACHE_FM:  do_cache_FM  (request); break;
    case MOESIF_CACHE_FFM: do_cache_FFM (request); break;
    default:
        fatal_error ("Invalid Cache State for MOESIF Protocol\n");
    }
}

void MOESIF_protocol::process_snoop_request (Mreq *request)
{
	switch (state) {
    case MOESIF_CACHE_F:   do_snoop_F   (request); break;
    case MOESIF_CACHE_I:   do_snoop_I   (request); break;
    case MOESIF_CACHE_S:   do_snoop_S   (request); break;
    case MOESIF_CACHE_E:   do_snoop_E   (request); break;
    case MOESIF_CACHE_O:   do_snoop_O   (request); break;
    case MOESIF_CACHE_M:   do_snoop_M   (request); break;
    case MOESIF_CACHE_IS:  do_snoop_IS  (request); break;
    case MOESIF_CACHE_IM:  do_snoop_IM  (request); break;
    case MOESIF_CACHE_SM:  do_snoop_SM  (request); break;
    case MOESIF_CACHE_OM:  do_snoop_OM  (request); break;
    case MOESIF_CACHE_OOM: do_snoop_OOM (request); break;
    case MOESIF_CACHE_FM:  do_snoop_FM  (request); break;
    case MOESIF_CACHE_FFM: do_snoop_FFM (request); break;
    default:
    	fatal_error ("Invalid Cache State for MOESIF Protocol\n");
    }
}

inline void MOESIF_protocol::do_cache_F (Mreq *request)
{
    switch (request->msg) {
    case LOAD:

        // data is in cache so give to processor
        send_DATA_to_proc(request->addr);
        break;

    case STORE:

        // GetM then wait for data on bus
        send_GETM(request->addr);
        state = MOESIF_CACHE_FM;
        Sim->cache_misses++;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: F state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_I (Mreq *request)
{
    switch (request->msg) {
    case LOAD:

        // GetS then wait for data on bus
        send_GETS(request->addr);
        state = MOESIF_CACHE_IS;
        Sim->cache_misses++;
        break;

    case STORE:

        // GetM then wait for data on bus
        send_GETM(request->addr);
        state = MOESIF_CACHE_IM;
        Sim->cache_misses++;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_S (Mreq *request)
{
    switch (request->msg) {
    case LOAD:

        // data is in cache so give to processor
        send_DATA_to_proc(request->addr);
        break;

    case STORE:

        // GetM then wait for data on bus
        send_GETM(request->addr);
        state = MOESIF_CACHE_SM;
        Sim->cache_misses++;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: S state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_E (Mreq *request)
{
    switch (request->msg) {
    case LOAD:

        // data is in cache so give to processor
        send_DATA_to_proc(request->addr);
        break;

    case STORE:

        // upgrade data in cache and give to processor
        send_DATA_to_proc(request->addr);
        state = MOESIF_CACHE_M;
        Sim->silent_upgrades++;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: E state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_O (Mreq *request)
{
    switch (request->msg) {
    case LOAD:

        // data is in cache so give to processor
        send_DATA_to_proc(request->addr);
        break;

    case STORE:

        // GetM then wait for data on bus
        send_GETM(request->addr);
        state = MOESIF_CACHE_OM;
        Sim->cache_misses++;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: O state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_M (Mreq *request)
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

inline void MOESIF_protocol::do_cache_IS (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    case STORE:
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: IS state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_IM (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    case STORE:
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: IM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_SM (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    case STORE:
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: SM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_OM (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    case STORE:
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: OM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_OOM (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    case STORE:
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: OOM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_FM (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    case STORE:
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: FM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_FFM (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    case STORE:
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: FFM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_F (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have data and are responsible for supplying it
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        break;

    case GETM:

        // we have data and are responsible for supplying it
        send_DATA_on_bus(request->addr,request->src_mid);
        // other cache entering state M, so invalidate
        state = MOESIF_CACHE_I;
        break;

    case DATA:

        // should not be receiving data
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: F state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_I (Mreq *request)
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

inline void MOESIF_protocol::do_snoop_S (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have clean data but state S cant supply it
        set_shared_line();
        break;

    case GETM:

        // other cache entering state M, so invalidate
        state = MOESIF_CACHE_I;
        break;

    case DATA:

        // should not be receiving data
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: S state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_E (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // supply data on bus then transition to state F
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_F;
        break;

    case GETM:

        // invalidate data
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_I;
        break;

    case DATA:

        // we already have the data
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: E state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_O (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have data and are responsible for supplying it
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        break;

    case GETM:

        // we have data and are responsible for supplying it
        send_DATA_on_bus(request->addr,request->src_mid);
        // other cache entering state M, so invalidate
        state = MOESIF_CACHE_I;
        break;

    case DATA:

        // should not be receiving data
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: O state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_M (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have data so supply it, then transition to state O
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_O;
        break;

    case GETM:

        // we have data so supply it, then invalidate
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_I;
        break;

    case DATA:

        // should not be receiving data
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: M state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_IS (Mreq *request)
{
    switch (request->msg) {
    case GETS:
    case GETM:
        // if in this state, then our own GetS is on the bus
        break;

    case DATA:
        
        // get data from bus
        send_DATA_to_proc(request->addr);

        // shared or exclusive
        if (get_shared_line()) {
            state = MOESIF_CACHE_S;
        } else {
            state = MOESIF_CACHE_E;
        }
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: IS state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_IM (Mreq *request)
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
        state = MOESIF_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: IM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_SM (Mreq *request)
{
    switch (request->msg) {
    case GETS:
    case GETM:
        // if in this state, then our own GetM is on the bus
        set_shared_line();
        break;
        
    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state M
        state = MOESIF_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: SM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_OM (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have data and are responsible for supplying it
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        break;

    case GETM:

        // should see our own GetM on the bus
        // we are responsibe for supplying it
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_OOM;
        break;
        
    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state M
        state = MOESIF_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: OM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_OOM (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // should not be seeing GetS on the bus
        set_shared_line();
        break;

    case GETM:

        // should not be seeing GetS on the bus
        set_shared_line();
        break;
        
    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state M
        state = MOESIF_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: OOM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_FM (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // we have data and are responsible for supplying it
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        break;

    case GETM:

        // should see our own GetM on the bus
        // we are responsibe for supplying it
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_FFM;
        break;
        
    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state M
        state = MOESIF_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: FM state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_FFM (Mreq *request)
{
    switch (request->msg) {
    case GETS:

        // should not be seeing GetS on the bus
        set_shared_line();
        break;

    case GETM:

        // should not be seeing GetS on the bus
        set_shared_line();
        break;
        
    case DATA:
        
        // data has been supplied, so send it to the processor
        send_DATA_to_proc(request->addr);
        // complete transition to state M
        state = MOESIF_CACHE_M;
        break;

    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: FFM state shouldn't see this message\n");
    }
}

