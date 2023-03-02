uniform vec2 p1;
uniform vec2 p2;
uniform float pixelSize;
uniform float lineWidth;
uniform float smoothWidth;

varying vec4 worldPos;

void main() {
	vec2 p = worldPos.xy;
	
	float d = abs((p.x - p1.x) * (p2.y - p1.y) - (p.y - p1.y) * (p2.x - p1.x)) /
			  sqrt(pow(p2.x - p1.x, 2.0) + pow(p2.y - p1.y, 2.0)); 			  
	
	float halfSmoothWidth = pixelSize * smoothWidth * 0.5;
	float halfLineWidth = lineWidth * pixelSize * 0.5;
	
	// Discard if we know nothing will be drawn
	if (d > halfLineWidth + halfSmoothWidth) {
		discard;
	}
	
	// Compute the anti-aliased alpha value for the line, based on the line width
	gl_FragColor.a = smoothstep(-halfSmoothWidth, halfSmoothWidth, halfLineWidth - d);	
}