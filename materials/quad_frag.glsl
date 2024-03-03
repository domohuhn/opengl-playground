#version 400

layout(pixel_center_integer) in vec4 gl_FragCoord;
in vec3 color;

uniform uint uHeight;
uniform uint uMeta[64];
// width, height
uniform uint uBufferSize[2];

uniform float upperFadeLimitLeft;
uniform float upperFadeWidthLeft;
uniform float lowerFadeWidthLeft;

uniform float upperFadeLimitRight;
uniform float upperFadeWidthRight;
uniform float lowerFadeWidthRight;

float getFadeWidth(float lower, float upper, float y, float yLimit) {
    float pos = clamp((y - float(uHeight))/(yLimit-float(uHeight)), 0.0, 1.0);
    return lower + pos * (upper-lower);
}

float computeSingleSideFade(float x, float width) {
    return clamp(x/width,0.0,1.0);
}


float computeSideFade(float x, float y) {
    float left = getFadeWidth(lowerFadeWidthLeft ,upperFadeWidthLeft, y, upperFadeLimitLeft);
    float right = getFadeWidth(lowerFadeWidthRight ,upperFadeWidthRight, y, upperFadeLimitRight);
    float maskTopL = step(y,upperFadeLimitLeft );
    float maskTopR = step(y,upperFadeLimitRight );
    float maskSideL = step(x,uBufferSize[0]/2 );
    float maskSideR = 1.0-maskSideL;
    float maskL = maskTopL*maskSideL;
    float maskR = maskTopR*maskSideR;
    float fadeL = (1.0- maskL) + (maskL * computeSingleSideFade(x,left)) ;
    float fadeR = (1.0- maskR) + (maskR * computeSingleSideFade(uBufferSize[0] - x,right)) ;
    return fadeL * fadeR;
}

int mapToArrayIndex(int x, int y) {
    int idx = x%63;
    return idx;
}

void main()
{
    int xpos = int(gl_FragCoord.x);
    int ypos = int(gl_FragCoord.y);
    if(ypos<uHeight) {
        int idx = mapToArrayIndex(xpos,ypos);
        gl_FragColor = vec4(vec3(float(uMeta[idx]/255.0)),1.0);
    } else {
        gl_FragColor = vec4(color * computeSideFade(gl_FragCoord.x,gl_FragCoord.y), 1.0);
    }
}
