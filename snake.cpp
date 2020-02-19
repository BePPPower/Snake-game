#include <iostream>
#include <ncurses.h>
#include <sys/time.h>
#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include <map>
using namespace std ;

int getRand(int a,int b){
	return (a + rand()%(b-a+1)) ;
}

WINDOW *creat_window(int height,int weight,int starty,int startx) ;

void destroy_win(WINDOW *local_win) ;

int game_win_height ;
int game_win_width ;
int game_win_starty ;
int game_win_startx ;

int score_win_heigh ;
int score_win_width ;
int score_win_starty ;
int score_win_startx ;
int score_writey ;
int score_writex ;

WINDOW *game_win,*score_win ;

int key ;

class Snake
{
private:
	int score ;
	int **box_map ;
	pair<int,int> head_locate ;
	pair<int,int> apple_locate ;


	vector<pair<int,int> > snake ;

	bool game_over ;

public:
	void initial() ;
	void show_apple();
	pair<int,int> produce_locate() ;
	bool iscash() ;
	void head_move() ;
	void boby_move() ;
	void show_snake() ;
	void move() ;
	bool iseat() ;

};






int main(){
    initscr() ;
    cbreak() ;
    noecho() ;
    curs_set(0) ;
    keypad(stdscr,TRUE) ;

    Snake *p = new Snake;
    p->initial() ;

    while(1){
 
    	p->move() ;
    	if(p->iscash()){
    		break ;
    	}
    }

    system("clear") ;
    mvprintw(LINES/3,COLS/3,"Game Over !") ;
    mvprintw(LINES/3+1,COLS/3,"Press any key to exit .") ;
    refresh() ;

    char c ;
    scanf("%c",&c) ;
    destroy_win(game_win) ;
    endwin() ;
    return 0 ;
}

WINDOW *creat_window(int height,int weight,int starty,int startx){
	WINDOW *local_win ;
	local_win = newwin(height,weight,starty,startx) ;
	box(local_win,'#','#') ;
	wrefresh(local_win) ;
	return local_win ;
}

void destroy_win(WINDOW *local_win){
	box(local_win,' ',' ') ;
	wrefresh(local_win) ;
	delwin(local_win) ;
}

void Snake::initial(){
	srand((unsigned)time(0));
	key = KEY_RIGHT ;

	game_win_height = LINES ;
    game_win_width = COLS/5*4 ;
    game_win_startx = game_win_starty = 0 ;

    score_win_heigh = LINES ;
    score_win_width = COLS/5 ;
    score_win_startx = COLS/5*4 ;
    score_win_starty = 0 ;
    score_writey = score_win_heigh/3 ;
    score_writex = score_win_width/3 ;

    score = 0 ;
    game_over = false;

    box_map = new int*[game_win_height] ;
    for(int i = 0;i<game_win_height;i++)
    	box_map[i] = new int[game_win_width] ;

    for(int i=0;i<game_win_height;i++)
    	for(int j = 0;j<game_win_width;j++){
    		if(i == 0 || i==game_win_height-1 || j == 0 || j == game_win_width-1)
    			box_map[i][j] = 1 ;
    		else box_map[i][j] = 0 ;
    	}


    game_win =  creat_window(game_win_height,game_win_width,game_win_starty,game_win_startx) ;
 //   wborder(game_win,'*', '*', '*', '*', '*', '*', '*', '*') ;

    score_win = creat_window(score_win_heigh,score_win_width,score_win_starty,score_win_startx) ;
//   wborder(score_win,'*', '*', '*', '*', '*', '*', '*', '*') ;
//    for(int i = 0;i<game_win_height;i++)
//    	for(int j = 0;j<game_win_width;j++)
//    		if(box_map[i][j] == 1 && (i == 0 || i == game_win_height-1))
//    			mvwaddch(game_win,i,j,'-') ;
//    		else if(box_map[i][j] == 1 && (j == 0 || j == game_win_width-1))
//    			mvwaddch(game_win,i,j,'|') ;
    
    wrefresh(game_win) ;

    mvwprintw(score_win,score_writey,score_writex,"Score:") ;
    mvwprintw(score_win,score_writey+1,score_writex+2,"%d",score) ;
    wrefresh(score_win) ;

    
    show_apple() ;

    head_locate = produce_locate() ;
    while(head_locate.first == apple_locate.first
    	  && head_locate.second == apple_locate.second)
    	head_locate = produce_locate() ;
    snake.push_back(head_locate) ;

    show_snake() ;

    
    
    wrefresh(game_win) ;
    		

}

