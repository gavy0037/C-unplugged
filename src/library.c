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

Library* load_library(char *songfile , char *albumfile){
    Library *lib = create_library();

    lib->songs = load_songs(songfile);
    lib->albums = load_albums(albumfile ,lib);
    return lib ;
}

void save_library(Library *lib , char *songsfile , char *albumsfile){
    save_albums(albumsfile , lib->albums);
    save_songs(songsfile , lib->songs);
}

void print_lib_song(Library *lib){
    if(lib == NULL || lib->songs == NULL){
        printf("\n---NO SONGS IN LIBRARY---\n");
        return ;
    }
    printf("\n--- SONGS IN LIBRARY ---\n");
    print_all_songs(lib->songs);
}

Song* search_song_in_lib(Library *lib , int id){
    if(lib == NULL) return NULL ;
    return find_song_by_id(lib->songs , id);
}

void add_song_to_lib(Library **lib , Song *song){
    if((*lib) == NULL || song == NULL) return ;
    add_song(&((*lib)->songs) , song);
}

void remove_song_from_lib(Library **lib , int id){
    Song *song = (*lib)->songs, *prev = NULL ;
    while(song != NULL){
        if(song->id == id){
            if(prev != NULL) prev->next = song->next ;
            else (*lib)->songs = song->next ;
            free(song);
            return ;
        }
        prev = song ;
        song = song->next ;
    }
    printf("Not found\n");
}

void list_all_albums(Library *lib){
    if(lib == NULL || lib->albums == NULL){
        printf("NO ALBUMS IN LIBRARY\n");
        return ;
    }
    printf("\n--- ALBUMS IN LIBRARY ---\n");
    Album *album = lib->albums ;
    while(album != NULL){
        print_album(album) ;
        album = album->next;
    }
}

Album* find_album_by_id(Library *lib , int id){
    if(lib == NULL) return NULL ;
    Album *album = lib->albums;
    while(album != NULL){
        if(album->id == id) return album ;

        album = album->next ;
    }
    printf("NO ALBUM FOUND WITH THIS ID IN LIBRARY\n");
    return NULL ;
}

void add_album_to_library(Library *lib , Album *album){
    if(lib== NULL || album == NULL) return ;

    add_album_to_list(&(lib->albums)  , album);
}

void remove_album_from_library(Library *lib , int id){
    if(lib == NULL || lib->albums == NULL) return ;
    Album *curr = lib->albums , *prev = NULL ;
    while(curr != NULL){
        if(curr->id == id){
            if(prev != NULL)prev->next = curr->next ;
            else lib->albums = curr->next ;
            free(curr);
            return ;
        }
        prev = curr ;
        curr = curr->next ;
    }
    printf("ALBUM NOT FOUND\n");
}