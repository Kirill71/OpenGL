layout (location = 0) in vec3 Position;         
layout (location = 1) in vec2 TexCoord;
                                                
uniform mat4 gWVP;                               

out vec2 TexCoord0;
                                                
void main(void)
{
   gl_Position = gWVP * vec4(Position, 1.0);
   TexCoord0 = TexCoord;
}










in vec2 TexCoord0;                                  
                                                    
uniform sampler2D gSampler;                         

void main()                                         
{                                                   
    FragColor = texture2D(gSampler, TexCoord0.xy);
}