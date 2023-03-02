uniform vec2 c;
uniform float r;
uniform float pixelSize;
uniform float lineWidth;
uniform float smoothWidth;

varying vec4 worldPos;

void main() {
	vec2 p = worldPos.xy;

	vec2 v = c - p;
	
	float d = sqrt(v.x * v.x + v.y * v.y);
	
	float halfSmoothWidth = pixelSize * smoothWidth * 0.5;
	float halfLineWidth = lineWidth * pixelSize * 0.5;
	
	// Discard if we know nothing will be drawn
	if (d > r + halfLineWidth + halfSmoothWidth) {
		discard;
	}
	
	// Compute the anti-aliased alpha value for the outer and inner circles, based on the line width
	float outerCircle = smoothstep(-halfSmoothWidth, halfSmoothWidth, r + halfLineWidth - d);
	float innerCircle = smoothstep(-halfSmoothWidth, halfSmoothWidth, r - halfLineWidth - d);
	
	// Set the color
	float color = outerCircle * innerCircle; 
	gl_FragColor.rgb = gl_Color.rgb * color; //vec3(color, color, color);
	
	// Set the alpha channel to render an anti-aliased circle
	gl_FragColor.a = outerCircle;				   
}