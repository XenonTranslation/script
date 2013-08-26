/**********************************************************************
	Xenon script formatter

 - Formats the text so it can fit in the Xenon text box.
 - Adds indentations during dialogues
 - A line containing '<' will not be formatted (as any line containing
this contains special effects)
 - Removes /* at start of line
 - Removes comments (starting by //) (/!\ This does not remove the line)
 - if for any reason you want a line not to be formatted, just put "<>"
right at the start of the line. This tag will be removed and the line will
not be formatted.
 - Removes lines starting with #. This way, you can put whole line comments.

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 500 //Maximum size for a string

//Global variables :
int lines=0; //line counter


/*
Initializes an empty string
*/
void init(char* str){
	int i;
	for(i=0;i<N;i++){
		str[i]='\0';
	}
}

/*
Creates a Japanese space
*/
char* japaneseSpace(char* str){
	str[0]=-127;
	str[1]=64;
	str[2]='\0';
	return str;
}

/*
Inserts several spaces (so that the indentation in dialogues
may be accurate)
*/
char* insertSpaces(char* spaces, int nameSize){
	int i=0;
	if(nameSize!=0){
		char space[3];
		japaneseSpace(space);
		while(i<nameSize){
			spaces[i]=space[0];
			spaces[i+1]=space[1];
			i+=2;
		}
		if(nameSize%2==0){
			spaces[i]=' ';
			i++;
		}
	}
	spaces[i]='\0';
	return spaces;
}

/*
Find the first colon in the string
Returns 0 if no colon is found (there cannot be
a colon in position 0 as the first character is
always '[' in a dialogue.
*/
int findColon(char* line){
	int i = 0;
	while(i<strlen(line) && line[i]!=':'){
		i++;
	}
	if(i>=strlen(line)){
		return 0;
	}else{
		return i;
	}
}

/*
Returns the size of the name :
"[Ryouko]:" for example is size 8.
"[Me]:" is size 4
*/
int nameSize(char* line){
	if(line[0]=='['){
		return findColon(line);
	}else{
		return 0;
	}
}

/*
Removes "/*" at the start of a string
*/
char* removeProblemIndicator(char*line, int* mustNotBeFormatted){
	char edittedLine[N];
	init(edittedLine);
	if(line[0]=='/' && line[1]=='*'){
		strcpy(edittedLine,&line[2]);
		strcpy(line,edittedLine);
	}else if(line[0]=='<' && line[1]=='>'){
		strcpy(edittedLine,&line[2]);
		strcpy(line,edittedLine);
		*mustNotBeFormatted=1;
	}
	return line;
}

/*
Adds \n and \0 at the end of a line
*/
char* addCRLF(char* line){
	int i = 0;
	while(line[i]!='\0'){
		i++;
	}
	line[i]='\n';
	line[i+1]='\0';
	return line;
}

/*
Remove comments (things that starts with //)
*/
char* removeComment(char* line){
	char edittedLine[N];
	init(edittedLine);
	char c1;
	char c2;
	int i=0;
	while((c2=line[i+1])!='\n'){
		c1=line[i];
		if(c1=='/' && c2=='/'){
			strncpy(edittedLine,line,i);
			printf("Edited : %s , %s",edittedLine,line);
			strcpy(line,edittedLine);
			addCRLF(line);
			return line;
		}
		i++;
	}
	return line;
}

/*
Useful to understand how works findLastWordBeforeX60()
*/
int findLastWordBefore60(char* line){
	int i = 59;
	while(i>=0 && line[i]!=' '){
		i--;
	}
	return i;
}

/*
Returns the last occurrence of "\\n" in the string
Returns the position of the "n". If not occurrence is found, returns -1;
Useless as it stands now
*/
int lastCRPosition(char *line){
	int i = strlen(line)-1;
	while(i>=1 && (line[i]!='n' || line[i-1]!='\\')){
		i--;
	}
	if(i==0){return -1;}else{return i;}
}

/*
Gives the position of the last space in the line before the end of a
line in a text box (60 characters)
deleteSpace is TRUE if the space must be deleted, FALSE if it must be
replace by \n.
*/
int findLastWordBeforeX60(char* line,int* deleteSpace, int endLinePosition){
	int lastCR = endLinePosition;
	int i = lastCR+61;
	while(i>lastCR && line[i]!=' '){
		i--;
	}
	if(i==lastCR+61){
		*deleteSpace=1;
	}
	return i;
}

/*
Checks if there is a need to amend the line
*/
int needToDoSomething(char* line,int endLinePosition){
	return strlen(line)-endLinePosition+1>60;
}

/*
Format the line so that it can fit in a text box.
deleteSpace is TRUE if the space must be deleted, FALSE if it must be
replace by \n.
*/
char* format60(char* line){
	char edittedLine[N];
	init(edittedLine);
	int endLinePosition=-1;
	while(needToDoSomething(line,endLinePosition)){
		int deleteSpace=0;
		int pos = findLastWordBeforeX60(line,&deleteSpace,endLinePosition);
		if(pos>=0){
			line[pos]='\0';
			char spaces[N];
			int name_size = nameSize(line);
			if(deleteSpace){
				sprintf(edittedLine,"%s%s%s",line,insertSpaces(spaces,name_size),&line[pos+1]);
				endLinePosition=pos-1;
			}else{
				sprintf(edittedLine,"%s\\n%s%s",line,insertSpaces(spaces,name_size),&line[pos+1]);
				endLinePosition=pos+1;
			}
			strcpy(line,edittedLine);
		}
	}
	return line;
}

/*
Returns the size of the line
*/
int size(char* line){
	int i=0;
	while(line[i]!='\n'){
		i++;
	}
	return i;
}

/*
Checks if there is any tag in the line.
If there is one, the line will not be formatted (you'll have
to do that manually). But this is a good thing as lines with tags
tend to be quite special.
*/
int noTag(char *line){
	int i=0;
	while(line[i]!='\n'){
		if(line[i]=='<'){
			return 0;
		}
		i++;
	}
	return 1;
}

/*
Functions that checks if there are more than 3 spaces
in a row. If so is the case, then the program is interrupted
(as the game will crash when it reaches this line)
*/
void checkSpaces(char* line){
	int c[3];
	int i = 0;
	while(i+2<strlen(line)){
		c[0]=line[i];
		c[1]=line[i+1];
		c[2]=line[i+2];
		if(c[0]==' ' && c[1]==' ' && c[2]==' '){
			printf("Line #%d : \"%s has more than 3 spaces in a row. Aborting...",lines,line);
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

/*
Do all the necessary changes
*/
char* format(char* line){
	if(line[0]!='\n'){
		int mustNotBeFormatted=0;
		removeProblemIndicator(line,&mustNotBeFormatted);
		removeComment(line);
		checkSpaces(line);
		if(size(line)>60 && noTag(line) && !mustNotBeFormatted){
			printf("%s %d",line,size(line));
			format60(line);
		}
	}
	return line;
}


/*
main : takes the input and output files as argument
*/
int main(int argc, char *argv[]){
    printf("Opening file %s\n",argv[1]);
	printf("Writing in file %s\n",argv[2]);
    FILE* read = fopen(argv[1],"r");
	FILE* write = fopen(argv[2],"w");
	int noCommentLines=0;
    char line[N];
    while(fgets(line,sizeof line,read)!=NULL){
		if(line[0]!='#'){
			if(noCommentLines%2==1){
				fprintf(write,"%s",format(line));
			}else{
				fprintf(write,"%s",line);
			}
			noCommentLines++;
		}
		lines++;
    }
    fclose(read);
	fclose(write);
    return EXIT_SUCCESS;
}
