#ifndef ALBUM_H
#define ALBUM_H

#include"song.h"

typedef struct Album{
    int id ;
    char name[100];
    Song *songlist ;
    struct Album *next ;    
}Album ;

Album* create_album(int id , char *name);

void add_song_to_album(Album** album , Song *song);//will add the song at last
void remove_song_from_album(Album** album , Song *song);
void print_album(Album* album);
void free_all_albums(Album **list);
void add_album_to_list(Album **list , Album *newalbum);

Album* load_albums(char *filename , Library *lib);
void save_albums(char *filename, Album *list);

#endif
