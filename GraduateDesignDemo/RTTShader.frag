#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
	vec4 vec = texture(screenTexture, TexCoords);
	if(TexCoords.x < 0.01 || TexCoords.x>0.99 ||TexCoords.y < 0.01 || TexCoords.y>0.99 ){
		FragColor = vec4(1.0f,1.0f,0.0f,1.0f);
	}else{
		FragColor = vec4(vec.rgb,1.0f);
	}

    //FragColor = vec4(1.0f,1.0f,0,1.0f);
}