#include  <LiquidCrystal.h>
#include <Servo.h>

//Inicio Pines y variables para sensor RGB
int S0 = 4;
int S1 = 5;
int S2 = 6;
int S3 = 7;
int sensorOut = 8;
int frequency = 0;
int red=0;
int blue=0;
int green=0;
boolean contado = false;
//Fin Pines y variables para sensor RGB

//Inicio Pines y variables para sensor ultrasonico
int triger = 2;
int echo = 3;
//Fin Pines y variables para sensor ultrasonico

//Inicio Variables para guardar el color y tama;o
int colorCaja = 0; //1 si es rojo ,2 si es azul , 3 si es amarillo 
int sizeCaja = 0; // 1 si es pequeña, 2 si es mediana, 3 si es azul
//Fin Variables para guardar el color y tama;o

//Pines display lcd
int RS = 23;
int E = 25;
int D4 = 22;
int D5 = 24;
int D6 = 26;
int D7 = 28;

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

Servo servo;

int pinFacilota = 35;
unsigned long previousTime = 0;
unsigned long minuto = 60000;

void setup() {
  //Servo
  servo.attach(40);
  servo.write(0);
  //Inicio configuraciones sensor RGB
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(pinFacilota,INPUT);
  
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  //Fin configuraciones sensor RGB

  //Inicio configuraciones sensor ultrasoninco
  pinMode(triger, OUTPUT);
  pinMode(echo, INPUT);
  //Fin configuraciones sensor ultrasoninco

  Serial.begin(9600);

  //inicializacion LCD
  lcd.begin(16, 2);
//  lcd.setCursor ( 0, 0);
//  lcd.print("!G11-SECCION-A!");
//  lcd.setCursor ( 0, 1);
//  lcd.print("PRACTICA2");
  //delay(60000);
  //lcd.clear();
}

//VARIABLES MENSAJES
int mensaje  = 0;
int numproductos = 0;
int productos1 = 0;
int productos2 = 0;
int productos3 = 0;
int estadoMensaje = 0;
boolean encendido = false;
int pinS = 0;
unsigned long currentTime = 0;
void loop() {
  //get current time
  currentTime = millis();
  color();  
  distancia();
  moverMotores();
  pinS = digitalRead(pinFacilota);
  delay(1);
  Serial.println("switch");
  Serial.println(pinS);
  actualizarMensaje();
}

void actualizarMensaje(){
  if(estadoMensaje == 0){
    imprimirMensaje0();
    if(pinS == 1){
      //se enciende la banda
      estadoMensaje = 1;
      lcd.clear();
    }
  }
  if(estadoMensaje == 1){
    imprimirMensaje1();
    if(pinS == 0){
      //se apaga la banda
      if(currentTime - previousTime >= minuto){
        estadoMensaje = 2;
        previousTime = currentTime;
        lcd.clear();
      }
    } else {
      previousTime = millis();
    }
  }
  if(estadoMensaje == 2){
    imprimirMensaje2();
    if(currentTime - previousTime >= minuto){
      previousTime = currentTime;
      estadoMensaje = 3;
      lcd.clear();
    }
  }
  if(estadoMensaje == 3){
    imprimirMensaje3();
    if(currentTime - previousTime >= minuto){
      previousTime = currentTime;
      estadoMensaje = 4;
      lcd.clear();
    }
  }
  if(estadoMensaje == 4){
    imprimirMensaje4();
    if(currentTime - previousTime >= minuto){
      previousTime = currentTime;
      estadoMensaje = 0;
      lcd.clear();
    }
  }
}

void imprimirMensaje0(){
  lcd.setCursor ( 0, 0);
  lcd.print("!G11-SECCION-A!-PRACTICA2");
  lcd.scrollDisplayLeft();
}

void imprimirMensaje1(){
  lcd.setCursor(0,0);
  lcd.print("G11-TOTAL PRODUCTOS -");
  lcd.setCursor(21,0);
  lcd.print(numproductos);
  lcd.scrollDisplayLeft();
  lcd.setCursor ( 7, 1);
  lcd.print(":)");
}

void imprimirMensaje2(){
  lcd.setCursor(0,0);
  lcd.print("¿:G11-Recipiente 1 -");
  lcd.setCursor(21,0);
  lcd.print(productos1);
  lcd.setCursor(23,0);
  lcd.print("Productos $&");
  lcd.scrollDisplayLeft();
  lcd.setCursor ( 4, 1);
  lcd.print((currentTime - previousTime)/1000);
  lcd.setCursor ( 7, 1);
  lcd.print("seg");
}

