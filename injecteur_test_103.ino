#include <Wire.h>
#include <U8x8lib.h>  

// on prends le bon drivers de la lib U8Glib

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 


/*define inputs outputs*/
#define injecteur_1 13
#define injecteur_2 4
#define injecteur_3 5
#define injecteur_4 6
#define button_select 2
#define button_marche_arret 7
#define button_haut 8
#define button_bas 9

int frequence = 0;

int menu = 0;

bool etat_injec_1 = true;
bool etat_injec_2 = true;
bool etat_injec_3 = true;
bool etat_injec_4 = true;

bool sortie_injecteur_1 = false;
bool sortie_injecteur_2 = false;
bool sortie_injecteur_3 = false;
bool sortie_injecteur_4 = false;
bool marche = false;

void setup(){
    Serial.begin(9600);  // Ouverture port Serie
    
    pinMode(injecteur_1, OUTPUT);
    pinMode(injecteur_2, OUTPUT);
    pinMode(injecteur_3, OUTPUT);
    pinMode(injecteur_4, OUTPUT); // sortie injecteurs

    pinMode(button_haut, INPUT_PULLUP);
    pinMode(button_bas, INPUT_PULLUP);
    pinMode(button_select, INPUT_PULLUP);
    pinMode(button_marche_arret, INPUT_PULLUP); // button de commande   
    frequence = 20;


       
 
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
   u8x8.drawString(2,1,"****START****");  // write  something to the internal memory
      //and transfer to the display

  delay(1000);
     /********set timer******/
   cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;

  OCR1A = 100;  // compare match register (not very important, sets the timeout for the first interrupt)
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
 
  
}

void loop(){
    affi();
    
    if(!digitalRead(button_haut)){
        menu ++;       
        delay (200);
         u8x8.clearDisplay();  
    }
    if(!digitalRead(button_bas)){
        menu --;
        delay (200);
         u8x8.clearDisplay();  
    }
    
    switch(menu){

       case 0: // marche - arrêt 
        affi();
        u8x8.drawString(1,3,">");
        if(!digitalRead(button_marche_arret)){
          marche = true;
           u8x8.clearDisplay();  
           affi();
             sortie_injecteur_1 = false;
             sortie_injecteur_2 = false;
             sortie_injecteur_3 = false;
             sortie_injecteur_4 = false;
              
            do{
                sortie_injecteur_1 = etat_injec_1 *  !sortie_injecteur_1;
                sortie_injecteur_2 = etat_injec_2 *  !sortie_injecteur_2;
                sortie_injecteur_3 = etat_injec_3 *  !sortie_injecteur_3;
                sortie_injecteur_4 = etat_injec_4 *  !sortie_injecteur_4;
                delay(frequence);
                
            }while(digitalRead(button_marche_arret));
             u8x8.clearDisplay();  
            marche = false;
             sortie_injecteur_1 = false;
             sortie_injecteur_2 = false;
             sortie_injecteur_3 = false;
             sortie_injecteur_4 = false;

            delay(200);
        }
        break;
        case 1: // fréquence 
        affi();
        u8x8.drawString(1,4,"-");
        if(!digitalRead(button_select)){
           delay (200);
            do{
                if(!digitalRead(button_bas)){
                frequence ++;       
                delay (200);
                 u8x8.clearDisplay();  
                }
                if(!digitalRead(button_haut)){
                frequence --;
                delay (200);
                 u8x8.clearDisplay();  
                }
                affi();
                u8x8.drawString(1,4,">");
            }while(digitalRead(button_select));
        }
             if(!digitalRead(button_marche_arret)){
           delay (200);
            do{
                if(!digitalRead(button_bas)){
                frequence += 10;       
                delay (200);
                 u8x8.clearDisplay();  
                }
                if(!digitalRead(button_haut)){
                frequence -= 10;
                delay (200);
                 u8x8.clearDisplay();  
                }
                affi();
                u8x8.drawString(1,4,"*");
            }while(digitalRead(button_marche_arret));          
        }
         delay (200);
        break;
        case 2: // injecteur_1
        affi();
        u8x8.drawString(1,6,">");
        if(!digitalRead(button_select)){
                  etat_injec_1 = !etat_injec_1;
                delay (200);
                 u8x8.clearDisplay();  
                }
        break;
        case 3: // injecteur_2
        affi();
        u8x8.drawString(1,7,">");
        if(!digitalRead(button_select)){
                  etat_injec_2 = !etat_injec_2;
                delay (200);
                 u8x8.clearDisplay();  
                }
        break;
        case 4: // injecteur_3
        affi();
        u8x8.drawString(8,6,">");
        if(!digitalRead(button_select)){
                  etat_injec_3 = !etat_injec_3;
                delay (200);
                 u8x8.clearDisplay();  
                }
        break;
        case 5: // injecteur_4
        affi();
        u8x8.drawString(8,7,">");
        if(!digitalRead(button_select)){
                  etat_injec_4 = !etat_injec_4;
                delay (200);
                 u8x8.clearDisplay();  
                }
        break;
       
        case 6:
        menu = 5;
        break;
        default :
        menu = 0;
        break;
    }      
}

