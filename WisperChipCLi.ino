//CAN-BUS Includes
//#include <SPI.h>
#include "mcp_can.h"
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

int arryCANID[127];
String arryCANMSG[127];
int IsInitiated=0;
int IsDumping=0;
int incomingByte;
char incomingChar;
int CommandValid;
String cmdBuild;
String StringValue;
int MsgColor=0;
int SlowDown=0;

String HexMode="off";

void setup() {
Serial.begin(500000);
Printwelcome();
}
 
void loop() {
   
  readincommingserial();


 
}
 
void Printwelcome()
{
  clearAndHome();
  dosworking();
  clearAndHome();
  Serial.println("");
  Serial.println("*********************************************************************");
  Serial.println("** Welcome to the WisperChip serial command line console.          **");
  Serial.println("** \033[1;31mWisperChip V1.01\033[0m  - Developed by Martin Viljoen                 **");
  Serial.println("*********************************************************************");
  Serial.println("Type \033[1;31mhelp\033[0m to get a list of commands.");
  Serial.println("");
}
void readincommingserial()
{
   
if (Serial.available() > 0) 
  {
   incomingByte = Serial.read();
   incomingChar = char(incomingByte);
      if (incomingByte != 13)
                {
                 if(incomingByte != 127){cmdBuild = cmdBuild + incomingChar;}else{cmdBuild= cmdBuild.substring(0,cmdBuild.length()-1);}//Remove Backspaces and the last mistake chracter
                }
                else
                {
                  Serial.println("");
                  if (cmdBuild=="help"){Printhelp();CommandValid=1;}
                  if (cmdBuild=="welcome"){Printwelcome();CommandValid=1;}
                  if (cmdBuild=="clear"){clearAndHome();CommandValid=1;}
                  if (cmdBuild=="reset"){ResetDuino();CommandValid=1;}
                  if (cmdBuild=="progress"){dosworking();CommandValid=1;}
                  if (cmdBuild=="print stringvalue"){Printstringvalue();CommandValid=1;}
                  if (cmdBuild.substring(0,16)=="set stringvalue "){Setstringvalue();CommandValid=1;} 
                  if (cmdBuild.substring(0,7)=="cltest "){ClTest();CommandValid=1;} 
                  if (cmdBuild.substring(0,9)=="slowdown "){SlowSystemDown();CommandValid=1;} 
                    
                  //Sample    
                  if (cmdBuild=="col"){PrintCol();CommandValid=1;}
                  //CAN Utils
                  if (cmdBuild=="caninit125"){initCAN125();CommandValid=1;}
                  if (cmdBuild=="caninit500"){initCAN500();CommandValid=1;}
                  if (cmdBuild=="candump"){IsDumping=1;candump();CommandValid=1;}
                  if (cmdBuild=="rawdump"){IsDumping=1;rawdump();CommandValid=1;}
                  if (cmdBuild=="killcan"){KillCan();CommandValid=1;}
                  if (cmdBuild=="emulate"){IsDumping=1;Emulator();CommandValid=1;}
                  if (cmdBuild.substring(0,11)=="bruteforce "){BruteForce();IsDumping=1;CommandValid=1;} 
                  if (cmdBuild.substring(0,8)=="hexmode "){HexModeOn();CommandValid=1;} 
                 
                  if (cmdBuild=="setcur"){SetCursor();CommandValid=1;}
                  if (cmdBuild=="ls mem"){ListMemory();CommandValid=1;}
                  
 
                  
                  
                   
                                                                    
                  //Check for invalid command.;
                  if(CommandValid==0){Serial.println("'"  + cmdBuild + "' is not recognized as an internal or external command."); IsDumping=0;}
                  //Clear Last Command
                  cmdBuild="";
                  CommandValid=0;
                }
     //Repeat the key strokes
     Serial.print(incomingChar);    
     //Serial.print(incomingByte);//Uncomment this is for debugging purposes.
  }
}  
 
