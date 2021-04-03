#include <LiquidCrystal.h> // on importe la bibliothèque
LiquidCrystal monEcran(12,8,5,4,3,2); // on crée l'objet écran
#include <Keypad.h>


#include <SoftwareSerial.h>   // librairie pour creer une nouvelle connexion serie max 9600 baud
#define TxD 1    // 1
#define RxD 0   // 0
SoftwareSerial BTSerial(RxD,TxD); // RX | TX  = > BT-TX=6 BT-RX=3


int cnt = 0; // nombre de données découpées
String data[25]; // stockage des données découpées

int i = -1;
String passADMIN = "1997";
String passTEMP = "1234";
int one_temp = 0;

#define pinR 9  // 0
#define pinV 6   // 0
#define pinB 11   // 0

int rouge =0 ;
int vert = 0;
int bleu = 0;
int lock_arcenciel = 1;


int affiche_B = 0;
String saisit = "    ";

int menu_lock = 0;
int pass_lock = 1;
int detection_alarme_lock = 1;
long timetodetection = 0;
long timetosonne = 0;
int bloque_sirene = 0;
int etat_alarme = 0;
long temps_to_actif_alarme = 0;

int tour = 0;
int tourChange = 0;
int uptade = 0;
int affiche_heure = 0;
int demmarage_time = 0;

int year = 0;
int month = 0;
int day =0;
int hour =0;
int minute = 0;
int seconde =0;


int etat_reveil = 0;
int reveil_heure = 0;
int reveil_minute = 0;
int reveilREDt[25];
int reveilGREt[25];
int reveilBLUt[25];
int REViterINC = 0;
int REVEILlongueur_SEQ = 0;
                          //VAR disco
int active_disco = 0;
float BPM_disco = 100; 
int etat_ACT3 = 0;
int REDt[25];
int GREt[25];
int BLUt[25];
int SREDt[25];
int SGREt[25];
int SBLUt[25];
float QUOmult=1;


int longueur_SEQ = 0;
unsigned long tpsCH = 0;
int iter_couleur = 0;
float floa = 0;
int ina = 0;
int decalage_synchro = 0;
int ju;

long temps_millis = 0;
String messageB;
int iterINC = 0;

word TYPE;
word r;
word g;
word b;

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
  };
byte rowPIN[ROWS] = {7,19,18,17};
byte colsPIN[COLS] = {16,15,14};
Keypad keypad = Keypad(makeKeymap(keys), rowPIN, colsPIN, ROWS, COLS );


int PINdetection = 20;      // 20
int PINalarme = 13;         // 13 
int PINled = 10;            //10

void setup() {
  BTSerial.begin(9600);
  
  monEcran.begin(16,2);
  pinMode(PINdetection,INPUT);      // detection porte ouvert !
  pinMode(PINalarme,OUTPUT);     // sorie alarme
  pinMode(PINled,OUTPUT);     // sortie led + blocage on / off
  digitalWrite(PINled,HIGH);    // high = relaie éteint, led éteint
  digitalWrite(PINalarme,0);  
}

