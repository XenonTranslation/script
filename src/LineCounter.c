#include <stdio.h>
#include <stdlib.h>

#define N 500

int main(int argc, char *argv[]){
    printf("Opening file %s\n",argv[1]);
    FILE* file = fopen(argv[1],"r");
    int lines=0;
    int emptyLines=0;
    int mustBeAmended=0;
    char line[N];
    while(fgets(line,sizeof line,file)!=NULL){
        if(lines%2==1){
            if(line[0]=='\n'){
                emptyLines++;
            }else if(line[0]=='/' && line[1]!='\0' && line[1]=='*'){
                mustBeAmended++;
            }
        }
        lines++;
    }
    printf("Current state : %f%%\nStill %d lines to be amended...\n", 100.0*(1.0-(float) emptyLines/(float) (lines/2)),mustBeAmended);
    system("PAUSE");
    return EXIT_SUCCESS;
}