void Printhelp()
{
  Serial.println("\033[1;31mHelp\033[0m - Displays a list of valid commands.");
  Serial.println("-----------------------------------------------------------------------------------------");
  Serial.println("help                 Shows this help screen.");
  Serial.println("welcome              Reprint the welcome screen.");
  Serial.println("clear                Clear the console screen.");  
  Serial.println("cltest [n]           VT100 color test");
  Serial.println("reset                Reset/Reboot Arduino");
  //Serial.println("progress             DOS style working in progress");
  //Serial.println("set stringvalue [n]  Set the integer to a value I.E set integer 1.");
  //Serial.println("print stringvalue    Print the current value of the string.");
  Serial.println("");
  Serial.println("caninit125           Init can-bus shield at 125Kbps.");
  Serial.println("caninit500           Init can-bus shield at 500Kbps.");  
  Serial.println("candump              Dump all CAN-BUS data to console while monitoring changing messages.");
  Serial.println("rawdump              Dump all CAN-BUS data to console without monitoring. ");
  Serial.println("emulate              Emulate the CAN-BUS.");
  Serial.println("bruteforce [n]       Perform brute force attack from ID");
  Serial.println("slowdown [n]         Create a delay between reading messages. Default=0.");
  Serial.println("ls mem               Print CAN messages currently in memory (Warning Buggy)");
  Serial.println("hexmode [on][off]    Show CAN-BUS message using HEX values instead of numbers.");
  Serial.println("killcan              Disconnect CAN-BUS.");
  Serial.println("-----------------------------------------------------------------------------------------");
  
}
void HexModeOn()
{
  StringValue = cmdBuild.substring(8,1000);
  StringValue.trim(); 
  if(StringValue=="off"){HexMode="off";}
  if(StringValue=="on"){HexMode="on";}

  Serial.println("HEX mode turned \033[1;31m" + StringValue + "\033[0m for CAN-BUS messages.");
  
}

void ListMemory()
{
    //int arryCANID[127];
    //String arryCANMSG[127];
    for (int x = 0; x < sizeof(arryCANID) / sizeof(arryCANID[0]); x++)
    {
      if(arryCANID[x]!=0)
      {
        Serial.print(arryCANID[x]);
        Serial.print("");
        Serial.println(arryCANMSG[x]);
      }
    }
    Serial.println("Memory dump complete");
}
void ClTest()
{
  StringValue = cmdBuild.substring(7,1000);
  StringValue.trim();
  Serial.println("Printing Color:" + StringValue);
  Serial.println("\033[1;" + StringValue + "mThe quick brown fox jumps over the lazy dog\033[0m");
 
}
void ResetDuino()
{
  Serial.println("Not supported, Press the reset button on arduino mega.");
}

void SlowSystemDown()
{
  StringValue = cmdBuild.substring(9,1000);
  StringValue.trim();  
  SlowDown=atoi(StringValue.c_str());
  Serial.print(SlowDown);
  Serial.println("s delay between reading message created. \033[1;31mWarning\033[0m a delay halt the system for the duration of the slowdown.");
}
void Setstringvalue()
{
  StringValue = cmdBuild.substring(16,1000);
  StringValue.trim();
  Serial.println("String Value has been set to " +  StringValue);
 
}
void Printstringvalue()
{
  Serial.println("The current string value is " + StringValue);
}
void clearAndHome()
{
  Serial.write(27);
  Serial.print("[2J"); // clear screen
  Serial.write(27);
  Serial.print("[H"); // cursor to home
}

void PrintCol()
{
  //31 red
  //32 green
  Serial.println("\033[1;31mgreen text\033[0m plain text\n");
}

