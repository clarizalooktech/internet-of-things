#define motionfeedTime 60
#define autofeedTime 120
#define BLYNK_PRINT stdout

#ifdef RASPBERRY
#include <BlynkApiWiringPi.h>
#else
#include <BlynkApiLinux.h>
#endif

#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>
#include <wiringPi.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);

#include <BlynkWidgets.h>

using namespace std;

// Global variables to store the system status
bool sensorOldState = 0;
time_t lastmotionTime = 0;
time_t lastfeedTime = 0;
int feedsLeft = 10;
int autofeed = 0;

// Setup function to parse the token ID, start Blynk, initiate wiringPi and set I/O pins
void setup(int argc, char* argv[])
{
    static const char* auth, * serv;
    static uint16_t port;
    parse_options(argc, argv, auth, serv, port);
    Blynk.begin(auth, serv, port);
    wiringPiSetup();
    pinMode(27, INPUT);
}

// Dispense function
void dispense()
{

    if (feedsLeft)      //Call python script if feeds available & then sync feed time and feeds left
    {
        cout << "Dispensing feed.\n";
        system("python servo.py");
        feedsLeft--;
        lastfeedTime = time(0);
        char* dt = ctime(&lastfeedTime);
        Blynk.virtualWrite(V0, dt);
        Blynk.virtualWrite(V3, feedsLeft);
        cout << dt << "\n";
    }
    else cout<<"No feeds left!!\n\n";

}

//Function executed as soon as connection established to server - sync initial feeds & autofeed enable
BLYNK_CONNECTED() 
{
    Blynk.virtualWrite(V3, feedsLeft);
    Blynk.syncVirtual(V5);
}

// Function to receive manual feed command from the Blynk App
BLYNK_WRITE(V1)
{
    int pinValue = param.asInt();

    if (pinValue == 1)  // When pressed, call dispense function & reset button state
    {
        cout << "Manual Feed.\n";
        dispense();
        int pinValue = 0;
        Blynk.syncVirtual(V1);
    }

}

// Function to receive shutdown command from the Blynk App
BLYNK_WRITE(V4)
{
    int pinValue = param.asInt();

    if (pinValue == 1)  // When pressed, reset button state & then initiate a system shutdown
    {
        cout << "Remote Shutdown.\n";
        int pinValue = 0;  
        Blynk.syncVirtual(V4); 
        system("sudo shutdown now");
    }

}

// Function to sync autofeed enable command from the Blynk App
BLYNK_WRITE(V5)
{
    autofeed = param.asInt();
}

// Endless loop for the embedded application - motion detection & autofeed (timed & motion)
void loop()
{
    Blynk.run();                                    // Continuously run the Blynk interface
    bool sensorCurrentState = digitalRead(27);

    if (sensorCurrentState - sensorOldState == 1)   // Check for rising edge from motion sensor
    {
        cout << "Motion Detected.\n";
        lastmotionTime = time(0);
        char* dt = ctime(&lastmotionTime);
        Blynk.virtualWrite(V2, dt);                 // Sync the last motion time
        
        // Only feed if sufficient time has passed & autofeed is enabled (motion detected case)
        if ((time(0) - lastfeedTime) > motionfeedTime && autofeed) dispense();
        else cout << "No feed dispensed.\n\n";
    }

    sensorOldState = sensorCurrentState;

    // Only feed if sufficient time has passed, autofeed is enabled & feeds available (no motion detected case)
    if ((time(0) - lastfeedTime) > autofeedTime && autofeed && feedsLeft)
    {
        cout << "Timed Feed.\n";
        dispense();
    }

}

// Main function to run the setup function and then start the endless loop
int main(int argc, char* argv[])
{
    setup(argc, argv);

    while (true)
    {
        loop();
    }

    return 0;
}