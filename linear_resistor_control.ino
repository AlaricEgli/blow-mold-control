

// initialize the library by providing the nuber of pins to it
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd1 = LiquidCrystal_I2C(0x26, 20, 4); // Change to (0x27,16,2) for 16x2 LCD.
LiquidCrystal_I2C lcd2 = LiquidCrystal_I2C(0x27, 20, 4); // Change to (0x27,16,2) for 16x2 LCD.

//OUTPUT TO RELAY(S)
  const int ledPin = 13;  // choose the pin for the LED
  const int clamp_close_relay = 53; //  CLAMP CLOSE RELAY
  const int clamp_open_relay = 51; // CLAMP OPEN RELAY
  const int air_on_relay = 49; // AIR ON RELAY
  const int screw_fwd_relay = 47; // SCREW FORWARD RELAY
  const int screw_rev_relay = 45; // SCREW REVERSE RELAY
  const int g_valve_relay = 43; // G VALVE RELAY
  const int y_valve_relay = 41; // Y VALVE RELAY

//END OUTPUT TO RELAYS

//STUFF FOR RUNNING THE PROGRAM
  unsigned long startMillis;
  unsigned long currentMillis;
  unsigned long purge_fwd_complete = 0; //VALUE TO SET WHEN PURGE FORWARD HAS REACHED END RESISTANCE VALUE
  unsigned long clamp_close_tts = 0; // TIME IN MILLISECONDS FROM STARTMILLIS TO BEGIN THE CLAMP CLOSE OPERATION
  unsigned long clamp_close_tte = 0; // TIME IN MILLISECONDS FROM STARTMILLIS TO END THE CLAMP CLOSE OPERATION
  unsigned long air_blow_tts = 0; //  TIME IN MILLISECONDS FROM STARTMILLIS TO BEGIN THE AIR BLOW OPERATION
  unsigned long air_blow_tte = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO END THE AIR BLOW OPERATION
  unsigned long purge_rev_complete = 0; //  VALUE TO SET WHEN THE PURGE RECOVERY HAS REACHED END RESISTANCE VALUE
  unsigned long clamp_open_tts = 0; //  TIME IN MILLISECONDS FROM STARTMILLIS TO BEGIN THE CLAMP OPEN OPERATION
  unsigned long clamp_open_tte = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO END THE CLAMP OPEN OPERATION
  unsigned long g_valve_tts1 = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO BEGIN OF G VALVE OPERATION 1
  unsigned long g_valve_tte1 = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO END OF G VALVE OPERATION 1
  unsigned long y_valve_tts1 = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO BEGIN OF Y VALVE OPERATION 1
  unsigned long y_valve_tte1 = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO END OF Y VALVE OPERATION 1
  unsigned long g_valve_tts2 = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO BEGIN OF G VALVE OPERATION 2
  unsigned long g_valve_tte2 = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO END OF G VALVE OPERATION 2
  unsigned long y_valve_tts2 = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO BEGIN OF Y VALVE OPERATION 2
  unsigned long y_valve_tte2 = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO END OF Y VALVE OPERATION 2
  unsigned long shot_fwd_tts = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO BEGIN OF SHOT FWD
  unsigned long shot_fwd_tte = 0; // TIME IN MILLISECONDS FROM THE STARTMILLIS TO END OF SHOT FWD
  unsigned long shot_rev_tts = 0; // TIME IN MILLISECONDS FROM THE STARTMILLIS TO START OF SHOT REVERSE
  unsigned long shot_rev_tte = 0; //  TIME IN MILLISECONDS FROM THE STARTMILLIS TO END OF SHOT REVERSE

  
//END OF STUFF FOR RUNNING THE PROGRAM 
  

