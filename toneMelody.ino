/*
Connections For this application;
Use Freeduino-RichBoard made by www.EmbeddedMarket.com
1. Connect Digital Pin 9 to Buz pin in Section 9 on Freeduino Board
2. Connect USB Cable
*/


/* Melody
 * (cleft) 2005 D. Cuartielles for K3
 *
 * This example uses a piezo speaker to play melodies.  It sends
 * a square wave of the appropriate frequency to the piezo, generating
 * the corresponding tone.
 *
 * The calculation of the tones is made following the mathematical
 * operation:
 *
 *       timeHigh = period / 2 = 1 / (2 * toneFrequency)
 *
 * where the different tones are described as in the table:
 *
 * note   frequency   period  timeHigh
 * c          261 Hz          3830  1915  
 * d          294 Hz          3400  1700  
 * e          329 Hz          3038  1519  
 * f          349 Hz          2864  1432  
 * g          392 Hz          2550  1275  
 * a          440 Hz          2272  1136  
 * b          493 Hz          2028  1014  
 * C          523 Hz          1912  956
 *
 * http://www.arduino.cc/en/Tutorial/Melody
 */


#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

BridgeServer server;
  
int speakerPin = 6;

int length = 15; // the number of notes

//twinkle twinkle little star
char notes[] = "ccggaagffeeddcggffeedggffeed"; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 200;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup() {
  pinMode(speakerPin, OUTPUT);

// Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();

  
}

void loop() {

  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
   
  }


  
  playMelody();
}


void process(BridgeClient client) {
  // read the command
  String command = client.readStringUntil('/');


  if (command == "tempo"){
    tempoCommand(client);
  }
}

void playMelody()
{for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    
    // pause between notes
    delay(tempo / 2); 
  }
}

void tempoCommand (BridgeClient client) {
  tempo = client.parseInt();

  // Send feedback to client
  client.print(F("tempo set to"));

  client.println(tempo);

  
}

