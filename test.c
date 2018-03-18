#include "test.h"
int8 controle =99; //vari�vel para controle do modo de teste  
void Testa_Motor()
{
   output_low(LED2);
   output_low(LED3);
   if(!input_state(LED1))
   {
      output_high(LED1);
      //printf("\rModo teste: Motores\r");
   }
   switch(controle)
   {
      case 0: MoverMotores(duty_teste, 0,'f'); break;
      case 1: MoverMotores(duty_teste, 0,'b'); break;
      case 2: MoverMotores(0,duty_teste,'f'); break;
      case 3: MoverMotores(0,duty_teste,'b'); break;
      case 4: MoverMotores(duty_teste, duty_teste,'f'); break;
      case 5: MoverMotores(duty_teste, duty_teste,'b'); break;
      case 6: MoverMotores(duty_teste, duty_teste,'h'); break;
      case 7: MoverMotores(duty_teste, duty_teste,'a'); break;
      case 99: MoverMotores(0,0,'p');
   }     
}

void Testa_Sensor()
{
   output_low(LED1);
   if(controle == 8) // controle = 8 -> teste sensores de dist�ncia 
   {
      if(!input_state(LED2))
      {
         output_high(LED2); //led indica que o rob� est� no modo teste de sensor de dist�ncia
         //printf("\rModo teste: Sensores de Distancia\r");
      }
      output_low(LED1);// apaga o led1 para mostrar que n�o est� mais no teste de motores 
      output_low(LED3);
 
      switch(Seta_Sensores('d')) //somente sensores de dist�ncias ligados
      {
         case 0b00000000: MoverMotores(0,0,'p'); break;
         case 0b00000001: MoverMotores(0,duty_teste,'f'); break;
         case 0b00000010: MoverMotores(duty_teste,duty_teste,'a'); break; //pouco prov�vel
         case 0b00000011: MoverMotores(0,duty_teste,'b'); break;
         case 0b00000100: MoverMotores(duty_teste,0,'f'); break;
         case 0b00000101: MoverMotores(duty_teste, duty_teste, 'a'); break; //improv�vel
         case 0b00000110: MoverMotores(duty_teste, 0, 'b'); break;
         case 0b00000111: MoverMotores(duty_teste, duty_teste,'f'); break; 
      }  
    }
      
      else if(controle == 9)// controle = 9 -> teste sensores de linha
      {
         if(!input_state(LED3))
         {
            output_high(LED3);
            //printf("\rModo teste: Sensores de Linha\r");
         }
         output_low(LED2);
         switch(Seta_Sensores('l')) //somente sensores de linha ligados
         {
            case 0b00000000: MoverMotores(0,0,'p'); break; // dupla chamada para garantir que o robo ficar� parado se estiver cego
            case 0b00001000: MoverMotores(0,duty_teste,'b'); break;   //rodas giram no sentido oposto como mais um indicativo de que o modo � teste do sensor de linha
            case 0b00010000:output_high(LED1); MoverMotores(duty_teste,0,'b'); break;
            case 0b00011000: output_high(LED2); MoverMotores(duty_teste,duty_teste,'b'); break;
            //default: output_high(LED4); MoverMotores(duty_teste,duty_teste,'f');
         }
      }
}

void Teste()
{
   if(controle < 8)
   {
      Testa_Motor();
   }
   else if(controle == 8 || controle ==9)
   {
      Testa_Sensor();
   }
}

/*------------------TODO: testar fun��o do sensor de corrente-----------------*/
void Sensor_de_Corrente(int8 canal) //fun��o para resgatar o valor da corrente nos motores
{
   int16 vs_digital; //valor de 0 a 1023 referente � tens�o vs na sa�da do BTN
   int16 vs; //valor de tens�o anal�gico na sa�da do BTN
   int16 aux =0; //variv�vel auxiliar para armazenar o valor instant�neo de vs
   int16 corrente =0;
   
   switch(canal)
   {
      case 7: set_adc_channel(sensor1);
      case 8: set_adc_channel(sensor2);
   }
   
   delay_us(10);
   aux = read_adc();
   
   if(aux > vs_digital && canal == 7)
   {
      vs_digital = aux;
      vs = (vs_digital*5)/1023;   
      corrente = ((int16) vs*19.5);
      printf("Corrente Motor 1: /n/r"); 
      printf("%lu A",corrente);
      printf("\r");
   }
   
   else if(aux > vs_digital && canal == 8)
   {
       vs = (vs_digital*5)/1023;
      
      corrente = ((int16) vs*19.5);
      printf("Corrente Motor 2: \n\r"); 
      printf("%lu A", corrente);
      printf("\r");
   }  
}
