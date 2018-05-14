#version 330 core

#include Commom.glsl


out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;





void main()
{ 
	
	//vec4 vec = texture(screenTexture, vec2(1.0f - TexCoords.x,TexCoords.y));
	vec4 vec = texture(screenTexture, vec2(TexCoords.x,TexCoords.y));
//	if(TexCoords.x < 0.01 || TexCoords.x>0.99 ||TexCoords.y < 0.01 || TexCoords.y>0.99 ){
//		FragColor = vec4(1.0f,1.0f,0.0f,1.0f);
//	}else{
		//FragColor = vec4(vec.rgb,1.0f);
		//FragColor = vec4(1.0f,1.0f,0.0f,1.0f);

		//后期处理
		
		//反向
		//FragColor = vec4(vec3(1.0f - vec.rgb),1.0f);

		//灰度
		//float avg = (vec.r+vec.g+vec.b)/3.0f;
		//FragColor = vec4(vec3(avg),1.0f);
		//加权灰度
		//avg =  0.2126 * vec.r + 0.7152 * vec.g + 0.0722 * vec.b;
		//FragColor = vec4(vec3(avg),1.0f);

		//通过卷积实现各种特殊效果
		
		float offset = 1.0 / 300.0;  
		vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
		);
//		//核效果
//		float kernel[9] = float[](
//        -1, -1, -1,
//        -1,  9, -1,
//        -1, -1, -1
//		);
//		//模糊效果
//		float kernel[9] = float[](
//		 1.0 / 16, 2.0 / 16, 1.0 / 16,
//		2.0 / 16, 4.0 / 16, 2.0 / 16,
//		1.0 / 16, 2.0 / 16, 1.0 / 16  
//		);
//		//边缘检测
		float kernel[9] = float[](
        1, 1, 1,
        1,  -8, 1,
        1, 1, 1 
		);
//
		vec3 sampleTex[9];
		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0);


	//}

   
}