void initCAN125()
{
  Serial.println("Connecting to CAN-BUS @RATE: \033[1;33m125Kbps\033[0m");
  int InitCanTimeCout=0;
      while (CAN_OK != CAN.begin(CAN_125KBPS) && InitCanTimeCout<=20 ) 
    {
        InitCanTimeCout++;
        Serial.println("Try Attempt:" + String(InitCanTimeCout) + "\033[1;31mCAN BUS Shield init fail. Trying again.\033[0m");
        delay(100);
        clearAndHome();
    }
    IsInitiated=1;
    if (CAN_OK==CAN.begin(CAN_125KBPS)){ Serial.println("\033[1;32mCAN-BUS Shield Connected @BAUD RATE:125Kbps\033[0m");}
}
void initCAN500()
{
  Serial.println("Connecting to CAN-BUS @RATE: \033[1;33m500Kbps\033[0m");
  int InitCanTimeCout=0;
      while (CAN_OK != CAN.begin(CAN_500KBPS) && InitCanTimeCout<=20 )
    {
        InitCanTimeCout++;
        Serial.println("Try Attempt:" + String(InitCanTimeCout) + "\033[1;31mCAN BUS Shield init fail. Trying again.\033[0m");
        delay(100);
        clearAndHome();
    }
    IsInitiated=1;
    if (CAN_OK==CAN.begin(CAN_500KBPS)){ Serial.println("\033[1;32mCAN-BUS Shield Connected @BAUD RATE:500Kbps\033[0m");}
    
}
void KillCan()
{
  IsInitiated=0;
  Serial.println("CAN-BUS Shield disconnected.");
}




void Emulator()
{
//clear memory
memset(arryCANID, 0, sizeof(arryCANID));
memset(arryCANMSG, 0, sizeof(arryCANMSG));
clearAndHome();  





String BuildCANMSG;


int arryCount=0;
int ArryIDIsOld=0;
int ArryMessageIsOld=0;
int ArryRCPPOS = 1;
int ArryLastListPos=0;


 
   for (int j=1; j <= 50000; j++)
   { 
    if(IsDumping==0){break;}   
    int CANID=random(1000,1050);
    int BIT1=random(255,255);
    int BIT2=random(255,255);
    int BIT3=random(255,255);
    int BIT4=random(255,255);
    int BIT5=random(255,255);
    int BIT6=random(255,255);
    int BIT7=random(255,255);
    int BIT8=random(255,255);
    
    //change BIT8 every fe times
    int RandomBIT = random(1,1000);
    if(RandomBIT==1){CANID=1024;BIT1=1;BIT2=1;BIT3=1;}
    if(RandomBIT==2){CANID=1025;BIT1=2;BIT2=4;BIT3=4;}
    if(RandomBIT==3){CANID=1026;BIT1=3;BIT2=1;BIT3=5;}
    if(RandomBIT==4){CANID=1027;BIT1=4;BIT2=110;BIT3=156;}
    if(CANID>=1049){BIT1=random(200,255);BIT2=234;BIT3=215;}

    
    //SlowDown
    delay(SlowDown);
    if(HexMode=="off")
    {
      BuildCANMSG=String(BIT1) + " " +  String(BIT2) + " " +  String(BIT3) + " " +  String(BIT4)  + " " +  String(BIT5)  + " " +  String(BIT6)  + " " +  String(BIT7)  + " " +  String(BIT8);
      
    }else
    {
      BuildCANMSG=String(BIT1,HEX) + " " +  String(BIT2,HEX) + " " +  String(BIT3,HEX) + " " +  String(BIT4,HEX)  + " " +  String(BIT5,HEX)  + " " +  String(BIT6,HEX)  + " " +  String(BIT7,HEX)  + " " +  String(BIT8,HEX);
    }

    


    //Check if Received ID has already been received
    ArryIDIsOld=0;
    ArryMessageIsOld=0;
    for (int arryCount=1; arryCount <= 99; arryCount++)
    {
      if(arryCANID[arryCount] == CANID)
        {         
          ArryIDIsOld=1;          
          //Check If Message Changed
          if(arryCANMSG[arryCount]==BuildCANMSG){MsgColor=0;}else{MsgColor=41;}  
          

            //Clear line to prevent text from flowing over old test
            Serial.print("\033[" + String(arryCount) + ";1H");
            Serial.print("\033[0m;                                     ");
            //Print Fresh Line
            Serial.print("\033[" + String(arryCount) + ";1H");
            if(HexMode=="off"){Serial.print("\033[1;" + String(MsgColor)  + "m" + String(CANID) + " " +  BuildCANMSG);}else{Serial.print("\033[1;" + String(MsgColor)  + "m" + String(CANID,HEX) + " " +  BuildCANMSG);}
            Serial.print(" ");
            Serial.print(char(BIT1));
            Serial.print(char(BIT2));
            Serial.print(char(BIT3));
            Serial.print(char(BIT4));
            Serial.print(char(BIT5));
            Serial.print(char(BIT6));
            Serial.print(char(BIT7));
            Serial.print(char(BIT8));
          break;
        }
    }
    
    if(ArryIDIsOld==0)
    {
      ArryLastListPos++;
      //Print Message because it has not yet been received
     Serial.print("\033[" + String(ArryLastListPos) + ";1H");
     
     if(HexMode=="off"){Serial.print("\033[1;0m" + String(CANID) + " " +  BuildCANMSG);}else{Serial.print("\033[1;0m" + String(CANID,HEX) + " " +  BuildCANMSG);}
            Serial.print(" ");
            Serial.print(char(BIT1));
            Serial.print(char(BIT2));
            Serial.print(char(BIT3));
            Serial.print(char(BIT4));
            Serial.print(char(BIT5));
            Serial.print(char(BIT6));
            Serial.print(char(BIT7));
            Serial.print(char(BIT8));
      
      //Update Array with CAN ID so that it will not repeat a new line      
      arryCANID[ArryLastListPos] = CANID;
      arryCANMSG[ArryLastListPos]=BuildCANMSG;

    }

    readincommingserial();
    if(IsDumping==0){break;} 
   } 
   Serial.print("\033[" + String(ArryLastListPos+1) + ";1H");
   Serial.println("\033[1;0mDump Complete");
   

  

}

