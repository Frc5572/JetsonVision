
String incomingByte;   // for incoming serial data
int led = 11;
int bright;
int i2cInternalAddress = 0;
int fromRoboRio = 0;
String dat="0:0:0:0:0";

int distance=0;
int y=0;
int x=0;
int yaw=0;
int pitch=0;



int toInt(String recv){
  int tnum=0;
  for(int i=0;i<recv.length();i++){
    char rec=recv.charAt(i);
    int num=-1;
    switch(rec){
      case('1'):
      num=1;
      break;
      case('2'):
      num=2;
      break;
      case('3'):
      num=3;
      break;
      case('4'):
      num=4;
      break;
      case('5'):
      num=5;
      break;
      case('6'):
      num=6;
      break;
      case('7'):
      num=7;
      break;
      case('8'):
      num=8;
      break;
      case('9'):
      num=9;
      break;

    }
    //Serial.print("num=");
    //Serial.println(num*pow(float(10),float(recv.length()-(i+1))));
    if(tnum==-1)
      break;
    tnum+=num*pow(float(10),float(recv.length()-(i+1)));
  }
  if(tnum>=99)
    tnum++;
  return tnum;
}
String readData(){
  String str; 
  if(Serial.available() > 0){
    str=Serial.readStringUntil('\n');
  }
  return str;
}
void readAndTrans(){
  //distance:x
  //yaw:pitch:x:y:distance
  if(Serial.available()>0){
    dat=readData();
    if(dat.indexOf(':')<0){
      distance=toInt(dat);
    }else{
      //distance=toInt(dat.substring(dat.indexOf(':')));
      //y=toInt(dat.substring(0,dat.indexOf(':')));
      String var="";
      int varn=0;
      for(int i=0;i<dat.length();i++){
        char at=dat.charAt(i);
        if(at==':'){
          
           switch(varn){
             case(0):
               yaw=toInt(var);
               break;
             case(1):
               pitch=toInt(var);
               break;
             case(2):
               x=toInt(var);
               break;
             case(3):
               y=toInt(var);
               break;
             case(4):
               distance=toInt(var);
               break;
           }
           varn++;
           var="";
        }else{
          var+=at;
        }
      }
      distance=toInt(var);
    }
  }
}
void setup()
{
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
}
void loop(){

  // read the incoming byte:
  readAndTrans();
  analogWrite(A0,yaw);
  analogWrite(A1,pitch);
  analogWrite(A2,x);
  analogWrite(A3,y);
  analogWrite(A4,distance);
  Serial.print("distance:\t");
  Serial.println(distance);
  Serial.print("y:\t");
  Serial.println(y);
  Serial.print("x:\t");
  Serial.println(x);
  Serial.print("yaw:\t");
  Serial.println(yaw);
  Serial.print("pitch:\t");
  Serial.println(pitch);
  //incomingByte.trim();
  
}


