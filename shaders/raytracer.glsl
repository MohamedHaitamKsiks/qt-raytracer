#version 430 core

// contst
#define UNI_SPHERE_MAX_ITER 100

// compute shader layout
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;


// data types

// vertex 3d
struct Vertex
{
    vec3 position;
    vec3 normal;
};

// mesh 3d info
struct MeshInfo
{
    int startIndex;
    int vertexCount;
};

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

// mesh instance command
struct MeshInstanceCommand
{
    int meshIndex;
    float transform[16];
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
    SphereCommand shpereCommands[];
};

layout(std430, binding = 4) readonly buffer meshInstanceCommandsBuffer {
    MeshInstanceCommand meshInstanceCommands[];
};

layout(std430, binding = 5) readonly buffer meshInfosBuffer {
    MeshInfo meshInfos[];
};

layout(std430, binding = 6) readonly buffer vertexBuffer {
    Vertex vertices[];
};

layout(std430, binding = 7) readonly buffer indexBuffer {
    int indices[];
};


// uniforms
uniform int u_FrameCounter; // current frame number;
// command counts
uniform int u_SphereCommandCount;
uniform int u_MeshInstanceCommandCount;
// settings
uniform int u_RayMaxBounce;
uniform int u_SamplePerPixel;
// skybox colors
uniform vec3 u_SkyColor;
uniform vec3 u_HorizonColor;
uniform vec3 u_GroundColor;


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
    // get sky position from ray
    float skyPosition = ray.direction.y;

    // get contribution of each sky color
    float skyContribution = sigmoid(skyPosition, 0.001, 500.0);
    float groundContribution = sigmoid(skyPosition, -0.001, -500.0);
    float horizonContribution = sigmoidDeriv(skyPosition, 0.0, 150.0) * 2.0;

    // return sky box color
    return skyContribution * u_SkyColor + horizonContribution * u_HorizonColor + groundContribution * u_GroundColor;
}

// copy rayhit info if new hit point is closer
void copyRayHitInfo(in RayHitInfo from, inout RayHitInfo to)
{
    if (from.hit && (!to.hit || from.depth < to.depth))
        to = from;
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

// transform vertex
void transformVertex(inout Vertex vertex, mat4 transform, mat3 normalTransform)
{
    vertex.position = vec3(transform * vec4(vertex.position, 1.0));
    vertex.normal = normalTransform * vertex.normal;
}

// triangle abc on hit
RayHitInfo triangleOnHit(in Vertex a, in Vertex b, in Vertex c, in Ray ray)
{
    RayHitInfo hitInfo;
    hitInfo.hit = false;

    // precision
    const float PRECISION = 0.0001;

    // check ray collide with surface defined by trinagle
    vec3 surfaceNormal = cross(b.position - a.position, c.position - a.position);
    if (abs(dot(surfaceNormal, ray.direction)) < PRECISION)
        return hitInfo;


    // we can express a point in ABC by the barycentric coordinates P = w * a + u * b + v * c where w + u + v = 1
    // then w = 1 - u - v
    // then P = a + u * (b - a) + v * (c - a)
    // for P the intersection between the ray and the triangle
    // we have O + D * t = a + u * (b - a) + v * (c - a)
    // then D * t + (a - b) * u + (a - c) * v = a - O
    // or D * t + AB * u + AC * v = AO
    vec3 D = ray.direction;
    vec3 AB = a.position - b.position;
    vec3 AC = a.position - c.position;
    vec3 AO = a.position - ray.origin;

    // let's solve the equation
    float t = dot(AO, surfaceNormal) / dot(D, surfaceNormal);
    if ( t < PRECISION)
        return hitInfo;

    vec3 H = cross(AC, D);
    float u = dot(AO, H) / dot(AB, H);

    float v = dot(AO - D * t - AB * u, AC) / dot(AC, AC);

    // check if point is inside triangle
    if (u + v > 1.0 || u < 0.0 || v < 0.0 || u > 1.0 || v > 1.0)
        return hitInfo;

    // get hit info
    hitInfo.hit = true;
    hitInfo.depth = t;
    hitInfo.normal = normalize(-surfaceNormal);//normalize(a.normal * (1 - u - v) + b.normal * u + c.normal * v);
    hitInfo.position = ray.origin + ray.direction * t;

    return hitInfo;
}

// mesh instance on hit
RayHitInfo meshInstanceOnHit(in MeshInstanceCommand command, in Ray ray)
{
    RayHitInfo hitInfo;
    hitInfo.hit = false;

    // cache normal transform
    mat4 transform = mat4(1.0);
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
            transform[i][j] = command.transform[j * 4 + i];
    }

    mat3 normalTransform = mat3(transpose(inverse(transform)));

    // get mesh
    MeshInfo info = meshInfos[command.meshIndex];

    // hit all triangles in mesh
    for (int i = info.startIndex; i < info.vertexCount; i += 3)
    {
        // get indices
        int indexA = indices[i];
        int indexB = indices[i + 1];
        int indexC = indices[i + 2];

        // get triangle
        Vertex vertexA = vertices[indexA];
        Vertex vertexB = vertices[indexB];
        Vertex vertexC = vertices[indexC];

        // apply transform
        transformVertex(vertexA, transform, normalTransform);
        transformVertex(vertexB, transform, normalTransform);
        transformVertex(vertexC, transform, normalTransform);

        // test hit on triangle
        copyRayHitInfo(triangleOnHit(vertexA, vertexB, vertexC, ray), hitInfo);

    }

    if(hitInfo.hit)
    {
        hitInfo.material = command.material;
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
        // sphere commands
        for (int i = 0; i < u_SphereCommandCount; i++)
        {
            copyRayHitInfo(sphereOnHit(shpereCommands[i], ray), hitInfo);
        }

        // mesh command
        for (int i = 0; i < u_MeshInstanceCommandCount; i++)
        {
            copyRayHitInfo(meshInstanceOnHit(meshInstanceCommands[i], ray), hitInfo);
        }


        // check for collision
        if (! hitInfo.hit)
        {
            lightColor += ray.color * getSkyBoxColor(ray);
            break;
        }


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
    return lightColor;
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

    if (u_FrameCounter <= 1)
        outColor = tracedColor;
    else
        outColor = outColor * (1.0 - weight) + tracedColor * weight;

    // store pixel color
    imageStore(imgOutput, texelCoord, outColor);
}
