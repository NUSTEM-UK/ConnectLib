/**
 * Connect.h - Library for Connect devices
 * Created by Jonathan Sanderson, July 2021
 * Northumbria University
 * See https://nustem.uk/connect
 * TODO: License
 *
 */

#ifndef Connect_h
#define Connect_h
#include <Kniwwelino.h>
#include <ConnectServo.h>
// #include <ServoMessenger.h>

#define WIFI_ON 1
#define VERSION "0.03"
#define NUMBER_OF_MOODS 6

// Function prototypes
static void messageReceived(String &t, String &p);
int getMoodIndexFromString(String moodString);
void change_mood();
void handleButtons();
void checkMood();
void connectSetup();
// Mood prototypes (all declared weak so they can be overridden)
void doHappy();
void doSad();
void doHeart();
void doSkull();
void doSilly();
void doDuck();
// Main event loop
void connectLoop();

void updateConnectServos();

/**
 * Mood structures and supporting scafolding
 */
// We need a function pointer type to include in our Mood struct
typedef void (*CallbackFunction)(void);
// ...and the struct itself
typedef struct {
    int index;
    String text;
    String icon;
    CallbackFunction callback;
} Mood;

// We need to keep track of the number of moods manually, because C
// ...and this has to be const for the array declaration to work
// ...and we need the array declaration here because we want moods[]
// to be global
// extern Mood moods[NUMBER_OF_MOODS];   // Initialiser has to be inside setup(), or bad things happen.

extern Mood myMood;        // Should really be intrinsicMood or internalMood
extern Mood extrinsicMood; // The current network mood. Should be externalMood?
extern Mood performedMood; // The current displayed mood

extern String received_string;
// static void messageReceived (String &t, String &p);
// void handleButtons();
// void checkMood();

// extern ServoMessenger ConnectMessenger;

#endif

