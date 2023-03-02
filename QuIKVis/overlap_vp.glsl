varying vec4 worldPos;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	gl_FrontColor = gl_Color;
	
	worldPos = gl_ModelViewMatrix * gl_Vertex;
}