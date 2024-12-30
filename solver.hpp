#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
using namespace sf;

struct VerletObject
{
	Vector2f position;
	Vector2f position_last;
	Vector2f acceleration;
	const float radius = 20.0f;

	VerletObject() = default;

	VerletObject(Vector2f position_, float radius_)
		: position{ position_ }
		, position_last{ position_ }
		, acceleration{ 0.0f, 0.0f }
		, radius{ radius_ }
	{}
	
	void update(float dt)
	{
		const Vector2f displacement = position - position_last;
		position_last = position;
		position = position + displacement + acceleration * (dt * dt);
		acceleration = {};
	}

	void accelerate(Vector2f a)
	{
		acceleration += a;
	}

	void setVelocity(Vector2f v, float dt)
	{
		position_last = position - (v * dt);
	}

	void addVelocity(Vector2f v, float dt)
	{
		position_last -= v * dt;
	}

	[[nodiscard]]
	Vector2f getVelocity(float dt) const
	{
		return(position - position_last) / dt;
	}
};

class Solver
{
	void applyGravity()
{
    for (int i = 0; i < obj_num; i++)
    {
        verlet_obj[i].accelerate(gravity);
    }

}

void solveCollisions()
{

    for (int i = 0; i < obj_num; i++)
    {
        for (int j = i + 1; j < obj_num; j++)
        {
            const Vector2f collision_axis = verlet_obj[i].position_current - verlet_obj[j].position_current;
            const float distance2 = (collision_axis.x * collision_axis.x) + (collision_axis.y * collision_axis.y);
            const float temp_dist = sqrt(distance2);
            const float min_dist = verlet_obj[i].rad + verlet_obj[j].rad;
            //std::cout << i << " " << j << " " << temp_dist << std::endl;
            if (distance2 < min_dist * min_dist)
            {
                const float distance = sqrt(distance2);
                const float overlap = (min_dist - distance);
                const Vector2f n = collision_axis / distance;
                verlet_obj[i].position_current += .5f * overlap * n;
                verlet_obj[j].position_current -= .5f * overlap * n;
                //std::cout << overlap << std::endl;
            }
        }


    }
}

void applyConstraint()
{
    for (int i = 0; i < obj_num; i++)
    {
        Vector2f distance_vec = verlet_obj[i].position_current - middle;
        //std::cout<< distance_vec.y << std::endl;
        float distance = sqrt(pow(distance_vec.x, 2) + pow(distance_vec.y, 2)) + 2;
        //std::cout << distance << std::endl;
        if (distance >  r_c - verlet_obj[i].rad )
        {
            const Vector2f n = distance_vec / distance;
            verlet_obj[i].position_current = middle + n * (r_c - verlet_obj[i].rad);
            //std::cout << "working" << std::endl;
        }

    }
}

void updatePositions(float dt)
{
    for (int i = 0; i < obj_num; i++)
    {
        Vector2f displacement = verlet_obj[i].position_current - verlet_obj[i].position_old;
        verlet_obj[i].position_old = verlet_obj[i].position_current;
        verlet_obj[i].position_current = verlet_obj[i].position_old + displacement + verlet_obj[i].acceleration * ( dt * dt);
        verlet_obj[i].acceleration = {};

        verlet_obj[i].setPos();
        //dt += 1.0;
    }

}

void increaseTime()
{
    dt += 1.0;
}

void update()
{
    applyGravity();
    for (int i = 0; i < sub_steps; i++)
    {
        applyConstraint();
        solveCollisions();
    }
    
    updatePositions(dt);
    increaseTime();
}



};