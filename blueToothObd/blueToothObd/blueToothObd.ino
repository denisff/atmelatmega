/*AT
OK
AT+CMODE=0
OK
AT+ROLE=1
OK
AT+INIT
OK
AT+INQ
+INQ:4D65:4D:CA6612,5A0204,7FFF
OK
AT+BIND=4D65,4D,CA6612
OK
AT+PAIR=4D65,4D,CA6612,30
OK*/
/* AT+BAUD1-------1200
    AT+BAUD2-------2400
    AT+BAUD3-------4800
    AT+BAUD4-------9600 - Default for hc-06
    AT+BAUD5------19200
    AT+BAUD6------38400
    AT+BAUD7------57600 - Johnny-five speed
    AT+BAUD8-----115200
    AT+BAUD9-----230400
    AT+BAUDA-----460800
    AT+BAUDB-----921600
    AT+BAUDC----1382400 */
#include <SoftwareSerial.h>
#define Rx 5                //Arduino pin connected to Tx of HC-05
#define Tx 6               //Arduino pin connected to Rx of HC-05
#define Reset 9              //Arduino pin connected to Reset of HC-05 (reset with LOW)
#define PIO11 7             //Arduino pin connected to PI011 of HC-05 (enter AT Mode with HIGH)
// Swap RX/TX connections on bluetooth chip

#define idle 1
#define Sendcommand 0
#define setMask 2
#define wait 3
#define getresponse 4
#define show 5
int state= 0;
boolean  bt_error_flag=false;
void sendATCommand(String command);
void enterATMode();
void resetBT();
void enterComMode();
void setupBlueToothConnection();
SoftwareSerial blueToothSerial(Rx, Tx); // RX, TX
void setupOBD2();
//#define mac "1014,05,220194"
#define mac "000D, 18, 3A6789" //"1014,05,220194" 0004,3E,315B53 "0D,18,3A6789" A4DB,30,F0DFCD
char buffer[70];

void setup()
{

pinMode(Rx, INPUT);
   pinMode(Tx, OUTPUT);
   pinMode(PIO11, OUTPUT);
   pinMode(Reset, OUTPUT);
   
   digitalWrite(PIO11, LOW);    //Set HC-05 to Com mode
   digitalWrite(Reset, HIGH);   //HC-05 no Reset

 blueToothSerial.begin(38400);
 
  Serial.begin(38400);
while (!Serial) {
	; // wait for serial port to connect. Needed for Leonardo only
}
Serial.println("Starting config");

setupBlueToothConnection();
setupOBD2();
}

void waitForResponse() {
	//while (!blueToothSerial.available());
	delay(5000);
	while (blueToothSerial.available()) {
		Serial.write(blueToothSerial.read());
	}
	Serial.write("\n");


	delay(100);
}

void loop()
{
int i=0;
delay(5000);
switch(state) {
	case Sendcommand : blueToothSerial.write("atcra412\r");
					waitForResponse();	
					blueToothSerial.write("atma\r"); 
					state=idle;break;
					
	case idle: if (blueToothSerial.available()>0) state=getresponse; else break;
	case getresponse: if (blueToothSerial.read()=='>') {
						while (blueToothSerial.available()>0) {
							buffer[i]=(blueToothSerial.read());
							i++;
						}
							blueToothSerial.write("atb\r");
							waitForResponse();
							state=show; break;
							} break;
	 default : state=idle; break;
}



}

