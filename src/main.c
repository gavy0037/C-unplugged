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

void log_command(const char *command);

int main() {
    Library *lib = load_library(SONGS_FILE, ALBUMS_FILE);
    Playlist *playlists = NULL;
    int songid = 0, albumid = 0;
    // Load data from previous session
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
    if(playlists == NULL){
        printf("WARNING : PLAYLIST's LOADING FAILED\n");
    }
    print_playlist(playlists);
    int choice;
    char name[100];

    while (1) {
        printf("\n======================\n");
        printf(" C-Unplugged CLI \n");
        printf("======================\n");
        printf("1. List all songs\n");
        printf("2. List all albums\n");
        printf("3. Create new album\n");
        printf("4. Add song to album\n");
        printf("5. Delete song from album\n");
        printf("6. Create new playlist\n");
        printf("7. Add album to playlist\n");
        printf("8. Add single song to playlist\n");
        printf("9. Play next / previous song\n");
        printf("10. View all playlists\n");
        printf("11. Save and Exit\n");
        printf("======================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                print_all_songs(lib->songs);
                log_command("list songs");
                break;

            case 2:
                list_all_albums(lib);
                log_command("list albums");
                break;

            case 3:
                printf("Enter new album name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                Album* album = create_album( ++albumid , name);
                add_album_to_library(lib, album);
                log_command("create album");
                break;

            case 4: {
                int aid, sid;
                printf("Enter album id to add song to: ");
                scanf("%d", &aid);
                getchar();
                Album *alb = find_album_by_id(lib, aid);
                if (!alb) {
                    printf("Album not found.\n");
                    break;
                }
                printf("Enter song id to add: ");
                scanf("%d", &sid);
                getchar();
                Song *ss = search_song_in_lib(lib, sid);
                if (!ss) {
                    printf("Song not found in library.\n");
                    break;
                }
                /* create a copy of the song node for album storage */
                Song *copy = create_song(ss->id, ss->name, ss->A_name, ss->duration);
                add_song_to_album(&alb, copy);
                log_command("add song to album");
                break;
            }

            case 5: {
                int aid, sid;
                printf("Enter album id to delete song from: ");
                scanf("%d", &aid);
                getchar();
                Album *alb = find_album_by_id(lib, aid);
                if (!alb) {
                    printf("Album not found.\n");
                    break;
                }
                printf("Enter song id to delete: ");
                scanf("%d", &sid);
                getchar();
                Song *toremove = find_song_by_id(alb->songlist, sid);
                if (!toremove) {
                    printf("Song not found in album.\n");
                    break;
                }
                remove_song_from_album(&alb, toremove);
                log_command("delete song from album");
                break;
            }

            case 6:
                printf("Enter new playlist name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                Playlist *newpl = create_playlist(name);
                add_playlist(&playlists, newpl);
                log_command("create playlist");
                break;

            case 9:
                {
                    if (playlists == NULL) {
                        printf("No playlists available.\n");
                        break;
                    }
                    printf("Enter playlist name to control: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = 0;
                    Playlist *pl = find_playlist_by_name(playlists, name);
                    if (!pl) {
                        break;
                    }
                    int dir;
                    printf("1. Play next\n2. Play previous\n3. Play current\nEnter choice: ");
                    scanf("%d", &dir);
                    getchar();
                    if (dir == 1) play_next_song(pl);
                    else if (dir == 2) play_prev_song(pl);
                    else if (dir == 3) play_current_song(pl);
                    else printf("Invalid option.\n");
                    log_command("play control");
                }
                break;

            case 10:
            {
                Playlist *temp = playlists;
                while (temp) {
                    print_playlist(temp);
                    temp = temp->next;
                }
                log_command("view playlists");
                break;

            }
            case 7: {
                if (playlists == NULL) {
                    printf("No playlists available.\n");
                    break;
                }
                printf("Enter playlist name to add album to: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                Playlist *pl = find_playlist_by_name(playlists, name);
                if (!pl) break;
                int aid;
                printf("Enter album id to add: ");
                scanf("%d", &aid);
                getchar();
                Album *alb = find_album_by_id(lib, aid);
                if (!alb) {
                    printf("Album not found.\n");
                    break;
                }
                add_album_to_playlist(pl, alb);
                log_command("add album to playlist");
                break;
            }

            case 8: {
                if (playlists == NULL) {
                    printf("No playlists available.\n");
                    break;
                }
                printf("Enter playlist name to add song to: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                Playlist *pl = find_playlist_by_name(playlists, name);
                if (!pl) break;
                int sid;
                printf("Enter song id to add: ");
                scanf("%d", &sid);
                getchar();
                Song *sng = search_song_in_lib(lib, sid);
                if (!sng) {
                    printf("Song not found in library.\n");
                    break;
                }
                add_song_to_playlist(pl, sng);
                log_command("add song to playlist");
                break;
            }
            case 11:
                save_library(lib, SONGS_FILE, ALBUMS_FILE);
                save_playlist(PLAYLISTS_FILE, playlists);
                printf("All data saved. Goodbye!\n");
                log_command("save and exit");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

// Append every user command to a log file
void log_command(const char *command) {
    FILE *log = fopen(LOG_FILE, "a");
    if (!log) return;
    fprintf(log, "%s\n", command);
    fclose(log);
}