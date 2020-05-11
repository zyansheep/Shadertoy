//p = Point measureing from, c = point of sphere, radius = radius of sphere

float RESOLUTION = 0.01;
float sphereSDF(vec3 p, vec3 c, float radius) {
	return length(p - c)-radius;
}

float sceneSDF(vec3 p) {
	return sphereSDF(p, vec3(0.,0.,10.), 1);
}

vec3 estimateNormal(vec3 p) {
  return normalize(vec3(
    sceneSDF(vec3(p.x + RESOLUTION, p.y, p.z)) - sceneSDF(vec3(p.x - RESOLUTION, p.y, p.z)),
    sceneSDF(vec3(p.x, p.y + RESOLUTION, p.z)) - sceneSDF(vec3(p.x, p.y - RESOLUTION, p.z)),
    sceneSDF(vec3(p.x, p.y, p.z + RESOLUTION)) - sceneSDF(vec3(p.x, p.y, p.z - RESOLUTION))
  ));
}

uniform vec2 resolution = vec2(1600.0, 1200.0);
void mainImage(out vec4 fragColor, in vec2 fragCoord) {
	vec2 uv = -0.5 + fragCoord.xy / iResolution.xy;
	uv.x *= iResolution.x / iResolution.y;

	vec3 ro = vec3(0., 0., -1); //Ray Origin (where the rays originate from "behind" the screen)
	vec3 ri = vec3(uv.x, uv.y, 0.); //Ray Intersection (3d point on screen that ray will pass through)
	vec3 rd = normalize(ri-ro); //Ray direction (of specific ray)
	
	vec3 cp = ro;
	
	float dist;
	vec3 normal;
	for(int i = 0; i < 100; i++) { //Max Marching Steps
		dist = sceneSDF(cp);
		if(dist < RESOLUTION){
			normal = estimateNormal(cp);
			break;
		}
		if(dist > 100.){ break; }
		cp += rd * dist; //March cp (current point) along rd (direction)
	}
	
	//dist = smoothstep(0.1, 0.09, dist);
	fragColor = vec4(-normal.zzz, 1.);
}