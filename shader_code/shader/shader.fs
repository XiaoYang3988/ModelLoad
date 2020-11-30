
#version 330          

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main(){
	// FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	FragColor = texture(texture1, TexCoord);
	FragColor = texture(texture2, TexCoord);
	FragColor = texture(texture3, TexCoord);
}