//BUTTON INPUT PIN ASSIGNMENTS

    //READING THE BUTTON RESISTANCE
      int upper_row_value; // READING THE UPPER BUTTON RESISTANCE VALUE
      int lower_row_value; // READING THE LOWER BUTTON RESISTANCE VALUE

    //AIR BLOW TO CLAMP OPEN DELAY
      int air_delay_up = 0; // DELAY BETWEEN AIR BLOW FINISHING AND CLAMP OPEN TIME UP 
      int air_delay_dn = 0; // DELAY BETWEEN AIR BLOW FINISHING AND CLAMP OPEN TIME DOWN
      
    //SHOT FORWARD TO CLAMP CLOSE DELAY INPUTS
      int sf_delay_up = 0; //  SHOT FORWARD START DELAY UP 
      int sf_delay_dn = 0; //  SHOT FORWARD START DELAY DOWN
      
    //CONTROL PARAMETERS
      int ESTOP = 0; // TIME TO PANIC BBY
      int cycle_start = 0;  // CYCLE START
      int cycle_start2 = 0; // CYCLE IS CURRENTLY RUNNING
      int g_valve = 0; //  HIGH PRESSURE G VALVE
      int y_valve = 0; //  HIGH PRESSURE Y VALVE
      
    //CLAMP BUTTON INPUTS
      int clamp_x1 = 0; // CLAMP OPEN TIME UP
      int clamp_x2 = 0; // CLAMP OPEN TIME DOWN
      int clamp_x3 = 0; // CLAMP CLOSE TIME UP
      int clamp_x4 = 0; // CLAMP CLOSE TIME DOWN

      
    //AIR BUTTON INPUTS
      int air_up = 0 ; // AIR TIME UP
      int air_down = 0; // AIR TIME DOWN

 
    //SCREW CONTROL INPUTS
      int shot_fr = 0;  // SHOT FORWARD RESISTANCE UP
      int shot_rr = 0;  // SHOT FORWARD RESISTANCE DOWN

    
      int shot_ru = 0;  // RECOVERY (SHOT REVERSE) RESISTANCE UP
      int shot_rd = 0;  // RECOVERY (SHOT REVERSE) RESISTANCE DOWN

//END BUTTON INPUT PIN ASSIGNMENTS

      


// VARIABLES FOR THE SCREW POSITION
    int screw_pos = 0; //VALUE FOR THE CURRENT SCREW POSITION (0-1023)
    int screw_fwd_pos = 0; // VALUE FOR THE POSITION I'D LIKE TO STOP THE SCREW AT (0-1023)
    int screw_return_pos = 0; //VALUE FOR WHERE I'D LIKE TO RETURN THE SCREW TO (0-1023)
    int screw_complete = 0 ; //SETTING A FLAG THAT THE SCREW IS DONE MOVING AND WE CAN BEGIN THE REST OF THE PROGRAM
 
//VARIBLES TO READ THE BUTTON INPUTS

    //AIR BLOW TO CLAMP OPEN DELAY BUTTONS
      int air_delay_up_btn = 0; //  DELAY BETWEEN AIR BLOW FINISHING AND CLAMP OPEN TIME UP BUTTON
      int air_delay_dn_btn = 0; //  DELAY BETWEEN AIR BLOW FINISHING AND CLAMP OPEN TIME DOWN BUTTON

    //SHOT FORWARD TO CLAMP CLOSE DELAY BUTTONS   
      int sf_delay_btn_up = 0; // SHOT FORWARD START DELAY UP 
      int sf_delay_btn_dn = 0; // SHOT FORWARD START DELAY DN
      
    //CONTROL PARAMETER BUTTONS
      int E_STOP_BTN = 0; //  SHIT HIT THE FAN BUTTON
      int cycle_start_btn = digitalRead(29); // CYCLE START BUTTON
      int g_valve_btn = 0;  //  G VALVE BUTTON
      int y_valve_btn = 0;  //  Y VALVE BUTTON 

    //CLAMP BUTTON INPUTS      
      int clamp_btn1 = 0; // VARIABLE FOR READING THE CLAMP OPEN TIME UP STATUS
      int clamp_btn2 = 0; // VARIABLE FOR READING THE CLAMP OPEN TIME DOWN STATUS
      int clamp_btn3 = 0; // VARIABLE FOR READING THE CLAMP CLOSE TIME UP STATUS
      int clamp_btn4 = 0; // VARIABLE FOR READING THE CLAMP CLOSE TIME DOWN STATUS

  
    //AIR BLOW BUTTONS
      int air_up_button = 0; // VARIABLE FOR READING THE AIR UP STATUS
      int air_down_button = 0; // VARIABLE FOR READING THE AIR DOWN STATUS


    //SCREW CONTROL BUTTONS
      int shot_fr_btn = 0;  //  SHOT FORWARD RESISTANCE UP BUTTON
      int shot_rr_btn = 0;  //  SHOT FORWARD RESISTANCE DOWN BUTTON

      int shot_ru_btn = 0;  //  RECOVERY (SHOT REVERSE) RESISTANCE UP BUTTON
      int shot_rd_btn = 0;  //  RECOVERY (SHOT REVERSE) RESISTANCE DOWN BUTTON


