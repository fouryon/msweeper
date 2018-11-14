#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// boardの大きさ
#define LENGTH 8

// Mの個数
#define NUM_OF_M 10

// Mが配置されている状態
#define MINE -1

// 点の開閉状態
#define OPEN 1
#define CLOSE 0

// ゲームの状態
#define CLEAR 1
#define PLAYING 0
#define GAMEOVER -1

typedef struct BOX{
  int M_pos;  // Mの配置か周囲のMの個数
  int state;  // 開閉の状態
} BOX;

void init_board(BOX board[LENGTH][LENGTH]);
void set_M(BOX board[LENGTH][LENGTH]);
void shuffle_board(BOX board[LENGTH][LENGTH]);
void input_count(BOX board[LENGTH][LENGTH]);
int count_around_M(BOX board[LENGTH][LENGTH], int x, int y);
void disp_board(BOX board[LENGTH][LENGTH]);
void input_coordinate(int *x, int *y);
int open_box(BOX board[LENGTH][LENGTH], int x, int y);
void game_clear_check(int *gameState);
void open_0(BOX board[LENGTH][LENGTH], int x, int y);
void game_end(int gameState);

int main(void)
{
  BOX board[LENGTH][LENGTH];
  int gameState = PLAYING;
  int x, y;

  srand((unsigned)time(NULL));

  init_board(board);
  set_M(board);
  input_count(board);
  disp_board(board);

  while (gameState == PLAYING) {
    input_coordinate(&x, &y);
    gameState = open_box(board, x, y);
    disp_board(board);
  }

  game_end(gameState);

  return 0;
}

void init_board(BOX board[LENGTH][LENGTH])
  // boardを初期化
{
  int x, y;

  for (y = 0; y < LENGTH; y++) {
    for (x = 0; x < LENGTH; x++) {
      board[y][x].M_pos = 0;
      board[y][x].state = CLOSE;
    }
  }

}

void set_M(BOX board[LENGTH][LENGTH])
  // Mをboard上に配置
{
  int x, y, i;

  for (i = 0; i < NUM_OF_M; i++){
    x = i % LENGTH;
    y = i / LENGTH;
    board[y][x].M_pos = MINE;
  }

  shuffle_board(board);

}

void shuffle_board(BOX board[LENGTH][LENGTH])
  // boardをシャッフルする
{
  int x, y, x_tmp, y_tmp, tmp;

  for (y = 0; y < LENGTH; y++) {
    for (x = 0; x < LENGTH; x++) {
      x_tmp = rand() % LENGTH;
      y_tmp = rand() % LENGTH;
      tmp = board[y][x].M_pos;
      board[y][x].M_pos = board[y_tmp][x_tmp].M_pos;
      board[y_tmp][x_tmp].M_pos = tmp;
    }
  }

}

void input_count(BOX board[LENGTH][LENGTH])
  // M_posに周囲のMの数を入れる
{
  int x, y;

  for (y = 0; y < LENGTH; y++) {
    for (x = 0; x < LENGTH; x++) {
      if (board[y][x].M_pos != MINE) {
        board[y][x].M_pos = count_around_M(board, x, y);
      }
    }
  }

}

int count_around_M(BOX board[LENGTH][LENGTH], int x, int y)
  // 指定座標の周囲にあるMを数える
{
  int count = 0;
  int x_tmp, y_tmp;

  for (y_tmp = y-1; y_tmp <= y+1; y_tmp++) {
    for (x_tmp = x-1; x_tmp <= x+1; x_tmp++) {
      if (x_tmp >= 0 && x_tmp < LENGTH &&
          y_tmp >= 0 && y_tmp < LENGTH &&
          board[y_tmp][x_tmp].M_pos == MINE){
        count++;
      }
    }
  }

  return count;
}

void disp_board(BOX board[LENGTH][LENGTH])
  // boardの表示
{
  int x, y;

  printf("\033[2J");  // 画面全体を削除
  printf("\033[H");   // カーソル位置を画面左上にする

  printf("   ");
  for (x = 0; x < LENGTH; x++) {
    printf("%d ", x);
  }
  puts("");puts("");

  for (y = 0; y < LENGTH; y++) {
    printf("%d  ", y);
    for (x = 0; x < LENGTH; x++) {
      if (board[y][x].state == OPEN) {
        if (board[y][x].M_pos == MINE){
          printf("M ");
        } else {
          printf("%d ", board[y][x].M_pos);
        }
      } else {
        printf(". ");
      }
    }
    puts("");
  }

}

void input_coordinate(int *x, int *y)
  // キー入力から開く座標を得る
{
  char dummy;

  while (1) {
    printf("縦座標を入力：");
    if (scanf("%d", y) == 1 && *y >= 0 && *y < LENGTH) break;
    scanf("%*[^\n]");  // 改行入力まで読み飛ばす
  }
  scanf("%*[^\n]");

  while (1) {
    printf("横座標を入力：");
    if (scanf("%d", x) == 1 && *x >= 0 && *x < LENGTH) break;
    scanf("%*[^\n]");
  }
  scanf("%*[^\n]");

  puts("");

}

int open_box(BOX board[LENGTH][LENGTH], int x, int y)
  // boardを1点開く
{
  int gameState = PLAYING;

  board[y][x].state = OPEN;
  game_clear_check(&gameState);

  if (board[y][x].M_pos == MINE){
    gameState = GAMEOVER;
  }else if (board[y][x].M_pos == 0) {
    open_0(board, x, y);
  }

  return gameState;
}

void game_clear_check(int *gameState)
  // ゲームクリアしているかどうかのチェック
{
  static int box_count = 0;

  box_count++;
  if (box_count == LENGTH*LENGTH - NUM_OF_M){
    // 開いていないマスがMの数と同じになったらクリア
    *gameState = CLEAR;
  }

}

void open_0(BOX board[LENGTH][LENGTH], int x, int y)
  // 0の周りの点を開く
{
  int x_tmp, y_tmp, dummy;

  for (y_tmp = y-1; y_tmp <= y+1; y_tmp++) {
    for (x_tmp = x-1; x_tmp <= x+1; x_tmp++) {
      if (board[y_tmp][x_tmp].state == CLOSE &&
          x_tmp >= 0 && x_tmp < LENGTH &&
          y_tmp >= 0 && y_tmp < LENGTH ){
        // 0の周りでまだ開いていないマスを開く
        dummy = open_box(board, x_tmp, y_tmp);
      }
    }
  }

}

void game_end(int gameState)
  // ゲーム終了時の処理
{
  if (gameState == CLEAR){
    puts("-----GAME CLEAR-----");
  }else{
    puts("-----GAME OVER-----");
  }

}