void SetCursor()
{
  //[{ROW}
  //Serial.println("\033[1;31mgreen text\033[0m plain text\n");
  Serial.println("\033[1;3H");
  Serial.print("T");
  Serial.println("\033[2;3H");
  Serial.print("E");
  Serial.println("\033[3;3H");
}




//Dump Can Bus data
void candump()
{
  int intNoDataCount=0;
  clearAndHome(); 
  
  //clean memory
  memset(arryCANID, 0, sizeof(arryCANID));
  memset(arryCANMSG, 0, sizeof(arryCANMSG));
  String BuildCANMSG;
  int arryCount=0;
  int ArryIDIsOld=0;
  int ArryMessageIsOld=0;
  int ArryRCPPOS = 1;
  int ArryLastListPos=0;  
  if(IsInitiated==0){Serial.println("CAN-BUS shield not initiated. Type \033[1;31mhelp\033[0m to get a list of initiation commands.");return;}
  unsigned char len = 0;
  unsigned char buf[8];

  int CANID,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8;
  
  while(IsDumping==1)
  {
  delay(SlowDown);
  readincommingserial();

      if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
      {
          intNoDataCount=0;
          CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
          unsigned long canId = CAN.getCanId();       
          CANID=canId;
          BIT1=buf[0];
          BIT2=buf[1];
          BIT3=buf[2];
          BIT4=buf[3];
          BIT5=buf[4];
          BIT6=buf[5];
          BIT7=buf[6];
          BIT8=buf[7];
      
      //Check Diplicates and print when necesarry
      if(HexMode=="off")
      {
        BuildCANMSG=String(BIT1) + " " +  String(BIT2) + " " +  String(BIT3) + " " +  String(BIT4)  + " " +  String(BIT5)  + " " +  String(BIT6)  + " " +  String(BIT7)  + " " +  String(BIT8);
      }else
      {
        BuildCANMSG=String(BIT1,HEX) + " " +  String(BIT2,HEX) + " " +  String(BIT3,HEX) + " " +  String(BIT4,HEX)  + " " +  String(BIT5,HEX)  + " " +  String(BIT6,HEX)  + " " +  String(BIT7,HEX)  + " " +  String(BIT8,HEX);
      }

    //BOF Check if Received ID has already been received
    ArryIDIsOld=0;
    ArryMessageIsOld=0;
    for (int arryCount=1; arryCount <= 99; arryCount++)
    {
      if(arryCANID[arryCount] == CANID)
        {         
          ArryIDIsOld=1;          
          //Check If Message Changed
          if(arryCANMSG[arryCount]==BuildCANMSG){MsgColor=0;}else{ MsgColor=41;}  
          
            //Clear line to prevent text from flowing over old test
            Serial.print("\033[" + String(arryCount) + ";1H");
            Serial.print("\033[0m;                                     ");
            //Print fresh line
            Serial.print("\033[" + String(arryCount) + ";1H");
            if(HexMode=="off"){Serial.print("\033[1;" + String(MsgColor)  + "m" + String(CANID) + " " +  BuildCANMSG);}else{Serial.print("\033[1;" + String(MsgColor)  + "m" + String(CANID,HEX) + " " +  BuildCANMSG);}
            Serial.print(" ");
            Serial.print(char(BIT1));
            Serial.print(char(BIT2));
            Serial.print(char(BIT3));
            Serial.print(char(BIT4));
            Serial.print(char(BIT5));
            Serial.print(char(BIT6));
            Serial.print(char(BIT7));
            Serial.print(char(BIT8));           
          break;
        }
    }
    
    if(ArryIDIsOld==0)
    {
      ArryLastListPos++;
      //Print Message because it has not yet been received
     Serial.print("\033[" + String(ArryLastListPos) + ";1H");
      //Serial.print(CANID);
     if(HexMode=="off"){Serial.print("\033[1;0m" + String(CANID) + " " +  BuildCANMSG);}else{Serial.print("\033[1;0m" + String(CANID,HEX) + " " +  BuildCANMSG);}
            Serial.print(" ");
            Serial.print(char(BIT1));
            Serial.print(char(BIT2));
            Serial.print(char(BIT3));
            Serial.print(char(BIT4));
            Serial.print(char(BIT5));
            Serial.print(char(BIT6));
            Serial.print(char(BIT7));
            Serial.print(char(BIT8));      

      //Update Array with CAN ID so that it will not repeat a new line      
      arryCANID[ArryLastListPos] = CANID;
      arryCANMSG[ArryLastListPos]=BuildCANMSG;

    }
   //EOF Check Diplicates and print when necesarry 

      }
      else
      {
        intNoDataCount++;
        if(intNoDataCount>=1000)
        {
        intNoDataCount=0;
        Serial.println("No data found. Check physical connection and connection speed.");
        break;
        }
      }
  
    
   }
  
}

