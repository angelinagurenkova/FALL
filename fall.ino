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


int record[3] = {0,0,0};
int speedGame = 0;
int station[3] = {0,0,0};
int scale = 1;
int contrast = 55;
String nameRec0 = "AAA";
String nameRec1 = "AAA";
String nameRec2 = "AAA";

void savingsettings()  {
      scale = 1;
      contrast = 55;
      speedGame = 0;

      nameRec0 = "ZZZ";
      record[0] = 0;

      nameRec1 = "ZZZ";
      record[1] = 0;

      nameRec2 = "ZZZ";
      record[2] = 0;
      
      if (EEPROM.read(0) >= 1 && EEPROM.read(0) <= 3)
        EEPROM.get(0,scale);
      if (EEPROM.read(9) >= 0 && EEPROM.read(9) <= 3)
        EEPROM.get(9,speedGame);
      if (EEPROM.read(18) >= 0 && EEPROM.read(18) <= 3)
       EEPROM.get(18,contrast);
        
        int count = 0;
        for (int i = 0; i < 3; ++i)
          if(EEPROM.read(237 + i*9) >= 'A' && EEPROM.read(237 + i*9) <= 'Z')
            count++;
            
        for (int i = 0; i < 3; ++i)
        if(count == 3)
          EEPROM.get(237 + i*9, nameRec0[i]);

        count = 0;
        for (int i = 0; i < 3; ++i)
          if(EEPROM.read(362 + i*9) >= 'A' && EEPROM.read(362 + i*9) <= 'Z')
            count++;
          
        for (int i = 0; i < 3; ++i)
        if(count == 3)
          EEPROM.get(362 + i*9, nameRec1[i]);

        count = 0;
        for (int i = 0; i < 3; ++i)
          if(EEPROM.read(487 + i*9) >= 'A' && EEPROM.read(487 + i*9) <= 'Z')
            count++;
          
        for (int i = 0; i < 3; ++i)
        if(count == 3)
          EEPROM.get(487 + i*9, nameRec2[i]);
        
      if(EEPROM.read(111) >= 0 && EEPROM.read(111) <= 32000)
        EEPROM.get(111, record[0]);
      if(EEPROM.read(128) >= 0 && EEPROM.read(128) <= 32000)
        EEPROM.get(128, record[1]);
      if(EEPROM.read(145) >= 0 && EEPROM.read(145) <= 32000)
        EEPROM.get(145, record[2]);
    }

void changeRecords(int score, String nameN){   
  if (score>record[0]){
    record[2]=record[1];
    nameRec2 = nameRec1;
    record[1]=record[0];
    nameRec1 = nameRec0;
    record[0]=score;
    nameRec0 = nameN;
  }
  else if (score>record[1]){
    record[2]=record[1];
    nameRec2 = nameRec1;
    record[1]=score;
    nameRec1 = nameN;
  }
  else if (score>record[2]){
    record[2]=score;
    nameRec2 = nameN;
}
for (int i = 0; i < 3; ++i)
        EEPROM.write(237 + i*9, nameRec0[i]);
      for (int i = 0; i < 3; ++i)
        EEPROM.write(362 + i*9, nameRec1[i]);
      for (int i = 0; i < 3; ++i)
        EEPROM.write(487 + i*9, nameRec2[i]);
      EEPROM.put(111, record[0]);
      EEPROM.put(128, record[1]);
      EEPROM.put(145, record[2]);
}



void startAnimation(){  //анимация при старте 
  for(int i = 1; i <=20; ++i){
    display.fillRect(31*scale, (i-1)*scale, 5*scale, 7*scale, WHITE);
    display.setCursor( 31*scale, i*scale);
    display.println("F");
    display.display();
    delay(70);
  }
  for(int i = 1; i <=20; ++i){
    display.fillRect(37*scale, (i-1)*scale, 5*scale, 7*scale, WHITE);
    display.setCursor( 37*scale, i*scale);
    display.println("A");
    display.display();
    delay(70);
  }
  for(int i = 1; i <=20; ++i){
    display.fillRect(43*scale, (i-1)*scale, 5*scale, 7*scale, WHITE);
    display.setCursor( 43*scale, i*scale);
    display.println("L");
    display.display();
    delay(70);
  }
  for(int i = 1; i <=20; ++i){
    display.fillRect(49*scale, (i-1)*scale, 5*scale, 7*scale, WHITE);
    display.setCursor( 49*scale, i*scale);
    display.println("L");
    display.display();
    delay(70);
  }
}
void BasicInterface()  {   //базовый интерфейс при игре
  display.drawRect(0, 0, 44*scale, 48*scale, BLACK);
   display.setCursor(48*scale,0);
   display.print("SCORE");
   display.setCursor(48*scale,16*scale);
   display.print("RECORD");
   display.setCursor(48*scale,33*scale);
   display.print("LEVEL");
}


