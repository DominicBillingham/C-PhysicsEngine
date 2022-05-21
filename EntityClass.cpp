
#include <SFML\Graphics.hpp>
using namespace std;
using namespace sf;

class Entity : public RectangleShape {

public:

    Entity(Vector2f position, Vector2f velocity, Vector2f size, float mass, bool stat, Color colour) {
        
        setPosition(position);
        setVel(velocity);
        setSize(size);
        setMass(mass);
        StaticEntity = stat;
        setFillColor(colour);

    }
    
    Entity() {}

    void setVel(Vector2f vel) { Velocity = vel; }
    Vector2f getVel() { return Velocity; }

    void SetAcc(Vector2f Acc) { Acceleration = Acc; }
    Vector2f getAcc() { return Acceleration; }

    void setMass(float mass) { Mass = mass; }
    float getMass() { return Mass; }

    bool IsStatic() { return StaticEntity; }

private:

    bool StaticEntity = false;
    float Mass = 0.0f;
    Vector2f Velocity;
    Vector2f Acceleration;

};