#include <MsTimer2.h>
const int LED = 12; //LEDをデジタルピン13に接続
const int RESET_LED = 13; //LEDをデジタルピン13に接続
const int BUTTON = A1; // ボタンをデジタルピン7に接続
const int RESET_BUTTON = A3; // ボタンをデジタルピン7に接続
 
int val = 0; //ボタンの状態
int old_val = 0; //valの前の値を保存しておく変数
int state = 0;   // LEDの状態を保存しておく変数
int wait = 0; 
int pomodoro_max_count = 1500;

int rval = 0;
int rold_val = 0; //valの前の値を保存しておく変数
int rstate = 0;   // LEDの状態を保存しておく変数

int count = 0;
// 7 LED
/////////////////////////////////////////////////
const byte digit[10] = //7seg 点灯パターン
{
  B00000011, // 数字 0
  B10011111, // 1
  B00100101, // 2
  B00001101, // 3
  B10011001, // 4
  B01001001, // 5
  B01000001, // 6
  B00011011, // 7
  B00000001, // 8
  B00001001  // 9
};



// setup
//////////////////////////////////////////////

void setup() {
  pinMode(LED,OUTPUT); //LED変数は出力に
  pinMode(BUTTON, INPUT); // ボタン変数は入力に
  Serial.begin(9600);

  MsTimer2::set(1000, flash); // 500msの期間
  MsTimer2::start();
}
 

// reset処理
//////////////////////////////////////////////
void reset (){
  if (wait == 1){
    rval = digitalRead(RESET_BUTTON);  //ボタンの状態を常時取得
      //Serial.println(rval);

     //ボタンが押された時に行う処理
     if((rval==1)&&(rold_val==0)){
       rstate = 1 - rstate; //LEDがオンなら1-1でオフ、オフなら1-0でオンに。
       delay(100); //バウンシング防止（スイッチの半端な接触バグ防止）
     }

     rold_val = rval; //変化を補足するために前のvalを保存

     //stateに応じてLEDを点灯。
     if(rstate==0){
       digitalWrite(RESET_LED,HIGH);
       count = 0;
       rval = 0;
       rstate = 1;
 
      val = 0;
      old_val = 0;
      wait = 0;
    }
  }else{
    count = 0;
  }
}

// start 処理
//////////////////////////////////////////////
void start_count(){
  val = digitalRead(BUTTON);  //ボタンの状態を常時取得
 
  //ボタンが押された時に行う処理
  if((val==1)&&(old_val==0)){
    state = 1 - state; //LEDがオンなら1-1でオフ、オフなら1-0でオンに。
    delay(100); //バウンシング防止（スイッチの半端な接触バグ防止）
  }
  old_val = val; //変化を補足するために前のvalを保存
  //stateに応じてLEDを点灯。
  if(state==1){
    digitalWrite(LED,HIGH);
    wait = 0;
  }else{
    digitalWrite(LED,LOW);
    wait = 1;
  }
  return wait;
}



// タイマー処理
///////////////////////////////////////////////
void flash (){
  if ( pomodoro_max_count > count ){
    if (wait == 1){
       count += 1;
       Serial.println(count);
    } else {
       Serial.println("wait");
    }
  }else{
  Serial.println("fin");
  }
}


// main
////////////////////////////////////////////////////
void loop() {
  start_count();
  reset();
}
