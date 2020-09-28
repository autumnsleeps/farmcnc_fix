#include <AccelStepper.h>
#include <MultiStepper.h>
//AccelStepper stepper(MotorInterface, StepPin, DirPin)
AccelStepper stepperZ(1,8,13);
//AccelStepper stepperZ(1,9,8);
AccelStepper stepperY(1,5,4);
AccelStepper stepperX(1,2,3);

MultiStepper steppers;

#define revBtnX 7
#define revBtnY 6
#define revBtnZ 11
#define waterPump 53

long initStepX = 1; //old homing process
long initStepY = 1; //old homing process
long initY = 1;
long initX = 1;
long initZ = 1;
int moveFinished = 0;
int modeNumber;
byte enableMode2 = 0;
byte enableMode3 = 0;
String workingModeAndData;
int xTempMode2 = 0;
int yTempMode2 = 0;
int xTempMode3 = 0;
int yTempMode3 = 0;
byte smallCabbage = 0;
byte bigCabbage = 0;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    Serial1.begin(19200);
//    Serial1.begin(9600);
//    MySerial.begin(115200);
    pinMode(revBtnX, INPUT_PULLUP);
    pinMode(revBtnY, INPUT_PULLUP);
    pinMode(revBtnZ, INPUT_PULLUP);
    pinMode(waterPump, OUTPUT);
    digitalWrite(waterPump, LOW);
    stepperY.setSpeed(800); 
    stepperY.setAcceleration(300);
    stepperX.setSpeed(800); 
    stepperX.setAcceleration(300);
    stepperZ.setSpeed(700);
    stepperZ.setAcceleration(150);

    //First time turning on the machine, we should do the homing process firstly
    quickHomingProcess();
    Serial.println("--------------------------------------------------------------------------");
    Serial.println("The machine can work properly now. Enter your mode and working properties:");
     
    while(1){
        if(Serial1.available()){
            workingModeAndData = Serial1.readStringUntil('#');

            Serial.println(workingModeAndData);

            if(workingModeAndData.startsWith("*")){
                //Xóa * và # ở đầu và cuối chuỗi
                workingModeAndData = workingModeAndData.substring(1,workingModeAndData.length()); 

                workingModeAndData.concat(";");

                String value[5]; 
            
                for (int i=0; i<5; i++){
                    value[i] = workingModeAndData.substring(0,workingModeAndData.indexOf(";",0));
                    workingModeAndData.remove(0,workingModeAndData.indexOf(";",0)+1);
                }
            }
        }
    }
}

void quickHomingProcess(){
    int speedHomingY = 1000;
    int accelerationHomingY = 300;
    int speedHomingX = 800;
    int accelerationHomingX = 300;
    int speedHomingZ = 700;
    int accelerationHomingZ = 150;

    while(digitalRead(revBtnZ)){
        stepperZ.setMaxSpeed(speedHomingZ);
        stepperZ.setAcceleration(accelerationHomingZ);
        stepperZ.moveTo(-initZ);
        initZ = initZ + 1;
        stepperZ.run();
    }
     
    while(digitalRead(revBtnY)){
        stepperY.setMaxSpeed(speedHomingY);
        stepperY.setAcceleration(accelerationHomingY);
        stepperY.moveTo(initY);
        initY = initY + 1;
        stepperY.run();
    }

    while(digitalRead(revBtnX)){
        stepperX.setMaxSpeed(speedHomingX);
        stepperX.setAcceleration(accelerationHomingX);
        stepperX.moveTo(initX);
        initX = initX + 1;
        stepperX.run();
    }
    
    stepperY.setCurrentPosition(0);
    stepperY.setMaxSpeed(1000);
    stepperY.setAcceleration(1000);
    delay(5);
    stepperX.setCurrentPosition(0);
    stepperX.setMaxSpeed(500);
    stepperX.setAcceleration(500);
    delay(5);
    Serial.println("The homing process is complete!");
}

void homingZAxis(){
    while(digitalRead(revBtnZ)){
        stepperZ.setMaxSpeed(800);
        stepperZ.setAcceleration(300);
        stepperZ.moveTo(-initZ);
        initZ++;
        stepperZ.run();
    } 
}

void moveByXYCoordinates(int xCoords, int yCoords){
    if(enableMode2 = 1){
        int xTempResultMode2 = xCoords - xTempMode2;
        int yTempResultMode2 = yCoords - yTempMode2;
        if((yTempResultMode2)!=0){
            stepperY.move(yTempResultMode2);
            stepperY.runToPosition();
        }
        if((xTempResultMode2)!=0){
            stepperX.move(xTempResultMode2);
            stepperX.runToPosition();  
        }
        stepperY.setCurrentPosition(0);
        stepperY.setMaxSpeed(1000);
        stepperY.setAcceleration(300);
        delay(50);
        stepperX.setCurrentPosition(0);
        stepperX.setMaxSpeed(600);
        stepperX.setAcceleration(400);
    }
    xTempMode2 = xCoords;
    yTempMode2 = yCoords;
}

void loop() {
  // put your main code here, to run repeatedly:

}
