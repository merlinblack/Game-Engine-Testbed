// Removed from engine.cpp but kept for future reference
//
 /*
        if( data->key == OIS::KC_T )    // T for Test!
        {
            // Testing.
            Ogre::Root *root = Ogre::Root::getSingletonPtr();
            Ogre::SceneManager* mgr = root->getSceneManager( "SceneManagerInstance" );
            Ogre::Entity *eTest = mgr->createEntity( "FloorNav.mesh" );

            NavigationMesh navMesh;

            navMesh.BuildFromOgreMesh( eTest->getMesh() );

            mgr->destroyEntity( eTest );

            // Get where the mouse is pointing on the floor
            GameEntityPtr base = gameEntityManager.getGameEntity( "Floor" );

            if( !base ) // test scene not yet loaded.
            {
                Ogre::LogManager::getSingleton().stream() << "Load the test scene first you doofus.";
                return true;
            }

            Ogre::Vector3 pos = base->hitPosition( _X, _Y );

            if( pos.x == Ogre::Math::POS_INFINITY )
                return true;    // Mouse was not over the Floor.

            // Test out path finding.
            NavigationPath* path = navMesh.findNavigationPath( Ogre::Vector3( 0, 25.6, 0 ), pos );

            mgr->destroyManualObject( "path" );

            if( path )
            {
                const Ogre::Real width = 5.0;

                NavigationPath *straightenedPath = navMesh.straightenPath( path, Ogre::Radian( Ogre::Math::PI/2 ), width );

                Ogre::SceneNode* myManualObjectNode;

                if( mgr->hasSceneNode( "path_node" ) )
                    myManualObjectNode = mgr->getSceneNode( "path_node" );
                else
                    myManualObjectNode = mgr->getRootSceneNode()->createChildSceneNode("path_node");

                Ogre::ManualObject* myManualObject =  mgr->createManualObject("path");

                myManualObject->begin("debug/yellow", Ogre::RenderOperation::OT_LINE_STRIP);

                myManualObject->position(*(path->begin()));
                for( NavigationPath::iterator i = path->begin() + 1; i != path->end(); i++ )
                {
                    Ogre::Vector3 tick;
                    Ogre::Vector3 direction = *i - *(i-1);
                    Ogre::Vector3 rightAngle( direction.z, 0, -direction.x );
                    rightAngle.normalise();

                    myManualObject->position(i->x, i->y+2, i->z);

                    tick = *i + rightAngle * width;
                    myManualObject->position( tick );
                    myManualObject->position(i->x, i->y+2, i->z);

                    tick = *i - rightAngle * width;
                    myManualObject->position( tick );
                    myManualObject->position(i->x, i->y+2, i->z);
                }

                myManualObject->end();

                myManualObject->begin("debug/cyan", Ogre::RenderOperation::OT_LINE_STRIP);

                myManualObject->position(*(straightenedPath->begin()));
                for( NavigationPath::iterator i = straightenedPath->begin() + 1; i != straightenedPath->end(); i++ )
                {
                    std::cout << *i << std::endl;

                    Ogre::Vector3 tick;
                    Ogre::Vector3 direction = *i - *(i-1);
                    Ogre::Vector3 rightAngle( direction.z, 0, -direction.x );
                    rightAngle.normalise();

                    myManualObject->position(i->x, i->y+2.5, i->z);

                    tick = *i + rightAngle * width;
                    myManualObject->position( tick );
                    myManualObject->position(i->x, i->y+2.5, i->z);

                    tick = *i - rightAngle * width;
                    myManualObject->position( tick );
                    myManualObject->position(i->x, i->y+2.5, i->z);
                }

                myManualObject->end();

                myManualObject->setQueryFlags( 0 );
                myManualObject->setRenderQueueGroup( Ogre::RENDER_QUEUE_OVERLAY );

                myManualObjectNode->attachObject(myManualObject);

                delete path;
                delete straightenedPath;
            }

            //navMesh.DebugTextDump( std::cout );

            return true;
        }
*/
 /*
        if( data->key == OIS::KC_Y )    // Yet another test.
        {
            // Get the robot and animate him.

            GameEntityPtr robot = gameEntityManager.getGameEntity( "Mike" );

            if( !robot ) // test scene not yet loaded.
            {
                Ogre::LogManager::getSingleton().stream() << "Load the test scene first you doofus.";
                return true;
            }

            MeshAnimationPtr anim( new MeshAnimation( robot->mesh, "Idle" ) );
            AnimationManager::getSingleton().addAnimation( anim );
            anim->setWeighting( 0.0f );
            anim->fadeIn();
            anim->start();
        }
 */
 /*
        if( data->key == OIS::KC_U )
        {
            // Get the robot and animate him.

            GameEntityPtr robot = gameEntityManager.getGameEntity( "Mike" );

            if( !robot ) // test scene not yet loaded.
            {
                Ogre::LogManager::getSingleton().stream() << "Load the test scene first you doofus.";
                return true;
            }

            MovementAnimationPtr anim( new MovementAnimation( robot->sceneNode,
                                                              robot->sceneNode->getPosition() + Ogre::Vector3( 10, 0, 0 ),
                                                              10.0f ) );

            AnimationManager::getSingleton().addAnimation( anim );
            anim->start();

            Ogre::Quaternion q( Ogre::Degree(180), Ogre::Vector3::UNIT_Y);

            RotationAnimationPtr anim2( new RotationAnimation( robot->sceneNode, q, 2 ));

            AnimationManager::getSingleton().addAnimation( anim2 );
            anim2->start();
        }
*/
