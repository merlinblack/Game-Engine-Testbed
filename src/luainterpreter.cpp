#include "luainterpreter.h"

using std::string;

// The address of this int in memory is used as a garenteed unique id
// in the lua registry
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
LuaInterpreter::State LuaInterpreter::insertLine( string& line, bool fInsertInOutput )
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

        // If the error is not a syntax error cuased by not enough of the
        // statement been yet entered...
        if( error.substr( error.length()-6, 5 ) != "<eof>" )
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

    // The statment compiled correctly, now run it.

    if( ! lua_pcall( mL, 0, LUA_MULTRET, 0 ) )
    {
        // The error message (if any) will be added to the output as part
        // of the stack reporting.
        lua_gc( mL, LUA_GCCOLLECT, 0 );     // Do a full garbage collection on errors.
        mState = LI_ERROR;
    }

    mCurrentStatement.clear();
    mFirstLine = true;

    // Report stack contents
    if ( lua_gettop(mL) > 0)
    {
      lua_getglobal(mL, "print");
      lua_insert(mL, 1);
      lua_pcall(mL, lua_gettop(mL)-1, 0, 0);
    }

    mPrompt = LI_PROMPT;

    // Clear stack
    lua_settop( mL, 0 );

    return mState;
}

// Callback for lua to provide output.
int LuaInterpreter::insertOutputFromLua( lua_State *L )
{
    // Retreive the current interpreter for current lua state.
    LuaInterpreter *interpreter;

    lua_pushlightuserdata( L, (void *)&LuaRegistryGUID );
    lua_gettable( L, LUA_REGISTRYINDEX );
    interpreter = static_cast<LuaInterpreter *>(lua_touserdata( L, -1 ));

    if( interpreter )
        interpreter->mOutput += lua_tostring( L, -2 );

    lua_settop( L, 0 );
    return 0;
}
