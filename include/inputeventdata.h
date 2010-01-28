#ifndef INPUTEVENTDATA_H_INCLUDED
#define INPUTEVENTDATA_H_INCLUDED

struct InputEventData : public EventData
{
    float x, y;
    OIS::KeyCode key;
	unsigned int parm;
};

#endif // INPUTEVENTDATA_H_INCLUDED
