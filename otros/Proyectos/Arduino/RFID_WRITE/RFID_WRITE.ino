#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mf(SS_PIN, RST_PIN);        // Create MFRC522 instance.

byte write(MFRC522::MIFARE_Key* key, byte block, String s){
  if(s.length()!=16 || block%4==3) return 0;
  mf.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mf.uid));
  return mf.MIFARE_Write(block, (byte*)s.c_str(), s.length());
}

byte write16(MFRC522::MIFARE_Key* key, byte block, String s, char c=' '){
  while(s.length()<16) s += c;
  if(s.length()>16) s = s.substring(0,16);
  return write(key, block, s);
}

void setup() {
        Serial.begin(9600);        // Initialize serial communications with the PC
        SPI.begin();                // Init SPI bus
        mf.PCD_Init();        // Init MFRC522 card
        
        pinMode(5,OUTPUT);
}

void loop() {
        
        // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) {
                key.keyByte[i] = 0xFF;
        }
        // Look for new cards
        if ( ! mf.PICC_IsNewCardPresent()) {
                return;
        }

        // Select one of the cards
        if ( ! mf.PICC_ReadCardSerial()) {
                return;
        }
        // Now a card is selected. The UID and SAK is in mfrc522.uid.
        tone(5,250,100);
        // Dump UID
        Serial.print("UID: ");
        for (byte i = 0; i < mf.uid.size; i++) {
                Serial.print(mf.uid.uidByte[i] < 0x10 ? " 0" : " ");
                Serial.print(mf.uid.uidByte[i], HEX);
        }
        Serial.println();
        
        Serial.print("Tipo: ");
        Serial.println(mf.PICC_GetTypeName(mf.PICC_GetType(mf.uid.sak)));
        
        /*write16(&key, 8, "Ivan Cea");
        write16(&key, 9, "14 - 12 - 1996");
        write16(&key, 10, "A Coruna, 15178");
        write16(&key, 12, "Oleiros");
        write16(&key, 13, "Os Galgos, N 3");*/
        
        for(int i=8; i<64; i++){
          //if(i%4==3) continue;
          byte status;
          byte block[18];
          byte blockSize=sizeof(block);
          
          Serial.print("Block ");
          if(i<10) Serial.print(" ");
          Serial.print(i);
          Serial.print(":  ");
          
          status = mf.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, i, &key, &(mf.uid));
          if(status != MFRC522::STATUS_OK){
            Serial.println("Fallo en la autenticacion.");
            break;
          }
          
          status = mf.MIFARE_Read(i,block,&blockSize);
          if(status != MFRC522::STATUS_OK){
            Serial.println("Fallo en la lectura.");
            break;
          }else
          
          for(int j=0; j<16; j++){
                /*Serial.print(block[j] < 0x10 ? " 0" : " ");
                Serial.print(block[j], HEX);*/
                Serial.print((char)block[j]);
	        //if ((j % 4) == 3) Serial.print(" ");
          }
          Serial.println();
        }
        
	      mf.PICC_HaltA();
        mf.PCD_StopCrypto1();
}
