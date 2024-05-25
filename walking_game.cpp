#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <thread>
#include <vector>

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

int main() {
    int rows = 10;
    int columns = 10;
    std::vector<std::vector<std::string>> map(rows, std::vector<std::string>(columns, " "));
    
    int coords_of_player_x = 0; // spawn coords
    int coords_of_player_y = 0;
    map[coords_of_player_x][coords_of_player_y] = "&";

    print_desk(map);

    setNonBlockingMode(true);

    while (true) {
        char ch;
        int n = read(STDIN_FILENO, &ch, 1);
        if (n > 0) {
            if (ch == 'w'){
                if (coords_of_player_x - 1 >= 0){
                    map[coords_of_player_x][coords_of_player_y] = " ";
                    coords_of_player_x--;
                    map[coords_of_player_x][coords_of_player_y] = "&";
                }
            } else if (ch == 's') {
                if (coords_of_player_x + 1 <= map.size() - 1){
                    map[coords_of_player_x][coords_of_player_y] = " ";
                    coords_of_player_x++;
                    map[coords_of_player_x][coords_of_player_y] = "&";
                }
            } else if (ch == 'd') {
                if (coords_of_player_y + 1 <= map[0].size() - 1){
                    map[coords_of_player_x][coords_of_player_y] = " ";
                    coords_of_player_y++;
                    map[coords_of_player_x][coords_of_player_y] = "&";
                }
            } else if (ch == 'a'){
                if (coords_of_player_y - 1 >= 0){
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