void rawdump()
{
  int intNoDataCount=0;
  String BuildCANMSG;
  unsigned char len = 0;
  unsigned char buf[8];
  int CANID,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8;
  
  if(IsInitiated==0){Serial.println("CAN-BUS shield not initiated. Type \033[1;31mhelp\033[0m to get a list of initiation commands.");return;}
  while(IsDumping==1)
  {
    readincommingserial();
    delay(SlowDown);
      if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
      {
          intNoDataCount=0;
          CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
          unsigned long canId = CAN.getCanId();       
          CANID=canId;
          BIT1=buf[0];
          BIT2=buf[1];
          BIT3=buf[2];
          BIT4=buf[3];
          BIT5=buf[4];
          BIT6=buf[5];
          BIT7=buf[6];
          BIT8=buf[7];
          
          if(HexMode=="off")
          {
            BuildCANMSG=String(BIT1) + " " +  String(BIT2) + " " +  String(BIT3) + " " +  String(BIT4)  + " " +  String(BIT5)  + " " +  String(BIT6)  + " " +  String(BIT7)  + " " +  String(BIT8);
            Serial.print(String(CANID) + " " + BuildCANMSG);
          }else
          {
            BuildCANMSG=String(BIT1,HEX) + " " +  String(BIT2,HEX) + " " +  String(BIT3,HEX) + " " +  String(BIT4,HEX)  + " " +  String(BIT5,HEX)  + " " +  String(BIT6,HEX)  + " " +  String(BIT7,HEX)  + " " +  String(BIT8,HEX);
            Serial.print(String(CANID,HEX) + " " + BuildCANMSG);
          }  
            Serial.print(" ");
            Serial.print(char(BIT1));
            Serial.print(char(BIT2));
            Serial.print(char(BIT3));
            Serial.print(char(BIT4));
            Serial.print(char(BIT5));
            Serial.print(char(BIT6));
            Serial.print(char(BIT7));
            Serial.print(char(BIT8));  
            Serial.println("");        
           
                
          
      }
      else
      {
        intNoDataCount++;
        if(intNoDataCount>=1000)
        {
        intNoDataCount=0;
        Serial.println("No data found. Check physical connection and connection speed.");
        break;
        }
      }    
    
  }
}





