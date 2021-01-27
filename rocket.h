#pragma once

namespace Tmpl8
{

class Rocket
{
  public:
    Rocket(vec2 position, vec2 direction, float collision_radius, allignments allignment, Sprite* rocket_sprite);
    ~Rocket();

    void tick();
    void draw(Surface* screen);

    bool intersects(vec2 position_other, float radius_other) const;

    vec2 get_position() const { return position; };
    float get_collision_radius() const { return collision_radius; };

    int getTileIndex(float tsizeX, float tsizeY);

    vec2 position;
    vec2 speed;

    float collision_radius;

    bool active;

    allignments allignment;

    int current_frame;
    Sprite* rocket_sprite;

    int currentTile;
};

} // namespace Tmpl8