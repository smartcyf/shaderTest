#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 center;
uniform vec2 resolution;
uniform int direction;
uniform float rate;

void main(void)
{
    //edge rate
    float _rate = rate;
    
    float deltaEdge = resolution[direction]*_rate;
    
    vec2 p = (gl_FragCoord.xy - center.xy);
    vec4 c = texture2D(CC_Texture0, v_texCoord);
    gl_FragColor.xyz = c.xyz;
    float deltaA = c.w/(deltaEdge*0.7);
    if (abs(p[direction])>(resolution[direction]-deltaEdge)) {
        for (float i=0.0; i<deltaEdge; i = i+1.0) {
            if(abs(p[direction])>(resolution[direction]-deltaEdge+i))
            {
                gl_FragColor.w = c.w-deltaA*i;
            }
        }
    }
    else if(abs(p[direction])<deltaEdge){
        for (float i=0.0; i<deltaEdge; i = i+1.0) {
            if(abs(p[direction])<deltaEdge-i)
            {
                gl_FragColor.w = c.w-deltaA*i;
            }
        }
    }
    else{
        gl_FragColor = texture2D(CC_Texture0, v_texCoord);
    }
    
}