void BruteForce()
{
  if(IsInitiated==0){Serial.println("CAN-BUS shield not initiated. Type \033[1;31mhelp\033[0m to get a list of initiation commands.");return;}
  IsDumping=1;
  int FromID=0;
  StringValue = cmdBuild.substring(11,1000);
  StringValue.trim();  
  FromID=atoi(StringValue.c_str());
  clearAndHome();
   int bruteCount=0;
   String BuildCANMSG="";
   
   int bBIT1=0;
   int bBIT2=0;
   int bBIT3=0;
   int bBIT4=0;
   int bBIT5=0;
   int bBIT6=0;
   int bBIT7=0;
   int bBIT8=0;
   
   int BruteCANID=FromID;
   
   while(BruteCANID<=1000000 )
   {
    readincommingserial();
    if(IsDumping==0){BruteCANID=1000000;break;return;}
    
    delay(SlowDown);

    bBIT1++;
    if(bBIT1>=255){bBIT2++;bBIT1=0;}
    if(bBIT2>=255){bBIT3++;bBIT2=0;}
    if(bBIT3>=255){bBIT4++;bBIT3=0;}
    if(bBIT4>=255){bBIT5++;bBIT4=0;}
    if(bBIT5>=255){bBIT6++;bBIT5=0;}
    if(bBIT6>=255){bBIT7++;bBIT6=0;}
    if(bBIT7>=255){bBIT8++;bBIT7=0;}
    if(bBIT8>=255){BruteCANID++;bBIT8=0;clearAndHome();}
    
    if(HexMode=="off")
    {
      BuildCANMSG=String(BruteCANID) + " " + String(bBIT1) + " " +  String(bBIT2) + " " +  String(bBIT3) + " " +  String(bBIT4)  + " " +  String(bBIT5)  + " " +  String(bBIT6)  + " " +  String(bBIT7)  + " " +  String(bBIT8);
    }else
    {
      BuildCANMSG=String(BruteCANID,HEX) + " " + String(bBIT1,HEX) + " " +  String(bBIT2,HEX) + " " +  String(bBIT3,HEX) + " " +  String(bBIT4,HEX)  + " " +  String(bBIT5,HEX)  + " " +  String(bBIT6,HEX)  + " " +  String(bBIT7,HEX)  + " " +  String(bBIT8,HEX);
    }


   Serial.print("\033[1;1H");
   if(HexMode=="off")
   {Serial.print("Performing infinite brute force attack from CAN-ID: \033[1;32m" + String(BruteCANID) + "\033[0m");}
   else
   {Serial.print("Performing infinite brute force attack from CAN-ID: \033[1;32m" + String(BruteCANID,HEX) + "\033[0m");}
   Serial.print("\033[2;1H");
   Serial.print("--------------------------------------------------------------------------");   
    
   Serial.print("\033[3;1H");
   Serial.print("\033[0m                                     ");
   Serial.print("\033[3;1H");
   Serial.println("Sending Message: " +  BuildCANMSG);
   }


  
}







void dosworking()
{
  int MyDelay=20;
  int progress=0;
  for (int i=0; i <= 10; i++)
  {
 
  progress = i * 10;
   
  Serial.println("|");
  Serial.print(progress);Serial.print("% complete.");  
  delay(MyDelay);
  clearAndHome();
   
  Serial.println("/");
  Serial.print(progress);Serial.print("% complete."); 
  delay(MyDelay);
  clearAndHome();
   
  Serial.println("-");
  Serial.print(progress);Serial.print("% complete."); 
  delay(MyDelay);
  clearAndHome();
   
  Serial.println("\\");
  Serial.print(progress);Serial.print("% complete."); 
  delay(MyDelay);
  clearAndHome();
  
  }
  Serial.println("100% Done!");
   
   
 
}
