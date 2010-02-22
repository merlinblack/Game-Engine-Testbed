/* -------------------------------------------------------------------------------
 *
 * The original of this file has been previously released to the public domain by
 * Nigel Atkinson.
 *
 * See the wiki on www.ogre3d.org
 *
 * -------------------------------------------------------------------------------
 */

#ifndef LUACONSOLE_H
#define LUACONSOLE_H

#include <OgreRoot.h>
#include <OgreFrameListener.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OIS.h>
#include <list>
#include <string>
#include "editstring.h"
#include "luainterpreter.h"

class LuaConsole: public Ogre::Singleton<LuaConsole>, Ogre::FrameListener, Ogre::LogListener
{
public:
    LuaConsole();
    virtual ~LuaConsole();

    void    init(Ogre::Root *root, lua_State *L );
    void    shutdown();
    void    setVisible(bool fVisible);
    bool    isVisible(){ return visible; }
    void    print( std::string text );
    bool    injectKeyPress( const OIS::KeyEvent &evt );
    void    clearHistory();
    void    clearConsole();

    // Frame listener
    bool    frameStarted(const Ogre::FrameEvent &evt);
    bool    frameEnded(const Ogre::FrameEvent &evt);

    // Log Listener
    void    messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName );
    bool    isLogging();
    void    setLogging( bool enable );

protected:
    bool                    visible;
    bool                    textChanged;
    float                   height;
    int                     start_line;
    bool                    cursor_blink;
    float                   cursor_blinkTime;
    bool                    enableLogging;

    Ogre::Overlay           *overlay;
    Ogre::OverlayContainer  *panel;
    Ogre::OverlayElement    *textbox;
    Ogre::Root              *root;

    EditString              editline;
    LuaInterpreter          *interpreter;

    std::list<std::string>  lines;
    std::list<std::string>  history;

    std::list<std::string>::iterator    history_line;

    void addToHistory( const std::string& cmd );
};

#endif // LUACONSOLE_H
