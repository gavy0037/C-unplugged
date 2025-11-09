#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"../include/album.h"
#include"../include/library.h"

Album* create_album(int id , char *name){
    Album *newalbum = (Album*)malloc(sizeof(Album));
    newalbum->id = id ;
    strcpy(newalbum->name,name) ;
    newalbum->next = NULL ;
    newalbum->songlist = NULL ;
    return newalbum ;
}

void add_song_to_album(Album** album , Song *song){
    if((*album) == NULL){
        return ;
    }
    add_song(&((*album)->songlist) , song);
}

void add_album_to_list(Album **list , Album *newalbum){
    if((*list) == NULL){
        (*list) = newalbum;
        return ;
    }
    Album *temp = (*list);
    while(temp->next != NULL){
        temp = temp->next ;
    }
    temp->next = (newalbum);
}

void remove_song_from_album(Album** album , Song *song){
    Song* temp = (*album)->songlist , *prev = NULL ;
    // if(temp != NULL && temp->id == song->id){
    //     prev = temp->next ;
    //     free(temp);
    //     (*album)->songlist = prev; 
    //     return ;
    // }
    while(temp != NULL){
        if(temp->id == song->id){
            if(prev) prev->next = temp->next ;
            else (*album)->songlist = temp->next ;
            free(temp);
            return; 
        }
        prev = temp ;
        temp = temp->next ;
    }
    printf("Song not found\n");
}

void print_album(Album* album){
    printf("\n === ID: %d\tAlbum: %s ===\n",album->id , album->name);
    print_all_songs(album->songlist);
}

void free_all_albums(Album** list){
    if((*list) == NULL){
        printf("***** Already empty ****\n");
        return ;
    }
    Album *temp = (*list);
    while(temp != NULL ){
        Album *next = temp->next ;
        free_song_list(&(temp->songlist));  
        free(temp);
        temp = next ;
    }
    (*list) = NULL ;
}

Album* load_albums(char *filename , Library *lib){//storage: id , name , id s of songs
    FILE *file ;
    file = fopen(filename,"r");
    if(file == NULL){
        printf("Could not find album file\n");
        return NULL ;
    }
    Album *albumlist = NULL ;
    char line[100];
    while(fgets(line , sizeof(line) , file) != NULL){
        int id ;
        char name[100] , songids[200];
        sscanf(line , "%d,%[^,],%s" , &id , name , songids);
        Album *temp = create_album(id , name);
        char *token = strtok(songids , "|");
        while(token != NULL){
            int id = atoi(token);
            Song *newsong = find_song_by_id(lib->songs , id);//to give the lib->songs
            if(newsong != NULL){
                Song *copy = create_song(newsong->id , newsong->name , newsong->A_name , newsong->duration);
                add_song_to_album(&temp , copy);
            }
            token = strtok(NULL , "|");
        }
        add_album_to_list(&albumlist , temp);
    }
    fclose(file);
    return albumlist ;
}

void save_albums(char *filename , Album *list){
    FILE* file ;
    file = fopen(filename , "w");
    if(file == NULL){
        printf("NO FILE FOUND\n");
        return ;
    }
    Album *curr = list ;
    while(curr != NULL){
        fprintf(file,"%d,%s,",curr->id , curr->name);
        Song *song = curr->songlist ;
        while(song != NULL){
            fprintf(file , "%d" , song->id);
            if(song->next != NULL) fprintf(file , "|");
            song = song->next ;
        }
        fprintf(file , "\n");
        curr = curr->next ;
    }
    fclose(file);
}
