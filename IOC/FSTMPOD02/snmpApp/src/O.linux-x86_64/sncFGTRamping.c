/* Generated with snc from ../sncFGTRamping.stt */

/* Program "sncFGTRamping" */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include "epicsTypes.h"
#include "seqCom.h"

/* Variable declarations */
struct UserVar {
# line 10 "../sncFGTRamping.stt"
	long chan;
# line 11 "../sncFGTRamping.stt"
	int v;
# line 12 "../sncFGTRamping.stt"
	long i;
# line 13 "../sncFGTRamping.stt"
	long j;
# line 14 "../sncFGTRamping.stt"
	long doneRampingToStandby;
# line 15 "../sncFGTRamping.stt"
	long endRamp;
# line 16 "../sncFGTRamping.stt"
	int control;
# line 17 "../sncFGTRamping.stt"
	int power;
# line 18 "../sncFGTRamping.stt"
	long real_power_state;
# line 30 "../sncFGTRamping.stt"
	double targetVoltage[32];
# line 31 "../sncFGTRamping.stt"
	double physicsVoltageSetting[32];
# line 32 "../sncFGTRamping.stt"
	double standbyVoltageSetting[32];
# line 33 "../sncFGTRamping.stt"
	double physicsRampRate[32];
# line 34 "../sncFGTRamping.stt"
	double standbyRampRate[32];
# line 35 "../sncFGTRamping.stt"
	double conditioningDelay[32];
# line 36 "../sncFGTRamping.stt"
	double conditioningVoltageStep[32];
# line 38 "../sncFGTRamping.stt"
	double voltageSetting[32];
# line 39 "../sncFGTRamping.stt"
	double voltageReading[32];
# line 40 "../sncFGTRamping.stt"
	double powerSwitch[32];
# line 41 "../sncFGTRamping.stt"
	double channelEnabled[32];
# line 42 "../sncFGTRamping.stt"
	double currentSetting[32];
# line 43 "../sncFGTRamping.stt"
	double rampRate[32];
# line 44 "../sncFGTRamping.stt"
	double autoResetDelay[32];
# line 45 "../sncFGTRamping.stt"
	double autoResetEnabled[32];
# line 46 "../sncFGTRamping.stt"
	double maxCycles[32];
# line 47 "../sncFGTRamping.stt"
	double tripStatus[32];
# line 49 "../sncFGTRamping.stt"
	string statusText;
};

/* C code definitions */
# line 2 "../sncFGTRamping.stt"


#include <stdio.h>
#include <string.h>



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
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		"",
	};
	*pVar = pVarInit;
}

/****** Code for state "init" in state set "power_status" ******/

