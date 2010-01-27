#include "editstring.h"

// Process a key press.  Return true if it was used.

bool EditString::injectKeyPress( const OIS::KeyEvent& arg )
{
  bool keyUsed = true;

  if( isgraph( arg.text ) || isspace( arg.text ) )
  {
    if( mInsert || mPosition == mText.end() )
    {
      mPosition = mText.insert( mPosition, arg.text );
    }
    else
    {
      *mPosition = arg.text;
    }
    mPosition++;
    mCaret++;
  }
  else
  {
    switch( arg.key )
    {
      case OIS::KC_BACK:
        if( mPosition != mText.begin() )
        {
          mPosition = mText.erase( --mPosition );
          --mCaret;
        }
        break;

      case OIS::KC_INSERT:
        mInsert = ! mInsert;
        break;

      case OIS::KC_HOME:
        mPosition = mText.begin();
        mCaret = 0;
        break;

      case OIS::KC_END:
        mPosition = mText.end();
        mCaret = (int)mText.length();
        break;

      case OIS::KC_LEFT:
        if( mPosition != mText.begin() )
        {
          mPosition--;
          mCaret--;
        }
        break;

      case OIS::KC_RIGHT:
        if( mPosition != mText.end() )
        {
          mPosition++;
          mCaret++;
        }
        break;

      case OIS::KC_DELETE:
        if( mPosition != mText.end() )
          mPosition = mText.erase( mPosition );
        break;

      default:
        keyUsed = false;
        break;
    }
  }

  return keyUsed;
}

