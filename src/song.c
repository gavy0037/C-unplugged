#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"../include/song.h"

Song *create_song(int id , char *name , char* A_name , char *duration){
    Song *newsong = malloc(sizeof(Song));
    newsong->id = id ;
    strcpy(newsong->name , name);
    strcpy(newsong->A_name , A_name);
    strcpy(newsong->duration , duration);
    newsong->duration[5] = '\0';
    newsong->next = NULL ;
    return newsong;
}

void add_song(Song **head , Song *newsong){
    if((*head) == NULL){
        (*head) = newsong ;
        return ;
    }
    Song *temp = (*head);
    while(temp->next != NULL) temp = temp->next ;

    temp->next = newsong ;
    return ;
}

void print_all_songs(Song *head){
    if(head == NULL){
        printf("Empty list\n");
        return ;
    }
    while(head!=NULL){
        printf("Id: %d    Song Name: %s    Artist's Name: %s    Duration: [%s]\n"
        ,head->id , head->name , head->A_name , head->duration );
        head = head->next ;
    }
}

void free_song_list(Song **head){
    if((*head) == NULL) return ;

    Song *temp = (*head);
    while(temp != NULL){
        Song *next = temp->next ;
        free(temp);
        temp = next ;
    }
    (*head) = NULL ;
}

Song *find_song_by_id(Song *head , int id ){
    while(head != NULL){
        if(head->id == id){
            return head ;
        }
        head = head->next ;
    }
    return NULL ;
}

Song* load_songs(char *filename){//file is like id,name,artis,duration
    FILE *file ;
    file = fopen(filename , "r");
    if(file == NULL){
        printf("ERROR : *** Could not load song ***\n");
        return NULL ;
    }
    Song *head ;
    head = NULL ;
    char getter[256];
    while(fgets(getter , sizeof(getter),file) != NULL){
        getter[strcspn(getter , "\n")] = '\0';

        char *token = strtok(getter , ",");
        if(token == NULL) continue ;

        int id = atoi(token);
        char *name = strtok(NULL , ",");
        char *artist = strtok(NULL , ",");
        char *dur = strtok(NULL , ",");

        if(name && artist && dur){
            Song *newsong = create_song(id , name , artist , dur);
            add_song(&head , newsong);
        }
    }

    fclose(file);
    return head ;
}


void save_songs(char *filename , Song *head){
    FILE* file ;
    file = fopen(filename,"w");

    if(file == NULL){
        printf("WARNING : *** Failed to add songs ***\n");
        return ;
    }

    Song* temp = head ;
    while(temp != NULL){
        fprintf(file , "%d,%s,%s,%s\n",temp->id, temp->name , temp->A_name , temp->duration);
        temp = temp->next ;
    }
    fclose(file);
}