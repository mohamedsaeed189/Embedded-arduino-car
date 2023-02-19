#include <Arduino_FreeRTOS.h>



 const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

//ldr
const int LED = 24; // set pin for led
const int LDR = A1; //set pin for photoresistor
int inputval=0; //variable for values to be read from A1
int outval =0; //variable for output value from A1

//7segment & joystick
int x=12;
int y=13;
int xValue= -5;
int yValue=-5;



int b=11; 
int cd=10; 
int fa=9; 
int ge=8; 

//take care of pwm pins

void clearDisplay(void) 
{ 
  digitalWrite(fa,LOW);
  digitalWrite(b,LOW);
  digitalWrite(cd,LOW);
  digitalWrite(ge,LOW);
} 
void displayD(void) 
{ 
  digitalWrite(b,HIGH);
  digitalWrite(cd,HIGH);
  digitalWrite(ge,HIGH);  
  digitalWrite(fa,LOW); 
} 
void displayP(void) 
{ 
  digitalWrite(fa,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(ge,HIGH);  
  digitalWrite(cd,LOW);
}
void displayN(void) 
{ 
  digitalWrite(fa,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(ge,LOW);  
  digitalWrite(cd,LOW);
}
void displayR(void) 
{ 
  digitalWrite(fa,HIGH);
  digitalWrite(b,LOW);
  digitalWrite(ge,LOW);  
  digitalWrite(cd,LOW);
}


//lane keeping
int leftBackward =7;
int leftForward = 6;
int rightForward = 5;
int rightBackward = 4;
int driverEnable=3;
int rightLaneAssist = 22; //from front reverse
int leftLaneAssist= 23;
int lanebuzzer=2;



TaskHandle_t Handle_bTask;
TaskHandle_t Handle_cTask;
TaskHandle_t Handle_dTask;


	static void TaskB(void* pvParameters) {//LDR
   // Serial.println("Task B: Started");
 
    while(1){
            
    inputval = analogRead(LDR); 
  outval = map(inputval, 0, 1023, 255, 0); //The 0-1023 can be changed depending on light from your setup

  int lightval = constrain(outval, 0,255);//the contain output values within 0-255 range
  analogWrite(LED, lightval);     
        Serial.println("Task B: Started");
      vTaskDelay(xDelay );


  }
   
}

	static void TaskC(void* pvParameters) {//joystick and 7 segment
 //   Serial.println("Task C: Started");
 
    while(1){
            
 xValue = digitalRead(x);
  yValue = digitalRead(y);   

  if(xValue==0 && yValue==0){
      clearDisplay(); 
      displayP();        
      
  }
  if(xValue==0 && yValue==1){
      clearDisplay(); 
      displayR();        
  }    
  if(xValue==1 && yValue==0){
      clearDisplay(); 
      displayN();         
  }  
  if(xValue==1 && yValue==1 ){
      clearDisplay();
      displayD();       
  }  
        Serial.println("Task C: Started");

     vTaskDelay(xDelay );
      }
   
   
}

static void TaskD(void* pvParameters) {//lane keeping
   // Serial.println("Task D: Started");
 
    while(1){
      //digitalWrite(leftBackward,HIGH);
    //digitalWrite(leftForward,HIGH);
    //digitalWrite(rightForward,HIGH);
    //digitalWrite(rightBackward,HIGH);  
    int rla = digitalRead(rightLaneAssist);
    int lla = digitalRead(leftLaneAssist); 

    if(lla==LOW && rla==LOW)
      {
      digitalWrite(leftForward,HIGH);
      digitalWrite(rightBackward,HIGH);
      digitalWrite(lanebuzzer,LOW);

       analogWrite(driverEnable,40);      
      }
    else if(lla==LOW){
      digitalWrite(leftForward,HIGH);
      digitalWrite(rightBackward,LOW);
      digitalWrite(lanebuzzer,HIGH);


    analogWrite(driverEnable,40);        
    }
    else if (rla==LOW){
      digitalWrite(leftForward,LOW);
      digitalWrite(rightBackward,HIGH);
      digitalWrite(lanebuzzer,HIGH);

      
   analogWrite(driverEnable,40);  
    }
    else{
      digitalWrite(leftForward,LOW);
      digitalWrite(rightBackward,LOW);
       digitalWrite(lanebuzzer,HIGH);
      
    analogWrite(driverEnable,0);  
    }    
        Serial.println("Task D: Started");

    vTaskDelay(xDelay );

    }
    
    }

void setup() {
  //ldr
   Serial.begin(9600);
   pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);

  //joy stick and 7 segment
   //Serial.begin(9600);
  pinMode(x,INPUT);
  pinMode(y,INPUT);
  pinMode(fa,OUTPUT);    
  pinMode(b,OUTPUT);
  pinMode(cd,OUTPUT);
  pinMode(ge,OUTPUT);

  //lane keeping
  pinMode(leftBackward,OUTPUT);
  pinMode(leftForward,OUTPUT);
  pinMode(rightBackward,OUTPUT);
  pinMode(rightForward,OUTPUT);
  pinMode(driverEnable,OUTPUT);
  pinMode(leftLaneAssist, INPUT);
  pinMode(rightLaneAssist, INPUT);  
  pinMode(lanebuzzer,OUTPUT);



    // Create the threads that will be managed by the rtos
    // Sets the stack size and priority of each task
    // Also initializes a handler pointer to each task, which are important to communicate with and retrieve info from tasks
    xTaskCreate(TaskB,     "Task B",       256, NULL,  1, &Handle_bTask); //ldr
    xTaskCreate(TaskC,     "Task C",       256, NULL, 2, &Handle_cTask); //joystick gear
    xTaskCreate(TaskD,     "Task D",       256, NULL,  2, &Handle_dTask); //lane
 
    // Start the RTOS, this function will never return and will schedule the tasks.
    vTaskStartScheduler();   
}

void loop() {
  // put your main code here, to run repeatedly:

}
