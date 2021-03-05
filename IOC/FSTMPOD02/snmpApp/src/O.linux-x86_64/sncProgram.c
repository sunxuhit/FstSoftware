/* Generated with snc from sncProgram.i */

/* Program "sncExample" */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include "epicsTypes.h"
#include "seqCom.h"

/* Variable declarations */
# line 2 "./../sncExample.stt"
static	double v = 0;

/* C code definitions */
# line 5 "./../sncExample.stt"

void loadBoard(int i) {
    printf("in the loadBoard function\n");
}


/* Program init func */
static void G_prog_init(struct UserVar *pVar)
{
}

/****** Code for state "init" in state set "ss1" ******/

/* Entry function for state "init" in state set "ss1" */
static void I_ss1_0_init(SS_ID ssId, struct UserVar *pVar)
{
# line 14 "./../sncExample.stt"
	v = 0;
# line 15 "./../sncExample.stt"
	seq_pvPut(ssId, 0/*v*/, 0);
# line 16 "./../sncExample.stt"
	printf("sncExample: initializing\n");
}

/* Delay function for state "init" in state set "ss1" */
static void D_ss1_0_init(SS_ID ssId, struct UserVar *pVar)
{
# line 18 "./../sncExample.stt"
	seq_delayInit(ssId, 0, (10));
}

/* Event function for state "init" in state set "ss1" */
static seqBool E_ss1_0_init(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 18 "./../sncExample.stt"
	if (seq_delay(ssId, 0))
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "init" in state set "ss1" */
static void A_ss1_0_init(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
# line 19 "./../sncExample.stt"
			printf("sncExample: Startup delay over\n");
# line 20 "./../sncExample.stt"
			v = 1;
# line 21 "./../sncExample.stt"
			seq_pvPut(ssId, 0/*v*/, 0);
		}
		return;
	}
}

/****** Code for state "low" in state set "ss1" ******/

