/*
 * player.h
 *
 *  Created on: 20/9/2018
 *      Author: sutto
 */

#ifndef SOURCES_PLAYER_H_
#define SOURCES_PLAYER_H_


#include "SDHC.h"



uint8_t Player_Init(void);
void	Player_Play_Song(int);
void	Player_Play(void);
void Player_Up_Volume(void);
void Player_Down_Volume(void);
void	Player_Fill_Buffer(void);
void test_play_sample(void);

#endif /* SOURCES_PLAYER_H_ */
