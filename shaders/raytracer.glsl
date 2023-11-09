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
uniform int u_RayMaxBounce;
uniform int u_SamplePerPixel;


// global variables
uint seed;


// get random integer
uint randi()
{
    uint state = seed * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u)+ 4u) ^ state)* 277803737u);
    seed += word * 155238;
    return (word >> 22u) ^ word;
};

// get random float between 0 and 1
float randf()
{
    return randi() / 4294967296.0;
}

float randRange(float min, float max)
{
    return (max - min) * randf() + min;
}

// get random in uni sphere
vec3 randInUniSphere()
{
    vec3 randomVector = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < UNI_SPHERE_MAX_ITER; i++)
    {
        randomVector = vec3(randRange(-1.0, 1.0), randRange(-1.0, 1.0), randRange(-1.0, 1.0));
        if ( length(randomVector) <= 1.0)
            break;

    }
    return normalize(randomVector);
}

// sigmoid function
float sigmoid(float t, float offset, float steepness)
{
    return 1.0 / (exp(- steepness * (t - offset)) + 1.0);
}

// sigmoid derivative
float sigmoidDeriv(float t, float offset, float steepness)
{
    return sigmoid(t, offset, steepness) * (1.0 - sigmoid(t, offset, steepness));
}

// sky color
vec3 getSkyBoxColor(in Ray ray)
{
    // get base colors
    vec3 skyColor = vec3(0.5, 0.8, 1.0);
    vec3 horizonColor = vec3(1.0, 1.0, 1.0);
    vec3 groundColor =  vec3(0.2, 0.2, 0.4);

    // get sky position from ray
    float skyPosition = ray.direction.y;

    // get contribution of each sky color
    float skyContribution = sigmoid(skyPosition, 0.001, 500.0);
    float groundContribution = sigmoid(skyPosition, -0.001, -500.0);
    float horizonContribution = sigmoidDeriv(skyPosition, 0.0, 150.0) * 2.0;

    // return sky box color
    return skyContribution * skyColor + horizonContribution * horizonColor + groundContribution * groundColor;
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
    // accumulated light color
    vec3 lightColor = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < u_RayMaxBounce; i++)
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
            lightColor += ray.color * hitInfo.material.color;
            break;
        }

        // bounce ray
        ray.origin = hitInfo.position;

        // get specular direction
        vec3 specularDir = reflect(ray.direction, hitInfo.normal);

        // get random direction
        vec3 diffuseDir = normalize(hitInfo.normal + randInUniSphere());
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
    return lightColor + ray.color * getSkyBoxColor(ray);
};

// output image
layout(rgba32f, binding = 0) uniform image2D imgOutput;

// raytracer main
void main() {
    // init seed
    seed = u_FrameCounter * 786231 + gl_GlobalInvocationID.x * gl_GlobalInvocationID.y * 8465132;

    // get image size
    float width = gl_NumWorkGroups.x * gl_WorkGroupSize.x;
    float height = gl_NumWorkGroups.y * gl_WorkGroupSize.y;

    // get texel position
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    // init pixel color
    vec4 outColor = imageLoad(imgOutput, texelCoord);
    vec4 tracedColor = vec4(0.0);

     // trace ray multiple times
    for (int i = 0; i < u_SamplePerPixel; i++)
    {
        // get ray
        Ray ray;
        ray.origin = vec3(0.0, 0.0, -2.0); // need to be uniform later

        vec3 offset = vec3(randf(), randf(), 0.0) / vec3(width, height, 1.0);
        vec3 pixelScreenPosition = (vec3( texelCoord.x / width - 0.5, texelCoord.y / height - 0.5, 0.0)
                                    + offset) * vec3(1.0, height / width, 0.0);

        ray.direction = normalize(pixelScreenPosition - ray.origin);
        ray.color = vec3(1.0, 1.0, 1.0);

        tracedColor += vec4(trace(ray), 1.0);
    }
    tracedColor /= u_SamplePerPixel;

    // average color over time
    float weight = 1.0 / u_FrameCounter;
    outColor = outColor * (1.0 - weight) + tracedColor * weight;

    // store pixel color
    imageStore(imgOutput, texelCoord, outColor);
}
