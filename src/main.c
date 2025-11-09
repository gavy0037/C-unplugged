#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include "album.h"
#include "song.h"
#include "playlist.h"

#define SONGS_FILE "data/songs.txt"
#define ALBUMS_FILE "data/albums.txt"
#define PLAYLISTS_FILE "data/playlist.txt"
#define LOG_FILE "data/commands.log"


void log_command(const char *command) {
    FILE *log = fopen(LOG_FILE, "a");
    if (!log) return;
    fprintf(log, "%s\n", command);
    fclose(log);
}

int main() {
    Library *lib = load_library(SONGS_FILE, ALBUMS_FILE);
    Playlist *playlists = NULL;
    int songid = 0, albumid = 0;
    Song *song = lib->songs ;
    while(song != NULL){
        song = song->next ;
        songid++ ;
    }
    Album *album = lib->albums ;
    while(album != NULL){
        album = album->next ;
        albumid++;
    }
    playlists = load_playlists(PLAYLISTS_FILE, lib);
    int option ;
    char name[100];//used to take the inputs of newly added things ;

    while (1) {

        printf("\n-----------------------------------\n");
        printf("|         C-unplugged menu        |\n");
        printf("-----------------------------------\n");
        printf("INSTRUCTIONS FOR ADDING A NEW PLAYLIST/ALBUM : Do not add a name with spaces\n");
        printf("1. View all songs\n");
        printf("2. View all albums\n");
        printf("3. View all Playlists\n");
        printf("4. Create new Album\n");
        printf("5. Add song to Album\n");
        printf("6. Create new Playlist\n");
        printf("7. Add song to playlist\n");
        printf("8. Add album to playlist\n");
        printf("9. Remove song from Album\n");
        printf("10. Play Songs through playlists\n");//FIRST LIST ALL THE PLAYLISTS AND THEN PROMPT THE USER TO SELECT PLAYLIST add features in this about play next prev or delete this current song or add song in this playlist
        
        //add sub features like after creating a album then if you want to add song to this album or playlist
        printf("10. Exit App\n");
        printf("======================\n");
        printf("Enter choice: ");
        scanf("%d", &option);
        getchar();// to get the extra newline so that it do not affect the input of names of new things
        
        switch(option){
            case 1:
                print_all_songs(lib->songs);
                log_command("View_songs");
                break;

            case 2:
                list_all_albums(lib);
                log_command("View_albums");
                break;

            case 3:
                Playlist *temp = playlists;
                while (temp) {
                    print_playlist(temp);
                    temp = temp->next;
                }
                log_command("View_playlists");
                break;
            case 4:
                printf("Enter new album name: ");
                scanf("%s",name);
                getchar();
                name[strcspn(name, "\n")] = 0;
                Album* album = create_album( ++albumid , name);
                add_album_to_library(lib, album);
                log_command("Created_new_Album");
                while(1){
                    char ch  ;
                    printf("Do you wish to add song to this album\n[y/n] :");
                    scanf("%c",&ch);
                    getchar();
                    print_all_songs(lib->songs);
                    if(ch == 'y'){
                        int ssid ;
                        printf("Enter song id to add: ");
                        scanf("%d", &ssid);
                        getchar();
                        Song *song = search_song_in_lib(lib, ssid);
                        if (song == NULL) {
                            printf("Song not found in library.\n");
                            break;
                        }
                        Song *copy = create_song(song->id, song->name, song->A_name, song->duration);
                        add_song_to_album(album, copy);
                        log_command("Add_song_to_album");
                    }else if(ch == 'n') break ;

                    else{
                        printf("Please enter a character from y or n\n");
                    }
                }
                break;
            case 5:
                int albumid ;
                list_all_albums(lib);
                printf("Enter album id to add song to: ");
                scanf("%d", &albumid);
                getchar();
                while(1){
                    int songid;
                    Album *alb = find_album_by_id(lib, albumid);
                    if (alb == NULL) {
                        printf("Album not found.\n");
                        break;
                    }
                    print_all_songs(lib->songs);
                    printf("Enter song id to add: ");
                    scanf("%d", &songid);
                    getchar();
                    Song *song = search_song_in_lib(lib, songid);
                    if (song == NULL) {
                        printf("Song does not exist\n");
                        break;
                    }
                    Song *copy = create_song(song->id, song->name, song->A_name, song->duration);
                    add_song_to_album(&alb, copy);
                    log_command("Add_song_to_album");
                    printf("Do you wish to add another song to this album\n[y/n]");
                    char ch ;
                    scanf("%c",&ch);
                    if(ch == 'n') break ;
                    else if(ch != 'n' && ch != 'y'){
                        printf("Invalid choice\n");
                    }
                }
                break;
            case 6:
                printf("Enter new playlist name: ");
                scanf("%s" , name);
                name[strcspn(name, "\n")] = 0;
                Playlist *newpl = create_playlist(name);
                add_playlist(&playlists, newpl);
                log_command("Create_playlist");
                while(1){
                    char ch  ;
                    printf("Do you wish to add song to this Playlist\n[y/n]: ");
                    scanf("%c",&ch);
                    getchar();
                    if(ch == 'y'){
                        int ssid ;
                        print_all_songs(lib->songs);
                        printf("Enter song id to add: ");
                        scanf("%d", &ssid);
                        getchar();
                        Song *song = search_song_in_lib(lib, ssid);
                        if (song == NULL) {
                            printf("Song does not exist\n");
                            break;
                        }
                        Song *copy = create_song(song->id, song->name, song->A_name, song->duration);
                        add_song_to_album(album, copy);
                        log_command("Add_song_to_album");
                    }else if(ch == 'n') break ;

                    else{
                        printf("Please enter a character from y or n\n");
                    }
                }
                break;
            case 7:
                if (playlists == NULL) {
                    printf("No playlists available.\n");
                    break;
                }
                printf("Enter playlist name to add songs to: ");
                scanf("%s",name);
                getchar();
                name[strcspn(name, "\n")] = 0;
                Playlist *pl = find_playlist_by_name(playlists, name);
                while(1){
                    if (pl == NULL) break;
                    int sid;
                    print_all_songs(lib->songs);
                    printf("Enter song id to add: ");
                    scanf("%d", &sid);
                    getchar();
                    Song *sng = search_song_in_lib(lib, sid);
                    if (sng == NULL) {
                        printf("Song not found in library.\n");
                        break;
                    }
                    add_song_to_playlist(pl, sng);
                    log_command("add_song_to_playlist");
                    printf("Do you wish to add another song to this playlist\n[y/n]");
                    char ch ;
                    scanf("%c",&ch);
                    getchar();

                }
                break;
        }
    }
    return 0;
}