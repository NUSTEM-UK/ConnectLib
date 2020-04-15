/**
 * Connect.h - Library for Connect devices
 * Created by Jonathan Sanderson, April 2020
 * Northumbria University 
 * See https://nustem.uk/connect
 * TODO: License
*/

#ifndef Connect_h
#define Connect_h

#include "Kniwwelino.h"

class Connect
{
    public:
        Connect(int pin); //root object, needs variable stubs
        void func1();
        void func2();
    private:
        int _pin;
};

void connectInit(void);



#endif