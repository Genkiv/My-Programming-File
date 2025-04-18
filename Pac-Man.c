typedef volatile unsigned short hword;

#define VRAM 0x06000000
/*色の情報*/
#define white 0x7FFF
#define lightblue 0x7FE0
#define red 0x001F
#define yellow 0x02FF
#define blue 0x7C00
#define black 0x0000
#define orange 0x001FF
#define pink 0x5C1F
/*キー入力の判定に使用する*/
#define akey 0x0001
#define bkey 0x0002
#define upkey 0x0040
#define downkey 0x0080
#define leftkey 0x0020
#define rightkey 0x0010
#define startkey 0x0008
//マップのスタート位置の座標
#define xstart_map 2
#define ystart_map 4
/*配列を引数とするための配列の長さを指定*/
#define map_len 192
#define map_line 144
#define PacMan_len 14
#define word_len 10
/*ドットの最大の個数*/
#define max_dot_num 157

void draw_point (hword, hword, hword);

/*マップ関連*/
/*マップを描画するメソッド*/
void draw_map (hword, hword, hword);
/*マップのドットごとの色の情報を格納するメソッド。衝突判定でそのドットの色を見て判定する。*/
void make_pixel_map(hword (*)[192], hword);
/*パックマンが動くことができる動線を設定する*/
void make_flow_line(hword (*)[192], hword, hword (*)[192], hword);
/*右向きに動線を描くためのメソッド*/
hword right_make_line(hword, hword, hword,hword (*)[192], hword, hword (*)[192], hword);
/*下向きに動線を描くためのメソッド*/
hword down_make_line(hword, hword, hword,hword (*)[192], hword, hword (*)[192], hword);
/*動線を描くためのメソッド*/
void draw_flow_line(hword (*)[192], hword);

/*パックマン関連*/
/*上向きパックマンの描画*/
void up_draw_PacMan(hword, hword, hword, hword (*)[14], hword, hword (*)[14], hword, hword (*)[14], hword, hword (*)[14], hword);
/*下向きパックマンの描画*/
void down_draw_PacMan(hword, hword, hword, hword (*)[14], hword, hword (*)[14], hword, hword (*)[14], hword, hword (*)[14], hword);
/*左向きパックマンの描画*/
void left_draw_PacMan(hword, hword, hword, hword (*)[14], hword, hword (*)[14], hword, hword (*)[14], hword, hword (*)[14], hword);
/*右向きパックマンの描画*/
void right_draw_PacMan(hword, hword, hword, hword (*)[14], hword, hword (*)[14], hword, hword (*)[14], hword, hword (*)[14], hword);
/*パックマンを動かす*/
hword move_pacman(hword, hword *, hword, hword, hword (*)[192], hword, hword (*)[192], hword, hword (*)[192], hword, hword *, hword, hword (*)[14], hword , hword (*)[14], 
hword , hword (*)[14], hword , hword (*)[14], hword );
/*食べたドットを計算する*/
hword eat_dots(hword, hword, hword (*)[192], hword, hword *, hword);

/*ゴーストを描画する*/
void draw_ghost(hword, hword, hword, hword (*)[14], hword);

/*スコア関連*/
/*ゲーム中に右上にスコアを表示する*/
void display_score(hword, hword (*)[10], hword, hword (*)[10], hword, hword (*)[10], hword, hword (*)[10], hword, hword (*)[10], hword, hword (*)[10][10], hword);
/*最後にスコアを表示する*/
void display_last_score(hword, hword (*)[10], hword, hword (*)[10], hword, hword (*)[10], hword, hword (*)[10], hword, hword (*)[10], hword, hword (*)[10][10], hword);
/*割り算を行う*/
hword div(hword);


