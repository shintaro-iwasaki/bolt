#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <abt.h>

# define KMP_SIZE_T_MAX (0xFFFFFFFFFFFFFFFF)
#define TCW_SYNC_8(a,b)     (a) = (b)
#define TCW_SYNC_PTR(a,b)   TCW_SYNC_8((a),(b))
#define TCR_SYNC_8(a)       (a)
#define TCR_SYNC_PTR(a)     ((void *)TCR_SYNC_8(a))
# define VOLATILE_CAST(x)        (x)
#define CACHE_LINE                  128
# define KMP_ALIGN_CACHE      __attribute__((aligned(CACHE_LINE))) 

/* typedef used by the omp library calls */
typedef int                kmp_int32;
typedef unsigned int       kmp_uint32;

/*!
 *  * Tell the fork call which compiler generated the fork call, and therefore how to deal with the call.
 *   Not used by now in this implementation but needed for the correct
 *    *  library linkage
 *   */
enum fork_context_e
{
    fork_context_gnu,                           /**< Called from GNU generated code, so must not invoke the microtask internally. */
    fork_context_intel,                         /**< Called from Intel generated code.  */
    fork_context_last
};

/*!
 *  * The ident structure that describes a source location.
 *  Not used by now in this library but needed for the correct
 *  library linkage
 *   */
typedef struct ident {
    int reserved_1;   /**<  might be used in Fortran; see above  */
    int flags;        /**<  also f.flags; KMP_IDENT_xxx flags; KMP_IDENT_KMPC identifies this union member  */
    int reserved_2;   /**<  not really used in Fortran any more; see above */
#if USE_ITT_BUILD
                            /*  but currently used for storing region-specific ITT */
                            /*  contextual information. */
#endif /* USE_ITT_BUILD */
    int reserved_3;   /**< source[4] in Fortran, do not use for C++  */
    char const *psource;    /**< String describing the source location.
                            The string is composed of semi-colon separated fields which describe the source file,
                            the function and a pair of line numbers that delimit the construct.
                             */
} ident_t;


/*
 * microtasks_t is the poiter to the code that is going
 * to be executed
 */
typedef void    (*microtask_t)( int *gtid, int *npr, ... );

/*
 * launch_t id the pointer function which calls the microtask
 */
typedef int     (*launch_t)( int gtid ,int tid, microtask_t t_pkfn,int t_argc_nested,void **t_argv_nested);

/*
 *Pointer to the microtask_t function
 *
 */
typedef void (*kmpc_micro)              ( int * global_tid, int * bound_tid, ... );


/*
 * Global variable managed by master thread for controlling
 * the nested parallelism
 */
int g_nested=0;

/* Main team structure*/
typedef struct team {
ABT_xstream master;
ABT_xstream *team;
int num_xstreams;
int num_pools;
ABT_pool *pools;
int using_task;
int single;
int ntasks;
ABT_mutex mutex;
}team_t;

team_t * main_team;

/*Parameters used in function calls */
typedef struct parameters_call{
microtask_t t_pkfn;
int t_argc;
int gtid;
int tid;
void **t_argv;
launch_t    invoker;
}parameters_call_t;

/*
 * Function where the parallel code is executed.
 * Each work unit call this function by itself
 *
 */
int __kmp_invoke_microtask( microtask_t pkfn,
                        int gtid, int tid,
                        int argc, void *p_argv[] ) {

 switch (argc) {
  default:
    fprintf(stderr, "Too many args to microtask: %d!\n", argc);
    fflush(stderr);
    exit(-1);
  case 0:

    (*pkfn)(&gtid, &tid);
break;
  case 1:
    (*pkfn)(&gtid, &tid, p_argv[0]);
break;
  case 2:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1]);
break;
case 3:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2]);
    break;
  case 4:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3]);
    break;
  case 5:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4]);
    break;
  case 6:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5]);
    break;
  case 7:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5], p_argv[6]);
    break;
  case 8:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5], p_argv[6], p_argv[7]);
    break;
  case 9:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5], p_argv[6], p_argv[7], p_argv[8]);
    break;
  case 10:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5], p_argv[6], p_argv[7], p_argv[8], p_argv[9]);
    break;
  case 11:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5], p_argv[6], p_argv[7], p_argv[8], p_argv[9], p_argv[10]);
    break;
  case 12:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5], p_argv[6], p_argv[7], p_argv[8], p_argv[9], p_argv[10],
            p_argv[11]);
    break;
  case 13:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5], p_argv[6], p_argv[7], p_argv[8], p_argv[9], p_argv[10],
            p_argv[11], p_argv[12]);
    break;
  case 14:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5], p_argv[6], p_argv[7], p_argv[8], p_argv[9], p_argv[10],
            p_argv[11], p_argv[12], p_argv[13]);
    break;
  case 15:
    (*pkfn)(&gtid, &tid, p_argv[0], p_argv[1], p_argv[2], p_argv[3], p_argv[4],
            p_argv[5], p_argv[6], p_argv[7], p_argv[8], p_argv[9], p_argv[10],
            p_argv[11], p_argv[12], p_argv[13], p_argv[14]);
}
return 1;

}