//END VARIABLES TO READ THE BUTTON INPUTS

//VARIABLES FOR THE DISPLAY
  float air_time = 40; //  VARIABLE FOR AIR CYCLE TIME
  float shot_fwd = 500; //  VARIABLE FOR THE SHOT FORWARD RESISTANCE
  float shot_back = 2; // VARIABLE FOR THE SHOT REVERSE RESISTANCE
  float clamp_open = 30; // VARIABLE FOR THE CLAMP OPEN TIME
  float clamp_close = 30; // VARIABLE FOR THE CLAMP CLOSE TIME
  float sf_delay = 0; //  VARIABLE FOR THE SHOT FORWARD TO CLAMP CLOSE DELAY
  float air_delay = 1; // VARIABLE FOR THE DELAY BETWEEN AIR END AND CLAMP OPEN
  unsigned long cycle_time = 0; //  PROGRAM CYCLE TIME
//END VARIABLES FOR THE DISPLAY

  

void setup() {
//CREATING OUR TIME TRACKING 
  startMillis = millis(); //TIME SINCE BOARD POWERED ON. BAD THINGS AT 49 DAYS UPTIME
//END OF CREATING OUR TIME TRACKING

//LCD STUFF  
  // Initiate the LCD:
       lcd1.init();
       lcd1.backlight();
       lcd2.init();
       lcd2.backlight();


  // WRITING TEXT INFO TO DISPLAY 1
     lcd1.setCursor(0,0); //NEW LCD LINE (LINE 1)
     lcd1.print("SHOT FORWARD:");

     lcd1.setCursor(0,1); //NEW LCD LINE (LINE 2)
     lcd1.print("SHOT FWD DLA:");

     lcd1.setCursor(0,2); //NEW LCD LINE (LINE 3)
     lcd1.print("AIR BLOW:");

     lcd1.setCursor(0,3); //NEW LCD LINE (LINE 4)
     lcd1.print("CLAMP OPEN DLA:");
     
  //  WRITING TEXT INFO TO DISPLAY 2
     lcd2.setCursor(0,0); //NEW LCD LINE (LINE 1)
     lcd2.print("SHOT REVERSE:");//text to print

     lcd2.setCursor(0,1); //NEW LCD LINE (LINE 2)
     lcd2.print("CLAMP OPEN:");

     lcd2.setCursor(0,2); //NEW LCD LINE (LINE 3)
     lcd2.print("CLAMP CLOSE:");

     lcd2.setCursor(0,3); //NEW LCD LINE (LINE 4)
     lcd2.print("CYCLE TIME:");
     
//END LCD STUFF

//PIN MODE INPUT SETUP

      pinMode(32, INPUT_PULLUP);

//END OF PIN MODE SETUP

    
//PIN MODE OUTPUT SETUP

    pinMode(ledPin, OUTPUT); //Declare LED as output
    pinMode(clamp_close_relay, OUTPUT); //CLAMP CLOSE RELAY
    pinMode(clamp_open_relay, OUTPUT);  //CLAMP OPEN RELAY
    pinMode(air_on_relay, OUTPUT);  //AIR ON RELAY
    pinMode(screw_fwd_relay, OUTPUT); //SHOT FORWARD (SCREW) RELAY
    pinMode(screw_rev_relay, OUTPUT); //SHOT REVERSE (SCREW) RELAY
    pinMode(g_valve_relay, OUTPUT); //G VALVE RELAY
    pinMode(y_valve_relay, OUTPUT); //Y VALVE RELAY
    
//END OF PIN MODE OUTPUT SETUP

Serial.begin(9600);

//MAKING SURE RELAYS ARE CURRENTLY OFF

                    digitalWrite(clamp_close_relay, HIGH);
                    digitalWrite(clamp_open_relay, HIGH);
                    digitalWrite(air_on_relay, HIGH);
                    digitalWrite(screw_fwd_relay, HIGH);
                    digitalWrite(screw_rev_relay, HIGH);
                    digitalWrite(g_valve_relay, HIGH);
                    digitalWrite(y_valve_relay, HIGH);
                    
//END OF MAKING SURE THE RELAYS ARE OFF

}

