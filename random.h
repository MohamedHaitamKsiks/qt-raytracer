#ifndef RANDOM_H
#define RANDOM_H

#include<QVector3D>
#include<random>

class Random
{
public:
    // init random
    static void init(int offset = 0);


    // random number between 0 and 1
    static inline float randf()
    {
        return static_cast <float> (randi()) / static_cast <float> (UINT32_MAX);

    }

    // random number between a and b
    static inline float range(float a, float b)
    {
        return (b - a) * randf() + a;
    }

    // random int
    static inline uint32_t randi()
    {
        uint32_t state = seed * 747796405u + 2891336453u;
        uint32_t word = ((state >> ((state >> 28u)+ 4u) ^ state)* 277803737u);
        seed++;
        return (word >> 22u) ^ word;
    }

    // get random direction in unishphere
    static inline QVector3D randUniSphere()
    {

        while (true) {
            // get random vector
            QVector3D v = QVector3D(Random::range(-1.0f, 1.0f),
                                    Random::range(-1.0f, 1.0f),
                                    Random::range(-1.0f, 1.0f));
            // check if in uni sphere
            if (v.lengthSquared() < 1.0f)
                return v.normalized();
        }
    }

private:
    static uint32_t seed;

};

#endif // RANDOM_H
