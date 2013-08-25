#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 500

void init(char* str){
	int i;
	for(i=0;i<N;i++){
		str[i]='\0';
	}
}

char* removeProblemIndicator(char*line){
	char edittedLine[N];
	init(edittedLine);
	if(line[0]=='/' && line[1]=='*'){
		strcpy(edittedLine,&line[2]);
		strcpy(line,edittedLine);
	}
	return line;
}

char* addCRLF(char* line){
	int i = 0;
	while(line[i]!='\0'){
		i++;
	}
	line[i]='\n';
	line[i+1]='\0';
	return line;
}

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

int findLastWordBefore60(char* line){
	int i = 59;
	while(i>=0 && line[i]!=' '){
		i--;
	}
	return i;
}

int findLastWordBeforeX60(char* line, int count){
	int i = 60*count+count-2;
	while(i>=60*(count-1)+count-1 && line[i]!=' '){
		i--;
	}
	return i;
}

int needToDoSomething(char* line, int count){
	if(count==1){ return 1;}
	else{return 0;}
}

char* format60(char* line){
	char edittedLine[N];
	init(edittedLine);
	int count=1;
	while(needToDoSomething(line,count)){
		int pos = findLastWordBeforeX60(line,count);
		if(pos>=0){
			line[pos]='\0';
			sprintf(edittedLine,"%s\\n%s",line,&line[pos+1]);
			strcpy(line,edittedLine);
		}
		count++;
	}
	return line;
}

int size(char* line){
	int i=0;
	while(line[i]!='\n'){
		i++;
	}
	return i;
}

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
    system("PAUSE");
    return EXIT_SUCCESS;
}