void imprimirMensaje3(){
  lcd.setCursor(0,0);
  lcd.print("¿:G11-Recipiente 2 -");
  lcd.setCursor(21,0);
  lcd.print(productos2);
  lcd.setCursor(23,0);
  lcd.print("Productos $&");
  lcd.scrollDisplayLeft();
  lcd.setCursor ( 4, 1);
  lcd.print((currentTime - previousTime)/1000);
  lcd.setCursor ( 7, 1);
  lcd.print("seg");
}

void imprimirMensaje4(){
  lcd.setCursor(0,0);
  lcd.print("¿:G11-Recipiente 3 -");
  lcd.setCursor(21,0);
  lcd.print(productos1);
  lcd.setCursor(23,0);
  lcd.print("Productos $&");
  lcd.scrollDisplayLeft();
  lcd.setCursor ( 4, 1);
  lcd.print((currentTime - previousTime)/1000);
  lcd.setCursor ( 7, 1);
  lcd.print("seg");
}

void color(){
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  red = pulseIn(sensorOut, digitalRead(sensorOut)==HIGH ? LOW : HIGH);

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blue = pulseIn(sensorOut, digitalRead(sensorOut)==HIGH ? LOW : HIGH);

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  green = pulseIn(sensorOut, digitalRead(sensorOut)==HIGH ? LOW : HIGH);

  if (red < blue && red < green && green > blue)
  {  
   //Serial.println("Es color rojo"); 
   colorCaja = 1;
  } else if (blue < red && blue < green)   
  {  
   //Serial.println("Es color azul");
   colorCaja = 2;
  } else if(red< green && red < blue && green < blue)  {
    //Serial.println("Es color amarillo");
    colorCaja = 3;
  }
}

void distancia(){
  digitalWrite(triger, LOW);
  delayMicroseconds(2);

  digitalWrite(triger, HIGH);
  delayMicroseconds(10);

  float tiempo = pulseIn(echo, HIGH);
  float d = (tiempo/2)/29.1;
  //Serial.print(d);
  //Serial.println(" cm");
  
  if(d > 10.5){
    //Serial.println("No hay nada en la banda papi");
    //Serial.println(d); 
    sizeCaja = 0; 
    colorCaja=0;
    contado = false;
  } else if( d< 10.5 && d>7.5 && !contado){
    //Serial.println("Caja pequeña");
    numproductos++;
    sizeCaja = 1;
    contado = true;
  } else if( d< 7.5 && d>4.5 && !contado){
    //Serial.println("Caja mediana");
    numproductos++;
    sizeCaja = 2;
    contado = true;
  }else if( d > 0 && d< 4.5 && !contado){
    //Serial.println("Caja grande");
    sizeCaja = 3;
    numproductos++;
    contado = true;
  }
  delay(500);
}

void moverMotores(){
  switch(sizeCaja){
    case 0://NO HAY NADA EN LA BANDA
      //Serial.println("No mover el motor");
      break;
    case 1:// CAJA PEQUEÑA
      switch(colorCaja){
        case 0:
          //Serial.println("No mover el motor");
          break;
        case 1://pequeño rojo recipiente 2
          productos2++;
          moverMotorCentro();
          break;
        case 2://pequeño azul recipiente 1
          productos1++;
          moverMotorIzquierda();
          break;
        case 3://pequeño amarillo recipiente 3
          productos3++;
          moverMotorDerecha();
          break;  
      }
      break;
    case 2:// CAJA MEDIANA
      switch(colorCaja){
        case 0:
          //Serial.println("No mover el motor");
          break;
        case 1://mediano rojo recipiente 1
          productos1++;
          moverMotorIzquierda();
          break;
        case 2://mediano azul recipiente 3
          productos3++;
          moverMotorDerecha();
          break;
        case 3://mediano amarillo recipiente 2
          productos2++;
          moverMotorCentro();
          break;  
      }
      break;
    case 3: // CAJA GRANDE
      switch(colorCaja){
        case 0:
          //Serial.println("No mover el motor");
          break;
        case 1://grande rojo recipiente 3
          productos3++;
          moverMotorDerecha();
          break;
        case 2://grande azul recipiente 2
          productos2++;
          moverMotorCentro();
          break;
        case 3://grande amarillo recipiente 1;
          productos1++;
          moverMotorIzquierda();
          break;  
      }
      break;
  }  
}

void moverMotorIzquierda(){//Recipiente 1
  //Serial.println("Mover motor al recipiente 1");
  servo.write(60);
  delay(1000);
  servo.write(0);
}

void moverMotorCentro(){//Recipiente 2
  //Serial.println("Mover motor al recipiente 2");
}

void moverMotorDerecha(){//Recipiente 3
  //Serial.println("Mover motor al recipiente 3");
  servo.write(130);
  delay(1000);
  servo.write(0);
}
