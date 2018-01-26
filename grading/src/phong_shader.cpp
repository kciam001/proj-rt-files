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
    vec3 light_position;

    //------------------------ambient----------------------------------------------------
    ambient = world.ambient_color * world.ambient_intensity * color_ambient;
	color += ambient;
    //------------------------shadows----------------------------------------------------

 //    if(world.enable_shadows)
	// {
	// 	//std::cout << world.lights.size();

	// 	for(unsigned i = 0; i < world.lights.size(); i++)
	// 	{
	// 		light_position = world.lights[i]->position;
	// 		Ray lightRay(intersection_point, light_position-intersection_point);
	// 		Hit hit;

	// 		if(world.Closest_Intersection(lightRay, hit) != NULL)
	// 		{
	// 			//std::cout << "TEST C";
	// 			numObstructions++;
	// 		}
	// 		else
	// 		{
	// 			//std::cout << "TEST A ";
	// 		}
	// 	}

	// 	if(numObstructions == world.lights.size())
	// 	{
	// 		//std::cout << "TEST B";
	// 		return ambient;
	// 	}

	// }

	//--------------------------------------------------------------------------

    for(unsigned i = 0; i < world.lights.size(); i++)
    {
	    vec3 light_color = world.lights[i]->Emitted_Light(ray);
	    light_position = world.lights[i]->position;
	    bool shadowFlag = false;

	    if(world.enable_shadows)
	    {
	    	Ray lightRay(intersection_point, light_position-intersection_point);
	    	Hit hit;

	    	if(world.Closest_Intersection(lightRay, hit) != NULL)
	    	{
	    		shadowFlag = true;
	    	}
	    }

	    if(!shadowFlag)
	    {

		    //-------------------------diffuse--------------------------------------
		    vec3 diffuse;
		    double diffuseI;
		    

		    //vector from intersection point to the light source
		    vec3 L = (light_position - intersection_point);
		    //dot(n,l) = costheta
			diffuseI = std::max(dot(L.normalized(), same_side_normal.normalized()), 0.0);
			//decay proportional to square distance between intersection point and light source

			//Ray lightRay(world.lights[0]->position, L);

		    diffuse = diffuseI * color_diffuse * light_color;

		    diffuse = diffuse / pow(L.magnitude(), 2);

		    //------------------------specular--------------------------------------

		    vec3 specular;

		    vec3 reflected = (2 * dot(L.normalized(), same_side_normal.normalized()) * same_side_normal.normalized() - L.normalized());
		    double specularI = pow(fmax(dot(reflected.normalized(), (ray.endpoint - intersection_point).normalized()), 0), specular_power);

		    

		    specular = specularI * color_specular * light_color;
		    specular = specular / pow(L.magnitude(), 2);


		    // TODO: determine the color

		    color += diffuse + specular;
		}

	}

    return color;
}
