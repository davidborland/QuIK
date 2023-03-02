uniform vec2 c1, c2;
uniform float rMax1, rMax2, rMin1, rMin2;
uniform float pixelSize;
uniform float lineWidth;
uniform float smoothWidth;
uniform float intersectionAlpha;
uniform float circleAlpha;
uniform float minCircleDashSize;

varying vec4 worldPos;

void main() {
	vec2 p = worldPos.xy;

	vec2 v1 = c1 - p;
	vec2 v2 = c2 - p;
	
	float d1 = sqrt(v1.x * v1.x + v1.y * v1.y);
	float d2 = sqrt(v2.x * v2.x + v2.y * v2.y);
	
	float halfSmoothWidth = pixelSize * smoothWidth * 0.5;
	float halfLineWidth = lineWidth * pixelSize * 0.5;
	
	bool in1 = d1 <= rMax1 + halfLineWidth + halfSmoothWidth &&
	           d1 >= rMin1 - halfLineWidth - halfSmoothWidth;
	bool in2 = d2 <= rMax2 + halfLineWidth + halfSmoothWidth &&
			   d2 >= rMin2 - halfLineWidth - halfSmoothWidth;
	
	// Discard if we know nothing will be drawn
	if (!in1 && !in2) {
		discard;
	}
	
	// Initialize the fragment color
	gl_FragColor = gl_Color;
	gl_FragColor.a = 0.0;
	
	// Compute the anti-aliased alpha value for the outer and inner max circles, based on the line width
	float outerCircleMax1 = smoothstep(-halfSmoothWidth, halfSmoothWidth, rMax1 + halfLineWidth - d1);
	float innerCircleMax1 = smoothstep(-halfSmoothWidth, halfSmoothWidth, rMax1 - halfLineWidth - d1);
	float outerCircleMax2 = smoothstep(-halfSmoothWidth, halfSmoothWidth, rMax2 + halfLineWidth - d2);
	float innerCircleMax2 = smoothstep(-halfSmoothWidth, halfSmoothWidth, rMax2 - halfLineWidth - d2);
	
	// Compute the anti-aliased alpha value for the outer and inner min circles, based on the line width
	float outerCircleMin1 = smoothstep(-halfSmoothWidth, halfSmoothWidth, rMin1 + halfLineWidth - d1);
	float innerCircleMin1 = smoothstep(-halfSmoothWidth, halfSmoothWidth, rMin1 - halfLineWidth - d1);
	float outerCircleMin2 = smoothstep(-halfSmoothWidth, halfSmoothWidth, rMin2 + halfLineWidth - d2);
	float innerCircleMin2 = smoothstep(-halfSmoothWidth, halfSmoothWidth, rMin2 - halfLineWidth - d2);
						 
	// Compute the regions to be drawn
	float intersection = outerCircleMax1 * outerCircleMax2 * 
						 (1.0 - innerCircleMin1) * (1.0 - innerCircleMin2);
//float intersection = (outerCircleMax1 + outerCircleMax2);		// Only used for paper figure
	float circleMax1 = outerCircleMax1 - innerCircleMax1;			 
	float circleMax2 = outerCircleMax2 - innerCircleMax2;
	float circleMin1 = (outerCircleMin1 - innerCircleMin1) * cos(acos(dot(normalize(v1), vec2(1, 0))) * rMin1 / pixelSize * minCircleDashSize);			 
	float circleMin2 = (outerCircleMin2 - innerCircleMin2) * cos(acos(dot(normalize(v2), vec2(1, 0))) * rMin2 / pixelSize * minCircleDashSize);	
					   
	// Set the final fragment alpha
//float fadeAlpha = d1 / rMax1 * d2 / rMax2;
//float fadeAlpha = d2 / max(rMax1, rMax2) * d1 / max(rMax1, rMax2);
//float fadeAlpha = min(d1 / rMax1, d2 / rMax2);
//float fadeAlpha = d1 / rMax2 * d2 / rMax1;
float fadeAlpha = 1.0;

	
	gl_FragColor.a = intersection * intersectionAlpha * fadeAlpha +
	                 max(max(circleMax1, circleMax2), max(circleMin1, circleMin2)) * circleAlpha;
}