#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define RST 12
#define CE  13
#define DC  11
#define DIN  10
#define CLK  9 

#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1
#define BUTTON_UP 2  //кнопка вверх
#define BUTTON_RIGHT 3  //кнопка вправо
#define BUTTON_DOWN 4  //кнопка вниз
#define BUTTON_LEFT 5  //кнопка влево
#define BUTTON_G 8  //кнопка при надавливании на джойстик
#define DELAY 200  //пауза при считывании значений

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);
int record[5] = {0,0,0,0,0};
int speedGame = 0;
int station[3] = {0,0,0};


void changeRecords(int score){
  for(int i = 0; i < 5; ++i){
    if (score > record[i]){
      for( int j = 4; j > i; --j)
        record[j] = record[j-1];
    record[i] = score;
  }
}
}
void BasicInterface()  {
  display.drawLine(0, 3, 0, 80, BLACK);
  display.drawLine(47, 3, 47, 80, BLACK);
  display.drawLine(1, 3, 46, 50, BLACK);
  display.drawLine(1, 80, 46, 80, BLACK);
   display.setCursor(55,0);
   display.print("SCORE");
   display.setCursor(55,16);
   display.print("RECORD");
   display.setCursor(55,68);
   display.print("LEVEL");
}

void MoveBlocks(){
  for (int i = 0; i<4; ++i)
    display.fillRect(7 + 12*i, 6, 14 + 12*i, 9, BLACK);
  delay(1000);
  for (int i = 0; i < 4; ++i)
    display.fillRect(7 + 12*i, 10, 14 + 12*i, 13, BLACK);
  delay(1000);
  for (int i = 0; i < 13; ++i){
    display.fillRect(7 + 12*i, 6 + 4*i, 14 + 12*i, 9 + 4*i, WHITE);
    display.fillRect(7 + 12*i, 14 + 4*i, 14 + 12*i, 17 + 4*i, BLACK);
    delay(1000);
  }
}

void drawBlocks1(int i,int numberblock, int state){
  if (state ==0)
    display.fillRect(7 + 12*numberblock, 6, 10 + 12*numberblock, 9, BLACK);
  else 
    display.fillRect(7 + 12*numberblock, 6, 14 + 12*numberblock, 9, BLACK);
}


void drawBlocks2(int i, int numberblock, int state){
  if (state == 3)
    display.fillRect(7 + 12*numberblock, 6 + i*4, 10 + 12*numberblock, 9 + i*4, BLACK);
  else if (state == 5)
    display.fillRect(7 + 12*numberblock, 6 + i*4, 14 + 12*numberblock, 9 + i*4, BLACK);
}

void drawBlock(int i){
  display.fillRect(17 + (i)*12, 66, 24 + (i)*12, 73, WHITE);
  if (station[i] == 0)
    display.fillRect(17 + (i)*12, 70, 20 + (i)*12, 73, BLACK);
  else if (station[i] == 1)
    display.fillRect(17 + (i)*12, 70, 24 + (i)*12, 73, BLACK);
  else if (station[i] == 2){
    display.fillRect(17 + (i)*12, 66, 20 + (i)*12, 69, BLACK);
    display.fillRect(17 + (i)*12, 70, 24 + (i)*12, 73, BLACK);
  }
  else if (station[i] == 3)
    display.fillRect(17 + (i)*12, 66, 24 + (i)*12, 73, BLACK);
}


int ChangeBlocks(int level){
  for (int i = 0; i < (10-speedGame)*(11-level)*2; ++i){
    if (digitalRead(BUTTON_LEFT) == HIGH){
      station[0] = (++station[0])%4;
      drawBlock(0);
    }
    if ((digitalRead(BUTTON_UP) == HIGH) || (digitalRead(BUTTON_DOWN) == HIGH)){
      station[1] = (++station[1])%4;
      drawBlock(1);
    }
    if (digitalRead(BUTTON_LEFT) == HIGH){
      station[2] = (++station[2])%4;
      drawBlock(2);
    }
  delay(5);
  }
  
}

void drawRecords(){
  display.clearDisplay();
  display.setCursor(3,18);
  display.println("RECORD1 ");
  display.setCursor(42,18);
  display.println(record[0]);
  display.setCursor(3,28);
  display.println("RECORD2 ");
  display.setCursor(42,28);
  display.println(record[1]);
  display.setCursor(3,38);
  display.println("RECORD3 ");
  display.setCursor(42,38);
  display.println(record[2]);
  display.setCursor(3,48);
  display.println("RECORD4 ");
  display.setCursor(42,48);
  display.println(record[3]);
  display.setCursor(3,58);
  display.println("RECORD5 ");
  display.setCursor(42,58);
  display.println(record[4]);
}

void drawSettings(int speedGame){
display.clearDisplay();
display.setCursor(1,39);
display.println("GAMESPEED X1,");
display.setCursor(76,39);
}
void drawMenu (int choise){
  display.clearDisplay();
  display.setCursor(27,20);
  display.println("START");
  display.setCursor(18,38);
  display.println("SETTINGS");
  display.setCursor(23,58);
  display.println("RECORDS");
  if (choise == 1)
     display.drawRect(12, 15, 72, 32, BLACK);
  else
     display.drawRect(14, 17, 70, 29, BLACK);
  if (choise == 2)
     display.drawRect(12, 33, 72, 49, BLACK);
  else
     display.drawRect(14, 35, 70, 47, BLACK);
  if (choise == 3)
     display.drawRect(12, 51, 72, 69, BLACK);
  else 
     display.drawRect(14, 53, 70, 67, BLACK);
}



