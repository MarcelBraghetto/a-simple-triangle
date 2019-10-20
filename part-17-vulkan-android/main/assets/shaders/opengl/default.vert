uniform mat4 u_mvp;

attribute vec3 a_vertexPosition;
attribute vec2 a_texCoord;

varying vec2 v_texCoord;

void main()
{
    gl_Position = u_mvp * vec4(a_vertexPosition, 1.0);
    v_texCoord = a_texCoord;
}
