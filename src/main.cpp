
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
        RenderCommand command( owner_entity->GetTransform(), mesh_to_render, other_render_properties );
        Renderer->AddRenderCommand( command );
    }
}

class ExampleCollidableComponent
{
    Shape   collision_shape;
    
    void OnEvent( Event event );
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
    Light
    Animation
    Apply fullscreen effect (when you enter a volume in the world, maybe the sky goes red or something)
 
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

/*    Engine
*/

class Engine
{
    Renderer*       renderer;
    Audio*          audio;
    TaskManager*    task_manager;
    UISystem*       ui_system;
    EntityManager*  entity_manager;
    Network*        network;
    
    void EngineInit(void);
    void EngineFrame(void)
    {        
        renderer.SortCommands();
        renderer.Render();
    }
    void EngineShutdown(void);
    void EngineMouseMoveInput( int mouse_delta_x, int mouse_delta_y );
    void EngineMouseButtonInput( int mouse_button_state[8] );
    void EngineKeyboardInput( int key_state[256] );
}

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
    void LoadMesh( char mesh_name[] );
    void LoadTexture( char texture_name[] );
    
    // The renderer should have a system where you can overwrite the default
    // rendering and substitute your own callback or something. That way the
    // terrain can render itself however it wants
}

class Audio
{
    void*   audio_device; // OpenAL, FMod, etc
    
    void LoadSound( char sound_name[] );
    void LoadTrackForStreaming( char track_name[] );
    void PlaySound( int sound_id );
    void PlayTrack( int sound_id );
    void StopSound( int sound_id );
    void StopTrack( int sound_id );
}

class EntityManager
{
    Entity  entities;
    
    Entity* GetNewEntity( void );
    void DeleteEntity( Entity* entity );
    void UpdateEntities( void )
    {        
        foreach( Entity e in entities )
        {
            foreach( Component c in e )
            {
                c.Update();
            }
        }
        
        if( is_server )
            network->SendData( updated_data );
        else
            network->ReceiveData( updated_data );
    }
}

void UISystem
{
    void CreateButton( Position position, VoidFunc func, char label[] );
    void CreateToggle( Position position, bool* variable, char label[] );
    void CreateTextField( Position position, void* variable, Type variable_type, char label[] );
    void CreateMenu( Position position, char label[] );
    void CreateMenuItem( Menu parent_menu, VoidFunc func, char label[] );
    void CreateLabel( Position position, char label[] );
    void CreateValueLabel( Position, void* variable, Type variable_type, char label[] );
    void CreateGroup( void ); // Used to group things together, so a checkbox can be inserted
                              // with a button in a group, and it can move together
}

void Network
{
    Socket  tcp_socket;
    void SendData( NetworkAddress address, void* data );
    void ReceiveData( void* data );
}

void File
{
    char    filename[];
    FILE*   file;
    
    void LoadFileSync( char filename[] );
    void LoadFileAsync( char filename[] );
    void SaveFileSync( char filename[] );
    void SaveFileAsync( char filename[] );
}

void Scene
{
    Terrain*    terrain;
    Entity*     entities[];
    
    void LoadScene( char scene_file[] );
    void SaveScene( char scene_file[] );
    void ClearScene( void );
    void NewScene( void );
}

void Terrain
{
    int             seed;
    VertexBuffer    vertex_buffers[];
    IntexBuffer     index_buffers[];
    
    void UpdateTerrain( Position position );
    void Render( void );
}

void Input
{
    int mouse_button_state[8];
    int mouse_position[2];
    Key key_state[256];
    
    int GetKeyState( int key );
    int GetMouseState( int button );
    int GetMousePosition( void );
    int SetKeyState( int key_state[] );
    int SetMouseState( int button[8] );
}