pair<int,int> Snake::produce_locate(){
   	int x = getRand(1,game_win_width-2) ;
	int y = getRand(1,game_win_height-2) ;
	pair<int,int> apple = {y,x} ;
	return apple ;
}

bool Snake::iscash(){
	int y = snake[0].first ;
	int x = snake[0].second ;
	if(box_map[y][x] == 1)
		return TRUE ;
	return false ;
}

void Snake::boby_move(){

	for(int i = snake.size()-1;i>0;i--){
		if(i == snake.size()-1){
			int y = snake[i].first ;
			int x = snake[i].second ;
			box_map[y][x] = 0 ;
		}
		snake[i] = snake[i-1] ;
		if(i == 1){
			int y = snake[i].first ;
			int x = snake[i].second ;
			box_map[y][x] = 1 ;			
		}
	}
	snake[0] = head_locate ;

}

void Snake::head_move(){
	
	if(key == KEY_LEFT){
		head_locate.second-- ;
	}else if(key == KEY_RIGHT){
		head_locate.second++ ;
	}else if(key == KEY_UP){
		head_locate.first-- ;
	}else if(key == KEY_DOWN){
		head_locate.first++ ;
	}

}

void Snake::move(){
    wborder(game_win,'|', '|', '-', '-', '*', '*', '*', '*') ;
	wborder(score_win,'|', '|', '-', '-', '*', '*', '*', '*') ;
	wrefresh(game_win) ;
	wrefresh(score_win) ;

	 fd_set set;
	 FD_ZERO(&set);
	 FD_SET(0, &set);

	 struct timeval timeout;
	 timeout.tv_sec = 0;
	 timeout.tv_usec= 200000;

	 for(int i = 0;i<snake.size();i++)
	 	mvwaddch(game_win,snake[i].first,snake[i].second,' ') ;

	 if (select(1, &set, NULL, NULL, &timeout) == 0){
	 	head_move() ;
	 	if(iseat()){
	 		score += 10 ;
	 		snake.insert(snake.begin(),head_locate) ;
	 		box_map[snake[1].first][snake[1].second] =1 ;
	 		show_apple() ;
	 		mvwprintw(score_win,score_writey+1,score_writex+2,"%d",score) ;
	 	}
	 	else
	 		boby_move() ;
	 	
	 }

	 if (FD_ISSET(0, &set)){
	 	
	 	int tmp_key = getch() ;


	 	if(tmp_key == KEY_DOWN || tmp_key == KEY_UP ||
	 		tmp_key == KEY_RIGHT || tmp_key == KEY_LEFT)
	 		{
	 			if(snake.size() == 1)
	 				key = tmp_key ;
	 			else if(key == KEY_RIGHT && tmp_key==KEY_LEFT);
	 			else if(key == KEY_LEFT && tmp_key==KEY_RIGHT);
	 			else if(key == KEY_UP && tmp_key == KEY_DOWN);
	 			else if(key == KEY_DOWN && tmp_key == KEY_UP) ;
	 			else
	 				key = tmp_key ;

	 		}
	 	head_move() ;	
	 	if(iseat()){
	 		score += 10 ;
	 		snake.insert(snake.begin(),head_locate) ;
	 		box_map[snake[1].first][snake[1].second] =1 ;
	 		show_apple() ;
	 		mvwprintw(score_win,score_writey+1,score_writex+2,"%d",score) ;
	 	}else
	 		boby_move() ;
	 	
	 }
	 show_snake() ;

}

void Snake::show_snake(){
	 for(int i = 0;i<snake.size();i++)
	 	mvwprintw(game_win,snake[i].first,snake[i].second,"*") ;
	 wrefresh(game_win) ;
}

bool Snake::iseat(){
	if(head_locate.first == apple_locate.first && head_locate.second == apple_locate.second){
		return true ;
	}else
		return false ;
}

void Snake::show_apple(){
	apple_locate = produce_locate() ;
	while(box_map[apple_locate.first][apple_locate.second] == 1)
		apple_locate = produce_locate() ;
	mvwprintw(game_win,apple_locate.first,apple_locate.second,"*") ;
	wrefresh(game_win) ;
}