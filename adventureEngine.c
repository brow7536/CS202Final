#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define COMMANDLIM	50

typedef struct room room;
typedef struct door door;

typedef struct Item item_t;
typedef struct Room Room_t;

void input( char argbuf[], char *arglist[] );
char *makestring( char *buf );
void action( char *action[] );
Room_t *move( char direction[], Room_t *current );

typedef struct Item item_t;
typedef struct Room Room_t;


struct Item {
	char name[20];
	char desc[200];
};

struct Room {
	char name[20];
	char desc[200];
	item_t items[100];
	Room_t *north;
	Room_t *east;
	Room_t *south;
	Room_t *west;
} ;

void linkRoom(char *roomnum, Room_t *roomDb, int dir, int cur);
int isInArray(char *key, char *string);
void getItems(char *itembuf, item_t *itemDB, Room_t *roomDb, int i);



int main()
{
	/*variables*/
	char command[COMMANDLIM];
	char *commands[COMMANDLIM];
	int j;


//--------------------------------------------------
// ACCESS ROOM AND ITEM FILES AND BUILD DATABASES

	FILE *fitems;
	fitems = fopen("itemlist.txt","rw+");
	if(!fitems)
		perror("fopen");

	int x = 0;
	char bufItem[100];

	item_t *itemDB = calloc(50*sizeof(item_t), sizeof(item_t));

	//create database of items using itemlist.txt
	while(fgets(bufItem,sizeof(bufItem),fitems))
	{
		char *str;
		str = strtok(bufItem,"|");
		strcpy(itemDB[x].name, str);
		str = strtok(NULL, "|");
		strcpy(itemDB[x].desc, str);
		x++;
	}
	fclose(fitems);


	FILE *f;	

	f = fopen("roomlist.txt","rw+");
	if(!f)
		perror("fopen2");

	int i = 0;
	char buf[1000];
	char itembuf[100];
	char *dirbuf = calloc(10000,10000);

	Room_t *roomDb = malloc(50 * sizeof(Room_t));

	//create database of rooms using roomlist.txt
	while(fgets(buf,sizeof(buf),f))
	{	

		//use tokens to parse through the roomlist text file
		//This loop will give names, descriptions, and items
		//to the room database

		char *str = malloc(500);

		str=strtok(buf, "|");
		if(str != NULL)
		   strncpy(roomDb[i].name, str, sizeof roomDb[i].name -1);

		
		str=strtok(NULL, "|");
		if(str != NULL)
		   strncpy(roomDb[i].desc, str, sizeof roomDb[i].desc -1);
		

		str=strtok(NULL, "|");
		if(str != NULL)
		   strcpy(itembuf, str);
		//generate array of items contained in room
		getItems(itembuf,itemDB,roomDb,i);

		i++;
	}
	fclose(f);
	i=0;


	f = fopen("roomlist.txt","rw+");

	//this loop will reparse the roomlist file to link rooms to the
	//connected rooms. This will allow for easy room-to-room movement
	while(fgets(buf,sizeof(buf),f))
	{	

		char *str = malloc(1000);
		
		//move beyond the tokens for name, desc, items
		str=strtok(buf, "|");
		str=strtok(NULL, "|");
		str=strtok(NULL, "|");
		

		//begin linking rooms
		
		str=strtok(NULL, "|");
		
		if(str != NULL && str != " "){
		  dirbuf = str;
		  linkRoom(dirbuf,roomDb,0,i);
		}

		str=strtok(NULL, "|");
		if(str != NULL && str != " "){
		  dirbuf = str;
		  linkRoom(dirbuf,roomDb,1,i);
		}

		str=strtok(NULL, "|");
		if(str != NULL && str != " "){
		  dirbuf = str;
		  linkRoom(dirbuf,roomDb,2,i);
		}

		str=strtok(NULL, "|");
		if(str != NULL && str != " "){
		  dirbuf = str;
		  linkRoom(dirbuf,roomDb,3,i);
		}

		i++;
	}

	fclose(f);

//--------------------------------------------------

	
	Room_t *current = &roomDb[0];


	printf("You wake up in a strange room...\n");
	printf("%s\n\n", current->desc);
	while(1)
	{
		for(j = 0; j <= COMMANDLIM; j++)
			command[j] = '\0';
		//command = NULL;

		
		printf(">");
		if(fgets(command, COMMANDLIM, stdin))
		{
			input(command, commands);
			//printf("[D]%s\n",makestring(command));
			/* this will be moved to the action function later */
			if(strcmp(commands[0], /*command*/"go") == 0)
			{
				current = move( commands[1], current );
			}
			else if(strcmp(commands[0], "look") == 0)
			{
				printf("%s\n", current->desc);
			}
			else if(strcmp(commands[0], "quit") == 0 || strcmp(commands[0], "exit") == 0)
			{
				printf("Are you sure you want to quit? (y/n) ");
				if(fgets(command, COMMANDLIM, stdin))
				{
					if(command[0] == 'y')
						return 0;
					else;
						
				}			
			}
			else if(strcmp(commands[0], "kill") == 0)
			{
//				printf("got the first one\n");
				if(strcmp(commands[1], "self") == 0){
					printf("You die a horrible death, but that's not in the game yet.\n");
				}
				else printf("kill what?\n");
			}
			else
			{
				printf("I do not recognise that command.\n");
			}
		}
	}
	return 0;

}


