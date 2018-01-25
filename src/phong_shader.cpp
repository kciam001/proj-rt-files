#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const 
{
    vec3 color;
    vec3 ambient;


    //diffuse
    vec3 diffuse;
    double diffuseI;

    //vector from intersection point to the light source
    vec3 L = (world.lights[0]->position - intersection_point);
    //dot(n,l) = costheta

	diffuseI = fmax(dot(L.normalized(), same_side_normal.normalized()), 0);
	//decay proportional to square distance between intersection point and light source
	diffuseI = diffuseI / L.magnitude_squared();
  

    diffuse = diffuseI * color_diffuse * world.lights[0]->Emitted_Light(ray);
    
    //specular
    vec3 specular;



    // TODO: determine the color

    color = diffuse;
    return color;
}
