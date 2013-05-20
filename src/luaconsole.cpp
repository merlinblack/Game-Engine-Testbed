/* -------------------------------------------------------------------------------
 *
 * The original of this file has been previously released to the public domain by
 * Nigel Atkinson.
 *
 * See the wiki on www.ogre3d.org
 *
 * Updated to work with Betajaen's Gorilla system.
 *
 * -------------------------------------------------------------------------------
 */

#include "luaconsole.h"

#define CONSOLE_LINE_LENGTH 75
#define CONSOLE_LINE_COUNT 20
#define CONSOLE_MAX_LINES 32000
#define CONSOLE_MAX_HISTORY 64
#define CONSOLE_FONT_INDEX 14
#define CONSOLE_TAB_STOP 16

using namespace Ogre;
using namespace std;

#if OGRE_VERSION < 67584 /* 1.8.0 */
template<> LuaConsole *Singleton<LuaConsole>::ms_Singleton=0;

LuaConsole* LuaConsole::getSingletonPtr()
{
    return ms_Singleton;
}

LuaConsole& LuaConsole::getSingleton()
{
    assert( ms_Singleton );
    return *ms_Singleton;
}
#else
template<> LuaConsole *Singleton<LuaConsole>::msSingleton=0;

LuaConsole* LuaConsole::getSingletonPtr()
{
    return msSingleton;
}

LuaConsole& LuaConsole::getSingleton()
{
    assert( msSingleton );
    return *msSingleton;
}
#endif

LuaConsole::LuaConsole()
{
}

LuaConsole::~LuaConsole()
{
    if( interpreter )
        shutdown();
}

void LuaConsole::init(Gorilla::Screen *screen, lua_State *L)
{
    Root::getSingletonPtr()->addFrameListener(this);

    visible = false;
    start_line = 0;
    cursor_blinkTime = 0;
    cursor_blink = false;
    enableLogging = false;

    interpreter = new LuaInterpreter( L );
    print( interpreter->getOutput() );
    interpreter->clearOutput();

    mScreen = screen;
    mLayer = mScreen->createLayer(15);
    mGlyphData = mLayer->_getGlyphData( CONSOLE_FONT_INDEX );

    mConsoleText = mLayer->createMarkupText( CONSOLE_FONT_INDEX, 10, 10, Ogre::StringUtil::BLANK );
    mConsoleText->width( mScreen->getWidth() - 20 );

    mDecoration = mLayer->createRectangle( 8, 8, mScreen->getWidth() - 16,
                                           (mGlyphData->mLineHeight * CONSOLE_LINE_COUNT) + 2 );
    mDecoration->background_gradient( Gorilla::Gradient_NorthSouth,
                                      Gorilla::rgb( 128, 128, 128, 128 ),
                                      Gorilla::rgb(  64,  64,  64, 128 ) );
    mDecoration->border(2, Gorilla::rgb( 128, 128, 128, 128 ) );
    mPromptLine = mLayer->createCaption( CONSOLE_FONT_INDEX, 10, 10, interpreter->getPrompt() );
    mPromptLine->width( mScreen->getWidth() - 20 );

    print( "%5Lua Console %3'Gorilla'%5 Edition. %RPress %3`%R (Grave) to hide." );

    mLayer->setVisible( visible );

    LogManager::getSingleton().getDefaultLog()->addListener(this);
}

void LuaConsole::shutdown()
{
    if( interpreter )
    {
        delete interpreter;

        mScreen->destroy( mLayer );

        Root::getSingletonPtr()->removeFrameListener(this);

        LogManager::getSingleton().getDefaultLog()->removeListener(this);
    }
    interpreter = NULL;
}

void LuaConsole::setVisible(bool fVisible)
{
    visible = fVisible;
    mLayer->setVisible( visible );
}

#if OGRE_VERSION < 67584
void LuaConsole::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName )
#else
void LuaConsole::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skip )
#endif
{
    if( enableLogging )
        print( message );
}

bool LuaConsole::isLogging()
{
    return enableLogging;
}

void LuaConsole::setLogging( bool enable )
{
    enableLogging = enable;
}

bool LuaConsole::frameStarted(const Ogre::FrameEvent &evt)
{
    if( interpreter->getState() == LuaInterpreter::LI_YIELDING )
    {
        interpreter->resume();
    }

    if(visible)
    {
        cursor_blinkTime += evt.timeSinceLastFrame;

        if( cursor_blinkTime > 0.5f )
        {
            cursor_blinkTime -= 0.5f;
            cursor_blink = ! cursor_blink;
            textChanged = true;
        }
    }

    if(textChanged)
    {
        String text;
        std::list<string>::iterator i,start,end;

        //make sure is in range
        //NOTE: the code elsewhere relies on start_line's signedness.
        //I.e. the ability to go below zero and not wrap around to a high number.
        if(start_line < 0 )
            start_line = 0;
        if((unsigned)start_line > lines.size())
            start_line = lines.size();

        start=lines.begin();

        for(int c = 0; c < start_line; c++)
            start++;

        end = start;

        int line_count = 0;
        for(int c = 0; c < CONSOLE_LINE_COUNT-1; c++)
        {
            if(end == lines.end())
                break;
            end++;
            line_count++;
        }

        for(i = start; i != end; i++)
            text += (*i) + "\n";

        //add the edit line with cursor

        string editlineText( editline.getText() + " " );

        if( cursor_blink )
            editlineText[editline.getPosition()] = '_';

        mConsoleText->text( text.c_str() );

        mPromptLine->text( ( interpreter->getPrompt() + editlineText ).c_str() );
        mPromptLine->top( 10 + (line_count * mGlyphData->mLineHeight) );

        textChanged = false;
    }

    return true;
}

