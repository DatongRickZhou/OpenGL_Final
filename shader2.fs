#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

//uniform sampler2D ourTexture;
uniform sampler2D texture1; //default to texture0 bind, change which they are a bound to via code
uniform sampler2D texture2; //default to texture0 bind


void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	//FragColor = texture(texture2, TexCoord);//* vec4(ourColor, 1.0);
}