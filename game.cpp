#include "game.h"
#include "console/console.h"
#include "tetromino.h"
#include <random>
#include <iostream>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];
  //테트로미노가 존재할때마다 그 위치를 board_에 저장시켜야함. -> update에서 구현?

  Tetromino currentTetromino = randomTetromino();
  Tetromino nextTetromino = randomTetromino();
  Tetromino* holdTetromino;

  // 테트로미노의 위치
  int currentX=BOARD_WIDTH/2;
  int currentY=0;

  int lineScore=0;

  bool isHold = false;

  //shadow에 겹쳤을 때 멈추고 새로운 테트로미노를 생성.
  void stopTetromino(){
    isHold = false;
    currentTetromino = nextTetromino;
    nextTetromino = randomTetromino();
  }

  //테트로미노가 한줄 채워졌을때
  void clearOneLine(int clearY){
    for (int i = 0; i < BOARD_WIDTH; i++) {
      for (int j = clearY; j > 0; j--) {
        board_[i][j] = board_[i][j-1];
      }
    }

    for (int i = 0; i < BOARD_WIDTH; i++) {
      board_[i][0] = false;
    }

    lineScore++;
  }
  
  //랜덤 테트로미노 생성.
  Tetromino randomTetromino(){
    std::random_device rd;  // 하드웨어 기반의 난수 생성기
    std::mt19937 gen(rd()); // Mersenne Twister 엔진을 사용한 랜덤 숫자 생성기
    std::uniform_int_distribution<int> dis(0, 6); // 0부터 6까지의 균일 분포
    Tetromino t =Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
    switch(dis(gen)){
      case 0:
        t=Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
        break;
      case 1:
        t=Tetromino("J", 3, "OXXOOOXXX");
        break;
      case 2:
        t=Tetromino("L", 3, "XXOOOOXXX");
        break;
      case 3:
        t=Tetromino("O", 2, "OOOO");
        break;
      case 4:
        t=Tetromino("S", 3, "XOOOOXXXX");
        break;
      case 5:
        t=Tetromino("T", 3, "XOXOOOXXX");
        break;
      case 6:
        t=Tetromino("Z", 3, "OOXXOOXXX");
        break;
      default:
        t=Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
        break;
    }
    currentX=BOARD_WIDTH/2;
    currentY=0;
    return t;
  }

  void handleInput(){
    // 왼쪽 이동
    if (console::key(console::K_LEFT)) {
      bool movable = true;
      for (int i = 0; i < currentTetromino.size(); i++) {
        if (isTouchedBoard() || (board_[currentX][currentY + i] && board_[currentX - 1][currentY + i])) {
          movable = false;
          break;
        }
      }
      if (movable) {
        currentX--;
      }
    }
    // 오른쪽 이동
    else if (console::key(console::K_RIGHT)) {
      bool movable = true;
      for (int i = 0; i < currentTetromino.size(); i++) {
        if (isTouchedBoard() || (board_[currentX][currentY + i] && board_[currentX + 1][currentY + i])) {
          movable = false;
          break;
        }
      }
      if (movable) {
        currentX++;
      }
    }
    // 소프트 드롭
    else if (console::key(console::K_DOWN)) {

    }
    // 섀도우 위치에 꽝!
    else if (console::key(console::K_UP)){
      currentY = getShadow();
    }
    // 시계 방향으로 회전
    else if (console::key(console::K_X)) {
      Tetromino rotated = currentTetromino.rotatedCW();
      if (isValidMove(rotated)) {
        currentTetromino = rotated;
      }
    }
    // 반시계 방향으로 회전
    else if (console::key(console::K_Z)) {
      Tetromino rotated = currentTetromino.rotatedCCW();
      if (isValidMove(rotated)) {
        currentTetromino = rotated;
      }
    }
    // 홀드
    else if (console::key(console::K_SPACE)) {
      if (holdTetromino == NULL) {
        holdTetromino = currentTetromino.original();
        currentTetromino = randomTetromino();
      } else if (!isHold) {
        Tetromino temp = currentTetromino;
        currentTetromino = *holdTetromino;
        holdTetromino = temp.original();
      }
      isHold = true;
    }
  }

  // 테트로미노가 상하좌우의 벽에 닿았는지.
  // true인 부분이 board의 벽에 닿았는지로 수정해야함.
  bool isTouchedBoard() {
    return currentX + currentTetromino.size() >= BOARD_WIDTH || currentY + currentTetromino.size() >= BOARD_HEIGHT;
  }

  // 테트로미노를 회전시킬때 회전이 가능한지.
  bool isValidMove(Tetromino rotated) {
    for (int i = 0; i < rotated.size(); i++) {
      for (int j = 0; j < rotated.size(); j++) {
        if (board_[currentX+i][currentY+j] && rotated.check(i, j)) {
          return false;
        }
      }
    }
    return true;
  }

  // 현재 테트로미노의 x위치는 같고, y만 바꾸면 됨.
  // return값이 shadow의 y값.
  int getShadow() {
    int shadowY = BOARD_HEIGHT;
    for (int i = 0; i < currentTetromino.size(); i++) {
      int y = -1;
      for (int j = currentTetromino.size() - 1; j >= 0; j--) {
        if (currentTetromino.check(i, j)) {
          y = j;
          break;
        }
      }
      if (y == -1) {
        continue;
      }

      for (int j = y+1; j < BOARD_HEIGHT; j++) {
        if (board_[currentX+i][j]) {
          shadowY = shadowY > j ? j-1 : shadowY;
          break;
        }
      }
    }

    shadowY = shadowY != BOARD_HEIGHT ? shadowY - currentTetromino.size() + 1 : shadowY - 1;
    return shadowY;
  }

  // tetromino가 멈춰야하는지
  // 밑바닥에 닿았거나 board의 true를 만났거나.
  // 수정 요망
  bool isStopTet(){
    for (int i = 0; i<currentTetromino.size(); i++){
      for(int j = 0; j<currentTetromino.size(); j++){
        if(currentTetromino.check(i, j)&&board_[i][j+1]||currentTetromino.size()+j>=BOARD_HEIGHT)
          return true;
      }
    }
    return false;
  }