bool LuaConsole::frameEnded(const Ogre::FrameEvent &evt)
{
    return true;
}

void LuaConsole::print( std::string text )
{
    string line;
    string::iterator pos;
    int column;

    pos = text.begin();
    column = 1;

    while( pos != text.end() )
    {
        if( *pos == '\n' || column > CONSOLE_LINE_LENGTH )
        {
            lines.push_back( line );
            line.clear();
            if( *pos != '\n' )
                pos--;      //we want the current char for the next line
            column = 0;
        }
        else if( *pos == '\t' )
        {
            // Push at least one space.
            line.push_back(' ');
            column++;

            // fill until next multiple of CONSOLE_TAB_STOP
            while(( column % CONSOLE_TAB_STOP ) != 0 )
            {
                line.push_back(' ');
                column++;
            }
        }
        else
        {
            line.push_back( *pos );
            column++;
        }
        pos++;
    }
    if( line.length() )
    {
        if( lines.size() > CONSOLE_MAX_LINES-1 )
            lines.pop_front();

        lines.push_back( line );
    }

    // Make sure last text printed is in view.
    if( lines.size() > CONSOLE_LINE_COUNT - 1 )
        start_line = lines.size() - (CONSOLE_LINE_COUNT - 1);

    textChanged = true;

    return;
}

void LuaConsole::addToHistory( const string& cmd )
{
    history.remove( cmd );
    history.push_back( cmd );
    if( history.size() > CONSOLE_MAX_HISTORY )
        history.pop_front();
    history_line = history.end();
}

string LuaConsole::escapePercents( const string& cmd )
{
    // Escape '%' characters.
    string escaped = cmd;
    size_t pos = 0;
    while( (pos = escaped.find( '%', pos )) != string::npos )
    {
        escaped.insert( pos, 1, '%' );
        ++pos;
        ++pos;
    }
    return escaped;
}

bool LuaConsole::injectKeyPress( const OIS::KeyEvent &evt )
{
    string escaped;

    switch( evt.key )
    {
        case OIS::KC_RETURN:
            escaped = escapePercents( editline.getText() );
            print( interpreter->getPrompt() + escaped );
            interpreter->insertLine( editline.getText() );
            addToHistory( escaped );
            print( interpreter->getOutput() );
            interpreter->clearOutput();
            editline.clear();
            // Call Lua function to clear autoComplete state.
            {
                lua_State *L = interpreter->getLuaInstance();
                lua_getglobal( L, "autoCompleteClear" );
                if( !lua_isnil( L, -1 ) )
                {
                    lua_pcall( L, 0, 0, 0 );
                    // leave any error message on the stack.
                }
                else
                    lua_pop(L, 1);
            }
            break;

        case OIS::KC_PGUP:
            start_line -= CONSOLE_LINE_COUNT;
            textChanged = true;
            break;

        case OIS::KC_PGDOWN:
            start_line += CONSOLE_LINE_COUNT;
            textChanged = true;
            break;

        case OIS::KC_UP:
            if( !history.empty() )
            {
                if( history_line == history.begin() )
                    history_line = history.end();
                history_line--;
                editline.setText( *history_line );
                textChanged = true;
            }
            break;

        case OIS::KC_DOWN:
            if( !history.empty() )
            {
                history_line++;
                if( history_line == history.end() )
                    history_line = history.begin();
                editline.setText( *history_line );
                textChanged = true;
            }
            break;

        case OIS::KC_TAB:   // Tab completion implemented in Lua
            {
                lua_State *L = interpreter->getLuaInstance();
                lua_getglobal( L, "autoComplete" );
                if( !lua_isnil( L, -1 ) )
                {
                    lua_pushstring( L, editline.getText().c_str() );
                    if( !lua_pcall( L, 1, 1, 0 ) )
                    {
                        std::string ret(lua_tostring( L, -1 ));
                        editline.setText( ret );
                        lua_pop(L, 1);
                    }
                    // else leave the error message on the stack.
                }
                else
                    lua_pop(L, 1);
            }
            break;

        default:
            textChanged = editline.injectKeyPress( evt );
            break;
    }

    return true;
}

    void LuaConsole::clearHistory()
    {
        history.clear();
    }

    void LuaConsole::clearConsole()
    {
        lines.clear();
        textChanged = true;
    }