/* Delay function for state "init" in state set "power_status" */
static void D_power_status_0_init(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "init" in state set "power_status" */
static seqBool E_power_status_0_init(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 192 "../sncFGTRamping.stt"
	if (pVar->power == 0)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
# line 194 "../sncFGTRamping.stt"
	if (pVar->power == 1)
	{
		*pNextState = 1;
		*pTransNum = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "init" in state set "power_status" */
static void A_power_status_0_init(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "on" in state set "power_status" ******/

/* Entry function for state "on" in state set "power_status" */
static void I_power_status_0_on(SS_ID ssId, struct UserVar *pVar)
{
# line 200 "../sncFGTRamping.stt"
	pVar->real_power_state = 1;
}

/* Delay function for state "on" in state set "power_status" */
static void D_power_status_0_on(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "on" in state set "power_status" */
static seqBool E_power_status_0_on(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 202 "../sncFGTRamping.stt"
	if (pVar->power == 0)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "on" in state set "power_status" */
static void A_power_status_0_on(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "off" in state set "power_status" ******/

/* Entry function for state "off" in state set "power_status" */
static void I_power_status_0_off(SS_ID ssId, struct UserVar *pVar)
{
# line 207 "../sncFGTRamping.stt"
	pVar->real_power_state = 0;
}

/* Delay function for state "off" in state set "power_status" */
static void D_power_status_0_off(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "off" in state set "power_status" */
static seqBool E_power_status_0_off(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 209 "../sncFGTRamping.stt"
	if (pVar->power == 1)
	{
		*pNextState = 5;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "off" in state set "power_status" */
static void A_power_status_0_off(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "check_off_level1" in state set "power_status" ******/

/* Delay function for state "check_off_level1" in state set "power_status" */
static void D_power_status_0_check_off_level1(SS_ID ssId, struct UserVar *pVar)
{
# line 215 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 0, (7));
}

/* Event function for state "check_off_level1" in state set "power_status" */
static seqBool E_power_status_0_check_off_level1(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 213 "../sncFGTRamping.stt"
	if (pVar->power == 1)
	{
		*pNextState = 5;
		*pTransNum = 0;
		return TRUE;
	}
# line 215 "../sncFGTRamping.stt"
	if (pVar->power == 0 && seq_delay(ssId, 0))
	{
		*pNextState = 4;
		*pTransNum = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "check_off_level1" in state set "power_status" */
static void A_power_status_0_check_off_level1(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "check_off_level2" in state set "power_status" ******/

/* Delay function for state "check_off_level2" in state set "power_status" */
static void D_power_status_0_check_off_level2(SS_ID ssId, struct UserVar *pVar)
{
# line 221 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 0, (7));
}

/* Event function for state "check_off_level2" in state set "power_status" */
static seqBool E_power_status_0_check_off_level2(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 219 "../sncFGTRamping.stt"
	if (pVar->power == 1)
	{
		*pNextState = 5;
		*pTransNum = 0;
		return TRUE;
	}
# line 221 "../sncFGTRamping.stt"
	if (pVar->power == 0 && seq_delay(ssId, 0))
	{
		*pNextState = 2;
		*pTransNum = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "check_off_level2" in state set "power_status" */
static void A_power_status_0_check_off_level2(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "check_on_level1" in state set "power_status" ******/

/* Delay function for state "check_on_level1" in state set "power_status" */
static void D_power_status_0_check_on_level1(SS_ID ssId, struct UserVar *pVar)
{
# line 227 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 0, (7));
}

/* Event function for state "check_on_level1" in state set "power_status" */
static seqBool E_power_status_0_check_on_level1(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 225 "../sncFGTRamping.stt"
	if (pVar->power == 0)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
# line 227 "../sncFGTRamping.stt"
	if (pVar->power == 1 && seq_delay(ssId, 0))
	{
		*pNextState = 1;
		*pTransNum = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "check_on_level1" in state set "power_status" */
static void A_power_status_0_check_on_level1(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "init" in state set "ramp1" ******/

/* Entry function for state "init" in state set "ramp1" */
static void I_ramp1_1_init(SS_ID ssId, struct UserVar *pVar)
{
}

/* Delay function for state "init" in state set "ramp1" */
static void D_ramp1_1_init(SS_ID ssId, struct UserVar *pVar)
{
# line 238 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 0, (10));
}

/* Event function for state "init" in state set "ramp1" */
static seqBool E_ramp1_1_init(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 238 "../sncFGTRamping.stt"
	if (seq_delay(ssId, 0))
	{
		*pNextState = 1;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "init" in state set "ramp1" */
static void A_ramp1_1_init(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
# line 240 "../sncFGTRamping.stt"
			for (pVar->chan = 0; pVar->chan < 8; pVar->chan++)
			{
# line 241 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 228/*voltageSetting*/ + (VAR_ID)(pVar->chan));
# line 242 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 260/*voltageReading*/ + (VAR_ID)(pVar->chan));
# line 243 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 452/*channelEnabled*/ + (VAR_ID)(pVar->chan));
# line 244 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 484/*powerSwitch*/ + (VAR_ID)(pVar->chan));
# line 245 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 292/*currentSetting*/ + (VAR_ID)(pVar->chan));
# line 246 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 324/*rampRate*/ + (VAR_ID)(pVar->chan));
# line 247 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 356/*autoResetDelay*/ + (VAR_ID)(pVar->chan));
# line 248 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 388/*autoResetEnabled*/ + (VAR_ID)(pVar->chan));
# line 249 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 420/*maxCycles*/ + (VAR_ID)(pVar->chan));
# line 250 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 516/*tripStatus*/ + (VAR_ID)(pVar->chan));
# line 251 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 36/*physicsVoltageSetting*/ + (VAR_ID)(pVar->chan));
# line 252 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 68/*standbyVoltageSetting*/ + (VAR_ID)(pVar->chan));
# line 253 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 100/*physicsRampRate*/ + (VAR_ID)(pVar->chan));
# line 254 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 132/*standbyRampRate*/ + (VAR_ID)(pVar->chan));
# line 255 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 164/*conditioningDelay*/ + (VAR_ID)(pVar->chan));
# line 256 "../sncFGTRamping.stt"
				seq_pvMonitor(ssId, 196/*conditioningVoltageStep*/ + (VAR_ID)(pVar->chan));
			}
		}
		return;
	}
}

/****** Code for state "set_entry_state" in state set "ramp1" ******/

/* Entry function for state "set_entry_state" in state set "ramp1" */
static void I_ramp1_1_set_entry_state(SS_ID ssId, struct UserVar *pVar)
{
# line 262 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "UNKNOWN");
# line 263 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
# line 264 "../sncFGTRamping.stt"
	pVar->control = 0;
# line 265 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 0/*control*/, 0);
}

/* Delay function for state "set_entry_state" in state set "ramp1" */
static void D_ramp1_1_set_entry_state(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "set_entry_state" in state set "ramp1" */
static seqBool E_ramp1_1_set_entry_state(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 267 "../sncFGTRamping.stt"
	if (pVar->control == 1)
	{
		*pNextState = 13;
		*pTransNum = 0;
		return TRUE;
	}
# line 269 "../sncFGTRamping.stt"
	if (pVar->control == 2)
	{
		*pNextState = 14;
		*pTransNum = 1;
		return TRUE;
	}
# line 271 "../sncFGTRamping.stt"
	if (pVar->control == 3)
	{
		*pNextState = 15;
		*pTransNum = 2;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "set_entry_state" in state set "ramp1" */
static void A_ramp1_1_set_entry_state(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "prepare_to_ramp" in state set "ramp1" ******/

/* Entry function for state "prepare_to_ramp" in state set "ramp1" */
static void I_ramp1_1_prepare_to_ramp(SS_ID ssId, struct UserVar *pVar)
{
}

/* Delay function for state "prepare_to_ramp" in state set "ramp1" */
static void D_ramp1_1_prepare_to_ramp(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "prepare_to_ramp" in state set "ramp1" */
static seqBool E_ramp1_1_prepare_to_ramp(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 278 "../sncFGTRamping.stt"
	if (pVar->real_power_state == 0)
	{
		*pNextState = 3;
		*pTransNum = 0;
		return TRUE;
	}
# line 283 "../sncFGTRamping.stt"
	if (pVar->control == 1)
	{
		*pNextState = 3;
		*pTransNum = 1;
		return TRUE;
	}
# line 286 "../sncFGTRamping.stt"
	if (pVar->control == 2)
	{
		*pNextState = 4;
		*pTransNum = 2;
		return TRUE;
	}
# line 289 "../sncFGTRamping.stt"
	if (pVar->control == 3)
	{
		*pNextState = 8;
		*pTransNum = 3;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "prepare_to_ramp" in state set "ramp1" */
static void A_ramp1_1_prepare_to_ramp(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
# line 280 "../sncFGTRamping.stt"
			pVar->control = 1;
# line 281 "../sncFGTRamping.stt"
			seq_pvPut(ssId, 0/*control*/, SYNC);
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

/****** Code for state "set_to_off" in state set "ramp1" ******/

/* Entry function for state "set_to_off" in state set "ramp1" */
static void I_ramp1_1_set_to_off(SS_ID ssId, struct UserVar *pVar)
{
# line 296 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "Turning Off");
# line 297 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
# line 298 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 299 "../sncFGTRamping.stt"
		pVar->powerSwitch[pVar->i] = 0;
# line 300 "../sncFGTRamping.stt"
		seq_pvPut(ssId, 484/*powerSwitch*/ + (VAR_ID)(pVar->i), 0);
	}
# line 302 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 303 "../sncFGTRamping.stt"
		pVar->rampRate[pVar->i] = pVar->standbyRampRate[0];
# line 304 "../sncFGTRamping.stt"
		seq_pvPut(ssId, 324/*rampRate*/ + (VAR_ID)(pVar->i), 0);
	}
}

/* Delay function for state "set_to_off" in state set "ramp1" */
static void D_ramp1_1_set_to_off(SS_ID ssId, struct UserVar *pVar)
{
# line 307 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 0, (5));
}

/* Event function for state "set_to_off" in state set "ramp1" */
static seqBool E_ramp1_1_set_to_off(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 307 "../sncFGTRamping.stt"
	if (1 && seq_delay(ssId, 0))
	{
		*pNextState = 13;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "set_to_off" in state set "ramp1" */
static void A_ramp1_1_set_to_off(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
# line 308 "../sncFGTRamping.stt"
			for (pVar->i = 0; pVar->i < 8; pVar->i++)
			{
# line 309 "../sncFGTRamping.stt"
				pVar->targetVoltage[pVar->i] = 0.0;
# line 310 "../sncFGTRamping.stt"
				pVar->voltageSetting[pVar->i] = 0.0;
# line 311 "../sncFGTRamping.stt"
				seq_pvPut(ssId, 228/*voltageSetting*/ + (VAR_ID)(pVar->i), SYNC);
# line 312 "../sncFGTRamping.stt"
				pVar->rampRate[pVar->i] = pVar->standbyRampRate[0];
# line 313 "../sncFGTRamping.stt"
				seq_pvPut(ssId, 324/*rampRate*/ + (VAR_ID)(pVar->i), SYNC);
			}
		}
		return;
	}
}

/****** Code for state "ramp_to_standby" in state set "ramp1" ******/

/* Entry function for state "ramp_to_standby" in state set "ramp1" */
static void I_ramp1_1_ramp_to_standby(SS_ID ssId, struct UserVar *pVar)
{
# line 320 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "Ramping to Standby");
# line 321 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
# line 322 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 323 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->tripStatus[pVar->i] != 4)
		{
# line 324 "../sncFGTRamping.stt"
			if (pVar->voltageSetting[pVar->i] < pVar->standbyVoltageSetting[pVar->i])
			{
# line 325 "../sncFGTRamping.stt"
				pVar->targetVoltage[pVar->i] = ((pVar->voltageSetting[pVar->i] / pVar->conditioningVoltageStep[pVar->i]) + 1) * pVar->conditioningVoltageStep[pVar->i];
# line 326 "../sncFGTRamping.stt"
				if (pVar->targetVoltage[pVar->i] > pVar->standbyVoltageSetting[pVar->i])
				{
# line 327 "../sncFGTRamping.stt"
					pVar->targetVoltage[pVar->i] = pVar->standbyVoltageSetting[pVar->i];
				}
			}
			else
# line 329 "../sncFGTRamping.stt"
				if (pVar->voltageSetting[pVar->i] > pVar->standbyVoltageSetting[pVar->i])
				{
# line 330 "../sncFGTRamping.stt"
					pVar->targetVoltage[pVar->i] = pVar->standbyVoltageSetting[pVar->i];
				}
		}
		else
		{
# line 333 "../sncFGTRamping.stt"
			pVar->targetVoltage[pVar->i] = 0;
		}
# line 335 "../sncFGTRamping.stt"
		pVar->voltageSetting[pVar->i] = pVar->targetVoltage[pVar->i];
# line 336 "../sncFGTRamping.stt"
		seq_pvPut(ssId, 228/*voltageSetting*/ + (VAR_ID)(pVar->i), SYNC);
	}
# line 338 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 339 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->tripStatus[pVar->i] != 4)
		{
# line 340 "../sncFGTRamping.stt"
			pVar->powerSwitch[pVar->i] = 1;
# line 341 "../sncFGTRamping.stt"
			seq_pvPut(ssId, 484/*powerSwitch*/ + (VAR_ID)(pVar->i), SYNC);
		}
	}
# line 344 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 345 "../sncFGTRamping.stt"
		pVar->rampRate[pVar->i] = pVar->standbyRampRate[0];
# line 346 "../sncFGTRamping.stt"
		seq_pvPut(ssId, 324/*rampRate*/ + (VAR_ID)(pVar->i), 0);
	}
}

/* Delay function for state "ramp_to_standby" in state set "ramp1" */
static void D_ramp1_1_ramp_to_standby(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "ramp_to_standby" in state set "ramp1" */
static seqBool E_ramp1_1_ramp_to_standby(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 349 "../sncFGTRamping.stt"
	if (1)
	{
		*pNextState = 5;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "ramp_to_standby" in state set "ramp1" */
static void A_ramp1_1_ramp_to_standby(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "standby_ramping_delay" in state set "ramp1" ******/

/* Entry function for state "standby_ramping_delay" in state set "ramp1" */
static void I_ramp1_1_standby_ramping_delay(SS_ID ssId, struct UserVar *pVar)
{
}

/* Delay function for state "standby_ramping_delay" in state set "ramp1" */
static void D_ramp1_1_standby_ramping_delay(SS_ID ssId, struct UserVar *pVar)
{
# line 358 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 0, (5));
}

/* Event function for state "standby_ramping_delay" in state set "ramp1" */
static seqBool E_ramp1_1_standby_ramping_delay(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 358 "../sncFGTRamping.stt"
	if (seq_delay(ssId, 0))
	{
		*pNextState = 6;
		*pTransNum = 0;
		return TRUE;
	}
# line 360 "../sncFGTRamping.stt"
	if (pVar->control != 3 && pVar->control != 2)
	{
		*pNextState = 2;
		*pTransNum = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "standby_ramping_delay" in state set "ramp1" */
static void A_ramp1_1_standby_ramping_delay(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "check_standby_rampup" in state set "ramp1" ******/

/* Entry function for state "check_standby_rampup" in state set "ramp1" */
static void I_ramp1_1_check_standby_rampup(SS_ID ssId, struct UserVar *pVar)
{
# line 366 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "Ramping");
# line 367 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
# line 368 "../sncFGTRamping.stt"
	pVar->endRamp = 1;
# line 369 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 370 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->tripStatus[pVar->i] != 4)
		{
# line 371 "../sncFGTRamping.stt"
			if (pVar->targetVoltage[pVar->i] != 0)
			{
# line 373 "../sncFGTRamping.stt"
				if (pVar->voltageReading[pVar->i] / pVar->targetVoltage[pVar->i] < 0.97 || pVar->voltageReading[pVar->i] / pVar->targetVoltage[pVar->i] > 1.03)
				{
# line 374 "../sncFGTRamping.stt"
					pVar->endRamp = 0;
				}
			}
		}
	}
}

/* Delay function for state "check_standby_rampup" in state set "ramp1" */
static void D_ramp1_1_check_standby_rampup(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "check_standby_rampup" in state set "ramp1" */
static seqBool E_ramp1_1_check_standby_rampup(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 380 "../sncFGTRamping.stt"
	if (pVar->control != 2 && pVar->control != 3 || pVar->real_power_state == 0)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
# line 382 "../sncFGTRamping.stt"
	if (pVar->endRamp == 1)
	{
		*pNextState = 7;
		*pTransNum = 1;
		return TRUE;
	}
# line 384 "../sncFGTRamping.stt"
	if (pVar->endRamp == 0)
	{
		*pNextState = 5;
		*pTransNum = 2;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "check_standby_rampup" in state set "ramp1" */
static void A_ramp1_1_check_standby_rampup(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "standby_ramping_step_finished" in state set "ramp1" ******/

/* Entry function for state "standby_ramping_step_finished" in state set "ramp1" */
static void I_ramp1_1_standby_ramping_step_finished(SS_ID ssId, struct UserVar *pVar)
{
# line 390 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "Ramp Step Finished");
# line 391 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
# line 392 "../sncFGTRamping.stt"
	pVar->endRamp = 1;
# line 393 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 394 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->tripStatus[pVar->i] != 4)
		{
# line 395 "../sncFGTRamping.stt"
			if (pVar->targetVoltage[pVar->i] != pVar->standbyVoltageSetting[pVar->i])
			{
# line 396 "../sncFGTRamping.stt"
				pVar->endRamp = 0;
			}
		}
	}
}

/* Delay function for state "standby_ramping_step_finished" in state set "ramp1" */
static void D_ramp1_1_standby_ramping_step_finished(SS_ID ssId, struct UserVar *pVar)
{
# line 403 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 0, (pVar->conditioningDelay[0]));
# line 405 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 1, (pVar->conditioningDelay[0]));
}

/* Event function for state "standby_ramping_step_finished" in state set "ramp1" */
static seqBool E_ramp1_1_standby_ramping_step_finished(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 401 "../sncFGTRamping.stt"
	if (pVar->control == 2 && pVar->endRamp == 1)
	{
		*pNextState = 14;
		*pTransNum = 0;
		return TRUE;
	}
# line 403 "../sncFGTRamping.stt"
	if (seq_delay(ssId, 0) && pVar->endRamp == 1)
	{
		*pNextState = 14;
		*pTransNum = 1;
		return TRUE;
	}
# line 405 "../sncFGTRamping.stt"
	if (seq_delay(ssId, 1) && pVar->endRamp == 0)
	{
		*pNextState = 4;
		*pTransNum = 2;
		return TRUE;
	}
# line 407 "../sncFGTRamping.stt"
	if (pVar->control != 2 && pVar->control != 3 || pVar->real_power_state == 0)
	{
		*pNextState = 2;
		*pTransNum = 3;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "standby_ramping_step_finished" in state set "ramp1" */
static void A_ramp1_1_standby_ramping_step_finished(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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

/****** Code for state "ramp_to_physics" in state set "ramp1" ******/

/* Entry function for state "ramp_to_physics" in state set "ramp1" */
static void I_ramp1_1_ramp_to_physics(SS_ID ssId, struct UserVar *pVar)
{
# line 414 "../sncFGTRamping.stt"
	pVar->doneRampingToStandby = 1;
# line 415 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 416 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->tripStatus[pVar->i] != 4)
		{
# line 417 "../sncFGTRamping.stt"
			if (pVar->voltageSetting[pVar->i] < pVar->standbyVoltageSetting[pVar->i])
			{
# line 418 "../sncFGTRamping.stt"
				pVar->doneRampingToStandby = 0;
			}
		}
	}
}

/* Delay function for state "ramp_to_physics" in state set "ramp1" */
static void D_ramp1_1_ramp_to_physics(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "ramp_to_physics" in state set "ramp1" */
static seqBool E_ramp1_1_ramp_to_physics(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 423 "../sncFGTRamping.stt"
	if (pVar->doneRampingToStandby == 1)
	{
		*pNextState = 9;
		*pTransNum = 0;
		return TRUE;
	}
# line 425 "../sncFGTRamping.stt"
	if (pVar->doneRampingToStandby == 0)
	{
		*pNextState = 4;
		*pTransNum = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "ramp_to_physics" in state set "ramp1" */
static void A_ramp1_1_ramp_to_physics(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "begin_physics_ramp" in state set "ramp1" ******/

/* Entry function for state "begin_physics_ramp" in state set "ramp1" */
static void I_ramp1_1_begin_physics_ramp(SS_ID ssId, struct UserVar *pVar)
{
# line 431 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "Ramping to Physics");
# line 432 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
# line 433 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 434 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->tripStatus[pVar->i] != 4)
		{
# line 435 "../sncFGTRamping.stt"
			if (pVar->voltageSetting[pVar->i] < pVar->physicsVoltageSetting[pVar->i])
			{
# line 436 "../sncFGTRamping.stt"
				pVar->targetVoltage[pVar->i] = ((pVar->voltageSetting[pVar->i] / pVar->conditioningVoltageStep[pVar->i]) + 1) * pVar->conditioningVoltageStep[pVar->i];
# line 437 "../sncFGTRamping.stt"
				if (pVar->targetVoltage[pVar->i] > pVar->physicsVoltageSetting[pVar->i])
				{
# line 438 "../sncFGTRamping.stt"
					pVar->targetVoltage[pVar->i] = pVar->physicsVoltageSetting[pVar->i];
				}
			}
			else
# line 440 "../sncFGTRamping.stt"
				if (pVar->voltageSetting[pVar->i] > pVar->physicsVoltageSetting[pVar->i])
				{
# line 441 "../sncFGTRamping.stt"
					pVar->targetVoltage[pVar->i] = pVar->physicsVoltageSetting[pVar->i];
				}
		}
		else
		{
# line 444 "../sncFGTRamping.stt"
			pVar->targetVoltage[pVar->i] = 0;
		}
# line 446 "../sncFGTRamping.stt"
		pVar->voltageSetting[pVar->i] = pVar->targetVoltage[pVar->i];
# line 447 "../sncFGTRamping.stt"
		seq_pvPut(ssId, 228/*voltageSetting*/ + (VAR_ID)(pVar->i), 0);
# line 448 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->tripStatus[pVar->i] != 4)
		{
# line 449 "../sncFGTRamping.stt"
			if ((pVar->physicsVoltageSetting[pVar->i] - pVar->targetVoltage[pVar->i]) <= pVar->conditioningVoltageStep[pVar->i])
			{
# line 450 "../sncFGTRamping.stt"
				pVar->rampRate[pVar->i] = pVar->physicsRampRate[0];
			}
			else
			{
# line 452 "../sncFGTRamping.stt"
				pVar->rampRate[pVar->i] = pVar->standbyRampRate[0];
			}
# line 454 "../sncFGTRamping.stt"
			seq_pvPut(ssId, 324/*rampRate*/ + (VAR_ID)(pVar->i), 0);
		}
	}
# line 457 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 458 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->powerSwitch[pVar->i] == 0 && pVar->tripStatus[pVar->i] != 4)
		{
# line 459 "../sncFGTRamping.stt"
			pVar->powerSwitch[pVar->i] = 1;
# line 460 "../sncFGTRamping.stt"
			seq_pvPut(ssId, 484/*powerSwitch*/ + (VAR_ID)(pVar->i), 0);
		}
	}
}

/* Delay function for state "begin_physics_ramp" in state set "ramp1" */
static void D_ramp1_1_begin_physics_ramp(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "begin_physics_ramp" in state set "ramp1" */
static seqBool E_ramp1_1_begin_physics_ramp(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 464 "../sncFGTRamping.stt"
	if (1)
	{
		*pNextState = 10;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "begin_physics_ramp" in state set "ramp1" */
static void A_ramp1_1_begin_physics_ramp(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "physics_ramping_delay" in state set "ramp1" ******/

/* Entry function for state "physics_ramping_delay" in state set "ramp1" */
static void I_ramp1_1_physics_ramping_delay(SS_ID ssId, struct UserVar *pVar)
{
}

/* Delay function for state "physics_ramping_delay" in state set "ramp1" */
static void D_ramp1_1_physics_ramping_delay(SS_ID ssId, struct UserVar *pVar)
{
# line 472 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 0, (5));
}

/* Event function for state "physics_ramping_delay" in state set "ramp1" */
static seqBool E_ramp1_1_physics_ramping_delay(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 472 "../sncFGTRamping.stt"
	if (seq_delay(ssId, 0))
	{
		*pNextState = 11;
		*pTransNum = 0;
		return TRUE;
	}
# line 474 "../sncFGTRamping.stt"
	if (pVar->control != 3 || pVar->real_power_state == 0)
	{
		*pNextState = 2;
		*pTransNum = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "physics_ramping_delay" in state set "ramp1" */
static void A_ramp1_1_physics_ramping_delay(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "check_physics_rampup" in state set "ramp1" ******/

/* Entry function for state "check_physics_rampup" in state set "ramp1" */
static void I_ramp1_1_check_physics_rampup(SS_ID ssId, struct UserVar *pVar)
{
# line 480 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "Ramping");
# line 481 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
# line 482 "../sncFGTRamping.stt"
	pVar->endRamp = 1;
# line 483 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 484 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->tripStatus[pVar->i] != 4)
		{
# line 485 "../sncFGTRamping.stt"
			if (pVar->targetVoltage[pVar->i] != 0)
			{
# line 487 "../sncFGTRamping.stt"
				if (pVar->voltageReading[pVar->i] / pVar->targetVoltage[pVar->i] < 0.97)
				{
# line 488 "../sncFGTRamping.stt"
					pVar->endRamp = 0;
				}
			}
		}
	}
}

/* Delay function for state "check_physics_rampup" in state set "ramp1" */
static void D_ramp1_1_check_physics_rampup(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "check_physics_rampup" in state set "ramp1" */
static seqBool E_ramp1_1_check_physics_rampup(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 494 "../sncFGTRamping.stt"
	if (pVar->control != 2 && pVar->control != 3 || pVar->real_power_state == 0)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
# line 496 "../sncFGTRamping.stt"
	if (pVar->endRamp == 1)
	{
		*pNextState = 12;
		*pTransNum = 1;
		return TRUE;
	}
# line 498 "../sncFGTRamping.stt"
	if (pVar->endRamp == 0)
	{
		*pNextState = 10;
		*pTransNum = 2;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "check_physics_rampup" in state set "ramp1" */
static void A_ramp1_1_check_physics_rampup(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "physics_ramping_step_finished" in state set "ramp1" ******/

/* Entry function for state "physics_ramping_step_finished" in state set "ramp1" */
static void I_ramp1_1_physics_ramping_step_finished(SS_ID ssId, struct UserVar *pVar)
{
# line 504 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "Ramp Step Finished");
# line 505 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
# line 506 "../sncFGTRamping.stt"
	pVar->endRamp = 1;
# line 507 "../sncFGTRamping.stt"
	for (pVar->i = 0; pVar->i < 8; pVar->i++)
	{
# line 508 "../sncFGTRamping.stt"
		if (pVar->channelEnabled[pVar->i] == 1 && pVar->tripStatus[pVar->i] != 4)
		{
# line 509 "../sncFGTRamping.stt"
			if (pVar->targetVoltage[pVar->i] != pVar->physicsVoltageSetting[pVar->i])
			{
# line 510 "../sncFGTRamping.stt"
				pVar->endRamp = 0;
			}
		}
	}
}

/* Delay function for state "physics_ramping_step_finished" in state set "ramp1" */
static void D_ramp1_1_physics_ramping_step_finished(SS_ID ssId, struct UserVar *pVar)
{
# line 517 "../sncFGTRamping.stt"
	seq_delayInit(ssId, 0, (pVar->conditioningDelay[0]));
}

/* Event function for state "physics_ramping_step_finished" in state set "ramp1" */
static seqBool E_ramp1_1_physics_ramping_step_finished(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 515 "../sncFGTRamping.stt"
	if (pVar->endRamp == 1)
	{
		*pNextState = 15;
		*pTransNum = 0;
		return TRUE;
	}
# line 517 "../sncFGTRamping.stt"
	if (seq_delay(ssId, 0) && pVar->endRamp == 0)
	{
		*pNextState = 9;
		*pTransNum = 1;
		return TRUE;
	}
# line 519 "../sncFGTRamping.stt"
	if (pVar->control != 2 && pVar->control != 3 || pVar->real_power_state == 0)
	{
		*pNextState = 2;
		*pTransNum = 2;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "physics_ramping_step_finished" in state set "ramp1" */
static void A_ramp1_1_physics_ramping_step_finished(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	}
}

/****** Code for state "off" in state set "ramp1" ******/

/* Entry function for state "off" in state set "ramp1" */
static void I_ramp1_1_off(SS_ID ssId, struct UserVar *pVar)
{
# line 525 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "OFF");
# line 526 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
}

/* Delay function for state "off" in state set "ramp1" */
static void D_ramp1_1_off(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "off" in state set "ramp1" */
static seqBool E_ramp1_1_off(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 529 "../sncFGTRamping.stt"
	if (pVar->control != 1)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "off" in state set "ramp1" */
static void A_ramp1_1_off(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "standby" in state set "ramp1" ******/

/* Entry function for state "standby" in state set "ramp1" */
static void I_ramp1_1_standby(SS_ID ssId, struct UserVar *pVar)
{
# line 534 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "STANDBY");
# line 535 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
}

/* Delay function for state "standby" in state set "ramp1" */
static void D_ramp1_1_standby(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "standby" in state set "ramp1" */
static seqBool E_ramp1_1_standby(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 538 "../sncFGTRamping.stt"
	if (pVar->control != 2 || pVar->real_power_state == 0)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "standby" in state set "ramp1" */
static void A_ramp1_1_standby(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
{
	switch(transNum)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "physics" in state set "ramp1" ******/

/* Entry function for state "physics" in state set "ramp1" */
static void I_ramp1_1_physics(SS_ID ssId, struct UserVar *pVar)
{
# line 543 "../sncFGTRamping.stt"
	sprintf(pVar->statusText, "PHYSICS");
# line 544 "../sncFGTRamping.stt"
	seq_pvPut(ssId, 3/*statusText*/, 0);
}

/* Delay function for state "physics" in state set "ramp1" */
static void D_ramp1_1_physics(SS_ID ssId, struct UserVar *pVar)
{
}

/* Event function for state "physics" in state set "ramp1" */
static seqBool E_ramp1_1_physics(SS_ID ssId, struct UserVar *pVar, int *pTransNum, int *pNextState)
{
# line 547 "../sncFGTRamping.stt"
	if (pVar->control != 3 || pVar->real_power_state == 0)
	{
		*pNextState = 2;
		*pTransNum = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "physics" in state set "ramp1" */
static void A_ramp1_1_physics(SS_ID ssId, struct UserVar *pVar, int transNum, int *pNextState)
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
	{"{SYSTEM}:pwr_reading", offsetof(struct UserVar, power), "power", "int", 1, 3, 0, 1, 0, 0},
	{"{SYSTEM}:{SLOT}0:ramp_state_text", offsetof(struct UserVar, statusText), "statusText", "string", 1, 4, 0, 1, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}0", offsetof(struct UserVar, targetVoltage[0]), "targetVoltage[0]", "double", 1, 5, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}1", offsetof(struct UserVar, targetVoltage[1]), "targetVoltage[1]", "double", 1, 6, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}2", offsetof(struct UserVar, targetVoltage[2]), "targetVoltage[2]", "double", 1, 7, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}3", offsetof(struct UserVar, targetVoltage[3]), "targetVoltage[3]", "double", 1, 8, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}4", offsetof(struct UserVar, targetVoltage[4]), "targetVoltage[4]", "double", 1, 9, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}5", offsetof(struct UserVar, targetVoltage[5]), "targetVoltage[5]", "double", 1, 10, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}6", offsetof(struct UserVar, targetVoltage[6]), "targetVoltage[6]", "double", 1, 11, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}7", offsetof(struct UserVar, targetVoltage[7]), "targetVoltage[7]", "double", 1, 12, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[8]), "targetVoltage[8]", "double", 1, 13, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[9]), "targetVoltage[9]", "double", 1, 14, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[10]), "targetVoltage[10]", "double", 1, 15, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[11]), "targetVoltage[11]", "double", 1, 16, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[12]), "targetVoltage[12]", "double", 1, 17, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[13]), "targetVoltage[13]", "double", 1, 18, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[14]), "targetVoltage[14]", "double", 1, 19, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[15]), "targetVoltage[15]", "double", 1, 20, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[16]), "targetVoltage[16]", "double", 1, 21, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[17]), "targetVoltage[17]", "double", 1, 22, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[18]), "targetVoltage[18]", "double", 1, 23, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[19]), "targetVoltage[19]", "double", 1, 24, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[20]), "targetVoltage[20]", "double", 1, 25, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[21]), "targetVoltage[21]", "double", 1, 26, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[22]), "targetVoltage[22]", "double", 1, 27, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[23]), "targetVoltage[23]", "double", 1, 28, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[24]), "targetVoltage[24]", "double", 1, 29, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[25]), "targetVoltage[25]", "double", 1, 30, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[26]), "targetVoltage[26]", "double", 1, 31, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[27]), "targetVoltage[27]", "double", 1, 32, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[28]), "targetVoltage[28]", "double", 1, 33, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[29]), "targetVoltage[29]", "double", 1, 34, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[30]), "targetVoltage[30]", "double", 1, 35, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, targetVoltage[31]), "targetVoltage[31]", "double", 1, 36, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}0", offsetof(struct UserVar, physicsVoltageSetting[0]), "physicsVoltageSetting[0]", "double", 1, 37, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}1", offsetof(struct UserVar, physicsVoltageSetting[1]), "physicsVoltageSetting[1]", "double", 1, 38, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}2", offsetof(struct UserVar, physicsVoltageSetting[2]), "physicsVoltageSetting[2]", "double", 1, 39, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}3", offsetof(struct UserVar, physicsVoltageSetting[3]), "physicsVoltageSetting[3]", "double", 1, 40, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}4", offsetof(struct UserVar, physicsVoltageSetting[4]), "physicsVoltageSetting[4]", "double", 1, 41, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}5", offsetof(struct UserVar, physicsVoltageSetting[5]), "physicsVoltageSetting[5]", "double", 1, 42, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}6", offsetof(struct UserVar, physicsVoltageSetting[6]), "physicsVoltageSetting[6]", "double", 1, 43, 0, 0, 0, 0},
	{"{SYSTEM}:physics_voltage_{SLOT}7", offsetof(struct UserVar, physicsVoltageSetting[7]), "physicsVoltageSetting[7]", "double", 1, 44, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[8]), "physicsVoltageSetting[8]", "double", 1, 45, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[9]), "physicsVoltageSetting[9]", "double", 1, 46, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[10]), "physicsVoltageSetting[10]", "double", 1, 47, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[11]), "physicsVoltageSetting[11]", "double", 1, 48, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[12]), "physicsVoltageSetting[12]", "double", 1, 49, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[13]), "physicsVoltageSetting[13]", "double", 1, 50, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[14]), "physicsVoltageSetting[14]", "double", 1, 51, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[15]), "physicsVoltageSetting[15]", "double", 1, 52, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[16]), "physicsVoltageSetting[16]", "double", 1, 53, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[17]), "physicsVoltageSetting[17]", "double", 1, 54, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[18]), "physicsVoltageSetting[18]", "double", 1, 55, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[19]), "physicsVoltageSetting[19]", "double", 1, 56, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[20]), "physicsVoltageSetting[20]", "double", 1, 57, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[21]), "physicsVoltageSetting[21]", "double", 1, 58, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[22]), "physicsVoltageSetting[22]", "double", 1, 59, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[23]), "physicsVoltageSetting[23]", "double", 1, 60, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[24]), "physicsVoltageSetting[24]", "double", 1, 61, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[25]), "physicsVoltageSetting[25]", "double", 1, 62, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[26]), "physicsVoltageSetting[26]", "double", 1, 63, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[27]), "physicsVoltageSetting[27]", "double", 1, 64, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[28]), "physicsVoltageSetting[28]", "double", 1, 65, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[29]), "physicsVoltageSetting[29]", "double", 1, 66, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[30]), "physicsVoltageSetting[30]", "double", 1, 67, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsVoltageSetting[31]), "physicsVoltageSetting[31]", "double", 1, 68, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_{SLOT}0", offsetof(struct UserVar, standbyVoltageSetting[0]), "standbyVoltageSetting[0]", "double", 1, 69, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_{SLOT}1", offsetof(struct UserVar, standbyVoltageSetting[1]), "standbyVoltageSetting[1]", "double", 1, 70, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_{SLOT}2", offsetof(struct UserVar, standbyVoltageSetting[2]), "standbyVoltageSetting[2]", "double", 1, 71, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_{SLOT}3", offsetof(struct UserVar, standbyVoltageSetting[3]), "standbyVoltageSetting[3]", "double", 1, 72, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_{SLOT}4", offsetof(struct UserVar, standbyVoltageSetting[4]), "standbyVoltageSetting[4]", "double", 1, 73, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_{SLOT}5", offsetof(struct UserVar, standbyVoltageSetting[5]), "standbyVoltageSetting[5]", "double", 1, 74, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_{SLOT}6", offsetof(struct UserVar, standbyVoltageSetting[6]), "standbyVoltageSetting[6]", "double", 1, 75, 0, 0, 0, 0},
	{"{SYSTEM}:standby_voltage_{SLOT}7", offsetof(struct UserVar, standbyVoltageSetting[7]), "standbyVoltageSetting[7]", "double", 1, 76, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[8]), "standbyVoltageSetting[8]", "double", 1, 77, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[9]), "standbyVoltageSetting[9]", "double", 1, 78, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[10]), "standbyVoltageSetting[10]", "double", 1, 79, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[11]), "standbyVoltageSetting[11]", "double", 1, 80, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[12]), "standbyVoltageSetting[12]", "double", 1, 81, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[13]), "standbyVoltageSetting[13]", "double", 1, 82, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[14]), "standbyVoltageSetting[14]", "double", 1, 83, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[15]), "standbyVoltageSetting[15]", "double", 1, 84, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[16]), "standbyVoltageSetting[16]", "double", 1, 85, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[17]), "standbyVoltageSetting[17]", "double", 1, 86, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[18]), "standbyVoltageSetting[18]", "double", 1, 87, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[19]), "standbyVoltageSetting[19]", "double", 1, 88, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[20]), "standbyVoltageSetting[20]", "double", 1, 89, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[21]), "standbyVoltageSetting[21]", "double", 1, 90, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[22]), "standbyVoltageSetting[22]", "double", 1, 91, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[23]), "standbyVoltageSetting[23]", "double", 1, 92, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[24]), "standbyVoltageSetting[24]", "double", 1, 93, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[25]), "standbyVoltageSetting[25]", "double", 1, 94, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[26]), "standbyVoltageSetting[26]", "double", 1, 95, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[27]), "standbyVoltageSetting[27]", "double", 1, 96, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[28]), "standbyVoltageSetting[28]", "double", 1, 97, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[29]), "standbyVoltageSetting[29]", "double", 1, 98, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[30]), "standbyVoltageSetting[30]", "double", 1, 99, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyVoltageSetting[31]), "standbyVoltageSetting[31]", "double", 1, 100, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_{SLOT}0", offsetof(struct UserVar, physicsRampRate[0]), "physicsRampRate[0]", "double", 1, 101, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_{SLOT}1", offsetof(struct UserVar, physicsRampRate[1]), "physicsRampRate[1]", "double", 1, 102, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_{SLOT}2", offsetof(struct UserVar, physicsRampRate[2]), "physicsRampRate[2]", "double", 1, 103, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_{SLOT}3", offsetof(struct UserVar, physicsRampRate[3]), "physicsRampRate[3]", "double", 1, 104, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_{SLOT}4", offsetof(struct UserVar, physicsRampRate[4]), "physicsRampRate[4]", "double", 1, 105, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_{SLOT}5", offsetof(struct UserVar, physicsRampRate[5]), "physicsRampRate[5]", "double", 1, 106, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_{SLOT}6", offsetof(struct UserVar, physicsRampRate[6]), "physicsRampRate[6]", "double", 1, 107, 0, 0, 0, 0},
	{"{SYSTEM}:physics_ramp_rate_{SLOT}7", offsetof(struct UserVar, physicsRampRate[7]), "physicsRampRate[7]", "double", 1, 108, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[8]), "physicsRampRate[8]", "double", 1, 109, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[9]), "physicsRampRate[9]", "double", 1, 110, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[10]), "physicsRampRate[10]", "double", 1, 111, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[11]), "physicsRampRate[11]", "double", 1, 112, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[12]), "physicsRampRate[12]", "double", 1, 113, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[13]), "physicsRampRate[13]", "double", 1, 114, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[14]), "physicsRampRate[14]", "double", 1, 115, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[15]), "physicsRampRate[15]", "double", 1, 116, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[16]), "physicsRampRate[16]", "double", 1, 117, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[17]), "physicsRampRate[17]", "double", 1, 118, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[18]), "physicsRampRate[18]", "double", 1, 119, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[19]), "physicsRampRate[19]", "double", 1, 120, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[20]), "physicsRampRate[20]", "double", 1, 121, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[21]), "physicsRampRate[21]", "double", 1, 122, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[22]), "physicsRampRate[22]", "double", 1, 123, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[23]), "physicsRampRate[23]", "double", 1, 124, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[24]), "physicsRampRate[24]", "double", 1, 125, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[25]), "physicsRampRate[25]", "double", 1, 126, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[26]), "physicsRampRate[26]", "double", 1, 127, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[27]), "physicsRampRate[27]", "double", 1, 128, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[28]), "physicsRampRate[28]", "double", 1, 129, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[29]), "physicsRampRate[29]", "double", 1, 130, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[30]), "physicsRampRate[30]", "double", 1, 131, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, physicsRampRate[31]), "physicsRampRate[31]", "double", 1, 132, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_{SLOT}0", offsetof(struct UserVar, standbyRampRate[0]), "standbyRampRate[0]", "double", 1, 133, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_{SLOT}1", offsetof(struct UserVar, standbyRampRate[1]), "standbyRampRate[1]", "double", 1, 134, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_{SLOT}2", offsetof(struct UserVar, standbyRampRate[2]), "standbyRampRate[2]", "double", 1, 135, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_{SLOT}3", offsetof(struct UserVar, standbyRampRate[3]), "standbyRampRate[3]", "double", 1, 136, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_{SLOT}4", offsetof(struct UserVar, standbyRampRate[4]), "standbyRampRate[4]", "double", 1, 137, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_{SLOT}5", offsetof(struct UserVar, standbyRampRate[5]), "standbyRampRate[5]", "double", 1, 138, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_{SLOT}6", offsetof(struct UserVar, standbyRampRate[6]), "standbyRampRate[6]", "double", 1, 139, 0, 0, 0, 0},
	{"{SYSTEM}:standby_ramp_rate_{SLOT}7", offsetof(struct UserVar, standbyRampRate[7]), "standbyRampRate[7]", "double", 1, 140, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[8]), "standbyRampRate[8]", "double", 1, 141, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[9]), "standbyRampRate[9]", "double", 1, 142, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[10]), "standbyRampRate[10]", "double", 1, 143, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[11]), "standbyRampRate[11]", "double", 1, 144, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[12]), "standbyRampRate[12]", "double", 1, 145, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[13]), "standbyRampRate[13]", "double", 1, 146, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[14]), "standbyRampRate[14]", "double", 1, 147, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[15]), "standbyRampRate[15]", "double", 1, 148, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[16]), "standbyRampRate[16]", "double", 1, 149, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[17]), "standbyRampRate[17]", "double", 1, 150, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[18]), "standbyRampRate[18]", "double", 1, 151, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[19]), "standbyRampRate[19]", "double", 1, 152, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[20]), "standbyRampRate[20]", "double", 1, 153, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[21]), "standbyRampRate[21]", "double", 1, 154, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[22]), "standbyRampRate[22]", "double", 1, 155, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[23]), "standbyRampRate[23]", "double", 1, 156, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[24]), "standbyRampRate[24]", "double", 1, 157, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[25]), "standbyRampRate[25]", "double", 1, 158, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[26]), "standbyRampRate[26]", "double", 1, 159, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[27]), "standbyRampRate[27]", "double", 1, 160, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[28]), "standbyRampRate[28]", "double", 1, 161, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[29]), "standbyRampRate[29]", "double", 1, 162, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[30]), "standbyRampRate[30]", "double", 1, 163, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, standbyRampRate[31]), "standbyRampRate[31]", "double", 1, 164, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_{SLOT}0", offsetof(struct UserVar, conditioningDelay[0]), "conditioningDelay[0]", "double", 1, 165, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_{SLOT}1", offsetof(struct UserVar, conditioningDelay[1]), "conditioningDelay[1]", "double", 1, 166, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_{SLOT}2", offsetof(struct UserVar, conditioningDelay[2]), "conditioningDelay[2]", "double", 1, 167, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_{SLOT}3", offsetof(struct UserVar, conditioningDelay[3]), "conditioningDelay[3]", "double", 1, 168, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_{SLOT}4", offsetof(struct UserVar, conditioningDelay[4]), "conditioningDelay[4]", "double", 1, 169, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_{SLOT}5", offsetof(struct UserVar, conditioningDelay[5]), "conditioningDelay[5]", "double", 1, 170, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_{SLOT}6", offsetof(struct UserVar, conditioningDelay[6]), "conditioningDelay[6]", "double", 1, 171, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_delay_{SLOT}7", offsetof(struct UserVar, conditioningDelay[7]), "conditioningDelay[7]", "double", 1, 172, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[8]), "conditioningDelay[8]", "double", 1, 173, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[9]), "conditioningDelay[9]", "double", 1, 174, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[10]), "conditioningDelay[10]", "double", 1, 175, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[11]), "conditioningDelay[11]", "double", 1, 176, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[12]), "conditioningDelay[12]", "double", 1, 177, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[13]), "conditioningDelay[13]", "double", 1, 178, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[14]), "conditioningDelay[14]", "double", 1, 179, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[15]), "conditioningDelay[15]", "double", 1, 180, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[16]), "conditioningDelay[16]", "double", 1, 181, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[17]), "conditioningDelay[17]", "double", 1, 182, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[18]), "conditioningDelay[18]", "double", 1, 183, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[19]), "conditioningDelay[19]", "double", 1, 184, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[20]), "conditioningDelay[20]", "double", 1, 185, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[21]), "conditioningDelay[21]", "double", 1, 186, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[22]), "conditioningDelay[22]", "double", 1, 187, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[23]), "conditioningDelay[23]", "double", 1, 188, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[24]), "conditioningDelay[24]", "double", 1, 189, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[25]), "conditioningDelay[25]", "double", 1, 190, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[26]), "conditioningDelay[26]", "double", 1, 191, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[27]), "conditioningDelay[27]", "double", 1, 192, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[28]), "conditioningDelay[28]", "double", 1, 193, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[29]), "conditioningDelay[29]", "double", 1, 194, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[30]), "conditioningDelay[30]", "double", 1, 195, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningDelay[31]), "conditioningDelay[31]", "double", 1, 196, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_{SLOT}0", offsetof(struct UserVar, conditioningVoltageStep[0]), "conditioningVoltageStep[0]", "double", 1, 197, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_{SLOT}1", offsetof(struct UserVar, conditioningVoltageStep[1]), "conditioningVoltageStep[1]", "double", 1, 198, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_{SLOT}2", offsetof(struct UserVar, conditioningVoltageStep[2]), "conditioningVoltageStep[2]", "double", 1, 199, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_{SLOT}3", offsetof(struct UserVar, conditioningVoltageStep[3]), "conditioningVoltageStep[3]", "double", 1, 200, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_{SLOT}4", offsetof(struct UserVar, conditioningVoltageStep[4]), "conditioningVoltageStep[4]", "double", 1, 201, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_{SLOT}5", offsetof(struct UserVar, conditioningVoltageStep[5]), "conditioningVoltageStep[5]", "double", 1, 202, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_{SLOT}6", offsetof(struct UserVar, conditioningVoltageStep[6]), "conditioningVoltageStep[6]", "double", 1, 203, 0, 0, 0, 0},
	{"{SYSTEM}:conditioning_voltage_step_{SLOT}7", offsetof(struct UserVar, conditioningVoltageStep[7]), "conditioningVoltageStep[7]", "double", 1, 204, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[8]), "conditioningVoltageStep[8]", "double", 1, 205, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[9]), "conditioningVoltageStep[9]", "double", 1, 206, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[10]), "conditioningVoltageStep[10]", "double", 1, 207, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[11]), "conditioningVoltageStep[11]", "double", 1, 208, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[12]), "conditioningVoltageStep[12]", "double", 1, 209, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[13]), "conditioningVoltageStep[13]", "double", 1, 210, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[14]), "conditioningVoltageStep[14]", "double", 1, 211, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[15]), "conditioningVoltageStep[15]", "double", 1, 212, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[16]), "conditioningVoltageStep[16]", "double", 1, 213, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[17]), "conditioningVoltageStep[17]", "double", 1, 214, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[18]), "conditioningVoltageStep[18]", "double", 1, 215, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[19]), "conditioningVoltageStep[19]", "double", 1, 216, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[20]), "conditioningVoltageStep[20]", "double", 1, 217, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[21]), "conditioningVoltageStep[21]", "double", 1, 218, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[22]), "conditioningVoltageStep[22]", "double", 1, 219, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[23]), "conditioningVoltageStep[23]", "double", 1, 220, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[24]), "conditioningVoltageStep[24]", "double", 1, 221, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[25]), "conditioningVoltageStep[25]", "double", 1, 222, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[26]), "conditioningVoltageStep[26]", "double", 1, 223, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[27]), "conditioningVoltageStep[27]", "double", 1, 224, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[28]), "conditioningVoltageStep[28]", "double", 1, 225, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[29]), "conditioningVoltageStep[29]", "double", 1, 226, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[30]), "conditioningVoltageStep[30]", "double", 1, 227, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, conditioningVoltageStep[31]), "conditioningVoltageStep[31]", "double", 1, 228, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_{SLOT}0", offsetof(struct UserVar, voltageSetting[0]), "voltageSetting[0]", "double", 1, 229, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_{SLOT}1", offsetof(struct UserVar, voltageSetting[1]), "voltageSetting[1]", "double", 1, 230, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_{SLOT}2", offsetof(struct UserVar, voltageSetting[2]), "voltageSetting[2]", "double", 1, 231, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_{SLOT}3", offsetof(struct UserVar, voltageSetting[3]), "voltageSetting[3]", "double", 1, 232, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_{SLOT}4", offsetof(struct UserVar, voltageSetting[4]), "voltageSetting[4]", "double", 1, 233, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_{SLOT}5", offsetof(struct UserVar, voltageSetting[5]), "voltageSetting[5]", "double", 1, 234, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_{SLOT}6", offsetof(struct UserVar, voltageSetting[6]), "voltageSetting[6]", "double", 1, 235, 0, 0, 0, 0},
	{"{SYSTEM}:output_voltage_{SLOT}7", offsetof(struct UserVar, voltageSetting[7]), "voltageSetting[7]", "double", 1, 236, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[8]), "voltageSetting[8]", "double", 1, 237, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[9]), "voltageSetting[9]", "double", 1, 238, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[10]), "voltageSetting[10]", "double", 1, 239, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[11]), "voltageSetting[11]", "double", 1, 240, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[12]), "voltageSetting[12]", "double", 1, 241, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[13]), "voltageSetting[13]", "double", 1, 242, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[14]), "voltageSetting[14]", "double", 1, 243, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[15]), "voltageSetting[15]", "double", 1, 244, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[16]), "voltageSetting[16]", "double", 1, 245, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[17]), "voltageSetting[17]", "double", 1, 246, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[18]), "voltageSetting[18]", "double", 1, 247, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[19]), "voltageSetting[19]", "double", 1, 248, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[20]), "voltageSetting[20]", "double", 1, 249, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[21]), "voltageSetting[21]", "double", 1, 250, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[22]), "voltageSetting[22]", "double", 1, 251, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[23]), "voltageSetting[23]", "double", 1, 252, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[24]), "voltageSetting[24]", "double", 1, 253, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[25]), "voltageSetting[25]", "double", 1, 254, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[26]), "voltageSetting[26]", "double", 1, 255, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[27]), "voltageSetting[27]", "double", 1, 256, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[28]), "voltageSetting[28]", "double", 1, 257, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[29]), "voltageSetting[29]", "double", 1, 258, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[30]), "voltageSetting[30]", "double", 1, 259, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageSetting[31]), "voltageSetting[31]", "double", 1, 260, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_{SLOT}0", offsetof(struct UserVar, voltageReading[0]), "voltageReading[0]", "double", 1, 261, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_{SLOT}1", offsetof(struct UserVar, voltageReading[1]), "voltageReading[1]", "double", 1, 262, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_{SLOT}2", offsetof(struct UserVar, voltageReading[2]), "voltageReading[2]", "double", 1, 263, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_{SLOT}3", offsetof(struct UserVar, voltageReading[3]), "voltageReading[3]", "double", 1, 264, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_{SLOT}4", offsetof(struct UserVar, voltageReading[4]), "voltageReading[4]", "double", 1, 265, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_{SLOT}5", offsetof(struct UserVar, voltageReading[5]), "voltageReading[5]", "double", 1, 266, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_{SLOT}6", offsetof(struct UserVar, voltageReading[6]), "voltageReading[6]", "double", 1, 267, 0, 0, 0, 0},
	{"{SYSTEM}:output_sense_voltage_{SLOT}7", offsetof(struct UserVar, voltageReading[7]), "voltageReading[7]", "double", 1, 268, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[8]), "voltageReading[8]", "double", 1, 269, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[9]), "voltageReading[9]", "double", 1, 270, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[10]), "voltageReading[10]", "double", 1, 271, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[11]), "voltageReading[11]", "double", 1, 272, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[12]), "voltageReading[12]", "double", 1, 273, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[13]), "voltageReading[13]", "double", 1, 274, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[14]), "voltageReading[14]", "double", 1, 275, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[15]), "voltageReading[15]", "double", 1, 276, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[16]), "voltageReading[16]", "double", 1, 277, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[17]), "voltageReading[17]", "double", 1, 278, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[18]), "voltageReading[18]", "double", 1, 279, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[19]), "voltageReading[19]", "double", 1, 280, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[20]), "voltageReading[20]", "double", 1, 281, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[21]), "voltageReading[21]", "double", 1, 282, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[22]), "voltageReading[22]", "double", 1, 283, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[23]), "voltageReading[23]", "double", 1, 284, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[24]), "voltageReading[24]", "double", 1, 285, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[25]), "voltageReading[25]", "double", 1, 286, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[26]), "voltageReading[26]", "double", 1, 287, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[27]), "voltageReading[27]", "double", 1, 288, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[28]), "voltageReading[28]", "double", 1, 289, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[29]), "voltageReading[29]", "double", 1, 290, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[30]), "voltageReading[30]", "double", 1, 291, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, voltageReading[31]), "voltageReading[31]", "double", 1, 292, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_{SLOT}0", offsetof(struct UserVar, currentSetting[0]), "currentSetting[0]", "double", 1, 293, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_{SLOT}1", offsetof(struct UserVar, currentSetting[1]), "currentSetting[1]", "double", 1, 294, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_{SLOT}2", offsetof(struct UserVar, currentSetting[2]), "currentSetting[2]", "double", 1, 295, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_{SLOT}3", offsetof(struct UserVar, currentSetting[3]), "currentSetting[3]", "double", 1, 296, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_{SLOT}4", offsetof(struct UserVar, currentSetting[4]), "currentSetting[4]", "double", 1, 297, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_{SLOT}5", offsetof(struct UserVar, currentSetting[5]), "currentSetting[5]", "double", 1, 298, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_{SLOT}6", offsetof(struct UserVar, currentSetting[6]), "currentSetting[6]", "double", 1, 299, 0, 0, 0, 0},
	{"{SYSTEM}:output_current_{SLOT}7", offsetof(struct UserVar, currentSetting[7]), "currentSetting[7]", "double", 1, 300, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[8]), "currentSetting[8]", "double", 1, 301, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[9]), "currentSetting[9]", "double", 1, 302, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[10]), "currentSetting[10]", "double", 1, 303, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[11]), "currentSetting[11]", "double", 1, 304, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[12]), "currentSetting[12]", "double", 1, 305, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[13]), "currentSetting[13]", "double", 1, 306, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[14]), "currentSetting[14]", "double", 1, 307, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[15]), "currentSetting[15]", "double", 1, 308, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[16]), "currentSetting[16]", "double", 1, 309, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[17]), "currentSetting[17]", "double", 1, 310, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[18]), "currentSetting[18]", "double", 1, 311, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[19]), "currentSetting[19]", "double", 1, 312, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[20]), "currentSetting[20]", "double", 1, 313, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[21]), "currentSetting[21]", "double", 1, 314, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[22]), "currentSetting[22]", "double", 1, 315, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[23]), "currentSetting[23]", "double", 1, 316, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[24]), "currentSetting[24]", "double", 1, 317, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[25]), "currentSetting[25]", "double", 1, 318, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[26]), "currentSetting[26]", "double", 1, 319, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[27]), "currentSetting[27]", "double", 1, 320, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[28]), "currentSetting[28]", "double", 1, 321, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[29]), "currentSetting[29]", "double", 1, 322, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[30]), "currentSetting[30]", "double", 1, 323, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, currentSetting[31]), "currentSetting[31]", "double", 1, 324, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_{SLOT}0", offsetof(struct UserVar, rampRate[0]), "rampRate[0]", "double", 1, 325, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_{SLOT}1", offsetof(struct UserVar, rampRate[1]), "rampRate[1]", "double", 1, 326, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_{SLOT}2", offsetof(struct UserVar, rampRate[2]), "rampRate[2]", "double", 1, 327, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_{SLOT}3", offsetof(struct UserVar, rampRate[3]), "rampRate[3]", "double", 1, 328, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_{SLOT}4", offsetof(struct UserVar, rampRate[4]), "rampRate[4]", "double", 1, 329, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_{SLOT}5", offsetof(struct UserVar, rampRate[5]), "rampRate[5]", "double", 1, 330, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_{SLOT}6", offsetof(struct UserVar, rampRate[6]), "rampRate[6]", "double", 1, 331, 0, 0, 0, 0},
	{"{SYSTEM}:output_rise_rate_{SLOT}7", offsetof(struct UserVar, rampRate[7]), "rampRate[7]", "double", 1, 332, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[8]), "rampRate[8]", "double", 1, 333, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[9]), "rampRate[9]", "double", 1, 334, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[10]), "rampRate[10]", "double", 1, 335, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[11]), "rampRate[11]", "double", 1, 336, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[12]), "rampRate[12]", "double", 1, 337, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[13]), "rampRate[13]", "double", 1, 338, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[14]), "rampRate[14]", "double", 1, 339, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[15]), "rampRate[15]", "double", 1, 340, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[16]), "rampRate[16]", "double", 1, 341, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[17]), "rampRate[17]", "double", 1, 342, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[18]), "rampRate[18]", "double", 1, 343, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[19]), "rampRate[19]", "double", 1, 344, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[20]), "rampRate[20]", "double", 1, 345, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[21]), "rampRate[21]", "double", 1, 346, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[22]), "rampRate[22]", "double", 1, 347, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[23]), "rampRate[23]", "double", 1, 348, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[24]), "rampRate[24]", "double", 1, 349, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[25]), "rampRate[25]", "double", 1, 350, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[26]), "rampRate[26]", "double", 1, 351, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[27]), "rampRate[27]", "double", 1, 352, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[28]), "rampRate[28]", "double", 1, 353, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[29]), "rampRate[29]", "double", 1, 354, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[30]), "rampRate[30]", "double", 1, 355, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, rampRate[31]), "rampRate[31]", "double", 1, 356, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}0:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[0]), "autoResetDelay[0]", "double", 1, 357, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}1:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[1]), "autoResetDelay[1]", "double", 1, 358, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}2:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[2]), "autoResetDelay[2]", "double", 1, 359, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}3:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[3]), "autoResetDelay[3]", "double", 1, 360, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}4:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[4]), "autoResetDelay[4]", "double", 1, 361, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}5:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[5]), "autoResetDelay[5]", "double", 1, 362, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}6:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[6]), "autoResetDelay[6]", "double", 1, 363, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}7:ControlSQ.DLY4", offsetof(struct UserVar, autoResetDelay[7]), "autoResetDelay[7]", "double", 1, 364, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[8]), "autoResetDelay[8]", "double", 1, 365, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[9]), "autoResetDelay[9]", "double", 1, 366, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[10]), "autoResetDelay[10]", "double", 1, 367, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[11]), "autoResetDelay[11]", "double", 1, 368, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[12]), "autoResetDelay[12]", "double", 1, 369, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[13]), "autoResetDelay[13]", "double", 1, 370, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[14]), "autoResetDelay[14]", "double", 1, 371, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[15]), "autoResetDelay[15]", "double", 1, 372, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[16]), "autoResetDelay[16]", "double", 1, 373, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[17]), "autoResetDelay[17]", "double", 1, 374, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[18]), "autoResetDelay[18]", "double", 1, 375, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[19]), "autoResetDelay[19]", "double", 1, 376, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[20]), "autoResetDelay[20]", "double", 1, 377, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[21]), "autoResetDelay[21]", "double", 1, 378, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[22]), "autoResetDelay[22]", "double", 1, 379, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[23]), "autoResetDelay[23]", "double", 1, 380, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[24]), "autoResetDelay[24]", "double", 1, 381, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[25]), "autoResetDelay[25]", "double", 1, 382, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[26]), "autoResetDelay[26]", "double", 1, 383, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[27]), "autoResetDelay[27]", "double", 1, 384, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[28]), "autoResetDelay[28]", "double", 1, 385, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[29]), "autoResetDelay[29]", "double", 1, 386, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[30]), "autoResetDelay[30]", "double", 1, 387, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetDelay[31]), "autoResetDelay[31]", "double", 1, 388, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}0:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[0]), "autoResetEnabled[0]", "double", 1, 389, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}1:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[1]), "autoResetEnabled[1]", "double", 1, 390, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}2:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[2]), "autoResetEnabled[2]", "double", 1, 391, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}3:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[3]), "autoResetEnabled[3]", "double", 1, 392, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}4:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[4]), "autoResetEnabled[4]", "double", 1, 393, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}5:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[5]), "autoResetEnabled[5]", "double", 1, 394, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}6:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[6]), "autoResetEnabled[6]", "double", 1, 395, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}7:AutoResetEnable", offsetof(struct UserVar, autoResetEnabled[7]), "autoResetEnabled[7]", "double", 1, 396, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[8]), "autoResetEnabled[8]", "double", 1, 397, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[9]), "autoResetEnabled[9]", "double", 1, 398, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[10]), "autoResetEnabled[10]", "double", 1, 399, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[11]), "autoResetEnabled[11]", "double", 1, 400, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[12]), "autoResetEnabled[12]", "double", 1, 401, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[13]), "autoResetEnabled[13]", "double", 1, 402, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[14]), "autoResetEnabled[14]", "double", 1, 403, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[15]), "autoResetEnabled[15]", "double", 1, 404, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[16]), "autoResetEnabled[16]", "double", 1, 405, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[17]), "autoResetEnabled[17]", "double", 1, 406, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[18]), "autoResetEnabled[18]", "double", 1, 407, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[19]), "autoResetEnabled[19]", "double", 1, 408, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[20]), "autoResetEnabled[20]", "double", 1, 409, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[21]), "autoResetEnabled[21]", "double", 1, 410, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[22]), "autoResetEnabled[22]", "double", 1, 411, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[23]), "autoResetEnabled[23]", "double", 1, 412, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[24]), "autoResetEnabled[24]", "double", 1, 413, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[25]), "autoResetEnabled[25]", "double", 1, 414, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[26]), "autoResetEnabled[26]", "double", 1, 415, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[27]), "autoResetEnabled[27]", "double", 1, 416, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[28]), "autoResetEnabled[28]", "double", 1, 417, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[29]), "autoResetEnabled[29]", "double", 1, 418, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[30]), "autoResetEnabled[30]", "double", 1, 419, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, autoResetEnabled[31]), "autoResetEnabled[31]", "double", 1, 420, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}0:maxCycles", offsetof(struct UserVar, maxCycles[0]), "maxCycles[0]", "double", 1, 421, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}1:maxCycles", offsetof(struct UserVar, maxCycles[1]), "maxCycles[1]", "double", 1, 422, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}2:maxCycles", offsetof(struct UserVar, maxCycles[2]), "maxCycles[2]", "double", 1, 423, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}3:maxCycles", offsetof(struct UserVar, maxCycles[3]), "maxCycles[3]", "double", 1, 424, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}4:maxCycles", offsetof(struct UserVar, maxCycles[4]), "maxCycles[4]", "double", 1, 425, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}5:maxCycles", offsetof(struct UserVar, maxCycles[5]), "maxCycles[5]", "double", 1, 426, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}6:maxCycles", offsetof(struct UserVar, maxCycles[6]), "maxCycles[6]", "double", 1, 427, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}7:maxCycles", offsetof(struct UserVar, maxCycles[7]), "maxCycles[7]", "double", 1, 428, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[8]), "maxCycles[8]", "double", 1, 429, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[9]), "maxCycles[9]", "double", 1, 430, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[10]), "maxCycles[10]", "double", 1, 431, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[11]), "maxCycles[11]", "double", 1, 432, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[12]), "maxCycles[12]", "double", 1, 433, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[13]), "maxCycles[13]", "double", 1, 434, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[14]), "maxCycles[14]", "double", 1, 435, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[15]), "maxCycles[15]", "double", 1, 436, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[16]), "maxCycles[16]", "double", 1, 437, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[17]), "maxCycles[17]", "double", 1, 438, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[18]), "maxCycles[18]", "double", 1, 439, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[19]), "maxCycles[19]", "double", 1, 440, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[20]), "maxCycles[20]", "double", 1, 441, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[21]), "maxCycles[21]", "double", 1, 442, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[22]), "maxCycles[22]", "double", 1, 443, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[23]), "maxCycles[23]", "double", 1, 444, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[24]), "maxCycles[24]", "double", 1, 445, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[25]), "maxCycles[25]", "double", 1, 446, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[26]), "maxCycles[26]", "double", 1, 447, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[27]), "maxCycles[27]", "double", 1, 448, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[28]), "maxCycles[28]", "double", 1, 449, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[29]), "maxCycles[29]", "double", 1, 450, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[30]), "maxCycles[30]", "double", 1, 451, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, maxCycles[31]), "maxCycles[31]", "double", 1, 452, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}0:sw_enabled", offsetof(struct UserVar, channelEnabled[0]), "channelEnabled[0]", "double", 1, 453, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}1:sw_enabled", offsetof(struct UserVar, channelEnabled[1]), "channelEnabled[1]", "double", 1, 454, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}2:sw_enabled", offsetof(struct UserVar, channelEnabled[2]), "channelEnabled[2]", "double", 1, 455, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}3:sw_enabled", offsetof(struct UserVar, channelEnabled[3]), "channelEnabled[3]", "double", 1, 456, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}4:sw_enabled", offsetof(struct UserVar, channelEnabled[4]), "channelEnabled[4]", "double", 1, 457, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}5:sw_enabled", offsetof(struct UserVar, channelEnabled[5]), "channelEnabled[5]", "double", 1, 458, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}6:sw_enabled", offsetof(struct UserVar, channelEnabled[6]), "channelEnabled[6]", "double", 1, 459, 0, 0, 0, 0},
	{"{SYSTEM}:{SLOT}7:sw_enabled", offsetof(struct UserVar, channelEnabled[7]), "channelEnabled[7]", "double", 1, 460, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[8]), "channelEnabled[8]", "double", 1, 461, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[9]), "channelEnabled[9]", "double", 1, 462, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[10]), "channelEnabled[10]", "double", 1, 463, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[11]), "channelEnabled[11]", "double", 1, 464, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[12]), "channelEnabled[12]", "double", 1, 465, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[13]), "channelEnabled[13]", "double", 1, 466, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[14]), "channelEnabled[14]", "double", 1, 467, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[15]), "channelEnabled[15]", "double", 1, 468, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[16]), "channelEnabled[16]", "double", 1, 469, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[17]), "channelEnabled[17]", "double", 1, 470, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[18]), "channelEnabled[18]", "double", 1, 471, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[19]), "channelEnabled[19]", "double", 1, 472, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[20]), "channelEnabled[20]", "double", 1, 473, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[21]), "channelEnabled[21]", "double", 1, 474, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[22]), "channelEnabled[22]", "double", 1, 475, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[23]), "channelEnabled[23]", "double", 1, 476, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[24]), "channelEnabled[24]", "double", 1, 477, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[25]), "channelEnabled[25]", "double", 1, 478, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[26]), "channelEnabled[26]", "double", 1, 479, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[27]), "channelEnabled[27]", "double", 1, 480, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[28]), "channelEnabled[28]", "double", 1, 481, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[29]), "channelEnabled[29]", "double", 1, 482, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[30]), "channelEnabled[30]", "double", 1, 483, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, channelEnabled[31]), "channelEnabled[31]", "double", 1, 484, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_{SLOT}0", offsetof(struct UserVar, powerSwitch[0]), "powerSwitch[0]", "double", 1, 485, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_{SLOT}1", offsetof(struct UserVar, powerSwitch[1]), "powerSwitch[1]", "double", 1, 486, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_{SLOT}2", offsetof(struct UserVar, powerSwitch[2]), "powerSwitch[2]", "double", 1, 487, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_{SLOT}3", offsetof(struct UserVar, powerSwitch[3]), "powerSwitch[3]", "double", 1, 488, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_{SLOT}4", offsetof(struct UserVar, powerSwitch[4]), "powerSwitch[4]", "double", 1, 489, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_{SLOT}5", offsetof(struct UserVar, powerSwitch[5]), "powerSwitch[5]", "double", 1, 490, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_{SLOT}6", offsetof(struct UserVar, powerSwitch[6]), "powerSwitch[6]", "double", 1, 491, 0, 0, 0, 0},
	{"{SYSTEM}:output_switch_{SLOT}7", offsetof(struct UserVar, powerSwitch[7]), "powerSwitch[7]", "double", 1, 492, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[8]), "powerSwitch[8]", "double", 1, 493, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[9]), "powerSwitch[9]", "double", 1, 494, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[10]), "powerSwitch[10]", "double", 1, 495, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[11]), "powerSwitch[11]", "double", 1, 496, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[12]), "powerSwitch[12]", "double", 1, 497, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[13]), "powerSwitch[13]", "double", 1, 498, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[14]), "powerSwitch[14]", "double", 1, 499, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[15]), "powerSwitch[15]", "double", 1, 500, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[16]), "powerSwitch[16]", "double", 1, 501, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[17]), "powerSwitch[17]", "double", 1, 502, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[18]), "powerSwitch[18]", "double", 1, 503, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[19]), "powerSwitch[19]", "double", 1, 504, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[20]), "powerSwitch[20]", "double", 1, 505, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[21]), "powerSwitch[21]", "double", 1, 506, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[22]), "powerSwitch[22]", "double", 1, 507, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[23]), "powerSwitch[23]", "double", 1, 508, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[24]), "powerSwitch[24]", "double", 1, 509, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[25]), "powerSwitch[25]", "double", 1, 510, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[26]), "powerSwitch[26]", "double", 1, 511, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[27]), "powerSwitch[27]", "double", 1, 512, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[28]), "powerSwitch[28]", "double", 1, 513, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[29]), "powerSwitch[29]", "double", 1, 514, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[30]), "powerSwitch[30]", "double", 1, 515, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, powerSwitch[31]), "powerSwitch[31]", "double", 1, 516, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_{SLOT}0", offsetof(struct UserVar, tripStatus[0]), "tripStatus[0]", "double", 1, 517, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_{SLOT}1", offsetof(struct UserVar, tripStatus[1]), "tripStatus[1]", "double", 1, 518, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_{SLOT}2", offsetof(struct UserVar, tripStatus[2]), "tripStatus[2]", "double", 1, 519, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_{SLOT}3", offsetof(struct UserVar, tripStatus[3]), "tripStatus[3]", "double", 1, 520, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_{SLOT}4", offsetof(struct UserVar, tripStatus[4]), "tripStatus[4]", "double", 1, 521, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_{SLOT}5", offsetof(struct UserVar, tripStatus[5]), "tripStatus[5]", "double", 1, 522, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_{SLOT}6", offsetof(struct UserVar, tripStatus[6]), "tripStatus[6]", "double", 1, 523, 0, 0, 0, 0},
	{"{SYSTEM}:trip_status_{SLOT}7", offsetof(struct UserVar, tripStatus[7]), "tripStatus[7]", "double", 1, 524, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[8]), "tripStatus[8]", "double", 1, 525, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[9]), "tripStatus[9]", "double", 1, 526, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[10]), "tripStatus[10]", "double", 1, 527, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[11]), "tripStatus[11]", "double", 1, 528, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[12]), "tripStatus[12]", "double", 1, 529, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[13]), "tripStatus[13]", "double", 1, 530, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[14]), "tripStatus[14]", "double", 1, 531, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[15]), "tripStatus[15]", "double", 1, 532, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[16]), "tripStatus[16]", "double", 1, 533, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[17]), "tripStatus[17]", "double", 1, 534, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[18]), "tripStatus[18]", "double", 1, 535, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[19]), "tripStatus[19]", "double", 1, 536, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[20]), "tripStatus[20]", "double", 1, 537, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[21]), "tripStatus[21]", "double", 1, 538, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[22]), "tripStatus[22]", "double", 1, 539, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[23]), "tripStatus[23]", "double", 1, 540, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[24]), "tripStatus[24]", "double", 1, 541, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[25]), "tripStatus[25]", "double", 1, 542, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[26]), "tripStatus[26]", "double", 1, 543, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[27]), "tripStatus[27]", "double", 1, 544, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[28]), "tripStatus[28]", "double", 1, 545, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[29]), "tripStatus[29]", "double", 1, 546, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[30]), "tripStatus[30]", "double", 1, 547, 0, 0, 0, 0},
	{NULL, offsetof(struct UserVar, tripStatus[31]), "tripStatus[31]", "double", 1, 548, 0, 0, 0, 0}
};

