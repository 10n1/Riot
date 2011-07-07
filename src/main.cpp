
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
    Component   components[];
    int         num_components;
    
    void AddComponent( ComponentType type );
    void RemoveComponent( ComponentType type );
    Component GetComponent( ComponentType type );
    Transform GetTransform( void );
    void SetTransform( Transform new_transform ); // Should probably buffer the transform, but thats irrelevant now
}

class Component
{
    Entity* owner_entity;
}

//-------------------------------------------------------
class ExampleMeshRenderComponent
{
    Mesh    mesh_to_render;
    
    void ProcessComponent( void )
    {
        Renderer->SetTransform( owner_entity->GetTransform() );
        Renderer->RenderMesh( mesh_to_render );
    }
}

class ExampleCollidableComponent
{
    Shape   collision_shape;
}

class ExampleRigidBody
{
    float3  velocity;
    float3  acceleration;
    float   mass;
    
    void ProcessComponent( void )
    {
        ExampleCollidableComponent collision_body = owner_entity->GetComponent( ExampleCollidableComponent );
        
        if( collision_body == nullptr ) return;
        
        PerformPhysics( collision_body );
        Transform new_transform = UpdateObject( owner_entity->GetTransform(), velocity, acceleration, mass );
        owner_entity->SetTransform( new_transform );
    }
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
*/

/*    Behavior location
Can be a component:
    Audio emitter
    Particle emitter
    Particle renderer
    Mesh renderer
    Collidable
    Rigid body
    Gameplay trigger
    Controller
        AI controller
        User input (for controlling entities [aka a character])
    Debug visualizations
    Inventory system
    Camera
 
Not component based:
    Renderer
    Tasking system
    Networking
    File IO
    Input system
    Terrain
    UI system
    Picking
*/

/*    Subsystems
*/
class GraphicsDevice
{
    void*   system_gfx_device; // ID3D11Device, CGLContextObj, etc
    
    void SetVertexBuffer( VertexBuffer buffer ); 
    ...
}

class Renderer
{
    GraphicsDevice  graphics_device;
    RenderCommand   render_commands;
    
    void AddRenderCommand( RenderCommand command );
    void SortCommands( void );
    void Render( void );
}