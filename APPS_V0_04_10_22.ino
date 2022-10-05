
//TecH2Racing Eletronics Team
//APPS MODULE Rev.4/10/22 
//calculo diferencial precisa ser revisto
 
#include <SPI.h>          
#include <mcp2515.h>

#define potAccelA A2 // potenciometro A do acelerador
#define potAccelB A3 // potenciometro B do acelerador
#define CAN0_INT 3 // Set INT to pin 3
int brakeSwitch = 6; // snesor de freio pino 6
/*valores de falha
1 = falha bppd
2 = apps implausability */
unsigned int contador;

struct can_frame APPS_BSE;

MCP2515 mcp2515(10);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();               
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); 
  mcp2515.setNormalMode();
  pinMode(brakeSwitch, INPUT_PULLUP);
  int brakeState = digitalRead(brakeSwitch);
  
  
  delay(10);
}


void Send_APPS_BSE(int a, int b, int c, int d, int e, int f, int g, int h){
  APPS_BSE.can_id  = 0x3;         
  APPS_BSE.can_dlc = 8;               
  APPS_BSE.data[0] = a;      
  APPS_BSE.data[1] = b;
  APPS_BSE.data[2] = c;
  APPS_BSE.data[3] = d;
  APPS_BSE.data[4] = e;
  APPS_BSE.data[5] = f;
  APPS_BSE.data[6] = g;
  APPS_BSE.data[7] = h;
  mcp2515.sendMessage(&APPS_BSE); 
  Serial.println("Message <APPS_BSE> Successfully sent!");
  Serial.println(a);
  Serial.println(b);
  Serial.println(c);
  Serial.println(d);
  Serial.println(e);
  Serial.println(f);
  Serial.println(g);
  Serial.println(h);    
}


void APPS_BSEloop(int accel) //função para do pedal plausibility check e controle geral do RPM
{
  
  //CODIGO ACELERADOR E FREIO
  int rpm = 0;
  int Pt_nDesEdrv = 0;
  int byte6 = 0;
  int byte5 = 0;
  int byte4 = digitalRead(brakeSwitch);
  //variavel do valor cru e apos offset 
  rpm = map(accel, 0, 100, 0, 1500); //mapeamento posição do pedal em velocidade
  Pt_nDesEdrv = ((rpm + 24000)/6); //mapeamento velocidade em niveis lidos pelo BRM
  byte6 = Pt_nDesEdrv >> 8; //shift para direita com intuito de obter os 5 primeiros dos 13 bits que devem ser alocadosno byte1 segundo a planilha 
  byte5 = Pt_nDesEdrv & 255; //logica and com o intuito de se obter apenas os ultimos 8 digitos que devem ser alocados para o byte2 segundo a planilha
    Serial.println("rpm:");
    Serial.println(rpm);
    Serial.println("pos offset:"); 
    Serial.println(Pt_nDesEdrv);
    Serial.println("byte 5");
    Serial.println(byte5);
    Serial.println("byte 6");
    Serial.println(byte6);
    
    //not braking
      if (byte4 == HIGH)//acelerador pressionado freio em repouso (valores iniciais para teste)
      {
      //MENSAGEM CAN

      Send_APPS_BSE(0, 0, 0, 0, 0, byte5, byte6, 0);
      Serial.println("acelerando");
      }
      // braking
      else if (byte4 == LOW)
      {
       //MENSAGEM CAN PARA MOTOR PARADO
       
       Send_APPS_BSE(0, 0, 0, 0, 1, byte5, byte6, 0);
       Serial.println("freiando");
      }

      // APPS_BSE and throttle at the same time
      else if (byte4 == LOW && accel > 25)
      {
       Serial.println("bppd = falha");
       while (accel > 5)
        {
          byte4 = digitalRead(brakeSwitch);
          Send_APPS_BSE(0, 0, 0, 1, byte4, 160, 15, 0); //falha bppd com valor 1
          //delay(10);
          int tensaoPotA = analogRead(potAccelA);
          int posA = map(tensaoPotA, 102, 900, 0, 100);
          accel = posA;
        }
       }
       }
 

void loop() 
{
  int brakeState = digitalRead(brakeSwitch);
  Serial.println("estadodo freio:");
  Serial.println(brakeState);
  
  int tensaoPotA = analogRead(potAccelA);
  Serial.println("read:");
  Serial.println(tensaoPotA);
  
  if (tensaoPotA <= 102)
  {
    tensaoPotA = 102;
  }
   else if (tensaoPotA >= 900)
  {
    tensaoPotA = 900;
  }
  
  int tensaoPotB = analogRead(potAccelB);
  Serial.println("read:");
  Serial.println(tensaoPotB);
  
  if (tensaoPotB <= 102)
  {
    tensaoPotB = 102;
  }
   else if (tensaoPotB >= 450) //valor definido arbritariamente, é necessario fazer estudo da curva de tensão do sensor
  {
    tensaoPotB = 450;
  }
  
  //estabelecendo offset de 0,5V

  int posA, posB, quo1, quo2; // DECLARANDO AS VARAVEIS DAS TENSÕES, DAS POSIÇÕES DOS POTÊNCIOMETROS  E AS VARAVEIS COMPARATIVAS

  posA = map(tensaoPotA, 102, 900, 0, 100);
  posB = map(tensaoPotB, 102, 450, 0, 100); // FUNÇÃO PARA CHEGAR NA POSIÇÃO DO PEDAL EM % A PARTIR DA VOLTAGEM
  // FUNÇÃO A SER UTILIZADA TALVEZ NÃO SEJA A MAP  
   Serial.println("position:");
  Serial.println(posA);

  quo2 = (posA*100)/posB;
  quo1 = (posB*100)/posA;
  

 if ( (quo1 >= 90) && (quo2 <= 110) && (quo2 >= 90) && (quo1 <= 110) && (tensaoPotA >= 102) && (tensaoPotA <= 900) && (tensaoPotB >= 102) && (tensaoPotB <= 450)) //Checagem de falha do PEDAL
  {
   contador = millis(); //reseta contador de falha
    
   APPS_BSEloop(posA); //RPM request para ECU
  }
  
  else //TIMER DE 100MS PARA DEFINIR COMO FALHA
  {
    if ((millis() - contador) <= 100)
    {
      //falha = 2; 
      // stop the motor "(EV) Motor(s) must be immediately stopped completely."
      Send_APPS_BSE(0, 0, 0, 2, digitalRead(brakeSwitch), 160, 15, 0);
        
    }
    //codigo para falha não significar perda do controle do carro
    else if ((tensaoPotA >= 102) && (tensaoPotA <= 900)) // APENAS potAaccel quebrou
    {
      APPS_BSEloop(posA);
    }  

    else if ((tensaoPotB >= 90) && (tensaoPotB <= 450)) // APENAS potBaccel quebrou
    {
      APPS_BSEloop(posB);
    }
  }
}


