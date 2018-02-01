#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const
{
	vec3 color;

    vec3 dir = -ray.direction;

    vec3 reflected_dir = (2 * dot(dir.normalized(), same_side_normal.normalized()) * same_side_normal.normalized() - dir.normalized());
    Ray reflected_ray(intersection_point, reflected_dir.normalized());
    vec3 shader_color = shader->Shade_Surface(ray, intersection_point, same_side_normal, recursion_depth, is_exiting);

    vec3 reflected_color = world.Cast_Ray(reflected_ray, ++recursion_depth);

    

    color = (reflectivity * reflected_color + (1 - reflectivity) * shader_color);


    //(2 * dot(dir.normalized(), same_side_normal.normalized()) * same_side_normal.normalized() - dir.normalized());
    // (D - 2 *(dot(D, N))*N)
	    // TODO: determine the color
	   


	return color;
}
