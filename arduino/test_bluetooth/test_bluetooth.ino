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
  // Leer un dato (esto seria vref). No podemos printear pedir dato porque enconces se guarda en el archivo y no es lo que queremos
  char c = BT.read();
  if(c >= '0' && c <= '9') data += c;

  if(c == '\r'){
    // Printear en el documento. Habria que poner todos los datos que queremos guardar separados por comas (pwm,v,vref)
    last_data = "coche:" + data + "\n\r";
    data = "";
  }
 }
 else{
  BT.write(last_data.c_str());
 }
 delay(100);
}
