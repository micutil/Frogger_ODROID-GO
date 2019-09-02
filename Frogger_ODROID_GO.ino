//Frogger ODROID-GO
//By Pappani Federico
//federicopappani.it

/******************************************************************************/
//Modified by Micutil
//Added launcher code for SD-Update / LovyanLauncher.
//Added binary for Odroid-GO (using ESP32-Chimera-Core).
/******************************************************************************/

//libraries
#include <M5Stack.h>
#include "M5StackUpdater.h"

//#include <odroid_go.h> //comment out by micono
//#include <EEPROM.h>
//Screen size is 320x240

//colors
const unsigned int FROGCOL = 1216; //0x04C0
const unsigned int WATER = 32311; //0x7E37
const unsigned int WATER2 = 36863; //0x8FFF
const unsigned int ROSETEST = 64543; //0xFC1F
const unsigned int BROWN = 33280; //0x8200
const unsigned int STREET = 12710; //0x31A6
const unsigned int MLINE = 48631; //0xBDF7
const unsigned int TREE1 = 39529; //0x6168
const unsigned int TREE2 = 7380; //0x1CD4
const unsigned int ROCKS = 29120; //0x71C0
const unsigned int GRASS = 709; //0x02C5

//positions & variables
int frog_x = 155;
int frog_y = 220;
int delta = 0;

int car1_x = 0;
int car1_y = 139;
int car2_x = 320;
int car2_y = 156;
int car3_x = 80;
int car3_y = 172;
int car4_x = 320;
int car4_y = 187;
int car5_x = 40;
int car5_y = 203;

int tree1_x = 350;
int tree1_y = 42;
int tree2_x = 0;
int tree2_y = 58;
int tree3_x = 100;
int tree3_y = 76;
int tree4_x = 240;
int tree4_y = 90;
int tree5_x = 180;
int tree5_y = 108;

uint8_t movement = 0;
uint8_t lifes = 3;
int score = 0;
uint8_t screenBl = 150;
uint8_t clipped = 0;
bool volume = false;
int highestScore = 0;
bool pressed = false;

void setup(){
  //SD.begin();
  //File save = SD.open("/frogger_score.txt");
  //if (!save) SD.("/frogger_score.txt", "0");
  //highestScore = save.read().toInt();

  //comment out by micono
  //Serial.begin(115200);
  //Serial.println("Frogger ODROID-GO | federicopappani.it");
  
  //Serial.print("Highest score: ");
  //Serial.print(highestScore);
  //pinMode(25, OUTPUT);
  //pinMode(26, OUTPUT);
  //digitalWrite(25, HIGH);
  //digitalWrite(26, HIGH);

  M5.begin();
  Wire.begin();
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }
  
  //GO.begin();
  //GO.Speaker.setVolume(0);
}

void loop(){
  intro();
  gameloop();
  gameover();
  }

void gameloop(){
  drawFixed();
  while(1){
  pressed = false;
  M5.update();
  checkMovements();
  printBg();
  printTrees();
  printFrog();
  if (!pressed) checkMovements();
  moveObjects();
  printCars();
  checkCollisions();
  if (!pressed) checkMovements();
  moveIfClipped();
  if (lifes > 0){
    M5.lcd.setTextSize(1);
    M5.lcd.setCursor(310, 5);
    M5.lcd.print(lifes);
  }
  else {break;}
  delay(30);
  if (!pressed) checkMovements();
  }
  }
  
void drawFixed(){
  M5.lcd.setBrightness(screenBl);
  M5.lcd.fillScreen(BLACK);
  //M5.lcd.drawJpgFile(SD, "/top_bg.jpg", 0);
  M5.lcd.setTextColor(WHITE);
  M5.lcd.setTextSize(2);
  M5.lcd.setCursor(2, 0);
  M5.lcd.print("Frogger ODROID-GO ");
  M5.lcd.drawFastVLine(211, 0, 17, WHITE);
  M5.lcd.setCursor(1, 232);
  M5.lcd.fillRect(0, 230, 320, 240, BLACK);
  printScore();
  //M5.lcd.print("federicopappani.it - 2019");
  }