void affi(){
     
    u8x8.drawString(2,1,"TEST INJECTEUR");
     
    u8x8.drawString(2,3,"MARCHE: ");
    if(marche){
        u8x8.setCursor(10,3);
        u8x8.print("ON");
    }
    if(!marche){
        u8x8.setCursor(10,3);
        u8x8.print("OFF");
    }    
    
    u8x8.drawString(2,4,"freq: ");
    u8x8.setCursor(8,4);
    u8x8.print(frequence);
    u8x8.drawString(12,4,"ms");
    
    u8x8.drawString(2,5,"+ INJECT N: +");
    u8x8.drawString(2,6,"1:");
    if(etat_injec_1){
        u8x8.setCursor(5,6);
        u8x8.print("ON");
    }
    if(!etat_injec_1){
        u8x8.setCursor(5,6);
        u8x8.print("OFF");
    }
    
    u8x8.drawString(9,6,"3:");
    if(etat_injec_3){
        u8x8.setCursor(12,6);
        u8x8.print("ON");
    }
    if(!etat_injec_3){
        u8x8.setCursor(12,6);
        u8x8.print("OFF");
    }
    
    u8x8.drawString(2,7,"2:");
    if(etat_injec_2){
        u8x8.setCursor(5,7);
        u8x8.print("ON");
    }
    if(!etat_injec_2){
        u8x8.setCursor(5,7);
        u8x8.print("OFF");
    }
    
    u8x8.drawString(9,7,"4:");
    if(etat_injec_4){
        u8x8.setCursor(12,7);
        u8x8.print("ON");
    }
    if(!etat_injec_4){
        u8x8.setCursor(12,7);
        u8x8.print("OFF");
    }
}

 ISR(TIMER1_COMPA_vect){
  if(marche){
      if(sortie_injecteur_1){
      digitalWrite(injecteur_1, HIGH);
      }
      if(!sortie_injecteur_1){
      digitalWrite(injecteur_1, LOW);
      }
  
      if(sortie_injecteur_2){
      digitalWrite(injecteur_2, HIGH);
      }
      if(!sortie_injecteur_2){
      digitalWrite(injecteur_2, LOW);
      }
  
      if(sortie_injecteur_3){
      digitalWrite(injecteur_3, HIGH);
      }
      if(!sortie_injecteur_3){
      digitalWrite(injecteur_3, LOW);
      }
  
      if(sortie_injecteur_4){
      digitalWrite(injecteur_4, HIGH);
      }
      if(!sortie_injecteur_4){
      digitalWrite(injecteur_4, LOW);
      }
  }
  
  if(!marche){
      digitalWrite(injecteur_1, LOW);
      digitalWrite(injecteur_2, LOW);
      digitalWrite(injecteur_3, LOW);
      digitalWrite(injecteur_4, LOW);
  }  
}
