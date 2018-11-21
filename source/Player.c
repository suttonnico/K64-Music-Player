/*
 * player.c
 *
 *  Created on: 20/9/2018
 *      Author: sutto
 */

#ifndef SOURCES_PLAYER_C_
#define SOURCES_PLAYER_C_

//#include "dac_adc.h"
#include "Player.h"
#include "dac.h"
#include "fsl_debug_console.h"
#include "fsl_dac.h"
#include "SDHC.h"
#include "SysTick.h"


//DEBUG
#include "PORT.h"
#include "GPIO.H"
const PCRstr PORT_OUT_PUSHPULL2 = {0, 0, 0, 0, 0, 0, 1, 0, PORT_mGPIO, 0, 0, PORT_eDisabled, 0, 0, 0};

#define MAX_LEN 1000000
#define start_cd 0x46464952
#define MAX_SONGS 6
#define BUFFER_LENGTH 128
#define MAX_VOLUME 20

#include "SysTick.h"
uint32_t pData[128];
int song_starts[MAX_SONGS];
uint32_t right_buffer[BUFFER_LENGTH*2];
uint32_t left_buffer[BUFFER_LENGTH*2];
int current_song_num = 0;

char current_sample_out = 0;
char current_buffer_in = 0;
char current_buffer_out = 0;

int current_sector = 0;
char feed =  0;
char play_flag = 1;
char volume = 10;

void Player_Play_Sample(void)
{

	if(play_flag == 1)
	{
		if(current_buffer_out == 0)
			test_buf_0();
		else
			test_buf_1();
	}
	//PRINTF("%d\n",BUFFER[current_sample_out++]*4096.0/255/2);
}

void Player_Up_Volume(void)
{
	if(volume < MAX_VOLUME)
		volume += 1;
}

void Player_Down_Volume(void)
{
	if(volume > 0)
		volume -= 1;
}

void Player_Play(void)
{
	play_flag *= -1;
}

uint8_t Player_Init(void)
{

//	int i = 0;
//	int j = 0;
//	int song_ind = 0;
//	int last_song = 0;


	//DEBUG
	PORT_Init();
	PORT_Configure2(PORTC, 2, PORT_OUT_PUSHPULL2);
	GPIO_SetDirPin(PTC, 2, GPIO__OUT);
	GPIO_SetPin(PTC, 2);


	SDHC_Init();
	uint8_t card_status;
	PRINTF("SD INIT\n");
	card_status =SDHC_InitCard();
	PRINTF("SD INIT DONE\n");
	PRINTF("DAC ADC INIT\n");
	dac_config_t dacConfigStruct;
	DAC_Init(DAC0, &dacConfigStruct);

	PRINTF("\nDAC ADC INIT DONE\n");
/*
	for(i=0;i<MAX_LEN;i++)
	{
		//uint32_t pData[5000];
		//PRINTF("%d",i);
		SDHC_Read_Sector(i,pData);
		if(pData[0] == start_cd)
		{
			song_starts[song_ind++] = i;
			PRINTF("Song %d start %d\n",song_ind-1,i);
		}

	}
	for(i=0;i<MAX_SONGS;i++)
		if(song_starts[i]==0)
			last_song=i-1;
*/
	song_starts[0] = 40992;
	song_starts[1]=144632;//

	SysTick_Init (2720L,Player_Play_Sample);

	return card_status;

}
int convert_num(uint32_t x)
{

	int out;
	float resta;
	if(x > 0x00007FFF)
	{
		resta = (float)x - (float)0x0000FFFF -1;
		resta = resta*2048 / (float)0x7FFF;
		out =  ((int)resta)+2048;
	}
	else
		out =  (int)(((int)((float)x*2048/(float)0x7FFF)))+2048;
	return out/(MAX_VOLUME)*volume;
}

void Player_Play_Song(int song_num)
{
	int i;
	int dif = 54;
	play_flag =1;
	//select song
	current_song_num = song_num;
	current_sector = song_starts[current_song_num];	//54 es el offset con el inicio del track
	SDHC_Read_Sector(current_sector++,pData);
	current_buffer_in = 0;

	for(i=0;i<BUFFER_LENGTH-dif;i++)
	{
		left_buffer[i+BUFFER_LENGTH*current_buffer_in+dif] = (pData[i+dif]&0xFFFF0000)>>16;
		right_buffer[i+BUFFER_LENGTH*current_buffer_in+dif] = pData[i+dif]&0x0000FFFF;
		//PRINTF("Debug muestra: %x \n",left_buffer[i+BUFFER_LENGTH*current_buffer_in+dif]);
		//PRINTF("Debug muestra: %x \n",right_buffer[i+BUFFER_LENGTH*current_buffer_in+dif]);
	}
	current_buffer_in = 1;
	Player_Fill_Buffer();

}


void Player_Fill_Buffer(void)
{
	if(feed == 1){

		//DEBUG
		GPIO_SetPin(PTC, 2);

	int i;
	SDHC_Read_Sector(current_sector++,pData);
	//PRINTF("sector %d\n",current_sector);
	for(i=0;i<BUFFER_LENGTH;i++)
	{
		left_buffer[i+BUFFER_LENGTH*current_buffer_in] = (pData[i]&0xFFFF0000)>>16;
		right_buffer[i+BUFFER_LENGTH*current_buffer_in] = pData[i]&0x0000FFFF;
	}
	feed = 0;

	//DEBUG
	GPIO_ClearPin(PTC, 2);
	}
}

void test_buf_0(void)
{
	//PRINTF("%d\t",left_buffer[current_buffer_out*BUFFER_LENGTH+current_sample_out]);
	//PRINTF("%d\n",convert_num(left_buffer[current_buffer_out*BUFFER_LENGTH+current_sample_out]));
	DAC_Set_Value((uint16_t)convert_num(right_buffer[current_sample_out++]));
	//test_set_DAC(BUFFER[current_sample_out++]*2048/0xFF+2048);

	if(current_sample_out==BUFFER_LENGTH)
		{
			current_buffer_out = 1;
			current_sample_out = 0;
			current_buffer_in = 0;
			//Player_Fill_Buffer();
			feed = 1;
		}
}

void test_buf_1(void)
{
	//PRINTF("%d\t",left_buffer[current_buffer_out*BUFFER_LENGTH+current_sample_out]);
	//PRINTF("%d\n",convert_num(right_buffer[current_buffer_out*BUFFER_LENGTH+current_sample_out++])/0x7FFF*4+2048);
	DAC_Set_Value((uint16_t)convert_num(right_buffer[BUFFER_LENGTH+current_sample_out++]));
	//test_set_DAC(BUFFER[current_sample_out++]*2048/0xFF+2048);
	if(current_sample_out==BUFFER_LENGTH)
	{
		current_buffer_out = 0;
		current_sample_out = 0;
		current_buffer_in = 1;
		feed = 1;
		//Player_Fill_Buffer();

	}
}




#endif /* SOURCES_PLAYER_C_ */