void checkCollisions(){
  if (((frog_x >= car1_x) && (frog_x <= (car1_x + 20)) && (frog_y >= car1_y) && (frog_y <= (car1_y + 10))) ||
     ((frog_x >= car2_x) && (frog_x <= (car2_x + 20)) && (frog_y >= car2_y) && (frog_y <= (car2_y + 10))) ||
     ((frog_x >= car3_x) && (frog_x <= (car3_x + 20)) && (frog_y >= car3_y) && (frog_y <= (car3_y + 10))) ||
     ((frog_x >= car4_x) && (frog_x <= (car4_x + 20)) && (frog_y >= car4_y) && (frog_y <= (car4_y + 10))) ||
     ((frog_x >= car5_x) && (frog_x <= (car5_x + 20)) && (frog_y >= car5_y) && (frog_y <= (car5_y + 10)))) dead();
  if ((frog_x >= (tree1_x - 8)) && (frog_x <= (tree1_x + 55)) && (frog_y >= tree1_y) && (frog_y <= (tree1_y + 10)) && (clipped != 1)){delta = frog_x - tree1_x; clipped = 1;}
  if ((frog_x >= (tree2_x - 8)) && (frog_x <= (tree2_x + 70)) && (frog_y >= tree2_y) && (frog_y <= (tree2_y + 10)) && (clipped != 2)){delta = frog_x - tree2_x; clipped = 2;}
  if ((frog_x >= (tree3_x - 8)) && (frog_x <= (tree3_x + 55)) && (frog_y >= tree3_y) && (frog_y <= (tree3_y + 10)) && (clipped != 3)){delta = frog_x - tree3_x; clipped = 3;}
  if ((frog_x >= (tree4_x - 8)) && (frog_x <= (tree4_x + 70)) && (frog_y >= tree4_y) && (frog_y <= (tree4_y + 10)) && (clipped != 4)){delta = frog_x - tree4_x; clipped = 4;}
  if ((frog_x >= (tree5_x - 8)) && (frog_x <= (tree5_x + 55)) && (frog_y >= tree5_y) && (frog_y <= (tree5_y + 10)) && (clipped != 5)){delta = frog_x - tree5_x; clipped = 5;}
  //check collision with river
  if ((frog_y > 40) && (frog_y < 120) && (clipped == 0)) dead();
  if ((frog_y > 17) && (frog_y < 40)) win();
}

void moveIfClipped(){
  if (clipped == 0) return;
  switch(clipped){
    case 1:
      frog_x = tree1_x + delta;
      break;
    case 2:
      frog_x = tree2_x + delta;
      break;
    case 3:
      frog_x = tree3_x + delta;
      break;
    case 4:
      frog_x = tree4_x + delta;
      break;
    case 5:
      frog_x = tree5_x + delta;
      break;
    default:
      break;
    }
  }

void printTrees(){
  M5.lcd.fillRect(tree1_x, tree1_y, 50, 10, TREE1);
  M5.lcd.fillRect(tree2_x, tree2_y, 65, 10, STREET);
  M5.lcd.fillRect(tree3_x, tree3_y, 50, 10, TREE1);
  M5.lcd.fillRect(tree4_x, tree4_y, 65, 10, STREET);
  M5.lcd.fillRect(tree5_x, tree5_y, 50, 10, TREE1);
  }
  
