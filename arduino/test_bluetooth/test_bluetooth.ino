#include <SoftwareSerial.h>
SoftwareSerial BT(7,4); // RX | TX del arduino

String data = "";
String last_data = "tyfgukyguhñuohh\n\r";

void setup()
   {  
      BT.begin(9600);
   if(BT.available()) BT.write("Waiting for command...");

   BT.setTimeout(10);

   }

void loop()
{
 if(BT.available()){
  char c = BT.read();
  if(c >= '0' && c <= '9') data += c;

  if(c == '\r'){
    //String aux = "coche:" + data + "\n\r";
    last_data = "coche:" + data + "\n\r";
    data = "";
  }
 }
 else{
  BT.write(last_data.c_str());
 }
 delay(100);
}
