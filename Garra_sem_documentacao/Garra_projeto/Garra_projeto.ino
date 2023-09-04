#include <Servo.h>

// Arquivo Feito por Mateus Bassi

#include <LiquidCrystal_I2C.h>
//"LiquidCrystal_I2C.h" é a biblioteca do LCD com o I2C 

#include <Servo.h>
//"Servo.h" é a biblioteca dos Servos que serão usados

Servo servo_1;// Trabalho com 0º - 180º // Base
Servo servo_2;// Trabalho com 55º - 170º //parte 1  
Servo servo_3;// Trabalho com 95º - 170º //parte 2
Servo servo_4;// Trabalho com 105º-170º para mais ... // Garra.
//Inicia os servos e dá nome a eles

// ------ Declara a Base dos Servos (ponto inicial)--------
int base_servo1 = 90;
int base_servo2 = 100;
int base_servo3 = 115;
int base_servo4 = 170;

// ------ Declara o ponto final dos Servos --------
int fim_servo1 = 0;
int fim_servo2 = 0;
int fim_servo3 = 0;
int estado_garra = 0;

//Define as portas que o estão os botões e quais são interrupt 
#define botx 5
#define boty 4
#define btn_braco 3 
#define btn_garra 2 

//Variaves para controle do Visor (LCD e Movimentação do Braço)
int x =0;
int y = 1;

LiquidCrystal_I2C lcd(0x27,16,2); //Inicia o LCD com o I2C

void setup()
{
    //---------Inicia o LCD e seu backLight -------------------
    lcd.init(); //Inicia o LCD  com I2C
    lcd.backlight();//Inicio do "fundo"

    //--------Inicia e já coloca as informações no LCD---------
    lcd.setCursor(0, 0);
  	lcd.print("Ctrl_Garra ");
  	lcd.setCursor(0,1);
	  lcd.print("Val_X:");
  	lcd.setCursor(6,1);
  	lcd.print("A"); // Variavel trocavel
  	lcd.setCursor(8,1);
  	lcd.print("Val_Y:");
  	lcd.setCursor(14,1);
  	lcd.print(y); // Variavel trocavel

    //-----------Declara os botões ----------
  	pinMode(botx,INPUT);
  	pinMode(boty,INPUT);
  	pinMode(btn_braco,INPUT);
    pinMode(btn_garra,INPUT);

    //-----------Declara os Interrupt e qual função deve chamar ------
    attachInterrupt(digitalPinToInterrupt(btn_garra),movi_garra,RISING); //Movimento da pinça

  	//Declara quais os pinos que estão conectados os servos
  	servo_1.attach(8);
  	servo_2.attach(9);
  	servo_3.attach(10);
  	servo_4.attach(11);

  	//Define a posição incial dos servos
    servo_1.write(base_servo1); //base
    delay(500);
    servo_2.write(base_servo2);//parte 1
    delay(500);
    servo_3.write(base_servo3);//parte 2
    delay(500);
    servo_4.write(base_servo4);//garra aberta
    delay(500);
      
}

void loop()
{
  if(digitalRead(botx)==HIGH){//só muda no LCD 
    botx_local();
  }
  if(digitalRead(boty)==HIGH){//só muda no LCD
  boty_local();  
  }
  if(digitalRead(btn_braco)==HIGH){ //movimentação do Braço 
    movi_braco();
    Serial.println("Funcionou"); // usado para ver se está rodando certinho no serial 
  }
 // Serial.println(digitalRead(btn_garra));
}
     
int botx_local(){  //Visual
x ++;
lcd.setCursor(6,1);
if (x % 2 == 0){
    lcd.print("A");
}else {
    lcd.print("B");       
}
 delay(250);
 return x;
}
  	
int boty_local(){ //Visual
  
  if(y<2){
	y++; 
  	lcd.setCursor(14,1);
    lcd.print(y);
    delay(250);
  }else{
  	y=1;
  	lcd.setCursor(14,1);
    lcd.print(y);
    delay(250);
  }
}

void movi_braco(){//movimento do braço 
	
  	if (x % 2 == 0){ // Primeiro IF controla a posição da base
    		//lcd.print("A");
      	  for(int i = base_servo1; i<120;i++){
          servo_1.write(i); //base
          delay(15);
          fim_servo1= i;
          }
    }else {
       		//lcd.print("B"); 
      		for(int i = base_servo1; i<40;i--){
          servo_1.write(i); //base
          delay(15);
          fim_servo1= i;
          }
    }
    if ( y==1){      
        	for(int i = base_servo3; i<130;i++){
          servo_3.write(i); //base
          delay(15);
          fim_servo3= i;
          }
          for(int i = base_servo2; i<160;i++){
          servo_2.write(i); //base
          delay(15);
          fim_servo2= i;
          }
      delay(3000);
      //delay para chegar la e para fechar a garra ou abrir 
      reset_garra();
      
    }else if( y==2){
      for(int i = base_servo3; i<180;i++){
          servo_3.write(i); //base
          delay(15);
          fim_servo3= i;
          }
          for(int i = base_servo2; i<130;i++){
          servo_2.write(i); //base
          delay(15);
          fim_servo2= i;
          }
      delay(3000); 
      reset_garra();
    }
  	
}

void movi_garra(){ // movimento do pinça 

  estado_garra ++;
  delay(5);

  if (estado_garra % 2 == 0){//Quando for par ele abre

    for(int i= 170;i>110;i--){
      servo_4.write(i);
      delay(15);
      Serial.println(estado_garra);
    }
    
  }else {//quando for impar ele fechar

    for(int i= 110;i<170;i++){
      servo_4.write(i);
      delay(15);
      Serial.println(estado_garra);
    }  
         
  }
}

int reset_garra(){ // inicia o movimento de retorno da garra
  // ----------------Movimento reset Parte 1--------------
  if(fim_servo2 == 0){
    //Faz nada
  }else {
    if (fim_servo2>base_servo2){
    for(int i = fim_servo2; i>base_servo2;i--){  
    servo_2.write(i); //base
    delay(15);}
    fim_servo2 = base_servo2;
    }else {
    for(int i = fim_servo2; i<base_servo2;i++){  
    servo_2.write(i); //base
    delay(15);}
    }
  }
  // ----------------Movimento reset Parte 2--------------
  if(fim_servo3 == 0){
    //Faz nada
  }else {
    if (fim_servo3>base_servo3){
    for(int i = fim_servo3; i>base_servo3;i--){  
    servo_3.write(i); //base
    delay(15);}
    fim_servo3 = base_servo3;
    }else {
    for(int i = fim_servo3; i<base_servo3;i++){  
    servo_3.write(i); //base
    delay(15);}
    fim_servo3 = base_servo3;
    }
  }  
  
  // ----------------Movimento reset Base --------------
  if(fim_servo1 == 0){
    //Faz nada
  }else {
    if (fim_servo1>base_servo1){
    for(int i = fim_servo1; i>base_servo1;i--){  
    servo_1.write(i); //base
    delay(15);
    fim_servo1 = base_servo1;
    }
    }else {
    for(int i = fim_servo1; i<base_servo1;i++){  
    servo_1.write(i); //base
    delay(15);
    fim_servo1 = base_servo1;
    }
    }
  }
}