bool menu(bool backToMenu){
   backToMenu = false;
   display.clearDisplay();
   int menuChoise =1;
   drawMenu(menuChoise);
   while(digitalRead(BUTTON_G) == LOW){
     if (digitalRead(BUTTON_UP) == HIGH){
        if (menuChoise < 3)
          ++menuChoise;
        drawMenu(menuChoise);
     }
      if (digitalRead(BUTTON_DOWN) == HIGH){
        if (menuChoise > 1)
         --menuChoise;
        drawMenu(menuChoise);
      }
     delay(5);
    }
  if (menuChoise ==1){
     backToMenu = false;
     bool backToGame = true;
     display.clearDisplay();
     while(backToGame == true){
      display.clearDisplay();
      backToGame = false;
      newGame;

      while((digitalRead(BUTTON_G) == LOW)&& (digitalRead(BUTTON_LEFT) == LOW));
      if (digitalRead(BUTTON_G) == HIGH)
        backToGame = true;
      else if (digitalRead(BUTTON_LEFT) == HIGH)
        backToMenu = true;
     }
   }
  else if (menuChoise == 2){
      drawRecords;
      while (digitalRead(BUTTON_LEFT) == LOW);
      if (digitalRead(BUTTON_LEFT) == HIGH)
        backToMenu = true;
    }
  else if (menuChoise ==3){
      drawSettings(speedGame);
      while (digitalRead(BUTTON_LEFT) == LOW){
        if ((digitalRead(BUTTON_DOWN) == HIGH) and (speedGame <4)) {
          ++speedGame;
          drawSettings(speedGame);
        }
        if ((digitalRead(BUTTON_UP) == HIGH) and (speedGame >0)) {
          speedGame -=1;
          drawSettings(speedGame);
        }
      }
      if (digitalRead(BUTTON_LEFT) == HIGH)
        backToMenu = true;
   }
   return(backToMenu);
}
void newGame(){
  int level = 1;
  int score = 0;
  BasicInterface();
  display.setCursor(55,8);
  display.println(score);
  display.setCursor(55, 24);
  display.println(record[0]);
  display.setCursor(55,76);
  display.println(level);
  while(digitalRead(BUTTON_G) == LOW);
  bool coincidence = true;
  while ( coincidence == true){
    int Blocks[3];
    for (int i = 0; i < 3; ++i)
      Blocks[i] = rand()%3;
      for(int j = 0; j<3; ++j)
        drawBlock(j);
      for( int j = 0; j < 3; ++j)
        drawBlocks1(0, j, Blocks[j]);
      ChangeBlocks(level);
      display.fillRect(7,6,38,9,WHITE);
      for( int j = 0; j < 3; ++j)
        drawBlocks2(0, j, Blocks[j]);
      for( int j = 0; j < 3; ++j)
        drawBlocks1(1, j, Blocks[j]);
      ChangeBlocks(level);
      for( int k = 2; k < 15; ++k){
        display.fillRect(7,6 + 4*(k-2),38,9 +4&(k-2),WHITE);
        for(int j = 0; j < 3; ++j)
          drawBlocks2(k-1, j, Blocks[j]);
        for(int j = 0; j < 3; ++j)
          drawBlocks1(k, j, Blocks[j]);
        ChangeBlocks(level);
      }
    if ((station[0] == Blocks[0]) and (station[1]==Blocks[0]) and (station[2]==Blocks[2]))
      coincidence = true;
    else
      coincidence = false;
    score +=10;
    display.fillRect(55,8, 83, 14, WHITE);
    display.setCursor(55,8);
    display.println(score);
    if (score > record[0]){
      display.fillRect(55,25, 83, 31, WHITE);
      display.setCursor(55,25);
      display.println(score);
    }
    changeRecords(score); 
    if ((score % 12 == 0) and (level <=5)){
      ++level;
      display.fillRect(55, 76, 59, 82, WHITE);
      display.setCursor(55,76);
      display.println(level);
    }
  }
  display.clearDisplay();
  display.setCursor(0, 34);
  display.println("DO YOU WANT TO CONTINUE?");
}

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_UP, INPUT);  //режим чтения
  digitalWrite(BUTTON_UP, HIGH);  //включаем подтягивающий резистор
  pinMode(BUTTON_RIGHT, INPUT);
  digitalWrite(BUTTON_RIGHT, HIGH);
  pinMode(BUTTON_DOWN, INPUT);
  digitalWrite(BUTTON_DOWN, HIGH);
  pinMode(BUTTON_LEFT, INPUT);
  digitalWrite(BUTTON_LEFT, HIGH);
  pinMode(BUTTON_G, INPUT);
  digitalWrite(BUTTON_G, HIGH);

  display.begin();
  display.clearDisplay();
  display.display();
  display.setContrast(60);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  delay(1000);

  bool backToMenu = true;
  while (backToMenu == true)
    menu(backToMenu);
  
}


void loop() {
}
