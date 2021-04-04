//==========================================//
// TEXT ADVENTURE GAME                      //
// Computer Architecture Project Fall 2020  //
// By Trevor Duquette                       //
//==========================================//

// libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <pitches.h>

// keypad set-up
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5,4,3,2};
byte colPins[COLS] = {9,8,7,6};
Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

// display
LiquidCrystal_I2C lcd(0x27,20,4);

// game variables
#define Password_Length 5
int stage = 0;
int playerChoice = 0;
int hasBoards = 0;
int hasVine = 0;
int hasVase = 0;
int hasCode = 0;

// keypad password
char Data[Password_Length];
char Master[Password_Length] = "0123";
byte data_count = 0;
byte master_count = 0;

// sounds
const int buzzer = 10;
void neutralSound() {
    tone(buzzer,500);
    delay(250);
    noTone(buzzer);
}
void happySound() {
    tone(buzzer,1000);
    delay(150);
    noTone(buzzer);
    tone(buzzer,1000);
    delay(150);
    noTone(buzzer);
}
//void sadSound() {
//    tone(buzzer,250);
//    delay(500);
//    noTone(buzzer);
//}
void ominousSound() {
    tone(buzzer,250);
    delay(300);
    noTone(buzzer);
    tone(buzzer,150);
    delay(300);
    noTone(buzzer);
    tone(buzzer,50);
    delay(300);
    noTone(buzzer);
}

//                |1                  |2                  |3                  |4
//                --------------------------------------------------------------------------------
//String title01 = "=====================  Text Adventure  ==By Trevor Duquette=====================";

// game text

String title01 = "====================";
String title02 = "=  Text Adventure  =";
String title03 = "=By Trevor Duquette=";
String title04 = "====================";

// bridge text

String bridge01 = "After many long days";
String bridge02 = "of travel, you have";
String bridge03 = "arrived to save the";
String bridge04 = "kidnapped princess!";

String bridge11 = "There is a rickety";
String bridge12 = "bridge in front of";
String bridge13 = "you, under which is";
String bridge14 = "a river of lava.";

String bridgeOptions01 = "You feel the heat.";
String bridgeOptions02 = "A: Look again.";
String bridgeOptions03 = "B: Cross the bridge!";
String bridgeOptions04 = "C: Check the woods.";

String cabin01 = "A forest hugs the ";
String cabin02 = "ravine's edge. You";
String cabin03 = "spot a sturdy little";
String cabin04 = "wood cabin nearby.";

String cabinOptions01 = "It looks abandoned.";
String cabinOptions02 = "A: Look again.";
String cabinOptions03 = "B: Take some boards.";
String cabinOptions04 = "C: Return to bridge.";

//String cabinBack01 = "On your way back you";
//String cabinBack02 = "think you hear a";
//String cabinBack03 = "roar, far away...";
//String cabinBack04 = "The dragon?";

String cabinAction01 = "You pry some boards ";
String cabinAction02 = "off the cabin. It";
String cabinAction03 = "shifts, and a cloud";
String cabinAction04 = "of dust rolls out...";

String cabinItem01 = "====================";
String cabinItem02 = "=   You Acquired   =";
String cabinItem03 = "=      Boards      =";
String cabinItem04 = "====================";

String cabinAfter01 = "The empty space in";
String cabinAfter02 = "between the missing";
String cabinAfter03 = "boards revealed some";
String cabinAfter04 = "writing... [01--]?";

String bridgeSuccess01 = "You reach a group of";
String bridgeSuccess02 = "broken planks, but";
String bridgeSuccess03 = "luckily you have";
String bridgeSuccess04 = "spare boards! Yes!";

// walls text

String walls01 = "Past the bridge is a";
String walls02 = "castle. All around";
String walls03 = "it are stone walls.";
String walls04 = "Drawbridge is up.";

String walls11 = "In a nearby field,";
String walls12 = "you see white bones";
String walls13 = "and rusty armor in ";
String walls14 = "the weeds.";
//                      "--------------------"
String wallsOptions01 = "You're at the walls.";
String wallsOptions02 = "A: Look again.";
String wallsOptions03 = "B: Start climbing!";
String wallsOptions04 = "C: Check out bones.";

String battlefield01 = "It looks like a big";
String battlefield02 = "battle took place";
String battlefield03 = "here centuries ago.";
String battlefield04 = "Who did this?";

