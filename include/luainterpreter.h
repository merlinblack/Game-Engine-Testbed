#ifndef LUAINTERPRETER_H
#define LUAINTERPRETER_H

#include <lua.hpp>
#include <string>

#define LI_PROMPT  ">"
#define LI_PROMPT2 ">>"
#define LI_MESSAGE "Nigels wizzbang Lua Interpreting Class. Version 0.1. 2009\n"

class LuaInterpreter
{
    public:
        enum State
        {
            LI_READY = 0,
            LI_NEED_MORE_INPUT,
            LI_ERROR
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

    protected:
        lua_State *mL;
        std::string mCurrentStatement;
        std::string mOutput;
        std::string mPrompt;
        State mState;
        bool mFirstLine;

    private:
};

#endif // LUAINTERPRETER_H
