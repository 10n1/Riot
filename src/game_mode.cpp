/*
Pseudo-call stacks for events that happen in game mode
*/

// Startup
main
    InitOSStuff
        CreateWindow
    Engine::EngineInit
        Engine::InitRenderer
        Engine::InitAudio
        Engine::InitEntityManager
        Engine::InitUI
        Engine::InitInput
        Scene::LoadOrCreateScene
            Terrain::LoadOrCreateTerrain
            Scene::LoadEntities
                EntityManager::GetNewEntity
                Entity::AttachComponents
    OSLoop
        Engine::EngineFrame
            EntityManager::UpdateEntities // Should this be Scene::UpdateEntities?
                Entity::UpdateEachComponents
            Engine::ProcessEvents
            Renderer::SortCommands
            Renderer::Render
        ProcessOSMessages
            Engine::PassInputToEngine

// Create New Object
ProcessOSMessages
    Engine::PassInputToEngine
        Engine::QueueEvent( CREATE_NEW_OBJECT )
Engine::EngineFrame
    Engine::ProcessEvents
        Scene::CreateNewObjectOfType( OBJECT_TYPE )
            EntityManager::GetNewEntity
            Entity::AddComponent( TYPE_n )
        Scene::OnEvent( CREATE_NEW_OBJECT )

// Create new mesh render component
Entity::AddComponent( MESH_RENDER_COMPONENT )
    Renderer::LoadMesh
        IfMeshAlreadyExistsReturnItsIDOrLoad

// Mesh render processing
MeshRenderComponent::Process
    EncodeCommand
    Renderer::AddCommand( NEW_COMMAND )

// Render sorting
Renderer::SortCommands
    SortByTypeLightOrMesh
    SortByTransparency
    SortByMaterial
    SortByMesh
    AddInstancesOfEachMesh
    SortSingleMeshesByDepth

// Rendering
Renderer::Render
    RenderOpaqueObjectsDeferred
        LoopThroughCommandsAndRenderMeshes
        RenderGBufferWithLights
    RenderTransparentObjectsWithForwardRenderer

// Player starts to dig in the terrain

ProcessOSMessages
    Engine::PassInputToEngine
        Engine::QueueEvent( PLAYER_DIG_TERRAIN )
Engine::EngineFrame
    ProcessEvents
        Terrain::OnEvent( PLAYER_DIG_TERRAIN )
            