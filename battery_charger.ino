
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 20, 4);


float chargeLimit[2][2]={{4.00,7.00},{9,12.5}};
int tempLimit=33;


float voltage = 0.0;
double currentValue = 0;
float R1 = 29000.0;
float R2 = 7120.0;
int value = 0;




float temperature;
int polerityLimit = 200;

void setup() {
  Serial.begin(9600);

  sensors.begin();
  lcd.begin();
  lcd.backlight();

  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3,1);

}

void loop() {


  
  getTemperature();
  getVoltCorrent();
  int a = analogRead(A1);
  int b = analogRead(A2);
  int c = a - b;

  String secondLine="V:"+ String(voltage)+" I:"+String(abs(currentValue));
  String thirdLine="T:"+ String(temperature<0?0:temperature);

  int charg=getPercent();
  String charge=charg<0?"0":String(charg);

  if (c < polerityLimit * -1)
    dis("Wrong polarity!", secondLine,thirdLine,"Charge: "+charge+"%");
  else if (c > polerityLimit){
     dis("Battery detection!", secondLine,thirdLine,"Charge: "+charge+"%");
     //digitalWrite(3, 1);
  }
   
  else{
     dis("Not connected!", secondLine,thirdLine,"Charge: "+charge+"%");
     //digitalWrite(3, 0);
  }
   

  // Serial.println("A1: "+String(a)+" | A2: "+String(voltage)+"   |   Sub: "+(a-b>0?"Right polerity":"Wrong polerity!"));

//&& (voltage>=chargeLimit[0][0] && voltage<=chargeLimit[0][1]) || (voltage>=chargeLimit[1][0] && voltage<=chargeLimit[1][1])
 if ((temperature < tempLimit)  && (temperature>0) && (c > polerityLimit) )
    digitalWrite(3, 0);
  else
    digitalWrite(3, 1);

  delay(300);

}

void getTemperature() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
}

void dis(String a, String b, String c, String d) {
  Serial.println(a + "\t" + temperature + "\t" + c + "\t" + d + "\tCurrent: " + String(currentValue));
  lcd.clear();

  lcd.setCursor(0, 0); // set cursor to 1 symbol of 1 line
  lcd.print(a);

  lcd.setCursor(0, 1); // set cursor to 1 symbol of 2 line
  lcd.print(b);

  lcd.setCursor(0, 2); // set cursor to 1 symbol of 2 line
  lcd.print(c);

  lcd.setCursor(0, 3); // set cursor to 1 symbol of 2 line
  lcd.print(d);



}



void getVoltCorrent() {
  voltage = ((analogRead(A0) * 5.0) / 1024.0) / (R2 / (R1 + R2));
  currentValue = ((((analogRead(A3) / 1024.0) * 5000) - 2500) / 96);
}


int getPercent(){
  int type=0;
  if(voltage>7)
    type=1;
  int y = map(voltage,chargeLimit[type][0] , chargeLimit[type][1], 1, 100);
  return y;
}
