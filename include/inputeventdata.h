/*
-----------------------------------------------------------------------------

Copyright (c) 2010 Nigel Atkinson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef INPUTEVENTDATA_H_INCLUDED
#define INPUTEVENTDATA_H_INCLUDED

struct InputEventData : public EventData
{
    short x, y;
    OIS::KeyCode key;
    unsigned int parm;
};

struct InputEventDataBindingHelper
{
    static int getKey( InputEventData const* d )
    {
        return (int)d->key;
    }
    static void setKey( InputEventData* d, int key )
    {
        d->key = (OIS::KeyCode)key;
    }

    static char getText( InputEventData const* d )
    {
        static char text;
        text = (char)d->parm;
        return text;
    }

    static void setText( InputEventData* d, const char text )
    {
        d->parm = (unsigned int)text;
    }
};

#endif // INPUTEVENTDATA_H_INCLUDED
