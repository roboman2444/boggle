#include "stdlib.h"
#include "stdio.h"
#include "string.h"


#define WORDMAX 1024

#define BOARDX 5
#define BOARDY 5
#define MAXDEPTH 26

#define LEASTDEPTH 2
#define FASTERCHECK



char **wordlist;

int wordcount = 0;

int wordsize = 0;

int numtimes = 0;


int findwords(char ** board, int * skips, int depth, char * line, int dictstart, int dictend, int pos[2]){


//	if(depth > MAXDEPTH) return 0;
	if(pos[0] < 0 || pos[0] >= BOARDY || pos[1] < 0 || pos[1] >= BOARDX) return 0;

	int i;
	for(i = 0; i < depth; i++){
		if(skips[i*2] == pos[0] && skips[(i*2)+1] == pos[1]) return 0;
	}


	char current = board[pos[0]][pos[1]];
	for(; dictstart <= dictend && current != wordlist[dictstart][depth]; dictstart++); // used to be <
	for(; dictend >= dictstart && current != wordlist[dictend][depth]; dictend--); // used to be >
	if(dictstart > dictend) return 0; // used to be ==


	numtimes++;

	char * myline = malloc((depth+2)*sizeof(char));
	sprintf(myline, "%s%c", line, current);



	if(depth > LEASTDEPTH){

#ifdef FASTERCHECK
		if(wordlist[dictstart][depth+1] == '\0'){
			printf("%s\n", wordlist[dictstart]);
		}
#else
		int word;
		for(word  = dictstart; word <= dictend; word++){
			if(wordlist[word][depth+1] == '\0'){
				printf("%s\n", wordlist[word]);
				break;
			}
		}
#endif
	}


	skips[depth*2] = pos[0];
	skips[(depth*2) +1] = pos[1];

	int x,y;
	int newpos[2];
	if(depth < MAXDEPTH){
		for(x = -1; x < 2; x++){
			newpos[0] = pos[0]+x;
			for(y = -1; y < 2; y++){
				if(x == 0 && y == 0) continue;
				newpos[1] = pos[1]+y;
				findwords(board, skips, depth+1, myline, dictstart, dictend, newpos);
			}
		}
	}
	skips[depth*2] = -1;
	skips[(depth*2) +1] = -1;

	free(myline);
	return 1;
}
void gettable(char ** board, int boardx, int boardy){
	printf("please enter table\n");
	char line;
	int x,y;
	for(y = 0; y < boardy; y++){
		for(x = 0; x < boardx; x++){
			line = getc(stdin);
			if((line >= 'a' && line <= 'z')||(line >= 'A' && line <= 'Z')) board[y][x] = line;
		}
		printf("       %s\n", board[y]);
	}
}



int countlines(FILE * f){
	int count = 0;
	char line[WORDMAX];
	while (fgets(line, WORDMAX, f)){
		count++;
	}
	return count;
}


int loadwordlist(const char * filename){
	int count = 0;
	char line[WORDMAX];
	FILE * f;
	if(!( f = fopen(filename, "r"))) return count;
	count = countlines(f);
	wordlist = malloc(count * sizeof(char *));
	rewind(f);
	int i = 0;
	while(fgets(line, WORDMAX, f)){
		int len = strlen(line);
		if(len>0 && line[len-1] == '\n'){
			line[len-1] = '\0';
			--len;
		}
		if (len == 0 || line[0] =='#') continue;
		wordlist[i] = malloc(len*sizeof(char));
		strncpy(wordlist[i], line, len);
		i++;
	}
	fclose(f);
	//debug
	for(i=0; i < count; i++){
		wordsize += sizeof(wordlist[i]);
	}
	return count;
}


int main(int argc, char * argv[]){
	if(argc > 1);
	wordcount = loadwordlist(argv[1]);
//	printf("number of words: %i\nsize of dict: %i\n", wordcount, wordsize);
	char **board;
	int *skips;
	board = malloc(BOARDY * sizeof(char *));
	int i;
	for(i = 0; i < BOARDY; i++){
		board[i] = malloc(BOARDX * sizeof(char *));
	}

	skips = malloc (MAXDEPTH * 2 * sizeof(int));
	memset(skips, -1, MAXDEPTH*2*sizeof(int));

	board[0] = "eosue";
	board[1] = "oarst";
	board[2] = "eenkw";
	board[3] = "guhtr";
	board[4] = "sqsxv";

	skips = malloc(MAXDEPTH * sizeof(int*));
	memset(skips, -1, MAXDEPTH*2*sizeof(int));

	int pos[2] = {0,0};
	for(; pos[0] < BOARDX; pos[0]++){
		for(pos[1] = 0; pos[1] < BOARDY; pos[1]++){
//			memset(skips, -1, MAXDEPTH*2*sizeof(int));
			findwords(board, skips, 0, "", 0, wordcount-1,pos);
		}
	}

	printf("numtimes: %i\n", numtimes);


	//cleanup
	free(skips);
	for(i = 0; i < BOARDY; i++){
//		free(board[i]);
//todo
	}
	free(board);

	for(i = 0; i < wordcount; i++){
		free(wordlist[i]);
	}
	free(wordlist);

	return 0;
}
