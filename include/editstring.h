/* -------------------------------------------------------------------------------
 *
 * The original of this file has been previously released to the public domain by
 * Nigel Atkinson.
 *
 * See the wiki on www.ogre3d.org
 *
 * -------------------------------------------------------------------------------
 */

#pragma once

#include <string>
#include <OIS.h>

class EditString
{
public:
    EditString(void) : mInsert(true), mPosition(mText.begin()), mCaret(0)
    {}

    EditString( std::string newText )
    {
        setText( newText );
    }

    ~EditString(void)
    {}


protected:

    // The text for editing
    std::string mText;

    // Overwrite or insert
    bool mInsert;

    // Position for insert / overwrite
    std::string::iterator mPosition;

    // Caret Position - for positioning the cursor.
    int mCaret;

public:
    void setText( std::string& newText )
    {
        mText = newText;
        mPosition = mText.end();
        mCaret = (int)mText.length();
    }

    void clear()
    {
        mText.clear();
        mPosition = mText.end();
        mCaret = 0;
    }

    std::string & getText() { return mText; }

    bool inserting() { return mInsert; }

    bool injectKeyPress( const OIS::KeyEvent& arg );

    // gets the current position in the text for cursor placement
    int getPosition(void) { return mCaret; }
};

