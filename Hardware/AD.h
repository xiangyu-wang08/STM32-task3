#ifndef __AD_H__
#define __AD_H__

extern uint16_t AD_DMAValue[500];

void AD_Init(void);
uint16_t AD_GetValue(void);
void AD_DMACapture_Init ( uint16_t num);

//void AD_GetValue(void);


#endif