void drawBlocks1(int i,int numberblock, int state){   //прорисовка первого рядо движущихся блоков
  if (state ==0)
    display.fillRect(5*scale + 12*numberblock*scale, 1*scale + i*4*scale, 4*scale, 4*scale, BLACK);
  else 
    display.fillRect(5*scale + 12*numberblock*scale, 1*scale + i*4*scale, 8*scale, 4*scale, BLACK);
}


void drawBlocks2(int i, int numberblock, int state){  //прорисовка второго ряда
  if (state == 2)
    display.fillRect(5*scale + 12*numberblock*scale, 1*scale + i*4*scale, 4*scale, 4*scale, BLACK);
  else if (state == 3)
    display.fillRect(5*scale + 12*numberblock*scale, 1*scale + i*4*scale, 8*scale, 4*scale, BLACK);
}

void drawBlock(int i){   //прорисовка управляемых блоков
  display.fillRect(5*scale + (i)*scale*12, 37*scale, 8*scale, 8*scale, WHITE);
  if (station[i] == 0)
    display.fillRect(5*scale + (i)*12*scale, 41*scale, 4*scale, 4*scale, BLACK);
  else if (station[i] == 1)
    display.fillRect(5*scale + (i)*12*scale, 41*scale, 8*scale, 4*scale, BLACK);
  else if (station[i] == 2){
    display.fillRect(5*scale + (i)*12*scale, 37*scale, 4*scale, 4*scale, BLACK);
    display.fillRect(5*scale + (i)*12*scale, 41*scale, 8*scale, 4*scale, BLACK);
  }
  else if (station[i] == 3)
    display.fillRect(5*scale + (i)*12*scale, 37*scale, 8*scale, 8*scale, BLACK);
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
  display.setCursor(3*scale,10*scale);
  display.println(nameRec0);
  display.setCursor(47*scale,10*scale);
  display.println(record[0]);
  display.setCursor(3*scale,20*scale);
  display.println(nameRec1);
  display.setCursor(47*scale,20*scale);
  display.println(record[1]);
  display.setCursor(3*scale,30*scale);
  display.println(nameRec2);
  display.setCursor(47*scale,30*scale);
  display.println(record[2]);
  display.display();
}