/*
 * Function which is invoked by the main function and
 * invokes the microtasks
 */
int
__kmp_invoke_task_func( int gtid, int tid,
microtask_t t_pkfn,int t_argc_nested,void **t_argv_nested )
{
    int rc= __kmp_invoke_microtask( (microtask_t) TCR_SYNC_PTR(t_pkfn),
      gtid, tid, t_argc_nested, (void **) t_argv_nested );
    return rc;
}


/*
 * This function is the Work unit function that is
 * going to be executed. This is the first step
 *
 */
void execution_start(void * args){
    parameters_call_t * a;
    a=(parameters_call_t *)args;
    a->invoker( a->gtid,a->tid ,a->t_pkfn,a->t_argc,a->t_argv);
}


/* Helper function to know the tid inside a xstream if it
 * is a work unit */
int get_tid(int gtid){
    return gtid%main_team->num_xstreams;
}


/* function executed when a for loop is found. Each work unit executes it
 * and obtain the range of iteration to be executed 
 */

void
__kmpc_for_static_init_4( ident_t *loc, int gtid, int schedtype, int *plastiter,
                      int *plower, int *pupper,
                      int *pstride, int incr, int chunk )
{
    gtid=get_tid(gtid);
   
    int totalit=((*pupper)-(*plower))+1;
    int nxstreams=main_team->num_xstreams;
    int chunksize=totalit/nxstreams;
    int no_divisible=totalit%nxstreams;
   
    if(no_divisible==0){
        *plower=gtid*chunksize;
        *pupper=(gtid+1)*chunksize-1;
    }else{
        if(gtid<no_divisible){
            *plower=gtid*(chunksize+1);
       	    *pupper=(gtid+1)*chunksize+gtid;
	}
	else{
		*plower=gtid*(chunksize+1);
		*pupper=(gtid+1)*chunksize+gtid;
	}
    }
}

/*
 * Indicates that the for loop is over
 */
void
__kmpc_for_static_fini( ident_t *loc, int global_tid )
{
  /* Nothing by the moment */
}

/*
 * Return the number of the current Execution Stream
 */
int omp_get_thread_num(){
    int gtid;
    ABT_xstream_self_rank(&gtid);
    return gtid;
}

/*
 * Return the number of Execution Streams
 */
int omp_get_num_threads(){
    return main_team->num_xstreams;
}

/*
 * It is added in the compiled code for each 
 * #pragma omp barrier
 */
int 
__kmpc_cancel_barrier(ident_t *loc, int gtid) {
  
    /* Nothing by the moment */
    return 1;
}



/*
 * This function finalizes the execution streams
 * and Argobots
 * It is included by the icc compiler to 
 * indicate the end of the program but clang 
 * compiler does not do that.
 */
void
__kmpc_end(ident_t *loc)
{

    int i;
    for (i = 1; i < main_team->num_xstreams; i++) {
        ABT_xstream_join(main_team->team[i]);
        ABT_xstream_free(&main_team->team[i]);
    }

    ABT_finalize();
}

/*
 * Critical section functions
 */
typedef int kmp_critical_name[8];

void
__kmpc_critical( ident_t * loc, int global_tid, kmp_critical_name * crit ) {

    ABT_mutex_lock(main_team->mutex);
}

void
__kmpc_end_critical(ident_t *loc, int global_tid, kmp_critical_name *crit)
{
    ABT_mutex_unlock(main_team->mutex);
}

/*
 * Single section functions
 * Implemented by now as a master region
 */
int
__kmpc_single(ident_t *loc, int global_tid)
{
    int rc = 0;
    int gtid;
    ABT_xstream_self_rank(&gtid);
    main_team->single=1;
    main_team->ntasks=0;
    return (rc==gtid)?1:0;
}

