/* Generated with snc from ../sncFGTControl.stt */

/* Program "sncFGTControl" */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include "epicsTypes.h"
#include "seqCom.h"

/* Variable declarations */
struct UserVar {
# line 5 "../sncFGTControl.stt"
	int chan;
# line 6 "../sncFGTControl.stt"
	int board;
# line 7 "../sncFGTControl.stt"
	int channelref;
# line 8 "../sncFGTControl.stt"
	int v;
# line 9 "../sncFGTControl.stt"
	int i;
# line 10 "../sncFGTControl.stt"
	int j;
# line 11 "../sncFGTControl.stt"
	int doneRampingToStandby;
# line 12 "../sncFGTControl.stt"
	int endRamp;
# line 13 "../sncFGTControl.stt"
	int control;
# line 15 "../sncFGTControl.stt"
	char buff[1025];
# line 16 "../sncFGTControl.stt"
	char *next;
# line 18 "../sncFGTControl.stt"
	int m;
# line 19 "../sncFGTControl.stt"
	int ii;
# line 27 "../sncFGTControl.stt"
	string hv_filename;
# line 31 "../sncFGTControl.stt"
	double targetVoltage[32];
# line 32 "../sncFGTControl.stt"
	double physicsVoltageSetting[32];
# line 33 "../sncFGTControl.stt"
	double standbyVoltageSetting[32];
# line 34 "../sncFGTControl.stt"
	double physicsRampRate[32];
# line 35 "../sncFGTControl.stt"
	double standbyRampRate[32];
# line 36 "../sncFGTControl.stt"
	double conditioningDelay[32];
# line 37 "../sncFGTControl.stt"
	double conditioningVoltageStep[32];
# line 38 "../sncFGTControl.stt"
	double tripStatus[32];
# line 40 "../sncFGTControl.stt"
	double voltageSetting[32];
# line 41 "../sncFGTControl.stt"
	double voltageReading[32];
# line 42 "../sncFGTControl.stt"
	double powerSwitch[32];
# line 43 "../sncFGTControl.stt"
	double channelEnabled[32];
# line 44 "../sncFGTControl.stt"
	double currentSetting[32];
# line 45 "../sncFGTControl.stt"
	double rampRate[32];
# line 46 "../sncFGTControl.stt"
	double autoResetDelay[32];
# line 47 "../sncFGTControl.stt"
	double autoResetEnabled[32];
# line 48 "../sncFGTControl.stt"
	double maxCycles[332];
};

/* C code definitions */
# line 435 "../sncFGTControl.stt"


#include <stdio.h>
#include <string.h>
char boardname[3];
char channelName[256];

FILE *fp;



/* Program init func */
static void G_prog_init(struct UserVar *pVar)
{
	static struct UserVar pVarInit = {
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		0,
		0,
		0,
		"",
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};
	*pVar = pVarInit;
}

/****** Code for state "init" in state set "ss1" ******/

/* Entry function for state "init" in state set "ss1" */
static void I_ss1_0_init(SS_ID ssId, struct UserVar *pVar)
{
# line 450 "../sncFGTControl.stt"
	pVar->v = 1;
# line 451 "../sncFGTControl.stt"
	seq_pvPut(ssId, 1/*v*/, 0);
}

/* Delay function for state "init" in state set "ss1" */
static void D_ss1_0_init(SS_ID ssId, struct UserVar *pVar)
{
# line 454 "../sncFGTControl.stt"
	seq_delayInit(ssId, 0, (5));
}

/* Event function for state "init" in state set "ss1" */
static seqBool E_ss1_0_init(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 454 "../sncFGTControl.stt"
	if (seq_delay(ssId, 0))
	{
		*pNextState = 1;
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
# line 455 "../sncFGTControl.stt"
			printf("sncFGTControl: Startup delay over\n");
# line 456 "../sncFGTControl.stt"
			pVar->v = 1;
# line 457 "../sncFGTControl.stt"
			seq_pvPut(ssId, 1/*v*/, 0);
# line 458 "../sncFGTControl.stt"
			for (pVar->board = 0; pVar->board < 1; pVar->board++)
			{
# line 459 "../sncFGTControl.stt"
				if (pVar->board == 0)
				{
# line 460 "../sncFGTControl.stt"
					boardname[0] = 0;
				}
				else
# line 461 "../sncFGTControl.stt"
					if (pVar->board == 1)
					{
# line 462 "../sncFGTControl.stt"
						strcpy(boardname, "10");
					}
					else
# line 463 "../sncFGTControl.stt"
						if (pVar->board == 2)
						{
# line 464 "../sncFGTControl.stt"
							strcpy(boardname, "20");
						}
# line 467 "../sncFGTControl.stt"
				for (pVar->channelref = 0; pVar->channelref < 24; pVar->channelref++)
				{
# line 468 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 195/*voltageSetting*/ + (VAR_ID)(pVar->channelref));
# line 469 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 719/*voltageReading*/ + (VAR_ID)(pVar->channelref));
# line 470 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 751/*channelEnabled*/ + (VAR_ID)(pVar->channelref));
# line 471 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 687/*powerSwitch*/ + (VAR_ID)(pVar->channelref));
# line 472 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 227/*currentSetting*/ + (VAR_ID)(pVar->channelref));
# line 473 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 259/*rampRate*/ + (VAR_ID)(pVar->channelref));
# line 474 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 291/*autoResetDelay*/ + (VAR_ID)(pVar->channelref));
# line 475 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 323/*autoResetEnabled*/ + (VAR_ID)(pVar->channelref));
# line 476 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 355/*maxCycles*/ + (VAR_ID)(pVar->channelref));
# line 477 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 783/*tripStatus*/ + (VAR_ID)(pVar->channelref));
# line 478 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 3/*physicsVoltageSetting*/ + (VAR_ID)(pVar->channelref));
# line 479 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 35/*standbyVoltageSetting*/ + (VAR_ID)(pVar->channelref));
# line 480 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 67/*physicsRampRate*/ + (VAR_ID)(pVar->channelref));
# line 481 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 99/*standbyRampRate*/ + (VAR_ID)(pVar->channelref));
# line 482 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 131/*conditioningDelay*/ + (VAR_ID)(pVar->channelref));
# line 483 "../sncFGTControl.stt"
					seq_pvMonitor(ssId, 163/*conditioningVoltageStep*/ + (VAR_ID)(pVar->channelref));
				}
			}
		}
		return;
	}
}

/****** Code for state "pause_then_load" in state set "ss1" ******/

/* Delay function for state "pause_then_load" in state set "ss1" */
static void D_ss1_0_pause_then_load(SS_ID ssId, struct UserVar *pVar)
{
# line 489 "../sncFGTControl.stt"
	seq_delayInit(ssId, 0, (5));
}

