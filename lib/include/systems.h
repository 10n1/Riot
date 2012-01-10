/*
 *  systems.h
 *  lib
 *
 *  Created by Kyle C Weicht on 1/10/12..
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#ifndef lib_systems_h
#define lib_systems_h

/*******************************************************************\
 Render engine
\*******************************************************************/
void Initialize(void* window);
void Shutdown();

int CreateVertexShader(const char* source);
int CreatePixelShader(const char* source);
int CreateMaterial(int vshader, int pshader);
int CreateMesh(mesh_data* meshData);
int CreateView(view_data* viewData);

void Draw(mesh_id mesh, material_id material, view_id view);

void Render(void);


/*******************************************************************\
 System
\*******************************************************************/
void InitializeApplication(void);
void ShutdownApplication(void);

void StartMainLoop(void);
void CreateWindow(int width, int height); // Support only one window. Makes much simpler
void* GetWindowHandle(void);

void MessageBox(void);
void LoadFileBox(void);
void SaveFileBox(void);
void DirectoryServices(void);


/*******************************************************************\
 Input (Separate or part of system? Probably part of system,
    otherwise theres a dependency)
\*******************************************************************/
void GetMouseState(void);
void GetKeyboardState(void);


/*******************************************************************\
 Low level/platform/utility/....?????
\*******************************************************************/
void UnicodeSupportShit(void);
void ThreadingSupportShit(void);
void MemorySupportShit(void);
void TimerSupportShit(void);
void HardwareInformationShit(void);


/*******************************************************************\
 Audio
\*******************************************************************/
void Initialize(void);
void Shutdown(void);

int LoadFile(const char* filename);
void SetListenerPosition(void);

void PlaySound(sound_id sound);
void PlaySound3D(sound_id sound);
void PlayMusic(sound_id music);


/*******************************************************************\
 Physics
\*******************************************************************/
void Initialize(void);
void Shutdown(void);

int CreateEntity(void);
void SetEntityShape(shape_data* shapeData);
void SetEntityCallback(void);

void Update(void);

#endif /* include guard */