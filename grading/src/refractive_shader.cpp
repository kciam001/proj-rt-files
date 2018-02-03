#include "refractive_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Refractive_Shader::
Shade_Surface(const Ray& ray, const vec3& intersection_point,
        const vec3& same_side_normal, int recursion_depth,bool is_exiting) const
{
    //Hints: Use REFRACTIVE_INDICES::AIR for air refractive_index
    //       Use is_exiting to decide the refractive indices on the ray and transmission sides
    vec3 reflection_color;
    vec3 refraction_color;
    double ni;
    double nr;
    //double theta_i;
    double cos_theta_r;
    double cos_theta_i;

    double reflectance_ratio=-1;
    if(!world.disable_fresnel_refraction)
    {
        //TODO (Test 27+): Compute the refraction_color:
        if(is_exiting)
        {
            nr = REFRACTIVE_INDICES::AIR;
            ni = refractive_index;
        }
        else
        {
            nr = refractive_index;
            ni = REFRACTIVE_INDICES::AIR;
        }
        cos_theta_i = dot(ray.direction.normalized(), same_side_normal.normalized()); //view ray?

        double tirCheck = 1 - pow(ni/nr, 2) * (1 - pow(cos_theta_i, 2));

        
        cos_theta_r = sqrt(tirCheck);
        // - Check if it is total internal reflection. 

        //      If so update the reflectance_ratio for total internal refraction
        //
        //      else, follow the instructions below
        //
        //        (Test 28+): Update the reflectance_ratio 
        //
        //        (Test 27+): Cast the refraction ray and compute the refraction_color
        //

        if(tirCheck < 0)
        {
            reflectance_ratio = 1;
            refraction_color[0] = 0;
            refraction_color[1] = 0;
            refraction_color[2] = 0;
        }
        else
        {
            vec3 refractive_dir = (ni / nr) * (ray.direction.normalized() - cos_theta_i * same_side_normal.normalized())
             - cos_theta_r * same_side_normal.normalized();
            Ray refractive_ray(intersection_point, refractive_dir.normalized());
            recursion_depth += 1;
            refraction_color = world.Cast_Ray(refractive_ray, recursion_depth);

            double r_paralell = pow((nr * cos_theta_i - ni * cos_theta_r) / (nr * cos_theta_i + ni * cos_theta_r), 2);
            double r_intersect = pow((ni * cos_theta_i - nr * cos_theta_r) / (ni * cos_theta_i + nr * cos_theta_r), 2);
            reflectance_ratio = (r_paralell + r_intersect) / 2;
        }


    }

    if(!world.disable_fresnel_reflection)
    {
        //TODO:(Test 26+): Compute reflection_color:
        // - Cast Reflection Ray andd get color
        vec3 dir = -ray.direction;

        vec3 reflected_dir = (2 * dot(dir.normalized(), same_side_normal.normalized()) * same_side_normal.normalized() - dir.normalized());
        Ray reflected_ray(intersection_point, reflected_dir.normalized());

        recursion_depth += 1;
        reflection_color = world.Cast_Ray(reflected_ray, recursion_depth);       

    }

    Enforce_Refractance_Ratio(reflectance_ratio);
    vec3 color;
    // TODO: (Test 26+) Compute final 'color' by blending reflection_color and refraction_color using 
    //                  reflectance_ratio

    color = reflectance_ratio * reflection_color + (1 - reflectance_ratio) * refraction_color;
    return color;
}

void Refractive_Shader::
Enforce_Refractance_Ratio(double& reflectance_ratio) const
{
    if(world.disable_fresnel_reflection) reflectance_ratio=0;
    else if(world.disable_fresnel_refraction) reflectance_ratio=1;
}

