#include "defs.h"


void initRoom(RoomType* room, char* name){
	strcpy(room->name, name);
	
	RoomListType* roomList = malloc(sizeof(RoomListType));
	room->rooms = roomList;
	initRoomList(room->rooms);
	
	EvidenceListType* evidenceList = malloc(sizeof(EvidenceListType));
	(room)->evidence = evidenceList;
	initEvidenceList(room->evidence);
	
	initHunterArray(&room->hunters);
	
	room->ghost = NULL;
	
	sem_init(&room->mutex, 0, 1);
}


void initRoomList(RoomListType* roomList){
	roomList->head = NULL;
	roomList->tail = NULL;
	roomList->totalRooms = 0;
}


void appendRoom(RoomListType* list, RoomNodeType* room){
	list->totalRooms++;
	room->next = NULL;
	//If an empty list, the room will be the head and tail
	if (list->head == NULL){
		list->head = room;
		list->tail = room;
		return;
	}
	
	//Not an empty list
	list->tail->next = room;
	list->tail = room;
}


void connectRooms(RoomType* room1, RoomType* room2){
	RoomNodeType* room1Node = calloc(1, sizeof(RoomNodeType));
    	room1Node->room = room1;
    	room1Node->next = NULL;
	appendRoom(room2->rooms, room1Node);
	
	RoomNodeType* room2Node = calloc(1, sizeof(RoomNodeType));
    	room2Node->room = room2;
    	room2Node->next = NULL;
	appendRoom(room1->rooms, room2Node);
}


void cleanupRoomData(RoomListType* list){
	RoomNodeType* curr = list->head;
	if (curr == NULL) return;
	
	while(1){
		cleanupRoomList(curr->room->rooms);
		free(curr->room->rooms);
		cleanupEvidenceData(curr->room->evidence);
		cleanupEvidenceList(curr->room->evidence);
		free(curr->room->evidence);
		free(curr->room);
		if (curr->next == NULL) break;
		curr = curr->next;
	}
}

void cleanupRoomList(RoomListType* list){
	RoomNodeType* curr = list->head;
	RoomNodeType* next = NULL;
	
	while(1){
		if (curr == NULL) break;
		next = curr->next;
		free(curr);
		curr = next;
	}
}

