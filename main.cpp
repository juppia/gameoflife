#include <ncurses.h>
#include <vector>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int max_x, max_y;
int curs_x = 0;
int curs_y = 0;

void render(std::vector<std::vector<char>>& map, bool isGame){
	erase();
	auto map_next = map;
	if(isGame){
		for(int y = 0; y < max_y; y++){
			for(int x = 0; x < max_x; x++){
				if(map[y][x] == '#' || map[y][x] == 'd'){
					int cells_around = 0;
					if(y-1 >= 0){
						if(map[y-1][x] == '#'){
							cells_around++;
						}
						if(x-1 >= 0){
							if(map[y-1][x-1] == '#'){
								cells_around++;
							}
						}
						if(x+1 < max_x){
							if(map[y-1][x+1] == '#'){
								cells_around++;
							}
						}
					}
					if(y+1 < max_y){
						if(map[y+1][x] == '#'){
							cells_around++;
						}
						if(x-1 >= 0){
							if(map[y+1][x-1] == '#'){
								cells_around++;
							}
						}
						if(x+1 < max_x){
							if(map[y+1][x+1] == '#'){
								cells_around++;
							}
						}
					}
					if(x-1 >= 0){
						if(map[y][x-1] == '#'){
							cells_around++;
						}
					}
					if(x+1 < max_x){
						if(map[y][x+1] == '#'){
							cells_around++;
						}
					}
					
					if(cells_around <= 1 || cells_around >= 4){
						map_next[y][x] = 'd';
					}
					else if(cells_around == 3 && map[y][x] == 'd'){
						map_next[y][x] = '#';
					}
				}
			}
		}
	}
	
	
	map = map_next;
	for(int y = 0; y < max_y; y++){
		for(int x = 0; x < max_x; x++){
			if(!isGame){
				if(y == curs_y && x == curs_x){
					mvaddch(y, x, 'C');
				}
			}
			if(map_next[y][x] == '#')
				mvaddch(y, x, map_next[y][x]);
		}
	}
	refresh();
}

int main(){
	initscr();
	noecho();
	curs_set(0);
	
	getmaxyx(stdscr, max_y, max_x);
	std::vector<std::vector<char>> map(max_y, std::vector<char>(max_x, 'd'));
    bool config = true;
    
	while(config){
		int ch = getch();
		switch(ch){
			case ' ':
				config = false;
				break;
			case 'w':
				curs_y--;
				break;
			case 's':
				curs_y++;
				break;
			case 'a':
				curs_x--;
				break;
			case 'd':
				curs_x++;
				break;
			case 'e':
				map[curs_y][curs_x] = '#';
				curs_x++;
				break;
			case 'q':
				endwin();
				return 1;
				break;
		}
		if(curs_y < 0) curs_y = 0;
		else if(curs_y >= max_y) curs_y = max_y - 1;
		
		if(curs_x < 0) curs_x = 0;
		else if(curs_x >= max_x) curs_x = max_x - 1;
		render(map, false);
	}
	bool isGame = true;
	while(isGame){
		render(map, true); 
		std::this_thread::sleep_for(200ms);
	}
	
	getch();
	endwin();
	return 0;
}
