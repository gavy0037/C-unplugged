#ifndef LIBRARY_H
#define LIBRARY_H

#include "song.h"
#include "album.h"

struct Album ;

typedef struct Library {
    Song *songs;
    
    Album* albums ;
}Library;

Library* create_library();
void free_library(Library **lib);

void load_library(char *songfile, char *albumfile);
void save_library(Library *lib, char *songsfile, char *albumsfile);

void print_lib_song(Library *lib);
Song* search_song_in_lib(Library *lib, int id);
void add_song_to_lib(Library **lib, Song *song);
void remove_song_from_lib(Library **lib, int id);

void list_all_albums(Library *lib);
Album* find_album_by_id(Library *lib, int id);
void add_album_to_library(Library *lib, Album *album);
void remove_album_from_library(Library *lib, int id);

#endif