/* Delay function for state "low" in state set "ss1" */
static void D_ss1_0_low(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "low" in state set "ss1" */
static seqBool E_ss1_0_low(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 25 "./../sncExample.stt"
	if (v > 5.0)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "low" in state set "ss1" */
static void A_ss1_0_low(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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

/****** Code for state "high" in state set "ss1" ******/

/* Delay function for state "high" in state set "ss1" */
static void D_ss1_0_high(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "high" in state set "ss1" */
static seqBool E_ss1_0_high(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 30 "./../sncExample.stt"
	if (v <= 5.0)
	{
		*pNextState = 1;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "high" in state set "ss1" */
static void A_ss1_0_high(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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

/****** Code for state "waiting" in state set "ss1" ******/

/* Entry function for state "waiting" in state set "ss1" */
static void I_ss1_0_waiting(SS_ID ssId, struct UserVar *pVar)
{
# line 36 "./../sncExample.stt"
	printf("waiting\n");
}

/* Delay function for state "waiting" in state set "ss1" */
static void D_ss1_0_waiting(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "waiting" in state set "ss1" */
static seqBool E_ss1_0_waiting(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 38 "./../sncExample.stt"
	if (v == 6)
	{
		*pNextState = 4;
		*pTransNum = 0;
		return TRUE;
	}
# line 41 "./../sncExample.stt"
	if (v == 7)
	{
		*pNextState = 5;
		*pTransNum = 1;
		return TRUE;
	}
# line 44 "./../sncExample.stt"
	if (v == 8)
	{
		*pNextState = 6;
		*pTransNum = 2;
		return TRUE;
	}
# line 47 "./../sncExample.stt"
	if (v == 9)
	{
		*pNextState = 7;
		*pTransNum = 3;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "waiting" in state set "ss1" */
static void A_ss1_0_waiting(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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

/****** Code for state "loading_all" in state set "ss1" ******/

/* Entry function for state "loading_all" in state set "ss1" */
static void I_ss1_0_loading_all(SS_ID ssId, struct UserVar *pVar)
{
# line 53 "./../sncExample.stt"
	loadBoard(1);
# line 54 "./../sncExample.stt"
	loadBoard(2);
# line 55 "./../sncExample.stt"
	loadBoard(3);
# line 56 "./../sncExample.stt"
	v = 1;
# line 57 "./../sncExample.stt"
	seq_pvPut(ssId, 0/*v*/, 0);
}

/* Delay function for state "loading_all" in state set "ss1" */
static void D_ss1_0_loading_all(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "loading_all" in state set "ss1" */
static seqBool E_ss1_0_loading_all(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 59 "./../sncExample.stt"
	if (1)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "loading_all" in state set "ss1" */
static void A_ss1_0_loading_all(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "loading_board1" in state set "ss1" ******/

/* Entry function for state "loading_board1" in state set "ss1" */
static void I_ss1_0_loading_board1(SS_ID ssId, struct UserVar *pVar)
{
# line 64 "./../sncExample.stt"
	loadBoard(1);
# line 65 "./../sncExample.stt"
	v = 1;
# line 66 "./../sncExample.stt"
	seq_pvPut(ssId, 0/*v*/, 0);
}

/* Delay function for state "loading_board1" in state set "ss1" */
static void D_ss1_0_loading_board1(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "loading_board1" in state set "ss1" */
static seqBool E_ss1_0_loading_board1(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 68 "./../sncExample.stt"
	if (1)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "loading_board1" in state set "ss1" */
static void A_ss1_0_loading_board1(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "loading_board2" in state set "ss1" ******/

/* Entry function for state "loading_board2" in state set "ss1" */
static void I_ss1_0_loading_board2(SS_ID ssId, struct UserVar *pVar)
{
# line 74 "./../sncExample.stt"
	loadBoard(2);
# line 75 "./../sncExample.stt"
	v = 1;
# line 76 "./../sncExample.stt"
	seq_pvPut(ssId, 0/*v*/, 0);
}

/* Delay function for state "loading_board2" in state set "ss1" */
static void D_ss1_0_loading_board2(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "loading_board2" in state set "ss1" */
static seqBool E_ss1_0_loading_board2(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 78 "./../sncExample.stt"
	if (1)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "loading_board2" in state set "ss1" */
static void A_ss1_0_loading_board2(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "loading_board3" in state set "ss1" ******/

/* Entry function for state "loading_board3" in state set "ss1" */
static void I_ss1_0_loading_board3(SS_ID ssId, struct UserVar *pVar)
{
# line 84 "./../sncExample.stt"
	loadBoard(3);
# line 85 "./../sncExample.stt"
	v = 1;
# line 86 "./../sncExample.stt"
	seq_pvPut(ssId, 0/*v*/, 0);
}

/* Delay function for state "loading_board3" in state set "ss1" */
static void D_ss1_0_loading_board3(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "loading_board3" in state set "ss1" */
static seqBool E_ss1_0_loading_board3(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 88 "./../sncExample.stt"
	if (1)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "loading_board3" in state set "ss1" */
static void A_ss1_0_loading_board3(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/************************ Tables ************************/

/* Channel table */
static seqChan G_channels[] = {
	/* chName, offset, varName, varType, count, eventNum, efId, monitored, queueSize, queueIndex */
	{"{SYSTEM}:master_control_switch", (size_t)&v, "v", "double", 1, 1, 0, 1, 0, 0}
};

/* Event masks for state set "ss1" */
static const seqMask	EM_ss1_0_init[] = {
	0x00000000,
};
static const seqMask	EM_ss1_0_low[] = {
	0x00000002,
};
static const seqMask	EM_ss1_0_high[] = {
	0x00000002,
};
static const seqMask	EM_ss1_0_waiting[] = {
	0x00000002,
};
static const seqMask	EM_ss1_0_loading_all[] = {
	0x00000000,
};
static const seqMask	EM_ss1_0_loading_board1[] = {
	0x00000000,
};
static const seqMask	EM_ss1_0_loading_board2[] = {
	0x00000000,
};
static const seqMask	EM_ss1_0_loading_board3[] = {
	0x00000000,
};

/* State table for state set "ss1" */
static seqState G_ss1_states[] = {
	{
	/* state name */        "init",
	/* action function */   A_ss1_0_init,
	/* event function */    E_ss1_0_init,
	/* delay function */    D_ss1_0_init,
	/* entry function */    I_ss1_0_init,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_init,
	/* state options */     (0)
	},
	{
	/* state name */        "low",
	/* action function */   A_ss1_0_low,
	/* event function */    E_ss1_0_low,
	/* delay function */    D_ss1_0_low,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_low,
	/* state options */     (0)
	},
	{
	/* state name */        "high",
	/* action function */   A_ss1_0_high,
	/* event function */    E_ss1_0_high,
	/* delay function */    D_ss1_0_high,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_high,
	/* state options */     (0)
	},
	{
	/* state name */        "waiting",
	/* action function */   A_ss1_0_waiting,
	/* event function */    E_ss1_0_waiting,
	/* delay function */    D_ss1_0_waiting,
	/* entry function */    I_ss1_0_waiting,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_waiting,
	/* state options */     (0)
	},
	{
	/* state name */        "loading_all",
	/* action function */   A_ss1_0_loading_all,
	/* event function */    E_ss1_0_loading_all,
	/* delay function */    D_ss1_0_loading_all,
	/* entry function */    I_ss1_0_loading_all,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_loading_all,
	/* state options */     (0)
	},
	{
	/* state name */        "loading_board1",
	/* action function */   A_ss1_0_loading_board1,
	/* event function */    E_ss1_0_loading_board1,
	/* delay function */    D_ss1_0_loading_board1,
	/* entry function */    I_ss1_0_loading_board1,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_loading_board1,
	/* state options */     (0)
	},
	{
	/* state name */        "loading_board2",
	/* action function */   A_ss1_0_loading_board2,
	/* event function */    E_ss1_0_loading_board2,
	/* delay function */    D_ss1_0_loading_board2,
	/* entry function */    I_ss1_0_loading_board2,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_loading_board2,
	/* state options */     (0)
	},
	{
	/* state name */        "loading_board3",
	/* action function */   A_ss1_0_loading_board3,
	/* event function */    E_ss1_0_loading_board3,
	/* delay function */    D_ss1_0_loading_board3,
	/* entry function */    I_ss1_0_loading_board3,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_loading_board3,
	/* state options */     (0)
	},
};

/* State set table */
static seqSS G_state_sets[] = {
	{
	/* state set name */    "ss1",
	/* states */            G_ss1_states,
	/* number of states */  8,
	/* number of delays */  1
	},
};

/* Program table (global) */
seqProgram sncExample = {
	/* magic number */      2001013,
	/* program name */      "sncExample",
	/* channels */          G_channels,
	/* num. channels */     1,
	/* state sets */        G_state_sets,
	/* num. state sets */   1,
	/* user var size */     0,
	/* param */             "",
	/* num. event flags */  0,
	/* encoded options */   (0 | OPT_CONN | OPT_NEWEF | OPT_MAIN),
	/* init func */         G_prog_init,
	/* entry func */        NULL,
	/* exit func */         NULL,
	/* num. queues */       0
};

#define PROG_NAME sncExample
#include "seqMain.c"

/* Register sequencer commands and program */
#include "epicsExport.h"
static void sncExampleRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&sncExample);
}
epicsExportRegistrar(sncExampleRegistrar);
