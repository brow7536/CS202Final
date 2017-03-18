#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define COMMANDLIM	50

typedef struct room room;
void input( char argbuf[], char *arglist[] );
char *makestring( char *buf );
void action( char *action[] );
void move( char direction );
void start();




int main()
{
	/*variables*/
	char command[COMMANDLIM];
	char *commands[COMMANDLIM];
	int j;
//	room current = NULL ;
	/*game shell*/
	printf("Start\n");
	while(1)
	{
		for(j = 0; j <= COMMANDLIM; j++)
			command[j] = 0;
		printf(">");
		if(fgets(command, COMMANDLIM, stdin))
		{
			input(command, commands);
			
			/* this will be moved to the action function later */
			if(strcmp(commands[0], /*command*/"go") == 0)
			{
				printf("went\n");
			}
			else if(strcmp(commands[0], "look") == 0)
			{
				printf("you see\n");
			}
			else
			{
				printf("I do not recognise that command.\n");
			}
		}
	}
	return 0;

}

struct room {
	char *name;
	char *see;
	char *path;
	char *items;
	room *north;
	room *south;
	room *east;
	room *west;

};

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

void start(){








}
