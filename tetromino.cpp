#include "tetromino.h"
#include "console/console.h"
#include <string>
#include <random>
#include <iostream>

#define SHADOW_STRING "⛶"
#define BLOCK_STRING "■"

class Tetromino {
  static const int MAX_SIZE = 4;

  // 디버그용 이름
  std::string name_;

  // 테트로미노의 모양을 저장하는 배열
  bool shape_[MAX_SIZE][MAX_SIZE];

  // 회전되지 않은 원래 테트로미노 객체를 저장하는 포인터
  Tetromino *original_;

  // 테트로미노의 사이즈
  int size_;

public:
  // 사이즈와 shape 문자열을 통해 생성한다.
  // 문자열은 size * size 길이의 문자열이 주어진다.
  // 테트로미노 생성자
  Tetromino(std::string name, int size, std::string shape){
    name_=name;
    size_=size;
    for(int i = 0; i<size; i++){
      for (int j = 0; j<size; j++){
        for(int k = 0; k<shape.length(); k++){
          if(shape[k]=='O')
            shape_[i][j]=true;
          else if(shape[k]=='X')
            shape_[i][j]=false;
        }
      }
    }
    if(original_==NULL)
      *original_ = Tetromino(name_, size_, shape);
  }

  // 이름을 반환한다.
  std::string name() { return name_; }

  // 테트로미노의 사이즈를 반환한다.
  int size() { return size_; }

  // 회전되지 않은 원래 테트로미노 객체의 포인터를 반환한다.
  Tetromino *original() { return original_; }

  // 시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
  Tetromino rotatedCW(){
    bool rot_shape_[size_][size_];
    for(int i = 0; i<size_; i++){
      for (int j = 0; j<size_; j++){
        rot_shape_[i][j]=shape_[j][size_-i-1];
      }
    }
    //->shape_[i][j] = shape_[j][size-i-1]
    //입력받는 인수가 stirng이므로 string으로 전환
    std::string sShape;
    for(int k = 0; k<size_*size_; k++){
      if(rot_shape_[k])
        sShape+='O';
      else
        sShape+='X';
    }
    return Tetromino(name_, size_, sShape);
    //rot_shape_를 Tetromino에 삽입
  }

  // 반시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
  Tetromino rotatedCCW(){
    bool rot_shape_[size_][size_];
    for(int i = 0; i<size_; i++){
      for (int j = 0; j<size_; j++){
        rot_shape_[i][j]=shape_[size_-1-j][i];
      }
    }
    //->shape_[i][j] = shape_[size-j-1][i]
    //입력받는 인수가 stirng이므로 string으로 전환
    std::string sShape;
    for(int k = 0; k<size_*size_; k++){
          if(rot_shape_[k])
            sShape+='O';
          else
            sShape+='X';
        }
    return Tetromino(name_, size_, sShape);
    //rot_shape_를 Tetromino에 삽입
  }

  // 화면의 x, y 위치에 s 문자열로  테트로미노를 그린다
  void drawAt(std::string s, int x, int y){
    //그냥 테트로미노의 좌표를 저장하고 game에서의 draw로 얘를 불러내면될듯.
    for(int i=0; i<size_; i++){
      for(int j=0; j<size_; j++){

      }
    }
  }

  // 테트로미노의 좌상단 기준 x, y 위치에 블록이 있는지 여부를 나타내는 함수
  // 얘로 블럭끼리 겹치지않게 하는듯?
  bool check(int x, int y) { return shape_[x][y]; }

  // 각 테트로미노 종류에 대한 선언
  // cpp 파일에서 Tetromino Tetromino::I(...); 와 같이 구현한다
  static Tetromino I, O, T, S, Z, J, L;

  //shadow는 private으로 구현
  //hold도 private.
  //game에서 구현해야하나?
};