String battlefieldOptions01 = "So many bones...";
String battlefieldOptions02 = "A: Look again.";
String battlefieldOptions03 = "B: Check skeletons.";
String battlefieldOptions04 = "C: Back to the wall.";

//String battlefieldBack01 = "The ground shudders";
//String battlefieldBack02 = "from a distant";
//String battlefieldBack03 = "impact... Is the";
//String battlefieldBack04 = "princess safe?";

String battlefieldAction01 = "Vines interweave the";
String battlefieldAction02 = "femurs and rib cages";
String battlefieldAction03 = "like rope... Which";
String battlefieldAction04 = "you could use!";

String battlefieldItem01 = "====================";
String battlefieldItem02 = "=   You Acquired   =";
String battlefieldItem03 = "=       Vines      =";
String battlefieldItem04 = "====================";

String battlefieldAfter01 = "All that's left are";
String battlefieldAfter02 = "skeletons and grass.";
String battlefieldAfter03 = "You see numbers on";
String battlefieldAfter04 = "a stone... [0-2-]";

String wallsSuccess01 = "You lasso a gargoyle";
String wallsSuccess02 = "with your vine, and";
String wallsSuccess03 = "up the wall you go.";
String wallsSuccess04 = "You see the castle.";

//String wallsSuccess11 = "see the main castle";
//String wallsSuccess12 = "and one tower, much";
//String wallsSuccess13 = "taller than the rest";
//String wallsSuccess14 = "which is your goal.";

// tower text

String tower01 = "You open the";
String tower02 = "drawbridge to make";
String tower03 = "your escape easier.";
String tower04 = "This was a mistake!";

String tower11 = "The battefield... ";
String tower12 = "the skeletons...";
String tower13 = "they're coming alive";
String tower14 = "and rushing in!";

String towerOptions01 = "There are dozens!";
String towerOptions02 = "A: Look again.";
String towerOptions03 = "B: Fight them!";
String towerOptions04 = "C: Retreat for now.";

String retreat01 = "Your sword won't be";
String retreat02 = "much use against";
String retreat03 = "skeletons, so you";
String retreat04 = "make a hasty exit.";

String retreatOptions01 = "They're chasing you.";
String retreatOptions02 = "A: Why am I running?";
String retreatOptions03 = "B: Find new weapon.";
String retreatOptions04 = "C: Head back, fight!";

//String retreatBack01 = "You run back to see";
//String retreatBack02 = "the skeleton mob all";
//String retreatBack03 = "waving rusty knives";
//String retreatBack04 = "and chattering!";

String retreatAction01 = "You need a blunt";
String retreatAction02 = "weapon... something ";
String retreatAction03 = "to smash brittle";
String retreatAction04 = "bones with... Oh!";

String retreatItem01 = "====================";
String retreatItem02 = "=   You Acquired   =";
String retreatItem03 = "=      A Chair     =";
String retreatItem04 = "====================";

String retreatAfter01 = "You already have a";
String retreatAfter02 = "weapon! You notice";
String retreatAfter03 = "a number under the";
String retreatAfter04 = "chair... [0--3]";

String towerSuccess01 = "Wildly swinging";
String towerSuccess02 = "your trusty chair,";
String towerSuccess03 = "you smash through";
String towerSuccess04 = "the skeletons! WHAM!";

// castle text

String castle01 = "You see the princess";
String castle02 = "in the castle's high";
String castle03 = "tower. But the tower";
String castle04 = "door is locked.";

String castleOptions01 = "The lock has numbers";
String castleOptions02 = "A: Look again.";
String castleOptions03 = "B: Kick the door in!";
String castleOptions04 = "C: Enter a number.";

String castleCombo01 = "Enter three digits...";

String ep01 = "You rush up the";
String ep02 = "winding steps of the";
String ep03 = "tower and into the";
String ep04 = "princess's arms.";

String ep11 = "'You saved me!' she";
String ep12 = "cries. 'I've been ";
String ep13 = "trapped here for so";
String ep14 = "many years...'";

//            "--------------------"
String ep21 = "Her eyes glow red";
String ep22 = "and she bares her";
String ep23 = "fangs. 'Now we are ";
String ep24 = "both trapped! Haha!'";

//            "--------------------"
String ep31 = "====================";
String ep32 = "=        The       =";
String ep33 = "=        End       =";
String ep34 = "====================";

