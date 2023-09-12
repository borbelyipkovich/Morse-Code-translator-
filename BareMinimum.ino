#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
struct KeyValue {
  char key;
  char* value;
};
KeyValue units[] = {
  {'A', ".-"},
  {'B', "-..."},
  {'C', "-.-."},
  {'D', "-.."},
  {'E', "."},
  {'F', "..-."},
  {'G', "--."},
  {'H', "...."},
  {'I', ".."},
  {'J', ".---"},
  {'K', "-.-"},
  {'L', ".-.."},
  {'M', "--"},
  {'N', "-."},
  {'O', "---"},
  {'P', ".--."},
  {'Q', "--.-"},
  {'R', ".-."},
  {'S', "..."},
  {'T', "-"},
  {'U', "..-"},
  {'V', "...-"},
  {'W', ".--"},
  {'X', "-..-"},
  {'Y', "-.--"},
  {'Z', "--.."}
};

char* words[] = {
  "apple",
  "banana",
  "cherry",
  "dog",
  "cat",
  "elephant",
  "giraffe",
  "helicopter",
  "icecream",
  "jump",
  "kangaroo",
  "lemon",
  "mango",
  "notebook",
  "orange",
  "penguin",
  "quilt",
  "rainbow",
  "sunshine",
  "tiger",
  "umbrella",
  "violin",
  "watermelon",
  "xylophone",
  "zebra",
  "adventure",
  "beautiful",
  "chocolate",
  "delight",
  "elephant",
  "fantastic",
  "graceful",
  "happiness",
  "incredible",
  "journey",
  "kindness",
  "laughter",
  "magnificent",
  "nightingale",
  "opportunity",
  "peaceful",
  "quirky",
  "rainbow",
  "serendipity",
  "tranquil",
  "unique",
  "vibrant",
  "wonderful",
  "xenial",
  "youthful",
  "zesty",
  "balance",
  "calm",
  "dream",
  "energetic",
  "freedom",
  "gratitude",
  "hope",
  "inspiration",
  "joy",
  "knowledge",
  "love",
  "motivation",
  "nurture",
  "optimism",
  "patience",
  "quiet",
  "resilience",
  "smile",
  "trust",
  "unity",
  "victory",
  "wisdom",
  "xanadu",
  "yearning",
  "zeal",
  "achievement",
  "belief",
  "curiosity",
  "determination"
};

int str_l;

