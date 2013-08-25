#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 500 //Maximum size for a string

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
Removes "/*" at the start of a string
*/
char* removeProblemIndicator(char*line){
	char edittedLine[N];
	init(edittedLine);
	if(line[0]=='/' && line[1]=='*'){
		strcpy(edittedLine,&line[2]);
		strcpy(line,edittedLine);
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
			return addCRLF(line);
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
*/
int findLastWordBeforeX60(char* line){
	int lastCR = lastCRPosition(line);
	int i = lastCR+60;
	while(i>lastCR && line[i]!=' '){
		i--;
	}
	return i;
}

/*
Checks if there is a need to amend the line
*/
int needToDoSomething(char* line){
	return strlen(line)-lastCRPosition(line)+1>60;
}

/*
Format the line so that it can fit in a text box
*/
char* format60(char* line){
	char edittedLine[N];
	init(edittedLine);
	while(needToDoSomething(line)){
		int pos = findLastWordBeforeX60(line);
		if(pos>=0){
			line[pos]='\0';
			sprintf(edittedLine,"%s\\n%s",line,&line[pos+1]);
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
Do all the necessary changes
*/
char* format(char* line){
	if(line[0]!='\n'){
		removeProblemIndicator(line);
		removeComment(line);
		if(size(line)>60 && noTag(line)){
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
	int lines=0;
    char line[N];
    while(fgets(line,sizeof line,read)!=NULL){
        if(lines%2==1){
            fprintf(write,"%s",format(line));
        }else{
			fprintf(write,"%s",line);
		}
		lines++;
    }
    fclose(read);
	fclose(write);
    return EXIT_SUCCESS;
}
