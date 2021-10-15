/* SNC Version 2.0.12: Wed Jul 25 17:06:40 2012: sncProgram.i */

/* Event flags */

/* Program "sncExample" */
#include "seqCom.h"

#define NUM_SS 1
#define NUM_CHANNELS 1
#define NUM_EVENTS 0
#define NUM_QUEUES 0

#define MAX_STRING_SIZE 40

#define ASYNC_OPT FALSE
#define CONN_OPT TRUE
#define DEBUG_OPT FALSE
#define MAIN_OPT TRUE
#define NEWEF_OPT TRUE
#define REENT_OPT FALSE

extern struct seqProgram sncExample;

/* Main program */
#include <string.h>
#include "epicsThread.h"
#include "iocsh.h"

int main(int argc,char *argv[]) {
    char * macro_def;
    epicsThreadId threadId;
    int callIocsh = 0;
    if(argc>1 && strcmp(argv[1],"-s")==0) {
        callIocsh=1;
        --argc; ++argv;
    }
    macro_def = (argc>1)?argv[1]:NULL;
    threadId = seq((void *)&sncExample, macro_def, 0);
    if(callIocsh) {
        seqRegisterSequencerCommands();
        iocsh(0);
    } else {
        epicsThreadExitMain();
    }
    return(0);
}

/* Variable declarations */
static double	v;

/* Not used (avoids compilation warnings) */
struct UserVar {
	int	dummy;
};

/* C code definitions */
# line 6 "./../sncExample.stt"

# line 7 "./../sncExample.stt"
void loadBoard(int i) {
# line 8 "./../sncExample.stt"
    printf("in the loadBoard function\n");
# line 9 "./../sncExample.stt"
}
# line 9 "./../sncExample.stt"


/* Entry handler */
static void entry_handler(SS_ID ssId, struct UserVar *pVar)
{
}

/* Code for state "init" in state set "ss1" */

/* Entry function for state "init" in state set "ss1" */
static void I_ss1_init(SS_ID ssId, struct UserVar *pVar)
{
/* Entry 1: */
# line 14 "./../sncExample.stt"
	v = 0;
# line 15 "./../sncExample.stt"
	seq_pvPut(ssId, 0 /* v */, 0);
# line 16 "./../sncExample.stt"
	printf("sncExample: initializing\n");
}

/* Delay function for state "init" in state set "ss1" */
static void D_ss1_init(SS_ID ssId, struct UserVar *pVar)
{
# line 22 "./../sncExample.stt"
	seq_delayInit(ssId, 0, (10));
}

