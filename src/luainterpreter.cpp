/* -------------------------------------------------------------------------------
 *
 * The original of this file has been previously released to the public domain by
 * Nigel Atkinson.
 *
 * See the wiki on www.ogre3d.org
 *
 * -------------------------------------------------------------------------------
 */


#include "luainterpreter.h"
#include <sstream>

using std::string;
using std::stringstream;

// The address of this int in memory is used as a guaranteed unique id
// in the Lua registry
static const char LuaRegistryGUID = 0;

LuaInterpreter::LuaInterpreter(lua_State *L) : mL(L), mState(LI_READY), mFirstLine(true)
{
    mOutput.clear();
    mCurrentStatement.clear();
    lua_pushlightuserdata( mL, (void *)&LuaRegistryGUID );
    lua_pushlightuserdata( mL, this );
    lua_settable( mL, LUA_REGISTRYINDEX );

    lua_register( mL, "interpreterOutput", &LuaInterpreter::insertOutputFromLua );

#ifdef LI_MESSAGE
    mOutput = LI_MESSAGE;
#endif
    mPrompt = LI_PROMPT;
}

LuaInterpreter::~LuaInterpreter()
{
    lua_register( mL, "interpreterOutput", NULL );
    lua_pushlightuserdata( mL, (void *)&LuaRegistryGUID );
    lua_pushnil( mL );
    lua_settable( mL, LUA_REGISTRYINDEX );
}

// Retrieves the current output from the interpreter.
string LuaInterpreter::getOutput()
{
    return mOutput;
}

// Insert (another) line of text into the interpreter.
LuaInterpreter::State LuaInterpreter::insertLine( std::string& line, bool fInsertInOutput )
{
    if( fInsertInOutput == true )
    {
        mOutput += line;
        mOutput += '\n';
    }

    if( mFirstLine && line.substr(0,1) == "=" )
    {
        line = "return " + line.substr(1, line.length()-1 );
    }

    mCurrentStatement += " ";
    mCurrentStatement += line;
    mFirstLine = false;

    mState = LI_READY;

    if( luaL_loadstring( mL, mCurrentStatement.c_str() ) )
    {
        string error( lua_tostring( mL, -1 ) );
        lua_pop( mL, 1 );

        // If the error is not a syntax error caused by not enough of the
        // statement been yet entered...
        if( error.substr( error.length()-5, 5 ) != "<eof>" )
        {
            mOutput += error;
            mOutput += "\n";
            mOutput += LI_PROMPT;
            mCurrentStatement.clear();
            mState = LI_ERROR;

            return mState;
        }
        // Otherwise...
        else
        {
            // Try putting "()" on the end.  This allows commands such as 'quit' to invoke 'quit()'
            if( luaL_loadstring( mL, (mCurrentStatement + "()").c_str() ) )
            {
                // Didn't work.  Remove error message.
                lua_pop( mL, 1 );

                // Secondary prompt
                mPrompt = LI_PROMPT2;

                mState = LI_NEED_MORE_INPUT;

                return mState;
            }
        }
    }

    // The statement compiled correctly, get a new thread, grab a reference, and run it.

    lua_State *thread = lua_newthread( mL );

    // Grab a reference
    int mCoroutineRef = luaL_ref( mL, LUA_REGISTRYINDEX );

    // Move compiled chunk to thread.
    lua_xmove( mL, thread, 1 );

    // Run!
    int ret = lua_resume( thread, mL, 0 );

    switch( ret )
    {
        default:
            // The error message (if any) will be added to the output as part
            // of the stack reporting.
            luaL_unref( mL, LUA_REGISTRYINDEX, mCoroutineRef );
            // Zap nil return value and replace with message.
            if( lua_isnil( thread, 1 ) )
            {
                lua_pushstring( thread, "Error: " );
                lua_replace( thread, 1 );
                // Adjust the 'extra' error info off the stack.
                lua_settop( thread, 3 );
            }
            // else leave it alone.

            mState = LI_ERROR;
            break;
        case LUA_YIELD:
            // Chunk has yielded.
            mCoroutines.push_back( mCoroutineRef );
            mState = LI_YIELDING;
            break;
        case 0:
            // Ran to completion.
            luaL_unref( mL, LUA_REGISTRYINDEX, mCoroutineRef );
            break;
    }

    mCurrentStatement.clear();
    mFirstLine = true;

    reportStack( thread );

    mPrompt = LI_PROMPT;

    return mState;
}

