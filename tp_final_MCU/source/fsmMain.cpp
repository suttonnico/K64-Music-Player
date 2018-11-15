/*
 * fsmMain.cpp
 *
 *  Created on: Feb 16, 2018
 *      Author: martin
 */

#include "fsmMain.h"


void stateMove(event newEvent, fsmState **currentState, userData *data, eventQueue *queue)
{
	int i;

	for(i=0;(((*currentState)[i]).currentEvent != newEvent)&&(((*currentState)[i]).currentEvent != NO_EVENT);i++)
		;

	((*currentState)[i]).action(data,queue);
	*currentState=((*currentState)[i]).nextState;

}


/////////////////////STATE  {EVENT, 		NEXT STATE,	ACTION}///////////////////
fsmState Idle[]{ 			{NO_EVENT,		Idle,		no_Action},
							{PLAY,			Idle, 		no_Action},
							{PAUSE,			Idle, 		no_Action},
							{SONG_1,		playSong1,	song1Play},
							{SONG_2,		playSong2,	song2Play},
							{SONG_3,		playSong3,	song3Play},
							{SONG_4,		playSong4,	song4Play},
							{SONG_5,		playSong5,	song5Play},
							{MORE_VOLUME,	Idle, 		moreVolume},
							{LESS_VOLUME,	Idle, 		lessVolume}
};

fsmState playSong1[]{		{NO_EVENT, 		playSong1,	no_Action},
							{PLAY,			playSong1,	no_Action},
							{PAUSE,			pauseSong1,	song1Pause},
							{SONG_1,		playSong1,	no_Action},
							{SONG_2,		playSong2,	song2Play},
							{SONG_3,		playSong3,	song3Play},
							{SONG_4,		playSong4,	song4Play},
							{SONG_5,		playSong5,	song5Play},
							{MORE_VOLUME,	playSong1,	moreVolume},
							{LESS_VOLUME,	playSong1,	lessVolume}

};

fsmState playSong2[]{		{NO_EVENT, 		playSong2,	no_Action},
							{PLAY,			playSong2,	no_Action},
							{PAUSE,			pauseSong2,	song2Pause},
							{SONG_1,		playSong1,	song1Play},
							{SONG_2,		playSong2,	no_Action},
							{SONG_3,		playSong3,	song3Play},
							{SONG_4,		playSong4,	song4Play},
							{SONG_5,		playSong5,	song5Play},
							{MORE_VOLUME,	playSong2,	moreVolume},
							{LESS_VOLUME,	playSong2,	lessVolume}

};

fsmState playSong3[]{		{NO_EVENT, 		playSong3,	no_Action},
							{PLAY,			playSong3,	no_Action},
							{PAUSE,			pauseSong3,	song3Pause},
							{SONG_1,		playSong1,	song1Play},
							{SONG_2,		playSong2,	song2Play},
							{SONG_3,		playSong3,	no_Action},
							{SONG_4,		playSong4,	song4Play},
							{SONG_5,		playSong5,	song5Play},
							{MORE_VOLUME,	playSong3,	moreVolume},
							{LESS_VOLUME,	playSong3,	lessVolume}

};

fsmState playSong4[]{		{NO_EVENT, 		playSong4,	no_Action},
							{PLAY,			playSong4,	no_Action},
							{PAUSE,			pauseSong4,	song4Pause},
							{SONG_1,		playSong1,	song1Play},
							{SONG_2,		playSong2,	song2Play},
							{SONG_3,		playSong3,	song3Play},
							{SONG_4,		playSong4,	no_Action},
							{SONG_5,		playSong5,	song5Play},
							{MORE_VOLUME,	playSong4,	moreVolume},
							{LESS_VOLUME,	playSong4,	lessVolume}

};

fsmState playSong5[]{		{NO_EVENT, 		playSong5,	no_Action},
							{PLAY,			playSong5,	no_Action},
							{PAUSE,			pauseSong5,	song5Pause},
							{SONG_1,		playSong1,	song1Play},
							{SONG_2,		playSong2,	song2Play},
							{SONG_3,		playSong3,	song3Play},
							{SONG_4,		playSong4,	song4Play},
							{SONG_5,		playSong5,	no_Action},
							{MORE_VOLUME,	playSong5,	moreVolume},
							{LESS_VOLUME,	playSong5,	lessVolume}

};

