#include <EEPROM.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1
#define BUTTON_UP 4  //кнопка вверх
#define BUTTON_RIGHT 3  //кнопка вправо
#define BUTTON_DOWN 2  //кнопка вниз
#define BUTTON_LEFT 5  //кнопка влево
#define BUTTON_G 8  //кнопка при надавливании на джойстик

Adafruit_PCD8544 display = Adafruit_PCD8544(9, 10, 11, 12, 13);


int record[5] = {0,0,0,0,0};
int speedGame = 0;
int station[3] = {0,0,0};


void changeRecords(int score){   
  for(int i = 0; i < 5; ++i){
    if (score > record[i]){
      for( int j = 4; j > i+1; --j)
        record[j] = record[j-1];
    record[i] = score;
  }
  EEPROM.put(i, record[i]);
}
}

void startAnimation(){  //анимация при старте 
  display.setCursor( 31, 20);
  display.println("FALL");
  display.display();
  delay(3000);
}
void BasicInterface()  {   //базовый интерфейс при игре
  display.drawRect(0, 0, 44, 48, BLACK);
   display.setCursor(48,0);
   display.print("SCORE");
   display.setCursor(48,16);
   display.print("RECORD");
   display.setCursor(48,33);
   display.print("LEVEL");
}


void drawBlocks1(int i,int numberblock, int state){   //прорисовка первого рядо движущихся блоков
  if (state ==0)
    display.fillRect(5 + 12*numberblock, 1 + i*4, 4, 4, BLACK);
  else 
    display.fillRect(5 + 12*numberblock, 1 + i*4, 8, 4, BLACK);
}


void drawBlocks2(int i, int numberblock, int state){  //прорисовка второго ряда
  if (state == 2)
    display.fillRect(5 + 12*numberblock, 1 + i*4, 4, 4, BLACK);
  else if (state == 3)
    display.fillRect(5 + 12*numberblock, 1 + i*4, 8, 4, BLACK);
}

void drawBlock(int i){   //прорисовка управляемых блоков
  display.fillRect(5 + (i)*12, 37, 8, 8, WHITE);
  if (station[i] == 0)
    display.fillRect(5 + (i)*12, 41, 4, 4, BLACK);
  else if (station[i] == 1)
    display.fillRect(5 + (i)*12, 41, 8, 4, BLACK);
  else if (station[i] == 2){
    display.fillRect(5 + (i)*12, 37, 4, 4, BLACK);
    display.fillRect(5 + (i)*12, 41, 8, 4, BLACK);
  }
  else if (station[i] == 3)
    display.fillRect(5 + (i)*12, 37, 8, 8, BLACK);
}


int ChangeBlocks(int level){   //управление блоками
  for (int i = 0; i < ((10-speedGame)*(11-level)*35)/300; ++i){
    if (digitalRead(BUTTON_LEFT) == LOW){
      station[0] = (++station[0])%4;
      drawBlock(0);
      display.display();
    }
    if ((digitalRead(BUTTON_UP) == LOW) || (digitalRead(BUTTON_DOWN) == LOW)){
      station[1] = (++station[1])%4;
      drawBlock(1);
      display.display();
    }
    if (digitalRead(BUTTON_RIGHT) == LOW){
      station[2] = (++station[2])%4;
      drawBlock(2);
      display.display();
    }
  delay(150);
  }
  
}

void drawRecords(){  //отдел с рекордами
  display.clearDisplay();
  display.setCursor(3,0);
  display.println("RECORD1 ");
  display.setCursor(47,0);
  display.println(record[0]);
  display.setCursor(3,10);
  display.println("RECORD2 ");
  display.setCursor(47,10);
  display.println(record[1]);
  display.setCursor(3,20);
  display.println("RECORD3 ");
  display.setCursor(47,20);
  display.println(record[2]);
  display.setCursor(3,30);
  display.println("RECORD4 ");
  display.setCursor(47,30);
  display.println(record[3]);
  display.setCursor(3,40);
  display.println("RECORD5 ");
  display.setCursor(47,40);
  display.println(record[4]);
  display.display();
}

void drawSettings(int speedGame){   //отдел с настройками
display.clearDisplay();
display.setCursor(1,20);
display.println("GAMESPEED X1,");
display.setCursor(76,20);
display.println(speedGame);
}
void drawMenu (int choise){  //прорисовка меню
  display.clearDisplay();
  display.setCursor(27,4);
  display.println("START");
  display.setCursor(18,20);
  display.println("SETTINGS");
  display.setCursor(20,36);
  display.println("RECORDS");

  if (choise == 1){
     display.drawRect(12, 1, 60, 13, BLACK);
  }
  else{
     display.drawRect(14, 3, 56, 11, BLACK);
}
  if (choise == 2){
     display.drawRect(12, 16, 60, 13, BLACK);
  }
  else{
     display.drawRect(14, 18, 56, 11, BLACK);
  }
  if (choise == 3){
     display.drawRect(12, 32, 60, 13, BLACK);
  }
  else {
     display.drawRect(14, 34, 56, 11, BLACK);
  }
}