/* Event masks for state set "power_status" */
static const seqMask	EM_power_status_0_init[] = {
	0x00000008,
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
static const seqMask	EM_power_status_0_on[] = {
	0x00000008,
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
static const seqMask	EM_power_status_0_off[] = {
	0x00000008,
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
static const seqMask	EM_power_status_0_check_off_level1[] = {
	0x00000008,
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
static const seqMask	EM_power_status_0_check_off_level2[] = {
	0x00000008,
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
static const seqMask	EM_power_status_0_check_on_level1[] = {
	0x00000008,
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

/* State table for state set "power_status" */
static seqState G_power_status_states[] = {
	{
	/* state name */        "init",
	/* action function */   A_power_status_0_init,
	/* event function */    E_power_status_0_init,
	/* delay function */    D_power_status_0_init,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  EM_power_status_0_init,
	/* state options */     (0)
	},
	{
	/* state name */        "on",
	/* action function */   A_power_status_0_on,
	/* event function */    E_power_status_0_on,
	/* delay function */    D_power_status_0_on,
	/* entry function */    I_power_status_0_on,
	/* exit function */     0,
	/* event mask array */  EM_power_status_0_on,
	/* state options */     (0)
	},
	{
	/* state name */        "off",
	/* action function */   A_power_status_0_off,
	/* event function */    E_power_status_0_off,
	/* delay function */    D_power_status_0_off,
	/* entry function */    I_power_status_0_off,
	/* exit function */     0,
	/* event mask array */  EM_power_status_0_off,
	/* state options */     (0)
	},
	{
	/* state name */        "check_off_level1",
	/* action function */   A_power_status_0_check_off_level1,
	/* event function */    E_power_status_0_check_off_level1,
	/* delay function */    D_power_status_0_check_off_level1,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  EM_power_status_0_check_off_level1,
	/* state options */     (0)
	},
	{
	/* state name */        "check_off_level2",
	/* action function */   A_power_status_0_check_off_level2,
	/* event function */    E_power_status_0_check_off_level2,
	/* delay function */    D_power_status_0_check_off_level2,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  EM_power_status_0_check_off_level2,
	/* state options */     (0)
	},
	{
	/* state name */        "check_on_level1",
	/* action function */   A_power_status_0_check_on_level1,
	/* event function */    E_power_status_0_check_on_level1,
	/* delay function */    D_power_status_0_check_on_level1,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  EM_power_status_0_check_on_level1,
	/* state options */     (0)
	},
};

/* Event masks for state set "ramp1" */
static const seqMask	EM_ramp1_1_init[] = {
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
static const seqMask	EM_ramp1_1_set_entry_state[] = {
	0x00000002,
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
static const seqMask	EM_ramp1_1_prepare_to_ramp[] = {
	0x00000002,
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
static const seqMask	EM_ramp1_1_set_to_off[] = {
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
static const seqMask	EM_ramp1_1_ramp_to_standby[] = {
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
static const seqMask	EM_ramp1_1_standby_ramping_delay[] = {
	0x00000002,
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
static const seqMask	EM_ramp1_1_check_standby_rampup[] = {
	0x00000002,
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
static const seqMask	EM_ramp1_1_standby_ramping_step_finished[] = {
	0x00000002,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000020,
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
static const seqMask	EM_ramp1_1_ramp_to_physics[] = {
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
static const seqMask	EM_ramp1_1_begin_physics_ramp[] = {
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
static const seqMask	EM_ramp1_1_physics_ramping_delay[] = {
	0x00000002,
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
static const seqMask	EM_ramp1_1_check_physics_rampup[] = {
	0x00000002,
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
static const seqMask	EM_ramp1_1_physics_ramping_step_finished[] = {
	0x00000002,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000020,
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
static const seqMask	EM_ramp1_1_off[] = {
	0x00000002,
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
static const seqMask	EM_ramp1_1_standby[] = {
	0x00000002,
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
static const seqMask	EM_ramp1_1_physics[] = {
	0x00000002,
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

/* State table for state set "ramp1" */
static seqState G_ramp1_states[] = {
	{
	/* state name */        "init",
	/* action function */   A_ramp1_1_init,
	/* event function */    E_ramp1_1_init,
	/* delay function */    D_ramp1_1_init,
	/* entry function */    I_ramp1_1_init,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_init,
	/* state options */     (0)
	},
	{
	/* state name */        "set_entry_state",
	/* action function */   A_ramp1_1_set_entry_state,
	/* event function */    E_ramp1_1_set_entry_state,
	/* delay function */    D_ramp1_1_set_entry_state,
	/* entry function */    I_ramp1_1_set_entry_state,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_set_entry_state,
	/* state options */     (0)
	},
	{
	/* state name */        "prepare_to_ramp",
	/* action function */   A_ramp1_1_prepare_to_ramp,
	/* event function */    E_ramp1_1_prepare_to_ramp,
	/* delay function */    D_ramp1_1_prepare_to_ramp,
	/* entry function */    I_ramp1_1_prepare_to_ramp,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_prepare_to_ramp,
	/* state options */     (0)
	},
	{
	/* state name */        "set_to_off",
	/* action function */   A_ramp1_1_set_to_off,
	/* event function */    E_ramp1_1_set_to_off,
	/* delay function */    D_ramp1_1_set_to_off,
	/* entry function */    I_ramp1_1_set_to_off,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_set_to_off,
	/* state options */     (0)
	},
	{
	/* state name */        "ramp_to_standby",
	/* action function */   A_ramp1_1_ramp_to_standby,
	/* event function */    E_ramp1_1_ramp_to_standby,
	/* delay function */    D_ramp1_1_ramp_to_standby,
	/* entry function */    I_ramp1_1_ramp_to_standby,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_ramp_to_standby,
	/* state options */     (0)
	},
	{
	/* state name */        "standby_ramping_delay",
	/* action function */   A_ramp1_1_standby_ramping_delay,
	/* event function */    E_ramp1_1_standby_ramping_delay,
	/* delay function */    D_ramp1_1_standby_ramping_delay,
	/* entry function */    I_ramp1_1_standby_ramping_delay,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_standby_ramping_delay,
	/* state options */     (0)
	},
	{
	/* state name */        "check_standby_rampup",
	/* action function */   A_ramp1_1_check_standby_rampup,
	/* event function */    E_ramp1_1_check_standby_rampup,
	/* delay function */    D_ramp1_1_check_standby_rampup,
	/* entry function */    I_ramp1_1_check_standby_rampup,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_check_standby_rampup,
	/* state options */     (0)
	},
	{
	/* state name */        "standby_ramping_step_finished",
	/* action function */   A_ramp1_1_standby_ramping_step_finished,
	/* event function */    E_ramp1_1_standby_ramping_step_finished,
	/* delay function */    D_ramp1_1_standby_ramping_step_finished,
	/* entry function */    I_ramp1_1_standby_ramping_step_finished,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_standby_ramping_step_finished,
	/* state options */     (0)
	},
	{
	/* state name */        "ramp_to_physics",
	/* action function */   A_ramp1_1_ramp_to_physics,
	/* event function */    E_ramp1_1_ramp_to_physics,
	/* delay function */    D_ramp1_1_ramp_to_physics,
	/* entry function */    I_ramp1_1_ramp_to_physics,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_ramp_to_physics,
	/* state options */     (0)
	},
	{
	/* state name */        "begin_physics_ramp",
	/* action function */   A_ramp1_1_begin_physics_ramp,
	/* event function */    E_ramp1_1_begin_physics_ramp,
	/* delay function */    D_ramp1_1_begin_physics_ramp,
	/* entry function */    I_ramp1_1_begin_physics_ramp,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_begin_physics_ramp,
	/* state options */     (0)
	},
	{
	/* state name */        "physics_ramping_delay",
	/* action function */   A_ramp1_1_physics_ramping_delay,
	/* event function */    E_ramp1_1_physics_ramping_delay,
	/* delay function */    D_ramp1_1_physics_ramping_delay,
	/* entry function */    I_ramp1_1_physics_ramping_delay,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_physics_ramping_delay,
	/* state options */     (0)
	},
	{
	/* state name */        "check_physics_rampup",
	/* action function */   A_ramp1_1_check_physics_rampup,
	/* event function */    E_ramp1_1_check_physics_rampup,
	/* delay function */    D_ramp1_1_check_physics_rampup,
	/* entry function */    I_ramp1_1_check_physics_rampup,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_check_physics_rampup,
	/* state options */     (0)
	},
	{
	/* state name */        "physics_ramping_step_finished",
	/* action function */   A_ramp1_1_physics_ramping_step_finished,
	/* event function */    E_ramp1_1_physics_ramping_step_finished,
	/* delay function */    D_ramp1_1_physics_ramping_step_finished,
	/* entry function */    I_ramp1_1_physics_ramping_step_finished,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_physics_ramping_step_finished,
	/* state options */     (0)
	},
	{
	/* state name */        "off",
	/* action function */   A_ramp1_1_off,
	/* event function */    E_ramp1_1_off,
	/* delay function */    D_ramp1_1_off,
	/* entry function */    I_ramp1_1_off,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_off,
	/* state options */     (0)
	},
	{
	/* state name */        "standby",
	/* action function */   A_ramp1_1_standby,
	/* event function */    E_ramp1_1_standby,
	/* delay function */    D_ramp1_1_standby,
	/* entry function */    I_ramp1_1_standby,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_standby,
	/* state options */     (0)
	},
	{
	/* state name */        "physics",
	/* action function */   A_ramp1_1_physics,
	/* event function */    E_ramp1_1_physics,
	/* delay function */    D_ramp1_1_physics,
	/* entry function */    I_ramp1_1_physics,
	/* exit function */     0,
	/* event mask array */  EM_ramp1_1_physics,
	/* state options */     (0)
	},
};

/* State set table */
static seqSS G_state_sets[] = {
	{
	/* state set name */    "power_status",
	/* states */            G_power_status_states,
	/* number of states */  6,
	/* number of delays */  1
	},

	{
	/* state set name */    "ramp1",
	/* states */            G_ramp1_states,
	/* number of states */  16,
	/* number of delays */  2
	},
};

/* Program table (global) */
seqProgram sncFGTRamping = {
	/* magic number */      2001013,
	/* program name */      "sncFGTRamping",
	/* channels */          G_channels,
	/* num. channels */     548,
	/* state sets */        G_state_sets,
	/* num. state sets */   2,
	/* user var size */     sizeof(struct UserVar),
	/* param */             "deviceName=(null)",
	/* num. event flags */  0,
	/* encoded options */   (0 | OPT_CONN | OPT_NEWEF | OPT_REENT),
	/* init func */         G_prog_init,
	/* entry func */        NULL,
	/* exit func */         NULL,
	/* num. queues */       0
};

/* Register sequencer commands and program */
#include "epicsExport.h"
static void sncFGTRampingRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&sncFGTRamping);
}
epicsExportRegistrar(sncFGTRampingRegistrar);
