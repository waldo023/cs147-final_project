
#include <HttpClient.h>


#include "ICM_20948.h"



HttpClient http;

http_header_t headers[] = {
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers with NULL
};

http_request_t request;
http_response_t response;

ICM_20948_I2C myICM;

float noiseMin_X = 100.0;
float noiseMax_X= -100.0;

float noiseMin_Y = 100.0;
float noiseMax_Y= -100.0;

int calibTime = 6;
unsigned int calibEnd;

float t = 0.08;

float stepsPerRevolution = 200; 
float degPerMM = 22.73;

void setup() {

    Serial.begin(115200);
    while(!Serial){};
    
    Wire.begin();
    Wire.setClock(400000);
    
    bool initialized = false;
    while( !initialized ){
        myICM.begin(Wire,1);
        
        Serial.print( F("Initialization of the sensor returned: ") );
        Serial.println( myICM.statusString() );
        if( myICM.status != ICM_20948_Stat_Ok ){
          Serial.println( "Trying again..." );
          delay(500);
        }else{
          initialized = true;
        }
    }
    
    myICM.swReset();
    if (myICM.status != ICM_20948_Stat_Ok)
    {
        Serial.print(F("Software Reset returned: "));
        Serial.println(myICM.statusString());
    }
    delay(250);
    
    // Now wake the sensor up
    myICM.sleep(false);
    myICM.lowPower(false);
    calibEnd = millis()*calibTime;
}

void loop() {
    int stepsX;
    int stepsY;
    if( myICM.dataReady() ){
    
        myICM.getAGMT();   
        float xval = myICM.accX()/1000*9.8;
        float yval = myICM.accY()/1000*9.8;
    
    
        if(millis() < calibEnd){    //noise calibration
            if(xval < noiseMin_X){
                noiseMin_X = xval;
            }else if(xval > noiseMax_X){
                noiseMax_X = xval;
            }
            if(yval < noiseMin_Y){
                noiseMin_Y = yval;
            }else if(yval > noiseMax_Y){
                noiseMax_Y = yval;
            }
            Serial.print(".");
            delay(30);
            return;
        }
    
        else{

            float distX = movement(xval, noiseMin_X, noiseMax_X );
            float distY = movement(yval, noiseMin_Y, noiseMax_Y );
            
            
            stepsX = distX*degPerMM/(360/stepsPerRevolution);
            stepsY = distY*degPerMM/(360/stepsPerRevolution);
        
        }
    
    }
    else{
        Serial.println("Waiting for data");
        delay(500);
        return;
    }
    
    request.hostname = "3.22.234.112";
    request.port = 5000;
    request.path = "/?X=" + String(stepsX) + "&Y=" + String(stepsY);
    
    http.post(request, response, headers);
    
    Serial.print("Application>\tResponse status: ");
    Serial.println(response.status);
    
    Serial.print("Application>\tHTTP Response Body:");
    Serial.println(response.body);
    
    Serial.print("X Steps: ");
    Serial.print(stepsX);
    
    Serial.print(" Y Steps: ");
    Serial.print(stepsY);
	
	Serial.println();
	
	delay(80);
}

float movement(float accel,  float min, float max){
    if(accel > min and accel < max){
        return 0.0;
    }
    float dist = .5*(accel)*t*t;
    return dist*1000;
}

