#version 100
precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 tint;

void main()
{
    // Sample the texture's alpha
    float alpha = texture2D(texture0, fragTexCoord).a;

    // Use tint color RGB, apply alpha mask
    gl_FragColor = vec4(tint.rgb, alpha * tint.a);
}