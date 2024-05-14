

#include "Can.h"
#include "Can_Cfg.h"

#include "Std_Types.h"
#include "CanIf_Types.h"
#include "ComStack_Types.h"
#include "Mcu.h"
void CanIf_Config(){
	
	
}

void Can_InitController( uint8 controller, const Can_ControllerConfigType *config){	

}

Can_ReturnType Can_SetControllerMode( uint8 Controller, Can_StateTransitionType transition ){
	
	return CAN_OK;
}

Can_ReturnType Can_Write( Can_Arc_HTHType hth, Can_PduType *pduInfo ){
	return CAN_OK;
	
}