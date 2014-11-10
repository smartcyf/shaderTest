// http://www.cocos2d-iphone.org

attribute vec4 a_position;
attribute vec2 a_texCoord;

#ifdef GL_ES
varying mediump vec2 v_texCoord;
attribute mediump vec4 a_color;
varying mediump vec4 v_color;
varying lowp vec4 v_fragmentColor;
#else
varying vec2 v_texCoord;
attribute vec4 a_color;
varying vec4 v_color;
varying vec4 v_fragmentColor;
#endif

void main()
{
//    v_color = vec4(a_color.rgb * a_color.a, a_color.a*0.5);
//    gl_Position = CC_MVPMatrix * a_position;
//    gl_Position = (CC_PMatrix * CC_MVMatrix) * a_position;
//	v_texCoord = a_texCoord;
    
    gl_Position = CC_PMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
}

