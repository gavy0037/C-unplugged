#ifndef SONG_H
#define SONG_H

#include<stdio.h>
#include<stdlib.h>

typedef struct Song{
    int id ;
    char name[100];
    char A_name[100];
    char duration[5];
    struct Song *next ;
}Song ;

Song* create_song(int id, char *name ,char *A_name,char *duration);
void add_song(Song **head, Song *newSong);
void print_all_songs(Song *head);
void free_song_list(Song **head);
Song *find_song_by_id(Song *head , int id );


Song* load_songs(char *filename);
void save_songs(char *filename , Song *head);

#endif