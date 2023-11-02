#version 430 core

// contst
#define UNI_SPHERE_MAX_ITER 100

// compute shader layout
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;


// data types

// material
struct Material
{
    vec3 color;
    float smoothness;
    bool emissive;
};

//draw commands
struct SphereCommand
{
    vec3 center;
    float radius;
    Material material;
};

// ray
struct Ray
{
    vec3 origin;
    vec3 direction;
    vec3 color;
};

// rayhit info
struct RayHitInfo
{
    vec3 position;
    vec3 normal;
    float depth;
    bool hit;
    Material material;
};


// sphere command buffer
layout(std430, binding = 3) readonly buffer sphereCommandsBuffer {
    SphereCommand commands[];
};


// uniforms
uniform int u_FrameCounter; // current frame number;
uniform int u_SphereCommandCount;
// uniform int u_RayMaxBounce;
// uniform int u_SamplePerPixel;



// get random integer
uint randi(inout uint seed)
{
    uint state = (seed ) * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u)+ 4u) ^ state)* 277803737u);
    seed += word * 155238;
    return (word >> 22u) ^ word;
};

// get random float between 0 and 1
float randf(inout uint seed)
{
    return randi(seed) / 4294967296.0;
}

// get random in uni sphere
vec3 randInUniSphere(inout uint seed)
{
    vec3 randomVector = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < UNI_SPHERE_MAX_ITER; i++)
    {
        randomVector = vec3(randf(seed), randf(seed), randf(seed));
        if ( length(randomVector) <= 1.0)
            break;

    }
    return normalize(randomVector);
}

// sphere on hit
RayHitInfo sphereOnHit(in SphereCommand command, in Ray ray)
{
    // check if there is collision with the sphere by checking if there is any point in the ray that intersects with the sphere
    // we do that by solving || P(t) - C ||^2 = r^2
    RayHitInfo hitInfo;

    vec3 originCenter = ray.origin - command.center;

    float b = 2.0 * dot(ray.direction, originCenter);
    float c = dot(originCenter, originCenter) - command.radius * command.radius;

    float delta = b * b - 4.0 * c;

    if (delta >= 0.0)
    {
        hitInfo.depth = (- b - sqrt(delta)) / 2.0;
        hitInfo.position = ray.origin + ray.direction * hitInfo.depth;
        hitInfo.normal = normalize(hitInfo.position - command.center);
        hitInfo.hit = hitInfo.depth > 0.001;

        if (hitInfo.hit)
        {
            hitInfo.material = command.material;
        }
    }
    else
    {
        hitInfo.hit = false;
    }
    return hitInfo;
}

// trace ray
vec3 trace(in Ray ray)
{
    // init seed for randome number generator
    uint seed = u_FrameCounter + gl_GlobalInvocationID.x * gl_GlobalInvocationID.y * 8465132;

    // sky color
    vec3 skyColor = vec3(0.5, 0.8, 1.0);

    // accumulated light color
    vec3 lightColor = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < 3; i++)
    {
        RayHitInfo hitInfo;
        hitInfo.hit = false;


        // process all draw commands
        for (int i = 0; i < u_SphereCommandCount; i++)
        {
            RayHitInfo newHitInfo = sphereOnHit(commands[i], ray);
            if (newHitInfo.hit && (!hitInfo.hit || newHitInfo.depth < hitInfo.depth))
                hitInfo = newHitInfo;
        }

        // check for collision
        if (! hitInfo.hit)
            break;

        // emissive material
        if (hitInfo.material.emissive)
        {
            lightColor += ray.color * (hitInfo.material.color * 10.0);
            break;
        }

        // bounce ray
        ray.origin = hitInfo.position;

        // get specular direction
        vec3 specularDir = reflect(ray.direction, hitInfo.normal);

        // get random direction
        vec3 diffuseDir = randInUniSphere(seed);
        // check correct hemisphere
        if (dot(hitInfo.normal, diffuseDir) < 0.0)
            diffuseDir *= -1.0;
        diffuseDir = normalize(hitInfo.normal + diffuseDir);

        // get actual bounce direction
        ray.direction = (specularDir - diffuseDir) * hitInfo.material.smoothness + diffuseDir;

        // bouce ray color
        ray.color *= hitInfo.material.color;
    }

    // return computed color
    return lightColor + ray.color * skyColor;
};

// output image
layout(rgba32f, binding = 0) uniform image2D imgOutput;

// raytracer main
void main() {

    // get image size
    float width = gl_NumWorkGroups.x * gl_WorkGroupSize.x;
    float height = gl_NumWorkGroups.y * gl_WorkGroupSize.y;

    // get texel position
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    // init pixel color
    vec4 outColor = imageLoad(imgOutput, texelCoord);

    // get ray
    Ray ray;
    ray.origin = vec3(0.0, 0.0, -2.0); // need to be uniform later
    vec3 pixelScreenPosition = vec3( texelCoord.x / width - 0.5, texelCoord.y / height - 0.5, 0.0)
                            * vec3(1.0, height / width, 0.0);
    ray.direction = normalize(pixelScreenPosition - ray.origin);
    ray.color = vec3(1.0, 1.0, 1.0);

    // trace ray

    vec4 tracedColor = vec4(0.0);
    for (int i = 0; i < 10; i++)
        tracedColor += vec4(trace(ray), 1.0);
    tracedColor /= 10.0;

    // average color over time
    outColor = ( outColor * (u_FrameCounter - 1) + tracedColor ) / u_FrameCounter;

    // store pixel color
    imageStore(imgOutput, texelCoord, outColor);
}
