#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"../include/playlist.h"

Playlist* create_playlist(char *name) {
    Playlist *newplaylist = malloc(sizeof(Playlist));
    if (newplaylist == NULL) {
        return NULL;
    }
    newplaylist->curr = NULL;
    newplaylist->size = 0;
    newplaylist->next = NULL;
    strcpy(newplaylist->name, name);
    return newplaylist;
}

Playlist* find_playlist_by_name(Playlist *head  , char *name){
    Playlist *temp = head ;
    while(temp != NULL){
        if(strcmp(temp->name , name) == 0) return temp ;
        temp = temp->next ;
    }
    printf("---PLAYLIST NOT FOUND---\n");
    return NULL ;
}

void add_song_to_playlist(Playlist* playlist , Song *song){//add song to the next of current song
    Playlistnode* newpl = malloc(sizeof(Playlistnode));
    newpl->song = song ;
    if(playlist->curr == NULL){
        newpl->next = newpl->prev = newpl ;
        playlist->curr = newpl ;
    }else{
        Playlistnode* temp = playlist->curr->next ;
        playlist->curr->next = newpl ;
        temp->prev = newpl ;
        newpl->next = temp ;
        newpl->prev = playlist->curr ;
    }
    playlist->size++;
}

void remove_current_song(Playlist *pl){
    if(pl == NULL || pl->curr == NULL) return ;
    Playlistnode *temp = pl->curr->next ;
    pl->curr->prev->next = pl->curr->next ;
    pl->curr->next->prev = pl->curr->prev ;
    pl->curr->next = pl->curr->prev = NULL ;
    free(pl->curr);
    pl->curr = temp ;

    pl->size--;
}

void play_current_song(Playlist *pl){
    if(pl->curr == NULL){
        printf("---PLAYLIST IS EMPTY---\n");
    }else{
        Song *song = pl->curr->song ;
        printf("\n** PLAYING ** - ID: %d Name: %s Artist's Name: %s Duration: [%s]\n",
            song->id , song->name , song->A_name , song->duration);
    }
}

void play_next_song(Playlist* pl){
    if(pl->curr == NULL){
        printf("EMPTY PLAYLIST\n");
        return ;
    }
    pl->curr = pl->curr->next ;
}

void play_prev_song(Playlist* pl){
    if(pl->curr == NULL){
        printf("EMPTY PLAYLIST\n");
        return ;
    }
    pl->curr = pl->curr->prev ;
}

void add_album_to_playlist(Playlist *pl , Album *album){
    if(pl == NULL || album == NULL) return ;
    Song *song = album->songlist ;
    while(song != NULL){
        add_song_to_playlist(pl , song);
        song = song->next ;
    }
}

void clear_playlist(Playlist *pl){
    if(pl->curr == NULL) return ;
    Playlistnode *st = pl->curr  , *temp = pl->curr->next ;
    while (temp != st) {
        Playlistnode *next = temp->next;
        free(temp);
        temp = next;
    }
    free(st);
    pl->curr = NULL;
    pl->size = 0;
}

void print_playlist(Playlist *pl){
    if(pl->curr == NULL){
        printf("PLAYLIST IS EMPTY\n");
        return ;
    }
    printf("\n--- PLAYLIST: %s ---\n", pl->name);
    Playlistnode *st = pl->curr  , *temp = pl->curr ;
    do{
        printf("-> %s " ,temp->song->name);
        temp = temp->next ;
    } while(st != temp) ;
    printf("\n");
}

void add_playlist(Playlist **head , Playlist *newpl){
    if(newpl == NULL) return ;
    newpl->next = NULL ;
    if((*head) == NULL){
        *head = newpl ;
        return ;
    }

    Playlist *temp = (*head);
    while(temp->next != NULL) temp = temp->next ;
    temp->next = newpl ;
}

void remove_playlist_by_name(Playlist **head , char *name){
    if(head == NULL || *head == NULL) return ;

    Playlist *temp = (*head) , *prev ;
    
    while(temp != NULL){
        if(strcmp(temp->name,name) == 0){
            if(prev == NULL )prev->next = temp->next ;
            else *head = temp->next ;
            temp->next = NULL ;
            free(temp);
            printf("SUCCESFULLY DELETED\n");
            return ;
        }
        prev = temp ;
        temp = temp->next ;
    }
    printf("NOT FOUND\n");
}

// format of playlist.txt playlist name , Song ids 1|2|3|4

Playlist *load_playlists(char *filename , Library *lib){
    FILE *file ;
    file = fopen(filename , "r");
    if(file == NULL){
        printf("FAILED TO OPEN PLAYLIST.txt\n");
        return NULL ;
    }
    char line[250];
    Playlist *head = NULL ;
    while(fgets(line , sizeof(line) , file) != NULL){
        line[strcspn(line, "\n")] = 0;
        char name[100] , songids[200];
        sscanf(line , "%[^,],%s",name , songids);
        Playlist *newpl = create_playlist(name);
        char *token = strtok(songids , "|");
        while(token != NULL){
            int id = atoi(token);
            Song *song = search_song_in_lib(lib , id);
            if(song != NULL) add_song_to_playlist(newpl , song);
            token = strtok(NULL , "|");
        }
        add_playlist(&head , newpl);
    }
    fclose(file);

    return head ;
}   

void save_playlist(char *filename , Playlist *head){
    FILE *file ;
    file = fopen(filename , "w");
    if(file == NULL){
        printf("FAILED TO OPEN PLAYLIST.txt\n");
        return ;
    }
    while(head != NULL){
        fprintf(file , "%s," , head->name);
        Playlistnode *st , *temp ;
        st = temp = head->curr ;
        if(st != NULL){
            do{
                int id = temp->song->id ;
                fprintf(file , "%d" , id);
                if(temp->next != st){
                    fprintf(file ,"|");
                }
                temp = temp->next;
            }while(st != temp);
        }
        
        fprintf(file , "\n");
        head = head->next ;
    }
    printf("ALL THE CURRENT PLAYLISTS HAVE BEEN SUCCESFULLY STORED\n");
    fclose(file);

}