void
__kmpc_end_single(ident_t *loc, int global_tid)
{
    main_team->single=0;
}

/*
 * OMP Task parallelism functions needed
 */
#define KMP_TASKDATA_TO_TASK(taskdata) (kmp_task_t *) (taskdata + 1)
typedef kmp_int32 (* kmp_routine_entry_t)( kmp_int32, void * );

/* kmp_taskdata reduced to the minimum because most of the 
 * omp features are not used */

struct kmp_taskdata {
ident_t *               td_ident;
};

typedef struct kmp_taskdata  kmp_taskdata_t;


typedef struct  kmp_task {                   /* GEH: Shouldn't this be aligned somehow? */
    void *              shareds;            /**< pointer to block of pointers to shared vars   */
    kmp_routine_entry_t routine;            /**< pointer to routine to call for executing task */
} kmp_task_t;

//---------------------------------------------------------------------------------
//// __kmp_task_alloc: Allocate the taskdata and task data structures for a task
////
//// loc_ref: source location information
//// gtid: global thread number.
//// sizeof_kmp_task_t:  Size in bytes of kmp_task_t data structure including private vars accessed in task.
//// sizeof_shareds:  Size in bytes of array of pointers to shared vars accessed in task.
//// task_entry: Pointer to task code entry point generated by compiler.
//// returns: a pointer to the allocated kmp_task_t structure (task).


kmp_task_t *
__kmp_task_alloc( ident_t *loc_ref, int gtid,
                  size_t sizeof_kmp_task_t, size_t sizeof_shareds,
                  kmp_routine_entry_t task_entry )
{
    kmp_task_t *task;
    kmp_taskdata_t *taskdata;
    size_t shareds_offset;

    shareds_offset = sizeof( kmp_taskdata_t ) + sizeof_kmp_task_t;
   
    taskdata = (kmp_taskdata_t *) malloc( shareds_offset + sizeof_shareds );


    task                      = KMP_TASKDATA_TO_TASK(taskdata);

    if (sizeof_shareds > 0) {
        task->shareds         = & ((char *) taskdata)[ shareds_offset ];
    } else {
        task->shareds         = NULL;
    }
    task->routine             = task_entry;

    taskdata->td_ident        = loc_ref;

    return task;
}


/* This is the first step of the task creation mechanism */
kmp_task_t *
__kmpc_omp_task_alloc( ident_t *loc_ref, int gtid, int flags,
                       size_t sizeof_kmp_task_t, size_t sizeof_shareds,
                       kmp_routine_entry_t task_entry ){

    kmp_task_t *retval;


    retval = __kmp_task_alloc( loc_ref, gtid, sizeof_kmp_task_t,
                               sizeof_shareds, task_entry );


    return retval;



}

void execution_task(void * arg){

    int gtid;
    ABT_xstream_self_rank(&gtid);
    kmp_task_t * aux = (kmp_task_t *)arg;

    (*(aux->routine))(gtid, aux);

}

/* This is the second and final step for executing a task, it has been created in the
 * __kmpc_omp_task_alloc function and now, it is executed */
kmp_int32
__kmpc_omp_task_with_deps( ident_t *loc_ref, kmp_int32 gtid, kmp_task_t * new_task,
                            kmp_int32 ndeps, 
                            kmp_int32 ndeps_noalias)
{
    int dest;
    ABT_task task; 
    if(main_team->single==0){
 	dest=gtid%main_team->num_xstreams;
    }
    else{
	dest=main_team->ntasks%main_team->num_xstreams;
	main_team->ntasks++;
    }
     
    ABT_task_create( main_team->pools[dest], execution_task,
                   (void *)new_task,&task);
     
    ABT_task_free(&task);
    

 return 1;
}
/*
 * The argobots team is created
 * Only one team is used because
 * the parallel code is executed by Work units
 * inside team execution streams
 */
