# c++利用ncurses库实现贪吃蛇

**介绍**：这是一个基于ncurses库的简单贪吃蛇（Snake）游戏。代码很简短，一共三百行，很适合作为C/C++语言入门的小程序设计。


## 开发环境：
+ ncurses库
+ Linux（Ubuntu）
+ sublime文本编辑器

Note：Linux也可以换成windows，sublime文本编辑器可以换成DEVc++之类的IDE 。

## 运行效果

## 运行步骤

**前提**：要保证你的主机（无论是Windows还是Linux）已经安装好了g++编译器和ncurse库。

**具体步骤**：
1. 复制snake.cpp到你的本地目录下。 
2. 用g++编译：（在Linux下）命令为 g++ -o snake snake.cpp -l ncurse
3. 运行程序：（Linux下）命令为 ./snake

## 项目介绍
**关键函数**
1. create_window()函数
2. initial()函数
3. iseat()函数
4. head_move()函数
5. body_move()函数
6. move()函数

**具体介绍**
- create_window()函数：
把ncurses库中的有关创建窗口的函数，封装成一个create_window()函数，返回类型为 WINDOW* 。四个函数参数分别为：height--窗口高度、weight--窗口宽度、starty--窗口起始位置（列坐标）、startx--窗口起始位置（行坐标）。
```c
WINDOW *creat_window(int height,int weight,int starty,int startx){
	WINDOW *local_win ;
	local_win = newwin(height,weight,starty,startx) ;
	box(local_win,'#','#') ;
	wrefresh(local_win) ;
	return local_win ;
}
```
- initial()函数：
initial()函数主要完成游戏窗口及计分窗口的绘制、游戏开始随机产生苹果位置，游戏开始随机产生snake位置。
box_map是一个二维数组，box_map[i][j]表示坐标(i,j)这个位置不能去，也就是说如果蛇头碰到了一个(i,j)位置那就游戏结束了。
```c
void Snake::initial(){
	srand((unsigned)time(0));
	key = KEY_RIGHT ;
	//设计游戏窗口的尺寸。
	game_win_height = LINES ;
    game_win_width = COLS/5*4 ;
    game_win_startx = game_win_starty = 0 ;
	//设计计分窗口的尺寸。
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
//初始化box_map二维数组，限定游戏界面的边界box_map[i][j]=1，意思是游戏边界不能去。
    for(int i=0;i<game_win_height;i++)
    	for(int j = 0;j<game_win_width;j++){
    		if(i == 0 || i==game_win_height-1 || j == 0 || j == game_win_width-1)
    			box_map[i][j] = 1 ;
    		else box_map[i][j] = 0 ;
    	}


    game_win =  creat_window(game_win_height,game_win_width,game_win_starty,game_win_startx) ;

    score_win = creat_window(score_win_heigh,score_win_width,score_win_starty,score_win_startx) ;
    
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
```
- iseat()函数
这个函数其实就比较简单，如果snake头部坐标和苹果的坐标相等，说明蛇迟到了苹果，那么返回TURE,否则返回FALSE。
```c
bool Snake::iseat(){
	if(head_locate.first == apple_locate.first && head_locate.second == apple_locate.second){
		return true ;
	}else
		return false ;
}
```
- **head_move()、body_move()、move()函数**
这三个函数一起介绍，为什么将蛇的移动分成了三个函数来写呢？我是这样构思的：整条蛇采用vector<pair<int,int> >snake来存储，蛇头为snake[0]或者说head_locate,蛇身为snake[1...n],蛇身的移动body_move和蛇头的head_move是不一样的，蛇头的坐标可能向上下左右各个位置都移动，如：snake[0].first++代表蛇头向下移动一个位置，而蛇身的移动是后一个坐标赋值为前一个坐标，即：snake[n] = snake[n-1]。所以我将蛇身和蛇头的移动分开来了。然后move函数中调用head_move和body_move函数，实现整条蛇的移动。
 **head_move():**
 key用于接收功能键↑↓←→，根据key的值对蛇头的坐标做相应的改变。
```c
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
```
**body_move():**
从后向前遍历snake，将snake[n] 赋值为 snake[n-1]，同时不要忘记修稿box_map对应的值，因为蛇头碰到蛇身是算游戏结束的，所以一定要将相应蛇身的坐标box_map[i][j]赋值为1。
```c
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
```
**move()函数：**
首先刷新一遍窗口边界，然后利用timeout设计定时，如果在时间到来时还没有按键发生，就执行默认动作。利用getch接收按键，并根据按键来做相应动作。Note：如果蛇目前是向左移动，想让他改为向右移动这样是做不到的，因为这样蛇头和蛇身就重叠了。
另外，在move函数中还要判断这次移动是否吃到了苹果，即调用iseat()函数，若吃到了，则要重新生成一个苹果，且分数+10。
```c
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
```
- 其他
其他的一些函数，包括iscash()函数、show_apple()函数等，相信你能够通过阅读源码自己理解！！