LuaInterpreter::State LuaInterpreter::resume()
{
    if( mCoroutines.empty() == true )
    {
        return mState;  // No coroutines to run.
    }

    CoroutineList::iterator coroutine = mCoroutines.begin();

    while( coroutine != mCoroutines.end() )
    {

        lua_rawgeti( mL, LUA_REGISTRYINDEX, *coroutine );    // Get the thread back...
        lua_State* thread = lua_tothread( mL, -1 );
        lua_pop( mL, 1 );

        int ret = lua_resume( thread, mL, 0 );

        switch( ret )
        {
            default:
                // The error message (if any) will be added to the output as part of the stack reporting.
                luaL_unref( mL, LUA_REGISTRYINDEX, *coroutine );
                mState = LI_ERROR;
                coroutine = mCoroutines.erase( coroutine );
                break;
            case LUA_YIELD:
                // Chunk has yielded.
                mState = LI_YIELDING;
                coroutine++;
                break;
            case 0:
                // Ran to completion.
                luaL_unref( mL, LUA_REGISTRYINDEX, *coroutine );
                mState = LI_READY;
                coroutine = mCoroutines.erase( coroutine );
                break;
        }

        reportStack( thread );
    }

    return mState;
}

void LuaInterpreter::reportStack( lua_State* thread )
{
    // Report stack contents
    // In the case of a yielded chunk these are the parameters to yield.
    if ( lua_gettop(thread) > 0)
    {
      dumpstack( thread, "reportStack" );
      string stack = dumpstack_str( thread );
      lua_settop( thread, 0 );
      lua_getglobal( thread, "print" );
      lua_pushstring( thread, stack.c_str() );
      lua_pcall(thread, 1, 0, 0);
    }
}

// Callback for Lua to provide output.
int LuaInterpreter::insertOutputFromLua( lua_State *L )
{
    // Retrieve the current interpreter for current Lua state.
    LuaInterpreter *interpreter;

    lua_pushlightuserdata( L, (void *)&LuaRegistryGUID );
    lua_gettable( L, LUA_REGISTRYINDEX );
    interpreter = static_cast<LuaInterpreter *>(lua_touserdata( L, -1 ));

    if( interpreter )
        interpreter->mOutput += lua_tostring( L, -2 );

    lua_settop( L, 0 );
    return 0;
}

static const string dumpstack_str(lua_State* L )
{
    static stringstream ss;
    int i;
    int top = lua_gettop(L);

    ss.str("");

    for (i = 1; i <= top; i++)
    {
        ss << i << " " << luaL_typename(L,i) << " - ";
        switch (lua_type(L, i))
        {
            case LUA_TNUMBER:
                ss << lua_tonumber(L,i) << "\n";
            break;
            case LUA_TSTRING:
                ss << lua_tostring(L,i) << "\n";
                break;
            case LUA_TBOOLEAN:
                ss << (lua_toboolean(L, i) ? "true" : "false") << "\n";
                break;
            case LUA_TNIL:
                ss << "nil\n";
                break;
            default:
                ss << std::hex << "0x" << lua_topointer(L,i) << "\n";
                break;
        }
    }
    return ss.str();
}

static void dumpstack (lua_State *L, const char *message)
{
    printf("dumpstack -- %s\n",message);
    printf("%s\n", dumpstack_str( L ).c_str() );
    printf("dumpstack -- END\n");
}

