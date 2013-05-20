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
#include <OIS.h>
#include <list>
#include <string>
#include "editstring.h"
#include "luainterpreter.h"
#include "Gorilla.h"

class LuaConsole: public Ogre::Singleton<LuaConsole>, Ogre::FrameListener, Ogre::LogListener
{
public:
    LuaConsole();
    virtual ~LuaConsole();

    void    init(Gorilla::Screen *screen, lua_State *L );
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
#if OGRE_VERSION < 67584
    void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName );
#else
    void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skip );
#endif
    bool    isLogging();
    void    setLogging( bool enable );

    // Singleton
    static LuaConsole& getSingleton();
    static LuaConsole* getSingletonPtr();

protected:
    bool                    visible;
    bool                    textChanged;
    int                     start_line;
    bool                    cursor_blink;
    float                   cursor_blinkTime;
    bool                    enableLogging;

    // Gorilla
    Gorilla::Screen         *mScreen;
    Gorilla::Layer          *mLayer;
    Gorilla::GlyphData      *mGlyphData;
    Gorilla::Caption        *mPromptLine;
    Gorilla::MarkupText     *mConsoleText;
    Gorilla::Rectangle      *mDecoration;

    EditString              editline;
    LuaInterpreter          *interpreter;

    std::list<std::string>  lines;
    std::list<std::string>  history;

    std::list<std::string>::iterator    history_line;

    void addToHistory( const std::string& cmd );
    std::string escapePercents( const std::string& cmd );
};

#endif // LUACONSOLE_H