/* Event function for state "init" in state set "ss1" */
static long E_ss1_init(SS_ID ssId, struct UserVar *pVar, short *pTransNum, short *pNextState)
{
# line 22 "./../sncExample.stt"
	if (seq_delay(ssId, 0))
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "init" in state set "ss1" */
static void A_ss1_init(SS_ID ssId, struct UserVar *pVar, short transNum)
{
	switch(transNum)
	{
	case 0:
		{
# line 19 "./../sncExample.stt"
			printf("sncExample: Startup delay over\n");
			v = 1;
			seq_pvPut(ssId, 0 /* v */, 0);
		}
		return;
	}
}
/* Code for state "low" in state set "ss1" */

/* Delay function for state "low" in state set "ss1" */
static void D_ss1_low(SS_ID ssId, struct UserVar *pVar)
{
# line 27 "./../sncExample.stt"
}

/* Event function for state "low" in state set "ss1" */
static long E_ss1_low(SS_ID ssId, struct UserVar *pVar, short *pTransNum, short *pNextState)
{
# line 27 "./../sncExample.stt"
	if (v > 5.0)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "low" in state set "ss1" */
static void A_ss1_low(SS_ID ssId, struct UserVar *pVar, short transNum)
{
	switch(transNum)
	{
	case 0:
		{
# line 26 "./../sncExample.stt"
			printf("sncExample: Changing to high\n");
		}
		return;
	}
}
/* Code for state "high" in state set "ss1" */

/* Delay function for state "high" in state set "ss1" */
static void D_ss1_high(SS_ID ssId, struct UserVar *pVar)
{
# line 32 "./../sncExample.stt"
}

/* Event function for state "high" in state set "ss1" */
static long E_ss1_high(SS_ID ssId, struct UserVar *pVar, short *pTransNum, short *pNextState)
{
# line 32 "./../sncExample.stt"
	if (v <= 5.0)
	{
		*pNextState = 1;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "high" in state set "ss1" */
static void A_ss1_high(SS_ID ssId, struct UserVar *pVar, short transNum)
{
	switch(transNum)
	{
	case 0:
		{
# line 31 "./../sncExample.stt"
			printf("sncExample: Changing to low\n");
		}
		return;
	}
}
/* Code for state "waiting" in state set "ss1" */

/* Entry function for state "waiting" in state set "ss1" */
static void I_ss1_waiting(SS_ID ssId, struct UserVar *pVar)
{
/* Entry 1: */
# line 36 "./../sncExample.stt"
	printf("waiting\n");
}

/* Delay function for state "waiting" in state set "ss1" */
static void D_ss1_waiting(SS_ID ssId, struct UserVar *pVar)
{
# line 40 "./../sncExample.stt"
# line 43 "./../sncExample.stt"
# line 46 "./../sncExample.stt"
# line 49 "./../sncExample.stt"
}

/* Event function for state "waiting" in state set "ss1" */
static long E_ss1_waiting(SS_ID ssId, struct UserVar *pVar, short *pTransNum, short *pNextState)
{
# line 40 "./../sncExample.stt"
	if (v == 6)
	{
		*pNextState = 4;
		*pTransNum = 0;
		return TRUE;
	}
# line 43 "./../sncExample.stt"
	if (v == 7)
	{
		*pNextState = 5;
		*pTransNum = 1;
		return TRUE;
	}
# line 46 "./../sncExample.stt"
	if (v == 8)
	{
		*pNextState = 6;
		*pTransNum = 2;
		return TRUE;
	}
# line 49 "./../sncExample.stt"
	if (v == 9)
	{
		*pNextState = 7;
		*pTransNum = 3;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "waiting" in state set "ss1" */
static void A_ss1_waiting(SS_ID ssId, struct UserVar *pVar, short transNum)
{
	switch(transNum)
	{
	case 0:
		{
# line 39 "./../sncExample.stt"
			printf("load all configurations\n");
		}
		return;
	case 1:
		{
# line 42 "./../sncExample.stt"
			printf("load board 1 configurations\n");
		}
		return;
	case 2:
		{
# line 45 "./../sncExample.stt"
			printf("load board 2 configurations\n");
		}
		return;
	case 3:
		{
# line 48 "./../sncExample.stt"
			printf("load board 3 configurations\n");
		}
		return;
	}
}
/* Code for state "loading_all" in state set "ss1" */

/* Entry function for state "loading_all" in state set "ss1" */
static void I_ss1_loading_all(SS_ID ssId, struct UserVar *pVar)
{
/* Entry 1: */
# line 53 "./../sncExample.stt"
	loadBoard(1);
# line 54 "./../sncExample.stt"
	loadBoard(2);
# line 55 "./../sncExample.stt"
	loadBoard(3);
# line 56 "./../sncExample.stt"
	v = 1;
# line 57 "./../sncExample.stt"
	seq_pvPut(ssId, 0 /* v */, 0);
}

/* Delay function for state "loading_all" in state set "ss1" */
static void D_ss1_loading_all(SS_ID ssId, struct UserVar *pVar)
{
# line 60 "./../sncExample.stt"
}

/* Event function for state "loading_all" in state set "ss1" */
static long E_ss1_loading_all(SS_ID ssId, struct UserVar *pVar, short *pTransNum, short *pNextState)
{
# line 60 "./../sncExample.stt"
	if (1)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "loading_all" in state set "ss1" */
static void A_ss1_loading_all(SS_ID ssId, struct UserVar *pVar, short transNum)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}
/* Code for state "loading_board1" in state set "ss1" */

/* Entry function for state "loading_board1" in state set "ss1" */
static void I_ss1_loading_board1(SS_ID ssId, struct UserVar *pVar)
{
/* Entry 1: */
# line 64 "./../sncExample.stt"
	loadBoard(1);
# line 65 "./../sncExample.stt"
	v = 1;
# line 66 "./../sncExample.stt"
	seq_pvPut(ssId, 0 /* v */, 0);
}

/* Delay function for state "loading_board1" in state set "ss1" */
static void D_ss1_loading_board1(SS_ID ssId, struct UserVar *pVar)
{
# line 69 "./../sncExample.stt"
}

/* Event function for state "loading_board1" in state set "ss1" */
static long E_ss1_loading_board1(SS_ID ssId, struct UserVar *pVar, short *pTransNum, short *pNextState)
{
# line 69 "./../sncExample.stt"
	if (1)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "loading_board1" in state set "ss1" */
static void A_ss1_loading_board1(SS_ID ssId, struct UserVar *pVar, short transNum)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}
/* Code for state "loading_board2" in state set "ss1" */

/* Entry function for state "loading_board2" in state set "ss1" */
static void I_ss1_loading_board2(SS_ID ssId, struct UserVar *pVar)
{
/* Entry 1: */
# line 74 "./../sncExample.stt"
	loadBoard(2);
# line 75 "./../sncExample.stt"
	v = 1;
# line 76 "./../sncExample.stt"
	seq_pvPut(ssId, 0 /* v */, 0);
}

/* Delay function for state "loading_board2" in state set "ss1" */
static void D_ss1_loading_board2(SS_ID ssId, struct UserVar *pVar)
{
# line 79 "./../sncExample.stt"
}

/* Event function for state "loading_board2" in state set "ss1" */
static long E_ss1_loading_board2(SS_ID ssId, struct UserVar *pVar, short *pTransNum, short *pNextState)
{
# line 79 "./../sncExample.stt"
	if (1)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "loading_board2" in state set "ss1" */
static void A_ss1_loading_board2(SS_ID ssId, struct UserVar *pVar, short transNum)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}
/* Code for state "loading_board3" in state set "ss1" */

/* Entry function for state "loading_board3" in state set "ss1" */
static void I_ss1_loading_board3(SS_ID ssId, struct UserVar *pVar)
{
/* Entry 1: */
# line 84 "./../sncExample.stt"
	loadBoard(3);
# line 85 "./../sncExample.stt"
	v = 1;
# line 86 "./../sncExample.stt"
	seq_pvPut(ssId, 0 /* v */, 0);
}

/* Delay function for state "loading_board3" in state set "ss1" */
static void D_ss1_loading_board3(SS_ID ssId, struct UserVar *pVar)
{
# line 89 "./../sncExample.stt"
}

/* Event function for state "loading_board3" in state set "ss1" */
static long E_ss1_loading_board3(SS_ID ssId, struct UserVar *pVar, short *pTransNum, short *pNextState)
{
# line 89 "./../sncExample.stt"
	if (1)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "loading_board3" in state set "ss1" */
static void A_ss1_loading_board3(SS_ID ssId, struct UserVar *pVar, short transNum)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/* Exit handler */
static void exit_handler(SS_ID ssId, struct UserVar *pVar)
{
}

/************************ Tables ***********************/

/* Database Blocks */
static struct seqChan seqChan[NUM_CHANNELS] = {
  {"{SYSTEM}:master_control_switch", (void *)&v, "v", 
    "double", 1, 1, 0, 1, 0, 0, 0},

};

/* Event masks for state set ss1 */
	/* Event mask for state init: */
static bitMask	EM_ss1_init[] = {
	0x00000000,
};
	/* Event mask for state low: */
static bitMask	EM_ss1_low[] = {
	0x00000002,
};
	/* Event mask for state high: */
static bitMask	EM_ss1_high[] = {
	0x00000002,
};
	/* Event mask for state waiting: */
static bitMask	EM_ss1_waiting[] = {
	0x00000002,
};
	/* Event mask for state loading_all: */
static bitMask	EM_ss1_loading_all[] = {
	0x00000000,
};
	/* Event mask for state loading_board1: */
static bitMask	EM_ss1_loading_board1[] = {
	0x00000000,
};
	/* Event mask for state loading_board2: */
static bitMask	EM_ss1_loading_board2[] = {
	0x00000000,
};
	/* Event mask for state loading_board3: */
static bitMask	EM_ss1_loading_board3[] = {
	0x00000000,
};

/* State Blocks */

static struct seqState state_ss1[] = {
	/* State "init" */ {
	/* state name */       "init",
	/* action function */ (ACTION_FUNC) A_ss1_init,
	/* event function */  (EVENT_FUNC) E_ss1_init,
	/* delay function */   (DELAY_FUNC) D_ss1_init,
	/* entry function */   (ENTRY_FUNC) I_ss1_init,
	/* exit function */   (EXIT_FUNC) 0,
	/* event mask array */ EM_ss1_init,
	/* state options */   (0)},

	/* State "low" */ {
	/* state name */       "low",
	/* action function */ (ACTION_FUNC) A_ss1_low,
	/* event function */  (EVENT_FUNC) E_ss1_low,
	/* delay function */   (DELAY_FUNC) D_ss1_low,
	/* entry function */   (ENTRY_FUNC) 0,
	/* exit function */   (EXIT_FUNC) 0,
	/* event mask array */ EM_ss1_low,
	/* state options */   (0)},

	/* State "high" */ {
	/* state name */       "high",
	/* action function */ (ACTION_FUNC) A_ss1_high,
	/* event function */  (EVENT_FUNC) E_ss1_high,
	/* delay function */   (DELAY_FUNC) D_ss1_high,
	/* entry function */   (ENTRY_FUNC) 0,
	/* exit function */   (EXIT_FUNC) 0,
	/* event mask array */ EM_ss1_high,
	/* state options */   (0)},

	/* State "waiting" */ {
	/* state name */       "waiting",
	/* action function */ (ACTION_FUNC) A_ss1_waiting,
	/* event function */  (EVENT_FUNC) E_ss1_waiting,
	/* delay function */   (DELAY_FUNC) D_ss1_waiting,
	/* entry function */   (ENTRY_FUNC) I_ss1_waiting,
	/* exit function */   (EXIT_FUNC) 0,
	/* event mask array */ EM_ss1_waiting,
	/* state options */   (0)},

	/* State "loading_all" */ {
	/* state name */       "loading_all",
	/* action function */ (ACTION_FUNC) A_ss1_loading_all,
	/* event function */  (EVENT_FUNC) E_ss1_loading_all,
	/* delay function */   (DELAY_FUNC) D_ss1_loading_all,
	/* entry function */   (ENTRY_FUNC) I_ss1_loading_all,
	/* exit function */   (EXIT_FUNC) 0,
	/* event mask array */ EM_ss1_loading_all,
	/* state options */   (0)},

	/* State "loading_board1" */ {
	/* state name */       "loading_board1",
	/* action function */ (ACTION_FUNC) A_ss1_loading_board1,
	/* event function */  (EVENT_FUNC) E_ss1_loading_board1,
	/* delay function */   (DELAY_FUNC) D_ss1_loading_board1,
	/* entry function */   (ENTRY_FUNC) I_ss1_loading_board1,
	/* exit function */   (EXIT_FUNC) 0,
	/* event mask array */ EM_ss1_loading_board1,
	/* state options */   (0)},

	/* State "loading_board2" */ {
	/* state name */       "loading_board2",
	/* action function */ (ACTION_FUNC) A_ss1_loading_board2,
	/* event function */  (EVENT_FUNC) E_ss1_loading_board2,
	/* delay function */   (DELAY_FUNC) D_ss1_loading_board2,
	/* entry function */   (ENTRY_FUNC) I_ss1_loading_board2,
	/* exit function */   (EXIT_FUNC) 0,
	/* event mask array */ EM_ss1_loading_board2,
	/* state options */   (0)},

	/* State "loading_board3" */ {
	/* state name */       "loading_board3",
	/* action function */ (ACTION_FUNC) A_ss1_loading_board3,
	/* event function */  (EVENT_FUNC) E_ss1_loading_board3,
	/* delay function */   (DELAY_FUNC) D_ss1_loading_board3,
	/* entry function */   (ENTRY_FUNC) I_ss1_loading_board3,
	/* exit function */   (EXIT_FUNC) 0,
	/* event mask array */ EM_ss1_loading_board3,
	/* state options */   (0)},


};

/* State Set Blocks */
static struct seqSS seqSS[NUM_SS] = {
	/* State set "ss1" */ {
	/* ss name */            "ss1",
	/* ptr to state block */ state_ss1,
	/* number of states */   8,
	/* error state */        -1},
};

/* Program parameter list */
static char prog_param[] = "";

/* State Program table (global) */
struct seqProgram sncExample = {
	/* magic number */       20000315,
	/* *name */              "sncExample",
	/* *pChannels */         seqChan,
	/* numChans */           NUM_CHANNELS,
	/* *pSS */               seqSS,
	/* numSS */              NUM_SS,
	/* user variable size */ 0,
	/* *pParams */           prog_param,
	/* numEvents */          NUM_EVENTS,
	/* encoded options */    (0 | OPT_CONN | OPT_NEWEF | OPT_MAIN),
	/* entry handler */      (ENTRY_FUNC) entry_handler,
	/* exit handler */       (EXIT_FUNC) exit_handler,
	/* numQueues */          NUM_QUEUES,
};
#include "epicsExport.h"


/* Register sequencer commands and program */

void sncExampleRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&sncExample);
}
epicsExportRegistrar(sncExampleRegistrar);