void setupOBD2(){
blueToothSerial.write("atz\r");
waitForResponse();
blueToothSerial.write("atsp6\r");
waitForResponse();
blueToothSerial.write("ate0\r");
waitForResponse();
blueToothSerial.write("ath1\r");
waitForResponse();
blueToothSerial.write("atcaf0\r");
waitForResponse();
blueToothSerial.write("atS0\r");
waitForResponse();

}
void setupBlueToothConnection()
{
  
  bt_error_flag=false;                    //set blue tooth error flag to false
  
  enterATMode();                          //enter HC-05 AT mode
  delay(500);
  sendATCommand("STATE?");  
  sendATCommand("RESET");                  //send to HC-05 RESET
  delay(1000);
  sendATCommand("ORGL");                   //send ORGL, reset to original properties
  sendATCommand("ROLE=1");                 //send ROLE=1, set role to master
  sendATCommand("PSWD=1234");
  sendATCommand("CMODE=0");                //send CMODE=0, set connection mode to specific address
 sendATCommand("ADCN"); 
 sendATCommand("INQM?");
 
  sendATCommand("INIT");                   //send INIT, cant connect without this cmd 
  delay(1000); 
  sendATCommand("INQ");
  sendATCommand("BIND=mac");    //send BIND=??, bind HC-05 to OBD bluetooth address
  sendATCommand("PAIR=000D,18,3A6789,1234"); //send PAIR, pair with OBD address +pinkode
  delay(1000); 
 Serial.println("Pair");
  sendATCommand("STATE?");
  sendATCommand("LINK=000D,18,3A6789");    //send LINK, link with OBD address 1234,
 Serial.println("link");
  sendATCommand("STATE?");
  enterComMode();                          //enter HC-05 communication mode

 
  delay(500);
}
void resetBT()
{
 digitalWrite(Reset, LOW);
 delay(2000);
 digitalWrite(Reset, HIGH);
}

//----------------------------------------------------------//
//--------Enter HC-05 bluetooth module command mode---------//
//-------------set HC-05 mode pin to LOW--------------------//
void enterComMode()
{
 blueToothSerial.flush();
 delay(500);
 digitalWrite(PIO11, LOW);
 //resetBT();
 delay(500);
 blueToothSerial.begin(38400); //default communication baud rate of HC-05 is 38400
}

//----------------------------------------------------------//
//----------Enter HC-05 bluetooth moduel AT mode------------//
//-------------set HC-05 mode pin to HIGH--------------------//
void enterATMode()
{
 blueToothSerial.flush();
 delay(500);
 digitalWrite(PIO11, HIGH);
 //resetBT();
 delay(500);
 blueToothSerial.begin(38400);//HC-05 AT mode baud rate is 38400

}

//----------------------------------------------------------//

void sendATCommand(String command)
{
  
  char recvChar;
  char str[2];
  int i,retries;
  boolean OK_flag;
  
 // if (!(bt_error_flag)){                                  //if no bluetooth connection error
    retries=0;
    OK_flag=false;
    
   // while ((retries<BT_CMD_RETRIES) && (!(OK_flag))){     //while not OK and bluetooth cmd retries not reached
      
       blueToothSerial.print("AT");                       //sent AT cmd to HC-05
       if(command.length() > 1){
         blueToothSerial.print("+");
         blueToothSerial.print(command);
       }
       blueToothSerial.print("\r\n");
	   waitForResponse();
    /*  
      while (blueToothSerial.available()<=0);              //wait while no data
      
      i=0;
      while (blueToothSerial.available()>0){               // while data is available
        recvChar = blueToothSerial.read();                 //read data from HC-05
          if (i<2){
            str[i]=recvChar;                               //put received char to str
            i=i+1;
          }
      }
      retries=retries+1;                                  //increase retries 
      if ((str[0]=='O') && (str[1]=='K')) OK_flag=true;   //if response is OK then OK-flag set to true
      delay(1000);
    }
  
    if (retries>=BT_CMD_RETRIES) {                        //if bluetooth retries reached
      bt_error_flag=true;                                 //set bluetooth error flag to true
    }
  } */
  
}

/*


// Should respond with OK
blueToothSerial.print("AT\r\n");
waitForResponse();
 
  blueToothSerial.print("AT\r\n");
waitForResponse();
// Should respond with its version
blueToothSerial.print("AT+CMODE=1\r\n");
waitForResponse();

// Set pin to 0000
blueToothSerial.print("AT+ROLE=1\r\n");
waitForResponse();

// Set the name to ROBOT_NAME



//blueToothSerial.print("AT+INIT\r\n");
//waitForResponse();
// Set baudrate to 57600 AT+BAUD7"
//blueToothSerial.print("AT+INQ\r\n");
//waitForResponse();
//blueToothSerial.print("AT+BIND=mac\r\n");
//waitForResponse();
//blueToothSerial.print("AT+BIND?\r\n");
//waitForResponse();
blueToothSerial.print("AT+PAIR=mac,1234\r\n");
waitForResponse();
blueToothSerial.print("AT+FSAD=mac\r\n");
waitForResponse();
//blueToothSerial.print("AT+LINK=1234,mac\r\n"); //
//waitForResponse();
Serial.println("Done!");

// blueToothSerial.begin(BLUETOOTH_SPEED);
*/