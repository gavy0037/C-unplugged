#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include "album.h"
#include "song.h"
#include "playlist.h"

#define SONGS_FILE "data/songs.txt"
#define ALBUMS_FILE "data/albums.txt"
#define PLAYLISTS_FILE "data/playlists.txt"
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

    int choice;
    char name[100];

    while (1) {
        printf("\n======================\n");
        printf("🎵  C-Unplugged CLI  🎵\n");
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
        getchar(); // remove newline

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

            case 6:
                printf("Enter new playlist name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                Playlist *newpl = create_playlist(name);
                add_playlist(&playlists, newpl);
                log_command("create playlist");
                break;

            case 9:
                // Example: interact with currently selected playlist
                // In future: let user choose which playlist to play
                if (playlists && playlists->curr)
                    play_next_song(playlists);
                else
                    printf("No playlist or songs to play.\n");
                log_command("play next");
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
            case 11:
                save_library(lib, SONGS_FILE, ALBUMS_FILE);
                save_playlist(PLAYLISTS_FILE, playlists);
                printf("✅ All data saved. Goodbye!\n");
                log_command("save and exit");
                return 0;

            default:
                printf("❌ Invalid choice.\n");
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