#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>


using namespace sf;

const float r_c = 500.0f;
const int obj_num = 2;
float dt = 0.0f;
const Vector2f middle = { 720.0, 540.0 };
const Vector2f start = { 400, 300 };
Vector2f gravity = { 0.0f, 0.0002f };
const int sub_steps = 6;

struct verletObject
{
    CircleShape test;
    Vector2f position_current;
    Vector2f position_old;
    Vector2f acceleration = { 0.0f, 0.0f };
    float rad = 20.0f;


    float getDistance(Vector2f pos1, Vector2f pos2)
    {
        return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));
    }

    bool checkConstraintCollision(Vector2f position_current, float rad_obj)
    {
        return getDistance(middle, position_current) > r_c - rad_obj;
    }

    bool checkObjCollision(verletObject obj1, verletObject obj2)
    {
        return getDistance(obj1.position_current, obj2.position_current) < obj1.rad + obj2.rad;
    }

    //void applyConstraint()
    //{
        //Vector2f distance_vec = position_current - middle;
        //const Vector2f n = distance_vec / getDistance(middle, position_current);
        //position_current = middle + n * (r_c - rad);
    //}

    void accelerate(Vector2f acc)
    {
        acceleration += acc;
    }

    void setPos()
    {
        test.setPosition(position_current);
    }



};

verletObject verlet_obj[obj_num];


void applyGravity()
{
    for (int i = 0; i < obj_num; i++)
    {
        verlet_obj[i].accelerate(gravity);
        //std::cout << "hello" << std::endl;
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
    //for (int i = 0; i < sub_steps; i++)
    //{
        
    //}
    applyConstraint();
    solveCollisions();
    updatePositions(dt);
    //increaseTime();
}

void populate()
{
    for (int i = 0; i < obj_num; i++)
    {
        Vector2f temp = { 20.0f * float(i), 0.0f };
        verlet_obj[i].position_current = start + temp;
        verlet_obj[i].position_old = start + temp;
        verlet_obj[i].rad = 20.0;
        verlet_obj[i].test.setRadius(verlet_obj[i].rad);
        verlet_obj[i].test.setOrigin(verlet_obj[i].rad, verlet_obj[i].rad);
        verlet_obj[i].test.setFillColor(Color::Red);
    }
}



int main()
{
    RenderWindow window(VideoMode(1440, 1080), "SFML works!"); //creates a window of given resolution
    window.setFramerateLimit(60);


    CircleShape constraint(r_c);
    constraint.setOrigin(r_c, r_c);
    constraint.setPosition(middle);
    constraint.setFillColor(Color::Transparent);
    constraint.setOutlineThickness(2.0);
    constraint.setPointCount(128);
    verletObject verlet_obj[obj_num];

    //CircleShape obj[obj_num];

    populate();

    while (window.isOpen()) //creates a game loop 
    {
        Event event; //creates event object
        while (window.pollEvent(event)) //loop that checks for events
        {
            if (event.type == Event::Closed) //checks if the window is closed
                window.close();
        } //ends the event loop

        //;

        window.clear();
        update();
        for (int l = 0; l < obj_num; l++)
        {
            window.draw(verlet_obj[l].test);
            std::cout << dt << std::endl;
        }
        window.draw(constraint);
        window.display();
        dt += 1.0;
    }

    return 0;
}