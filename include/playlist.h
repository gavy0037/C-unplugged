#ifndef PLAYLIST_H
#define PLAYLIST_H

#include"song.h"
#include"album.h"
#include"library.h"

typedef struct Playlistnode{
    Song *song ;
    struct Playlistnode *next ;
    struct Playlistnode *prev ;
} Playlistnode ;

typedef struct Playlist{
    char name[100];
    Playlistnode *curr ;
    int size ;
    struct Playlist *next ;
}Playlist ;


Playlist* create_playlist(char *name);
Playlist* find_playlist_by_name(Playlist *head , char *name);
void add_song_to_playlist(Playlist *pl, Song *song);
void remove_current_song(Playlist *pl);
void play_current_song(Playlist *pl);
void play_next_song(Playlist *pl);
void play_prev_song(Playlist *pl);
void add_album_to_playlist(Playlist *pl, Album *album); // adds all album songs in album order to playlist
void clear_playlist(Playlist *pl);
void print_playlist(Playlist *pl);

// void save_playlist(const char *filename, Playlist *pl);
// Playlist* load_playlist(const char *filename, Library *lib);

void add_playlist(Playlist **head  , Playlist *newpl );
void remove_playlist_by_name(Playlist **head , char *name);
Playlist* load_playlist(char *filename , Library *lib);
void save_playlist(char *filename , Playlist *head);

#endif