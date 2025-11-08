#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"../include/playlist.h"

Playlist* create_playlist(){
    Playlist *newplaylist = malloc(sizeof(Playlist));
    newplaylist->curr = NULL; ;
    newplaylist->size = 0 ;
    return newplaylist ;
}

void add_song_to_playlist(Playlist* playlist , Song *song){//add song to the next of current song
    playlist->size++;
    Playlistnode* newpl = malloc(sizeof(Playlistnode));
    newpl->song = song ;
    if(playlist->curr == NULL){
        playlist->curr = newpl ;
        newpl->next = newpl->prev = newpl ;
    }else{
        Playlistnode* temp = playlist->curr->next ;
        playlist->curr->next = newpl ;
        temp->prev = newpl ;
        newpl->next = temp ;
        newpl->prev = playlist->curr ;
    }
    printf("---SONG ADDED SUCCESFULLY---\n");
}

void remove_current_song(Playlist *pl){
    pl->curr->prev->next = pl->curr->next ;
    pl->curr->next->prev = pl->curr->prev ;
    pl->curr->next = pl->curr->prev = NULL ;
    free(pl->curr);
}

void play_current_song(Playlist *pl){
    if(pl->curr == NULL){
        printf("---PLAYLIST IS EMPTY---\n");
    }else{
        Song *song = pl->curr->song ;
        printf("** PLAYING ** - ID: %d Name: %s Artist's Name: %s Duration: %s\n",
            song->id , song->name , song->A_name , song->duration);
    }
}

void play_next_song(Playlist* pl){
    if(pl->curr == NULL){
        printf("EMPTY PLAYLIST\n");
        return ;
    }
    pl->curr = pl->curr->next ;
    play_current_song(pl);
}

void play_prev_song(Playlist* pl){
    if(pl->curr == NULL){
        printf("EMPTY PLAYLIST\n");
        return ;
    }
    pl->curr = pl->curr->prev ;
    play_current_song(pl);
}

void add_album_to_playlist(Playlist *pl , Album *album){
    Song *song = album->songlist ;
    int count = 0 ;
    while(song != NULL){
        add_song_to_playlist(pl , song);
        song = song->next ;
        count++;
    }
    pl->size+=count ;
}

void clear_playlist(Playlist *pl){
    if(pl->curr == NULL) return ;
    Playlistnode *st = pl->curr  , *temp = pl->curr ;
    do{
        Playlistnode *next = temp->next ;
        free(temp);
        temp = next ;
    }while(st != temp);

    pl->size = 0 ;

}

void print_playlist(Playlist *pl){
    printf("\n---PLAYLIST---\n");
    Playlistnode *st = pl->curr  , *temp = pl->curr ;
    do{
        printf("Song id: %d Song's name: %s\n" , 
            temp->song->id , temp->song->name);
        temp = temp->next ;

    } while(st != temp) ;
}

//file storage name of playlist songID 1|2|3 ....