void loop() {
    
  
  
   
      if(BTSerial.available() > 3) {
    TYPE=BTSerial.read();
    r=BTSerial.read();
    g=BTSerial.read();
    b=BTSerial.read();
    BTSerial.flush();
    
    if(TYPE ==13){    //reset tempo
    iter_couleur = 0; // =============
    tpsCH = millis();
    }else if(TYPE == 16){   // decalage note lum
      if(r==1){
        if(iter_couleur == 0){
          iter_couleur = longueur_SEQ -1;
        }else{
          iter_couleur = iter_couleur -1;
        }
      }else{
        if(iter_couleur >= longueur_SEQ){
          iter_couleur = 1;
        }else{
          iter_couleur = iter_couleur +1;
        }
      }
    }else if(TYPE == 15){   // variation tempo
      if(r==1){
        QUOmult = 0.00625;
      }else if(r==2){
        QUOmult = 0.125;
      }else if(r==3){
        QUOmult = 0.25;
      }else if(r==4){
        QUOmult = 0.5;
      }else if(r==5){
        QUOmult = 1;
      }else if(r==6){
        QUOmult = 2;
      }else if(r==7){
        QUOmult = 4;
      }else if(r==8){
        QUOmult = 8;
      }else if(r==9){
        QUOmult = 0.00625;
      }
    }
    
    
    else if(TYPE == 14){     //decalage sychro
        if(r==1){
          decalage_synchro = decalage_synchro - round(0.25*ina);
        }
        if(r==2){
          decalage_synchro = decalage_synchro - round(0.125*ina);
        }
        if(r==3){
          decalage_synchro = decalage_synchro + round(0.25*ina);
        }
        if(r==4){
          decalage_synchro = decalage_synchro + round(0.125*ina);
        }                     // ==========
      
    }else if(TYPE == 1) {        //test connection ->1 et 2
        BTSerial.write(15);   // connection fait
        affiche_B = 1;
        delay(1000);
        if(etat_alarme == 1){
          BTSerial.write(11);
        }else{
          BTSerial.write(10);
        }
        year = r+2000;
        month = g;
        day = b;    
    }else if(TYPE==2){
    
        hour = r;
        minute = g;
        seconde = b;
        demmarage_time = 1;   //==================
        
    }else if(TYPE == 10) {          // active alarme
           monEcran.clear();
           monEcran.print("code bon");
           delay(500);  
          if(etat_alarme == 0){                 // passage en actif
              etat_alarme = 1;    
              digitalWrite(PINled,HIGH);  
              saisit = "    ";
              i = -1;
              pass_lock = 1;
              menu_lock = 0;
              affiche_heure = 1;
              detection_alarme_lock = 0;
              temps_to_actif_alarme = 10000;
              timetodetection = millis();
              uptade = 0;
              BTSerial.write(11); }   //=========
              
    }else if(TYPE == 11){    //desactive alarme
          monEcran.clear();
          monEcran.print("code bon");
          delay(500);                                                                                          // passage en desactif
              if(etat_alarme == 1){
              noTone(PINalarme);
              etat_alarme = 0; 
              digitalWrite(PINled,LOW);   
              saisit = "    ";
              i = -1;
              pass_lock = 1;
              menu_lock = 0;
              affiche_heure = 1;
              detection_alarme_lock = 1;
              bloque_sirene = 0;
              uptade = 0;  
              BTSerial.write(10); }   // ==============
              
    }else if(TYPE == 3){            // set light constent
              lock_arcenciel = 1;  
              rouge = r;
              vert = g;
              bleu = b;             // ==============
              
    }else if(TYPE == 5){            // arc en ciel 
      lock_arcenciel = 0;
      rouge = 255;
      vert = 1;
      bleu = 1;
      tour = 0;
      tourChange = r;               //===============
      
    }else if(TYPE == 4){          // turn off light
      rouge =0 ;
      vert = 0;
      bleu = 0; 
      lock_arcenciel = 1;         // ==============
      
    }else if(TYPE ==7){       //desactiver reveil
      etat_reveil = 0;
      BTSerial.write(13);      //======
      
    }else if(TYPE ==8){       //disco on      avec BPM
      if(r == 250){
        BPM_disco = r+g;
      }else{
        BPM_disco = r;
      }
        floa = 60;
        floa= BPM_disco / floa;
        floa = 1/floa;
        floa = floa *1000;
        ina = floa;
        ju = ina;
                          //======================
      
    }else if(150 <=TYPE and TYPE < 200){  // active reveil on 
      if(TYPE == 150){ 
        REViterINC = 0;
        etat_reveil = 1;
        REVEILlongueur_SEQ = r;   
        reveil_heure = g;
        reveil_minute = b;
        BTSerial.write(12);
      }else{
       reveilREDt[iterINC] = r;
       reveilGREt[iterINC] = g;
       reveilBLUt[iterINC] = b;
       REViterINC = REViterINC+1; 
      }                                 //===================
    }else if(100 <=TYPE and TYPE < 150){     //disco seq
      if(TYPE == 100){
        active_disco = 1;  
        longueur_SEQ = r;   
        iterINC = 0;
      }else{
       SREDt[iterINC] = r;
       SGREt[iterINC] = g;
       SBLUt[iterINC] = b;
       iterINC = iterINC+1;
      }                               //============
    }else if(TYPE == 17){       // disco off
     active_disco = 0; 
     rouge = 0;
     vert = 0;
     bleu = 0;
    }
        
 
 
             
  }                                                       // ================================================      
    
    
            
  


if(menu_lock == 0){                           // menu action ==============================================
    if(affiche_B == 1){
        monEcran.setCursor(15,0);
        monEcran.print("B");
       }
       if(etat_alarme == 0){
        monEcran.setCursor(0,1);
        monEcran.print("alarme desactif");
       }else{
        monEcran.setCursor(0,1);
        monEcran.print("alarme actif");
       } 
    char key = keypad.getKey();
    if (key != NO_KEY) {
        if(key == '*'){
          affiche_heure = 0;
          monEcran.clear();
          monEcran.print("entrer pass");
          monEcran.setCursor(0,1);
          pass_lock = 0;
          menu_lock = 1;
        }
        if(key == '5'){
          if(lock_arcenciel == 1){
            lock_arcenciel = 0;
            rouge = 255;
            vert = 1;
            bleu = 1;
            tour = 0;
            tourChange = 50; 
          }else{
            lock_arcenciel = 1;
            rouge = 0;
            vert = 0;
            bleu = 0;
          }          }
          if(key == '1'){
            tone(PINalarme,800);
            delay(5000);
            noTone(PINalarme);
          }
          if(key == '3'){
            if(etat_ACT3 == 0){
              etat_ACT3 = 1;
              digitalWrite(PINled,HIGH);
            }else{
              etat_ACT3 = 0;
              digitalWrite(PINled,LOW);
            }
          }
     




    }


  
}                                             //  ================================


if(pass_lock == 0){                                  //  gestion du mot de passe
char key = keypad.getKey();
    if (key != NO_KEY) {
       if(key != '#'){
          i = i+1;
          saisit[i] = key;  
          monEcran.print(key);      
       }else{
        if(saisit == passADMIN){                // code bon
          monEcran.clear();
          monEcran.print("code bon");
          monEcran.setCursor(0,1);  
          if(etat_alarme == 0){                 // passage en actif
              etat_alarme = 1;
              monEcran.print("alarme actif");        
              delay(1000);
              saisit = "    ";
              i = -1;
              pass_lock = 1;
              menu_lock = 0;
              affiche_heure = 1;
              detection_alarme_lock = 0;
              temps_to_actif_alarme = 10000;
              timetodetection = millis();
              uptade = 0;
              digitalWrite(PINled,HIGH);
          }else{                                // passage en desactif
              noTone(PINalarme);
              etat_alarme = 0;
              monEcran.print("alarme inactif");        
              delay(1000);
              saisit = "    ";
              i = -1;
              pass_lock = 1;
              menu_lock = 0;
              affiche_heure = 1;
              detection_alarme_lock = 1;
              bloque_sirene = 0;
              uptade = 0;
              digitalWrite(PINled,LOW);
       }       
        }else if(saisit == passTEMP and etat_alarme == 1){       // pass temporaire
              monEcran.clear();
              monEcran.print("vous avez 10 min");         
              delay(1000);
              saisit = "    ";
              i = -1;
              pass_lock = 1;
              menu_lock = 1;
              affiche_heure = 0;
              detection_alarme_lock = 0;
              temps_to_actif_alarme = 20000;
              timetodetection = millis();
              bloque_sirene = 0;
              uptade = 0;
           
        }else{  
          monEcran.setCursor(0,1);   
          monEcran.print("          ");
          monEcran.setCursor(0,1);  
          monEcran.print("code faux");
          delay(500);
          monEcran.setCursor(0,1); 
          monEcran.print("          ");
          monEcran.setCursor(0,1); 
          saisit = "    ";
          i = -1;
          pass_lock = 1;
          menu_lock = 0;
          affiche_heure = 1;
        }    } }}                             // ======================================

      



      if(etat_alarme == 1){                           // detection de l'alarme
          if(millis() - timetodetection >= temps_to_actif_alarme){
            if(analogRead(PINdetection) == LOW or bloque_sirene == 1){
              delay(200);                             // test suplémentaire en cas de faux contact
             if(analogRead(PINdetection) == LOW or bloque_sirene == 1){
              if(uptade == 0){      
                uptade = 1;
                monEcran.clear();
                monEcran.print("intrus detecte");
                monEcran.setCursor(0,1);  
                timetosonne = millis();
                
              }
              if(millis() - timetosonne >= 10000){
             tone(PINalarme,800);
              }
              pass_lock = 0;
              affiche_heure = 0;
              menu_lock = 1;
              bloque_sirene = 1;         }  }     }   }            //=========================




if(lock_arcenciel == 0){
  tour++;
  if(tour == tourChange){
    tour = 0;
    
  if(vert < 255 and rouge == 255 and bleu == 1){
    vert = vert +1;
  }

  if(rouge > 1 and vert == 255 and bleu == 1){
    rouge = rouge -1;
  }
  
  if(bleu < 255 and rouge == 1 and vert == 255){
    bleu = bleu +1;
  }
  
  if(vert > 1 and bleu == 255 and rouge == 1){
    vert = vert -1;
  }
  
  if(rouge < 255 and bleu == 255 and vert == 1){
    rouge = rouge +1;
  }
  
  if(bleu > 1 and rouge == 255 and vert == 1){
    bleu = bleu -1;
  }
  }
}
                                                                           //gestion couleur disco
  if(active_disco == 1){
  if(millis() >= (tpsCH + decalage_synchro)){
    if(iter_couleur >= longueur_SEQ){
      for(int i=0;i<longueur_SEQ;i++){
        REDt[i] = SREDt[i];
        GREt[i] = SGREt[i];
        BLUt[i] = SBLUt[i];
      }
      ju =round(ina*QUOmult);
      iter_couleur = 0;
      rouge = REDt[iter_couleur];
      vert = GREt[iter_couleur];
      bleu = BLUt[iter_couleur];
    }else{
      rouge = REDt[iter_couleur];
      vert = GREt[iter_couleur];
      bleu = BLUt[iter_couleur];
    }
    iter_couleur = iter_couleur+1;
    tpsCH = ju + tpsCH;
  }
  }

  if(demmarage_time == 1){
    
  }



  if(etat_reveil == 1){
    
  }



 

               
analogWrite(pinR, rouge);  
analogWrite(pinV, vert);
analogWrite(pinB, bleu);



           
   }    //fin loop
  





 int keyTOint(char key){
  if(key == '1'){
    return 1;
  }
  if(key == '2'){
    return 2;
  }
  if(key == '3'){
    return 3;
  }
  if(key == '4'){
    return 4;
  }
  if(key == '5'){
    return 5;
  }
  if(key == '6'){
    return 6;
  }
  if(key == '7'){
    return 7;
  }
  if(key == '8'){
    return 8;
  }
  if(key == '9'){
    return 9;
  }
  if(key == '0'){
    return 0;
  }

  
 }

  
