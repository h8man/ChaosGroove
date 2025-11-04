#version 330
in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform vec4 colDiffuse; // from DrawTexture*() tint
out vec4 finalColor;

void main()
{
    float a = texture(texture0, fragTexCoord).a;
    finalColor = vec4(colDiffuse.rgb, a * colDiffuse.a);
}