void printBg(){
  //bg blocks
  M5.lcd.fillRect(0, 18, 320, 22, GRASS);
  M5.lcd.fillRect(0, 40, 320, 81, WATER);
  M5.lcd.fillRect(0, 121, 320, 15, BROWN);
  M5.lcd.fillRect(0, 136, 320, 80, STREET);
  M5.lcd.fillRect(0, 216, 320, 15, BROWN);
  M5.lcd.fillCircle(312, 8, 6, RED);
  //road lines
  M5.lcd.drawFastHLine(0, 152, 40, MLINE);
  M5.lcd.drawFastHLine(0, 168, 40, MLINE);
  M5.lcd.drawFastHLine(0, 184, 40, MLINE);
  M5.lcd.drawFastHLine(0, 200, 40, MLINE);
  M5.lcd.drawFastHLine(80, 152, 60, MLINE);
  M5.lcd.drawFastHLine(80, 168, 60, MLINE);
  M5.lcd.drawFastHLine(80, 184, 60, MLINE);
  M5.lcd.drawFastHLine(80, 200, 60, MLINE);
  M5.lcd.drawFastHLine(180, 152, 60, MLINE);
  M5.lcd.drawFastHLine(180, 168, 60, MLINE);
  M5.lcd.drawFastHLine(180, 184, 60, MLINE);
  M5.lcd.drawFastHLine(180, 200, 60, MLINE);
  M5.lcd.drawFastHLine(280, 152, 40, MLINE);
  M5.lcd.drawFastHLine(280, 168, 40, MLINE);
  M5.lcd.drawFastHLine(280, 184, 40, MLINE);
  M5.lcd.drawFastHLine(280, 200, 40, MLINE);
  //rocks
  M5.lcd.fillCircle(31, 223, 3, ROCKS);
  M5.lcd.fillCircle(76, 228, 2, ROCKS);
  M5.lcd.fillCircle(103, 225, 1, ROCKS);
  M5.lcd.fillCircle(150, 223, 2, STREET);
  M5.lcd.fillCircle(204, 224, 2, STREET);
  M5.lcd.fillCircle(273, 226, 3, ROCKS);
  M5.lcd.fillCircle(315, 221, 1, ROCKS);
  M5.lcd.fillCircle(80, 129, 2, STREET);
  M5.lcd.fillCircle(31, 131, 3, ROCKS);
  M5.lcd.fillCircle(171, 130, 1, ROCKS);
  M5.lcd.fillCircle(238, 125, 3, ROCKS);
  M5.lcd.fillCircle(312, 131, 1, STREET);
  M5.lcd.fillCircle(118, 123, 2, STREET);
  }
  
void checkMovements(){
  pressed = true;
  if (movement >= 2) movement = 0;
  if ((M5.JOY_Y.isAxisPressed() == 2) && (movement == 0)) {frog_y -= 16; movement ++; clipped = 0; delta = 0;}
  else if ((M5.JOY_Y.isAxisPressed() == 1) && (movement == 0)) {frog_y += 16; movement ++; clipped = 0; delta = 0;}
  else if ((M5.JOY_X.isAxisPressed() == 2) && (movement == 0)) {frog_x -= 16; movement ++;}
  else if ((M5.JOY_X.isAxisPressed() == 1) && (movement == 0)) {frog_x += 16; movement ++;}
  else movement ++;
  if ((frog_x < 0) && (clipped == 0)) {frog_x += 16;}
  if ((frog_x > 300) && (clipped == 0)) {frog_x -= 16;}
  if ((frog_y > 233) && (clipped == 0)) {frog_y -= 16;}
  if ((frog_y < 17) && (clipped == 0)) {frog_y += 16;}
  if (M5.BtnStart.isPressed() == 1 && screenBl < 175){
    screenBl += 50;
    M5.lcd.setBrightness(screenBl);
    }
  else if (M5.BtnSelect.isPressed() == 1 && screenBl > 75){
    screenBl -= 50;
    M5.lcd.setBrightness(screenBl);
  }
  //if (M5.BtnVolume.isPressed() == 1) if (!volume) {M5.Speaker.setVolume(3); volume = true;} else {M5.Speaker.setVolume(0); volume = false;}
    
  }
  
void printFrog(){
  M5.lcd.fillRect(frog_x, frog_y, 13, 10, FROGCOL);
  //eyes
  M5.lcd.fillRect(frog_x, frog_y, 3, 3, RED);
  M5.lcd.fillRect(frog_x + 10, frog_y, 3, 3, RED);
  //legs
  M5.lcd.fillRect(frog_x, frog_y + 7, 4, 1, BLACK);
  M5.lcd.fillRect(frog_x + 9, frog_y + 7, 4, 1, BLACK);
  }

void printScore(){
  M5.lcd.fillRect(216, 0, 65, 20, BLACK);
  M5.lcd.setCursor(217, 0);
  M5.lcd.setTextSize(2);
  M5.lcd.setTextColor(WHITE);
  M5.lcd.print(score);
  }
  
void moveObjects(){
  car1_x = (car1_x + 15) % 350;
  if (car2_x < -40) car2_x += 350;
  car2_x = car2_x - 14;
  car3_x = (car3_x + 11) % 350;
  if (car4_x < -40) car4_x += 350;
  car4_x = car4_x - 15;
  car5_x = (car5_x + 13) % 350;
  if (tree1_x < -30) tree1_x += 350;
  tree1_x = tree1_x - 7;
  tree2_x = (tree2_x + 10) % 350;
  if (tree3_x < -30) tree3_x += 350;
  tree3_x = tree3_x - 8;
  tree4_x = (tree4_x + 9) % 350;
  if (tree5_x < -30) tree5_x += 350;
  tree5_x = tree5_x - 8;
  }