/* Event function for state "pause_then_load" in state set "ss1" */
static seqBool E_ss1_0_pause_then_load(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 489 "../sncFGTControl.stt"
	if (seq_delay(ssId, 0))
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "pause_then_load" in state set "ss1" */
static void A_ss1_0_pause_then_load(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "waiting" in state set "ss1" ******/

/* Entry function for state "waiting" in state set "ss1" */
static void I_ss1_0_waiting(SS_ID ssId, struct UserVar *pVar)
{
}

/* Delay function for state "waiting" in state set "ss1" */
static void D_ss1_0_waiting(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "waiting" in state set "ss1" */
static seqBool E_ss1_0_waiting(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 496 "../sncFGTControl.stt"
	if (pVar->v == 3)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
# line 499 "../sncFGTControl.stt"
	if (pVar->v == 4)
	{
		*pNextState = 4;
		*pTransNum = 1;
		return TRUE;
	}
# line 502 "../sncFGTControl.stt"
	if (pVar->v == 5)
	{
		*pNextState = 5;
		*pTransNum = 2;
		return TRUE;
	}
# line 505 "../sncFGTControl.stt"
	if (pVar->v == 6)
	{
		*pNextState = 6;
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
		}
		return;
	case 1:
		{
		}
		return;
	case 2:
		{
		}
		return;
	case 3:
		{
		}
		return;
	}
}

/****** Code for state "loading_all" in state set "ss1" ******/

/* Entry function for state "loading_all" in state set "ss1" */
static void I_ss1_0_loading_all(SS_ID ssId, struct UserVar *pVar)
{
# line 511 "../sncFGTControl.stt"
	seq_pvGet(ssId, 2/*hv_filename*/, 0);
# line 514 "../sncFGTControl.stt"
	sprintf(pVar->buff, "%s.csv", pVar->hv_filename);
# line 516 "../sncFGTControl.stt"
	fp = fopen(pVar->buff, "r");
# line 517 "../sncFGTControl.stt"
	if (fp)
	{
# line 518 "../sncFGTControl.stt"
		pVar->m = 0;
# line 520 "../sncFGTControl.stt"
		for (pVar->ii = 0; pVar->ii < 2; pVar->ii++)
		{
# line 520 "../sncFGTControl.stt"
			fgets(pVar->buff, 1024, fp);
		}
# line 522 "../sncFGTControl.stt"
		while (NULL != fgets(pVar->buff, 1024, fp) && pVar->m < 24)
		{
# line 524 "../sncFGTControl.stt"
			pVar->next = pVar->buff;
# line 525 "../sncFGTControl.stt"
			while (pVar->next[0] != ',')
# line 525 "../sncFGTControl.stt"
				pVar->next++;
# line 537 "../sncFGTControl.stt"
			sscanf(pVar->next + 1, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &pVar->standbyVoltageSetting[pVar->m], &pVar->physicsVoltageSetting[pVar->m], &pVar->currentSetting[pVar->m], &pVar->standbyRampRate[pVar->m], &pVar->physicsRampRate[pVar->m], &pVar->conditioningDelay[pVar->m], &pVar->conditioningVoltageStep[pVar->m], &pVar->maxCycles[pVar->m], &pVar->autoResetDelay[pVar->m], &pVar->autoResetEnabled[pVar->m], &pVar->channelEnabled[pVar->m]);
# line 543 "../sncFGTControl.stt"
			pVar->m++;
		}
# line 546 "../sncFGTControl.stt"
		fclose(fp);
# line 547 "../sncFGTControl.stt"
		for (pVar->i = 0; pVar->i < 24; pVar->i++)
		{
# line 548 "../sncFGTControl.stt"
			seq_pvPut(ssId, 195/*voltageSetting*/ + (VAR_ID)(pVar->i), SYNC);
# line 549 "../sncFGTControl.stt"
			seq_pvPut(ssId, 227/*currentSetting*/ + (VAR_ID)(pVar->i), SYNC);
# line 550 "../sncFGTControl.stt"
			seq_pvPut(ssId, 259/*rampRate*/ + (VAR_ID)(pVar->i), SYNC);
# line 551 "../sncFGTControl.stt"
			seq_pvPut(ssId, 355/*maxCycles*/ + (VAR_ID)(pVar->i), SYNC);
# line 552 "../sncFGTControl.stt"
			seq_pvPut(ssId, 291/*autoResetDelay*/ + (VAR_ID)(pVar->i), SYNC);
# line 553 "../sncFGTControl.stt"
			seq_pvPut(ssId, 323/*autoResetEnabled*/ + (VAR_ID)(pVar->i), SYNC);
# line 554 "../sncFGTControl.stt"
			seq_pvPut(ssId, 751/*channelEnabled*/ + (VAR_ID)(pVar->i), SYNC);
# line 555 "../sncFGTControl.stt"
			seq_pvPut(ssId, 3/*physicsVoltageSetting*/ + (VAR_ID)(pVar->i), SYNC);
# line 556 "../sncFGTControl.stt"
			seq_pvPut(ssId, 35/*standbyVoltageSetting*/ + (VAR_ID)(pVar->i), SYNC);
# line 557 "../sncFGTControl.stt"
			seq_pvPut(ssId, 67/*physicsRampRate*/ + (VAR_ID)(pVar->i), SYNC);
# line 558 "../sncFGTControl.stt"
			seq_pvPut(ssId, 99/*standbyRampRate*/ + (VAR_ID)(pVar->i), SYNC);
# line 559 "../sncFGTControl.stt"
			seq_pvPut(ssId, 131/*conditioningDelay*/ + (VAR_ID)(pVar->i), SYNC);
# line 560 "../sncFGTControl.stt"
			seq_pvPut(ssId, 163/*conditioningVoltageStep*/ + (VAR_ID)(pVar->i), SYNC);
		}
	}
}

/* Delay function for state "loading_all" in state set "ss1" */
static void D_ss1_0_loading_all(SS_ID ssId, struct UserVar *pVar)
{
# line 567 "../sncFGTControl.stt"
	seq_delayInit(ssId, 0, (1));
}

/* Event function for state "loading_all" in state set "ss1" */
static seqBool E_ss1_0_loading_all(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 567 "../sncFGTControl.stt"
	if (seq_delay(ssId, 0))
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
# line 571 "../sncFGTControl.stt"
	if (pVar->v == 1)
	{
		*pNextState = 2;
		*pTransNum = 1;
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
# line 568 "../sncFGTControl.stt"
			pVar->v = 1;
# line 569 "../sncFGTControl.stt"
			seq_pvPut(ssId, 1/*v*/, SYNC);
		}
		return;
	case 1:
		{
		}
		return;
	}
}

/****** Code for state "saving_all" in state set "ss1" ******/

/* Entry function for state "saving_all" in state set "ss1" */
static void I_ss1_0_saving_all(SS_ID ssId, struct UserVar *pVar)
{
# line 577 "../sncFGTControl.stt"
	seq_pvGet(ssId, 2/*hv_filename*/, 0);
# line 580 "../sncFGTControl.stt"
	sprintf(pVar->buff, "%s.csv", pVar->hv_filename);
# line 582 "../sncFGTControl.stt"
	fp = fopen(pVar->buff, "w");
# line 583 "../sncFGTControl.stt"
	pVar->i = 0;
# line 585 "../sncFGTControl.stt"
	fprintf(fp, "{SYSTEM} HV Settings\n");
# line 586 "../sncFGTControl.stt"
	fprintf(fp, "Channel(ignored), standbyV, physicsV, current limit, standbyRamp, physicsRamp, delay(s), Vstep, maxCycles, resetDelay, autoResetEnabled (0=no), channelEnabled(0=no)\n");
# line 588 "../sncFGTControl.stt"
	for (pVar->board = 0; pVar->board < 3; pVar->board++)
	{
# line 589 "../sncFGTControl.stt"
		if (pVar->board == 0)
		{
# line 590 "../sncFGTControl.stt"
			boardname[0] = 0;
		}
		else
# line 591 "../sncFGTControl.stt"
			if (pVar->board == 1)
			{
# line 592 "../sncFGTControl.stt"
				strcpy(boardname, "10");
			}
			else
# line 593 "../sncFGTControl.stt"
				if (pVar->board == 2)
				{
# line 594 "../sncFGTControl.stt"
					strcpy(boardname, "20");
				}
# line 597 "../sncFGTControl.stt"
		for (pVar->chan = 0; pVar->chan < 8; pVar->chan++)
		{
# line 598 "../sncFGTControl.stt"
			sprintf(channelName, "u%s%c", boardname, '0' + pVar->chan);
# line 602 "../sncFGTControl.stt"
			fprintf(fp, "%s, %f, %f, %f, %f, %f, ", channelName, pVar->standbyVoltageSetting[pVar->i], pVar->physicsVoltageSetting[pVar->i], pVar->currentSetting[pVar->i], pVar->standbyRampRate[pVar->i], pVar->physicsRampRate[pVar->i]);
# line 604 "../sncFGTControl.stt"
			fprintf(fp, "%f, %f, %f, %f, %f, %f\n", pVar->conditioningDelay[pVar->i], pVar->conditioningVoltageStep[pVar->i], pVar->maxCycles[pVar->i], pVar->autoResetDelay[pVar->i], pVar->autoResetEnabled[pVar->i], pVar->channelEnabled[pVar->i]);
# line 606 "../sncFGTControl.stt"
			pVar->i++;
		}
	}
# line 610 "../sncFGTControl.stt"
	fclose(fp);
# line 611 "../sncFGTControl.stt"
	pVar->v = 1;
# line 612 "../sncFGTControl.stt"
	seq_pvPut(ssId, 1/*v*/, 0);
}

/* Delay function for state "saving_all" in state set "ss1" */
static void D_ss1_0_saving_all(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "saving_all" in state set "ss1" */
static seqBool E_ss1_0_saving_all(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 614 "../sncFGTControl.stt"
	if (1)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "saving_all" in state set "ss1" */
static void A_ss1_0_saving_all(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "powering_all" in state set "ss1" ******/

/* Entry function for state "powering_all" in state set "ss1" */
static void I_ss1_0_powering_all(SS_ID ssId, struct UserVar *pVar)
{
# line 619 "../sncFGTControl.stt"
	for (pVar->i = 0; pVar->i < 24; pVar->i++)
	{
# line 620 "../sncFGTControl.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->powerSwitch[pVar->i] == 0 && pVar->tripStatus[pVar->i] != 4)
		{
# line 621 "../sncFGTControl.stt"
			pVar->powerSwitch[pVar->i] = 1;
# line 622 "../sncFGTControl.stt"
			seq_pvPut(ssId, 687/*powerSwitch*/ + (VAR_ID)(pVar->i), 0);
		}
	}
# line 626 "../sncFGTControl.stt"
	pVar->v = 1;
# line 627 "../sncFGTControl.stt"
	seq_pvPut(ssId, 1/*v*/, 0);
}

/* Delay function for state "powering_all" in state set "ss1" */
static void D_ss1_0_powering_all(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "powering_all" in state set "ss1" */
static seqBool E_ss1_0_powering_all(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 629 "../sncFGTControl.stt"
	if (1)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "powering_all" in state set "ss1" */
static void A_ss1_0_powering_all(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "loading_ramp_all" in state set "ss1" ******/

/* Entry function for state "loading_ramp_all" in state set "ss1" */
static void I_ss1_0_loading_ramp_all(SS_ID ssId, struct UserVar *pVar)
{
# line 634 "../sncFGTControl.stt"
	for (pVar->i = 0; pVar->i < 24; pVar->i++)
	{
# line 635 "../sncFGTControl.stt"
		pVar->currentSetting[pVar->i] = 0.000015;
# line 636 "../sncFGTControl.stt"
		seq_pvPut(ssId, 227/*currentSetting*/ + (VAR_ID)(pVar->i), SYNC);
	}
# line 638 "../sncFGTControl.stt"
	pVar->v = 1;
# line 639 "../sncFGTControl.stt"
	seq_pvPut(ssId, 1/*v*/, SYNC);
}

/* Delay function for state "loading_ramp_all" in state set "ss1" */
static void D_ss1_0_loading_ramp_all(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "loading_ramp_all" in state set "ss1" */
static seqBool E_ss1_0_loading_ramp_all(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 641 "../sncFGTControl.stt"
	if (1)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "loading_ramp_all" in state set "ss1" */
static void A_ss1_0_loading_ramp_all(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	{"{SYSTEM}:ramp_control_switch", offsetof(struct UserVar, control), "control", "int", 1, 1, 0, 1, 0, 0},
	{"{SYSTEM}:master_control_switch", offsetof(struct UserVar, v), "v", "int", 1, 2, 0, 1, 0, 0},
	{"{SYSTEM}:hv_settings", offsetof(struct UserVar, hv_filename), "hv_filename", "string", 1, 3, 0, 1, 0, 0},
	{"{SYSTEM}:physics_voltage_u0", offsetof(struct UserVar, physicsVoltageSetting[0]), "physicsVoltageSetting[0]", "double", 1, 4, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u1", offsetof(struct UserVar, physicsVoltageSetting[1]), "physicsVoltageSetting[1]", "double", 1, 5, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u2", offsetof(struct UserVar, physicsVoltageSetting[2]), "physicsVoltageSetting[2]", "double", 1, 6, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u3", offsetof(struct UserVar, physicsVoltageSetting[3]), "physicsVoltageSetting[3]", "double", 1, 7, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u4", offsetof(struct UserVar, physicsVoltageSetting[4]), "physicsVoltageSetting[4]", "double", 1, 8, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u5", offsetof(struct UserVar, physicsVoltageSetting[5]), "physicsVoltageSetting[5]", "double", 1, 9, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u6", offsetof(struct UserVar, physicsVoltageSetting[6]), "physicsVoltageSetting[6]", "double", 1, 10, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u7", offsetof(struct UserVar, physicsVoltageSetting[7]), "physicsVoltageSetting[7]", "double", 1, 11, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u100", offsetof(struct UserVar, physicsVoltageSetting[8]), "physicsVoltageSetting[8]", "double", 1, 12, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u101", offsetof(struct UserVar, physicsVoltageSetting[9]), "physicsVoltageSetting[9]", "double", 1, 13, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u102", offsetof(struct UserVar, physicsVoltageSetting[10]), "physicsVoltageSetting[10]", "double", 1, 14, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u103", offsetof(struct UserVar, physicsVoltageSetting[11]), "physicsVoltageSetting[11]", "double", 1, 15, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u104", offsetof(struct UserVar, physicsVoltageSetting[12]), "physicsVoltageSetting[12]", "double", 1, 16, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u105", offsetof(struct UserVar, physicsVoltageSetting[13]), "physicsVoltageSetting[13]", "double", 1, 17, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u106", offsetof(struct UserVar, physicsVoltageSetting[14]), "physicsVoltageSetting[14]", "double", 1, 18, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u107", offsetof(struct UserVar, physicsVoltageSetting[15]), "physicsVoltageSetting[15]", "double", 1, 19, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u200", offsetof(struct UserVar, physicsVoltageSetting[16]), "physicsVoltageSetting[16]", "double", 1, 20, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u201", offsetof(struct UserVar, physicsVoltageSetting[17]), "physicsVoltageSetting[17]", "double", 1, 21, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u202", offsetof(struct UserVar, physicsVoltageSetting[18]), "physicsVoltageSetting[18]", "double", 1, 22, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u203", offsetof(struct UserVar, physicsVoltageSetting[19]), "physicsVoltageSetting[19]", "double", 1, 23, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u204", offsetof(struct UserVar, physicsVoltageSetting[20]), "physicsVoltageSetting[20]", "double", 1, 24, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u205", offsetof(struct UserVar, physicsVoltageSetting[21]), "physicsVoltageSetting[21]", "double", 1, 25, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u206", offsetof(struct UserVar, physicsVoltageSetting[22]), "physicsVoltageSetting[22]", "double", 1, 26, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_u207", offsetof(struct UserVar, physicsVoltageSetting[23]), "physicsVoltageSetting[23]", "double", 1, 27, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[24]), "physicsVoltageSetting[24]", "double", 1, 28, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[25]), "physicsVoltageSetting[25]", "double", 1, 29, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[26]), "physicsVoltageSetting[26]", "double", 1, 30, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[27]), "physicsVoltageSetting[27]", "double", 1, 31, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[28]), "physicsVoltageSetting[28]", "double", 1, 32, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[29]), "physicsVoltageSetting[29]", "double", 1, 33, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[30]), "physicsVoltageSetting[30]", "double", 1, 34, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[31]), "physicsVoltageSetting[31]", "double", 1, 35, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u0", offsetof(struct UserVar, standbyVoltageSetting[0]), "standbyVoltageSetting[0]", "double", 1, 36, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u1", offsetof(struct UserVar, standbyVoltageSetting[1]), "standbyVoltageSetting[1]", "double", 1, 37, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u2", offsetof(struct UserVar, standbyVoltageSetting[2]), "standbyVoltageSetting[2]", "double", 1, 38, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u3", offsetof(struct UserVar, standbyVoltageSetting[3]), "standbyVoltageSetting[3]", "double", 1, 39, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u4", offsetof(struct UserVar, standbyVoltageSetting[4]), "standbyVoltageSetting[4]", "double", 1, 40, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u5", offsetof(struct UserVar, standbyVoltageSetting[5]), "standbyVoltageSetting[5]", "double", 1, 41, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u6", offsetof(struct UserVar, standbyVoltageSetting[6]), "standbyVoltageSetting[6]", "double", 1, 42, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u7", offsetof(struct UserVar, standbyVoltageSetting[7]), "standbyVoltageSetting[7]", "double", 1, 43, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u100", offsetof(struct UserVar, standbyVoltageSetting[8]), "standbyVoltageSetting[8]", "double", 1, 44, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u101", offsetof(struct UserVar, standbyVoltageSetting[9]), "standbyVoltageSetting[9]", "double", 1, 45, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u102", offsetof(struct UserVar, standbyVoltageSetting[10]), "standbyVoltageSetting[10]", "double", 1, 46, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u103", offsetof(struct UserVar, standbyVoltageSetting[11]), "standbyVoltageSetting[11]", "double", 1, 47, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u104", offsetof(struct UserVar, standbyVoltageSetting[12]), "standbyVoltageSetting[12]", "double", 1, 48, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u105", offsetof(struct UserVar, standbyVoltageSetting[13]), "standbyVoltageSetting[13]", "double", 1, 49, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u106", offsetof(struct UserVar, standbyVoltageSetting[14]), "standbyVoltageSetting[14]", "double", 1, 50, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u107", offsetof(struct UserVar, standbyVoltageSetting[15]), "standbyVoltageSetting[15]", "double", 1, 51, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u200", offsetof(struct UserVar, standbyVoltageSetting[16]), "standbyVoltageSetting[16]", "double", 1, 52, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u201", offsetof(struct UserVar, standbyVoltageSetting[17]), "standbyVoltageSetting[17]", "double", 1, 53, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u202", offsetof(struct UserVar, standbyVoltageSetting[18]), "standbyVoltageSetting[18]", "double", 1, 54, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u203", offsetof(struct UserVar, standbyVoltageSetting[19]), "standbyVoltageSetting[19]", "double", 1, 55, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u204", offsetof(struct UserVar, standbyVoltageSetting[20]), "standbyVoltageSetting[20]", "double", 1, 56, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u205", offsetof(struct UserVar, standbyVoltageSetting[21]), "standbyVoltageSetting[21]", "double", 1, 57, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u206", offsetof(struct UserVar, standbyVoltageSetting[22]), "standbyVoltageSetting[22]", "double", 1, 58, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_u207", offsetof(struct UserVar, standbyVoltageSetting[23]), "standbyVoltageSetting[23]", "double", 1, 59, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[24]), "standbyVoltageSetting[24]", "double", 1, 60, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[25]), "standbyVoltageSetting[25]", "double", 1, 61, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[26]), "standbyVoltageSetting[26]", "double", 1, 62, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[27]), "standbyVoltageSetting[27]", "double", 1, 63, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[28]), "standbyVoltageSetting[28]", "double", 1, 64, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[29]), "standbyVoltageSetting[29]", "double", 1, 65, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[30]), "standbyVoltageSetting[30]", "double", 1, 66, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[31]), "standbyVoltageSetting[31]", "double", 1, 67, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u0", offsetof(struct UserVar, physicsRampRate[0]), "physicsRampRate[0]", "double", 1, 68, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u1", offsetof(struct UserVar, physicsRampRate[1]), "physicsRampRate[1]", "double", 1, 69, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u2", offsetof(struct UserVar, physicsRampRate[2]), "physicsRampRate[2]", "double", 1, 70, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u3", offsetof(struct UserVar, physicsRampRate[3]), "physicsRampRate[3]", "double", 1, 71, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u4", offsetof(struct UserVar, physicsRampRate[4]), "physicsRampRate[4]", "double", 1, 72, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u5", offsetof(struct UserVar, physicsRampRate[5]), "physicsRampRate[5]", "double", 1, 73, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u6", offsetof(struct UserVar, physicsRampRate[6]), "physicsRampRate[6]", "double", 1, 74, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u7", offsetof(struct UserVar, physicsRampRate[7]), "physicsRampRate[7]", "double", 1, 75, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u100", offsetof(struct UserVar, physicsRampRate[8]), "physicsRampRate[8]", "double", 1, 76, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u101", offsetof(struct UserVar, physicsRampRate[9]), "physicsRampRate[9]", "double", 1, 77, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u102", offsetof(struct UserVar, physicsRampRate[10]), "physicsRampRate[10]", "double", 1, 78, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u103", offsetof(struct UserVar, physicsRampRate[11]), "physicsRampRate[11]", "double", 1, 79, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u104", offsetof(struct UserVar, physicsRampRate[12]), "physicsRampRate[12]", "double", 1, 80, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u105", offsetof(struct UserVar, physicsRampRate[13]), "physicsRampRate[13]", "double", 1, 81, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u106", offsetof(struct UserVar, physicsRampRate[14]), "physicsRampRate[14]", "double", 1, 82, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u107", offsetof(struct UserVar, physicsRampRate[15]), "physicsRampRate[15]", "double", 1, 83, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u200", offsetof(struct UserVar, physicsRampRate[16]), "physicsRampRate[16]", "double", 1, 84, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u201", offsetof(struct UserVar, physicsRampRate[17]), "physicsRampRate[17]", "double", 1, 85, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u202", offsetof(struct UserVar, physicsRampRate[18]), "physicsRampRate[18]", "double", 1, 86, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u203", offsetof(struct UserVar, physicsRampRate[19]), "physicsRampRate[19]", "double", 1, 87, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u204", offsetof(struct UserVar, physicsRampRate[20]), "physicsRampRate[20]", "double", 1, 88, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u205", offsetof(struct UserVar, physicsRampRate[21]), "physicsRampRate[21]", "double", 1, 89, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u206", offsetof(struct UserVar, physicsRampRate[22]), "physicsRampRate[22]", "double", 1, 90, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_u207", offsetof(struct UserVar, physicsRampRate[23]), "physicsRampRate[23]", "double", 1, 91, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[24]), "physicsRampRate[24]", "double", 1, 92, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[25]), "physicsRampRate[25]", "double", 1, 93, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[26]), "physicsRampRate[26]", "double", 1, 94, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[27]), "physicsRampRate[27]", "double", 1, 95, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[28]), "physicsRampRate[28]", "double", 1, 96, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[29]), "physicsRampRate[29]", "double", 1, 97, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[30]), "physicsRampRate[30]", "double", 1, 98, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[31]), "physicsRampRate[31]", "double", 1, 99, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u0", offsetof(struct UserVar, standbyRampRate[0]), "standbyRampRate[0]", "double", 1, 100, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u1", offsetof(struct UserVar, standbyRampRate[1]), "standbyRampRate[1]", "double", 1, 101, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u2", offsetof(struct UserVar, standbyRampRate[2]), "standbyRampRate[2]", "double", 1, 102, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u3", offsetof(struct UserVar, standbyRampRate[3]), "standbyRampRate[3]", "double", 1, 103, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u4", offsetof(struct UserVar, standbyRampRate[4]), "standbyRampRate[4]", "double", 1, 104, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u5", offsetof(struct UserVar, standbyRampRate[5]), "standbyRampRate[5]", "double", 1, 105, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u6", offsetof(struct UserVar, standbyRampRate[6]), "standbyRampRate[6]", "double", 1, 106, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u7", offsetof(struct UserVar, standbyRampRate[7]), "standbyRampRate[7]", "double", 1, 107, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u100", offsetof(struct UserVar, standbyRampRate[8]), "standbyRampRate[8]", "double", 1, 108, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u101", offsetof(struct UserVar, standbyRampRate[9]), "standbyRampRate[9]", "double", 1, 109, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u102", offsetof(struct UserVar, standbyRampRate[10]), "standbyRampRate[10]", "double", 1, 110, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u103", offsetof(struct UserVar, standbyRampRate[11]), "standbyRampRate[11]", "double", 1, 111, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u104", offsetof(struct UserVar, standbyRampRate[12]), "standbyRampRate[12]", "double", 1, 112, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u105", offsetof(struct UserVar, standbyRampRate[13]), "standbyRampRate[13]", "double", 1, 113, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u106", offsetof(struct UserVar, standbyRampRate[14]), "standbyRampRate[14]", "double", 1, 114, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u107", offsetof(struct UserVar, standbyRampRate[15]), "standbyRampRate[15]", "double", 1, 115, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u200", offsetof(struct UserVar, standbyRampRate[16]), "standbyRampRate[16]", "double", 1, 116, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u201", offsetof(struct UserVar, standbyRampRate[17]), "standbyRampRate[17]", "double", 1, 117, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u202", offsetof(struct UserVar, standbyRampRate[18]), "standbyRampRate[18]", "double", 1, 118, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u203", offsetof(struct UserVar, standbyRampRate[19]), "standbyRampRate[19]", "double", 1, 119, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u204", offsetof(struct UserVar, standbyRampRate[20]), "standbyRampRate[20]", "double", 1, 120, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u205", offsetof(struct UserVar, standbyRampRate[21]), "standbyRampRate[21]", "double", 1, 121, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u206", offsetof(struct UserVar, standbyRampRate[22]), "standbyRampRate[22]", "double", 1, 122, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_u207", offsetof(struct UserVar, standbyRampRate[23]), "standbyRampRate[23]", "double", 1, 123, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[24]), "standbyRampRate[24]", "double", 1, 124, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[25]), "standbyRampRate[25]", "double", 1, 125, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[26]), "standbyRampRate[26]", "double", 1, 126, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[27]), "standbyRampRate[27]", "double", 1, 127, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[28]), "standbyRampRate[28]", "double", 1, 128, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[29]), "standbyRampRate[29]", "double", 1, 129, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[30]), "standbyRampRate[30]", "double", 1, 130, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[31]), "standbyRampRate[31]", "double", 1, 131, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u0", offsetof(struct UserVar, conditioningDelay[0]), "conditioningDelay[0]", "double", 1, 132, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u1", offsetof(struct UserVar, conditioningDelay[1]), "conditioningDelay[1]", "double", 1, 133, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u2", offsetof(struct UserVar, conditioningDelay[2]), "conditioningDelay[2]", "double", 1, 134, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u3", offsetof(struct UserVar, conditioningDelay[3]), "conditioningDelay[3]", "double", 1, 135, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u4", offsetof(struct UserVar, conditioningDelay[4]), "conditioningDelay[4]", "double", 1, 136, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u5", offsetof(struct UserVar, conditioningDelay[5]), "conditioningDelay[5]", "double", 1, 137, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u6", offsetof(struct UserVar, conditioningDelay[6]), "conditioningDelay[6]", "double", 1, 138, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u7", offsetof(struct UserVar, conditioningDelay[7]), "conditioningDelay[7]", "double", 1, 139, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u100", offsetof(struct UserVar, conditioningDelay[8]), "conditioningDelay[8]", "double", 1, 140, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u101", offsetof(struct UserVar, conditioningDelay[9]), "conditioningDelay[9]", "double", 1, 141, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u102", offsetof(struct UserVar, conditioningDelay[10]), "conditioningDelay[10]", "double", 1, 142, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u103", offsetof(struct UserVar, conditioningDelay[11]), "conditioningDelay[11]", "double", 1, 143, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u104", offsetof(struct UserVar, conditioningDelay[12]), "conditioningDelay[12]", "double", 1, 144, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u105", offsetof(struct UserVar, conditioningDelay[13]), "conditioningDelay[13]", "double", 1, 145, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u106", offsetof(struct UserVar, conditioningDelay[14]), "conditioningDelay[14]", "double", 1, 146, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u107", offsetof(struct UserVar, conditioningDelay[15]), "conditioningDelay[15]", "double", 1, 147, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u200", offsetof(struct UserVar, conditioningDelay[16]), "conditioningDelay[16]", "double", 1, 148, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u201", offsetof(struct UserVar, conditioningDelay[17]), "conditioningDelay[17]", "double", 1, 149, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u202", offsetof(struct UserVar, conditioningDelay[18]), "conditioningDelay[18]", "double", 1, 150, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u203", offsetof(struct UserVar, conditioningDelay[19]), "conditioningDelay[19]", "double", 1, 151, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u204", offsetof(struct UserVar, conditioningDelay[20]), "conditioningDelay[20]", "double", 1, 152, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u205", offsetof(struct UserVar, conditioningDelay[21]), "conditioningDelay[21]", "double", 1, 153, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u206", offsetof(struct UserVar, conditioningDelay[22]), "conditioningDelay[22]", "double", 1, 154, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_u207", offsetof(struct UserVar, conditioningDelay[23]), "conditioningDelay[23]", "double", 1, 155, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[24]), "conditioningDelay[24]", "double", 1, 156, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[25]), "conditioningDelay[25]", "double", 1, 157, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[26]), "conditioningDelay[26]", "double", 1, 158, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[27]), "conditioningDelay[27]", "double", 1, 159, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[28]), "conditioningDelay[28]", "double", 1, 160, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[29]), "conditioningDelay[29]", "double", 1, 161, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[30]), "conditioningDelay[30]", "double", 1, 162, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[31]), "conditioningDelay[31]", "double", 1, 163, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u0", offsetof(struct UserVar, conditioningVoltageStep[0]), "conditioningVoltageStep[0]", "double", 1, 164, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u1", offsetof(struct UserVar, conditioningVoltageStep[1]), "conditioningVoltageStep[1]", "double", 1, 165, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u2", offsetof(struct UserVar, conditioningVoltageStep[2]), "conditioningVoltageStep[2]", "double", 1, 166, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u3", offsetof(struct UserVar, conditioningVoltageStep[3]), "conditioningVoltageStep[3]", "double", 1, 167, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u4", offsetof(struct UserVar, conditioningVoltageStep[4]), "conditioningVoltageStep[4]", "double", 1, 168, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u5", offsetof(struct UserVar, conditioningVoltageStep[5]), "conditioningVoltageStep[5]", "double", 1, 169, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u6", offsetof(struct UserVar, conditioningVoltageStep[6]), "conditioningVoltageStep[6]", "double", 1, 170, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u7", offsetof(struct UserVar, conditioningVoltageStep[7]), "conditioningVoltageStep[7]", "double", 1, 171, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u100", offsetof(struct UserVar, conditioningVoltageStep[8]), "conditioningVoltageStep[8]", "double", 1, 172, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u101", offsetof(struct UserVar, conditioningVoltageStep[9]), "conditioningVoltageStep[9]", "double", 1, 173, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u102", offsetof(struct UserVar, conditioningVoltageStep[10]), "conditioningVoltageStep[10]", "double", 1, 174, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u103", offsetof(struct UserVar, conditioningVoltageStep[11]), "conditioningVoltageStep[11]", "double", 1, 175, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u104", offsetof(struct UserVar, conditioningVoltageStep[12]), "conditioningVoltageStep[12]", "double", 1, 176, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u105", offsetof(struct UserVar, conditioningVoltageStep[13]), "conditioningVoltageStep[13]", "double", 1, 177, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u106", offsetof(struct UserVar, conditioningVoltageStep[14]), "conditioningVoltageStep[14]", "double", 1, 178, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u107", offsetof(struct UserVar, conditioningVoltageStep[15]), "conditioningVoltageStep[15]", "double", 1, 179, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u200", offsetof(struct UserVar, conditioningVoltageStep[16]), "conditioningVoltageStep[16]", "double", 1, 180, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u201", offsetof(struct UserVar, conditioningVoltageStep[17]), "conditioningVoltageStep[17]", "double", 1, 181, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u202", offsetof(struct UserVar, conditioningVoltageStep[18]), "conditioningVoltageStep[18]", "double", 1, 182, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u203", offsetof(struct UserVar, conditioningVoltageStep[19]), "conditioningVoltageStep[19]", "double", 1, 183, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u204", offsetof(struct UserVar, conditioningVoltageStep[20]), "conditioningVoltageStep[20]", "double", 1, 184, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u205", offsetof(struct UserVar, conditioningVoltageStep[21]), "conditioningVoltageStep[21]", "double", 1, 185, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u206", offsetof(struct UserVar, conditioningVoltageStep[22]), "conditioningVoltageStep[22]", "double", 1, 186, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_u207", offsetof(struct UserVar, conditioningVoltageStep[23]), "conditioningVoltageStep[23]", "double", 1, 187, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[24]), "conditioningVoltageStep[24]", "double", 1, 188, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[25]), "conditioningVoltageStep[25]", "double", 1, 189, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[26]), "conditioningVoltageStep[26]", "double", 1, 190, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[27]), "conditioningVoltageStep[27]", "double", 1, 191, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[28]), "conditioningVoltageStep[28]", "double", 1, 192, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[29]), "conditioningVoltageStep[29]", "double", 1, 193, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[30]), "conditioningVoltageStep[30]", "double", 1, 194, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[31]), "conditioningVoltageStep[31]", "double", 1, 195, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u0", offsetof(struct UserVar, voltageSetting[0]), "voltageSetting[0]", "double", 1, 196, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u1", offsetof(struct UserVar, voltageSetting[1]), "voltageSetting[1]", "double", 1, 197, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u2", offsetof(struct UserVar, voltageSetting[2]), "voltageSetting[2]", "double", 1, 198, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u3", offsetof(struct UserVar, voltageSetting[3]), "voltageSetting[3]", "double", 1, 199, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u4", offsetof(struct UserVar, voltageSetting[4]), "voltageSetting[4]", "double", 1, 200, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u5", offsetof(struct UserVar, voltageSetting[5]), "voltageSetting[5]", "double", 1, 201, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u6", offsetof(struct UserVar, voltageSetting[6]), "voltageSetting[6]", "double", 1, 202, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u7", offsetof(struct UserVar, voltageSetting[7]), "voltageSetting[7]", "double", 1, 203, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u100", offsetof(struct UserVar, voltageSetting[8]), "voltageSetting[8]", "double", 1, 204, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u101", offsetof(struct UserVar, voltageSetting[9]), "voltageSetting[9]", "double", 1, 205, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u102", offsetof(struct UserVar, voltageSetting[10]), "voltageSetting[10]", "double", 1, 206, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u103", offsetof(struct UserVar, voltageSetting[11]), "voltageSetting[11]", "double", 1, 207, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u104", offsetof(struct UserVar, voltageSetting[12]), "voltageSetting[12]", "double", 1, 208, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u105", offsetof(struct UserVar, voltageSetting[13]), "voltageSetting[13]", "double", 1, 209, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u106", offsetof(struct UserVar, voltageSetting[14]), "voltageSetting[14]", "double", 1, 210, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u107", offsetof(struct UserVar, voltageSetting[15]), "voltageSetting[15]", "double", 1, 211, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u200", offsetof(struct UserVar, voltageSetting[16]), "voltageSetting[16]", "double", 1, 212, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u201", offsetof(struct UserVar, voltageSetting[17]), "voltageSetting[17]", "double", 1, 213, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u202", offsetof(struct UserVar, voltageSetting[18]), "voltageSetting[18]", "double", 1, 214, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u203", offsetof(struct UserVar, voltageSetting[19]), "voltageSetting[19]", "double", 1, 215, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u204", offsetof(struct UserVar, voltageSetting[20]), "voltageSetting[20]", "double", 1, 216, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u205", offsetof(struct UserVar, voltageSetting[21]), "voltageSetting[21]", "double", 1, 217, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u206", offsetof(struct UserVar, voltageSetting[22]), "voltageSetting[22]", "double", 1, 218, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_u207", offsetof(struct UserVar, voltageSetting[23]), "voltageSetting[23]", "double", 1, 219, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[24]), "voltageSetting[24]", "double", 1, 220, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[25]), "voltageSetting[25]", "double", 1, 221, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[26]), "voltageSetting[26]", "double", 1, 222, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[27]), "voltageSetting[27]", "double", 1, 223, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[28]), "voltageSetting[28]", "double", 1, 224, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[29]), "voltageSetting[29]", "double", 1, 225, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[30]), "voltageSetting[30]", "double", 1, 226, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[31]), "voltageSetting[31]", "double", 1, 227, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u0", offsetof(struct UserVar, currentSetting[0]), "currentSetting[0]", "double", 1, 228, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u1", offsetof(struct UserVar, currentSetting[1]), "currentSetting[1]", "double", 1, 229, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u2", offsetof(struct UserVar, currentSetting[2]), "currentSetting[2]", "double", 1, 230, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u3", offsetof(struct UserVar, currentSetting[3]), "currentSetting[3]", "double", 1, 231, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u4", offsetof(struct UserVar, currentSetting[4]), "currentSetting[4]", "double", 1, 232, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u5", offsetof(struct UserVar, currentSetting[5]), "currentSetting[5]", "double", 1, 233, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u6", offsetof(struct UserVar, currentSetting[6]), "currentSetting[6]", "double", 1, 234, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u7", offsetof(struct UserVar, currentSetting[7]), "currentSetting[7]", "double", 1, 235, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u100", offsetof(struct UserVar, currentSetting[8]), "currentSetting[8]", "double", 1, 236, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u101", offsetof(struct UserVar, currentSetting[9]), "currentSetting[9]", "double", 1, 237, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u102", offsetof(struct UserVar, currentSetting[10]), "currentSetting[10]", "double", 1, 238, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u103", offsetof(struct UserVar, currentSetting[11]), "currentSetting[11]", "double", 1, 239, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u104", offsetof(struct UserVar, currentSetting[12]), "currentSetting[12]", "double", 1, 240, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u105", offsetof(struct UserVar, currentSetting[13]), "currentSetting[13]", "double", 1, 241, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u106", offsetof(struct UserVar, currentSetting[14]), "currentSetting[14]", "double", 1, 242, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u107", offsetof(struct UserVar, currentSetting[15]), "currentSetting[15]", "double", 1, 243, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u200", offsetof(struct UserVar, currentSetting[16]), "currentSetting[16]", "double", 1, 244, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u201", offsetof(struct UserVar, currentSetting[17]), "currentSetting[17]", "double", 1, 245, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u202", offsetof(struct UserVar, currentSetting[18]), "currentSetting[18]", "double", 1, 246, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u203", offsetof(struct UserVar, currentSetting[19]), "currentSetting[19]", "double", 1, 247, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u204", offsetof(struct UserVar, currentSetting[20]), "currentSetting[20]", "double", 1, 248, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u205", offsetof(struct UserVar, currentSetting[21]), "currentSetting[21]", "double", 1, 249, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u206", offsetof(struct UserVar, currentSetting[22]), "currentSetting[22]", "double", 1, 250, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_u207", offsetof(struct UserVar, currentSetting[23]), "currentSetting[23]", "double", 1, 251, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[24]), "currentSetting[24]", "double", 1, 252, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[25]), "currentSetting[25]", "double", 1, 253, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[26]), "currentSetting[26]", "double", 1, 254, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[27]), "currentSetting[27]", "double", 1, 255, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[28]), "currentSetting[28]", "double", 1, 256, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[29]), "currentSetting[29]", "double", 1, 257, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[30]), "currentSetting[30]", "double", 1, 258, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[31]), "currentSetting[31]", "double", 1, 259, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u0", offsetof(struct UserVar, rampRate[0]), "rampRate[0]", "double", 1, 260, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u1", offsetof(struct UserVar, rampRate[1]), "rampRate[1]", "double", 1, 261, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u2", offsetof(struct UserVar, rampRate[2]), "rampRate[2]", "double", 1, 262, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u3", offsetof(struct UserVar, rampRate[3]), "rampRate[3]", "double", 1, 263, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u4", offsetof(struct UserVar, rampRate[4]), "rampRate[4]", "double", 1, 264, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u5", offsetof(struct UserVar, rampRate[5]), "rampRate[5]", "double", 1, 265, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u6", offsetof(struct UserVar, rampRate[6]), "rampRate[6]", "double", 1, 266, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u7", offsetof(struct UserVar, rampRate[7]), "rampRate[7]", "double", 1, 267, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u100", offsetof(struct UserVar, rampRate[8]), "rampRate[8]", "double", 1, 268, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u101", offsetof(struct UserVar, rampRate[9]), "rampRate[9]", "double", 1, 269, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u102", offsetof(struct UserVar, rampRate[10]), "rampRate[10]", "double", 1, 270, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u103", offsetof(struct UserVar, rampRate[11]), "rampRate[11]", "double", 1, 271, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u104", offsetof(struct UserVar, rampRate[12]), "rampRate[12]", "double", 1, 272, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u105", offsetof(struct UserVar, rampRate[13]), "rampRate[13]", "double", 1, 273, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u106", offsetof(struct UserVar, rampRate[14]), "rampRate[14]", "double", 1, 274, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u107", offsetof(struct UserVar, rampRate[15]), "rampRate[15]", "double", 1, 275, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u200", offsetof(struct UserVar, rampRate[16]), "rampRate[16]", "double", 1, 276, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u201", offsetof(struct UserVar, rampRate[17]), "rampRate[17]", "double", 1, 277, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u202", offsetof(struct UserVar, rampRate[18]), "rampRate[18]", "double", 1, 278, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u203", offsetof(struct UserVar, rampRate[19]), "rampRate[19]", "double", 1, 279, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u204", offsetof(struct UserVar, rampRate[20]), "rampRate[20]", "double", 1, 280, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u205", offsetof(struct UserVar, rampRate[21]), "rampRate[21]", "double", 1, 281, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u206", offsetof(struct UserVar, rampRate[22]), "rampRate[22]", "double", 1, 282, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_u207", offsetof(struct UserVar, rampRate[23]), "rampRate[23]", "double", 1, 283, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[24]), "rampRate[24]", "double", 1, 284, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[25]), "rampRate[25]", "double", 1, 285, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[26]), "rampRate[26]", "double", 1, 286, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[27]), "rampRate[27]", "double", 1, 287, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[28]), "rampRate[28]", "double", 1, 288, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[29]), "rampRate[29]", "double", 1, 289, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[30]), "rampRate[30]", "double", 1, 290, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[31]), "rampRate[31]", "double", 1, 291, 0, 0, 0, 0},
	{"{SYSTEM}:u0:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[0]), "autoResetDelay[0]", "double", 1, 292, 0, 0, 0, 0},
	{"{SYSTEM}:u1:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[1]), "autoResetDelay[1]", "double", 1, 293, 0, 0, 0, 0},
	{"{SYSTEM}:u2:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[2]), "autoResetDelay[2]", "double", 1, 294, 0, 0, 0, 0},
	{"{SYSTEM}:u3:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[3]), "autoResetDelay[3]", "double", 1, 295, 0, 0, 0, 0},
	{"{SYSTEM}:u4:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[4]), "autoResetDelay[4]", "double", 1, 296, 0, 0, 0, 0},
	{"{SYSTEM}:u5:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[5]), "autoResetDelay[5]", "double", 1, 297, 0, 0, 0, 0},
	{"{SYSTEM}:u6:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[6]), "autoResetDelay[6]", "double", 1, 298, 0, 0, 0, 0},
	{"{SYSTEM}:u7:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[7]), "autoResetDelay[7]", "double", 1, 299, 0, 0, 0, 0},
	{"{SYSTEM}:u100:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[8]), "autoResetDelay[8]", "double", 1, 300, 0, 0, 0, 0},
	{"{SYSTEM}:u101:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[9]), "autoResetDelay[9]", "double", 1, 301, 0, 0, 0, 0},
	{"{SYSTEM}:u102:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[10]), "autoResetDelay[10]", "double", 1, 302, 0, 0, 0, 0},
	{"{SYSTEM}:u103:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[11]), "autoResetDelay[11]", "double", 1, 303, 0, 0, 0, 0},
	{"{SYSTEM}:u104:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[12]), "autoResetDelay[12]", "double", 1, 304, 0, 0, 0, 0},
	{"{SYSTEM}:u105:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[13]), "autoResetDelay[13]", "double", 1, 305, 0, 0, 0, 0},
	{"{SYSTEM}:u106:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[14]), "autoResetDelay[14]", "double", 1, 306, 0, 0, 0, 0},
	{"{SYSTEM}:u107:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[15]), "autoResetDelay[15]", "double", 1, 307, 0, 0, 0, 0},
	{"{SYSTEM}:u200:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[16]), "autoResetDelay[16]", "double", 1, 308, 0, 0, 0, 0},
	{"{SYSTEM}:u201:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[17]), "autoResetDelay[17]", "double", 1, 309, 0, 0, 0, 0},
	{"{SYSTEM}:u202:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[18]), "autoResetDelay[18]", "double", 1, 310, 0, 0, 0, 0},
	{"{SYSTEM}:u203:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[19]), "autoResetDelay[19]", "double", 1, 311, 0, 0, 0, 0},
	{"{SYSTEM}:u204:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[20]), "autoResetDelay[20]", "double", 1, 312, 0, 0, 0, 0},
	{"{SYSTEM}:u205:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[21]), "autoResetDelay[21]", "double", 1, 313, 0, 0, 0, 0},
	{"{SYSTEM}:u206:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[22]), "autoResetDelay[22]", "double", 1, 314, 0, 0, 0, 0},
	{"{SYSTEM}:u207:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[23]), "autoResetDelay[23]", "double", 1, 315, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[24]), "autoResetDelay[24]", "double", 1, 316, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[25]), "autoResetDelay[25]", "double", 1, 317, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[26]), "autoResetDelay[26]", "double", 1, 318, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[27]), "autoResetDelay[27]", "double", 1, 319, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[28]), "autoResetDelay[28]", "double", 1, 320, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[29]), "autoResetDelay[29]", "double", 1, 321, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[30]), "autoResetDelay[30]", "double", 1, 322, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[31]), "autoResetDelay[31]", "double", 1, 323, 0, 0, 0, 0},
	{"{SYSTEM}:u0:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[0]), "autoResetEnabled[0]", "double", 1, 324, 0, 0, 0, 0},
	{"{SYSTEM}:u1:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[1]), "autoResetEnabled[1]", "double", 1, 325, 0, 0, 0, 0},
	{"{SYSTEM}:u2:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[2]), "autoResetEnabled[2]", "double", 1, 326, 0, 0, 0, 0},
	{"{SYSTEM}:u3:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[3]), "autoResetEnabled[3]", "double", 1, 327, 0, 0, 0, 0},
	{"{SYSTEM}:u4:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[4]), "autoResetEnabled[4]", "double", 1, 328, 0, 0, 0, 0},
	{"{SYSTEM}:u5:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[5]), "autoResetEnabled[5]", "double", 1, 329, 0, 0, 0, 0},
	{"{SYSTEM}:u6:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[6]), "autoResetEnabled[6]", "double", 1, 330, 0, 0, 0, 0},
	{"{SYSTEM}:u7:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[7]), "autoResetEnabled[7]", "double", 1, 331, 0, 0, 0, 0},
	{"{SYSTEM}:u100:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[8]), "autoResetEnabled[8]", "double", 1, 332, 0, 0, 0, 0},
	{"{SYSTEM}:u101:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[9]), "autoResetEnabled[9]", "double", 1, 333, 0, 0, 0, 0},
	{"{SYSTEM}:u102:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[10]), "autoResetEnabled[10]", "double", 1, 334, 0, 0, 0, 0},
	{"{SYSTEM}:u103:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[11]), "autoResetEnabled[11]", "double", 1, 335, 0, 0, 0, 0},
	{"{SYSTEM}:u104:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[12]), "autoResetEnabled[12]", "double", 1, 336, 0, 0, 0, 0},
	{"{SYSTEM}:u105:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[13]), "autoResetEnabled[13]", "double", 1, 337, 0, 0, 0, 0},
	{"{SYSTEM}:u106:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[14]), "autoResetEnabled[14]", "double", 1, 338, 0, 0, 0, 0},
	{"{SYSTEM}:u107:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[15]), "autoResetEnabled[15]", "double", 1, 339, 0, 0, 0, 0},
	{"{SYSTEM}:u200:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[16]), "autoResetEnabled[16]", "double", 1, 340, 0, 0, 0, 0},
	{"{SYSTEM}:u201:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[17]), "autoResetEnabled[17]", "double", 1, 341, 0, 0, 0, 0},
	{"{SYSTEM}:u202:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[18]), "autoResetEnabled[18]", "double", 1, 342, 0, 0, 0, 0},
	{"{SYSTEM}:u203:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[19]), "autoResetEnabled[19]", "double", 1, 343, 0, 0, 0, 0},
	{"{SYSTEM}:u204:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[20]), "autoResetEnabled[20]", "double", 1, 344, 0, 0, 0, 0},
	{"{SYSTEM}:u205:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[21]), "autoResetEnabled[21]", "double", 1, 345, 0, 0, 0, 0},
	{"{SYSTEM}:u206:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[22]), "autoResetEnabled[22]", "double", 1, 346, 0, 0, 0, 0},
	{"{SYSTEM}:u207:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[23]), "autoResetEnabled[23]", "double", 1, 347, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[24]), "autoResetEnabled[24]", "double", 1, 348, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[25]), "autoResetEnabled[25]", "double", 1, 349, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[26]), "autoResetEnabled[26]", "double", 1, 350, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[27]), "autoResetEnabled[27]", "double", 1, 351, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[28]), "autoResetEnabled[28]", "double", 1, 352, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[29]), "autoResetEnabled[29]", "double", 1, 353, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[30]), "autoResetEnabled[30]", "double", 1, 354, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[31]), "autoResetEnabled[31]", "double", 1, 355, 0, 0, 0, 0},
	{"{SYSTEM}:u0:maxCycles", offsetof(struct UserVar, maxCycles[0]), "maxCycles[0]", "double", 1, 356, 0, 0, 0, 0},
	{"{SYSTEM}:u1:maxCycles", offsetof(struct UserVar, maxCycles[1]), "maxCycles[1]", "double", 1, 357, 0, 0, 0, 0},
	{"{SYSTEM}:u2:maxCycles", offsetof(struct UserVar, maxCycles[2]), "maxCycles[2]", "double", 1, 358, 0, 0, 0, 0},
	{"{SYSTEM}:u3:maxCycles", offsetof(struct UserVar, maxCycles[3]), "maxCycles[3]", "double", 1, 359, 0, 0, 0, 0},
	{"{SYSTEM}:u4:maxCycles", offsetof(struct UserVar, maxCycles[4]), "maxCycles[4]", "double", 1, 360, 0, 0, 0, 0},
	{"{SYSTEM}:u5:maxCycles", offsetof(struct UserVar, maxCycles[5]), "maxCycles[5]", "double", 1, 361, 0, 0, 0, 0},
	{"{SYSTEM}:u6:maxCycles", offsetof(struct UserVar, maxCycles[6]), "maxCycles[6]", "double", 1, 362, 0, 0, 0, 0},
	{"{SYSTEM}:u7:maxCycles", offsetof(struct UserVar, maxCycles[7]), "maxCycles[7]", "double", 1, 363, 0, 0, 0, 0},
	{"{SYSTEM}:u100:maxCycles", offsetof(struct UserVar, maxCycles[8]), "maxCycles[8]", "double", 1, 364, 0, 0, 0, 0},
	{"{SYSTEM}:u101:maxCycles", offsetof(struct UserVar, maxCycles[9]), "maxCycles[9]", "double", 1, 365, 0, 0, 0, 0},
	{"{SYSTEM}:u102:maxCycles", offsetof(struct UserVar, maxCycles[10]), "maxCycles[10]", "double", 1, 366, 0, 0, 0, 0},
	{"{SYSTEM}:u103:maxCycles", offsetof(struct UserVar, maxCycles[11]), "maxCycles[11]", "double", 1, 367, 0, 0, 0, 0},
	{"{SYSTEM}:u104:maxCycles", offsetof(struct UserVar, maxCycles[12]), "maxCycles[12]", "double", 1, 368, 0, 0, 0, 0},
	{"{SYSTEM}:u105:maxCycles", offsetof(struct UserVar, maxCycles[13]), "maxCycles[13]", "double", 1, 369, 0, 0, 0, 0},
	{"{SYSTEM}:u106:maxCycles", offsetof(struct UserVar, maxCycles[14]), "maxCycles[14]", "double", 1, 370, 0, 0, 0, 0},
	{"{SYSTEM}:u107:maxCycles", offsetof(struct UserVar, maxCycles[15]), "maxCycles[15]", "double", 1, 371, 0, 0, 0, 0},
	{"{SYSTEM}:u200:maxCycles", offsetof(struct UserVar, maxCycles[16]), "maxCycles[16]", "double", 1, 372, 0, 0, 0, 0},
	{"{SYSTEM}:u201:maxCycles", offsetof(struct UserVar, maxCycles[17]), "maxCycles[17]", "double", 1, 373, 0, 0, 0, 0},
	{"{SYSTEM}:u202:maxCycles", offsetof(struct UserVar, maxCycles[18]), "maxCycles[18]", "double", 1, 374, 0, 0, 0, 0},
	{"{SYSTEM}:u203:maxCycles", offsetof(struct UserVar, maxCycles[19]), "maxCycles[19]", "double", 1, 375, 0, 0, 0, 0},
	{"{SYSTEM}:u204:maxCycles", offsetof(struct UserVar, maxCycles[20]), "maxCycles[20]", "double", 1, 376, 0, 0, 0, 0},
	{"{SYSTEM}:u205:maxCycles", offsetof(struct UserVar, maxCycles[21]), "maxCycles[21]", "double", 1, 377, 0, 0, 0, 0},
	{"{SYSTEM}:u206:maxCycles", offsetof(struct UserVar, maxCycles[22]), "maxCycles[22]", "double", 1, 378, 0, 0, 0, 0},
	{"{SYSTEM}:u207:maxCycles", offsetof(struct UserVar, maxCycles[23]), "maxCycles[23]", "double", 1, 379, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[24]), "maxCycles[24]", "double", 1, 380, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[25]), "maxCycles[25]", "double", 1, 381, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[26]), "maxCycles[26]", "double", 1, 382, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[27]), "maxCycles[27]", "double", 1, 383, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[28]), "maxCycles[28]", "double", 1, 384, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[29]), "maxCycles[29]", "double", 1, 385, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[30]), "maxCycles[30]", "double", 1, 386, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[31]), "maxCycles[31]", "double", 1, 387, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[32]), "maxCycles[32]", "double", 1, 388, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[33]), "maxCycles[33]", "double", 1, 389, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[34]), "maxCycles[34]", "double", 1, 390, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[35]), "maxCycles[35]", "double", 1, 391, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[36]), "maxCycles[36]", "double", 1, 392, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[37]), "maxCycles[37]", "double", 1, 393, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[38]), "maxCycles[38]", "double", 1, 394, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[39]), "maxCycles[39]", "double", 1, 395, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[40]), "maxCycles[40]", "double", 1, 396, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[41]), "maxCycles[41]", "double", 1, 397, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[42]), "maxCycles[42]", "double", 1, 398, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[43]), "maxCycles[43]", "double", 1, 399, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[44]), "maxCycles[44]", "double", 1, 400, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[45]), "maxCycles[45]", "double", 1, 401, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[46]), "maxCycles[46]", "double", 1, 402, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[47]), "maxCycles[47]", "double", 1, 403, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[48]), "maxCycles[48]", "double", 1, 404, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[49]), "maxCycles[49]", "double", 1, 405, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[50]), "maxCycles[50]", "double", 1, 406, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[51]), "maxCycles[51]", "double", 1, 407, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[52]), "maxCycles[52]", "double", 1, 408, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[53]), "maxCycles[53]", "double", 1, 409, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[54]), "maxCycles[54]", "double", 1, 410, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[55]), "maxCycles[55]", "double", 1, 411, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[56]), "maxCycles[56]", "double", 1, 412, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[57]), "maxCycles[57]", "double", 1, 413, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[58]), "maxCycles[58]", "double", 1, 414, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[59]), "maxCycles[59]", "double", 1, 415, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[60]), "maxCycles[60]", "double", 1, 416, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[61]), "maxCycles[61]", "double", 1, 417, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[62]), "maxCycles[62]", "double", 1, 418, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[63]), "maxCycles[63]", "double", 1, 419, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[64]), "maxCycles[64]", "double", 1, 420, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[65]), "maxCycles[65]", "double", 1, 421, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[66]), "maxCycles[66]", "double", 1, 422, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[67]), "maxCycles[67]", "double", 1, 423, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[68]), "maxCycles[68]", "double", 1, 424, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[69]), "maxCycles[69]", "double", 1, 425, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[70]), "maxCycles[70]", "double", 1, 426, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[71]), "maxCycles[71]", "double", 1, 427, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[72]), "maxCycles[72]", "double", 1, 428, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[73]), "maxCycles[73]", "double", 1, 429, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[74]), "maxCycles[74]", "double", 1, 430, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[75]), "maxCycles[75]", "double", 1, 431, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[76]), "maxCycles[76]", "double", 1, 432, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[77]), "maxCycles[77]", "double", 1, 433, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[78]), "maxCycles[78]", "double", 1, 434, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[79]), "maxCycles[79]", "double", 1, 435, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[80]), "maxCycles[80]", "double", 1, 436, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[81]), "maxCycles[81]", "double", 1, 437, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[82]), "maxCycles[82]", "double", 1, 438, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[83]), "maxCycles[83]", "double", 1, 439, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[84]), "maxCycles[84]", "double", 1, 440, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[85]), "maxCycles[85]", "double", 1, 441, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[86]), "maxCycles[86]", "double", 1, 442, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[87]), "maxCycles[87]", "double", 1, 443, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[88]), "maxCycles[88]", "double", 1, 444, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[89]), "maxCycles[89]", "double", 1, 445, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[90]), "maxCycles[90]", "double", 1, 446, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[91]), "maxCycles[91]", "double", 1, 447, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[92]), "maxCycles[92]", "double", 1, 448, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[93]), "maxCycles[93]", "double", 1, 449, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[94]), "maxCycles[94]", "double", 1, 450, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[95]), "maxCycles[95]", "double", 1, 451, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[96]), "maxCycles[96]", "double", 1, 452, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[97]), "maxCycles[97]", "double", 1, 453, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[98]), "maxCycles[98]", "double", 1, 454, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[99]), "maxCycles[99]", "double", 1, 455, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[100]), "maxCycles[100]", "double", 1, 456, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[101]), "maxCycles[101]", "double", 1, 457, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[102]), "maxCycles[102]", "double", 1, 458, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[103]), "maxCycles[103]", "double", 1, 459, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[104]), "maxCycles[104]", "double", 1, 460, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[105]), "maxCycles[105]", "double", 1, 461, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[106]), "maxCycles[106]", "double", 1, 462, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[107]), "maxCycles[107]", "double", 1, 463, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[108]), "maxCycles[108]", "double", 1, 464, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[109]), "maxCycles[109]", "double", 1, 465, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[110]), "maxCycles[110]", "double", 1, 466, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[111]), "maxCycles[111]", "double", 1, 467, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[112]), "maxCycles[112]", "double", 1, 468, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[113]), "maxCycles[113]", "double", 1, 469, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[114]), "maxCycles[114]", "double", 1, 470, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[115]), "maxCycles[115]", "double", 1, 471, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[116]), "maxCycles[116]", "double", 1, 472, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[117]), "maxCycles[117]", "double", 1, 473, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[118]), "maxCycles[118]", "double", 1, 474, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[119]), "maxCycles[119]", "double", 1, 475, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[120]), "maxCycles[120]", "double", 1, 476, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[121]), "maxCycles[121]", "double", 1, 477, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[122]), "maxCycles[122]", "double", 1, 478, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[123]), "maxCycles[123]", "double", 1, 479, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[124]), "maxCycles[124]", "double", 1, 480, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[125]), "maxCycles[125]", "double", 1, 481, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[126]), "maxCycles[126]", "double", 1, 482, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[127]), "maxCycles[127]", "double", 1, 483, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[128]), "maxCycles[128]", "double", 1, 484, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[129]), "maxCycles[129]", "double", 1, 485, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[130]), "maxCycles[130]", "double", 1, 486, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[131]), "maxCycles[131]", "double", 1, 487, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[132]), "maxCycles[132]", "double", 1, 488, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[133]), "maxCycles[133]", "double", 1, 489, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[134]), "maxCycles[134]", "double", 1, 490, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[135]), "maxCycles[135]", "double", 1, 491, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[136]), "maxCycles[136]", "double", 1, 492, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[137]), "maxCycles[137]", "double", 1, 493, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[138]), "maxCycles[138]", "double", 1, 494, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[139]), "maxCycles[139]", "double", 1, 495, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[140]), "maxCycles[140]", "double", 1, 496, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[141]), "maxCycles[141]", "double", 1, 497, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[142]), "maxCycles[142]", "double", 1, 498, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[143]), "maxCycles[143]", "double", 1, 499, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[144]), "maxCycles[144]", "double", 1, 500, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[145]), "maxCycles[145]", "double", 1, 501, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[146]), "maxCycles[146]", "double", 1, 502, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[147]), "maxCycles[147]", "double", 1, 503, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[148]), "maxCycles[148]", "double", 1, 504, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[149]), "maxCycles[149]", "double", 1, 505, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[150]), "maxCycles[150]", "double", 1, 506, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[151]), "maxCycles[151]", "double", 1, 507, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[152]), "maxCycles[152]", "double", 1, 508, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[153]), "maxCycles[153]", "double", 1, 509, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[154]), "maxCycles[154]", "double", 1, 510, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[155]), "maxCycles[155]", "double", 1, 511, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[156]), "maxCycles[156]", "double", 1, 512, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[157]), "maxCycles[157]", "double", 1, 513, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[158]), "maxCycles[158]", "double", 1, 514, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[159]), "maxCycles[159]", "double", 1, 515, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[160]), "maxCycles[160]", "double", 1, 516, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[161]), "maxCycles[161]", "double", 1, 517, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[162]), "maxCycles[162]", "double", 1, 518, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[163]), "maxCycles[163]", "double", 1, 519, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[164]), "maxCycles[164]", "double", 1, 520, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[165]), "maxCycles[165]", "double", 1, 521, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[166]), "maxCycles[166]", "double", 1, 522, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[167]), "maxCycles[167]", "double", 1, 523, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[168]), "maxCycles[168]", "double", 1, 524, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[169]), "maxCycles[169]", "double", 1, 525, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[170]), "maxCycles[170]", "double", 1, 526, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[171]), "maxCycles[171]", "double", 1, 527, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[172]), "maxCycles[172]", "double", 1, 528, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[173]), "maxCycles[173]", "double", 1, 529, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[174]), "maxCycles[174]", "double", 1, 530, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[175]), "maxCycles[175]", "double", 1, 531, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[176]), "maxCycles[176]", "double", 1, 532, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[177]), "maxCycles[177]", "double", 1, 533, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[178]), "maxCycles[178]", "double", 1, 534, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[179]), "maxCycles[179]", "double", 1, 535, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[180]), "maxCycles[180]", "double", 1, 536, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[181]), "maxCycles[181]", "double", 1, 537, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[182]), "maxCycles[182]", "double", 1, 538, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[183]), "maxCycles[183]", "double", 1, 539, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[184]), "maxCycles[184]", "double", 1, 540, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[185]), "maxCycles[185]", "double", 1, 541, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[186]), "maxCycles[186]", "double", 1, 542, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[187]), "maxCycles[187]", "double", 1, 543, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[188]), "maxCycles[188]", "double", 1, 544, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[189]), "maxCycles[189]", "double", 1, 545, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[190]), "maxCycles[190]", "double", 1, 546, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[191]), "maxCycles[191]", "double", 1, 547, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[192]), "maxCycles[192]", "double", 1, 548, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[193]), "maxCycles[193]", "double", 1, 549, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[194]), "maxCycles[194]", "double", 1, 550, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[195]), "maxCycles[195]", "double", 1, 551, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[196]), "maxCycles[196]", "double", 1, 552, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[197]), "maxCycles[197]", "double", 1, 553, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[198]), "maxCycles[198]", "double", 1, 554, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[199]), "maxCycles[199]", "double", 1, 555, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[200]), "maxCycles[200]", "double", 1, 556, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[201]), "maxCycles[201]", "double", 1, 557, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[202]), "maxCycles[202]", "double", 1, 558, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[203]), "maxCycles[203]", "double", 1, 559, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[204]), "maxCycles[204]", "double", 1, 560, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[205]), "maxCycles[205]", "double", 1, 561, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[206]), "maxCycles[206]", "double", 1, 562, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[207]), "maxCycles[207]", "double", 1, 563, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[208]), "maxCycles[208]", "double", 1, 564, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[209]), "maxCycles[209]", "double", 1, 565, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[210]), "maxCycles[210]", "double", 1, 566, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[211]), "maxCycles[211]", "double", 1, 567, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[212]), "maxCycles[212]", "double", 1, 568, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[213]), "maxCycles[213]", "double", 1, 569, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[214]), "maxCycles[214]", "double", 1, 570, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[215]), "maxCycles[215]", "double", 1, 571, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[216]), "maxCycles[216]", "double", 1, 572, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[217]), "maxCycles[217]", "double", 1, 573, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[218]), "maxCycles[218]", "double", 1, 574, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[219]), "maxCycles[219]", "double", 1, 575, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[220]), "maxCycles[220]", "double", 1, 576, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[221]), "maxCycles[221]", "double", 1, 577, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[222]), "maxCycles[222]", "double", 1, 578, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[223]), "maxCycles[223]", "double", 1, 579, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[224]), "maxCycles[224]", "double", 1, 580, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[225]), "maxCycles[225]", "double", 1, 581, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[226]), "maxCycles[226]", "double", 1, 582, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[227]), "maxCycles[227]", "double", 1, 583, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[228]), "maxCycles[228]", "double", 1, 584, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[229]), "maxCycles[229]", "double", 1, 585, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[230]), "maxCycles[230]", "double", 1, 586, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[231]), "maxCycles[231]", "double", 1, 587, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[232]), "maxCycles[232]", "double", 1, 588, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[233]), "maxCycles[233]", "double", 1, 589, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[234]), "maxCycles[234]", "double", 1, 590, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[235]), "maxCycles[235]", "double", 1, 591, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[236]), "maxCycles[236]", "double", 1, 592, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[237]), "maxCycles[237]", "double", 1, 593, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[238]), "maxCycles[238]", "double", 1, 594, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[239]), "maxCycles[239]", "double", 1, 595, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[240]), "maxCycles[240]", "double", 1, 596, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[241]), "maxCycles[241]", "double", 1, 597, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[242]), "maxCycles[242]", "double", 1, 598, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[243]), "maxCycles[243]", "double", 1, 599, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[244]), "maxCycles[244]", "double", 1, 600, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[245]), "maxCycles[245]", "double", 1, 601, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[246]), "maxCycles[246]", "double", 1, 602, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[247]), "maxCycles[247]", "double", 1, 603, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[248]), "maxCycles[248]", "double", 1, 604, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[249]), "maxCycles[249]", "double", 1, 605, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[250]), "maxCycles[250]", "double", 1, 606, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[251]), "maxCycles[251]", "double", 1, 607, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[252]), "maxCycles[252]", "double", 1, 608, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[253]), "maxCycles[253]", "double", 1, 609, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[254]), "maxCycles[254]", "double", 1, 610, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[255]), "maxCycles[255]", "double", 1, 611, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[256]), "maxCycles[256]", "double", 1, 612, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[257]), "maxCycles[257]", "double", 1, 613, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[258]), "maxCycles[258]", "double", 1, 614, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[259]), "maxCycles[259]", "double", 1, 615, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[260]), "maxCycles[260]", "double", 1, 616, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[261]), "maxCycles[261]", "double", 1, 617, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[262]), "maxCycles[262]", "double", 1, 618, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[263]), "maxCycles[263]", "double", 1, 619, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[264]), "maxCycles[264]", "double", 1, 620, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[265]), "maxCycles[265]", "double", 1, 621, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[266]), "maxCycles[266]", "double", 1, 622, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[267]), "maxCycles[267]", "double", 1, 623, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[268]), "maxCycles[268]", "double", 1, 624, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[269]), "maxCycles[269]", "double", 1, 625, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[270]), "maxCycles[270]", "double", 1, 626, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[271]), "maxCycles[271]", "double", 1, 627, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[272]), "maxCycles[272]", "double", 1, 628, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[273]), "maxCycles[273]", "double", 1, 629, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[274]), "maxCycles[274]", "double", 1, 630, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[275]), "maxCycles[275]", "double", 1, 631, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[276]), "maxCycles[276]", "double", 1, 632, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[277]), "maxCycles[277]", "double", 1, 633, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[278]), "maxCycles[278]", "double", 1, 634, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[279]), "maxCycles[279]", "double", 1, 635, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[280]), "maxCycles[280]", "double", 1, 636, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[281]), "maxCycles[281]", "double", 1, 637, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[282]), "maxCycles[282]", "double", 1, 638, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[283]), "maxCycles[283]", "double", 1, 639, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[284]), "maxCycles[284]", "double", 1, 640, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[285]), "maxCycles[285]", "double", 1, 641, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[286]), "maxCycles[286]", "double", 1, 642, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[287]), "maxCycles[287]", "double", 1, 643, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[288]), "maxCycles[288]", "double", 1, 644, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[289]), "maxCycles[289]", "double", 1, 645, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[290]), "maxCycles[290]", "double", 1, 646, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[291]), "maxCycles[291]", "double", 1, 647, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[292]), "maxCycles[292]", "double", 1, 648, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[293]), "maxCycles[293]", "double", 1, 649, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[294]), "maxCycles[294]", "double", 1, 650, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[295]), "maxCycles[295]", "double", 1, 651, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[296]), "maxCycles[296]", "double", 1, 652, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[297]), "maxCycles[297]", "double", 1, 653, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[298]), "maxCycles[298]", "double", 1, 654, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[299]), "maxCycles[299]", "double", 1, 655, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[300]), "maxCycles[300]", "double", 1, 656, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[301]), "maxCycles[301]", "double", 1, 657, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[302]), "maxCycles[302]", "double", 1, 658, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[303]), "maxCycles[303]", "double", 1, 659, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[304]), "maxCycles[304]", "double", 1, 660, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[305]), "maxCycles[305]", "double", 1, 661, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[306]), "maxCycles[306]", "double", 1, 662, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[307]), "maxCycles[307]", "double", 1, 663, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[308]), "maxCycles[308]", "double", 1, 664, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[309]), "maxCycles[309]", "double", 1, 665, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[310]), "maxCycles[310]", "double", 1, 666, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[311]), "maxCycles[311]", "double", 1, 667, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[312]), "maxCycles[312]", "double", 1, 668, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[313]), "maxCycles[313]", "double", 1, 669, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[314]), "maxCycles[314]", "double", 1, 670, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[315]), "maxCycles[315]", "double", 1, 671, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[316]), "maxCycles[316]", "double", 1, 672, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[317]), "maxCycles[317]", "double", 1, 673, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[318]), "maxCycles[318]", "double", 1, 674, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[319]), "maxCycles[319]", "double", 1, 675, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[320]), "maxCycles[320]", "double", 1, 676, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[321]), "maxCycles[321]", "double", 1, 677, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[322]), "maxCycles[322]", "double", 1, 678, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[323]), "maxCycles[323]", "double", 1, 679, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[324]), "maxCycles[324]", "double", 1, 680, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[325]), "maxCycles[325]", "double", 1, 681, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[326]), "maxCycles[326]", "double", 1, 682, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[327]), "maxCycles[327]", "double", 1, 683, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[328]), "maxCycles[328]", "double", 1, 684, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[329]), "maxCycles[329]", "double", 1, 685, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[330]), "maxCycles[330]", "double", 1, 686, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[331]), "maxCycles[331]", "double", 1, 687, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u0", offsetof(struct UserVar, powerSwitch[0]), "powerSwitch[0]", "double", 1, 688, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u1", offsetof(struct UserVar, powerSwitch[1]), "powerSwitch[1]", "double", 1, 689, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u2", offsetof(struct UserVar, powerSwitch[2]), "powerSwitch[2]", "double", 1, 690, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u3", offsetof(struct UserVar, powerSwitch[3]), "powerSwitch[3]", "double", 1, 691, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u4", offsetof(struct UserVar, powerSwitch[4]), "powerSwitch[4]", "double", 1, 692, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u5", offsetof(struct UserVar, powerSwitch[5]), "powerSwitch[5]", "double", 1, 693, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u6", offsetof(struct UserVar, powerSwitch[6]), "powerSwitch[6]", "double", 1, 694, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u7", offsetof(struct UserVar, powerSwitch[7]), "powerSwitch[7]", "double", 1, 695, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u100", offsetof(struct UserVar, powerSwitch[8]), "powerSwitch[8]", "double", 1, 696, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u101", offsetof(struct UserVar, powerSwitch[9]), "powerSwitch[9]", "double", 1, 697, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u102", offsetof(struct UserVar, powerSwitch[10]), "powerSwitch[10]", "double", 1, 698, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u103", offsetof(struct UserVar, powerSwitch[11]), "powerSwitch[11]", "double", 1, 699, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u104", offsetof(struct UserVar, powerSwitch[12]), "powerSwitch[12]", "double", 1, 700, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u105", offsetof(struct UserVar, powerSwitch[13]), "powerSwitch[13]", "double", 1, 701, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u106", offsetof(struct UserVar, powerSwitch[14]), "powerSwitch[14]", "double", 1, 702, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u107", offsetof(struct UserVar, powerSwitch[15]), "powerSwitch[15]", "double", 1, 703, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u200", offsetof(struct UserVar, powerSwitch[16]), "powerSwitch[16]", "double", 1, 704, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u201", offsetof(struct UserVar, powerSwitch[17]), "powerSwitch[17]", "double", 1, 705, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u202", offsetof(struct UserVar, powerSwitch[18]), "powerSwitch[18]", "double", 1, 706, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u203", offsetof(struct UserVar, powerSwitch[19]), "powerSwitch[19]", "double", 1, 707, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u204", offsetof(struct UserVar, powerSwitch[20]), "powerSwitch[20]", "double", 1, 708, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u205", offsetof(struct UserVar, powerSwitch[21]), "powerSwitch[21]", "double", 1, 709, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u206", offsetof(struct UserVar, powerSwitch[22]), "powerSwitch[22]", "double", 1, 710, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_u207", offsetof(struct UserVar, powerSwitch[23]), "powerSwitch[23]", "double", 1, 711, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[24]), "powerSwitch[24]", "double", 1, 712, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[25]), "powerSwitch[25]", "double", 1, 713, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[26]), "powerSwitch[26]", "double", 1, 714, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[27]), "powerSwitch[27]", "double", 1, 715, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[28]), "powerSwitch[28]", "double", 1, 716, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[29]), "powerSwitch[29]", "double", 1, 717, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[30]), "powerSwitch[30]", "double", 1, 718, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[31]), "powerSwitch[31]", "double", 1, 719, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u0", offsetof(struct UserVar, voltageReading[0]), "voltageReading[0]", "double", 1, 720, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u1", offsetof(struct UserVar, voltageReading[1]), "voltageReading[1]", "double", 1, 721, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u2", offsetof(struct UserVar, voltageReading[2]), "voltageReading[2]", "double", 1, 722, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u3", offsetof(struct UserVar, voltageReading[3]), "voltageReading[3]", "double", 1, 723, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u4", offsetof(struct UserVar, voltageReading[4]), "voltageReading[4]", "double", 1, 724, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u5", offsetof(struct UserVar, voltageReading[5]), "voltageReading[5]", "double", 1, 725, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u6", offsetof(struct UserVar, voltageReading[6]), "voltageReading[6]", "double", 1, 726, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u7", offsetof(struct UserVar, voltageReading[7]), "voltageReading[7]", "double", 1, 727, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u100", offsetof(struct UserVar, voltageReading[8]), "voltageReading[8]", "double", 1, 728, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u101", offsetof(struct UserVar, voltageReading[9]), "voltageReading[9]", "double", 1, 729, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u102", offsetof(struct UserVar, voltageReading[10]), "voltageReading[10]", "double", 1, 730, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u103", offsetof(struct UserVar, voltageReading[11]), "voltageReading[11]", "double", 1, 731, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u104", offsetof(struct UserVar, voltageReading[12]), "voltageReading[12]", "double", 1, 732, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u105", offsetof(struct UserVar, voltageReading[13]), "voltageReading[13]", "double", 1, 733, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u106", offsetof(struct UserVar, voltageReading[14]), "voltageReading[14]", "double", 1, 734, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u107", offsetof(struct UserVar, voltageReading[15]), "voltageReading[15]", "double", 1, 735, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u200", offsetof(struct UserVar, voltageReading[16]), "voltageReading[16]", "double", 1, 736, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u201", offsetof(struct UserVar, voltageReading[17]), "voltageReading[17]", "double", 1, 737, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u202", offsetof(struct UserVar, voltageReading[18]), "voltageReading[18]", "double", 1, 738, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u203", offsetof(struct UserVar, voltageReading[19]), "voltageReading[19]", "double", 1, 739, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u204", offsetof(struct UserVar, voltageReading[20]), "voltageReading[20]", "double", 1, 740, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u205", offsetof(struct UserVar, voltageReading[21]), "voltageReading[21]", "double", 1, 741, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u206", offsetof(struct UserVar, voltageReading[22]), "voltageReading[22]", "double", 1, 742, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_u207", offsetof(struct UserVar, voltageReading[23]), "voltageReading[23]", "double", 1, 743, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[24]), "voltageReading[24]", "double", 1, 744, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[25]), "voltageReading[25]", "double", 1, 745, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[26]), "voltageReading[26]", "double", 1, 746, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[27]), "voltageReading[27]", "double", 1, 747, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[28]), "voltageReading[28]", "double", 1, 748, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[29]), "voltageReading[29]", "double", 1, 749, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[30]), "voltageReading[30]", "double", 1, 750, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[31]), "voltageReading[31]", "double", 1, 751, 0, 0, 0, 0},
	{"{SYSTEM}:u0:sw_enabled", offsetof(struct UserVar, channelEnabled[0]), "channelEnabled[0]", "double", 1, 752, 0, 0, 0, 0},
	{"{SYSTEM}:u1:sw_enabled", offsetof(struct UserVar, channelEnabled[1]), "channelEnabled[1]", "double", 1, 753, 0, 0, 0, 0},
	{"{SYSTEM}:u2:sw_enabled", offsetof(struct UserVar, channelEnabled[2]), "channelEnabled[2]", "double", 1, 754, 0, 0, 0, 0},
	{"{SYSTEM}:u3:sw_enabled", offsetof(struct UserVar, channelEnabled[3]), "channelEnabled[3]", "double", 1, 755, 0, 0, 0, 0},
	{"{SYSTEM}:u4:sw_enabled", offsetof(struct UserVar, channelEnabled[4]), "channelEnabled[4]", "double", 1, 756, 0, 0, 0, 0},
	{"{SYSTEM}:u5:sw_enabled", offsetof(struct UserVar, channelEnabled[5]), "channelEnabled[5]", "double", 1, 757, 0, 0, 0, 0},
	{"{SYSTEM}:u6:sw_enabled", offsetof(struct UserVar, channelEnabled[6]), "channelEnabled[6]", "double", 1, 758, 0, 0, 0, 0},
	{"{SYSTEM}:u7:sw_enabled", offsetof(struct UserVar, channelEnabled[7]), "channelEnabled[7]", "double", 1, 759, 0, 0, 0, 0},
	{"{SYSTEM}:u100:sw_enabled", offsetof(struct UserVar, channelEnabled[8]), "channelEnabled[8]", "double", 1, 760, 0, 0, 0, 0},
	{"{SYSTEM}:u101:sw_enabled", offsetof(struct UserVar, channelEnabled[9]), "channelEnabled[9]", "double", 1, 761, 0, 0, 0, 0},
	{"{SYSTEM}:u102:sw_enabled", offsetof(struct UserVar, channelEnabled[10]), "channelEnabled[10]", "double", 1, 762, 0, 0, 0, 0},
	{"{SYSTEM}:u103:sw_enabled", offsetof(struct UserVar, channelEnabled[11]), "channelEnabled[11]", "double", 1, 763, 0, 0, 0, 0},
	{"{SYSTEM}:u104:sw_enabled", offsetof(struct UserVar, channelEnabled[12]), "channelEnabled[12]", "double", 1, 764, 0, 0, 0, 0},
	{"{SYSTEM}:u105:sw_enabled", offsetof(struct UserVar, channelEnabled[13]), "channelEnabled[13]", "double", 1, 765, 0, 0, 0, 0},
	{"{SYSTEM}:u106:sw_enabled", offsetof(struct UserVar, channelEnabled[14]), "channelEnabled[14]", "double", 1, 766, 0, 0, 0, 0},
	{"{SYSTEM}:u107:sw_enabled", offsetof(struct UserVar, channelEnabled[15]), "channelEnabled[15]", "double", 1, 767, 0, 0, 0, 0},
	{"{SYSTEM}:u200:sw_enabled", offsetof(struct UserVar, channelEnabled[16]), "channelEnabled[16]", "double", 1, 768, 0, 0, 0, 0},
	{"{SYSTEM}:u201:sw_enabled", offsetof(struct UserVar, channelEnabled[17]), "channelEnabled[17]", "double", 1, 769, 0, 0, 0, 0},
	{"{SYSTEM}:u202:sw_enabled", offsetof(struct UserVar, channelEnabled[18]), "channelEnabled[18]", "double", 1, 770, 0, 0, 0, 0},
	{"{SYSTEM}:u203:sw_enabled", offsetof(struct UserVar, channelEnabled[19]), "channelEnabled[19]", "double", 1, 771, 0, 0, 0, 0},
	{"{SYSTEM}:u204:sw_enabled", offsetof(struct UserVar, channelEnabled[20]), "channelEnabled[20]", "double", 1, 772, 0, 0, 0, 0},
	{"{SYSTEM}:u205:sw_enabled", offsetof(struct UserVar, channelEnabled[21]), "channelEnabled[21]", "double", 1, 773, 0, 0, 0, 0},
	{"{SYSTEM}:u206:sw_enabled", offsetof(struct UserVar, channelEnabled[22]), "channelEnabled[22]", "double", 1, 774, 0, 0, 0, 0},
	{"{SYSTEM}:u207:sw_enabled", offsetof(struct UserVar, channelEnabled[23]), "channelEnabled[23]", "double", 1, 775, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[24]), "channelEnabled[24]", "double", 1, 776, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[25]), "channelEnabled[25]", "double", 1, 777, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[26]), "channelEnabled[26]", "double", 1, 778, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[27]), "channelEnabled[27]", "double", 1, 779, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[28]), "channelEnabled[28]", "double", 1, 780, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[29]), "channelEnabled[29]", "double", 1, 781, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[30]), "channelEnabled[30]", "double", 1, 782, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[31]), "channelEnabled[31]", "double", 1, 783, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u0", offsetof(struct UserVar, tripStatus[0]), "tripStatus[0]", "double", 1, 784, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u1", offsetof(struct UserVar, tripStatus[1]), "tripStatus[1]", "double", 1, 785, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u2", offsetof(struct UserVar, tripStatus[2]), "tripStatus[2]", "double", 1, 786, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u3", offsetof(struct UserVar, tripStatus[3]), "tripStatus[3]", "double", 1, 787, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u4", offsetof(struct UserVar, tripStatus[4]), "tripStatus[4]", "double", 1, 788, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u5", offsetof(struct UserVar, tripStatus[5]), "tripStatus[5]", "double", 1, 789, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u6", offsetof(struct UserVar, tripStatus[6]), "tripStatus[6]", "double", 1, 790, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u7", offsetof(struct UserVar, tripStatus[7]), "tripStatus[7]", "double", 1, 791, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u100", offsetof(struct UserVar, tripStatus[8]), "tripStatus[8]", "double", 1, 792, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u101", offsetof(struct UserVar, tripStatus[9]), "tripStatus[9]", "double", 1, 793, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u102", offsetof(struct UserVar, tripStatus[10]), "tripStatus[10]", "double", 1, 794, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u103", offsetof(struct UserVar, tripStatus[11]), "tripStatus[11]", "double", 1, 795, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u104", offsetof(struct UserVar, tripStatus[12]), "tripStatus[12]", "double", 1, 796, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u105", offsetof(struct UserVar, tripStatus[13]), "tripStatus[13]", "double", 1, 797, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u106", offsetof(struct UserVar, tripStatus[14]), "tripStatus[14]", "double", 1, 798, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u107", offsetof(struct UserVar, tripStatus[15]), "tripStatus[15]", "double", 1, 799, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u200", offsetof(struct UserVar, tripStatus[16]), "tripStatus[16]", "double", 1, 800, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u201", offsetof(struct UserVar, tripStatus[17]), "tripStatus[17]", "double", 1, 801, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u202", offsetof(struct UserVar, tripStatus[18]), "tripStatus[18]", "double", 1, 802, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u203", offsetof(struct UserVar, tripStatus[19]), "tripStatus[19]", "double", 1, 803, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u204", offsetof(struct UserVar, tripStatus[20]), "tripStatus[20]", "double", 1, 804, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u205", offsetof(struct UserVar, tripStatus[21]), "tripStatus[21]", "double", 1, 805, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u206", offsetof(struct UserVar, tripStatus[22]), "tripStatus[22]", "double", 1, 806, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_u207", offsetof(struct UserVar, tripStatus[23]), "tripStatus[23]", "double", 1, 807, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[24]), "tripStatus[24]", "double", 1, 808, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[25]), "tripStatus[25]", "double", 1, 809, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[26]), "tripStatus[26]", "double", 1, 810, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[27]), "tripStatus[27]", "double", 1, 811, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[28]), "tripStatus[28]", "double", 1, 812, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[29]), "tripStatus[29]", "double", 1, 813, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[30]), "tripStatus[30]", "double", 1, 814, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[31]), "tripStatus[31]", "double", 1, 815, 0, 0, 0, 0}
};