// death text

String bridgeDeath01 = "You make it halfway";
String bridgeDeath02 = "before stepping on";
String bridgeDeath03 = "a rotten board and";
String bridgeDeath04 = "falling below...";

String wallsDeath01 = "Your fingers give";
String wallsDeath02 = "out as you climb,";
String wallsDeath03 = "and you fall back,";
String wallsDeath04 = "breaking your neck.";

String towerDeath01 = "You heroically smash";
String towerDeath02 = "a few skeletons, but";
String towerDeath03 = "your sword breaks,";
String towerDeath04 = "and they swarm you.";

String castleDeath01 = "The door breaks in a";
String castleDeath02 = "puff of green poison";
String castleDeath03 = "which melts off your";
String castleDeath04 = "face, painfully...";

// other text

String youDied01 = "";
String youDied02 = "        YOU";
String youDied03 = "        DIED";
String youDied04 = "";

//String pressA01 = "";
//String pressA02 = "  PRESS A TO START";
//String pressA03 = "        ";
//String pressA04 = "";

// method for printing text on lcd, saves a LOT of work and room...
void displayText(String row1, String row2, String row3, String row4) {
  lcd.setCursor(0,0);
  lcd.print(row1);
  lcd.setCursor(0,1);
  lcd.print(row2);
  lcd.setCursor(0,2);
  lcd.print(row3);
  lcd.setCursor(0,3);
  lcd.print(row4);
}

void displayScrollingText(String row1, String row2, String row3, String row4) {
   lcd.setCursor(0,0);
   lcd.print(row1);
   delay(200);
   lcd.setCursor(0,1);
   lcd.print(row2);
   delay(200);
   lcd.setCursor(0,2);
   lcd.print(row3);
   delay(200);
   lcd.setCursor(0,3);
   lcd.print(row4);
}

// for clearing screen
void newScreen(){
  delay(2000);
  lcd.clear();
}

void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0;
  }
  return;
}

