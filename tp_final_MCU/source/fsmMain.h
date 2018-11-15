/*
 * fsmMain.h
 *
 *  Created on: Feb 16, 2018
 *      Author: martin
 */

#ifndef FSMMAIN_H_
#define FSMMAIN_H_

#include "actions.h"

typedef enum{
	NO_EVENT,
	PLAY,
	PAUSE,
	SONG_1,
	SONG_2,
	SONG_3,
	SONG_4,
	SONG_5,
	MORE_VOLUME,
	LESS_VOLUME,
}event;

typedef struct
{
	event eventQ[QUEUE_LENGTH];
	unsigned int index;
}eventQueue;

typedef struct state{
	event currentEvent;
	struct state * nextState;
	void (* action) (userData&);
}fsmState;



typedef struct{
	int song[5];
	int downVolume;
	int upVolume;
	int play;
	int pause;
}userData;

extern fsmState Idle[],
				playSong1[],
				playSong2[],
				playSong3[],
				playSong4[],
				playSong5[],
				pauseSong1[],
				pauseSong2[],
				pauseSong3[],
				pauseSong4[],
				pauseSong5[]
			    ;


void stateMove(event newEvent, fsmState **currentState, userData *data, eventQueue *queue);
void initQueue (eventQueue * queue);
int pushEvent (eventQueue * queue, event newEvent);
event pullEvent(eventQueue * queue);

#endif /* FSMMAIN_H_ */
