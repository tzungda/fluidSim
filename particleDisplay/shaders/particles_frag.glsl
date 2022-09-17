#version 400

in float colorFactor;
in vec2 TexCoord;
uniform sampler2D ParticleTex;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, TexCoord);
	FragColor.r *= ( 0.6 * colorFactor );
	FragColor.g *= ( 0.5 * colorFactor );
	FragColor.b *= colorFactor;
    FragColor.a *= 1.0;
}