int main(void){
    hword *ptr;
    volatile hword *key;
    ptr = (hword*) 0x04000000;
    *ptr = 0x0F03;
    
    ptr = (hword*)VRAM;
    hword i;
    hword j;
    /*パックマンのx座標とy座標*/
    hword mx = 11;
    hword my = 11;
    /*パックマンが食べたドットの数*/
    hword dot_num;
    dot_num = 0;
    /*現在のスコア*/
    hword score;
    score=dot_num*100;
    /*パックマンが今いる座標*/
    static hword exists_pacman_map[144][192] ;
    exists_pacman_map[my][mx]=1;
    //cieはcoordinate
    /*パックマンが今いる座標を保存*/
    static hword exists_pacman_cie[2];
    exists_pacman_cie[0] = mx;
    exists_pacman_cie[1] = my;

    /*パックマンの口の形を格納する配列*/
    static hword pacman_mouse[2];
    pacman_mouse[0] = 1;
    pacman_mouse[1] = 0;

    /*パックマンの向きを上下左右に反転させる配列*/
    static hword replace[14][14] ;
    /*口を閉じたパックマンを描画する情報を格納した配列*/
    static hword PacMan0[14][14] ;
    PacMan0[0][0] = 0; PacMan0[0][1] = 0; PacMan0[0][2] = 0; PacMan0[0][3] = 0; PacMan0[0][4] = 0; PacMan0[0][5] = 1; PacMan0[0][6] = 1; 
    PacMan0[0][7] = 1; PacMan0[0][8] = 1; PacMan0[0][9] = 0; PacMan0[0][10] = 0; PacMan0[0][11] = 0; PacMan0[0][12] = 0; PacMan0[0][13] = 0;
    PacMan0[1][0] = 0; PacMan0[1][1] = 0; PacMan0[1][2] = 0; PacMan0[1][3] = 1; PacMan0[1][4] = 1; PacMan0[1][5] = 1; PacMan0[1][6] = 1; 
    PacMan0[1][7] = 1; PacMan0[1][8] = 1; PacMan0[1][9] = 1; PacMan0[1][10] = 1; PacMan0[1][11] = 0; PacMan0[1][12] = 0; PacMan0[1][13] = 0;
    PacMan0[2][0] = 0; PacMan0[2][1] = 0; PacMan0[2][2] = 1; PacMan0[2][3] = 1; PacMan0[2][4] = 1; PacMan0[2][5] = 1; PacMan0[2][6] = 1; 
    PacMan0[2][7] = 1; PacMan0[2][8] = 1; PacMan0[2][9] = 1; PacMan0[2][10] = 1; PacMan0[2][11] = 1; PacMan0[2][12] = 0; PacMan0[2][13] = 0;
    PacMan0[3][0] = 0; PacMan0[3][1] = 1; PacMan0[3][2] = 1; PacMan0[3][3] = 1; PacMan0[3][4] = 1; PacMan0[3][5] = 1; PacMan0[3][6] = 1; 
    PacMan0[3][7] = 1; PacMan0[3][8] = 1; PacMan0[3][9] = 1; PacMan0[3][10] = 1; PacMan0[3][11] = 1; PacMan0[3][12] = 1; PacMan0[3][13] = 0;
    PacMan0[4][0] = 0; PacMan0[4][1] = 1; PacMan0[4][2] = 1; PacMan0[4][3] = 1; PacMan0[4][4] = 1; PacMan0[4][5] = 1; PacMan0[4][6] = 1; 
    PacMan0[4][7] = 1; PacMan0[4][8] = 1; PacMan0[4][9] = 1; PacMan0[4][10] = 1; PacMan0[4][11] = 1; PacMan0[4][12] = 1; PacMan0[4][13] = 0;
    PacMan0[5][0] = 1; PacMan0[5][1] = 1; PacMan0[5][2] = 1; PacMan0[5][3] = 1; PacMan0[5][4] = 1; PacMan0[5][5] = 1; PacMan0[5][6] = 1; 
    PacMan0[5][7] = 1; PacMan0[5][8] = 1; PacMan0[5][9] = 1; PacMan0[5][10] = 1; PacMan0[5][11] = 1; PacMan0[5][12] = 1; PacMan0[5][13] = 1;
    PacMan0[6][0] = 1; PacMan0[6][1] = 1; PacMan0[6][2] = 1; PacMan0[6][3] = 1; PacMan0[6][4] = 1; PacMan0[6][5] = 1; PacMan0[6][6] = 1; 
    PacMan0[6][7] = 1; PacMan0[6][8] = 1; PacMan0[6][9] = 1; PacMan0[6][10] = 1; PacMan0[6][11] = 1; PacMan0[6][12] = 1; PacMan0[6][13] = 1;
    PacMan0[7][0] = 1; PacMan0[7][1] = 1; PacMan0[7][2] = 1; PacMan0[7][3] = 1; PacMan0[7][4] = 1; PacMan0[7][5] = 1; PacMan0[7][6] = 1; 
    PacMan0[7][7] = 1; PacMan0[7][8] = 1; PacMan0[7][9] = 1; PacMan0[7][10] = 1; PacMan0[7][11] = 1; PacMan0[7][12] = 1; PacMan0[7][13] = 1;
    PacMan0[8][0] = 1; PacMan0[8][1] = 1; PacMan0[8][2] = 1; PacMan0[8][3] = 1; PacMan0[8][4] = 1; PacMan0[8][5] = 1; PacMan0[8][6] = 1; 
    PacMan0[8][7] = 1; PacMan0[8][8] = 1; PacMan0[8][9] = 1; PacMan0[8][10] = 1; PacMan0[8][11] = 1; PacMan0[8][12] = 1; PacMan0[8][13] = 1;
    PacMan0[9][0] = 0; PacMan0[9][1] = 1; PacMan0[9][2] = 1; PacMan0[9][3] = 1; PacMan0[9][4] = 1; PacMan0[9][5] = 1; PacMan0[9][6] = 1; 
    PacMan0[9][7] = 1; PacMan0[9][8] = 1; PacMan0[9][9] = 1; PacMan0[9][10] = 1; PacMan0[9][11] = 1; PacMan0[9][12] = 1; PacMan0[9][13] = 0;
    PacMan0[10][0] = 0; PacMan0[10][1] = 1; PacMan0[10][2] = 1; PacMan0[10][3] = 1; PacMan0[10][4] = 1; PacMan0[10][5] = 1; PacMan0[10][6] = 1; 
    PacMan0[10][7] = 1; PacMan0[10][8] = 1; PacMan0[10][9] = 1; PacMan0[10][10] = 1; PacMan0[10][11] = 1; PacMan0[10][12] = 1; PacMan0[10][13] = 0;
    PacMan0[11][0] = 0; PacMan0[11][1] = 0; PacMan0[11][2] = 1; PacMan0[11][3] = 1; PacMan0[11][4] = 1; PacMan0[11][5] = 1; PacMan0[11][6] = 1; 
    PacMan0[11][7] = 1; PacMan0[11][8] = 1; PacMan0[11][9] = 1; PacMan0[11][10] = 1; PacMan0[11][11] = 1; PacMan0[11][12] = 0; PacMan0[11][13] = 0;
    PacMan0[12][0] = 0; PacMan0[12][1] = 0; PacMan0[12][2] = 0; PacMan0[12][3] = 1; PacMan0[12][4] = 1; PacMan0[12][5] = 1; PacMan0[12][6] = 1; 
    PacMan0[12][7] = 1; PacMan0[12][8] = 1; PacMan0[12][9] = 1; PacMan0[12][10] = 1; PacMan0[12][11] = 0; PacMan0[12][12] = 0; PacMan0[12][13] = 0;
    PacMan0[13][0] = 0; PacMan0[13][1] = 0; PacMan0[13][2] = 0; PacMan0[13][3] = 0; PacMan0[13][4] = 0; PacMan0[13][5] = 1; PacMan0[13][6] = 1; 
    PacMan0[13][7] = 1; PacMan0[13][8] = 1; PacMan0[13][9] = 0; PacMan0[13][10] = 0; PacMan0[13][11] = 0; PacMan0[13][12] = 0; PacMan0[13][13] = 0;
    /*口を中くらいにあけたパックマンを描画する情報を格納した配列*/
    static hword PacMan1[14][14] ;
    PacMan1[0][0] = 0; PacMan1[0][1] = 0; PacMan1[0][2] = 0; PacMan1[0][3] = 0; PacMan1[0][4] = 0; PacMan1[0][5] = 1; PacMan1[0][6] = 1; 
    PacMan1[0][7] = 1; PacMan1[0][8] = 1; PacMan1[0][9] = 0; PacMan1[0][10] = 0; PacMan1[0][11] = 0; PacMan1[0][12] = 0; PacMan1[0][13] = 0;
    PacMan1[1][0] = 0; PacMan1[1][1] = 0; PacMan1[1][2] = 0; PacMan1[1][3] = 1; PacMan1[1][4] = 1; PacMan1[1][5] = 1; PacMan1[1][6] = 1; 
    PacMan1[1][7] = 1; PacMan1[1][8] = 1; PacMan1[1][9] = 1; PacMan1[1][10] = 1; PacMan1[1][11] = 0; PacMan1[1][12] = 0; PacMan1[1][13] = 0;
    PacMan1[2][0] = 0; PacMan1[2][1] = 0; PacMan1[2][2] = 1; PacMan1[2][3] = 1; PacMan1[2][4] = 1; PacMan1[2][5] = 1; PacMan1[2][6] = 1; 
    PacMan1[2][7] = 1; PacMan1[2][8] = 1; PacMan1[2][9] = 1; PacMan1[2][10] = 1; PacMan1[2][11] = 1; PacMan1[2][12] = 0; PacMan1[2][13] = 0;
    PacMan1[3][0] = 0; PacMan1[3][1] = 1; PacMan1[3][2] = 1; PacMan1[3][3] = 1; PacMan1[3][4] = 1; PacMan1[3][5] = 1; PacMan1[3][6] = 1; 
    PacMan1[3][7] = 1; PacMan1[3][8] = 1; PacMan1[3][9] = 1; PacMan1[3][10] = 1; PacMan1[3][11] = 1; PacMan1[3][12] = 1; PacMan1[3][13] = 0;
    PacMan1[4][0] = 0; PacMan1[4][1] = 0; PacMan1[4][2] = 0; PacMan1[4][3] = 1; PacMan1[4][4] = 1; PacMan1[4][5] = 1; PacMan1[4][6] = 1; 
    PacMan1[4][7] = 1; PacMan1[4][8] = 1; PacMan1[4][9] = 1; PacMan1[4][10] = 1; PacMan1[4][11] = 1; PacMan1[4][12] = 1; PacMan1[4][13] = 0;
    PacMan1[5][0] = 0; PacMan1[5][1] = 0; PacMan1[5][2] = 0; PacMan1[5][3] = 0; PacMan1[5][4] = 0; PacMan1[5][5] = 0; PacMan1[5][6] = 0; 
    PacMan1[5][7] = 1; PacMan1[5][8] = 1; PacMan1[5][9] = 1; PacMan1[5][10] = 1; PacMan1[5][11] = 1; PacMan1[5][12] = 1; PacMan1[5][13] = 1;
    PacMan1[6][0] = 0; PacMan1[6][1] = 0; PacMan1[6][2] = 0; PacMan1[6][3] = 0; PacMan1[6][4] = 0; PacMan1[6][5] = 0; PacMan1[6][6] = 0; 
    PacMan1[6][7] = 0; PacMan1[6][8] = 0; PacMan1[6][9] = 1; PacMan1[6][10] = 1; PacMan1[6][11] = 1; PacMan1[6][12] = 1; PacMan1[6][13] = 1;
    PacMan1[7][0] = 0; PacMan1[7][1] = 0; PacMan1[7][2] = 0; PacMan1[7][3] = 0; PacMan1[7][4] = 0; PacMan1[7][5] = 0; PacMan1[7][6] = 0; 
    PacMan1[7][7] = 0; PacMan1[7][8] = 0; PacMan1[7][9] = 1; PacMan1[7][10] = 1; PacMan1[7][11] = 1; PacMan1[7][12] = 1; PacMan1[7][13] = 1;
    PacMan1[8][0] = 0; PacMan1[8][1] = 0; PacMan1[8][2] = 0; PacMan1[8][3] = 0; PacMan1[8][4] = 0; PacMan1[8][5] = 0; PacMan1[8][6] = 0; 
    PacMan1[8][7] = 1; PacMan1[8][8] = 1; PacMan1[8][9] = 1; PacMan1[8][10] = 1; PacMan1[8][11] = 1; PacMan1[8][12] = 1; PacMan1[8][13] = 1;
    PacMan1[9][0] = 0; PacMan1[9][1] = 0; PacMan1[9][2] = 0; PacMan1[9][3] = 1; PacMan1[9][4] = 1; PacMan1[9][5] = 1; PacMan1[9][6] = 1; 
    PacMan1[9][7] = 1; PacMan1[9][8] = 1; PacMan1[9][9] = 1; PacMan1[9][10] = 1; PacMan1[9][11] = 1; PacMan1[9][12] = 1; PacMan1[9][13] = 0;
    PacMan1[10][0] = 0; PacMan1[10][1] = 1; PacMan1[10][2] = 1; PacMan1[10][3] = 1; PacMan1[10][4] = 1; PacMan1[10][5] = 1; PacMan1[10][6] = 1; 
    PacMan1[10][7] = 1; PacMan1[10][8] = 1; PacMan1[10][9] = 1; PacMan1[10][10] = 1; PacMan1[10][11] = 1; PacMan1[10][12] = 1; PacMan1[10][13] = 0;
    PacMan1[11][0] = 0; PacMan1[11][1] = 0; PacMan1[11][2] = 1; PacMan1[11][3] = 1; PacMan1[11][4] = 1; PacMan1[11][5] = 1; PacMan1[11][6] = 1; 
    PacMan1[11][7] = 1; PacMan1[11][8] = 1; PacMan1[11][9] = 1; PacMan1[11][10] = 1; PacMan1[11][11] = 1; PacMan1[11][12] = 0; PacMan1[11][13] = 0;
    PacMan1[12][0] = 0; PacMan1[12][1] = 0; PacMan1[12][2] = 0; PacMan1[12][3] = 1; PacMan1[12][4] = 1; PacMan1[12][5] = 1; PacMan1[12][6] = 1; 
    PacMan1[12][7] = 1; PacMan1[12][8] = 1; PacMan1[12][9] = 1; PacMan1[12][10] = 1; PacMan1[12][11] = 0; PacMan1[12][12] = 0; PacMan1[12][13] = 0;
    PacMan1[13][0] = 0; PacMan1[13][1] = 0; PacMan1[13][2] = 0; PacMan1[13][3] = 0; PacMan1[13][4] = 0; PacMan1[13][5] = 1; PacMan1[13][6] = 1; 
    PacMan1[13][7] = 1; PacMan1[13][8] = 1; PacMan1[13][9] = 0; PacMan1[13][10] = 0; PacMan1[13][11] = 0; PacMan1[13][12] = 0; PacMan1[13][13] = 0;
    /*口を全開にしたパックマンを描画する情報を格納した配列*/
    static hword PacMan2[14][14];
    PacMan2[0][0] = 0; PacMan2[0][1] = 0; PacMan2[0][2] = 0; PacMan2[0][3] = 0; PacMan2[0][4] = 1; PacMan2[0][5] = 1; PacMan2[0][6] = 1; 
    PacMan2[0][7] = 1; PacMan2[0][8] = 1; PacMan2[0][9] = 0; PacMan2[0][10] = 0; PacMan2[0][11] = 0; PacMan2[0][12] = 0; PacMan2[0][13] = 0;
    PacMan2[1][0] = 0; PacMan2[1][1] = 0; PacMan2[1][2] = 0; PacMan2[1][3] = 0; PacMan2[1][4] = 1; PacMan2[1][5] = 1; PacMan2[1][6] = 1; 
    PacMan2[1][7] = 1; PacMan2[1][8] = 1; PacMan2[1][9] = 1; PacMan2[1][10] = 1; PacMan2[1][11] = 0; PacMan2[1][12] = 0; PacMan2[1][13] = 0;
    PacMan2[2][0] = 0; PacMan2[2][1] = 0; PacMan2[2][2] = 0; PacMan2[2][3] = 0; PacMan2[2][4] = 0; PacMan2[2][5] = 1; PacMan2[2][6] = 1; 
    PacMan2[2][7] = 1; PacMan2[2][8] = 1; PacMan2[2][9] = 1; PacMan2[2][10] = 1; PacMan2[2][11] = 1; PacMan2[2][12] = 0; PacMan2[2][13] = 0;
    PacMan2[3][0] = 0; PacMan2[3][1] = 0; PacMan2[3][2] = 0; PacMan2[3][3] = 0; PacMan2[3][4] = 0; PacMan2[3][5] = 0; PacMan2[3][6] = 1; 
    PacMan2[3][7] = 1; PacMan2[3][8] = 1; PacMan2[3][9] = 1; PacMan2[3][10] = 1; PacMan2[3][11] = 1; PacMan2[3][12] = 1; PacMan2[3][13] = 0;
    PacMan2[4][0] = 0; PacMan2[4][1] = 0; PacMan2[4][2] = 0; PacMan2[4][3] = 0; PacMan2[4][4] = 0; PacMan2[4][5] = 0; PacMan2[4][6] = 0; 
    PacMan2[4][7] = 1; PacMan2[4][8] = 1; PacMan2[4][9] = 1; PacMan2[4][10] = 1; PacMan2[4][11] = 1; PacMan2[4][12] = 1; PacMan2[4][13] = 0;
    PacMan2[5][0] = 0; PacMan2[5][1] = 0; PacMan2[5][2] = 0; PacMan2[5][3] = 0; PacMan2[5][4] = 0; PacMan2[5][5] = 0; PacMan2[5][6] = 0; 
    PacMan2[5][7] = 0; PacMan2[5][8] = 1; PacMan2[5][9] = 1; PacMan2[5][10] = 1; PacMan2[5][11] = 1; PacMan2[5][12] = 1; PacMan2[5][13] = 1;
    PacMan2[6][0] = 0; PacMan2[6][1] = 0; PacMan2[6][2] = 0; PacMan2[6][3] = 0; PacMan2[6][4] = 0; PacMan2[6][5] = 0; PacMan2[6][6] = 0; 
    PacMan2[6][7] = 0; PacMan2[6][8] = 0; PacMan2[6][9] = 1; PacMan2[6][10] = 1; PacMan2[6][11] = 1; PacMan2[6][12] = 1; PacMan2[6][13] = 1;
    PacMan2[7][0] = 0; PacMan2[7][1] = 0; PacMan2[7][2] = 0; PacMan2[7][3] = 0; PacMan2[7][4] = 0; PacMan2[7][5] = 0; PacMan2[7][6] = 0; 
    PacMan2[7][7] = 0; PacMan2[7][8] = 0; PacMan2[7][9] = 1; PacMan2[7][10] = 1; PacMan2[7][11] = 1; PacMan2[7][12] = 1; PacMan2[7][13] = 1;
    PacMan2[8][0] = 0; PacMan2[8][1] = 0; PacMan2[8][2] = 0; PacMan2[8][3] = 0; PacMan2[8][4] = 0; PacMan2[8][5] = 0; PacMan2[8][6] = 0; 
    PacMan2[8][7] = 0; PacMan2[8][8] = 1; PacMan2[8][9] = 1; PacMan2[8][10] = 1; PacMan2[8][11] = 1; PacMan2[8][12] = 1; PacMan2[8][13] = 1;
    PacMan2[9][0] = 0; PacMan2[9][1] = 0; PacMan2[9][2] = 0; PacMan2[9][3] = 0; PacMan2[9][4] = 0; PacMan2[9][5] = 0; PacMan2[9][6] = 0; 
    PacMan2[9][7] = 1; PacMan2[9][8] = 1; PacMan2[9][9] = 1; PacMan2[9][10] = 1; PacMan2[9][11] = 1; PacMan2[9][12] = 1; PacMan2[9][13] = 0;
    PacMan2[10][0] = 0; PacMan2[10][1] = 0; PacMan2[10][2] = 0; PacMan2[10][3] = 0; PacMan2[10][4] = 0; PacMan2[10][5] = 0; PacMan2[10][6] = 1; 
    PacMan2[10][7] = 1; PacMan2[10][8] = 1; PacMan2[10][9] = 1; PacMan2[10][10] = 1; PacMan2[10][11] = 1; PacMan2[10][12] = 1; PacMan2[10][13] = 0;
    PacMan2[11][0] = 0; PacMan2[11][1] = 0; PacMan2[11][2] = 0; PacMan2[11][3] = 0; PacMan2[11][4] = 0; PacMan2[11][5] = 1; PacMan2[11][6] = 1; 
    PacMan2[11][7] = 1; PacMan2[11][8] = 1; PacMan2[11][9] = 1; PacMan2[11][10] = 1; PacMan2[11][11] = 1; PacMan2[11][12] = 0; PacMan2[11][13] = 0;
    PacMan2[12][0] = 0; PacMan2[12][1] = 0; PacMan2[12][2] = 0; PacMan2[12][3] = 0; PacMan2[12][4] = 1; PacMan2[12][5] = 1; PacMan2[12][6] = 1; 
    PacMan2[12][7] = 1; PacMan2[12][8] = 1; PacMan2[12][9] = 1; PacMan2[12][10] = 1; PacMan2[12][11] = 0; PacMan2[12][12] = 0; PacMan2[12][13] = 0;
    PacMan2[13][0] = 0; PacMan2[13][1] = 0; PacMan2[13][2] = 0; PacMan2[13][3] = 0; PacMan2[13][4] = 1; PacMan2[13][5] = 1; PacMan2[13][6] = 1; 
    PacMan2[13][7] = 1; PacMan2[13][8] = 1; PacMan2[13][9] = 0; PacMan2[13][10] = 0; PacMan2[13][11] = 0; PacMan2[13][12] = 0; PacMan2[13][13] = 0;
    /*上向きゴーストの配列*/
    static hword up_ghost[14][14];
    up_ghost[0][0] = 0; up_ghost[0][1] = 0; up_ghost[0][2] = 0; up_ghost[0][3] = 0; up_ghost[0][4] = 0; up_ghost[0][5] = 1; up_ghost[0][6] = 1;
    up_ghost[0][7] = 1; up_ghost[0][8] = 1; up_ghost[0][9] = 0; up_ghost[0][10] = 0; up_ghost[0][11] = 0; up_ghost[0][12] = 0; up_ghost[0][13] = 0;
    up_ghost[1][0] = 0; up_ghost[1][1] = 0; up_ghost[1][2] = 0; up_ghost[1][3] = 3; up_ghost[1][4] = 3; up_ghost[1][5] = 1; up_ghost[1][6] = 1; 
    up_ghost[1][7] = 1; up_ghost[1][8] = 1; up_ghost[1][9] = 3; up_ghost[1][10] = 3; up_ghost[1][11] = 0; up_ghost[1][12] = 0; up_ghost[1][13] = 0;
    up_ghost[2][0] = 0; up_ghost[2][1] = 0; up_ghost[2][2] = 2; up_ghost[2][3] = 3; up_ghost[2][4] = 3; up_ghost[2][5] = 2; up_ghost[2][6] = 1; 
    up_ghost[2][7] = 1; up_ghost[2][8] = 2; up_ghost[2][9] = 3; up_ghost[2][10] = 3; up_ghost[2][11] = 2; up_ghost[2][12] = 0; up_ghost[2][13] = 0;
    up_ghost[3][0] = 0; up_ghost[3][1] = 1; up_ghost[3][2] = 2; up_ghost[3][3] = 2; up_ghost[3][4] = 2; up_ghost[3][5] = 2; up_ghost[3][6] = 1; 
    up_ghost[3][7] = 1; up_ghost[3][8] = 2; up_ghost[3][9] = 2; up_ghost[3][10] = 2; up_ghost[3][11] = 2; up_ghost[3][12] = 1; up_ghost[3][13] = 0;
    up_ghost[4][0] = 0; up_ghost[4][1] = 1; up_ghost[4][2] = 2; up_ghost[4][3] = 2; up_ghost[4][4] = 2; up_ghost[4][5] = 2; up_ghost[4][6] = 1;
    up_ghost[4][7] = 1; up_ghost[4][8] = 2; up_ghost[4][9] = 2; up_ghost[4][10] = 2; up_ghost[4][11] = 2; up_ghost[4][12] = 1; up_ghost[4][13] = 0;
    up_ghost[5][0] = 0; up_ghost[5][1] = 1; up_ghost[5][2] = 1; up_ghost[5][3] = 2; up_ghost[5][4] = 2; up_ghost[5][5] = 1; up_ghost[5][6] = 1;
    up_ghost[5][7] = 1; up_ghost[5][8] = 1; up_ghost[5][9] = 2; up_ghost[5][10] = 2; up_ghost[5][11] = 1; up_ghost[5][12] = 1; up_ghost[5][13] = 0;
    up_ghost[6][0] = 1; up_ghost[6][1] = 1; up_ghost[6][2] = 1; up_ghost[6][3] = 1; up_ghost[6][4] = 1; up_ghost[6][5] = 1; up_ghost[6][6] = 1; 
    up_ghost[6][7] = 1; up_ghost[6][8] = 1; up_ghost[6][9] = 1; up_ghost[6][10] = 1; up_ghost[6][11] = 1; up_ghost[6][12] = 1; up_ghost[6][13] = 1;
    up_ghost[7][0] = 1; up_ghost[7][1] = 1; up_ghost[7][2] = 1; up_ghost[7][3] = 1; up_ghost[7][4] = 1; up_ghost[7][5] = 1; up_ghost[7][6] = 1; 
    up_ghost[7][7] = 1; up_ghost[7][8] = 1; up_ghost[7][9] = 1; up_ghost[7][10] = 1; up_ghost[7][11] = 1; up_ghost[7][12] = 1; up_ghost[7][13] = 1;
    up_ghost[8][0] = 1; up_ghost[8][1] = 1; up_ghost[8][2] = 1; up_ghost[8][3] = 1; up_ghost[8][4] = 1; up_ghost[8][5] = 1; up_ghost[8][6] = 1;
    up_ghost[8][7] = 1; up_ghost[8][8] = 1; up_ghost[8][9] = 1; up_ghost[8][10] = 1; up_ghost[8][11] = 1; up_ghost[8][12] = 1; up_ghost[8][13] = 1;
    up_ghost[9][0] = 1; up_ghost[9][1] = 1; up_ghost[9][2] = 1; up_ghost[9][3] = 1; up_ghost[9][4] = 1; up_ghost[9][5] = 1; up_ghost[9][6] = 1;
    up_ghost[9][7] = 1; up_ghost[9][8] = 1; up_ghost[9][9] = 1; up_ghost[9][10] = 1; up_ghost[9][11] = 1; up_ghost[9][12] = 1; up_ghost[9][13] = 1;
    up_ghost[10][0] = 1; up_ghost[10][1] = 1; up_ghost[10][2] = 1; up_ghost[10][3] = 1; up_ghost[10][4] = 1; up_ghost[10][5] = 1; up_ghost[10][6] = 1; 
    up_ghost[10][7] = 1; up_ghost[10][8] = 1; up_ghost[10][9] = 1; up_ghost[10][10] = 1; up_ghost[10][11] = 1; up_ghost[10][12] = 1; up_ghost[10][13] = 1;
    up_ghost[11][0] = 1; up_ghost[11][1] = 1; up_ghost[11][2] = 1; up_ghost[11][3] = 1; up_ghost[11][4] = 1; up_ghost[11][5] = 1; up_ghost[11][6] = 1;
    up_ghost[11][7] = 1; up_ghost[11][8] = 1; up_ghost[11][9] = 1; up_ghost[11][10] = 1; up_ghost[11][11] = 1; up_ghost[11][12] = 1; up_ghost[11][13] = 1;
    up_ghost[12][0] = 1; up_ghost[12][1] = 1; up_ghost[12][2] = 1; up_ghost[12][3] = 1; up_ghost[12][4] = 0; up_ghost[12][5] = 1; up_ghost[12][6] = 1; 
    up_ghost[12][7] = 1; up_ghost[12][8] = 1; up_ghost[12][9] = 0; up_ghost[12][10] = 1; up_ghost[12][11] = 1; up_ghost[12][12] = 1; up_ghost[12][13] = 1;
    up_ghost[13][0] = 0; up_ghost[13][1] = 1; up_ghost[13][2] = 1; up_ghost[13][3] = 0; up_ghost[13][4] = 0; up_ghost[13][5] = 0; up_ghost[13][6] = 1;
    up_ghost[13][7] = 1; up_ghost[13][8] = 0; up_ghost[13][9] = 0; up_ghost[13][10] = 0; up_ghost[13][11] = 1; up_ghost[13][12] = 1; up_ghost[13][13] = 0;
    /*下向きゴーストの配列*/
    static hword down_ghost[14][14];
    down_ghost[0][0] = 0; down_ghost[0][1] = 0; down_ghost[0][2] = 0; down_ghost[0][3] = 0; down_ghost[0][4] = 0; down_ghost[0][5] = 1; down_ghost[0][6] = 1; 
    down_ghost[0][7] = 1; down_ghost[0][8] = 1; down_ghost[0][9] = 0; down_ghost[0][10] = 0; down_ghost[0][11] = 0; down_ghost[0][12] = 0; down_ghost[0][13] = 0;
    down_ghost[1][0] = 0; down_ghost[1][1] = 0; down_ghost[1][2] = 0; down_ghost[1][3] = 1; down_ghost[1][4] = 1; down_ghost[1][5] = 1; down_ghost[1][6] = 1; 
    down_ghost[1][7] = 1; down_ghost[1][8] = 1; down_ghost[1][9] = 1; down_ghost[1][10] = 1; down_ghost[1][11] = 0; down_ghost[1][12] = 0; down_ghost[1][13] = 0;
    down_ghost[2][0] = 0; down_ghost[2][1] = 0; down_ghost[2][2] = 1; down_ghost[2][3] = 1; down_ghost[2][4] = 1; down_ghost[2][5] = 1; down_ghost[2][6] = 1;
    down_ghost[2][7] = 1; down_ghost[2][8] = 1; down_ghost[2][9] = 1; down_ghost[2][10] = 1; down_ghost[2][11] = 1; down_ghost[2][12] = 0; down_ghost[2][13] = 0;
    down_ghost[3][0] = 0; down_ghost[3][1] = 1; down_ghost[3][2] = 1; down_ghost[3][3] = 1; down_ghost[3][4] = 1; down_ghost[3][5] = 1; down_ghost[3][6] = 1;
    down_ghost[3][7] = 1; down_ghost[3][8] = 1; down_ghost[3][9] = 1; down_ghost[3][10] = 1; down_ghost[3][11] = 1; down_ghost[3][12] = 1; down_ghost[3][13] = 0;
    down_ghost[4][0] = 0; down_ghost[4][1] = 1; down_ghost[4][2] = 1; down_ghost[4][3] = 2; down_ghost[4][4] = 2; down_ghost[4][5] = 1; down_ghost[4][6] = 1;
    down_ghost[4][7] = 1; down_ghost[4][8] = 1; down_ghost[4][9] = 2; down_ghost[4][10] = 2; down_ghost[4][11] = 1; down_ghost[4][12] = 1; down_ghost[4][13] = 0;
    down_ghost[5][0] = 0; down_ghost[5][1] = 1; down_ghost[5][2] = 2; down_ghost[5][3] = 2; down_ghost[5][4] = 2; down_ghost[5][5] = 2; down_ghost[5][6] = 1; 
    down_ghost[5][7] = 1; down_ghost[5][8] = 2; down_ghost[5][9] = 2; down_ghost[5][10] = 2; down_ghost[5][11] = 2; down_ghost[5][12] = 1; down_ghost[5][13] = 0;
    down_ghost[6][0] = 1; down_ghost[6][1] = 1; down_ghost[6][2] = 2; down_ghost[6][3] = 2; down_ghost[6][4] = 2; down_ghost[6][5] = 2; down_ghost[6][6] = 1; 
    down_ghost[6][7] = 1; down_ghost[6][8] = 2; down_ghost[6][9] = 2; down_ghost[6][10] = 2; down_ghost[6][11] = 2; down_ghost[6][12] = 1; down_ghost[6][13] = 1;
    down_ghost[7][0] = 1; down_ghost[7][1] = 1; down_ghost[7][2] = 2; down_ghost[7][3] = 3; down_ghost[7][4] = 3; down_ghost[7][5] = 2; down_ghost[7][6] = 1; 
    down_ghost[7][7] = 1; down_ghost[7][8] = 2; down_ghost[7][9] = 3; down_ghost[7][10] = 3; down_ghost[7][11] = 2; down_ghost[7][12] = 1; down_ghost[7][13] = 1;
    down_ghost[8][0] = 1; down_ghost[8][1] = 1; down_ghost[8][2] = 1; down_ghost[8][3] = 3; down_ghost[8][4] = 3; down_ghost[8][5] = 1; down_ghost[8][6] = 1; 
    down_ghost[8][7] = 1; down_ghost[8][8] = 1; down_ghost[8][9] = 3; down_ghost[8][10] = 3; down_ghost[8][11] = 1; down_ghost[8][12] = 1; down_ghost[8][13] = 1;
    down_ghost[9][0] = 1; down_ghost[9][1] = 1; down_ghost[9][2] = 1; down_ghost[9][3] = 1; down_ghost[9][4] = 1; down_ghost[9][5] = 1; down_ghost[9][6] = 1; 
    down_ghost[9][7] = 1; down_ghost[9][8] = 1; down_ghost[9][9] = 1; down_ghost[9][10] = 1; down_ghost[9][11] = 1; down_ghost[9][12] = 1; down_ghost[9][13] = 1;
    down_ghost[10][0] = 1; down_ghost[10][1] = 1; down_ghost[10][2] = 1; down_ghost[10][3] = 1; down_ghost[10][4] = 1; down_ghost[10][5] = 1; down_ghost[10][6] = 1;
    down_ghost[10][7] = 1; down_ghost[10][8] = 1; down_ghost[10][9] = 1; down_ghost[10][10] = 1; down_ghost[10][11] = 1; down_ghost[10][12] = 1; down_ghost[10][13] = 1;
    down_ghost[11][0] = 1; down_ghost[11][1] = 1; down_ghost[11][2] = 1; down_ghost[11][3] = 1; down_ghost[11][4] = 1; down_ghost[11][5] = 1; down_ghost[11][6] = 1;
    down_ghost[11][7] = 1; down_ghost[11][8] = 1; down_ghost[11][9] = 1; down_ghost[11][10] = 1; down_ghost[11][11] = 1; down_ghost[11][12] = 1; down_ghost[11][13] = 1;
    down_ghost[12][0] = 1; down_ghost[12][1] = 1; down_ghost[12][2] = 1; down_ghost[12][3] = 1; down_ghost[12][4] = 0; down_ghost[12][5] = 1; down_ghost[12][6] = 1; 
    down_ghost[12][7] = 1; down_ghost[12][8] = 1; down_ghost[12][9] = 0; down_ghost[12][10] = 1; down_ghost[12][11] = 1; down_ghost[12][12] = 1; down_ghost[12][13] = 1;
    down_ghost[13][0] = 0; down_ghost[13][1] = 1; down_ghost[13][2] = 1; down_ghost[13][3] = 0; down_ghost[13][4] = 0; down_ghost[13][5] = 0; down_ghost[13][6] = 1; 
    down_ghost[13][7] = 1; down_ghost[13][8] = 0; down_ghost[13][9] = 0; down_ghost[13][10] = 0; down_ghost[13][11] = 1; down_ghost[13][12] = 1; down_ghost[13][13] = 0;
    /*左向きゴーストの配列*/
    static hword left_ghost[14][14];
    left_ghost[0][0] = 0; left_ghost[0][1] = 0; left_ghost[0][2] = 0; left_ghost[0][3] = 0; left_ghost[0][4] = 0; left_ghost[0][5] = 1; left_ghost[0][6] = 1;
    left_ghost[0][7] = 1; left_ghost[0][8] = 1; left_ghost[0][9] = 0; left_ghost[0][10] = 0; left_ghost[0][11] = 0; left_ghost[0][12] = 0; left_ghost[0][13] = 0;
    left_ghost[1][0] = 0; left_ghost[1][1] = 0; left_ghost[1][2] = 0; left_ghost[1][3] = 1; left_ghost[1][4] = 1; left_ghost[1][5] = 1; left_ghost[1][6] = 1; 
    left_ghost[1][7] = 1; left_ghost[1][8] = 1; left_ghost[1][9] = 1; left_ghost[1][10] = 1; left_ghost[1][11] = 0; left_ghost[1][12] = 0; left_ghost[1][13] = 0;
    left_ghost[2][0] = 0; left_ghost[2][1] = 0; left_ghost[2][2] = 1; left_ghost[2][3] = 1; left_ghost[2][4] = 1; left_ghost[2][5] = 1; left_ghost[2][6] = 1; 
    left_ghost[2][7] = 1; left_ghost[2][8] = 1; left_ghost[2][9] = 1; left_ghost[2][10] = 1; left_ghost[2][11] = 1; left_ghost[2][12] = 0; left_ghost[2][13] = 0;
    left_ghost[3][0] = 0; left_ghost[3][1] = 1; left_ghost[3][2] = 2; left_ghost[3][3] = 2; left_ghost[3][4] = 1; left_ghost[3][5] = 1; left_ghost[3][6] = 1; 
    left_ghost[3][7] = 1; left_ghost[3][8] = 2; left_ghost[3][9] = 2; left_ghost[3][10] = 1; left_ghost[3][11] = 1; left_ghost[3][12] = 1; left_ghost[3][13] = 0;
    left_ghost[4][0] = 1; left_ghost[4][1] = 2; left_ghost[4][2] = 2; left_ghost[4][3] = 2; left_ghost[4][4] = 2; left_ghost[4][5] = 1; left_ghost[4][6] = 1;
    left_ghost[4][7] = 2; left_ghost[4][8] = 2; left_ghost[4][9] = 2; left_ghost[4][10] = 2; left_ghost[4][11] = 1; left_ghost[4][12] = 1; left_ghost[4][13] = 1;
    left_ghost[5][0] = 1; left_ghost[5][1] = 3; left_ghost[5][2] = 3; left_ghost[5][3] = 2; left_ghost[5][4] = 2; left_ghost[5][5] = 1; left_ghost[5][6] = 1;
    left_ghost[5][7] = 3; left_ghost[5][8] = 3; left_ghost[5][9] = 2; left_ghost[5][10] = 2; left_ghost[5][11] = 1; left_ghost[5][12] = 1; left_ghost[5][13] = 1;
    left_ghost[6][0] = 1; left_ghost[6][1] = 3; left_ghost[6][2] = 3; left_ghost[6][3] = 2; left_ghost[6][4] = 2; left_ghost[6][5] = 1; left_ghost[6][6] = 1;
    left_ghost[6][7] = 3; left_ghost[6][8] = 3; left_ghost[6][9] = 2; left_ghost[6][10] = 2; left_ghost[6][11] = 1; left_ghost[6][12] = 1; left_ghost[6][13] = 1;
    left_ghost[7][0] = 1; left_ghost[7][1] = 1; left_ghost[7][2] = 2; left_ghost[7][3] = 2; left_ghost[7][4] = 1; left_ghost[7][5] = 1; left_ghost[7][6] = 1; 
    left_ghost[7][7] = 1; left_ghost[7][8] = 2; left_ghost[7][9] = 2; left_ghost[7][10] = 1; left_ghost[7][11] = 1; left_ghost[7][12] = 1; left_ghost[7][13] = 1;
    left_ghost[8][0] = 1; left_ghost[8][1] = 1; left_ghost[8][2] = 1; left_ghost[8][3] = 1; left_ghost[8][4] = 1; left_ghost[8][5] = 1; left_ghost[8][6] = 1; 
    left_ghost[8][7] = 1; left_ghost[8][8] = 1; left_ghost[8][9] = 1; left_ghost[8][10] = 1; left_ghost[8][11] = 1; left_ghost[8][12] = 1; left_ghost[8][13] = 1;
    left_ghost[9][0] = 1; left_ghost[9][1] = 1; left_ghost[9][2] = 1; left_ghost[9][3] = 1; left_ghost[9][4] = 1; left_ghost[9][5] = 1; left_ghost[9][6] = 1; 
    left_ghost[9][7] = 1; left_ghost[9][8] = 1; left_ghost[9][9] = 1; left_ghost[9][10] = 1; left_ghost[9][11] = 1; left_ghost[9][12] = 1; left_ghost[9][13] = 1;
    left_ghost[10][0] = 1; left_ghost[10][1] = 1; left_ghost[10][2] = 1; left_ghost[10][3] = 1; left_ghost[10][4] = 1; left_ghost[10][5] = 1; left_ghost[10][6] = 1;
    left_ghost[10][7] = 1; left_ghost[10][8] = 1; left_ghost[10][9] = 1; left_ghost[10][10] = 1; left_ghost[10][11] = 1; left_ghost[10][12] = 1; left_ghost[10][13] = 1;
    left_ghost[11][0] = 1; left_ghost[11][1] = 1; left_ghost[11][2] = 1; left_ghost[11][3] = 1; left_ghost[11][4] = 1; left_ghost[11][5] = 1; left_ghost[11][6] = 1; 
    left_ghost[11][7] = 1; left_ghost[11][8] = 1; left_ghost[11][9] = 1; left_ghost[11][10] = 1; left_ghost[11][11] = 1; left_ghost[11][12] = 1; left_ghost[11][13] = 1;
    left_ghost[12][0] = 1; left_ghost[12][1] = 1; left_ghost[12][2] = 1; left_ghost[12][3] = 1; left_ghost[12][4] = 0; left_ghost[12][5] = 1; left_ghost[12][6] = 1; 
    left_ghost[12][7] = 1; left_ghost[12][8] = 1; left_ghost[12][9] = 0; left_ghost[12][10] = 1; left_ghost[12][11] = 1; left_ghost[12][12] = 1; left_ghost[12][13] = 1;
    left_ghost[13][0] = 0; left_ghost[13][1] = 1; left_ghost[13][2] = 1; left_ghost[13][3] = 0; left_ghost[13][4] = 0; left_ghost[13][5] = 0; left_ghost[13][6] = 1;
    left_ghost[13][7] = 1; left_ghost[13][8] = 0; left_ghost[13][9] = 0; left_ghost[13][10] = 0; left_ghost[13][11] = 1; left_ghost[13][12] = 1; left_ghost[13][13] = 0;
    /*右向きゴーストの配列*/
    static hword right_ghost[14][14];
    right_ghost[0][0] = 0; right_ghost[0][1] = 0; right_ghost[0][2] = 0; right_ghost[0][3] = 0; right_ghost[0][4] = 0; right_ghost[0][5] = 1; right_ghost[0][6] = 1; 
    right_ghost[0][7] = 1; right_ghost[0][8] = 1; right_ghost[0][9] = 0; right_ghost[0][10] = 0; right_ghost[0][11] = 0; right_ghost[0][12] = 0; right_ghost[0][13] = 0;
    right_ghost[1][0] = 0; right_ghost[1][1] = 0; right_ghost[1][2] = 0; right_ghost[1][3] = 1; right_ghost[1][4] = 1; right_ghost[1][5] = 1; right_ghost[1][6] = 1; 
    right_ghost[1][7] = 1; right_ghost[1][8] = 1; right_ghost[1][9] = 1; right_ghost[1][10] = 1; right_ghost[1][11] = 0; right_ghost[1][12] = 0; right_ghost[1][13] = 0;
    right_ghost[2][0] = 0; right_ghost[2][1] = 0; right_ghost[2][2] = 1; right_ghost[2][3] = 1; right_ghost[2][4] = 1; right_ghost[2][5] = 1; right_ghost[2][6] = 1; 
    right_ghost[2][7] = 1; right_ghost[2][8] = 1; right_ghost[2][9] = 1; right_ghost[2][10] = 1; right_ghost[2][11] = 1; right_ghost[2][12] = 0; right_ghost[2][13] = 0;
    right_ghost[3][0] = 0; right_ghost[3][1] = 1; right_ghost[3][2] = 1; right_ghost[3][3] = 1; right_ghost[3][4] = 2; right_ghost[3][5] = 2; right_ghost[3][6] = 1;
    right_ghost[3][7] = 1; right_ghost[3][8] = 1; right_ghost[3][9] = 1; right_ghost[3][10] = 2; right_ghost[3][11] = 2; right_ghost[3][12] = 1; right_ghost[3][13] = 0;
    right_ghost[4][0] = 1; right_ghost[4][1] = 1; right_ghost[4][2] = 1; right_ghost[4][3] = 2; right_ghost[4][4] = 2; right_ghost[4][5] = 2; right_ghost[4][6] = 2; 
    right_ghost[4][7] = 1; right_ghost[4][8] = 1; right_ghost[4][9] = 2; right_ghost[4][10] = 2; right_ghost[4][11] = 2; right_ghost[4][12] = 2; right_ghost[4][13] = 1;
    right_ghost[5][0] = 1; right_ghost[5][1] = 1; right_ghost[5][2] = 1; right_ghost[5][3] = 2; right_ghost[5][4] = 2; right_ghost[5][5] = 3; right_ghost[5][6] = 3; 
    right_ghost[5][7] = 1; right_ghost[5][8] = 1; right_ghost[5][9] = 2; right_ghost[5][10] = 2; right_ghost[5][11] = 3; right_ghost[5][12] = 3; right_ghost[5][13] = 1;
    right_ghost[6][0] = 1; right_ghost[6][1] = 1; right_ghost[6][2] = 1; right_ghost[6][3] = 2; right_ghost[6][4] = 2; right_ghost[6][5] = 3; right_ghost[6][6] = 3; 
    right_ghost[6][7] = 1; right_ghost[6][8] = 1; right_ghost[6][9] = 2; right_ghost[6][10] = 2; right_ghost[6][11] = 3; right_ghost[6][12] = 3; right_ghost[6][13] = 1;
    right_ghost[7][0] = 1; right_ghost[7][1] = 1; right_ghost[7][2] = 1; right_ghost[7][3] = 1; right_ghost[7][4] = 2; right_ghost[7][5] = 2; right_ghost[7][6] = 1;
    right_ghost[7][7] = 1; right_ghost[7][8] = 1; right_ghost[7][9] = 1; right_ghost[7][10] = 2; right_ghost[7][11] = 2; right_ghost[7][12] = 1; right_ghost[7][13] = 1;
    right_ghost[8][0] = 1; right_ghost[8][1] = 1; right_ghost[8][2] = 1; right_ghost[8][3] = 1; right_ghost[8][4] = 1; right_ghost[8][5] = 1; right_ghost[8][6] = 1;
    right_ghost[8][7] = 1; right_ghost[8][8] = 1; right_ghost[8][9] = 1; right_ghost[8][10] = 1; right_ghost[8][11] = 1; right_ghost[8][12] = 1; right_ghost[8][13] = 1;
    right_ghost[9][0] = 1; right_ghost[9][1] = 1; right_ghost[9][2] = 1; right_ghost[9][3] = 1; right_ghost[9][4] = 1; right_ghost[9][5] = 1; right_ghost[9][6] = 1; 
    right_ghost[9][7] = 1; right_ghost[9][8] = 1; right_ghost[9][9] = 1; right_ghost[9][10] = 1; right_ghost[9][11] = 1; right_ghost[9][12] = 1; right_ghost[9][13] = 1;
    right_ghost[10][0] = 1; right_ghost[10][1] = 1; right_ghost[10][2] = 1; right_ghost[10][3] = 1; right_ghost[10][4] = 1; right_ghost[10][5] = 1; right_ghost[10][6] = 1; 
    right_ghost[10][7] = 1; right_ghost[10][8] = 1; right_ghost[10][9] = 1; right_ghost[10][10] = 1; right_ghost[10][11] = 1; right_ghost[10][12] = 1; right_ghost[10][13] = 1;
    right_ghost[11][0] = 1; right_ghost[11][1] = 1; right_ghost[11][2] = 1; right_ghost[11][3] = 1; right_ghost[11][4] = 1; right_ghost[11][5] = 1; right_ghost[11][6] = 1; 
    right_ghost[11][7] = 1; right_ghost[11][8] = 1; right_ghost[11][9] = 1; right_ghost[11][10] = 1; right_ghost[11][11] = 1; right_ghost[11][12] = 1; right_ghost[11][13] = 1;
    right_ghost[12][0] = 1; right_ghost[12][1] = 1; right_ghost[12][2] = 1; right_ghost[12][3] = 1; right_ghost[12][4] = 0; right_ghost[12][5] = 1; right_ghost[12][6] = 1; 
    right_ghost[12][7] = 1; right_ghost[12][8] = 1; right_ghost[12][9] = 0; right_ghost[12][10] = 1; right_ghost[12][11] = 1; right_ghost[12][12] = 1; right_ghost[12][13] = 1;
    right_ghost[13][0] = 0; right_ghost[13][1] = 1; right_ghost[13][2] = 1; right_ghost[13][3] = 0; right_ghost[13][4] = 0; right_ghost[13][5] = 0; right_ghost[13][6] = 1;
    right_ghost[13][7] = 1; right_ghost[13][8] = 0; right_ghost[13][9] = 0; right_ghost[13][10] = 0; right_ghost[13][11] = 1; right_ghost[13][12] = 1; right_ghost[13][13] = 0;
    
    /*マップを描画する情報を格納した配列*/
    static hword map[18][24] ;
    /*1:左上外カーブ, 2:右上外カーブ, 3:左下外カーブ, 4:右下外カーブ,
     5:横棒上, 6:横棒下, 7:縦線左, 8:縦線右, 9:点, 10:黒 ,
    11:左上内カーブ, 12:右上内カーブ, 13:左下内カーブ, 14:右下内カーブ,
    15:左上特殊カーブ, 16:右上特殊カーブ, 17:左下特殊カーブ, 18:右下特殊カーブ
    19:ゲート　
    １つの塊をドット8×8とする
    3,4,5は下に道があるから上から４つ目にすることで道を広くする。
    1,2,6は上に道があるから上から５つ目に書くことで道を広くしている。*/

    map[0][0] = 4; map[0][1] = 5; map[0][2] = 5; map[0][3] = 5; map[0][4] = 5; map[0][5] = 5;
    map[0][6] = 5; map[0][7] = 5; map[0][8] = 5; map[0][9] = 5; map[0][10] = 5; map[0][11] = 5;
    map[0][12] = 5; map[0][13] = 5; map[0][14] = 5; map[0][15] = 5; map[0][16] = 5; map[0][17] = 5;
    map[0][18] = 5; map[0][19] = 5; map[0][20] = 5; map[0][21] = 5; map[0][22] = 5; map[0][23] = 3;
    map[1][0] = 7; map[1][1] = 9; map[1][2] = 9; map[1][3] = 9; map[1][4] = 9; map[1][5] = 9;
    map[1][6] = 9; map[1][7] = 9; map[1][8] = 9; map[1][9] = 9; map[1][10] = 9; map[1][11] = 9;
    map[1][12] = 9; map[1][13] = 9; map[1][14] = 9; map[1][15] = 9; map[1][16] = 9; map[1][17] = 9;
    map[1][18] = 9; map[1][19] = 9; map[1][20] = 9; map[1][21] = 9; map[1][22] = 9; map[1][23] = 8;
    map[2][0] = 7; map[2][1] = 9; map[2][2] = 14; map[2][3] = 13; map[2][4] = 9; map[2][5] = 14;
    map[2][6] = 6; map[2][7] = 6; map[2][8] = 6; map[2][9] = 13; map[2][10] = 9; map[2][11] = 14;
    map[2][12] = 13; map[2][13] = 9; map[2][14] = 14; map[2][15] = 6; map[2][16] = 6; map[2][17] = 6;
    map[2][18] = 13; map[2][19] = 9; map[2][20] = 14; map[2][21] = 13; map[2][22] = 9; map[2][23] = 8;
    map[3][0] = 7; map[3][1] = 9; map[3][2] = 12; map[3][3] = 11; map[3][4] = 9; map[3][5] = 8;
    map[3][6] = 4; map[3][7] = 5; map[3][8] = 5; map[3][9] = 11; map[3][10] = 9; map[3][11] = 12;
    map[3][12] = 11; map[3][13] = 9; map[3][14] = 12; map[3][15] = 5; map[3][16] = 5; map[3][17] = 3;
    map[3][18] = 7; map[3][19] = 9; map[3][20] = 12; map[3][21] = 11; map[3][22] = 9; map[3][23] = 8;
    map[4][0] = 7; map[4][1] = 9; map[4][2] = 9; map[4][3] = 9; map[4][4] = 9; map[4][5] = 8;
    map[4][6] = 7; map[4][7] = 9; map[4][8] = 9; map[4][9] = 9; map[4][10] = 9; map[4][11] = 9;
    map[4][12] = 9; map[4][13] = 9; map[4][14] = 9; map[4][15] = 9; map[4][16] = 9; map[4][17] = 8;
    map[4][18] = 7; map[4][19] = 9; map[4][20] = 9; map[4][21] = 9; map[4][22] = 9; map[4][23] = 8;
    map[5][0] = 2; map[5][1] = 6; map[5][2] = 6; map[5][3] = 13; map[5][4] = 9; map[5][5] = 8;
    map[5][6] = 7; map[5][7] = 9; map[5][8] = 14; map[5][9] = 6; map[5][10] = 17; map[5][11] = 19;
    map[5][12] = 19; map[5][13] = 18; map[5][14] = 6; map[5][15] = 13; map[5][16] = 9; map[5][17] = 8;
    map[5][18] = 7; map[5][19] = 9; map[5][20] = 14; map[5][21] = 6; map[5][22] = 6; map[5][23] = 1;
    map[6][0] = 10; map[6][1] = 10; map[6][2] = 10; map[6][3] = 7; map[6][4] = 9; map[6][5] = 12;
    map[6][6] = 11; map[6][7] = 9; map[6][8] = 8; map[6][9] = 10; map[6][10] = 10; map[6][11] = 10;
    map[6][12] = 10; map[6][13] = 10; map[6][14] = 10; map[6][15] = 7; map[6][16] = 9; map[6][17] = 12;
    map[6][18] = 11; map[6][19] = 9; map[6][20] = 8; map[6][21] = 10; map[6][22] = 10; map[6][23] = 10;
    map[7][0] = 10; map[7][1] = 10; map[7][2] = 10; map[7][3] = 7; map[7][4] = 9; map[7][5] = 9;
    map[7][6] = 9; map[7][7] = 9; map[7][8] = 8; map[7][9] = 10; map[7][10] = 10; map[7][11] = 10;
    map[7][12] = 10; map[7][13] = 10; map[7][14] = 10; map[7][15] = 7; map[7][16] = 9; map[7][17] = 9;
    map[7][18] = 9; map[7][19] = 9; map[7][20] = 8; map[7][21] = 10; map[7][22] = 10; map[7][23] = 10;
    map[8][0] = 10; map[8][1] = 10; map[8][2] = 10; map[8][3] = 7; map[8][4] = 9; map[8][5] = 14;
    map[8][6] = 13; map[8][7] = 9; map[8][8] = 8; map[8][9] = 10; map[8][10] = 10; map[8][11] = 10;
    map[8][12] = 10; map[8][13] = 10; map[8][14] = 10; map[8][15] = 7; map[8][16] = 9; map[8][17] = 14;
    map[8][18] = 13; map[8][19] = 9; map[8][20] = 8; map[8][21] = 10; map[8][22] = 10; map[8][23] = 10;
    map[9][0] = 4; map[9][1] = 5; map[9][2] = 5; map[9][3] = 11; map[9][4] = 9; map[9][5] = 8;
    map[9][6] = 7; map[9][7] = 9; map[9][8] = 12; map[9][9] = 5; map[9][10] = 5; map[9][11] = 3;
    map[9][12] = 4; map[9][13] = 5; map[9][14] = 5; map[9][15] = 11; map[9][16] = 9; map[9][17] = 8;
    map[9][18] = 7; map[9][19] = 9; map[9][20] = 12; map[9][21] = 5; map[9][22] = 5; map[9][23] = 3;
    map[10][0] = 7; map[10][1] = 9; map[10][2] = 9; map[10][3] = 9; map[10][4] = 9; map[10][5] = 8;
    map[10][6] = 7; map[10][7] = 9; map[10][8] = 9; map[10][9] = 9; map[10][10] = 9; map[10][11] = 8;
    map[10][12] = 7; map[10][13] = 9; map[10][14] = 9; map[10][15] = 9; map[10][16] = 9; map[10][17] = 8;
    map[10][18] = 7; map[10][19] = 9; map[10][20] = 9; map[10][21] = 9; map[10][22] = 9; map[10][23] = 8;
    map[11][0] = 7; map[11][1] = 9; map[11][2] = 14; map[11][3] = 13; map[11][4] = 9; map[11][5] = 8;
    map[11][6] = 2; map[11][7] = 6; map[11][8] = 6; map[11][9] = 13; map[11][10] = 9; map[11][11] = 8;
    map[11][12] = 7; map[11][13] = 9; map[11][14] = 14; map[11][15] = 6; map[11][16] = 6; map[11][17] = 1;
    map[11][18] = 7; map[11][19] = 9; map[11][20] = 14; map[11][21] = 13; map[11][22] = 9; map[11][23] = 8;
    map[12][0] = 7; map[12][1] = 9; map[12][2] = 12; map[12][3] = 11; map[12][4] = 9; map[12][5] = 12;
    map[12][6] = 5; map[12][7] = 5; map[12][8] = 5; map[12][9] = 11; map[12][10] = 9; map[12][11] = 12;
    map[12][12] = 11; map[12][13] = 9; map[12][14] = 12; map[12][15] = 5; map[12][16] = 5; map[12][17] = 5;
    map[12][18] = 11; map[12][19] = 9; map[12][20] = 12; map[12][21] = 11; map[12][22] = 9; map[12][23] = 8;
    map[13][0] = 7; map[13][1] = 9; map[13][2] = 9; map[13][3] = 9; map[13][4] = 9; map[13][5] = 9;
    map[13][6] = 9; map[13][7] = 9; map[13][8] = 9; map[13][9] = 9; map[13][10] = 9; map[13][11] = 9;
    map[13][12] = 9; map[13][13] = 9; map[13][14] = 9; map[13][15] = 9; map[13][16] = 9; map[13][17] = 9;
    map[13][18] = 9; map[13][19] = 9; map[13][20] = 9; map[13][21] = 9; map[13][22] = 9; map[13][23] = 8;
    map[14][0] = 7; map[14][1] = 9; map[14][2] = 14; map[14][3] = 13; map[14][4] = 9; map[14][5] = 14;
    map[14][6] = 13; map[14][7] = 9; map[14][8] = 14; map[14][9] = 6; map[14][10] = 17; map[14][11] = 10;
    map[14][12] = 10; map[14][13] = 18; map[14][14] = 6; map[14][15] = 13; map[14][16] = 9; map[14][17] = 14;
    map[14][18] = 13; map[14][19] = 9; map[14][20] = 14; map[14][21] = 13; map[14][22] = 9; map[14][23] = 8;
    map[15][0] = 7; map[15][1] = 9; map[15][2] = 12; map[15][3] = 11; map[15][4] = 9; map[15][5] = 8;
    map[15][6] = 7; map[15][7] = 9; map[15][8] = 12; map[15][9] = 5; map[15][10] = 15; map[15][11] = 10;
    map[15][12] = 10; map[15][13] = 16; map[15][14] = 5; map[15][15] = 11; map[15][16] = 9; map[15][17] = 8;
    map[15][18] = 7; map[15][19] = 9; map[15][20] = 12; map[15][21] = 11; map[15][22] = 9; map[15][23] = 8;
    map[16][0] = 7; map[16][1] = 9; map[16][2] = 9; map[16][3] = 9; map[16][4] = 9; map[16][5] = 8;
    map[16][6] = 7; map[16][7] = 9; map[16][8] = 9; map[16][9] = 9; map[16][10] = 9; map[16][11] = 9;
    map[16][12] = 9; map[16][13] = 9; map[16][14] = 9; map[16][15] = 9; map[16][16] = 9; map[16][17] = 8;
    map[16][18] = 7; map[16][19] = 9; map[16][20] = 9; map[16][21] = 9; map[16][22] = 9; map[16][23] = 8;
    map[17][0] = 2; map[17][1] = 6; map[17][2] = 6; map[17][3] = 6; map[17][4] = 6; map[17][5] = 1;
    map[17][6] = 2; map[17][7] = 6; map[17][8] = 6; map[17][9] = 6; map[17][10] = 6; map[17][11] = 6;
    map[17][12] = 6; map[17][13] = 6; map[17][14] = 6; map[17][15] = 6; map[17][16] = 6; map[17][17] = 1;
    map[17][18] = 2; map[17][19] = 6; map[17][20] = 6; map[17][21] = 6; map[17][22] = 6; map[17][23] = 1;
    /*スコアの「Ｓ」を描画する情報*/
    static hword write_s[10][10];
    write_s[0][0] = 0; write_s[0][1] = 0; write_s[0][2] = 0; write_s[0][3] = 0; write_s[0][4] = 0; write_s[0][5] = 0; write_s[0][6] = 0; write_s[0][7] = 0; write_s[0][8] = 0; write_s[0][9] = 0; 
    write_s[1][0] = 0; write_s[1][1] = 1; write_s[1][2] = 1; write_s[1][3] = 1; write_s[1][4] = 1; write_s[1][5] = 1; write_s[1][6] = 1; write_s[1][7] = 1; write_s[1][8] = 0; write_s[1][9] = 0; 
    write_s[2][0] = 0; write_s[2][1] = 1; write_s[2][2] = 0; write_s[2][3] = 0; write_s[2][4] = 0; write_s[2][5] = 0; write_s[2][6] = 0; write_s[2][7] = 0; write_s[2][8] = 0; write_s[2][9] = 0; 
    write_s[3][0] = 0; write_s[3][1] = 1; write_s[3][2] = 0; write_s[3][3] = 0; write_s[3][4] = 0; write_s[3][5] = 0; write_s[3][6] = 0; write_s[3][7] = 0; write_s[3][8] = 0; write_s[3][9] = 0; 
    write_s[4][0] = 0; write_s[4][1] = 1; write_s[4][2] = 0; write_s[4][3] = 0; write_s[4][4] = 0; write_s[4][5] = 0; write_s[4][6] = 0; write_s[4][7] = 0; write_s[4][8] = 0; write_s[4][9] = 0; 
    write_s[5][0] = 0; write_s[5][1] = 1; write_s[5][2] = 1; write_s[5][3] = 1; write_s[5][4] = 1; write_s[5][5] = 1; write_s[5][6] = 1; write_s[5][7] = 1; write_s[5][8] = 0; write_s[5][9] = 0; 
    write_s[6][0] = 0; write_s[6][1] = 0; write_s[6][2] = 0; write_s[6][3] = 0; write_s[6][4] = 0; write_s[6][5] = 0; write_s[6][6] = 0; write_s[6][7] = 1; write_s[6][8] = 0; write_s[6][9] = 0; 
    write_s[7][0] = 0; write_s[7][1] = 0; write_s[7][2] = 0; write_s[7][3] = 0; write_s[7][4] = 0; write_s[7][5] = 0; write_s[7][6] = 0; write_s[7][7] = 1; write_s[7][8] = 0; write_s[7][9] = 0; 
    write_s[8][0] = 0; write_s[8][1] = 0; write_s[8][2] = 0; write_s[8][3] = 0; write_s[8][4] = 0; write_s[8][5] = 0; write_s[8][6] = 0; write_s[8][7] = 1; write_s[8][8] = 0; write_s[8][9] = 0; 
    write_s[9][0] = 0; write_s[9][1] = 1; write_s[9][2] = 1; write_s[9][3] = 1; write_s[9][4] = 1; write_s[9][5] = 1; write_s[9][6] = 1; write_s[9][7] = 1; write_s[9][8] = 0; write_s[9][9] = 0; 
    /*スコアの「Ｃ」を描画する情報*/
    static hword write_c[10][10];
    write_c[0][0] = 0; write_c[0][1] = 0; write_c[0][2] = 0; write_c[0][3] = 0; write_c[0][4] = 0; write_c[0][5] = 0; write_c[0][6] = 0; write_c[0][7] = 0; write_c[0][8] = 0; write_c[0][9] = 0; 
    write_c[1][0] = 0; write_c[1][1] = 0; write_c[1][2] = 1; write_c[1][3] = 1; write_c[1][4] = 1; write_c[1][5] = 1; write_c[1][6] = 1; write_c[1][7] = 1; write_c[1][8] = 0; write_c[1][9] = 0; 
    write_c[2][0] = 0; write_c[2][1] = 0; write_c[2][2] = 1; write_c[2][3] = 0; write_c[2][4] = 0; write_c[2][5] = 0; write_c[2][6] = 0; write_c[2][7] = 0; write_c[2][8] = 0; write_c[2][9] = 0; 
    write_c[3][0] = 0; write_c[3][1] = 0; write_c[3][2] = 1; write_c[3][3] = 0; write_c[3][4] = 0; write_c[3][5] = 0; write_c[3][6] = 0; write_c[3][7] = 0; write_c[3][8] = 0; write_c[3][9] = 0; 
    write_c[4][0] = 0; write_c[4][1] = 0; write_c[4][2] = 1; write_c[4][3] = 0; write_c[4][4] = 0; write_c[4][5] = 0; write_c[4][6] = 0; write_c[4][7] = 0; write_c[4][8] = 0; write_c[4][9] = 0; 
    write_c[5][0] = 0; write_c[5][1] = 0; write_c[5][2] = 1; write_c[5][3] = 0; write_c[5][4] = 0; write_c[5][5] = 0; write_c[5][6] = 0; write_c[5][7] = 0; write_c[5][8] = 0; write_c[5][9] = 0; 
    write_c[6][0] = 0; write_c[6][1] = 0; write_c[6][2] = 1; write_c[6][3] = 0; write_c[6][4] = 0; write_c[6][5] = 0; write_c[6][6] = 0; write_c[6][7] = 0; write_c[6][8] = 0; write_c[6][9] = 0; 
    write_c[7][0] = 0; write_c[7][1] = 0; write_c[7][2] = 1; write_c[7][3] = 0; write_c[7][4] = 0; write_c[7][5] = 0; write_c[7][6] = 0; write_c[7][7] = 0; write_c[7][8] = 0; write_c[7][9] = 0; 
    write_c[8][0] = 0; write_c[8][1] = 0; write_c[8][2] = 1; write_c[8][3] = 0; write_c[8][4] = 0; write_c[8][5] = 0; write_c[8][6] = 0; write_c[8][7] = 0; write_c[8][8] = 0; write_c[8][9] = 0; 
    write_c[9][0] = 0; write_c[9][1] = 0; write_c[9][2] = 1; write_c[9][3] = 1; write_c[9][4] = 1; write_c[9][5] = 1; write_c[9][6] = 1; write_c[9][7] = 1; write_c[9][8] = 0; write_c[9][9] = 0; 
    /*スコアの「Ｏ」を描画する情報*/
    static hword write_o[10][10];
    write_o[0][0] = 0; write_o[0][1] = 0; write_o[0][2] = 0; write_o[0][3] = 0; write_o[0][4] = 0; write_o[0][5] = 0; write_o[0][6] = 0; write_o[0][7] = 0; write_o[0][8] = 0; write_o[0][9] = 0; 
    write_o[1][0] = 0; write_o[1][1] = 0; write_o[1][2] = 1; write_o[1][3] = 1; write_o[1][4] = 1; write_o[1][5] = 1; write_o[1][6] = 1; write_o[1][7] = 1; write_o[1][8] = 0; write_o[1][9] = 0; 
    write_o[2][0] = 0; write_o[2][1] = 0; write_o[2][2] = 1; write_o[2][3] = 0; write_o[2][4] = 0; write_o[2][5] = 0; write_o[2][6] = 0; write_o[2][7] = 1; write_o[2][8] = 0; write_o[2][9] = 0; 
    write_o[3][0] = 0; write_o[3][1] = 0; write_o[3][2] = 1; write_o[3][3] = 0; write_o[3][4] = 0; write_o[3][5] = 0; write_o[3][6] = 0; write_o[3][7] = 1; write_o[3][8] = 0; write_o[3][9] = 0; 
    write_o[4][0] = 0; write_o[4][1] = 0; write_o[4][2] = 1; write_o[4][3] = 0; write_o[4][4] = 0; write_o[4][5] = 0; write_o[4][6] = 0; write_o[4][7] = 1; write_o[4][8] = 0; write_o[4][9] = 0; 
    write_o[5][0] = 0; write_o[5][1] = 0; write_o[5][2] = 1; write_o[5][3] = 0; write_o[5][4] = 0; write_o[5][5] = 0; write_o[5][6] = 0; write_o[5][7] = 1; write_o[5][8] = 0; write_o[5][9] = 0; 
    write_o[6][0] = 0; write_o[6][1] = 0; write_o[6][2] = 1; write_o[6][3] = 0; write_o[6][4] = 0; write_o[6][5] = 0; write_o[6][6] = 0; write_o[6][7] = 1; write_o[6][8] = 0; write_o[6][9] = 0; 
    write_o[7][0] = 0; write_o[7][1] = 0; write_o[7][2] = 1; write_o[7][3] = 0; write_o[7][4] = 0; write_o[7][5] = 0; write_o[7][6] = 0; write_o[7][7] = 1; write_o[7][8] = 0; write_o[7][9] = 0; 
    write_o[8][0] = 0; write_o[8][1] = 0; write_o[8][2] = 1; write_o[8][3] = 0; write_o[8][4] = 0; write_o[8][5] = 0; write_o[8][6] = 0; write_o[8][7] = 1; write_o[8][8] = 0; write_o[8][9] = 0; 
    write_o[9][0] = 0; write_o[9][1] = 0; write_o[9][2] = 1; write_o[9][3] = 1; write_o[9][4] = 1; write_o[9][5] = 1; write_o[9][6] = 1; write_o[9][7] = 1; write_o[9][8] = 0; write_o[9][9] = 0; 
    /*スコアの「Ｒ」を描画する情報*/
    static hword write_r[10][10];
    write_r[0][0] = 0; write_r[0][1] = 0; write_r[0][2] = 0; write_r[0][3] = 0; write_r[0][4] = 0; write_r[0][5] = 0; write_r[0][6] = 0; write_r[0][7] = 0; write_r[0][8] = 0; write_r[0][9] = 0; 
    write_r[1][0] = 0; write_r[1][1] = 0; write_r[1][2] = 1; write_r[1][3] = 1; write_r[1][4] = 1; write_r[1][5] = 1; write_r[1][6] = 1; write_r[1][7] = 0; write_r[1][8] = 0; write_r[1][9] = 0; 
    write_r[2][0] = 0; write_r[2][1] = 0; write_r[2][2] = 1; write_r[2][3] = 0; write_r[2][4] = 0; write_r[2][5] = 0; write_r[2][6] = 1; write_r[2][7] = 0; write_r[2][8] = 0; write_r[2][9] = 0; 
    write_r[3][0] = 0; write_r[3][1] = 0; write_r[3][2] = 1; write_r[3][3] = 0; write_r[3][4] = 0; write_r[3][5] = 0; write_r[3][6] = 1; write_r[3][7] = 0; write_r[3][8] = 0; write_r[3][9] = 0; 
    write_r[4][0] = 0; write_r[4][1] = 0; write_r[4][2] = 1; write_r[4][3] = 0; write_r[4][4] = 0; write_r[4][5] = 0; write_r[4][6] = 1; write_r[4][7] = 0; write_r[4][8] = 0; write_r[4][9] = 0; 
    write_r[5][0] = 0; write_r[5][1] = 0; write_r[5][2] = 1; write_r[5][3] = 1; write_r[5][4] = 1; write_r[5][5] = 1; write_r[5][6] = 1; write_r[5][7] = 0; write_r[5][8] = 0; write_r[5][9] = 0; 
    write_r[6][0] = 0; write_r[6][1] = 0; write_r[6][2] = 1; write_r[6][3] = 1; write_r[6][4] = 1; write_r[6][5] = 0; write_r[6][6] = 0; write_r[6][7] = 0; write_r[6][8] = 0; write_r[6][9] = 0; 
    write_r[7][0] = 0; write_r[7][1] = 0; write_r[7][2] = 1; write_r[7][3] = 0; write_r[7][4] = 1; write_r[7][5] = 1; write_r[7][6] = 0; write_r[7][7] = 0; write_r[7][8] = 0; write_r[7][9] = 0; 
    write_r[8][0] = 0; write_r[8][1] = 0; write_r[8][2] = 1; write_r[8][3] = 0; write_r[8][4] = 0; write_r[8][5] = 1; write_r[8][6] = 1; write_r[8][7] = 0; write_r[8][8] = 0; write_r[8][9] = 0; 
    write_r[9][0] = 0; write_r[9][1] = 0; write_r[9][2] = 1; write_r[9][3] = 0; write_r[9][4] = 0; write_r[9][5] = 0; write_r[9][6] = 1; write_r[9][7] = 0; write_r[9][8] = 0; write_r[9][9] = 0; 
    /*スコアの「Ｅ」を描画する情報*/
    static hword write_e[10][10];
    write_e[0][0] = 0; write_e[0][1] = 0; write_e[0][2] = 0; write_e[0][3] = 0; write_e[0][4] = 0; write_e[0][5] = 0; write_e[0][6] = 0; write_e[0][7] = 0; write_e[0][8] = 0; write_e[0][9] = 0; 
    write_e[1][0] = 0; write_e[1][1] = 0; write_e[1][2] = 1; write_e[1][3] = 1; write_e[1][4] = 1; write_e[1][5] = 1; write_e[1][6] = 1; write_e[1][7] = 1; write_e[1][8] = 0; write_e[1][9] = 0; 
    write_e[2][0] = 0; write_e[2][1] = 0; write_e[2][2] = 1; write_e[2][3] = 0; write_e[2][4] = 0; write_e[2][5] = 0; write_e[2][6] = 0; write_e[2][7] = 0; write_e[2][8] = 0; write_e[2][9] = 0; 
    write_e[3][0] = 0; write_e[3][1] = 0; write_e[3][2] = 1; write_e[3][3] = 0; write_e[3][4] = 0; write_e[3][5] = 0; write_e[3][6] = 0; write_e[3][7] = 0; write_e[3][8] = 0; write_e[3][9] = 0; 
    write_e[4][0] = 0; write_e[4][1] = 0; write_e[4][2] = 1; write_e[4][3] = 0; write_e[4][4] = 0; write_e[4][5] = 0; write_e[4][6] = 0; write_e[4][7] = 0; write_e[4][8] = 0; write_e[4][9] = 0; 
    write_e[5][0] = 0; write_e[5][1] = 0; write_e[5][2] = 1; write_e[5][3] = 1; write_e[5][4] = 1; write_e[5][5] = 1; write_e[5][6] = 1; write_e[5][7] = 1; write_e[5][8] = 0; write_e[5][9] = 0; 
    write_e[6][0] = 0; write_e[6][1] = 0; write_e[6][2] = 1; write_e[6][3] = 0; write_e[6][4] = 0; write_e[6][5] = 0; write_e[6][6] = 0; write_e[6][7] = 0; write_e[6][8] = 0; write_e[6][9] = 0; 
    write_e[7][0] = 0; write_e[7][1] = 0; write_e[7][2] = 1; write_e[7][3] = 0; write_e[7][4] = 0; write_e[7][5] = 0; write_e[7][6] = 0; write_e[7][7] = 0; write_e[7][8] = 0; write_e[7][9] = 0; 
    write_e[8][0] = 0; write_e[8][1] = 0; write_e[8][2] = 1; write_e[8][3] = 0; write_e[8][4] = 0; write_e[8][5] = 0; write_e[8][6] = 0; write_e[8][7] = 0; write_e[8][8] = 0; write_e[8][9] = 0; 
    write_e[9][0] = 0; write_e[9][1] = 0; write_e[9][2] = 1; write_e[9][3] = 1; write_e[9][4] = 1; write_e[9][5] = 1; write_e[9][6] = 1; write_e[9][7] = 1; write_e[9][8] = 0; write_e[9][9] = 0; 
    /*スコアの数字を描画する情報*/
    static hword write_num[10][10][10];
    /*0*/
    write_num[0][0][0] = 0; write_num[0][0][1] = 0; write_num[0][0][2] = 0; write_num[0][0][3] = 0; write_num[0][0][4] = 0; write_num[0][0][5] = 0; write_num[0][0][6] = 0; write_num[0][0][7] = 0; 
    write_num[0][0][8] = 0; write_num[0][0][9] = 0; 
    write_num[0][1][0] = 0; write_num[0][1][1] = 0; write_num[0][1][2] = 1; write_num[0][1][3] = 1; write_num[0][1][4] = 1; write_num[0][1][5] = 1; write_num[0][1][6] = 1; write_num[0][1][7] = 1; 
    write_num[0][1][8] = 0; write_num[0][1][9] = 0; 
    write_num[0][2][0] = 0; write_num[0][2][1] = 0; write_num[0][2][2] = 1; write_num[0][2][3] = 0; write_num[0][2][4] = 0; write_num[0][2][5] = 0; write_num[0][2][6] = 0; write_num[0][2][7] = 1; 
    write_num[0][2][8] = 0; write_num[0][2][9] = 0; 
    write_num[0][3][0] = 0; write_num[0][3][1] = 0; write_num[0][3][2] = 1; write_num[0][3][3] = 0; write_num[0][3][4] = 0; write_num[0][3][5] = 0; write_num[0][3][6] = 0; write_num[0][3][7] = 1; 
    write_num[0][3][8] = 0; write_num[0][3][9] = 0; 
    write_num[0][4][0] = 0; write_num[0][4][1] = 0; write_num[0][4][2] = 1; write_num[0][4][3] = 0; write_num[0][4][4] = 0; write_num[0][4][5] = 0; write_num[0][4][6] = 0; write_num[0][4][7] = 1; 
    write_num[0][4][8] = 0; write_num[0][4][9] = 0; 
    write_num[0][5][0] = 0; write_num[0][5][1] = 0; write_num[0][5][2] = 1; write_num[0][5][3] = 0; write_num[0][5][4] = 0; write_num[0][5][5] = 0; write_num[0][5][6] = 0; write_num[0][5][7] = 1; 
    write_num[0][5][8] = 0; write_num[0][5][9] = 0; 
    write_num[0][6][0] = 0; write_num[0][6][1] = 0; write_num[0][6][2] = 1; write_num[0][6][3] = 0; write_num[0][6][4] = 0; write_num[0][6][5] = 0; write_num[0][6][6] = 0; write_num[0][6][7] = 1; 
    write_num[0][6][8] = 0; write_num[0][6][9] = 0; 
    write_num[0][7][0] = 0; write_num[0][7][1] = 0; write_num[0][7][2] = 1; write_num[0][7][3] = 0; write_num[0][7][4] = 0; write_num[0][7][5] = 0; write_num[0][7][6] = 0; write_num[0][7][7] = 1; 
    write_num[0][7][8] = 0; write_num[0][7][9] = 0; 
    write_num[0][8][0] = 0; write_num[0][8][1] = 0; write_num[0][8][2] = 1; write_num[0][8][3] = 0; write_num[0][8][4] = 0; write_num[0][8][5] = 0; write_num[0][8][6] = 0; write_num[0][8][7] = 1; 
    write_num[0][8][8] = 0; write_num[0][8][9] = 0; 
    write_num[0][9][0] = 0; write_num[0][9][1] = 0; write_num[0][9][2] = 1; write_num[0][9][3] = 1; write_num[0][9][4] = 1; write_num[0][9][5] = 1; write_num[0][9][6] = 1; write_num[0][9][7] = 1; 
    write_num[0][9][8] = 0; write_num[0][9][9] = 0; 
    /*1*/
    write_num[1][0][0] = 0; write_num[1][0][1] = 0; write_num[1][0][2] = 0; write_num[1][0][3] = 0; write_num[1][0][4] = 0; write_num[1][0][5] = 0; write_num[1][0][6] = 0; write_num[1][0][7] = 0; 
    write_num[1][0][8] = 0; write_num[1][0][9] = 0; 
    write_num[1][1][0] = 0; write_num[1][1][1] = 0; write_num[1][1][2] = 0; write_num[1][1][3] = 0; write_num[1][1][4] = 1; write_num[1][1][5] = 1; write_num[1][1][6] = 0; write_num[1][1][7] = 0; 
    write_num[1][1][8] = 0; write_num[1][1][9] = 0; 
    write_num[1][2][0] = 0; write_num[1][2][1] = 0; write_num[1][2][2] = 0; write_num[1][2][3] = 0; write_num[1][2][4] = 1; write_num[1][2][5] = 1; write_num[1][2][6] = 0; write_num[1][2][7] = 0; 
    write_num[1][2][8] = 0; write_num[1][2][9] = 0; 
    write_num[1][3][0] = 0; write_num[1][3][1] = 0; write_num[1][3][2] = 0; write_num[1][3][3] = 0; write_num[1][3][4] = 1; write_num[1][3][5] = 1; write_num[1][3][6] = 0; write_num[1][3][7] = 0; 
    write_num[1][3][8] = 0; write_num[1][3][9] = 0; 
    write_num[1][4][0] = 0; write_num[1][4][1] = 0; write_num[1][4][2] = 0; write_num[1][4][3] = 0; write_num[1][4][4] = 1; write_num[1][4][5] = 1; write_num[1][4][6] = 0; write_num[1][4][7] = 0; 
    write_num[1][4][8] = 0; write_num[1][4][9] = 0; 
    write_num[1][5][0] = 0; write_num[1][5][1] = 0; write_num[1][5][2] = 0; write_num[1][5][3] = 0; write_num[1][5][4] = 1; write_num[1][5][5] = 1; write_num[1][5][6] = 0; write_num[1][5][7] = 0; 
    write_num[1][5][8] = 0; write_num[1][5][9] = 0; 
    write_num[1][6][0] = 0; write_num[1][6][1] = 0; write_num[1][6][2] = 0; write_num[1][6][3] = 0; write_num[1][6][4] = 1; write_num[1][6][5] = 1; write_num[1][6][6] = 0; write_num[1][6][7] = 0; 
    write_num[1][6][8] = 0; write_num[1][6][9] = 0; 
    write_num[1][7][0] = 0; write_num[1][7][1] = 0; write_num[1][7][2] = 0; write_num[1][7][3] = 0; write_num[1][7][4] = 1; write_num[1][7][5] = 1; write_num[1][7][6] = 0; write_num[1][7][7] = 0; 
    write_num[1][7][8] = 0; write_num[1][7][9] = 0; 
    write_num[1][8][0] = 0; write_num[1][8][1] = 0; write_num[1][8][2] = 0; write_num[1][8][3] = 0; write_num[1][8][4] = 1; write_num[1][8][5] = 1; write_num[1][8][6] = 0; write_num[1][8][7] = 0; 
    write_num[1][8][8] = 0; write_num[1][8][9] = 0; 
    write_num[1][9][0] = 0; write_num[1][9][1] = 0; write_num[1][9][2] = 0; write_num[1][9][3] = 0; write_num[1][9][4] = 1; write_num[1][9][5] = 1; write_num[1][9][6] = 0; write_num[1][9][7] = 0; 
    write_num[1][9][8] = 0; write_num[1][9][9] = 0; 
    /*2*/
    write_num[2][0][0] = 0; write_num[2][0][1] = 0; write_num[2][0][2] = 0; write_num[2][0][3] = 0; write_num[2][0][4] = 0; write_num[2][0][5] = 0; write_num[2][0][6] = 0; write_num[2][0][7] = 0; 
    write_num[2][0][8] = 0; write_num[2][0][9] = 0; 
    write_num[2][1][0] = 0; write_num[2][1][1] = 0; write_num[2][1][2] = 1; write_num[2][1][3] = 1; write_num[2][1][4] = 1; write_num[2][1][5] = 1; write_num[2][1][6] = 1; write_num[2][1][7] = 1; 
    write_num[2][1][8] = 0; write_num[2][1][9] = 0; 
    write_num[2][2][0] = 0; write_num[2][2][1] = 0; write_num[2][2][2] = 0; write_num[2][2][3] = 0; write_num[2][2][4] = 0; write_num[2][2][5] = 0; write_num[2][2][6] = 0; write_num[2][2][7] = 1; 
    write_num[2][2][8] = 0; write_num[2][2][9] = 0; 
    write_num[2][3][0] = 0; write_num[2][3][1] = 0; write_num[2][3][2] = 0; write_num[2][3][3] = 0; write_num[2][3][4] = 0; write_num[2][3][5] = 0; write_num[2][3][6] = 0; write_num[2][3][7] = 1; 
    write_num[2][3][8] = 0; write_num[2][3][9] = 0; 
    write_num[2][4][0] = 0; write_num[2][4][1] = 0; write_num[2][4][2] = 0; write_num[2][4][3] = 0; write_num[2][4][4] = 0; write_num[2][4][5] = 0; write_num[2][4][6] = 0; write_num[2][4][7] = 1; 
    write_num[2][4][8] = 0; write_num[2][4][9] = 0; 
    write_num[2][5][0] = 0; write_num[2][5][1] = 0; write_num[2][5][2] = 1; write_num[2][5][3] = 1; write_num[2][5][4] = 1; write_num[2][5][5] = 1; write_num[2][5][6] = 1; write_num[2][5][7] = 1; 
    write_num[2][5][8] = 0; write_num[2][5][9] = 0; 
    write_num[2][6][0] = 0; write_num[2][6][1] = 0; write_num[2][6][2] = 1; write_num[2][6][3] = 0; write_num[2][6][4] = 0; write_num[2][6][5] = 0; write_num[2][6][6] = 0; write_num[2][6][7] = 0; 
    write_num[2][6][8] = 0; write_num[2][6][9] = 0; 
    write_num[2][7][0] = 0; write_num[2][7][1] = 0; write_num[2][7][2] = 1; write_num[2][7][3] = 0; write_num[2][7][4] = 0; write_num[2][7][5] = 0; write_num[2][7][6] = 0; write_num[2][7][7] = 0; 
    write_num[2][7][8] = 0; write_num[2][7][9] = 0; 
    write_num[2][8][0] = 0; write_num[2][8][1] = 0; write_num[2][8][2] = 1; write_num[2][8][3] = 0; write_num[2][8][4] = 0; write_num[2][8][5] = 0; write_num[2][8][6] = 0; write_num[2][8][7] = 0; 
    write_num[2][8][8] = 0; write_num[2][8][9] = 0; 
    write_num[2][9][0] = 0; write_num[2][9][1] = 0; write_num[2][9][2] = 1; write_num[2][9][3] = 1; write_num[2][9][4] = 1; write_num[2][9][5] = 1; write_num[2][9][6] = 1; write_num[2][9][7] = 1; 
    write_num[2][9][8] = 0; write_num[2][9][9] = 0; 
    /*3*/
    write_num[3][0][0] = 0; write_num[3][0][1] = 0; write_num[3][0][2] = 0; write_num[3][0][3] = 0; write_num[3][0][4] = 0; write_num[3][0][5] = 0; write_num[3][0][6] = 0; write_num[3][0][7] = 0; 
    write_num[3][0][8] = 0; write_num[3][0][9] = 0; 
    write_num[3][1][0] = 0; write_num[3][1][1] = 0; write_num[3][1][2] = 1; write_num[3][1][3] = 1; write_num[3][1][4] = 1; write_num[3][1][5] = 1; write_num[3][1][6] = 1; write_num[3][1][7] = 1; 
    write_num[3][1][8] = 0; write_num[3][1][9] = 0; 
    write_num[3][2][0] = 0; write_num[3][2][1] = 0; write_num[3][2][2] = 0; write_num[3][2][3] = 0; write_num[3][2][4] = 0; write_num[3][2][5] = 0; write_num[3][2][6] = 0; write_num[3][2][7] = 1; 
    write_num[3][2][8] = 0; write_num[3][2][9] = 0; 
    write_num[3][3][0] = 0; write_num[3][3][1] = 0; write_num[3][3][2] = 0; write_num[3][3][3] = 0; write_num[3][3][4] = 0; write_num[3][3][5] = 0; write_num[3][3][6] = 0; write_num[3][3][7] = 1; 
    write_num[3][3][8] = 0; write_num[3][3][9] = 0; 
    write_num[3][4][0] = 0; write_num[3][4][1] = 0; write_num[3][4][2] = 0; write_num[3][4][3] = 0; write_num[3][4][4] = 0; write_num[3][4][5] = 0; write_num[3][4][6] = 0; write_num[3][4][7] = 1; 
    write_num[3][4][8] = 0; write_num[3][4][9] = 0; 
    write_num[3][5][0] = 0; write_num[3][5][1] = 0; write_num[3][5][2] = 1; write_num[3][5][3] = 1; write_num[3][5][4] = 1; write_num[3][5][5] = 1; write_num[3][5][6] = 1; write_num[3][5][7] = 1; 
    write_num[3][5][8] = 0; write_num[3][5][9] = 0; 
    write_num[3][6][0] = 0; write_num[3][6][1] = 0; write_num[3][6][2] = 0; write_num[3][6][3] = 0; write_num[3][6][4] = 0; write_num[3][6][5] = 0; write_num[3][6][6] = 0; write_num[3][6][7] = 1; 
    write_num[3][6][8] = 0; write_num[3][6][9] = 0; 
    write_num[3][7][0] = 0; write_num[3][7][1] = 0; write_num[3][7][2] = 0; write_num[3][7][3] = 0; write_num[3][7][4] = 0; write_num[3][7][5] = 0; write_num[3][7][6] = 0; write_num[3][7][7] = 1; 
    write_num[3][7][8] = 0; write_num[3][7][9] = 0; 
    write_num[3][8][0] = 0; write_num[3][8][1] = 0; write_num[3][8][2] = 0; write_num[3][8][3] = 0; write_num[3][8][4] = 0; write_num[3][8][5] = 0; write_num[3][8][6] = 0; write_num[3][8][7] = 1; 
    write_num[3][8][8] = 0; write_num[3][8][9] = 0; 
    write_num[3][9][0] = 0; write_num[3][9][1] = 0; write_num[3][9][2] = 1; write_num[3][9][3] = 1; write_num[3][9][4] = 1; write_num[3][9][5] = 1; write_num[3][9][6] = 1; write_num[3][9][7] = 1; 
    write_num[3][9][8] = 0; write_num[3][9][9] = 0; 
    /*4*/
    write_num[4][0][0] = 0; write_num[4][0][1] = 0; write_num[4][0][2] = 0; write_num[4][0][3] = 0; write_num[4][0][4] = 0; write_num[4][0][5] = 0; write_num[4][0][6] = 0; write_num[4][0][7] = 0; 
    write_num[4][0][8] = 0; write_num[4][0][9] = 0; 
    write_num[4][1][0] = 0; write_num[4][1][1] = 0; write_num[4][1][2] = 1; write_num[4][1][3] = 0; write_num[4][1][4] = 0; write_num[4][1][5] = 0; write_num[4][1][6] = 0; write_num[4][1][7] = 0; 
    write_num[4][1][8] = 0; write_num[4][1][9] = 0; 
    write_num[4][2][0] = 0; write_num[4][2][1] = 0; write_num[4][2][2] = 1; write_num[4][2][3] = 0; write_num[4][2][4] = 0; write_num[4][2][5] = 0; write_num[4][2][6] = 0; write_num[4][2][7] = 0; 
    write_num[4][2][8] = 0; write_num[4][2][9] = 0; 
    write_num[4][3][0] = 0; write_num[4][3][1] = 0; write_num[4][3][2] = 1; write_num[4][3][3] = 0; write_num[4][3][4] = 1; write_num[4][3][5] = 0; write_num[4][3][6] = 0; write_num[4][3][7] = 0; 
    write_num[4][3][8] = 0; write_num[4][3][9] = 0; 
    write_num[4][4][0] = 0; write_num[4][4][1] = 0; write_num[4][4][2] = 1; write_num[4][4][3] = 0; write_num[4][4][4] = 1; write_num[4][4][5] = 0; write_num[4][4][6] = 0; write_num[4][4][7] = 0; 
    write_num[4][4][8] = 0; write_num[4][4][9] = 0; 
    write_num[4][5][0] = 0; write_num[4][5][1] = 0; write_num[4][5][2] = 1; write_num[4][5][3] = 1; write_num[4][5][4] = 1; write_num[4][5][5] = 1; write_num[4][5][6] = 1; write_num[4][5][7] = 1; 
    write_num[4][5][8] = 0; write_num[4][5][9] = 0; 
    write_num[4][6][0] = 0; write_num[4][6][1] = 0; write_num[4][6][2] = 0; write_num[4][6][3] = 0; write_num[4][6][4] = 1; write_num[4][6][5] = 0; write_num[4][6][6] = 0; write_num[4][6][7] = 0; 
    write_num[4][6][8] = 0; write_num[4][6][9] = 0; 
    write_num[4][7][0] = 0; write_num[4][7][1] = 0; write_num[4][7][2] = 0; write_num[4][7][3] = 0; write_num[4][7][4] = 1; write_num[4][7][5] = 0; write_num[4][7][6] = 0; write_num[4][7][7] = 0; 
    write_num[4][7][8] = 0; write_num[4][7][9] = 0; 
    write_num[4][8][0] = 0; write_num[4][8][1] = 0; write_num[4][8][2] = 0; write_num[4][8][3] = 0; write_num[4][8][4] = 1; write_num[4][8][5] = 0; write_num[4][8][6] = 0; write_num[4][8][7] = 0; 
    write_num[4][8][8] = 0; write_num[4][8][9] = 0; 
    write_num[4][9][0] = 0; write_num[4][9][1] = 0; write_num[4][9][2] = 0; write_num[4][9][3] = 0; write_num[4][9][4] = 1; write_num[4][9][5] = 0; write_num[4][9][6] = 0; write_num[4][9][7] = 0; 
    write_num[4][9][8] = 0; write_num[4][9][9] = 0; 
    /*5*/
    write_num[5][0][0] = 0; write_num[5][0][1] = 0; write_num[5][0][2] = 0; write_num[5][0][3] = 0; write_num[5][0][4] = 0; write_num[5][0][5] = 0; write_num[5][0][6] = 0; write_num[5][0][7] = 0; 
    write_num[5][0][8] = 0; write_num[5][0][9] = 0; 
    write_num[5][1][0] = 0; write_num[5][1][1] = 1; write_num[5][1][2] = 1; write_num[5][1][3] = 1; write_num[5][1][4] = 1; write_num[5][1][5] = 1; write_num[5][1][6] = 1; write_num[5][1][7] = 1; 
    write_num[5][1][8] = 0; write_num[5][1][9] = 0; 
    write_num[5][2][0] = 0; write_num[5][2][1] = 1; write_num[5][2][2] = 0; write_num[5][2][3] = 0; write_num[5][2][4] = 0; write_num[5][2][5] = 0; write_num[5][2][6] = 0; write_num[5][2][7] = 0; 
    write_num[5][2][8] = 0; write_num[5][2][9] = 0; 
    write_num[5][3][0] = 0; write_num[5][3][1] = 1; write_num[5][3][2] = 0; write_num[5][3][3] = 0; write_num[5][3][4] = 0; write_num[5][3][5] = 0; write_num[5][3][6] = 0; write_num[5][3][7] = 0; 
    write_num[5][3][8] = 0; write_num[5][3][9] = 0; 
    write_num[5][4][0] = 0; write_num[5][4][1] = 1; write_num[5][4][2] = 0; write_num[5][4][3] = 0; write_num[5][4][4] = 0; write_num[5][4][5] = 0; write_num[5][4][6] = 0; write_num[5][4][7] = 0; 
    write_num[5][4][8] = 0; write_num[5][4][9] = 0; 
    write_num[5][5][0] = 0; write_num[5][5][1] = 1; write_num[5][5][2] = 1; write_num[5][5][3] = 1; write_num[5][5][4] = 1; write_num[5][5][5] = 1; write_num[5][5][6] = 1; write_num[5][5][7] = 1; 
    write_num[5][5][8] = 0; write_num[5][5][9] = 0; 
    write_num[5][6][0] = 0; write_num[5][6][1] = 0; write_num[5][6][2] = 0; write_num[5][6][3] = 0; write_num[5][6][4] = 0; write_num[5][6][5] = 0; write_num[5][6][6] = 0; write_num[5][6][7] = 1; 
    write_num[5][6][8] = 0; write_num[5][6][9] = 0; 
    write_num[5][7][0] = 0; write_num[5][7][1] = 0; write_num[5][7][2] = 0; write_num[5][7][3] = 0; write_num[5][7][4] = 0; write_num[5][7][5] = 0; write_num[5][7][6] = 0; write_num[5][7][7] = 1; 
    write_num[5][7][8] = 0; write_num[5][7][9] = 0; 
    write_num[5][8][0] = 0; write_num[5][8][1] = 0; write_num[5][8][2] = 0; write_num[5][8][3] = 0; write_num[5][8][4] = 0; write_num[5][8][5] = 0; write_num[5][8][6] = 0; write_num[5][8][7] = 1; 
    write_num[5][8][8] = 0; write_num[5][8][9] = 0; 
    write_num[5][9][0] = 0; write_num[5][9][1] = 1; write_num[5][9][2] = 1; write_num[5][9][3] = 1; write_num[5][9][4] = 1; write_num[5][9][5] = 1; write_num[5][9][6] = 1; write_num[5][9][7] = 1; 
    write_num[5][9][8] = 0; write_num[5][9][9] = 0; 
    /*6*/
    write_num[6][0][0] = 0; write_num[6][0][1] = 0; write_num[6][0][2] = 0; write_num[6][0][3] = 0; write_num[6][0][4] = 0; write_num[6][0][5] = 0; write_num[6][0][6] = 0; write_num[6][0][7] = 0; 
    write_num[6][0][8] = 0; write_num[6][0][9] = 0; 
    write_num[6][1][0] = 0; write_num[6][1][1] = 0; write_num[6][1][2] = 1; write_num[6][1][3] = 1; write_num[6][1][4] = 1; write_num[6][1][5] = 1; write_num[6][1][6] = 1; write_num[6][1][7] = 1; 
    write_num[6][1][8] = 0; write_num[6][1][9] = 0; 
    write_num[6][2][0] = 0; write_num[6][2][1] = 0; write_num[6][2][2] = 1; write_num[6][2][3] = 0; write_num[6][2][4] = 0; write_num[6][2][5] = 0; write_num[6][2][6] = 0; write_num[6][2][7] = 0; 
    write_num[6][2][8] = 0; write_num[6][2][9] = 0; 
    write_num[6][3][0] = 0; write_num[6][3][1] = 0; write_num[6][3][2] = 1; write_num[6][3][3] = 0; write_num[6][3][4] = 0; write_num[6][3][5] = 0; write_num[6][3][6] = 0; write_num[6][3][7] = 0; 
    write_num[6][3][8] = 0; write_num[6][3][9] = 0; 
    write_num[6][4][0] = 0; write_num[6][4][1] = 0; write_num[6][4][2] = 1; write_num[6][4][3] = 0; write_num[6][4][4] = 0; write_num[6][4][5] = 0; write_num[6][4][6] = 0; write_num[6][4][7] = 0; 
    write_num[6][4][8] = 0; write_num[6][4][9] = 0; 
    write_num[6][5][0] = 0; write_num[6][5][1] = 0; write_num[6][5][2] = 1; write_num[6][5][3] = 1; write_num[6][5][4] = 1; write_num[6][5][5] = 1; write_num[6][5][6] = 1; write_num[6][5][7] = 1; 
    write_num[6][5][8] = 0; write_num[6][5][9] = 0; 
    write_num[6][6][0] = 0; write_num[6][6][1] = 0; write_num[6][6][2] = 1; write_num[6][6][3] = 0; write_num[6][6][4] = 0; write_num[6][6][5] = 0; write_num[6][6][6] = 0; write_num[6][6][7] = 1; 
    write_num[6][6][8] = 0; write_num[6][6][9] = 0; 
    write_num[6][7][0] = 0; write_num[6][7][1] = 0; write_num[6][7][2] = 1; write_num[6][7][3] = 0; write_num[6][7][4] = 0; write_num[6][7][5] = 0; write_num[6][7][6] = 0; write_num[6][7][7] = 1; 
    write_num[6][7][8] = 0; write_num[6][7][9] = 0; 
    write_num[6][8][0] = 0; write_num[6][8][1] = 0; write_num[6][8][2] = 1; write_num[6][8][3] = 0; write_num[6][8][4] = 0; write_num[6][8][5] = 0; write_num[6][8][6] = 0; write_num[6][8][7] = 1; 
    write_num[6][8][8] = 0; write_num[6][8][9] = 0; 
    write_num[6][9][0] = 0; write_num[6][9][1] = 0; write_num[6][9][2] = 1; write_num[6][9][3] = 1; write_num[6][9][4] = 1; write_num[6][9][5] = 1; write_num[6][9][6] = 1; write_num[6][9][7] = 1; 
    write_num[6][9][8] = 0; write_num[6][9][9] = 0; 
    /*7*/
    write_num[7][0][0] = 0; write_num[7][0][1] = 0; write_num[7][0][2] = 0; write_num[7][0][3] = 0; write_num[7][0][4] = 0; write_num[7][0][5] = 0; write_num[7][0][6] = 0; write_num[7][0][7] = 0; 
    write_num[7][0][8] = 0; write_num[7][0][9] = 0; 
    write_num[7][1][0] = 0; write_num[7][1][1] = 0; write_num[7][1][2] = 1; write_num[7][1][3] = 1; write_num[7][1][4] = 1; write_num[7][1][5] = 1; write_num[7][1][6] = 1; write_num[7][1][7] = 1; 
    write_num[7][1][8] = 0; write_num[7][1][9] = 0; 
    write_num[7][2][0] = 0; write_num[7][2][1] = 0; write_num[7][2][2] = 0; write_num[7][2][3] = 0; write_num[7][2][4] = 0; write_num[7][2][5] = 0; write_num[7][2][6] = 0; write_num[7][2][7] = 1; 
    write_num[7][2][8] = 0; write_num[7][2][9] = 0; 
    write_num[7][3][0] = 0; write_num[7][3][1] = 0; write_num[7][3][2] = 0; write_num[7][3][3] = 0; write_num[7][3][4] = 0; write_num[7][3][5] = 0; write_num[7][3][6] = 0; write_num[7][3][7] = 1; 
    write_num[7][3][8] = 0; write_num[7][3][9] = 0; 
    write_num[7][4][0] = 0; write_num[7][4][1] = 0; write_num[7][4][2] = 0; write_num[7][4][3] = 0; write_num[7][4][4] = 0; write_num[7][4][5] = 0; write_num[7][4][6] = 0; write_num[7][4][7] = 1; 
    write_num[7][4][8] = 0; write_num[7][4][9] = 0; 
    write_num[7][5][0] = 0; write_num[7][5][1] = 0; write_num[7][5][2] = 0; write_num[7][5][3] = 0; write_num[7][5][4] = 0; write_num[7][5][5] = 0; write_num[7][5][6] = 0; write_num[7][5][7] = 1; 
    write_num[7][5][8] = 0; write_num[7][5][9] = 0; 
    write_num[7][6][0] = 0; write_num[7][6][1] = 0; write_num[7][6][2] = 0; write_num[7][6][3] = 0; write_num[7][6][4] = 0; write_num[7][6][5] = 0; write_num[7][6][6] = 0; write_num[7][6][7] = 1; 
    write_num[7][6][8] = 0; write_num[7][6][9] = 0; 
    write_num[7][7][0] = 0; write_num[7][7][1] = 0; write_num[7][7][2] = 0; write_num[7][7][3] = 0; write_num[7][7][4] = 0; write_num[7][7][5] = 0; write_num[7][7][6] = 0; write_num[7][7][7] = 1; 
    write_num[7][7][8] = 0; write_num[7][7][9] = 0; 
    write_num[7][8][0] = 0; write_num[7][8][1] = 0; write_num[7][8][2] = 0; write_num[7][8][3] = 0; write_num[7][8][4] = 0; write_num[7][8][5] = 0; write_num[7][8][6] = 0; write_num[7][8][7] = 1; 
    write_num[7][8][8] = 0; write_num[7][8][9] = 0; 
    write_num[7][9][0] = 0; write_num[7][9][1] = 0; write_num[7][9][2] = 0; write_num[7][9][3] = 0; write_num[7][9][4] = 0; write_num[7][9][5] = 0; write_num[7][9][6] = 0; write_num[7][9][7] = 1; 
    write_num[7][9][8] = 0; write_num[7][9][9] = 0; 
    /*8*/
    write_num[8][0][0] = 0; write_num[8][0][1] = 0; write_num[8][0][2] = 0; write_num[8][0][3] = 0; write_num[8][0][4] = 0; write_num[8][0][5] = 0; write_num[8][0][6] = 0; write_num[8][0][7] = 0; 
    write_num[8][0][8] = 0; write_num[8][0][9] = 0; 
    write_num[8][1][0] = 0; write_num[8][1][1] = 0; write_num[8][1][2] = 1; write_num[8][1][3] = 1; write_num[8][1][4] = 1; write_num[8][1][5] = 1; write_num[8][1][6] = 1; write_num[8][1][7] = 1; 
    write_num[8][1][8] = 0; write_num[8][1][9] = 0; 
    write_num[8][2][0] = 0; write_num[8][2][1] = 0; write_num[8][2][2] = 1; write_num[8][2][3] = 0; write_num[8][2][4] = 0; write_num[8][2][5] = 0; write_num[8][2][6] = 0; write_num[8][2][7] = 1; 
    write_num[8][2][8] = 0; write_num[8][2][9] = 0; 
    write_num[8][3][0] = 0; write_num[8][3][1] = 0; write_num[8][3][2] = 1; write_num[8][3][3] = 0; write_num[8][3][4] = 0; write_num[8][3][5] = 0; write_num[8][3][6] = 0; write_num[8][3][7] = 1; 
    write_num[8][3][8] = 0; write_num[8][3][9] = 0; 
    write_num[8][4][0] = 0; write_num[8][4][1] = 0; write_num[8][4][2] = 1; write_num[8][4][3] = 0; write_num[8][4][4] = 0; write_num[8][4][5] = 0; write_num[8][4][6] = 0; write_num[8][4][7] = 1; 
    write_num[8][4][8] = 0; write_num[8][4][9] = 0; 
    write_num[8][5][0] = 0; write_num[8][5][1] = 0; write_num[8][5][2] = 1; write_num[8][5][3] = 1; write_num[8][5][4] = 1; write_num[8][5][5] = 1; write_num[8][5][6] = 1; write_num[8][5][7] = 1; 
    write_num[8][5][8] = 0; write_num[8][5][9] = 0; 
    write_num[8][6][0] = 0; write_num[8][6][1] = 0; write_num[8][6][2] = 1; write_num[8][6][3] = 0; write_num[8][6][4] = 0; write_num[8][6][5] = 0; write_num[8][6][6] = 0; write_num[8][6][7] = 1; 
    write_num[8][6][8] = 0; write_num[8][6][9] = 0; 
    write_num[8][7][0] = 0; write_num[8][7][1] = 0; write_num[8][7][2] = 1; write_num[8][7][3] = 0; write_num[8][7][4] = 0; write_num[8][7][5] = 0; write_num[8][7][6] = 0; write_num[8][7][7] = 1; 
    write_num[8][7][8] = 0; write_num[8][7][9] = 0; 
    write_num[8][8][0] = 0; write_num[8][8][1] = 0; write_num[8][8][2] = 1; write_num[8][8][3] = 0; write_num[8][8][4] = 0; write_num[8][8][5] = 0; write_num[8][8][6] = 0; write_num[8][8][7] = 1; 
    write_num[8][8][8] = 0; write_num[8][8][9] = 0; 
    write_num[8][9][0] = 0; write_num[8][9][1] = 0; write_num[8][9][2] = 1; write_num[8][9][3] = 1; write_num[8][9][4] = 1; write_num[8][9][5] = 1; write_num[8][9][6] = 1; write_num[8][9][7] = 1; 
    write_num[8][9][8] = 0; write_num[8][9][9] = 0; 
    /*9*/
    write_num[9][0][0] = 0; write_num[9][0][1] = 0; write_num[9][0][2] = 0; write_num[9][0][3] = 0; write_num[9][0][4] = 0; write_num[9][0][5] = 0; write_num[9][0][6] = 0; write_num[9][0][7] = 0; 
    write_num[9][0][8] = 0; write_num[9][0][9] = 0; 
    write_num[9][1][0] = 0; write_num[9][1][1] = 0; write_num[9][1][2] = 1; write_num[9][1][3] = 1; write_num[9][1][4] = 1; write_num[9][1][5] = 1; write_num[9][1][6] = 1; write_num[9][1][7] = 1; 
    write_num[9][1][8] = 0; write_num[9][1][9] = 0; 
    write_num[9][2][0] = 0; write_num[9][2][1] = 0; write_num[9][2][2] = 1; write_num[9][2][3] = 0; write_num[9][2][4] = 0; write_num[9][2][5] = 0; write_num[9][2][6] = 0; write_num[9][2][7] = 1; 
    write_num[9][2][8] = 0; write_num[9][2][9] = 0; 
    write_num[9][3][0] = 0; write_num[9][3][1] = 0; write_num[9][3][2] = 1; write_num[9][3][3] = 0; write_num[9][3][4] = 0; write_num[9][3][5] = 0; write_num[9][3][6] = 0; write_num[9][3][7] = 1; 
    write_num[9][3][8] = 0; write_num[9][3][9] = 0; 
    write_num[9][4][0] = 0; write_num[9][4][1] = 0; write_num[9][4][2] = 1; write_num[9][4][3] = 0; write_num[9][4][4] = 0; write_num[9][4][5] = 0; write_num[9][4][6] = 0; write_num[9][4][7] = 1; 
    write_num[9][4][8] = 0; write_num[9][4][9] = 0; 
    write_num[9][5][0] = 0; write_num[9][5][1] = 0; write_num[9][5][2] = 1; write_num[9][5][3] = 1; write_num[9][5][4] = 1; write_num[9][5][5] = 1; write_num[9][5][6] = 1; write_num[9][5][7] = 1; 
    write_num[9][5][8] = 0; write_num[9][5][9] = 0; 
    write_num[9][6][0] = 0; write_num[9][6][1] = 0; write_num[9][6][2] = 0; write_num[9][6][3] = 0; write_num[9][6][4] = 0; write_num[9][6][5] = 0; write_num[9][6][6] = 0; write_num[9][6][7] = 1; 
    write_num[9][6][8] = 0; write_num[9][6][9] = 0; 
    write_num[9][7][0] = 0; write_num[9][7][1] = 0; write_num[9][7][2] = 0; write_num[9][7][3] = 0; write_num[9][7][4] = 0; write_num[9][7][5] = 0; write_num[9][7][6] = 0; write_num[9][7][7] = 1; 
    write_num[9][7][8] = 0; write_num[9][7][9] = 0; 
    write_num[9][8][0] = 0; write_num[9][8][1] = 0; write_num[9][8][2] = 0; write_num[9][8][3] = 0; write_num[9][8][4] = 0; write_num[9][8][5] = 0; write_num[9][8][6] = 0; write_num[9][8][7] = 1; 
    write_num[9][8][8] = 0; write_num[9][8][9] = 0; 
    write_num[9][9][0] = 0; write_num[9][9][1] = 0; write_num[9][9][2] = 1; write_num[9][9][3] = 1; write_num[9][9][4] = 1; write_num[9][9][5] = 1; write_num[9][9][6] = 1; write_num[9][9][7] = 1; 
    write_num[9][9][8] = 0; write_num[9][9][9] = 0;

    /*全画面を黒色で塗りつぶす*/
    for(i=0;i<180;i++){
        for(j=0;j<240;j++){
            draw_point(j, i, black);
        }
    }
    /*「START」の入力を受けるとこのwhile文を抜け出す*/
    while(1){
        key = (hword*) 0x04000130;
        if(startkey==(startkey & *key)) {
        }
        else{
           break; 
        }
    }

    /*マップ表示*/
    for(i=0;i<18;i++){
        for(j=0; j<24; j++){
            draw_map(j*8+xstart_map, i*8+ystart_map, map[i][j]);
        } 
    }

    /*赤下向きゴースト表示*/
    draw_ghost(95+xstart_map, 126+ystart_map, red, down_ghost, 14);
    /*ピンク右向きゴースト表示*/
    draw_ghost(154+xstart_map, 37+ystart_map, pink, right_ghost, 14);
    /*水色左向きゴースト表示*/
    draw_ghost(36+xstart_map, 132+ystart_map, lightblue, left_ghost, 14);
    /*オレンジ色上向きゴースト表示*/
    draw_ghost(95+xstart_map, 55+ystart_map, orange, up_ghost, 14);
    /*マップ全体の色を1ドットごとに保存*/
    static hword pixel_map[144][192] ;
    make_pixel_map(pixel_map, 144);

    /*動線を作る*/
    static hword flow_line_map[144][192] ;
    for(i=0;i<140;i++){
        for(j=0;j<192;j++){
            flow_line_map[i][j]=0;
        }
    }
    make_flow_line(pixel_map,192,flow_line_map,192);

    /*exists_pacman_map配列に代入して一度パックマンの現在地の情報を０にする*/
    for(i=0;i<140;i++){
        for(j=0;j<192;j++){
            exists_pacman_map[i][j]=0;
        }
    }
    /*はじめに右向きパックマンをマップの左上に描画する*/
    right_draw_PacMan(exists_pacman_cie[0] + xstart_map, exists_pacman_cie[1] + ystart_map, 1, PacMan0, PacMan_len, PacMan1, 
    PacMan_len, PacMan2, PacMan_len, replace, PacMan_len);
    /*パックマンの向きを表す。１＝＞上、２＝＞下、３＝＞左、４＝＞右*/
    hword direction;
   
    while(1){
        /*キー入力を受け取ってdirectionにそれぞれ対応した数字を代入する*/
        for(i=0;i<10000;i++){
            
        }
         /*上向きの矢印キー入力を受け取る*/
        key = (hword*) 0x04000130;
        if(upkey==(upkey & *key)) {
        }
        /*入力があればdirectionに１を代入*/
        else{
            direction = 1;
        }
        /*下向きの矢印キー入力を受け取る*/
        if(downkey==(downkey & *key)) {
        }
        else{
            direction = 2;
        }
        /*左向きの矢印キー入力を受け取る*/
        if(leftkey==(leftkey & *key)) {
        }
        else{
            direction = 3;
        }
        /*右向きの矢印キー入力を受け取る*/
        if(rightkey==(rightkey & *key)) {
        }
        else{
            direction = 4;
        }
        /*パックマンを動かすメソッドを呼び出し、directionに応じた向きにパックマンを動かす。
        move_pacmanメソッドでは動いて食べたドットの数を返し、dot_numに代入することでドットの数の情報を同期させる*/
        dot_num = move_pacman(direction,pacman_mouse,2,dot_num,flow_line_map,144,exists_pacman_map,144,
        pixel_map,144,exists_pacman_cie,2,PacMan0,14,PacMan1,14,PacMan2,14,replace,14);
        /*それぞれの方向でパックマンがドットの白とコースの青、黒以外にぶつかるとwhile文を抜け出す*/
        /*左向き*/
        if (pixel_map[exists_pacman_cie[1] - 7][exists_pacman_cie[0]] == red || pixel_map[exists_pacman_cie[1] - 7][exists_pacman_cie[0]] == lightblue 
        || pixel_map[exists_pacman_cie[1] - 7][exists_pacman_cie[0]] == pink || pixel_map[exists_pacman_cie[1] - 7][exists_pacman_cie[0]] == orange)
        {
            break;
        }
        /*右向き*/
        if (pixel_map[exists_pacman_cie[1] + 8][exists_pacman_cie[0]] == red || pixel_map[exists_pacman_cie[1] + 8][exists_pacman_cie[0]] == lightblue
         || pixel_map[exists_pacman_cie[1] + 8][exists_pacman_cie[0]] == pink || pixel_map[exists_pacman_cie[1] + 8][exists_pacman_cie[0]] == orange)
        {
            break;
        }
        /*上向き*/
        if (pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] - 7] == red || pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] - 7] == lightblue 
        || pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] - 7] == pink || pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] - 7] == orange)
        {
            break;
        }
        /*下向き*/
        if (pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] + 8] == red || pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] + 8] == lightblue 
        || pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] + 8] == pink || pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] + 8] == orange)
        {
            break;
        }
        /*スコアを食べたドットの数から計算する*/
        score=dot_num*100;
        /*スコアを右上に表示する*/
        display_score(score,write_s,word_len,write_c,word_len,write_o,word_len,write_r,word_len,write_e,word_len,write_num,word_len);
        /*食べたドットの数が最大のドットの数と等しくなれば、つまりドットを全て食べればbreakする。*/
        if(dot_num==max_dot_num){
            break;
        }
    }
    /*画面全体を黒塗りする*/
    for(i=0;i<180;i++){
        for(j=0;j<240;j++){
            draw_point(j, i, black);
        }
    }
    /*最終的なスコアを画面の真ん中に表示する*/
    display_last_score(score,write_s,word_len,write_c,word_len,write_o,word_len,write_r,word_len,write_e,word_len,write_num,word_len);
    /*「Ｂ」入力を受け取ると初期状態に戻る＝＞メインメソッドの始めに戻る*/
    while(1){     
        key = (hword*) 0x04000130;
        if(bkey==(bkey & *key)) {
        }
        else{
           break; 
        }
    }
    return 0;
}

