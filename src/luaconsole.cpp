#include "luaconsole.h"

#define CONSOLE_LINE_LENGTH 96
#define CONSOLE_LINE_COUNT 24
#define CONSOLE_MAX_LINES 32000
#define CONSOLE_MAX_HISTORY 64

using namespace Ogre;
using namespace std;

template<> LuaConsole *Singleton<LuaConsole>::ms_Singleton=0;

LuaConsole::LuaConsole()
{
}

LuaConsole::~LuaConsole()
{
    if( root )
        shutdown();
}

void LuaConsole::init(Ogre::Root *root, lua_State *L)
{
    SceneManager *scene;
    OverlayManager &overlayManager = OverlayManager::getSingleton();

    if(!root->getSceneManagerIterator().hasMoreElements())
      OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "No scene manager found!", "init" );

    this->root=root;
    scene=root->getSceneManagerIterator().getNext();
    root->addFrameListener(this);

    visible = false;
    height = 0;
    start_line = 0;
    cursor_blinkTime = 0;
    cursor_blink = false;

    interpreter = new LuaInterpreter( L );
    print( interpreter->getOutput() );
    interpreter->clearOutput();

    textbox = overlayManager.createOverlayElement("TextArea","ConsoleText");
    textbox->setMetricsMode(GMM_RELATIVE);
    textbox->setPosition(0,0);
    textbox->setParameter("font_name","Console");
    textbox->setParameter("colour_top","1 1 1");
    textbox->setParameter("colour_bottom","0.5 0.5 0.5");
    textbox->setParameter("char_height","0.03");

    panel = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("Panel", "ConsolePanel"));
    panel->setMetricsMode(Ogre::GMM_RELATIVE);
    panel->setPosition(0, 0);
    panel->setDimensions(1, 0);
    panel->setMaterialName("console/background");
    panel->addChild(textbox);

    overlay = overlayManager.create("Console");
    overlay->add2D(panel);
    overlay->show();

    LogManager::getSingleton().getDefaultLog()->addListener(this);
}

void LuaConsole::shutdown()
{
    if( root )
    {
        delete interpreter;

        OverlayManager::getSingleton().destroyOverlayElement( textbox );
        OverlayManager::getSingleton().destroyOverlayElement( panel );
        OverlayManager::getSingleton().destroy( overlay );

        LogManager::getSingleton().getDefaultLog()->removeListener(this);
    }
    root = NULL;
}

void LuaConsole::setVisible(bool fVisible)
{
    visible = fVisible;
}

void LuaConsole::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName )
{
    print( message );
}

bool LuaConsole::frameStarted(const Ogre::FrameEvent &evt)
{
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

    if(visible && height < 1)
    {
        height += evt.timeSinceLastFrame * 10;
        textbox->show();

        if(height >= 1)
        {
            height = 1;
        }
    }
    else if( !visible && height > 0)
    {
        height -= evt.timeSinceLastFrame * 10;
        if(height <= 0)
        {
            height = 0;
            textbox->hide();
        }
    }

    textbox->setPosition(0, (height - 1) * 0.75);
    panel->setDimensions( 1, height * 0.75 );

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

        for(int c = 0; c < CONSOLE_LINE_COUNT; c++)
        {
            if(end == lines.end())
                break;
            end++;
        }

        for(i = start; i != end; i++)
            text += (*i) + "\n";

        //add the edit line with cursor
        string editlineText( editline.getText() + " " );
        if( cursor_blink )
            editlineText[editline.getPosition()] = '_';

        text += interpreter->getPrompt() + editlineText;

        textbox->setCaption(text);

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
            if( column > CONSOLE_LINE_LENGTH )
                pos--;      //we want the current char for the next line
            column = 0;
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
    if( lines.size() > CONSOLE_LINE_COUNT )
        start_line = lines.size() - CONSOLE_LINE_COUNT;

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

bool LuaConsole::injectKeyPress( const OIS::KeyEvent &evt )
{
    switch( evt.key )
    {
        case OIS::KC_RETURN:
            print( interpreter->getPrompt() + editline.getText() );
            interpreter->insertLine( editline.getText() );
            addToHistory( editline.getText() );
            print( interpreter->getOutput() );
            interpreter->clearOutput();
            editline.clear();
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

