#include<stdio.h>
#include<string.h>

#include "../include/utils.h"

void view_commands(char *filename){
    FILE *file = fopen(filename , "r");
    printf("\n\n**** COMMAND HISTORY ****\n\n");
    char history[100];
    while(fgets(history , sizeof(history) , file) != NULL){
        history[strcspn(history , "\n")] = 0 ;
        printf("%s\n" , history);
    }
    fclose(file);

    return ;
}