void draw_point (hword x, hword y, hword color){
	hword *ptr;
	ptr = (hword*)VRAM;
	ptr += 240 * y + x;
	*ptr = color;
}
/*マップのドットごとの色の情報を格納するメソッド。衝突判定でそのドットの色を見て判定する。*/
void make_pixel_map(hword pixel_map[][192], hword pixel_maplen){
    hword *ptr;
	ptr = (hword*)VRAM;
    ptr += 240 * ystart_map + xstart_map;
    hword i;
    hword j;
    /*ポインタを一つずつずらしていきそのポインタが持っている色の情報をpixel_map配列に代入する*/
    for(i=0;i<144;i++){
        for(j=0; j<192; j++){
            pixel_map[i][j] = *ptr;
            ptr ++;
        }
        ptr = ptr-192+240;
    }
}
/*パックマンが動くことができる動線を設定する*/
void make_flow_line(hword pixel_map[][192], hword pixel_maplen, hword flow_line_map[][192], hword flow_line_maplen){
    hword i;
    hword j;
    /*もしpixel_mapの色が白であればその点をflow_line_map配列に１を代入する。
    パックマンはflow_line_map配列が１の点のみ移動できる */
    for(i=0;i<144;i++){
        for(j=0; j<192; j++){
            if(pixel_map[i][j] == white){
                flow_line_map[i][j] = 1;
                /*その点から一つ右にずれる*/
                flow_line_map[i][j+1] = right_make_line(j+1,i,1,pixel_map, pixel_maplen, flow_line_map, flow_line_maplen);
                /*その点から一つ下にずれる*/
                flow_line_map[i+1][j] = down_make_line(j,i+1,1,pixel_map, pixel_maplen, flow_line_map, flow_line_maplen);
            }
        }
    }  
}
/*右向きに動線を設定*/
hword right_make_line(hword x, hword y, hword passed, hword pixel_map[][192], hword pixel_maplen, hword flow_line_map[][192], hword flow_line_maplen){
    /*再起メソッドが７回までしか再起しないようにする。一番端のドットとマップの線との距離が８だから*/
    hword count;
    count = passed+1;
    /*行き過ぎないように*/
    if(count > 8){
        return 0;
    }
    /*壁に当たったら*/
    else if(pixel_map[y][x] == blue){
        return 0;
    }
    /*ドットにぶつかったら*/
    else if(pixel_map[y][x] == white){
        return 1;
    }
    /*１つずつ右にずれていって白にたどり着いたら１を返す（再帰メソッド）*/
    else if(right_make_line(x+1, y, count, pixel_map, pixel_maplen, flow_line_map, flow_line_maplen)==1){
        flow_line_map[y][x] = 1;
        return 1;
    }
    else{
        return 0;
    }
}
/*下向きに動線を設定*/
hword down_make_line(hword x, hword y, hword passed, hword pixel_map[][192], hword pixel_maplen, hword flow_line_map[][192], hword flow_line_maplen){
    hword count;
    count = passed+1;
    /*行き過ぎないように*/
    if(count > 8){
        return 0;
    }
    /*壁に当たったら*/
    else if(pixel_map[y][x] == blue){
        return 0;
    }
    /*点にぶつかったら*/
    else if(pixel_map[y][x] == white){
        return 1;
    }
    /*１つずつ下にずれていって白にたどり着いたら１を返す（再帰メソッド）*/
    else if(down_make_line(x, y+1, count, pixel_map, pixel_maplen, flow_line_map, flow_line_maplen)==1){
        flow_line_map[y][x] = 1;
        return 1;
    }
    else{
        return 0;
    }
}
/*動線を描くためのメソッド。動線が正しくひかれているか確認するメソッド。メインメソッドで使用しない。*/
void draw_flow_line(hword flow_line_map[][192], hword flow_line_maplen){
    hword i;
    hword j;
    hword *ptr;
	ptr = (hword*)VRAM; 
    /*マップの左上の座標*/
    ptr += 240 * ystart_map + xstart_map;
    /**/
    for (i=0; i<144; i++)
    {
        for(j=0; j<192; j++)
        {
            if(flow_line_map[i][j]==1){
                *ptr = white;
            }
            ptr++;
        }
        ptr = ptr -192 + 240;
    }
    
}
/*マップを描画するメソッド*/
void draw_map (hword x, hword y, hword number){
	hword *ptr;
	ptr = (hword*)VRAM;
    hword a;
    hword b;
    hword i;
    /*マップには似たような形が多く存在するので、反転させることで配列を節約できた*/
    static hword replace[8][8];
	ptr += 240 * y + x;
    
    static hword curve1[8][8];/*外カーブ [y][x]の順*/
    curve1[0][4] = 1;
    curve1[1][4] = 1;
    curve1[2][4] = 1;
    curve1[3][3] = 1;
    curve1[4][2] = 1;
    curve1[4][1] = 1;
    curve1[4][0] = 1;
    
    

    static hword curve2[8][8] ;/*内カーブ [y][x]の順*/
    curve2[0][3] = 1;
    curve2[1][3] = 1;
    curve2[2][2] = 1;
    curve2[3][1] = 1;
    curve2[3][0] = 1;

    static hword curve3[8][8] ;/*特殊カーブ [y][x]の順*/
    curve3[0][7] = 1;
    curve3[1][7] = 1;
    curve3[2][6] = 1;
    curve3[3][5] = 1;
    curve3[3][4] = 1;
    curve3[3][3] = 1;
    curve3[3][2] = 1;
    curve3[3][1] = 1;
    curve3[3][0] = 1;

    static hword wall[8][8] ;/*横直線上*/
    /*何度も描画する*/
    for(i=0;i<8;i++){
        wall[3][i] = 1;
    }
    

    static hword dots[8][8] ;/*点*/
    dots[3][3] = 2;
    dots[3][4] = 2;
    dots[4][3] = 2;
    dots[4][4] = 2;

    static hword gate[8][8]; /*お化けが閉じ込められているエリアの入り口*/
    /*何度も描画する*/
    for(i=0;i<8;i++){
        gate[7][i] = 2;
    }

    /*numberは1:左上外カーブ, 2:右上外カーブ, 3:左下外カーブ, 4:右下外カーブ,
     5:横棒上, 6:横棒下, 7:縦線左, 8:縦線右, 9:点, 10:黒 ,
    11:左上内カーブ, 12:右上内カーブ, 13:左下内カーブ, 14:右下内カーブ,
    15:左上特殊カーブ, 16:右上特殊カーブ, 17:左下特殊カーブ, 18:右下特殊カーブ
    19:ゲート　である。この数字に応じて１から１９まで描画していく。
    replace配列を使って同じ配列を上下左右反転させて描画する。
    例えばcurev1配列を1,2,3,4で反転させて使用している。
    他も同様である。*/
    if(number==1){
        /*１つのマップが8×8であるため*/
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                /*curve1の配列の値が１であれば青、２であれば白を描画する。
                以下のfor文でもすべて同様である。*/
                if(curve1[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(curve1[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    /*curve1を左右反転させて描画*/
    else if(number==2){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = curve1[a][7-b];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    /*curve1を上下反転させて描画*/
    else if(number==3){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = curve1[7-a][b];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    /*curve1を上下左右反転させて描画*/
     else if(number==4){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = curve1[7-a][7-b];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==5){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(wall[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(wall[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==6){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = wall[7-a][b];   
            }
        }  
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==7){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = wall[b][a];   
            }
        }  
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==8){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][7-b] = wall[b][a];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==9){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(dots[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(dots[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==11){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(curve2[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(curve2[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==12){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = curve2[a][7-b];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==13){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = curve2[7-a][b];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==14){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = curve2[7-a][7-b];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==15){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(curve3[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(curve3[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==16){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = curve3[a][7-b];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==17){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = curve3[7-a][b];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    else if(number==18){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                replace[a][b] = curve3[7-a][7-b];   
            }
        }
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(replace[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(replace[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
    
    else if(number==19){
        for(a=0;a<8;a++){
            for(b=0;b<8;b++){
                if(gate[a][b]==1){
                    draw_point(x+b,y+a,blue);
                }
                else if(gate[a][b]==2){
                    draw_point(x+b,y+a,white);
                }
            }
        }
    }
}
/*上向きパックマンの描画*/
void up_draw_PacMan(hword x, hword y, hword number, hword PacMan0[][14], hword PacMan0_len, hword PacMan1[][14], hword PacMan1_len, 
hword PacMan2[][14], hword PacMan2_len, hword replace[][14], hword replace_len)
{
    hword i;
    hword j;
    /*numberは3種類のパックマンのうちどれにするかのnumber
        0ならただの丸、1ならチョイ開き、2なら45度開き */
    if(number == 0){
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                /*一度パックマンを描画する前に黒塗りしておく。
                j+x-6、i+y-6はパックマンの中心を設定している。*/
                draw_point(j+x-6, i+y-6, black);
                /*replace配列を使ってパックマンの向きを上下左右反転させて上向きにしている*/
                replace[i][j] = PacMan0[j][i];
                /*配列の値が１の時に黄色に描画する*/
                if(replace[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
    /*以下の文も同様である。*/
    else if(number == 1){
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                replace[i][j] = PacMan1[j][i];
                if(replace[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
    else{
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                replace[i][j] = PacMan2[j][i];
                if(replace[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
}
/*下向きパックマンの描画
上向きパックマンの処理と同様である。
replace配列を使用してパックマンを下向きにした*/
void down_draw_PacMan(hword x, hword y, hword number, hword PacMan0[][14], hword PacMan0_len, hword PacMan1[][14], 
hword PacMan1_len, hword PacMan2[][14], hword PacMan2_len, hword replace[][14], hword replace_len)
{
    hword i;
    hword j;
    if(number == 0){
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                replace[i][j] = PacMan0[j][13-i];
                if(replace[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
    else if(number == 1){
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                replace[i][j] = PacMan1[j][13-i];
                if(replace[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
    else{
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                replace[i][j] = PacMan2[j][13-i];
                if(replace[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
}
/*左向き向きパックマンの描画
上向きパックマンの処理と同様である。
PacMan配列のデフォルトが左向きなのでreplace配列を使用していない*/
void left_draw_PacMan(hword x, hword y, hword number, hword PacMan0[][14], hword PacMan0_len, hword PacMan1[][14], 
hword PacMan1_len, hword PacMan2[][14], hword PacMan2_len, hword replace[][14], hword replace_len)
{
    hword i;
    hword j;
    
    if(number == 0){
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                if(PacMan0[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
    else if(number == 1){
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                if(PacMan1[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
    else{
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                if(PacMan2[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
}
/*右向きパックマンの描画
上向きパックマンの処理と同様である。
replace配列を使用してパックマンを右向きにした*/
void right_draw_PacMan(hword x, hword y, hword number, hword PacMan0[][14], hword PacMan0_len, hword PacMan1[][14], hword PacMan1_len,
 hword PacMan2[][14], hword PacMan2_len, hword replace[][14], hword replace_len){
    hword i;
    hword j;

    if(number == 0){
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                replace[i][j] = PacMan0[i][13-j];
                if(replace[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
    else if(number == 1){
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                replace[i][j] = PacMan1[i][13-j];
                if(replace[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
    else{
        for(i=0;i<14;i++){
            for(j=0;j<14;j++){
                draw_point(j+x-6, i+y-6, black);
                replace[i][j] = PacMan2[i][13-j];
                if(replace[i][j]==1){
                    draw_point(j+x-6, i+y-6, yellow);
                }
            }
        }
    }
}
/*パックマンを動かすメソッド*/
hword move_pacman(hword direction, hword mouse[] , hword mouse_len, hword dot_num, hword flow_line_map[][192], hword flow_line_maplen,
 hword exists_pacman_map[][192], hword exists_pacman_maplen, hword pixel_map[][192], hword pixel_map_lan, hword *exists_pacman_cie, 
 hword exists_pacman_cie_len, hword PacMan0[][14], hword PacMan0_len, hword PacMan1[][14], hword PacMan1_len, hword PacMan2[][14],
  hword PacMan2_len, hword replace[][14], hword replace_len){
    hword i;
    hword j;
    /*パックマンの口の形３種類を２回に１回順番に表示させる。*/
    if(mouse[1] == 0){
        mouse[0]++;
        mouse[0] = mouse[0] % 3;
    }
    mouse[1]++;
    mouse[1] = mouse[1] % 2;
    
    /*上向きに進む場合*/
    if (direction == 1) {   
        /*パックマンが現在いる位置の一つ上のドットに動線が通っていれば*/
        if (flow_line_map[exists_pacman_cie[1] - 1][exists_pacman_cie[0]] == 1){
            /*前の位置にいたパックマンを消す*/
            for (i = 0; i < 14; i++) {
                draw_point(exists_pacman_cie[0] + xstart_map - 6 + i, exists_pacman_cie[1] + ystart_map + 7, black);
            }
            /*前の位置にパックマンはいなくなったという情報に書き換える*/
            exists_pacman_map[exists_pacman_cie[1]][exists_pacman_cie[0]] = 0;
            /*パックマンのいる位置を１つ上に変更する*/
            exists_pacman_cie[1]--;
            /*パックマンの中心のドットの１つ上のドットを中心としてパックマンを描画する*/
            up_draw_PacMan(exists_pacman_cie[0] + xstart_map, exists_pacman_cie[1] + ystart_map, mouse[0], PacMan0, PacMan0_len,
             PacMan1, PacMan1_len, PacMan2, PacMan2_len, replace, replace_len);
            /*パックマンがいる位置を更新する*/
            exists_pacman_map[exists_pacman_cie[1]][exists_pacman_cie[0]] = 1;
        }
    }
    /*下向きに進む場合、上向きの処理と同様にして処理する*/
    if (direction == 2)
    {
        if (flow_line_map[exists_pacman_cie[1] + 2][exists_pacman_cie[0]] == 1)
        {
            for (i = 0; i < 14; i++)
            {
                draw_point(exists_pacman_cie[0] + xstart_map - 6 + i, exists_pacman_cie[1] + ystart_map - 6, black);
            }
            exists_pacman_map[exists_pacman_cie[1]][exists_pacman_cie[0]] = 0;
            exists_pacman_cie[1]++;
            down_draw_PacMan(exists_pacman_cie[0] + xstart_map, exists_pacman_cie[1] + ystart_map, mouse[0], PacMan0, PacMan0_len, 
            PacMan1, PacMan1_len, PacMan2, PacMan2_len, replace, replace_len);
            exists_pacman_map[exists_pacman_cie[1]][exists_pacman_cie[0]] = 1;
        }
    }
    /*左向きに進む場合、上向きの処理と同様にして処理する*/
    if (direction == 3)
    {
        if (flow_line_map[exists_pacman_cie[1]][exists_pacman_cie[0] - 1] == 1)
        {
            for (i = 0; i < 14; i++)
            {
                draw_point(exists_pacman_cie[0] + xstart_map + 7, exists_pacman_cie[1] + ystart_map + i - 6, black);
            }
            exists_pacman_map[exists_pacman_cie[1]][exists_pacman_cie[0]] = 0;
            exists_pacman_cie[0]--;
            left_draw_PacMan(exists_pacman_cie[0] + xstart_map, exists_pacman_cie[1] + ystart_map, mouse[0], PacMan0, PacMan0_len,
             PacMan1, PacMan1_len, PacMan2, PacMan2_len, replace, replace_len);
            exists_pacman_map[exists_pacman_cie[1]][exists_pacman_cie[0]] = 1;
        }
    }
    /*右向きに進む場合、上向きの処理と同様にして処理する*/
    if (direction == 4)
    {
        
        if (flow_line_map[exists_pacman_cie[1]][exists_pacman_cie[0] + 2] == 1)
        {
            for (i = 0; i < 14; i++)
            {
                draw_point(exists_pacman_cie[0] + xstart_map - 6, exists_pacman_cie[1] + ystart_map + i - 6, black);
            }
            exists_pacman_map[exists_pacman_cie[1]][exists_pacman_cie[0]] = 0;
            exists_pacman_cie[0]++;
            right_draw_PacMan(exists_pacman_cie[0] + xstart_map, exists_pacman_cie[1] + ystart_map, mouse[0], PacMan0, PacMan0_len,
             PacMan1, PacMan1_len, PacMan2, PacMan2_len, replace, replace_len);
            exists_pacman_map[exists_pacman_cie[1]][exists_pacman_cie[0]] = 1;
        }
    }
    /*ドットを食べた数を計算するメソッドを使用してその数を代入する*/
   dot_num = eat_dots(direction, dot_num, pixel_map, pixel_map_lan, exists_pacman_cie, exists_pacman_cie_len);
   return dot_num;
}
/*食べたドットの数を計算する*/
hword eat_dots(hword direction, hword dot_num, hword pixel_map[][192], hword pixel_map_lan, hword *exists_pacman_cie, hword exists_pacman_cie_len){
    hword i;
    hword j;
    /*上向きに進む場合*/
    if (direction == 1){
        /*パックマンの１つ上のドットが白色であれば、つまり、パックマンが白いドットに当たれば*/
        if (pixel_map[exists_pacman_cie[1] - 7][exists_pacman_cie[0]] == white){
            /*食べたドットの数を増やす*/
            dot_num++;
            /*食べられたドットを消す*/
            for(i=7; i<9; i++){
                for(j=0;j<3;j++){
                    pixel_map[exists_pacman_cie[1]-i][exists_pacman_cie[0]-1+j]=black;
                }
            }
        }
    }
    /*下向きに進む場合、上向きの処理と同様にして処理する*/
    if (direction == 2){
        if (pixel_map[exists_pacman_cie[1] + 8][exists_pacman_cie[0]] == white){
            dot_num++;
            for(i=8; i<10; i++){
                for(j=0;j<3;j++){
                    pixel_map[exists_pacman_cie[1]+i][exists_pacman_cie[0]-1+j]=black;
                }
            }
        }
    }
    /*左向きに進む場合、上向きの処理と同様にして処理する*/
    if (direction == 3){
        if (pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] - 7] == white){
            dot_num++;
            for(i=7; i<9; i++){
                for(j=0;j<3;j++){
                    pixel_map[exists_pacman_cie[1]-1+j][exists_pacman_cie[0]-i]=black;
                }
            }
        }
    }
    /*右向きに進む場合、上向きの処理と同様にして処理する*/
    if (direction == 4){  
        if (pixel_map[exists_pacman_cie[1]][exists_pacman_cie[0] + 8] == white){
            dot_num++;
            for(i=8; i<10; i++){
                for(j=0;j<3;j++){
                    pixel_map[exists_pacman_cie[1]-1+j][exists_pacman_cie[0]+i]=black;
                }
            }
        }
    }
    return dot_num;
}
/*ゴーストを描画するメソッド*/
void draw_ghost(hword x, hword y, hword color, hword ghost[][14], hword ghost_len){ 
    hword i;
    hword j;
    /*ゴーストの大きさは14×14なのでその分for文を回す*/
    for(i=0;i<14;i++){
    	for(j=0;j<14;j++){	
            /*黒塗りしておく*/
            draw_point(j+x-6, i+y-6, black);
            /*ghost配列の値が０の部分に黒を描画*/
            if(ghost[i][j]==0){
                draw_point (j+x-6, i+y-6, black);
            }
            /*ghost配列の値が１の部分に引数の色を描画
            ゴーストの体の色を引数で変更できる*/
            else if(ghost[i][j]==1){
                draw_point (j+x-6, i+y-6, color);
            }
            /*ghost配列の値が２の部分に白を描画
            白目の部分*/
            else if(ghost[i][j]==2){
                draw_point (j+x-6, i+y-6, white);
            }
            /*ghost配列の値が３の部分に青を描画
            瞳の部分*/
            else if(ghost[i][j]==3){
                draw_point (j+x-6, i+y-6, blue);
            }  
  	    }
   }
}
/*スコア表示のための割り算を行うメソッド*/
hword div(hword score){
    hword count;
    hword i;
    count=0;
    /*スコアの数字を100で割った商を返す*/
    for(i=0;score>0;i++){
        score=score-100;
        count++;
    }
    return(count);
}
/*ゲーム中に右上にスコアを表示するメソッド*/
void display_score(hword score, hword write_s[][10], hword s_len, hword write_c[][10], hword c_len, hword write_o[][10], 
hword o_len, hword write_r[][10], hword r_len, hword write_e[][10], hword e_len, hword write_num[][10][10], hword num_len){
    hword i;
    hword j;
    hword x;
    hword y;
    hword count100=0;/*100の位*/
    hword count1000=0;/*1000の位*/
    hword count10000=0;/*10000の位*/
    hword keepdiv;/*商*/
    x=192;
    y=10;   
    /*「SCORE」を右上に表示させる
    文字の大きさは10×10である
    x座標のみを９ドット分ずらしてそれぞれの文字を描画する*/
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){          
            if(write_s[i][j]==1){
                draw_point(j+x,i+y,white);
            }           
        }
    }
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){          
            if(write_c[i][j]==1){
                draw_point(j+x+9,i+y,white);
            }    
        }
    }
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){            
            if(write_o[i][j]==1){
                draw_point(j+x+18,i+y,white);
            }   
        }
    }
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){ 
            if(write_r[i][j]==1){
                draw_point(j+x+27,i+y,white);
            }
        }
    }
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){           
            if(write_e[i][j]==1){
                draw_point(j+x+36,i+y,white);
            }
            
        }
    }
    /*1,10の位の0を描画する
    「SCORE」の下に書くためにy座標を１０ずらしている
    x座標も同様にして９ずつずらしている*/
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){         
            if(write_num[0][i][j]==1){
                draw_point(j+x+27,i+y+10,white);
                draw_point(j+x+36,i+y+10,white);
            }           
        }
    }
    /*商を代入*/
    keepdiv=div(score);
    /*商が100以上つまりスコアが10000以上の時*/
    if(keepdiv>=100){
        /*10000の位の値を取り出す*/
        for(i=0;keepdiv>=100;i++){
            keepdiv=keepdiv-100;
            count10000++;
        }
        /*1000の位の値を取り出す*/
        for(i=0;keepdiv>=10;i++){
            keepdiv=keepdiv-10;
            count1000++;
        }
        /*残った100の位の値を取り出す*/
        count100=keepdiv;
        for(i=0;i<10;i++){
            for(j=0;j<10;j++){   
                /*念のため黒塗りしておく*/
                draw_point(j+x+18,i+y+10,black);
                draw_point(j+x+9,i+y+10,black);
                draw_point(j+x,i+y+10,black);
                /*100の位を描画*/
                if(write_num[count100][i][j]==1){
                    draw_point(j+x+18,i+y+10,white);
                }
                /*1000の位を描画*/
                if(write_num[count1000][i][j]==1){
                    draw_point(j+x+9,i+y+10,white);
                }
                /*10000の位を描画*/
                if(write_num[count10000][i][j]==1){
                    draw_point(j+x,i+y+10,white);
                }
            }
        }
    }
    /*スコアが1000以下の場合
    上の処理の1000以下の位を表示させる処理と同様*/
    else if(keepdiv>=10){
        for(i=0;keepdiv>=10;i++){
            keepdiv=keepdiv-10;
            count1000++;
        }
        count100=keepdiv;

        for(i=0;i<10;i++){
            for(j=0;j<10;j++){     
                draw_point(j+x+18,i+y+10,black);
                draw_point(j+x+9,i+y+10,black);    
                if(write_num[count100][i][j]==1){
                    draw_point(j+x+18,i+y+10,white);
                }
                if(write_num[count1000][i][j]==1){
                    draw_point(j+x+9,i+y+10,white);
                }
            }
        }
    }
    else{
        for(i=0;i<10;i++){
            for(j=0;j<10;j++){       
                draw_point(j+x+18,i+y+10,black);
                if(write_num[keepdiv][i][j]==1){
                    draw_point(j+x+18,i+y+10,white);
                }
            }
        }
    }
}
/*最後にスコアを表示するメソッド
ゲーム中に右上にスコアを表示するメソッドと同様。
表示する位置を中央にしている*/
void display_last_score(hword score, hword write_s[][10], hword s_len, hword write_c[][10], hword c_len, hword write_o[][10], 
hword o_len, hword write_r[][10], hword r_len, hword write_e[][10], hword e_len, hword write_num[][10][10], hword num_len){
    hword i;
    hword j;
    hword x;
    hword y;
    hword count100=0;/*100の位*/
    hword count1000=0;/*1000の位*/
    hword count10000=0;/*10000の位*/
    hword keepdiv;
    x=95;
    y=70;
    
    // score
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){
            
            if(write_s[i][j]==1){
                draw_point(j+x,i+y,white);
            }
            
        }
    }
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){
            
            if(write_c[i][j]==1){
                draw_point(j+x+9,i+y,white);
            }
            
        }
    }

    for(i=0;i<10;i++){
        for(j=0;j<10;j++){        
            if(write_o[i][j]==1){
                draw_point(j+x+18,i+y,white);
            }           
        }
    }
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){       
            if(write_r[i][j]==1){
                draw_point(j+x+27,i+y,white);
            }         
        }
    }
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){        
            if(write_e[i][j]==1){
                draw_point(j+x+36,i+y,white);
            }          
        }
    }
    /*1,10の位の0*/
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){         
            if(write_num[0][i][j]==1){
                draw_point(j+x+27,i+y+10,white);
                draw_point(j+x+36,i+y+10,white);
            }
            
        }
    }
    keepdiv=div(score);
    if(keepdiv>=100){
        
        for(i=0;keepdiv>=100;i++){
            keepdiv=keepdiv-100;
            count10000++;
        }
        for(i=0;keepdiv>=10;i++){
            keepdiv=keepdiv-10;
            count1000++;
        }
        count100=keepdiv;

        for(i=0;i<10;i++){
            for(j=0;j<10;j++){   
                draw_point(j+x+18,i+y+10,black);
                draw_point(j+x+9,i+y+10,black);
                draw_point(j+x,i+y+10,black);
                if(write_num[count100][i][j]==1){
                    draw_point(j+x+18,i+y+10,white);
                }
                if(write_num[count1000][i][j]==1){
                    draw_point(j+x+9,i+y+10,white);
                }
                if(write_num[count10000][i][j]==1){
                    draw_point(j+x,i+y+10,white);
                }
            }
        }

    }
    else if(keepdiv>=10){     
        for(i=0;keepdiv>=10;i++){
            keepdiv=keepdiv-10;
            count1000++;
        }
        count100=keepdiv;
        for(i=0;i<10;i++){
            for(j=0;j<10;j++){     
                draw_point(j+x+18,i+y+10,black);
                draw_point(j+x+9,i+y+10,black);    
                if(write_num[count100][i][j]==1){
                    draw_point(j+x+18,i+y+10,white);
                }
                if(write_num[count1000][i][j]==1){
                    draw_point(j+x+9,i+y+10,white);
                }   
            }
        }
    }
    else{
        for(i=0;i<10;i++){
            for(j=0;j<10;j++){       
                draw_point(j+x+18,i+y+10,black);
                if(write_num[keepdiv][i][j]==1){
                    draw_point(j+x+18,i+y+10,white);
                }
            }
        }
    }
}