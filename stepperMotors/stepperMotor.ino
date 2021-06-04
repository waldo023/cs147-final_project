#include <HttpClient.h>



#include "ICM_20948.h"


#include "application.h" 
#include <string.h>

#define BUTTONX D2
#define BUTTONY D3

#define DIRPIN_X D7
#define STEPSPIN_X D6
#define RSTPIN_X A4

#define DIRPIN_Y D5
#define STEPSPIN_Y D4
#define RSTPIN_Y A5



HttpClient http;

http_header_t headers[] = {

    { "Accept" , "*/*"},
    { NULL, NULL } 
};

http_request_t request;
http_response_t response;



void setup()
{
    Serial.begin(9600);



	// Declare pins as Outputs
	pinMode(STEPSPIN_X, OUTPUT);
	pinMode(DIRPIN_X, OUTPUT);	
	pinMode(RSTPIN_X, OUTPUT);
    digitalWrite(RSTPIN_X, HIGH);
    
    pinMode(STEPSPIN_Y, OUTPUT);
	pinMode(DIRPIN_Y, OUTPUT);	
	pinMode(RSTPIN_Y, OUTPUT);
    digitalWrite(RSTPIN_Y, HIGH);
    
    
}
void loop()
{
    int stepsX;
    int stepsY;
 
    request.hostname = "3.22.234.112";
    request.port = 5000;
    request.path = "/request/";

    http.get(request, response, headers);
    
    Serial.print("Application>\tResponse status: ");
    Serial.println(response.status);
    
    Serial.print("Application>\tHTTP Response Body:");
    Serial.println(response.body);
    
    char str[response.body.length()+1];
    strcpy(str, response.body.c_str());
    
    stepsX = atoi(strtok(str, " "));
    
    stepsY = atoi(strtok(NULL, " "));
    
    moveMotors(stepsX, stepsY);

}



void moveMotors(int stepsX, int stepsY){
    digitalWrite(RSTPIN_X,HIGH);
    digitalWrite(RSTPIN_Y,HIGH);

	if(stepsX < 0){
	    digitalWrite(DIRPIN_X, LOW);
	}else{
	    digitalWrite(DIRPIN_X, HIGH);
	}
	
	if(stepsY < 0){
	    digitalWrite(DIRPIN_Y, HIGH);
	}else{
	    digitalWrite(DIRPIN_Y, LOW);
	}

	Serial.print("X Steps: ");
    Serial.print(stepsX);
    
    Serial.print(" Y Steps: ");
    Serial.print(stepsY);
	
	Serial.println();
	
    stepsX = abs(stepsX);
    
	stepsY = abs(stepsY);

	
	while(stepsY > 0 || stepsX > 0){
	    if(stepsY > 0){
	        digitalWrite(STEPSPIN_Y, HIGH);
		    delayMicroseconds(2000);
		    digitalWrite(STEPSPIN_Y, LOW);
		    delayMicroseconds(2000);
		    stepsY--;
	    }
	    if(stepsX > 0){
	        digitalWrite(STEPSPIN_X, HIGH);
		    delayMicroseconds(2000);
		    digitalWrite(STEPSPIN_X, LOW);
		    delayMicroseconds(2000);
		    stepsX--;
	    }

	}
}