char *makestring( char *buf )
{
   char  *cp;
   //buf[strlen(buf)-1] = '\0';    /* trim newline   */
   cp = (char *) malloc( strlen(buf)+1 );    /* get memory  */
   if ( cp == NULL )
   {         /* or die   */
      fprintf(stderr,"no memory\n");
      exit(1);
   }
   strcpy(cp, buf);     /* copy chars  */
   return cp;        /* return ptr  */
}

void input( char argbuf[], char *arglist[] ){
	char *tok = strtok(argbuf, " ");
	int i = 0;
	int t = 0;
	while(tok != NULL && i < COMMANDLIM){
		//printf("tok = %s\n", makestring(tok));

		arglist[i++] = makestring(tok);
		tok = strtok(NULL, " ");

		if(tok == NULL)
			arglist[i-1][strlen(arglist[i-1])-1] = '\0';

	}
}

Room_t *move( char direction[], Room_t *current ){
	
	if(direction[0] == 'n')
				{
					
				if(current->north != NULL){
//						current = current->north;
					printf("%s\n\n", current->north->desc);
					return current->north;
				}
				}
				else if(direction[0] == 's')
				{
					if(current->south != NULL){
//						current = current->south;
						printf("%s\n\n", current->south->desc);
						return current->south;
					 }
				}
				else if(direction[0] == 'e')
				{
					if(current->east != NULL){
//						current = current->east;
						printf("%s\n\n", current->east->desc);
						return current->east;
					}
				}
				else if(direction[0] == 'w')
				{
					if(current->west != NULL){
//						current = current->west;
						printf("%s\n\n", current->west->desc);
						return current->west;
					}
					
				}
				else if(direction == '\0'){
					printf("Where are you going?\n");
					return current; }
				else{
					printf("I don't know where that is.\n");
					return current; }
				
				printf("There is a wall there.\n");
				return current;
					
				printf("went\n");
				
}



//linkRoom will pass the addresses of connected rooms to the room
//in question
void linkRoom(char *roomnum, Room_t *roomDb, int dir, int cur)
{
	int i = 0;
	//hardcoded size of roomlist to avoid memory faults
	for(int j; j < 3; j++)
	{
		if(strcmp(roomDb[i].name,roomnum) == 0)
		{
			switch(dir){
				case 0:
					roomDb[cur].north = &roomDb[i];
					//printf("%s\n", roomDb[cur].north->name);
					break;
				case 1:
					roomDb[cur].east = &roomDb[i];
					//printf("%s\n", roomDb[cur].east->name);
					break;
				case 2:
					roomDb[cur].south = &roomDb[i];
					break;
				case 3:
					roomDb[cur].west = &roomDb[i];
					break;
			}
		}
		i++;
	}
}

//find a key within an array
int isInArray(char *key, char string[])
{
	int x = 0;
	int keyfound = 0;
	for(int i = 0; i < 100; i++)
	{
		if(string[i] == *key)
		{
			keyfound = x;
         
			return keyfound;
		}

		x++;
	}

	return keyfound;
}


//getItems adds all needed item structures to each room structure
void getItems(char itembuf[], item_t *itemDB, Room_t *roomDb, int i)
{
	char *itemstr = NULL;
	int j = 0;
	//if there are multiple items to be added
	if(itembuf != NULL && (isInArray(",",itembuf)!=0))
	{
		itemstr = strtok(itembuf,",");
		while(itemstr !=NULL)
		{
			//scan through the itemDB. 100 is an arbitrary number
			for(int k = 0;k<100;k++)
			{
				if(&itemDB[k] != NULL && itemstr != NULL)
				{
					if(strncmp(itemDB[k].name,itemstr, sizeof itemDB[k].name -1) == 0)
						roomDb[i].items[j] = itemDB[k];
				}
			}
			
			itemstr = strtok(NULL,",");
			j++; 
		}
	}
	//if there is only one item to be added
	else if(itembuf != NULL && (isInArray(",",itembuf)==0))
	{
		itemstr = itembuf;
		for(int k = 0;k<100;k++)
		{
			if(&itemDB[k] != NULL && itemstr != NULL)
			{
				if(strncmp(itemDB[k].name,itemstr, sizeof itemDB[k].name -1) == 0)
					roomDb[i].items[j] = itemDB[k];
			}
		}
	}
}