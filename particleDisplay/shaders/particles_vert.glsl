#version 400

layout (location = 0) in vec3 particlePos;       // particle position
layout (location = 1) in float particleVelLen;    // particle vel len

out float colorFactor;    
out vec2 TexCoord;   // Texture coordinate


uniform float ParticleSize = 1.0;            // Particle size

// Transformation matrices
uniform mat4 MV;
uniform mat4 Proj;

// Offsets to the position in camera coordinates for each vertex of the particle's quad
const vec3 offsets[] = vec3[](vec3(-0.5,-0.5,0), vec3(0.5,-0.5,0), vec3(0.5,0.5,0),
                              vec3(-0.5,-0.5,0), vec3(0.5,0.5,0), vec3(-0.5,0.5,0) );
// Texture coordinates for each vertex of the particle's quad
const vec2 texCoords[] = vec2[](vec2(0,0), vec2(1,0), vec2(1,1), vec2(0,0), vec2(1,1), vec2(0,1));

void main()
{
    vec3 cameraPos;  // Position in camera coordinates
    //
    vec3 pos = particlePos;// EmitterPos + VertexInitVel * t + Gravity * t * t;
    cameraPos = (MV * vec4(pos,1)).xyz + (offsets[gl_VertexID] * ParticleSize  );
    colorFactor = particleVelLen;

    TexCoord = texCoords[gl_VertexID];

    gl_Position = Proj * vec4(cameraPos, 1);
}