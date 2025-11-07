#include<stdio.h>
#include"include/song.h"

int main() {
    Song *library = get_songs_from_file("data/songs.txt");

    printf("🎵 Songs Loaded Initially:\n");
    print_all_songs(library);

    // Add a new song manually
    Song *newSong = create_song(4, "Perfect", "Ed Sheeran", "4:23");
    add_song(&library, newSong);

    printf("\n🎵 After Adding One Song:\n");
    print_all_songs(library);

    // Save updated list back to file
    add_all_songs("data/songs.txt", library);

    free_song_list(&library);

    if(library == NULL){
        printf("Memory cleared\n");
    }
    return 0;
}