fsmState pauseSong1[]{		{NO_EVENT, 		pauseSong1,	no_Action},
							{PLAY,			playSong1,	song1Play},
							{PAUSE,			pauseSong1,	no_Action},
							{SONG_1,		playSong1,	no_Action},
							{SONG_2,		playSong2,	song2Play},
							{SONG_3,		playSong3,	song3Play},
							{SONG_4,		playSong4,	song4Play},
							{SONG_5,		playSong5,	song5Play},
							{MORE_VOLUME,	playSong1,	moreVolume},
							{LESS_VOLUME,	playSong1,	lessVolume}

};

fsmState pauseSong2[]{		{NO_EVENT, 		pauseSong2,	no_Action},
							{PLAY,			playSong2,	song2Play},
							{PAUSE,			pauseSong2,	no_Action},
							{SONG_1,		playSong1,	song1Play},
							{SONG_2,		playSong2,	no_Action},
							{SONG_3,		playSong3,	song3Play},
							{SONG_4,		playSong4,	song4Play},
							{SONG_5,		playSong5,	song5Play},
							{MORE_VOLUME,	playSong1,	moreVolume},
							{LESS_VOLUME,	playSong1,	lessVolume}

};

fsmState pauseSong3[]{		{NO_EVENT, 		pauseSong3,	no_Action},
							{PLAY,			playSong3,	song3Play},
							{PAUSE,			pauseSong3,	no_Action},
							{SONG_1,		playSong1,	song1Play},
							{SONG_2,		playSong2,	song2Play},
							{SONG_3,		playSong3,	no_Action},
							{SONG_4,		playSong4,	song4Play},
							{SONG_5,		playSong5,	song5Play},
							{MORE_VOLUME,	playSong1,	moreVolume},
							{LESS_VOLUME,	playSong1,	lessVolume}

};

fsmState pauseSong4[]{		{NO_EVENT, 		pauseSong4,	no_Action},
							{PLAY,			playSong4,	song4Play},
							{PAUSE,			pauseSong4,	no_Action},
							{SONG_1,		playSong1,	song1Play},
							{SONG_2,		playSong2,	song2Play},
							{SONG_3,		playSong3,	song3Play},
							{SONG_4,		playSong4,	no_Action},
							{SONG_5,		playSong5,	song5Play},
							{MORE_VOLUME,	playSong1,	moreVolume},
							{LESS_VOLUME,	playSong1,	lessVolume}

};

fsmState pauseSong5[]{		{NO_EVENT, 		pauseSong5,	no_Action},
							{PLAY,			playSong5,	song5Play},
							{PAUSE,			pauseSong5,	no_Action},
							{SONG_1,		playSong1,	song1Play},
							{SONG_2,		playSong2,	song2Play},
							{SONG_3,		playSong3,	song3Play},
							{SONG_4,		playSong4,	song4Play},
							{SONG_5,		playSong5,	no_Action},
							{MORE_VOLUME,	playSong1,	moreVolume},
							{LESS_VOLUME,	playSong1,	lessVolume}

};



///////////////////////CREATE QUEUE/////////////////////////////////

void initQueue (eventQueue * queue)
{
	int i;

	queue->index = 0;
	for(i = 0;i < QUEUE_LENGTH;i++)
		queue->eventQ[i] = NO_EVENT;

}


int pushEvent (eventQueue * queue, event newEvent)
{
	if(queue->index != QUEUE_LENGTH)
		(queue->index)++;

	queue->eventQ[queue->index] = newEvent;

}

event pullEvent(eventQueue * queue)
{
	event newEvent;
	unsigned int i;

	newEvent = queue->eventQ[0];

	for(i = 0;i < queue->index;i++)
		queue->eventQ[i] = queue->eventQ[i+1];

	queue->eventQ[queue->index] = NO_EVENT;

	if(queue->index != 0)
		(queue->index)--;

	return newEvent;
}




