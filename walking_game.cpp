#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <thread>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdlib>

void setNonBlockingMode(bool enable) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);

    if (enable) {
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_cc[VMIN] = 1;
    } else {
        ttystate.c_lflag |= ICANON;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (enable) {
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    } else {
        fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    }
}

void print_desk(std::vector<std::vector<std::string>> map){
    std::string stuff(2*map[0].size()+1, '-');

    std::cout << stuff << std::endl;
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map[0].size(); j++){
            std::cout << "|" << map[i][j];
        }
        std::cout << "|" << std::endl << stuff << std::endl;
    }
}

void zombie_move(std::vector<std::vector<std::string>> map) {
    int zombie_coord_x = 9;
    int zombie_coord_y = 9;
    map[zombie_coord_x][zombie_coord_y] = "Z";

    while (true) {
        int vec = rand() % 4;

        if (vec == 0) {
            if (zombie_coord_x - 1 >= 0 && map[zombie_coord_x - 1][zombie_coord_y] != "|") {
                map[zombie_coord_x][zombie_coord_y] = " ";
                zombie_coord_x--;
                map[zombie_coord_x][zombie_coord_y] = "Z";
            } else {
                continue;
            }
        }
        else if (vec == 1) {
            // move zombie back
        }
        else if (vec == 2) {
            // move zombie right
        }
        else if (vec == 3) {
            // move zombie left
        }

        // zombie move 1 in 1 seconds
        system("clear");
        print_desk(map);
        usleep(10000000);
    }
}

int main() {
    int rows = 10;
    int columns = 10;

    std::vector<std::vector<std::string>> map(rows, std::vector<std::string>(columns, " "));
    
    int coords_of_player_x = 0; // spawn coords
    int coords_of_player_y = 0;

    map[coords_of_player_x][coords_of_player_y] = "&";
    map[coords_of_player_x+1][coords_of_player_y] = "|";

    print_desk(map);
    std::thread thread_obj(zombie_move, map);

    setNonBlockingMode(true);

    while (true) {
        char ch;
        int n = read(STDIN_FILENO, &ch, 1);
        if (n > 0) {
            if (ch == 'w') {
                // forward
                if (coords_of_player_x - 1 >= 0 && map[coords_of_player_x - 1][coords_of_player_y] != "|"){
                    map[coords_of_player_x][coords_of_player_y] = " ";
                    coords_of_player_x--;
                    map[coords_of_player_x][coords_of_player_y] = "&";
                }
            } else if (ch == 's') {
                // back
                if (coords_of_player_x + 1 <= map.size() - 1 && map[coords_of_player_x + 1][coords_of_player_y] != "|" ){
                    map[coords_of_player_x][coords_of_player_y] = " ";
                    coords_of_player_x++;
                    map[coords_of_player_x][coords_of_player_y] = "&";
                }
            } else if (ch == 'd') {
                // right
                if (coords_of_player_y + 1 <= map[0].size() - 1 && map[coords_of_player_x][coords_of_player_y + 1] != "|"){
                    map[coords_of_player_x][coords_of_player_y] = " ";
                    coords_of_player_y++;
                    map[coords_of_player_x][coords_of_player_y] = "&";
                }
            } else if (ch == 'a') {
                // left
                if (coords_of_player_y - 1 >= 0 && map[coords_of_player_x][coords_of_player_y - 1] != "|"){
                    map[coords_of_player_x][coords_of_player_y] = " ";
                    coords_of_player_y--;
                    map[coords_of_player_x][coords_of_player_y] = "&";
                }
            }
        }

        system("clear");
        print_desk(map);
        usleep(10);
    }

    setNonBlockingMode(false);

    return 0;
}