void loop(){
//Serial.print(startMillis);
currentMillis = millis();
// Serial.println(digitalRead(29));
// Serial.println(cycle_start2);

// READING AND ASSIGNING VARIABLES 
    upper_row_value = analogRead(A1); // READING IN THE RESISTANCE OF THE UPPER ROW 
    lower_row_value = analogRead(A0); // READING IN THE RESISTANCE OF THE LOWER ROW

// READING THE SCREW POSITION 
    screw_pos = analogRead(A2); // GRABS THE CURRENT RESISTANCE OF THE SCREW
// END OF READING THE SCREW POSITION


//SETTING VARIABLES
    // CODE TO SET AIR TIME
                 if (upper_row_value>=150 && upper_row_value<=180){  //Switch the LED on if button is pressed
                  // turn LED on:
                  digitalWrite(ledPin, HIGH);
                  Serial.print(startMillis);
                  air_time = air_time+.1;\
                  delay(250);
                  }
                     else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
              
                 if (lower_row_value>=150 && lower_row_value<=180){  //Switch the LED on if button is pressed
                  // turn LED on:
                  digitalWrite(ledPin, HIGH);
                  air_time = air_time-.1;\
                  delay(250);
                  }
                          else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
                
    //CODE TO SET THE CLAMP OPENING TIMES
                 if (upper_row_value>=460 && upper_row_value<=480){  //Switch the LED on if button is pressed
                  // turn LED on:
                  digitalWrite(ledPin, HIGH);
                  clamp_open = clamp_open+.1;\
                  delay(250);
                  }
                     else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
              
                if (lower_row_value>=460 && lower_row_value<=480){  //Switch the LED on if button is pressed
                    // turn LED on:
                   digitalWrite(ledPin, HIGH);
                   clamp_open = clamp_open-.1;\
                    delay(250);
                    }
                          else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }

    //CODE TO SET THE CLAMP CLOSING TIMES
                if (upper_row_value>=950 && upper_row_value<=1100){  //Switch the LED on if button is pressed
                  // turn LED on:
                  digitalWrite(ledPin, HIGH);
                  clamp_close = clamp_close+.1;\
                  delay(250);
                  }
                     else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
              
                if (lower_row_value>=950 && lower_row_value<=1100){  //Switch the LED on if button is pressed
                    // turn LED on:
                   digitalWrite(ledPin, HIGH);
                   clamp_close = clamp_close-.1;\
                    delay(250);
                    }
                          else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
                
      //CODE TO SET THE SHOT FORWARD TO CLAMP CLOSE DELAY
               if (upper_row_value>=110 && upper_row_value<=130){  //Switch the LED on if button is pressed
                  // turn LED on:
                  digitalWrite(ledPin, HIGH);
                  sf_delay = sf_delay+.1;\
                  delay(250);
                  }
                     else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
              
                if (lower_row_value>=110 && lower_row_value<=130){  //Switch the LED on if button is pressed
                    // turn LED on:
                   digitalWrite(ledPin, HIGH);
                   sf_delay = sf_delay-.1;\
                    delay(250);
                    }
                          else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
      //CODE TO SET THE AIR BLOW FINISH TO CLAMP OPEN TIME
                 if (upper_row_value>=200 && upper_row_value<=230){  //Switch the LED on if button is pressed
                  // turn LED on:
                  digitalWrite(ledPin, HIGH);
                  air_delay = air_delay+.1;\
                  delay(250);
                  }
                     else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
              
                if (lower_row_value>=200 && lower_row_value<=230){  //Switch the LED on if button is pressed
                    // turn LED on:
                   digitalWrite(ledPin, HIGH);
                   air_delay = air_delay-.1;\
                    delay(250);
                    }
                          else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }     
      //CODE TO SET THE SHOT FORWARD VALUE
                 if (upper_row_value>=60 && upper_row_value<=90){  //Switch the LED on if button is pressed
                  // turn LED on:
                  digitalWrite(ledPin, HIGH);
                  shot_fwd = shot_fwd+10;\
                  delay(250);
                  }
                     else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
              
                if (lower_row_value>=60 && lower_row_value<=90){  //Switch the LED on if button is pressed
                    // turn LED on:
                   digitalWrite(ledPin, HIGH);
                   shot_fwd = shot_fwd-10;\
                    delay(250);
                    }
                          else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }  

      //CODE TO SET THE SHOT REVERSE VALUE
                 if (upper_row_value>=280 && upper_row_value<=320){  //Switch the LED on if button is pressed
                  // turn LED on:
                  digitalWrite(ledPin, HIGH);
                  shot_back = shot_back+.1;\
                  delay(250);
                  }
                     else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }
              
                if (lower_row_value>=280 && lower_row_value<=320){  //Switch the LED on if button is pressed
                    // turn LED on:
                   digitalWrite(ledPin, HIGH);
                   shot_back = shot_back-.1;\
                    delay(250);
                    }
                          else {
                  // turn LED off:
                  digitalWrite(ledPin, LOW);
                }     

                                        
