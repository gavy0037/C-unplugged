#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"../include/song.h"
#include"../include/album.h"
#include"../include/library.h"

Library* create_library(){
    Library* newlib = (Library*)malloc(sizeof(Library));
    newlib->songs = NULL ;
    newlib->albums = NULL ;
    return newlib ;
}

void free_library(Library **lib){
    if(lib == NULL || (*lib) == NULL) return ;
    free_song_list(&((*lib)->songs));
    free_all_albums(&((*lib)->albums));

    free(*lib);
    (*lib) = NULL ;
}

Library* load_library(Library **lib , char *songfile , char *albumfile){
    (*lib)->songs = load_song(songfile);
    (*lib)->albums = load_albums(albumfile);
    return *lib ;
}

void save_library(Library *lib , char *songsfile , char *albumsfile){
    save_albums(albumsfile , lib->albums);
    save_songs(songsfile , lib->songs);
}

void print_lib_song(Library *lib){
    printf("\n--- SONGS IN LIBRARY ---\n");
    print_all_songs(lib->songs);
}

Song* search_song_in_lib(Library *lib , int id){
    return find_song_by_id(lib->songs , id);
}

void add_song_to_lib(Library **lib , Song *song){
    add_song(&((*lib)->songs) , song);
}

void remove_song_from_lib(Library **lib , int id){
    Song *song = (*lib)->songs, *prev = NULL ;
    if(song->id == id){
        prev = song->next ;
        free(song);
        (*lib)->songs = prev ;
        return ;
    }
    while(song != NULL){
        if(song->id == id){
            prev->next = song->next ;
            free(song);
            return ;
        }
        prev = song ;
        song = song->next ;
    }
    printf("Not found\n");
}

void list_all_albums(Library *lib){
    printf("\n--- ALBUMS IN LIBRARY ---\n");
    Album *album = lib->albums ;
    while(album != NULL){
        print_album(album) ;
        album = album->next;
    }
}

Album* find_album_by_id(Library *lib , int id){
    Album *album = lib->albums;
    while(album != NULL){
        if(album->id == id) return album ;

        album = album->next ;
    }
    printf("NO ALBUM FOUND WITH THIS ID IN LIBRARY\n");
    return NULL ;
}

void add_album_to_library(Library *lib , Album *album){
    Album *a = lib->albums ;
    if(a == NULL){
        lib->albums = album ;
        return ;
    }
    
    while(a->next != NULL) a = a->next ;
    a->next = album ;
}

void remove_album_from_library(Library *lib , int id){
    Album *curr = lib->albums , *prev = NULL ;
    if(curr != NULL && curr->id == id){
        prev = curr->next ;
        free(curr);
        lib->albums = prev ;
        return ;
    }
    while(curr != NULL){
        if(curr->id == id){
            prev->next = curr->next ;
            free(curr);
            return ;
        }
        prev = curr ;
        curr = curr->next ;
    }
    printf("ALBUM NOT FOUND\n");
}