void create_argobots_team(){

    int i;
    main_team = (team_t *) malloc (sizeof(team_t));
    if(getenv("OMP_NUM_THREADS")!=NULL)
        main_team->num_xstreams=atoi(getenv("OMP_NUM_THREADS"));
    else
        main_team->num_xstreams=1;
    if(getenv("OMP_USING_TASK")!=NULL)
        main_team->using_task=atoi(getenv("OMP_USING_TASK"));
    else
        main_team->using_task=0;

    main_team->num_pools=main_team->num_xstreams;
    ABT_xstream_self(&main_team->master);
    main_team->team=(ABT_xstream *) malloc(sizeof (ABT_xstream) * main_team->num_xstreams);
    main_team->pools=(ABT_pool *) malloc(sizeof (ABT_pool) * main_team->num_pools);
    ABT_mutex_create(&main_team->mutex);   
    main_team->single=0;
    for (i = 0; i < main_team->num_pools; i++) {
        ABT_pool_create_basic(ABT_POOL_FIFO, ABT_POOL_ACCESS_MPMC, ABT_TRUE,
            &main_team->pools[i]);
    }

    ABT_xstream_self(&main_team->team[0]);
    ABT_xstream_set_main_sched_basic(main_team->team[0], ABT_SCHED_DEFAULT,
        1, &main_team->pools[0]);
    
    for (i = 1; i < main_team->num_xstreams; i++) {
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 
		1, &main_team->pools[i%main_team->num_pools],
                ABT_SCHED_CONFIG_NULL, &main_team->team[i]);
        ABT_xstream_start(main_team->team[i]);
    }
}

/* For each parallel region in the code, 
 * this function is called and it creates the work units needed.
 * It can create ULTs and tasklets. It depends of an environment variable
 * by now, but it will be changed */
int
__kmp_fork_call(
    ident_t   * loc,
    int         gtid,
    enum fork_context_e  call_context, // Intel, GNU, ...
    int   argc,
    microtask_t microtask,
    launch_t    invoker,
    va_list   * ap, int nested
    )
{
    va_list    aux;
    void          **argv;
    int i;
    parameters_call_t * args;
  
    args=(parameters_call_t *) malloc(sizeof(parameters_call_t)*main_team->num_xstreams);
    int e;
    for (e=0;e<main_team->num_xstreams;e++){
    va_copy( aux,*ap);
        TCW_SYNC_PTR(args[e].t_pkfn, microtask);
        args[e].invoker     = invoker;
        args[e].t_argc = argc;
        args[e].t_argv = (void **) malloc(sizeof(void *)*argc);
        argv = (void**) args[e].t_argv;
        for( i=argc-1; i >= 0; --i ){
             *argv++ = va_arg( aux, void * );
        }
    }
    
    for (i=0;i< main_team->num_xstreams; i++) {
        int dest=(nested==1)?gtid:i;
        args[i].tid = i;
        args[i].gtid = ((gtid+1)*main_team->num_xstreams)+i;
        if(nested==1 && main_team->using_task==1){
	    ABT_task_create( main_team->pools[dest], execution_start,
                   (void *)&args[i],NULL);
	}
        else{
            ABT_thread_create( main_team->pools[dest], execution_start,
                   (void *)&args[i],ABT_THREAD_ATTR_NULL ,NULL);
        }
    }
   ABT_thread_yield();
    
    return 1;
}





/* For each parallel region in the code, 
 * this function is called and it waits until all the work is done.*/

void
__kmp_join_call(ident_t *loc, int gtid
               , int exit_teams
)
{
int i;
    for (i = 0; i < main_team->num_xstreams; i++) {
        size_t size;
        do {
            ABT_pool_get_size   (main_team->pools[i],&size);
        } while (size != 0 );

    }
}



/*
 * The main function of all runtime. It is called
 * each time a parallel code is found. It calls
 * a fork call where the code is splitted and executed and a join call
 * that waits all work is done
 */

void
__kmpc_fork_call(ident_t *loc, int argc, kmpc_micro microtask, ...)
{

    va_list     ap;
    va_start(   ap, microtask );

    if(main_team==NULL){
	ABT_init(0, NULL);
        create_argobots_team();
    }

    int gtid;
    int nested=0;
    ABT_xstream_self_rank(&gtid);
    if(gtid==0)
        g_nested++;

    if((gtid!=0) || (gtid==0 && g_nested>1)){
        nested=1;
    }

    __kmp_fork_call( loc, gtid, fork_context_intel,
            argc,
            VOLATILE_CAST(microtask_t) microtask,
            VOLATILE_CAST(launch_t)    __kmp_invoke_task_func,
            &ap, nested
            );
    __kmp_join_call( loc, gtid,0 );

    if (gtid==0){
       g_nested--;
    }
    /* If next line is uncommented, the code only can
 * execute 1 para llel region. This call should be added
 * by the compiler but it does not do that. Intel compiler
 * introduces it by default */
    /*__kmpc_end(loc);*/

    va_end( ap );
}



