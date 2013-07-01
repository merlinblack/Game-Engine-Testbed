/* -------------------------------------------------------------------------------
 *
 * The original of this file has been previously released to the public domain by
 * Nigel Atkinson.
 *
 * See the wiki on www.ogre3d.org
 *
 * -------------------------------------------------------------------------------
 */

#ifndef LUAINTERPRETER_H
#define LUAINTERPRETER_H

#include <lua.hpp>
#include <string>
#include <vector>

#define LI_PROMPT  ">"
#define LI_PROMPT2 ">>"
#define LI_MESSAGE "Nigels wizzbang Lua Interpreting Class. Version 0.1. 2009\n"

static const std::string dumpstack_str(lua_State* L );
static void dumpstack (lua_State *L, const char *message);

typedef std::vector<int> CoroutineList;

class LuaInterpreter
{
    public:
        enum State
        {
            LI_READY = 0,
            LI_NEED_MORE_INPUT,
            LI_ERROR,
            LI_YIELDING
        };

        LuaInterpreter(lua_State *L);
        virtual ~LuaInterpreter();

        // Retrieves the current output from the interpreter.
        std::string getOutput();
        void clearOutput() { mOutput.clear(); }

        std::string getPrompt() { return mPrompt; }

        // Insert (another) line of text into the interpreter.
        // If fInsertInOutput is true, the line will also go into the
        // output.
        State insertLine( std::string& line, bool fInsertInOutput = false );

        // Callback for lua to provide output.
        static int insertOutputFromLua( lua_State *L );

        // Retrieve the current state of affairs.
        State getState() { return mState; }

        // Retrieve the Lua instance
        lua_State* getLuaInstance() { return mL; }

        // Resume a prevously yielded chunk.
        State resume();

    protected:
        lua_State *mL;
        CoroutineList mCoroutines;
        std::string mCurrentStatement;
        std::string mOutput;
        std::string mPrompt;
        State mState;
        bool mFirstLine;

        void reportStack( lua_State* );
};

#endif // LUAINTERPRETER_H
