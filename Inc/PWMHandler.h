/*
 * PWMHandler.h
 *
 *  Created on: May 22, 2017
 *      Author: johan
 */

#ifndef PWMHANDLER_H_
#define PWMHANDLER_H_
#include "stm32f3xx_hal.h"
#include <map>
#include <string>
#include <vector>

typedef struct{
	TIM_TypeDef *name;
	uint32_t frequency;
	std::vector<uint32_t> channels;

}PWM_settings;


class PWMHandler {
private:
	TIM_MasterConfigTypeDef sMasterConfig;
	//TIM_OC_InitTypeDef sConfigOC;
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
	std::map<uint32_t, TIM_OC_InitTypeDef> channels;
	void initChannelConfig(TIM_OC_InitTypeDef *timer);
public:
	TIM_HandleTypeDef timer;
	PWMHandler(PWM_settings *settings);
	void setDutyCycle(uint32_t, int value);
	virtual ~PWMHandler();
};

#endif /* PWMHANDLER_H_ */
