import processing.serial.*;

//initialization 
Serial port; //create object from serial class

//declare sensor variables
int valP_temp; // Data received from the serial port - variable to store the light sensor reading
int valP_distance; 
int valP_lightSensor;

//boolean values that correlate to whether or not a certain light value was recceived (which will then dispense that candy using servos in arduino)
boolean candy1 = false;
boolean candy2 = false;
boolean candy3 = false;
boolean candy4 = false;
String lightVal = ""; 

byte[] inBuffer = new byte[5314]; //array of bytes for incoming data


//initialize states
final int state1_sleeping = 1;
final int state2_greeting = 2;  //also has instruction state in this one where it will switch in that method
final int state3_dispensing = 3;

//declare photo assets
PImage state1_image;
PImage state2_firstimage;
PImage state2_secondimage;
PImage state3_image;


//declare screen_state
int screen_state;

//declare boolean variable for if space bar pressed during state 2 (screen shows second set of instructions when user presses space)
boolean spacePressed = false;

void setup() {
 //set dimensions of window (UI)
 size (2500, 2500);   
 port = new Serial(this, "dev/cu.usbserial-210", 9600); //open port
 loadAssets();
 //set initial state at sleeping
 screen_state = 1; 
 background(255, 255, 255); 
 showScreen(state1_image);
}


void draw(){
  print("the screen state is:", screen_state);
  //check which screen state currently in, then draw accordingly 
  println("running"); //for testing purposes
  
  //intiial setup, gets string val with "&" eliminated
  print(port.available());
  if (0 < port.available()) { // If data is available to read,
    
    println(" ");
    
    port.readBytesUntil('&', inBuffer);  //read in all data until '&' is encountered
    
    if (inBuffer != null) {
      String myString = new String(inBuffer);
      //println(myString);  //for testing only
      
      
      //p is all sensor data (with t's, d's, l's ('&' is eliminated) ///////////////
      
      String[] p = splitTokens(myString, "&");  
      if (p.length < 2) return;  //exit this function if packet is broken
      //println(p[0]);   //for testing only
      
  if (screen_state == 1) {
     showScreen(state1_image);
     
     //get distance sensor reading //////////////////////////////////////////////////
      
     String[] distance_sensor = splitTokens(p[0], "d");  //get distance sensor reading, eliminates 'd'
     String distanceVal= "";    
     
     for (int i = 0; i< distance_sensor.length; i++)  {  //due to varied length of distance values read, loop to end of array and create a distance val based on that
      distanceVal = distanceVal + distance_sensor[i]; 
      }
      
      int distanceValNum = 0;
      distanceValNum = int(distanceValNum);   //convert distance string val to an integer value that can be compared numerically
      
      if (valP_distance < 5) { //if user waves close enough to the machine, it "awakens" and goes to state 2
         screen_state = 2;
      }
      
      
      //for testing purposes
      print("distance sensor: ");
      print(valP_distance);
      println(" ");  
      
  }
  
  else if (screen_state == 2) {
     if (spacePressed == false) { //only show first image if not pressed space yet
       showScreen(state2_firstimage);
     }
       //if space bar pressed, only checks if keypressed if in state 2 alrdy
     else if (spacePressed == true) {
        print("space pressed");
        showScreen(state2_secondimage); //show second set of instructions to user
        print("candy1 is: ", candy1);  //for testing purposes
        print("candy2 is: ", candy2);
        print("candy3 is: ", candy3);
        print("candy4 is: ", candy4);
        println("light val", lightVal);
         
          
      //get light sensor reading /////////////////////////////////////////////////
      
      String[] light_sensor = splitTokens(p[0], "a");  //get light sensor reading 
         //get light sensor val
         if (light_sensor.length == 3) { //{l9l}, light value is 1 digit
         lightVal = light_sensor[1]; //lightVal = "1";
         }

         else if (light_sensor.length == 4) { //{l99}, light value is 2 digits
           lightVal = light_sensor[1] + light_sensor[2];
         }
         else if (light_sensor.length == 5) { //{l999}, light value is 3 digits, highest its gone 
           lightVal = light_sensor[1] + light_sensor[2] + light_sensor[3];
         }
         int lightValNum = 0;
         lightValNum = (int(lightVal)); 
      
      if (lightValNum  >= 17 && lightValNum <= 33) {  //each threshold corresponds to a certain candy being outputted, turn the candy boolean true if 
                                                      //their light Val threshold is met
        candy1 = true;
        println("candy1", candy1);
      }
      
      else if (lightValNum>= 34 && lightValNum<= 40) {
        candy2= true;
        println("candy2", candy2);
      }
      
      else if (lightValNum>= 41 && lightValNum <= 51) {
        candy3 = true;
           println("candy3", candy3);
      }
      else if (lightValNum >= 53 && lightValNum<= 100) {
        candy4 = true;
        println("candy4", candy4);
      }
      
      if (candy1 ==true || candy2==true || candy3==true || candy4==true) {   //if any candys are true (only one will be true at a time,go to screen 3 (which shows 'dispensing your candy')
        screen_state = 3;
        print("candy1 is: ", candy1); //for testing purposes
        print("candy2 is: ", candy2);
        print("candy3 is: ", candy3);
        print("candy4 is: ", candy4);
      }
      
      print("light sensor:"); //uses string val from earlier
      print(lightVal);
      println(" ");  
          
  }
  

    }
    
    else if (screen_state == 3) {
      println("candy1", candy1);     
      showScreen(state3_image);
      delay(5000); //delay for five seconds to allow user to read "dispensing your candy" (and for the candy to dispense)
      reset();  //after 5 seconds, reset to screen state 1 and resets all variables to start the program again
    }
}
 
}
}

void showScreen(PImage img) {
  img.resize(1000, 0);
  image(img, 0, 0);
  
}

void keyPressed() {
  if ( key == ' ') 
     spacePressed = true;
     println("space bar pressed");
}

void loadAssets() {
 state1_image = loadImage("1sleeping.png");
 state2_firstimage = loadImage("2greeting.png");
 state2_secondimage = loadImage("2instruction.png");
 state3_image = loadImage("3dispensing.png");
}

void reset() { //ADD RESET CODE HERE 
 delay(4000);
 spacePressed = false; 
 screen_state = 1;
 candy1 = false;
 candy2 = false;
 candy3 = false;
 candy4 = false; 
}
