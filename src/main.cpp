
int main( int argc, char* argv[] )
{
    EngineInit();
    
    OSLoop();
    
    EngineShutdown();
    
    return 0;
}

void OSLoop( void )
{
    for( ;; )
    {
        EngineFrame();
        
        ProcessOSMessages();
    }
}

void ProcessOSMessages( void )
{
#if WINDOWS
    while( PeekMessage() )
    {
        ShittyWindowsMessaging();
    }
#elif OS_X
    while( nextEventMatchingMask )
    {
        ShittyOSXMessaging();
    }
#endif
}

/*
Objects
*/

class Entity
{
    Transform   object_transform;
    Component   components[100];
    int         num_components;
    
    void AddComponent( ComponentType type );
    void RemoveComponent( ComponentType type );
    Component GetComponent( ComponentType type );
}

class Component
{
    Entity* owner_entity;
}

/*    Object Creation
1.  Create new entity ( transform initialized to 0,0,0 )
2.  Add individual components with default values
3.  Update the values in the components
4.  Done?
*/

/*    Component interaction
Each component is 100% isolated from others and does everything on its own.
There will be some implicit dependencies on others (RigidBody will depend a collidable),
but when that happens, the parent will just ask it's owner (an entity) to get the component it needs.
If the entity doesn't have the requested dependant, it returns null and the original component
can still continue exectution. This might mean it just returns, but thats okay, as long as its graceful.

Some components don't do anything (A collidable component) while others do stuff every frame (particle
emitters).