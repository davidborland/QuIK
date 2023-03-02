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
	
	// Set the color
	gl_FragColor = gl_Color;
	
	// Set the alpha channel to render an anti-aliased circle
	gl_FragColor.a = smoothstep(-halfSmoothWidth, halfSmoothWidth, r + halfLineWidth - d);
}