void setup() {
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {

  // input from keypad for arguments
  char playerChoice = keypad.getKey();
  //playerChoice = keypad.getKey();

  // keypad output on console for testing
  if(playerChoice){
    Serial.println(playerChoice);
  }
  
//=============================================================================================================================
// STAGE 0 - Title Screen
//=============================================================================================================================

  // STAGE 0-0
  if(stage == 0) {
    displayScrollingText(title01,title02,title03,title04);
    happySound();
    newScreen();
    stage++;
  }

//=============================================================================================================================
// STAGE 1 - The Bridge
//=============================================================================================================================

  // bridge
  if(stage == 1) {
    displayScrollingText(bridge01,bridge02,bridge03,bridge04);
    neutralSound();
    newScreen();
    displayScrollingText(bridge11,bridge12,bridge13,bridge14);
    neutralSound();
    newScreen();
    stage++;
  }

  // bridge options
  if(stage == 2){
    displayText(bridgeOptions01,bridgeOptions02,bridgeOptions03,bridgeOptions04);
    if(playerChoice == 'A'){
      lcd.clear();
      stage--;
    } else if(playerChoice == 'B' && hasBoards == 0){
      lcd.clear();
      stage = 100;
    } else if(playerChoice == 'B' && hasBoards == 1){
      lcd.clear();
      displayScrollingText(bridgeSuccess01,bridgeSuccess02,bridgeSuccess03,bridgeSuccess04);
      happySound();
      newScreen();
      stage = 5;
    } else if(playerChoice == 'C'){
      lcd.clear();
      stage++;
    }
    playerChoice = '0';
  }

  // cabin
  if(stage == 3) {
    displayScrollingText(cabin01,cabin02,cabin03,cabin04);
    neutralSound();
    newScreen();
    stage++;
  }

  // cabin options
  if(stage == 4) {
    displayText(cabinOptions01,cabinOptions02,cabinOptions03,cabinOptions04);
    if(playerChoice == 'A'){
      lcd.clear();
      stage--;
    } else if(playerChoice == 'B' && hasBoards == 0){
      lcd.clear();
      displayScrollingText(cabinAction01,cabinAction02,cabinAction03,cabinAction04);
      neutralSound();
      newScreen();
      hasBoards = 1;
      displayScrollingText(cabinItem01,cabinItem02,cabinItem03,cabinItem04);
      happySound();
      newScreen();
    } else if(playerChoice == 'B' && hasBoards == 1){
      lcd.clear();
      displayScrollingText(cabinAfter01,cabinAfter02,cabinAfter03,cabinAfter04);
      neutralSound();
      newScreen();
    } else if(playerChoice == 'C'){
      lcd.clear();
//      displayScrollingText(cabinBack01,cabinBack02,cabinBack03,cabinBack04);
//      ominousSound();
//      newScreen();
      stage = 2;
    } 
    playerChoice = '0';
  }
  
//=============================================================================================================================
// STAGE 2 - The Wall
//============================================================================================================================

  // wall
  if(stage == 5) {
    displayScrollingText(walls01,walls02,walls03,walls04);
    neutralSound();
    newScreen();
    displayScrollingText(walls11,walls12,walls13,walls14);
    neutralSound();
    newScreen();
    stage++;
  }

  // wall options
  if(stage == 6){
    displayText(wallsOptions01,wallsOptions02,wallsOptions03,wallsOptions04);
    if(playerChoice == 'A'){
      lcd.clear();
      stage--;
    } else if(playerChoice == 'B' && hasVine == 0){
      lcd.clear();
      stage = 100;
    } else if(playerChoice == 'B' && hasVine == 1){
      lcd.clear();
      displayScrollingText(wallsSuccess01,wallsSuccess02,wallsSuccess03,wallsSuccess04);
      happySound();
      newScreen();
//      displayScrollingText(wallsSuccess11,wallsSuccess12,wallsSuccess13,wallsSuccess14);
//      happySound();
//      newScreen();
      stage = 9;
    } else if(playerChoice == 'C'){
      lcd.clear();
      stage++;
    }
    playerChoice = '0';
  }

  // battlefield
  if(stage == 7) {
    displayScrollingText(battlefield01,battlefield02,battlefield03,battlefield04);
    neutralSound();
    newScreen();
    stage++;
  }

  // battlefield options
  if(stage == 8) {
    displayText(battlefieldOptions01,battlefieldOptions02,battlefieldOptions03,battlefieldOptions04);
    if(playerChoice == 'A'){
      lcd.clear();
      stage--;
    } else if(playerChoice == 'B' && hasVine == 0){
      lcd.clear();
      displayScrollingText(battlefieldAction01,battlefieldAction02,battlefieldAction03,battlefieldAction04);
      neutralSound();
      newScreen();
      hasVine = 1;
      displayScrollingText(battlefieldItem01,battlefieldItem02,battlefieldItem03,battlefieldItem04);
      happySound();
      newScreen();
    } else if(playerChoice == 'B' && hasVine == 1){
      lcd.clear();
      displayScrollingText(battlefieldAfter01,battlefieldAfter02,battlefieldAfter03,battlefieldAfter04);
      neutralSound();
      newScreen();
    } else if(playerChoice == 'C'){
      lcd.clear();
//      displayScrollingText(battlefieldBack01,battlefieldBack02,battlefieldBack03,battlefieldBack04);
//      ominousSound();
//      newScreen();
      stage = 6;
    } 
    playerChoice = '0';
  }

//=============================================================================================================================
// STAGE 3 - The Tower (Really this should be called 'The SKeletons'...)
//============================================================================================================================

  // tower
  if(stage == 9) {
    displayScrollingText(tower01,tower02,tower03,tower04);
    neutralSound();
    newScreen();
    displayScrollingText(tower11,tower12,tower13,tower14);
    neutralSound();
    newScreen();
    stage++;
  }

  // tower options
  if(stage == 10){
    displayText(towerOptions01,towerOptions02,towerOptions03,towerOptions04);
    if(playerChoice == 'A'){
      lcd.clear();
      stage--;
    } else if(playerChoice == 'B' && hasVase == 0){
      lcd.clear();
      stage = 100;
    } else if(playerChoice == 'B' && hasVase == 1){
      lcd.clear();
      displayScrollingText(towerSuccess01,towerSuccess02,towerSuccess03,towerSuccess04);
      happySound();
      newScreen();
      stage = 13;
    } else if(playerChoice == 'C'){
      lcd.clear();
      stage++;
    }
    playerChoice = '0';
  }

  // retreat
  if(stage == 11) {
    displayScrollingText(retreat01,retreat02,retreat03,retreat04);
    neutralSound();
    newScreen();
    stage++;
  }

  // retreat options
  if(stage == 12) {
    displayText(retreatOptions01,retreatOptions02,retreatOptions03,retreatOptions04);
    if(playerChoice == 'A'){
      lcd.clear();
      stage--;
    } else if(playerChoice == 'B' && hasVase == 0){
      lcd.clear();
      displayScrollingText(retreatAction01,retreatAction02,retreatAction03,retreatAction04);
      neutralSound();
      newScreen();
      hasVase = 1;
      displayScrollingText(retreatItem01,retreatItem02,retreatItem03,retreatItem04);
      happySound();
      newScreen();
    } else if(playerChoice == 'B' && hasVase == 1){
      lcd.clear();
      displayScrollingText(retreatAfter01,retreatAfter02,retreatAfter03,retreatAfter04);
      neutralSound();
      newScreen();
      stage = 10;
    } else if(playerChoice == 'C'){
      lcd.clear();
//      displayScrollingText(retreatBack01,retreatBack02,retreatBack03,retreatBack04);
//      ominousSound();
//      newScreen();
      stage = 10;
    } 
    playerChoice = '0';
  }

  //=============================================================================================================================
// STAGE 4 - The Castle and Epilogue
//============================================================================================================================

  // castle
  if(stage == 13) {
    displayScrollingText(castle01,castle02,castle03,castle04);
    neutralSound();
    newScreen();
    stage++;
  }

  // castle door
  if(stage == 14){
    displayText(castleOptions01,castleOptions02,castleOptions03,castleOptions04);
    if(playerChoice == 'A'){
      lcd.clear();
      stage--;
    } else if(playerChoice == 'B' && hasCode == 0){
      lcd.clear();
      stage = 100;
      } else if(playerChoice == 'B' && hasCode == 1){
      lcd.clear();
      stage = 16;
    } else if(playerChoice == 'C'){
      lcd.clear();
      stage++;
    }
    playerChoice = '0';
  }

  // Epilogue
  if(stage == 15){
    lcd.setCursor(0,0);
    lcd.print("Enter three numbers:");
    if(playerChoice){
      Data[data_count] = playerChoice;
      lcd.setCursor(data_count,1);
      lcd.print(Data[data_count]);
      data_count++;
    }
    if(data_count == Password_Length-1){
      lcd.clear();
      if(!strcmp(Data,Master)){
        lcd.print("The door unlocks!");
        delay(1000);
        happySound;
        hasCode = 1;
        stage = 16;
      } else {
        lcd.print("Hmm... Incorrect.");
        neutralSound();
        delay(1000);
      }
      lcd.clear();
      clearData();
      //playerChoice = '0';
      stage = 14;
    }
  }

  if(stage == 16) {
    displayScrollingText(ep01,ep02,ep03,ep04);
    neutralSound();
    newScreen();
    displayScrollingText(ep11,ep12,ep13,ep14);
    neutralSound();
    newScreen();
    displayScrollingText(ep21,ep22,ep23,ep24);
    neutralSound();
    newScreen();
    displayScrollingText(ep31,ep32,ep33,ep34);
    ominousSound();
    newScreen();
    stage = 0;
    playerChoice = 0;
  }

//===========================================================================================
// STAGE 100 - GAME OVER
//===========================================================================================

  // this 'stage' contains game over screens based on where player died
  // STAGE 100
  if(stage == 100){
    if(hasBoards == 0){
      displayScrollingText(bridgeDeath01,bridgeDeath02,bridgeDeath03,bridgeDeath04);
      neutralSound();
      newScreen();
    } else if(hasVine == 0){
      displayScrollingText(wallsDeath01,wallsDeath02,wallsDeath03,wallsDeath04);
      neutralSound();
      newScreen();
    } else if(hasVase == 0){
      displayScrollingText(towerDeath01,towerDeath02,towerDeath03,towerDeath04);
      neutralSound();
      newScreen();
    } else {
      displayScrollingText(castleDeath01,castleDeath02,castleDeath03,castleDeath04);
      neutralSound();
      newScreen();
    } 
  displayText(youDied01,youDied02,youDied03,youDied04);
  ominousSound();
  newScreen();
  playerChoice = '0';

  hasBoards = 0;
  hasVine = 0;
  hasVase = 0;
  stage = 0;
  }
}