void drawSettings(int speedGame){   //отдел с настройками
display.clearDisplay();
display.setCursor(6*scale,20*scale);
display.println("GAMESPEED 1,");
display.setCursor(77*scale,20*scale);
display.println(speedGame);
display.setCursor(6*scale, 11*scale);
display.println("SCALE      x");
display.setCursor(6*scale, 29*scale);
display.println("CONTRAST");
display.setCursor(77*scale,11*scale);
display.println(scale);
display.setCursor(71*scale,29*scale);
display.println(contrast);
}
void drawMenu (int choise){  //прорисовка меню
  display.clearDisplay();
  display.setCursor(27*scale,4*scale);
  display.println("START");
  display.setCursor(18*scale,20*scale);
  display.println("SETTINGS");
  display.setCursor(20*scale,36*scale);
  display.println("RECORDS");

  if (choise == 1){
     display.drawRect(12*scale, 1*scale, 60*scale, 13*scale, BLACK);
  }
  else{
     display.drawRect(14*scale, 3*scale, 56*scale, 11*scale, BLACK);
}
  if (choise == 2){
     display.drawRect(12*scale, 16*scale, 60*scale, 13*scale, BLACK);
  }
  else{
     display.drawRect(14*scale, 18*scale, 56*scale, 11*scale, BLACK);
  }
  if (choise == 3){
     display.drawRect(12*scale, 32*scale, 60*scale, 13*scale, BLACK);
  }
  else {
     display.drawRect(14*scale, 34*scale, 56*scale, 11*scale, BLACK);
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
      int choise = 0;
      display.fillRect(2*scale, 13*scale + 9*choise*scale, 3*scale,3*scale, BLACK);
          display.display();
      while (digitalRead(BUTTON_LEFT) == HIGH){
        display.fillRect(2*scale, 13*scale + 9*choise*scale, 3*scale,3*scale, BLACK);
          display.display();
        if (digitalRead(BUTTON_RIGHT) == LOW){
          display.fillRect(2*scale, 13*scale + 9*choise*scale, 3*scale,3*scale, WHITE);
          choise = (choise+1)%3;
          display.fillRect(2*scale, 13*scale + 9*choise*scale, 3*scale,3*scale, BLACK);
          display.display();
        }
        
        if (choise == 0){
          if ((digitalRead(BUTTON_DOWN) == LOW) and (scale <4)) {
          ++scale;
          EEPROM.put(0,scale);
          display.setTextSize(scale);
          drawSettings(speedGame);
             display.display();
          }
          if ((digitalRead(BUTTON_UP) == LOW) and (scale >1)) {
           scale -=1;
           EEPROM.put(0,scale);
           display.setTextSize(scale);
           drawSettings(speedGame);
             display.display();
          }
        }
        
        if (choise == 1){
          if ((digitalRead(BUTTON_DOWN) == LOW) and (speedGame < 9)) {
          ++speedGame;
          EEPROM.put(9,speedGame);
          drawSettings(speedGame);
             display.display();

          }
          if ((digitalRead(BUTTON_UP) == LOW) and (speedGame >0)) {
           speedGame -=1;
           EEPROM.put(9,speedGame);
           drawSettings(speedGame);
             display.display();
          }
        }
        
        if (choise == 2){
          if ((digitalRead(BUTTON_DOWN) == LOW) and (contrast <80)) {
          contrast+=5;
          EEPROM.put(18,contrast);
          drawSettings(speedGame);
             display.display();
             display.setContrast(contrast);
          }
          if ((digitalRead(BUTTON_UP) == LOW) and (contrast >45)) {
           contrast -=5;
           EEPROM.put(18,contrast);
           drawSettings(speedGame);
             display.display();
             display.setContrast(contrast);
          }
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
  display.setCursor(48*scale,8*scale);
  display.println(score);
  display.setCursor(48*scale, 24*scale);
  display.println(record[0]);
  display.setCursor(48*scale,41*scale);
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
        display.fillRect(5*scale,1*scale + 4*(k-2)*scale, 32*scale, 8*scale,WHITE);
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
    display.fillRect(5*scale, 29*scale, 32*scale, 8*scale, WHITE);
    display.display();
    if ((station[0] == Blocks[0]) and (station[1]==Blocks[1]) and (station[2]==Blocks[2]))   // проверка на совпадение 
      coincidence = true;
    else
      coincidence = false;
    if (coincidence == true)
      score +=10;
    display.fillRect(48*scale,8*scale, 24*scale, 7*scale, WHITE); // меняем рекорд и счет
    display.setCursor(48*scale,8*scale);
    display.println(score);
    display.display();
    if (score > record[0]){
      display.fillRect(48*scale,24*scale, 24*scale, 8*scale, WHITE);
      display.setCursor(48*scale,25*scale);
      display.println(score);
      display.display();
    }
    if ((score % 12 == 0) and (level <=5)){
      ++level;
      display.fillRect(48*scale, 41*scale, 5*scale, 8*scale, WHITE);
      display.setCursor(48*scale,41*scale);
      display.println(level);
      display.display();
    }
  }
  for(int j =0; j < 3; ++j) 
    station[j] = 0;
  display.clearDisplay();
//  drawName("AAA",1);
String _name = "AAA";
          uint8_t _chosenChar = 0;
  while(digitalRead(BUTTON_G) == HIGH){
          {
            //Ввод имени игрока
            if(digitalRead(BUTTON_LEFT)==LOW)
              if(_chosenChar == 0)
                _chosenChar = 2;
                else _chosenChar -= 1;

            if(digitalRead(BUTTON_RIGHT)==LOW)
              if(_chosenChar == 2)
                _chosenChar = 0;
                else _chosenChar += 1;
                
            if(digitalRead(BUTTON_DOWN)==LOW)
              if(_name[_chosenChar] == 'A')
                _name[_chosenChar] = 'Z';
                else _name[_chosenChar] -= 1;

            if(digitalRead(BUTTON_UP)==LOW)
              if(_name[_chosenChar] == 'Z')
                _name[_chosenChar] = 'A';
                else _name[_chosenChar] += 1;
 
            display.clearDisplay(); 
            display.setCursor(0,20*scale);
            display.print("Enter Name:");
            display.setCursor(64*scale,20*scale);
            display.print(_name);
           
            display.display();
  }
    delay(200);
  }
  
  changeRecords(score, _name); 
  delay(200);
  display.clearDisplay();
  display.setCursor(0, 17*scale);
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
  display.setContrast(contrast);
  display.setTextSize(scale);
  display.setTextColor(BLACK);
  delay(1000);
  startAnimation();  
  savingsettings();
  bool backToMenu = true;
  while(backToMenu == true)
    menu(backToMenu);
  
}


void loop() {
}
