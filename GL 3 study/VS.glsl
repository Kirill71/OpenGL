#version 330                                    
                       
// data from openGL_VBO
// each of atributes has atribute location ( fixed as here). 
// If we use automatic atrib_location, we must request index_location from shader with "glGetAttribLocation"                       

layout (location = 0) in vec3 Position;	         
layout (location = 1) in vec2 TexCoord;
                                                
uniform mat4 gWVP;                               

out vec2 TexCoord0;
                               
void main(void)
{
	//vec3 newPos = Position;
	//newPos.y = newPos.y + sin ( sqrt( (newPos.x * newPos.x + newPos.z * newPos.z) ) * 50  );

	gl_Position = gWVP * vec4(Position, 1.0);
   
	TexCoord0 = TexCoord;
}
