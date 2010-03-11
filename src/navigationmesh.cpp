#include <navigationmesh.h>

NavigationCell::NavigationCell( Ogre::Vector3 a, Ogre::Vector3 b, Ogre::Vector3 c )
{
    mVertices[0] = a;
    mVertices[1] = b;
    mVertices[2] = c;
    mLinks[0] = mLinks[1] = mLinks[2] = 0;
}

bool NavigationCell::hasVertex( Ogre::Vector3& vec )
{
    if( mVertices[0] == vec )   return true;
    if( mVertices[1] == vec )   return true;
    if( mVertices[2] == vec )   return true;
    return false;
}

NavigationMesh::NavigationMesh( Ogre::Vector3 position /* = Ogre::Vector3::ZERO */ ,
                                Ogre::Quaternion rotation /* = Ogre::Quaternion::IDENTITY */ ,
                                Ogre::Vector3 scale /* = Ogre::Vector3::UNIT_SCALE */ ) :
    mPosition( position ),
    mRotation( rotation ),
    mScale( scale )
{
}

NavigationMesh::~NavigationMesh()
{
    // Note, Cells are "free'd" when mCells goes out of scope.
}

void NavigationMesh::BuildFromOgreMesh( Ogre::MeshPtr mesh )
{
    using namespace Ogre;

    size_t vertex_count;
    Vector3* vertices;
    size_t index_count;
    unsigned long* indices;

    OgreTools::GetMeshInformation( mesh, vertex_count, vertices, index_count, indices,
                                   mPosition, mRotation, mScale );

    // Add each triangle as a navigation cell.
    for( size_t i = 0; i < index_count; i += 3 )
    {
        NavigationCell cell( vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]] );

        mCells.push_back( cell );
    }

    delete [] vertices;
    delete [] indices;

    // Compute nieghbours.
    // There will be a better way, but this works for now
    // and this does not run very often.

    CellVector::iterator current;
    CellVector::iterator test;

    for( current = mCells.begin(); current != mCells.end(); current++ )
    {
        for( test = mCells.begin(); test != mCells.end(); test++ )
        {
            if( current != test )
            {
                if( test->hasVertex( current->mVertices[0] ) )
                {
                    if( test->hasVertex( current->mVertices[1] ) )
                    {
                        current->mLinks[0] = &(*test);
                        continue;
                    }
                    if( test->hasVertex( current->mVertices[2] ) )
                    {
                        current->mLinks[2] = &(*test);
                        continue;
                    }
                }
                if( test->hasVertex( current->mVertices[1] ) && test->hasVertex( current->mVertices[2] ) )
                {
                    current->mLinks[1] = &(*test);
                }
            }
        }
    }

    return;
}

void NavigationMesh::DebugTextDump( std::ostream &out )
{
    out << "Navigation Mesh" << std::endl;

    for( CellVector::iterator i = mCells.begin(); i != mCells.end(); i++ )
    {
        out << "Cell: " << &(*i) << std::endl;
        out << i->mVertices[0] << std::endl;
        out << i->mVertices[1] << std::endl;
        out << i->mVertices[2] << std::endl;
        out << "Link 1: " << i-> mLinks[0] << std::endl;
        out << "Link 2: " << i-> mLinks[1] << std::endl;
        out << "Link 3: " << i-> mLinks[2] << std::endl;
    }
}
