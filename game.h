#pragma once

namespace Tmpl8
{
//forward declarations
class Tank;
class Rocket;
class Smoke;
class Particle_beam;

class Game
{
  public:
    void set_target(Surface* surface) { screen = surface; }
    void init();
    void shutdown();
    void update(float deltaTime);
    void draw();
    void tick(float deltaTime);
    void fillKDTree();
    void updateTanks();
    void updateRockets();
    void update_tanks();

    //void insertion_sort_tanks_health(const std::vector<Tank>& original, std::vector<const Tank*>& sorted_tanks, int begin, int end);
    void merge_sort_tanks_health(std::vector<Tank*>& sorted_tanks, int begin, int end, int depth);
    void merge_tanks_health(std::vector<Tank*> v1, std::vector<Tank*> v2, std::vector<Tank*>& sorted_tanks, int begin, int end);
    void measure_performance();


    Tank& find_closest_enemy(Tank& current_tank);

    void FillGrid();

    void mouse_up(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void mouse_down(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void mouse_move(int x, int y)
    { /* implement if you want to detect mouse movement */
    }

    void key_up(int key)
    { /* implement if you want to handle keys */
    }

    void key_down(int key)
    { /* implement if you want to handle keys */
    }

  private:
    Surface* screen;

    Node* kdtree;

    vector<Tank> tanks;
    vector<Rocket> rockets;
    vector<Smoke> smokes;
    vector<Explosion> explosions;
    vector<Particle_beam> particle_beams;

    float tsizeX, tsizeY;

    Grid* grid;

    Font* frame_count_font;
    long long frame_count = 0;

    bool lock_update = false;

};

}; // namespace Tmpl8