void shuffleArray(char* arr[], int length) {
  for (int i = length - 1; i > 0; i--) {
    int j = random(i + 1);
    char* temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}
int numWords = sizeof(words) / sizeof(words[0]);

int bt = 2;
int bt_v;

int dt = 100; 

int dot_max;
int dash_min;

int dots[3];
int dashes[3];

int bt_back = 4;
int bt_back_v;

int bt_check = 7;

int led_green = 10;
int led_red = 8;

int begin_ = 0;
int setup_ = 0;

int counter = 0;

int count_dot = 0;
int iteration = 0;

int first_m;

int word_count = 0;

bool next = true;

int count_dash = 0;

const int maxStringLength = 50;

char charList[maxStringLength];

char* this_word;

char dot_and_dash_ans[33]; 

int dt_iter = 20; 

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.clear();
  lcd.setCursor(0,0);
  pinMode(bt,OUTPUT);
  pinMode(bt_back,OUTPUT);
  pinMode(bt_check,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(led_red,OUTPUT);
  randomSeed(analogRead(0));
  shuffleArray(words, numWords);

  digitalWrite(led_green,LOW);
  digitalWrite(led_red,LOW);

}

void begin(){
    while(digitalRead(bt) != HIGH){
      lcd.setCursor(0,0);
      lcd.print("Press the lower");
      lcd.setCursor(0,1);
      lcd.print("button to begin!");

      delay(dt);
  }
    begin_ = 1;
    lcd.clear(); 
}
void setting_up_txt(){
  lcd.clear();
  while(counter != 3){
    lcd.setCursor(3,0);
    lcd.print("Setting up");
    lcd.setCursor(2,1);
    lcd.print("dot and dash");
    counter ++;
    delay(1000);
  }
  
}
void dot_setup(){
  lcd.clear();
  lcd.print("Setting up dots");
  lcd.setCursor(0,1);
  lcd.print("Press it 3 times");
  while(iteration != 3){
    if(digitalRead(bt) == HIGH){
      while(digitalRead(bt) == HIGH){
        count_dot ++;
        delay(dt);
      }
      dots[iteration] = count_dot;
      iteration ++;
      count_dot = 0;
    }
  }
  first_m = max(dots[0],dots[1]);
  dot_max = max(first_m,dots[2]);
  iteration = 0;
  
}
void dash_setup(){
  lcd.clear();
  lcd.print("Setting up dash");
  lcd.setCursor(0,1);
  lcd.print("Press it 3 times");
  while(iteration != 3){
    if(digitalRead(bt) == HIGH){
      while(digitalRead(bt) == HIGH){
        count_dash ++;
        delay(dt);
      }
      dashes[iteration] = count_dash;
      iteration ++;
      count_dash = 0;
    }
  }
  iteration = 0;
  first_m = min(dashes[0],dashes[1]);
  dash_min = min(first_m,dashes[2]);
  
  
}
void setup_comms(){
  setting_up_txt();
  dot_setup();

  for(int i = 3; i > 0;i--){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Setting up");
    lcd.setCursor(2,1);
    lcd.print("dashes in ");
    lcd.setCursor(12,1);
    lcd.print(i);
    delay(1000);
  }
  dash_setup();
  setup_ = 1;
}

char* which(int i) {
  if (i == 1) {
    return "1st";
  } else if (i == 2) {
    return "2nd";
  } else if (i == 3) {
    return "3rd";
  } else {
    static char result[8]; // Static to retain the value between calls
    sprintf(result, "%dth", i);
    return result;
  }
}



char* morse_ans() {
  int listIndex = 0;
  this_word = words[word_count];
  for (int i = 0; this_word[i] != '\0'; i++) {
    charList[listIndex] = this_word[i];
    listIndex++;
  }
  charList[listIndex] = '\0';

  // Initialize dot_and_dash_ans as an empty string
  strcpy(dot_and_dash_ans, "");

  for (int i = 0; i < listIndex; i++) {
    for (int j = 0; j < 26; j++) {
      if (toupper(charList[i]) == units[j].key) {
        // Concatenate the Morse code value to dot_and_dash_ans
        strcat(dot_and_dash_ans, units[j].value);
      }
    }
  }

  return dot_and_dash_ans;
}


void play_txt(){
  lcd.clear();
  lcd.print("Your ");
  lcd.print(which(word_count + 1));
  lcd.print(" word is");
  lcd.setCursor(0,1);
  lcd.print(words[word_count]);
}


void print_screen(){

}

void loop() {

  if(begin_ == 0){
    begin();
  }
  if (setup_ == 0){
    setup_comms();
  }



  if(next == true){
    play_txt();
    next = false;
  }
  if(next == false){
    bt_v = digitalRead(bt);
    while(digitalRead(bt) != HIGH){
      delay(dt);
    }   
      int x = 0;
      int y = 0;
      char input_ans[33] = "";
      
      lcd.clear();
      while(digitalRead(bt)==HIGH){
        delay(dt);
      }
      while(strcmp(input_ans, morse_ans()) != 0){
        
        if(digitalRead(bt) == HIGH){
           
          while(digitalRead(bt) == HIGH){
            
            iteration ++;
            delay(dt);
          }
          lcd.clear();
          if(iteration <= dot_max){
            
            strcat(input_ans, ".");                     
          }
          else if(iteration >dash_min){
            
            strcat(input_ans,"-");
          }
          iteration = 0;
          
          if(x <= 15){
            lcd.setCursor(0,0);
            lcd.print(input_ans);
          }
          else{
          lcd.setCursor(0,0);
          lcd.print(input_ans);
          lcd.setCursor(0,1);
          lcd.print(input_ans + 16);
          }
          if(x < 32){
            x++;
          }
        }
        
        if(digitalRead(bt_back)==HIGH){
          while(digitalRead(bt_back)==HIGH){
            delay(dt);
          }
          if(strlen(input_ans)>0){
            input_ans[strlen(input_ans) - 1] = '\0';
            x--;
          }else{continue;}     
          lcd.clear();
          if(x <= 15){
            lcd.print(input_ans);
          }
          else{
          lcd.setCursor(0,0);
          lcd.print(input_ans);
          lcd.setCursor(0,1);
          lcd.print(input_ans + 16);
          }
        }
        if(digitalRead(bt_check) == HIGH){  

          while(digitalRead(bt_check)==HIGH){
            delay(dt);
          }
          if(strlen(input_ans) > 0){ 
          char morse_check[33];
          strncpy(morse_check, morse_ans(), x);
          morse_check[x] = '\0';
          Serial.println(input_ans);
          Serial.println(morse_check);
          if (strcmp(input_ans, morse_check) == 0){
            for(int i = 1; i < dt_iter;i++){
              digitalWrite(led_green,HIGH);
              delay(30);
              digitalWrite(led_green,LOW);
              delay(30);
            }
          }
          else{
            for(int i = 1; i < dt_iter;i++){
              digitalWrite(led_red,HIGH);
              delay(30);
              digitalWrite(led_red,LOW);
              delay(30);
              }
            }
          }
        }
        delay(dt);
      }
    lcd.clear();
    lcd.print("Congrats!");
    delay(2000);
    next = true;
    word_count ++;
  }
  delay(dt);
}