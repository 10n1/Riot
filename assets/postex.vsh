#ifdef OPENGL
    
    #define vScreenPosition gl_Position

    #define mul(l, r) (l * r)

    #define float2 vec2
    #define float3 vec3
    #define float4 vec4
    #define matrix mat4

    #define IN_SLOT_POSITION    0
    #define IN_SLOT_NORMAL      1
    #define IN_SLOT_TEXCOORD0   3
    #define OUT_SLOT_TEXCOORD0  0
    
    #define INPUT_PARAM(type, name, slot) layout(location=slot) in type name;
    #define OUTPUT_PARAM(type, name, slot) out type name;

    #define CONSTANT_BUFFER(name) layout(std140) uniform name 

    #define BEGIN_SHADER
    #define END_SHADER \
        void main()
#else
    #define IN_SLOT_POSITION    POSITION
    #define IN_SLOT_NORMAL      NORMAL
    #define IN_SLOT_TEXCOORD0   TEXCOORD

    #define OUT_SLOT_TEXCOORD0 TEXCOORD0

    #define INPUT_PARAM(type, name, slot) in type name : slot,
    #define OUTPUT_PARAM(type, name, slot) out type name : slot,

    #define CONSTANT_BUFFER(name) cbuffer name

    #define BEGIN_SHADER void main(
    #define END_SHADER out float4 vScreenPosition : SV_POSITION)
#endif

CONSTANT_BUFFER(cbuffer0)
{
    matrix  kViewProj;
};

CONSTANT_BUFFER(cbuffer1)
{
    matrix kWorld;
};

BEGIN_SHADER
    INPUT_PARAM(float4, vInPosition, IN_SLOT_POSITION)
    INPUT_PARAM(float2, vInTexCoord, IN_SLOT_TEXCOORD0)
    OUTPUT_PARAM(float2, vInterpTexCoord, OUT_SLOT_TEXCOORD0)
END_SHADER
{
    float4 worldPos = mul(kWorld, vInPosition);
    vScreenPosition = mul(kViewProj, worldPos);
    vInterpTexCoord = vInTexCoord;
}