bool menu(bool backToMenu){ //вызов меню и дальнешая работа с ним
   backToMenu = false;
   display.clearDisplay();
   int menuChoise =1;
   drawMenu(menuChoise);
   display.display();
   while(digitalRead(BUTTON_G) == HIGH){  //выбор раздела меню
     if (digitalRead(BUTTON_UP) == LOW){
        if (menuChoise < 3)
          ++menuChoise;
        drawMenu(menuChoise);
        display.display();
     }
     else if (digitalRead(BUTTON_DOWN) == LOW){
        if (menuChoise > 1)
         --menuChoise;
        drawMenu(menuChoise);
        display.display();

      }
     delay(200);
    }
  if (menuChoise ==1){  //начало игры
     backToMenu = false;
     bool backToGame = true;
     display.clearDisplay();
     while(backToGame == true){
      display.clearDisplay();
      backToGame = false;
      newGame();

      while((digitalRead(BUTTON_G) == HIGH)&& (digitalRead(BUTTON_LEFT) == HIGH));  //выбираем хотим ли мы продолжить игру или вернуться в меню
      if (digitalRead(BUTTON_G) == LOW)
        backToGame = true;
      else if (digitalRead(BUTTON_LEFT) == LOW)
        backToMenu = true;
     }
   }
  else if (menuChoise == 3){  //вызываем окно с рекордами
      drawRecords();
      display.display();

      while (digitalRead(BUTTON_LEFT) == HIGH);
      if (digitalRead(BUTTON_LEFT) == LOW)
        backToMenu = true;
    }
  else if (menuChoise ==2){  //вызов окна скорости и изменение ее
      drawSettings(speedGame);
         display.display();

      while (digitalRead(BUTTON_LEFT) == HIGH){
        if ((digitalRead(BUTTON_DOWN) == LOW) and (speedGame <5)) {
          ++speedGame;
          drawSettings(speedGame);
             display.display();

        }
        if ((digitalRead(BUTTON_UP) == LOW) and (speedGame >0)) {
          speedGame -=1;
          drawSettings(speedGame);
             display.display();

        }
        delay(200);
      }
      if (digitalRead(BUTTON_LEFT) == HIGH)
        backToMenu = true;
   }
   return(backToMenu);
}
void newGame(){
  int level = 1;
  int score = 0;
  BasicInterface();   //рисуем интерфейс 
  display.display();
  display.setCursor(48,8);
  display.println(score);
  display.setCursor(48, 24);
  display.println(record[0]);
  display.setCursor(48,41);
  display.println(level);
  display.display();
  while(digitalRead(BUTTON_G) == HIGH)  
    delay(120);
  bool coincidence = true;  //начало игры
  while ( coincidence == true){
    int Blocks[3];
    for (int i = 0; i < 3; ++i)  //прорисовываем первый ряд и задаем двигующиеся блоки 
      Blocks[i] = rand()%4;
      for(int j = 0; j<3; ++j)
        drawBlock(j);
        display.display();
      for( int j = 0; j < 3; ++j)
        drawBlocks1(0, j, Blocks[j]);
        display.display();
      ChangeBlocks(level);
      display.display();  
      display.fillRect(5,1,32,4,WHITE);   //прорисовываем 1 и 2 ряд
      for( int j = 0; j < 3; ++j)
        drawBlocks2(0, j, Blocks[j]);
        display.display();
      for( int j = 0; j < 3; ++j)
        drawBlocks1(1, j, Blocks[j]);
        display.display();
      ChangeBlocks(level);
      for( int k = 2; k < 9; ++k){   //идет само движение блоков 
        display.fillRect(5,1 + 4*(k-2), 32, 8,WHITE);
        display.display();
        for(int j = 0; j < 3; ++j)
          drawBlocks2(k-1, j, Blocks[j]);
          display.display();
        for(int j = 0; j < 3; ++j)
          drawBlocks1(k, j, Blocks[j]);
          display.display();
        ChangeBlocks(level);
        display.display();
      }
    display.fillRect(5, 29, 32, 8, WHITE);
    display.display();
    if ((station[0] == Blocks[0]) and (station[1]==Blocks[1]) and (station[2]==Blocks[2]))   // проверка на совпадение 
      coincidence = true;
    else
      coincidence = false;
    if (coincidence == true)
      score +=10;
    display.fillRect(48,8, 24, 7, WHITE); // меняем рекорд и счет
    display.setCursor(48,8);
    display.println(score);
    display.display();
    if (score > record[0]){
      display.fillRect(48,24, 24, 8, WHITE);
      display.setCursor(48,25);
      display.println(score);
      display.display();
    }
    changeRecords(score); 
    if ((score % 12 == 0) and (level <=5)){
      ++level;
      display.fillRect(48, 41, 5, 8, WHITE);
      display.setCursor(48,41);
      display.println(level);
      display.display();
    }
  }
  for(int j =0; j < 3; ++j) 
    station[j] = 0;
  display.clearDisplay();
  display.setCursor(0, 17);
  display.println("DO YOU WANT TO CONTINUE?");
  display.display();
}

void setup() {
  Serial.begin(9600);

  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);

  pinMode(BUTTON_UP, INPUT);
  digitalWrite(BUTTON_UP, HIGH); 
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
  startAnimation();  
  bool backToMenu = true;
  while(backToMenu == true)
    menu(backToMenu);
  
}


void loop() {
}
