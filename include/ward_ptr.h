/*
-----------------------------------------------------------------------------

Copyright (c) 2010 Nigel Atkinson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include <exception>
#include <map>
#include <LuaBridge.h>

/*
 * ward_ptr
 *
 * Similar to a weak reference, holds a pointer to a object.  Does not control
 * the life time of the object, but can be marked 'invalid', afterwhich any
 * attempt to dereference results in an exception.
 *
 */

template<typename T>
class ward_ptr;

struct ward_ptr_data
{
    int ref_count;
    bool valid;
};

template<typename T>
struct ward_ptr_map
{
    typedef std::map<const T*, ward_ptr_data* > type;
};

template<typename T>
class ward_ptr
{
private:
    T* ptr;
    ward_ptr_data* data;
    static typename ward_ptr_map<T>::type backref;

public:
    static void debug()
    {
        typename ward_ptr_map<T>::type::iterator it;
        for( it = backref.begin(); it != backref.end(); it++ )
        {
            const ward_ptr_data* wp = it->second;
        }
    }

    ward_ptr() : ptr(NULL), data(NULL)
    {
    }
    ward_ptr( const ward_ptr<T>& p )
    {
        debug();
        ptr = p.ptr;
        data = p.data;
        data->ref_count++;
    }
    ward_ptr<T>& operator = ( ward_ptr<T>& p )
    {
        debug();
        decref();
        ptr = p.ptr;
        data = p.data;
        data->ref_count++;
        return this;
    }
    ward_ptr( T* p )
    {
        debug();
        ptr = p;

        typename ward_ptr_map<T>::type::iterator it = backref.find( p );

        if( it != backref.end() )
        {
            data = it->second;
        }
        else
        {
            data = new ward_ptr_data;
            data->ref_count = 0;
            data->valid = true;
            backref[ptr] = data;
        }
        data->ref_count++;
    }
    T* get() const
    {
        debug();
        if( ! data->valid )
            throw std::logic_error( "ward_ptr: exception - instance pointer no longer valid." );
        return ptr;
    }
    T* operator->() const
    {
        if( ! data->valid )
            throw std::logic_error( "ward_ptr: exception - instance pointer no longer valid." );
        return ptr;
    }
    operator T*() const
    {
        if( ! data->valid )
            throw std::logic_error( "ward_ptr: exception - instance pointer no longer valid." );
        return ptr;
    }
    void invalidate()
    {
        data->valid = false;
    }
    virtual ~ward_ptr()
    {
        decref();
    }
    void decref()
    {
        data->ref_count--;
        if( data->ref_count == 0 )
        {
            typename ward_ptr_map<T>::type::iterator it = backref.find( ptr );
            backref.erase(it);
            delete data;
        }
    }
};

template<typename T>
typename ward_ptr_map<T>::type ward_ptr<T>::backref;

template<typename T>
T* get_pointer( ward_ptr<T> p )
{
    return p.get();
}

namespace luabridge
{
    template<class T>
        struct ContainerTraits<ward_ptr<T> >
        {
            typedef T Type;

            static T* get( ward_ptr<T> const& c )
            {
                return c.get();
            }
        };
};