//END OF SETTING VARIABLES


//PROGRAM RUN
                if (digitalRead(29) == LOW) { //CALCULATES THE POSITIONS I'D LIKE THE SCREW TO GO TO. 
                        // turn LED on:
                        digitalWrite(ledPin, HIGH);
                        cycle_start = 1; //FLAG TO ALLOW THE CYCLE TO START
                        cycle_start2 = 1; // FLAG THAT THE CYCLE IS STILL ONGOING
                        startMillis = millis();
                          Serial.print("MY STARTING TIME IS:");
                          Serial.print(startMillis);
                          Serial.println();
                          delay(250);
                        screw_return_pos = screw_pos ; // WRITES THE CURRENT SCREW POSITION SO WE KNOW WHERE TO RETURN TO
                          Serial.print("THE POSITION I'D LIKE TO RETURN MY SCREW TO:");
                          Serial.print(screw_return_pos);
                          Serial.println();
                        screw_fwd_pos = screw_pos-(shot_fwd) ; 
                          Serial.print("THE POSITION I'D LIKE TO STOP MY SCREW IS AT:");
                          Serial.print(screw_fwd_pos);
                          Serial.println();                   
                }
//END OF PROGRAM RUN 

                  
// WELCOME TO THE DANGER ZONE. THIS AREA WILL TURN ON THE RELAYS ALL WILLY NILLY
// WELCOME TO THE DANGER ZONE. THIS AREA WILL TURN ON THE RELAYS ALL WILLY NILLY
// WELCOME TO THE DANGER ZONE. THIS AREA WILL TURN ON THE RELAYS ALL WILLY NILLY
// WELCOME TO THE DANGER ZONE. THIS AREA WILL TURN ON THE RELAYS ALL WILLY NILLY

                       
                  if (currentMillis >= startMillis && screw_pos >= screw_fwd_pos && cycle_start==1 ) {
                   // turn LED on:
                    digitalWrite(ledPin, HIGH);
                    digitalWrite(screw_fwd_relay, LOW);
                    digitalWrite(y_valve_relay, LOW);
                    //lcd2.setCursor(12,3); 
                    //lcd2.print ("CLAMP CL");//prints clamp close relay status to LCD
                    Serial.println("I'm turning on the shot forward and Y valve relay now");
                  }


                  if (screw_pos <= screw_fwd_pos +10 && screw_pos >= screw_fwd_pos - 10 && cycle_start == 1) {
                      Serial.println("WE'VE DONE IT BOYS THE SCREW HAS MADE IT");
                      Serial.print(screw_pos);
                      Serial.println();
                      digitalWrite(ledPin, HIGH);
                      digitalWrite(screw_fwd_relay, HIGH);
                      digitalWrite(screw_rev_relay, LOW);
                      digitalWrite(y_valve_relay, LOW);
                      digitalWrite(g_valve_relay, LOW);                      
                    screw_complete = 1; //WE'VE DONE IT BOYS THE SCREW HAS MOVED INTO POSITION
                    cycle_start = 0;
                    clamp_close_tts = currentMillis+100+(sf_delay*1000);
                      Serial.print("MY CLAMP CLOSE TTS IS:");
                      Serial.print(clamp_close_tts);
                      Serial.println();
                    clamp_close_tte = clamp_close_tts+(clamp_close*1000);
                      Serial.print("MY CLAMP CLOSE TTE IS:");
                      Serial.print(clamp_close_tte);
                      Serial.println();
                    air_blow_tts = clamp_close_tte+250;  // 1/4 SECOND DELAY BETWEEN CLAMP CLOSE FINISH AND AIR BLOW BEGINNING
                      Serial.print("MY AIR BLOW TTS IS:");
                      Serial.print(air_blow_tts);
                      Serial.println();
                    air_blow_tte = air_blow_tts+(air_time*1000);
                      Serial.print("MY AIR BLOW TTE IS:");
                      Serial.print(air_blow_tte);
                      Serial.println();                          
                    shot_rev_tts = air_blow_tts+100; 
                      Serial.print("MY SHOT REVERSE TTS IS:");
                      Serial.print(shot_rev_tts);
                      Serial.println();
                    shot_rev_tte = shot_rev_tts+(shot_back*1000); // 
                      Serial.print("MY SHOT REVERSE TTE IS:");
                      Serial.print(shot_rev_tte);
                      Serial.println();                          
                    clamp_open_tts = air_blow_tte+(air_delay*1000);
                      Serial.print("MY CLAMP OPEN TTS IS:");
                      Serial.print(clamp_open_tts);
                      Serial.println();
                    clamp_open_tte = clamp_open_tts+(clamp_open*1000); // 
                      Serial.print("MY CLAMP OPEN TTE IS:");
                      Serial.print(clamp_open_tte);
                      Serial.println();
                    y_valve_tte1 = clamp_close_tte;
                      Serial.print("MY Y VALVE TTE PART 1 IS:");
                      Serial.print(y_valve_tte1);
                      Serial.println();
                    y_valve_tte2 = clamp_open_tte;
                      Serial.print("MY Y VALVE TTE PART 2 IS:");
                      Serial.print(y_valve_tte2);
                      Serial.println();
                    y_valve_tts1 = clamp_close_tts;
                      Serial.print("MY Y VALVE TTS PART 1 IS:");
                      Serial.print(y_valve_tts1);
                      Serial.println();           
                    y_valve_tts2 = clamp_open_tts;
                      Serial.print("MY Y VALVE TTS PART 2 IS:");
                      Serial.print(y_valve_tts2);
                      Serial.println();                                                                                 
                    cycle_time = (clamp_open_tte - startMillis)/10;
                      Serial.print("MY CYCLE TIME IS:");
                      Serial.print(cycle_time);
                      Serial.println();               
                 
                  }

                  if (screw_pos >= screw_return_pos && screw_complete == 1) {
                    Serial.println("THE EAGLE SCREW HAS LANDED");
                    Serial.println(screw_pos);
                    digitalWrite(screw_rev_relay, HIGH);
                    digitalWrite(g_valve_relay, HIGH);
                    screw_complete = 0; 
                  
                  }
                    
                  if (currentMillis >= clamp_close_tts && currentMillis <= clamp_close_tte) { //WE ARE TURNING THE CLAMP CLOSE RELAY ON, AND THEN BACK OFF HERE
                     // turn LED on:
                    digitalWrite(ledPin, HIGH);
                    digitalWrite(clamp_close_relay, LOW);
                    digitalWrite(y_valve_relay, LOW);
                    //lcd2.setCursor(12,3); 
                    //lcd2.print ("CLAMP CL");//prints clamp close relay status to LCD
                    Serial.println("I'm turning on the clamp close relay and Y valve relay now");
                  }
                    else {
                      digitalWrite(clamp_close_relay, HIGH);
                      }
                      
                  if (currentMillis >= air_blow_tts && currentMillis <= air_blow_tte) { //WE ARE TURNING THE AIR RELAY ON, AND THEN BACK OFF HERE
                     // turn LED on:
                    digitalWrite(ledPin, HIGH);
                    digitalWrite(air_on_relay, LOW);
                    //lcd2.setCursor(12,3); 
                    //lcd2.print ("AIR ON  ");//air relay status to LCD
                    Serial.println("I'm turning on the air relay now");
                      }
                        else {
                          digitalWrite(air_on_relay, HIGH);
                        }
                        
                  if (currentMillis >= clamp_open_tts && currentMillis <= clamp_open_tte) { 
                     // turn LED on:
                    digitalWrite(ledPin, HIGH);
                    digitalWrite(clamp_open_relay, LOW);
                    digitalWrite(y_valve_relay, LOW);
                    digitalWrite(g_valve_relay, HIGH);
                    cycle_start2 = 0;
                    //lcd2.setCursor(12,3); 
                    //lcd2.print ("CLAMP OP");//prints the clamp open relay status to LCD
                    Serial.println("I'm turning on the clamp open and Y valve relay now");
                      }
                        else {
                           digitalWrite(clamp_open_relay, HIGH);                                          
                        }               

                  if (currentMillis >= y_valve_tte2 && currentMillis <= y_valve_tte2+100) {
                    digitalWrite(y_valve_relay, HIGH);
                    Serial.println("I'm turning off the y valve relay now");
                  }
                  if (currentMillis >= g_valve_tte1 && currentMillis <= g_valve_tts2) {
                    digitalWrite(g_valve_relay, HIGH);
                    Serial.println("I'm turning off the G valve relay now");
                  }

                  if (currentMillis >= g_valve_tte2 && currentMillis <= g_valve_tte2) {
                    digitalWrite(g_valve_relay, HIGH);
                    Serial.println("I'm turning off the G valve relay now");
                  }
                  if (currentMillis >= clamp_open_tte+100 && cycle_start2 == 0) {
                    digitalWrite(clamp_close_relay, HIGH);
                    digitalWrite(clamp_open_relay, HIGH);
                    digitalWrite(air_on_relay, HIGH);
                    digitalWrite(screw_fwd_relay, HIGH);
                    digitalWrite(screw_rev_relay, HIGH);
                    digitalWrite(g_valve_relay, HIGH);
                    digitalWrite(y_valve_relay, HIGH);    
                    Serial.println("I'VE TURNED OFF ALL THE RELAYS. PRAY I DO NOT FURTHER TURN THEM OFF");                               
                  }
                  
                    