/* Event masks for state set "ss1" */
static const seqMask	EM_ss1_0_init[] = {
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};
static const seqMask	EM_ss1_0_pause_then_load[] = {
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};
static const seqMask	EM_ss1_0_waiting[] = {
	0x00000004,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};
static const seqMask	EM_ss1_0_loading_all[] = {
	0x00000004,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};
static const seqMask	EM_ss1_0_saving_all[] = {
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};
static const seqMask	EM_ss1_0_powering_all[] = {
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};
static const seqMask	EM_ss1_0_loading_ramp_all[] = {
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
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
	/* state name */        "pause_then_load",
	/* action function */   A_ss1_0_pause_then_load,
	/* event function */    E_ss1_0_pause_then_load,
	/* delay function */    D_ss1_0_pause_then_load,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_pause_then_load,
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
	/* state name */        "saving_all",
	/* action function */   A_ss1_0_saving_all,
	/* event function */    E_ss1_0_saving_all,
	/* delay function */    D_ss1_0_saving_all,
	/* entry function */    I_ss1_0_saving_all,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_saving_all,
	/* state options */     (0)
	},
	{
	/* state name */        "powering_all",
	/* action function */   A_ss1_0_powering_all,
	/* event function */    E_ss1_0_powering_all,
	/* delay function */    D_ss1_0_powering_all,
	/* entry function */    I_ss1_0_powering_all,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_powering_all,
	/* state options */     (0)
	},
	{
	/* state name */        "loading_ramp_all",
	/* action function */   A_ss1_0_loading_ramp_all,
	/* event function */    E_ss1_0_loading_ramp_all,
	/* delay function */    D_ss1_0_loading_ramp_all,
	/* entry function */    I_ss1_0_loading_ramp_all,
	/* exit function */     0,
	/* event mask array */  EM_ss1_0_loading_ramp_all,
	/* state options */     (0)
	},
};

/* State set table */
static seqSS G_state_sets[] = {
	{
	/* state set name */    "ss1",
	/* states */            G_ss1_states,
	/* number of states */  7,
	/* number of delays */  1
	},
};

/* Program table (global) */
seqProgram sncFGTControl = {
	/* magic number */      2001013,
	/* program name */      "sncFGTControl",
	/* channels */          G_channels,
	/* num. channels */     815,
	/* state sets */        G_state_sets,
	/* num. state sets */   1,
	/* user var size */     sizeof(struct UserVar),
	/* param */             "",
	/* num. event flags */  0,
	/* encoded options */   (0 | OPT_CONN | OPT_NEWEF | OPT_REENT),
	/* init func */         G_prog_init,
	/* entry func */        NULL,
	/* exit func */         NULL,
	/* num. queues */       0
};

/* Register sequencer commands and program */
#include "epicsExport.h"
static void sncFGTControlRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&sncFGTControl);
}
epicsExportRegistrar(sncFGTControlRegistrar);
