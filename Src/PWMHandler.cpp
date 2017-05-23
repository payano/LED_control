/*
 * PWMHandler.cpp
 *
 *  Created on: May 22, 2017
 *      Author: johan
 */

#include "PWMHandler.h"
#include <iostream>

PWMHandler::PWMHandler(PWM_settings *settings){
	// TODO Auto-generated constructor stub
	this->timer.Instance = TIM1; //TODO CHANGE THIS.
	this->timer.Init.Prescaler = (uint32_t)(SystemCoreClock / 130000);
	this->timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	this->timer.Init.Period = 100;
	this->timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	this->timer.Init.RepetitionCounter = 0;
	this->timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&this->timer) != HAL_OK)
	{
		//TODO YOLO
		//_Error_Handler(__FILE__, __LINE__);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&this->timer, &sMasterConfig) != HAL_OK)
	{
		//TODO YOLO
		//_Error_Handler(__FILE__, __LINE__);
	}

	//Configure PWM for all channels
	for(std::vector<uint32_t>::iterator it = settings->channels.begin(); it != settings->channels.end(); ++it) {
		TIM_OC_InitTypeDef *oc_timer = new TIM_OC_InitTypeDef;
		this->initChannelConfig(oc_timer);
		HAL_TIM_PWM_ConfigChannel(&this->timer, oc_timer, TIM_CHANNEL_1);
		this->channels.insert(std::pair<uint32_t,TIM_OC_InitTypeDef>(*it, *oc_timer));

	}

	this->sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	this->sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	this->sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	this->sBreakDeadTimeConfig.DeadTime = 0;
	this->sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	this->sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	this->sBreakDeadTimeConfig.BreakFilter = 0;
	this->sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	this->sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	this->sBreakDeadTimeConfig.Break2Filter = 0;
	this->sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&this->timer, &this->sBreakDeadTimeConfig) != HAL_OK)
	{
		//TODO STUFF
	}


	GPIO_InitTypeDef GPIO_InitStruct;
	if(this->timer.Instance==TIM1)
	{
		/* USER CODE BEGIN TIM1_MspPostInit 0 */

		/* USER CODE END TIM1_MspPostInit 0 */

		/**TIM1 GPIO Configuration
		PE9     ------> TIM1_CH1
		PE11     ------> TIM1_CH2
		PE13     ------> TIM1_CH3
		PE14     ------> TIM1_CH4
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	}


	for(std::vector<uint32_t>::iterator it = settings->channels.begin(); it != settings->channels.end(); ++it) {
		HAL_TIM_PWM_Start_IT(&this->timer, *it);
	}

}

void PWMHandler::initChannelConfig(TIM_OC_InitTypeDef *timer){
	timer->OCMode = TIM_OCMODE_PWM1;
	timer->Pulse = 0;
	timer->OCPolarity = TIM_OCPOLARITY_HIGH;
	timer->OCNPolarity = TIM_OCNPOLARITY_HIGH;
	timer->OCFastMode = TIM_OCFAST_DISABLE;
	timer->OCIdleState = TIM_OCIDLESTATE_RESET;
	timer->OCNIdleState = TIM_OCNIDLESTATE_RESET;

}

void PWMHandler::setDutyCycle(uint32_t channel, int value){
	//TODO - Fix loading from its own Config.
	//or use a shared one with only pulse stored as different.

	//TIM_OC_InitTypeDef channelConfig = this->channels.at(channel);
	//channelConfig.Pulse = value;
	//HAL_TIM_PWM_ConfigChannel(&this->timer, &channelConfig, channel);
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = value;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	//HAL_TIM_PWM_ConfigChannel()

	HAL_TIM_PWM_Stop_IT(&this->timer, channel);
	HAL_TIM_PWM_ConfigChannel(&this->timer,&sConfigOC,channel);
	HAL_TIM_PWM_Start_IT(&this->timer, channel);
	//HAL_TIM_PWM_Stop_IT(&this->timer, channel);
	//HAL_TIM_PWM_Start_IT(&this->timer, channel);


}
PWMHandler::~PWMHandler() {
	// TODO Auto-generated destructor stub
}