// GOOD JOB TOM CRUISE YOU MADE IT OUT OF THE DANGER ZONE
// GOOD JOB TOM CRUISE YOU MADE IT OUT OF THE DANGER ZONE
// GOOD JOB TOM CRUISE YOU MADE IT OUT OF THE DANGER ZONE
// GOOD JOB TOM CRUISE YOU MADE IT OUT OF THE DANGER ZONE

// WRITING VARIABLES TO THE DISPLAY
            // WRITE VARIABLES TO DISPLAY 1
                lcd1.setCursor(15,0);
                lcd1.print (shot_fwd); //prints purge forward time
              // set cursor position to start of the next line
                lcd1.setCursor(15,1);
                lcd1.print (sf_delay); //prints clamp close time
              // set cursor position to start of the next line
                lcd1.setCursor(15,2);
                lcd1.print (air_time); //prints air blow time
              // set cursor position to start of the next line
                lcd1.setCursor(15,3); 
                lcd1.print (air_delay);//prints recovery (shot reverse) value
            
            //  WRITE VARIABLES TO DISPLAY 2
                lcd2.setCursor(15,0);
                lcd2.print (shot_back); //prints purge forward time
              // set cursor position to start of the next line
                lcd2.setCursor(15,1);
                lcd2.print (clamp_open); //prints clamp close time
              // set cursor position to start of the next line
                lcd2.setCursor(15,2);
                lcd2.print (clamp_close); //prints air blow time
              // set cursor position to start of the next line
                lcd2.setCursor(15,3); 
                lcd2.print (cycle_time);//prints recovery (shot reverse) value
              // set cursor position to start of the next line

//END OF WRITING THE VARIABLES TO THE DISPLAY
}