public:
  // 게임의 한 프레임을 처리한다.
  void update(){
    //테트로미노가 하나 조작되고 있으면 새로 나오면 안됨.
    // 사용자 입력 처리
    handleInput();
    /*
    1. 입력받기
    2. 좌우인 경우, 움직일 수 있는지 여부 확인하고 움직이기
    3. 위인 경우, 걍 내려
    4. 아래인 경우, 빠르게 내리기
    5. 회전인 경우, 움직일 수 있는지 여부 확인하고 움직이기
    6. 홀드인 경우, 홀드를 이미 했었는지 여부 확인하고 홀드
    */
   if(isStopTet()){

   }
  }

  // 게임 화면을 그린다.
  void draw(){
    //틀 그리기.
    console::drawBox(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    //테트로미노가 존재하는곳에 그리기.
    //tetromino 구현 방법. (game안에서)
    for(int i=0; i<BOARD_WIDTH; i++){
      for(int j=0; j<BOARD_HEIGHT; j++){
        if(board_[i][j])
          console::draw(i,j, BLOCK_STRING);
        else
          console::draw(i,j," ");
        console::draw(currentX, getShadow(), SHADOW_STRING);
      }
    }
  }

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit() {
    /*
    종료 조건
    1. 40줄 성공!!
      - 성공 조건 검사하기
      - 성공했을 때 뜨는 화면 그리기
      -- line이 지워질때마다 value값 1씩 증가
    2. 생성되는 공간에 테트로미노가 존재하면 실패!!
      - 실패 조건 검사하기
      - 실패했을 때 뜨는 화면 그리기
    3. ESC 눌렀을 때 즉시 종료
      - 그냥 멈추기
    */
   if(console::key(console::K_ESC)){
    return true;
   }

  };

  Game() {
  currentTetromino = randomTetromino();
  nextTetromino = randomTetromino();
  holdTetromino;

  currentX=BOARD_WIDTH/2;
  currentY=0;

  lineScore=0;

  isHold = false;
  }
};