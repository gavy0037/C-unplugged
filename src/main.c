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


void logger(char *cmd) {
    FILE *log = fopen(LOG_FILE, "a");
    if (!log) return;
    fprintf(log, "%s\n", cmd) ;
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
    Playlist *temp ;
    while (1) {

        printf("\n-----------------------------------\n");
        printf("|         C-unplugged menu        |\n");
        printf("-----------------------------------\n");
        printf("INSTRUCTIONS FOR ADDING A NEW PLAYLIST/ALBUM : NO spaces are allowed and no duplicate names of albums or songs may exist\n");
        printf("1. View all songs\n");
        printf("2. View all albums\n");
        printf("3. View all Playlists\n");
        printf("4. Create new Album\n");
        printf("5. Add song to Album\n");
        printf("6. Create new Playlist\n");
        printf("7. Add song to playlist\n");
        printf("8. Add album to playlist\n");
        printf("9. Remove song from Album\n");
        printf("10. Play Songs through playlists\n");
        printf("11. Remove a Playlist\n");
        //add the feature to delete a playlist
       
        printf("12. Exit App\n");
        printf("======================\n");
        printf("Enter choice: ");
        scanf("%d", &option);
        getchar();// to get the extra newline so that it do not affect the input of names of new things
        
        switch(option){
            case 1:
                {print_all_songs(lib->songs);
                logger("View_songs");
                break;}

            case 2:
                {list_all_albums(lib);
                logger("View_albums");
                break;}

            case 3:
                {
                temp = playlists ;
                while (temp != NULL) {
                    print_playlist(temp);
                    temp = temp->next;
                }
                logger("View_playlists");
                break;}
            case 4:
                {printf("Enter new album name: ");
                scanf("%s",name);
                getchar();
                name[strcspn(name, "\n")] = 0;
                Album* album = create_album( ++albumid , name);
                add_album_to_library(lib, album);
                logger("Created_new_Album");
                while(1){
                    char ch  ;
                    printf("Do you wish to add song to this album [y/n] : ");
                    scanf("%c",&ch);
                    getchar();
                    if(ch == 'n') break ;
                    else if(ch == 'y'){
                        print_all_songs(lib->songs);
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
                        add_song_to_album(&album, copy);
                        logger("Add_song_to_album");
                    }else{
                        printf("Looks like you do not want to add a song to this album\n");
                        break ;
                    }
                }
                break;}
            case 5:
                {int albumid ;
                list_all_albums(lib);
                printf("\nEnter album id to add song to: ");
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
                    printf("SONG ADDED SUCCESFULLY\n");
                    logger("Add_song_to_album");
                    printf("Do you wish to add another song to this album [y/n]: ");
                    char ch ;
                    scanf("%c",&ch);
                    if(ch == 'n') break ;
                    else if(ch != 'n' && ch != 'y'){
                        printf("Invalid choice\n");
                    }
                }
                break;}
            case 6:
                {printf("Enter new playlist name: ");
                scanf("%s" , name);
                name[strcspn(name, "\n")] = 0;
                Playlist *newpl = create_playlist(name);
                if (newpl == NULL) {
                    printf("Failed to create playlist\n");
                    break;
                }
                add_playlist(&playlists, newpl);
                logger("Create_playlist");
                while(1){
                    printf("Do you wish to add song to this Playlist [y/n]: ");
                    char ch;
                    scanf(" %c", &ch);
                    getchar();
                    if(ch == 'y'){
                        int ssid;
                        print_all_songs(lib->songs);
                        printf("Enter song id to add: ");
                        scanf("%d", &ssid);
                        getchar();
                        Song *song = search_song_in_lib(lib, ssid);
                        if (song == NULL) {
                            printf("Song does not exist\n");
                            break;
                        }
                        add_song_to_playlist(newpl, song);
                        logger("Add_song_to_playlist");
                        printf("SONG ADDED SUCCESFULLy\n");
                    }else if(ch == 'n') break;
                    else{
                        printf("Looks like you don't want to enter a song\n");
                        break ;
                    }
                }
                break;}
            case 7:
                {if (playlists == NULL) {
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
                    logger("add_song_to_playlist");
                    printf("SONG ADDED SUCCESFULLY\n");
                    printf("Do you wish to add another song to this playlist [y/n]: ");
                    char ch ;
                    scanf("%c",&ch);
                    getchar();
                    if(ch == 'n') break ;
                    else if(ch != 'y' && ch != 'n'){
                        printf("seems like you dont want to add a new song\n");
                        break ;
                    }
                }
                break;}
            case 8:
                {if (playlists == NULL) {
                    printf("No playlists available\n");
                    break;
                }
                temp = playlists ;
                while (temp != NULL) {
                    print_playlist(temp);
                    temp = temp->next;
                }

                printf("Enter playlist name to add album to: ");
                scanf("%s",name);
                getchar();
                name[strcspn(name, "\n")] = 0;
                Playlist *pl = find_playlist_by_name(playlists, name);
                if (pl == NULL){
                    printf("No playlist exists for this name\n");
                    break ;
                }
                int aid;
                list_all_albums(lib);
                printf("Enter album id to add: ");
                scanf("%d", &aid);
                getchar();
                Album *alb = find_album_by_id(lib, aid);
                if (alb == NULL) {
                    printf("Album not found\n");
                    break;
                }
                add_album_to_playlist(pl, alb);
                logger("Add_album_to_playlist");
                break;}
            case 9:
                {int aid, sid;
                list_all_albums(lib);
                printf("Enter album id to delete song from: ");
                scanf("%d", &aid);
                getchar();
                Album *alb = find_album_by_id(lib, aid);
                if (alb == NULL) {
                    printf("Album not found.\n");
                    break;
                }
                print_all_songs(lib->songs);
                printf("Enter song id to delete: ");
                scanf("%d", &sid);
                getchar();
                Song *song = find_song_by_id(alb->songlist, sid);
                if (song == NULL ) {
                    printf("Song not found in album\n");
                    break;
                }
                remove_song_from_album(&alb, song);
                logger("delete_song_from_album");
                break;}
            case 10:
                if (playlists == NULL) {
                    printf("No playlists available.\n");
                    break;
                }
                temp = playlists ;
                while (temp != NULL) {
                    print_playlist(temp);
                    temp = temp->next;
                }
                printf("Enter playlist name to play: ");
                scanf("%s",name);
                getchar();
                name[strcspn(name, "\n")] = 0;
                Playlist *pl = find_playlist_by_name(playlists, name);
                if (pl == NULL) {
                    printf("No song exist for this name\n");
                    break;
                }
                while(1){
                    int c ;
                    play_current_song(pl);
                    printf("\n1. Play next\n2. Play previous\n3. Play current song again\n4. Exit playlist\nEnter choice: ");
                    scanf("%d",&c);
                    getchar();
                    if(c == 4) break ;
                    switch(c){
                        case 1:
                            play_next_song(pl);
                            break ;
                        case 2:
                            play_prev_song(pl);
                            break ;
                        case 3:
                            continue;
                    }
                }
                logger("play_song_through_playlist");
                break ;
            case 11:
                temp = playlists ;
                while (temp != NULL) {
                    print_playlist(temp);
                    temp = temp->next;
                }
                printf("Enter the name of Playlist needed to be removed: ");
                scanf("%s",name);
                getchar();
                remove_playlist_by_name(&playlists , name);
                break ;
            case 12:
                save_library(lib, SONGS_FILE, ALBUMS_FILE);
                save_playlist(PLAYLISTS_FILE, playlists);
                free_library(&lib);
                free(temp);

                Playlist* prev = playlists ;
                while(prev != NULL){
                    Playlist* next = prev->next ;
                    free_playlist(&prev);
                    prev = next ;
                }
                printf("----------- THANK YOU FOR USING MY APP -----------\n");
                logger("save_and_exit\n");
                return 0 ;
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;

}
