#include <iostream>
#include <exception>
#include <engine.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

using namespace std;

int main()
{
    cout << "Staring engine." << endl;

    //try
    {
        Engine engine;

        if( engine.initialise() )
        {
            cout << endl << "Successfully started game engine." << endl << endl;
        }

        engine.run();
    }
/*
    catch( exception& e )
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.what(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.what() << std::endl;
#endif
    }
*/
    return 0;
}

