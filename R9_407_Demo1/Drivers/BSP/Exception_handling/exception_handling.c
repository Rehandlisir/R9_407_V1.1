#include "./BSP/Exception_handling/exception_handling.h"
void ex_handl_Init(void)
{


	;
}
void ex_handl_brake(void)
{
	if (LEFT_BREAK_STATE || RIGHT_BRAKE_STATE ) //push
	{
		g_slaveReg[4] = 0;
	}
	else // drive
	{
		g_slaveReg[4] = 1;
	}
}

void ex_handl_LRmoter(void)
{
	
	
	
	;
	
	
}

void ex_handl_linearActuator(void)
{
	
	CanCmdled(LED_YELLOW,LED_GREEN,LED_GREEN,LED_GREEN,LED_GREEN);
	
}

void ex_handl_battary(void)
{
	
	;
	
}

void ex_handl_joystic(void)
{
	
	
	;
	
}

void ex_handl_indicatorlight(void)
{
	
	;
}

void ex_handl_excute(void)
{
	
	ex_handl_brake();
	
	ex_handl_LRmoter();
	
	ex_handl_linearActuator();
	
	ex_handl_battary();
	
	ex_handl_joystic();
	
}