void printCars(){
  //car1
  M5.lcd.fillRect(car1_x, car1_y, 20, 10, RED);
  M5.lcd.fillRect(car1_x + 18, car1_y, 2, 2, YELLOW);
  M5.lcd.fillRect(car1_x + 18, car1_y + 8, 2, 2, YELLOW);
  //car2
  M5.lcd.fillRect(car2_x, car2_y, 20, 10, WHITE);
  M5.lcd.fillRect(car2_x, car2_y, 2, 2, ORANGE);
  M5.lcd.fillRect(car2_x, car2_y + 8, 2, 2, ORANGE);
  M5.lcd.drawFastHLine(car2_x + 4, car2_y + 5, 9, MLINE);
  M5.lcd.drawFastVLine(car2_x + 4, car2_y, 10, BLACK);
  //car3
  M5.lcd.fillRect(car3_x, car3_y, 20, 10, BLUE);
  M5.lcd.fillRect(car3_x + 18, car3_y, 2, 2, YELLOW);
  M5.lcd.fillRect(car3_x + 18, car3_y + 8, 2, 2, YELLOW);
  //car4
  M5.lcd.fillRect(car4_x, car4_y, 20, 10, RED);
  M5.lcd.fillRect(car4_x, car4_y, 2, 2, YELLOW);
  M5.lcd.fillRect(car4_x, car4_y + 8, 2, 2, YELLOW);
  //car5
  M5.lcd.fillRect(car5_x, car5_y, 20, 10, BLUE);
  M5.lcd.fillRect(car5_x + 18, car5_y, 2, 2, YELLOW);
  M5.lcd.fillRect(car5_x + 18, car5_y + 8, 2, 2, YELLOW);
}

void intro(){
  //M5.lcd.setRotation(1); //Comment out by micono
  Serial.println("Frogger by Pappani Federico");
  M5.lcd.setTextColor(WHITE);
  //M5.lcd.fillCircle(160, 50, 30, ROSETEST);
  M5.lcd.setCursor(100, 50);
  M5.lcd.setTextColor(GREEN);
  M5.lcd.setTextSize(3);
  M5.lcd.print("Frogger");
  M5.lcd.setCursor(57, 100);
  M5.lcd.setTextSize(2);
  M5.lcd.setTextColor(WHITE);
  #ifdef ARDUINO_ODROID_ESP32
  M5.lcd.print("ODROID-GO Version");
  #else
  M5.lcd.print("M5Stack Version");
  #endif
  M5.lcd.setCursor(63, 130);
  M5.lcd.setTextColor(RED);
  M5.lcd.print("Press A to start");

  M5.lcd.setTextSize(1);
  M5.lcd.setTextColor(WHITE);
  M5.lcd.setCursor(83, 180);
  M5.lcd.print("federicopappani.it - 2019");

  M5.lcd.setTextSize(2);
  M5.lcd.setTextColor(TFT_CYAN);
  M5.lcd.setCursor(50, 190);
  M5.lcd.print("Modified by micutil");

//delay(3000);
  while(1){
    if(M5.BtnA.isPressed() == 1) break;
    M5.update();
    }
  M5.update();
  }

void gameover(){
  M5.lcd.fillScreen(BLACK);//M5.lcd.fillScreen(BLACK);
  M5.lcd.setTextSize(3);
  M5.lcd.setCursor(75, 50);
  M5.lcd.setTextColor(RED);
  M5.lcd.print("Game Over!\n\n    Score: ");
  M5.lcd.print(score);
  //if (score > highestScore){
  //  save.write("frogger_score.txt", score);
  //  Serial.println("Score updated!");
  //  highestScore = score;
  //  }
  delay(3000);
  delta = 0;
  clipped = false;
  M5.lcd.fillScreen(BLACK);
  score = 0;
  dead();
  lifes = 3;
  }

void win(){
  score += 100;
  //M5.Speaker.tone(700, 30);
  //M5.Speaker.tone(1050, 50);
  delay(500);
  frog_x = 155;
  frog_y = 220;
  printScore();
  }
  
void dead(){
  if (lifes > 0) lifes --;
  frog_x = 155;
  frog_y = 220;
  }
