#include "precomp.h" // include (only) this in every .cpp file

#define NUM_TANKS_BLUE 1279
#define NUM_TANKS_RED 1279

#define TANK_MAX_HEALTH 1000
#define ROCKET_HIT_VALUE 60
#define PARTICLE_BEAM_HIT_VALUE 50

#define TANK_MAX_SPEED 1.5

#define HEALTH_BARS_OFFSET_X 0
#define HEALTH_BAR_HEIGHT 70
#define HEALTH_BAR_WIDTH 1
#define HEALTH_BAR_SPACING 0

#define MAX_FRAMES 2000

//Global performance timer
#define REF_PERFORMANCE 31546 //UPDATE THIS WITH YOUR REFERENCE PERFORMANCE (see console after 2k frames)
static timer perf_timer;
static float duration;

//Load sprite files and initialize sprites
static Surface* background_img = new Surface("assets/Background_Grass.png");
static Surface* tank_red_img = new Surface("assets/Tank_Proj2.png");
static Surface* tank_blue_img = new Surface("assets/Tank_Blue_Proj2.png");
static Surface* rocket_red_img = new Surface("assets/Rocket_Proj2.png");
static Surface* rocket_blue_img = new Surface("assets/Rocket_Blue_Proj2.png");
static Surface* particle_beam_img = new Surface("assets/Particle_Beam.png");
static Surface* smoke_img = new Surface("assets/Smoke.png");
static Surface* explosion_img = new Surface("assets/Explosion.png");

static Sprite background(background_img, 1);
static Sprite tank_red(tank_red_img, 12);
static Sprite tank_blue(tank_blue_img, 12);
static Sprite rocket_red(rocket_red_img, 12);
static Sprite rocket_blue(rocket_blue_img, 12);
static Sprite smoke(smoke_img, 4);
static Sprite explosion(explosion_img, 9);
static Sprite particle_beam_sprite(particle_beam_img, 3);

const static vec2 tank_size(14, 18);
const static vec2 rocket_size(25, 24);

const static float tank_radius = 8.5f;
const static float rocket_radius = 10.f;

ThreadPool tp(THREADCOUNT);
std::mutex mut;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::init()
{
    frame_count_font = new Font("assets/digital_small.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ:?!=-0123456789.");

    tanks.reserve(NUM_TANKS_BLUE + NUM_TANKS_RED);

    uint rows = (uint)sqrt(NUM_TANKS_BLUE + NUM_TANKS_RED);
    uint max_rows = 12;

    float start_blue_x = tank_size.x + 10.0f;
    float start_blue_y = tank_size.y + 80.0f;

    float start_red_x = 980.0f;
    float start_red_y = 100.0f;

    float spacing = 15.0f;
    
    //Spawn blue tanks
    for (int i = 0; i < NUM_TANKS_BLUE; i++)
    {
        tanks.push_back(Tank(start_blue_x + ((i % max_rows) * spacing), start_blue_y + ((i / max_rows) * spacing), BLUE, &tank_blue, &smoke, 1200, 600, tank_radius, TANK_MAX_HEALTH, TANK_MAX_SPEED));
    }
    //Spawn red tanks
    for (int i = 0; i < NUM_TANKS_RED; i++)
    {
        tanks.push_back(Tank(start_red_x + ((i % max_rows) * spacing), start_red_y + ((i / max_rows) * spacing), RED, &tank_red, &smoke, 80, 80, tank_radius, TANK_MAX_HEALTH, TANK_MAX_SPEED));
    }

    particle_beams.push_back(Particle_beam(vec2(SCRWIDTH / 2, SCRHEIGHT / 2), vec2(100, 50), &particle_beam_sprite, PARTICLE_BEAM_HIT_VALUE));
    particle_beams.push_back(Particle_beam(vec2(80, 80), vec2(100, 50), &particle_beam_sprite, PARTICLE_BEAM_HIT_VALUE));
    particle_beams.push_back(Particle_beam(vec2(1200, 600), vec2(100, 50), &particle_beam_sprite, PARTICLE_BEAM_HIT_VALUE));

    grid = new Grid(vec2(GRIDROW, GRIDCOL), tanks);
    FillGrid();
}

// -----------------------------------------------------------
// Close down application
// -----------------------------------------------------------
void Game::shutdown()
{
}

void Game::fillKDTree() {
    if (kdtree != NULL){
        kdtree = NULL;
    }

    Tank* dummytank = new Tank(SCRWIDTH/2, SCRHEIGHT/2, DUMMY, &tank_red, &smoke, 80, 600, tank_radius, TANK_MAX_HEALTH, TANK_MAX_SPEED);
    kdtree = new Node(dummytank);
    for (int i = 1; i < tanks.size(); i++) {
        kdtree->insert(&tanks[i]);
    }
}

void Tmpl8::Game::FillGrid()
{
    float tsizeX = SCRWIDTH / grid->GetGsize().x;
    float tsizeY = SCRHEIGHT / grid->GetGsize().y;

    int index = 0;
    for (int i = 0; i < (int)tanks.size(); i++) {
        int indexX;
        int indexY;

        if (tanks[i].position.x > SCRWIDTH) {
            indexX = (GRIDROW -1);
        }else
            indexX = floor(tanks[i].position.x / tsizeX);

        if (tanks[i].position.y > SCRHEIGHT) {
            indexY = (GRIDCOL -1);
        }else
            indexY = floor(tanks[i].position.y / tsizeY);

        index = ((GRIDROW * indexY) + indexX);

        grid->getTiles()[index]->AddTankToTile(&tanks[i]);
        tanks[i].setCurrentTileIndex(index);
        tanks[i].setIndex(i);
    }
    cout << "done" << endl;
}


// -----------------------------------------------------------
// Iterates through all tanks and returns the closest enemy tank for the given tank
// -----------------------------------------------------------
Tank& Game::find_closest_enemy(Tank& current_tank)
{
    float closest_distance = numeric_limits<float>::infinity();
    int closest_index = 0;

    for (int i = 0; i < tanks.size(); i++)
    {
        if (tanks.at(i).allignment != current_tank.allignment && tanks.at(i).active)
        {
            float sqr_dist = fabsf((tanks.at(i).get_position() - current_tank.get_position()).sqr_length());
            if (sqr_dist < closest_distance)
            {
                closest_distance = sqr_dist;
                closest_index = i;
            }
        }
    }

    return tanks.at(closest_index);
}

void Game::updateTanks() {
    //Update tanks
    for (Tank& tank : tanks)
    {
        int depth = 0;
        if (tank.active)
        {
            vector<Tile*> surroundingTiles = grid->GetSurroundedTiles(tank.getCurrentTileIndex());
        

            for (int i = 0; i < (int)surroundingTiles.size(); i++) {
                vector<Tank*> thisTileTanks = surroundingTiles[i]->GetTanks();

                for (Tank* oTank : thisTileTanks)
                {
                    //std::cout << "normal in" << endl;
                    if (&tank == oTank) continue;

                    vec2 dir = tank.get_position() - oTank->get_position();
                    float dirSquaredLen = dir.sqr_length();

                    float colSquaredLen = (tank.get_collision_radius() + oTank->get_collision_radius());
                    colSquaredLen *= colSquaredLen;

                    if (dirSquaredLen < colSquaredLen)
                    {
                        tank.push(dir.normalized(), 0.6f);
                    }
                    //std::cout << "normal out" << endl;
                }

            }
          

            //Move tanks according to speed and nudges (see above) also reload
            tank.tick();

            //Shoot at closest target if reloaded
            if (tank.rocket_reloaded())
            {
                Tank& target = find_closest_enemy(tank);

                rockets.push_back(Rocket(tank.position, (target.get_position() - tank.position).normalized() * 3, rocket_radius, tank.allignment, ((tank.allignment == RED) ? &rocket_red : &rocket_blue)));

                tank.reload_rocket();
            }
        }
    }
}

void Game::updateRockets() {
    //Update rockets
    for (Rocket& rocket : rockets)
    {
        if (rocket.active) {
            float tsizeX = SCRWIDTH / grid->GetGsize().x;
            float tsizeY = SCRHEIGHT / grid->GetGsize().y;

            rocket.tick();

            if (rocket.get_position().x > 0 && rocket.get_position().y > 0 && rocket.get_position().x < SCRWIDTH && rocket.get_position().y < SCRHEIGHT) {
                int rocketTile = rocket.getTileIndex(tsizeX, tsizeY);
                std::vector<Tank*> tileTanks = grid->getTiles()[rocketTile]->GetTanks();

                //Check if rocket collides with enemy tank, spawn explosion and if tank is destroyed spawn a smoke plume
                for (Tank* tank : tileTanks)
                {
                    if (tank->active && (tank->allignment != rocket.allignment) && rocket.intersects(tank->position, tank->collision_radius))
                    {
                        explosions.push_back(Explosion(&explosion, tank->position));

                        if (tank->hit(ROCKET_HIT_VALUE))
                        {
                            smokes.push_back(Smoke(smoke, tank->position - vec2(0, 48)));
                        }

                        rocket.active = false;
                        break;
                    }
                }
            }
        }
    }

    //Remove exploded rockets with remove erase idiom
    rockets.erase(std::remove_if(rockets.begin(), rockets.end(), [](const Rocket& rocket) { return !rocket.active; }), rockets.end());
}

vector<future<void>> futures;
void Game::update_tanks()
{
    int workload = tanks.size();
    int chunksize = tanks.size() / THREADCOUNT;
    int start = 0;
    int end = start + chunksize;
    int remaining = workload % THREADCOUNT;
    int currently_remaining = remaining;



    for (size_t i = 0; i < THREADCOUNT; i++)
    {
        if (currently_remaining > 0) {
            end++;
            currently_remaining--;
        }

        futures.push_back(tp.enqueue([&] {
            for (size_t j = 0; j < end; j++)
            {
                Tank& tank = tanks.at(j);
                if (tank.active)
                {
                    vector<Tile*> surroundingTiles = grid->GetSurroundedTiles(tank.getCurrentTileIndex());


                    for (int i = 0; i < (int)surroundingTiles.size(); i++) {
                        vector<Tank*> thisTileTanks = surroundingTiles[i]->GetTanks();

                        for (Tank* oTank : thisTileTanks)
                        {
                            if (&tank == oTank) continue;

                            vec2 dir = tank.get_position() - oTank->get_position();
                            float dirSquaredLen = dir.sqr_length();

                            float colSquaredLen = (tank.get_collision_radius() + oTank->get_collision_radius());
                            colSquaredLen *= colSquaredLen;

                            if (dirSquaredLen < colSquaredLen)
                            {
                                tank.push(dir.normalized(), 0.6f);
                            }
                        }

                    }


                    //Move tanks according to speed and nudges (see above) also reload
                    tank.tick();

                    //Shoot at closest target if reloaded
                    if (tank.rocket_reloaded())
                    {
                        Tank& target = find_closest_enemy(tank);

                        rockets.push_back(Rocket(tank.position, (target.get_position() - tank.position).normalized() * 3, rocket_radius, tank.allignment, ((tank.allignment == RED) ? &rocket_red : &rocket_blue)));

                        tank.reload_rocket();
                    }
                }

            }
            }));
        start = end;
        end += chunksize;
    }

    for (int i = 0; i < futures.size(); i++)
    {
        
    }
}


// -----------------------------------------------------------
// Update the game state:
// Move all objects
// Update sprite frames
// Collision detection
// Targeting etc..
// -----------------------------------------------------------
unsigned int depth = 0;
void Game::update(float deltaTime)
{
    //update grid
    grid->UpdateTanksInTiles();

    if (depth <= THREADCOUNT) {
        std::future<void> fut = tp.enqueue([&] {updateTanks(); });
        depth++;
        fut.wait();
        depth--;
    }
    else {
        updateTanks();
    }

    //Helaas is het niet gelukt om deze functie aan de praat te krijgen, maar we hebben besloten het er wel in te houden
    //update_tanks();

    //Update smoke plumes
    for (Smoke& smoke : smokes)
    {
        smoke.tick();
    }

    if (depth <= THREADCOUNT) {
        std::future<void> fut = tp.enqueue([&] {updateRockets(); });
        depth++;
        fut.wait();
        depth--;
    }
    else {
        updateRockets();
    }


    //Update particle beams
    for (Particle_beam& particle_beam : particle_beams)
    {
        particle_beam.tick();
        vec2 offset = (0, 100);
        int beamTileIndex = grid->GetTileIndex(((particle_beam.min_position + particle_beam.max_position) / 2) + offset);
        vector<Tile*> surroundingTiles = grid->GetSurroundedTiles(beamTileIndex);
        for (int i = 0; i < (int)surroundingTiles.size(); i++) {
            std::vector<Tank*> tileTanks = surroundingTiles[i]->GetTanks();

            //Check if rocket collides with enemy tank, spawn explosion and if tank is destroyed spawn a smoke plume
            for (Tank* tank : tileTanks)
            {
                if (tank->active && particle_beam.rectangle.intersects_circle(tank->get_position(), tank->get_collision_radius()))
                {
                    if (tank->hit(particle_beam.damage))
                    {
                        smokes.push_back(Smoke(smoke, tank->position - vec2(0, 48)));
                    }
                }
            }
        }
    }
 

    //Update explosion sprites and remove when done with remove erase idiom
   
    for (Explosion& explosion : explosions){
        explosion.tick();
    } 

    explosions.erase(std::remove_if(explosions.begin(), explosions.end(), [](const Explosion& explosion) { return explosion.done(); }), explosions.end());
}


void Game::draw()
{
    // clear the graphics window
    screen->clear(0);

    //Draw background
    background.draw(screen, 0, 0);

    //Draw sprites
    for (int i = 0; i < NUM_TANKS_BLUE + NUM_TANKS_RED; i++)
    {
        tanks.at(i).draw(screen);

        vec2 tPos = tanks.at(i).get_position();
        // tread marks
        if ((tPos.x >= 0) && (tPos.x < SCRWIDTH) && (tPos.y >= 0) && (tPos.y < SCRHEIGHT))
            background.get_buffer()[(int)tPos.x + (int)tPos.y * SCRWIDTH] = sub_blend(background.get_buffer()[(int)tPos.x + (int)tPos.y * SCRWIDTH], 0x808080);
    }

    for (Rocket& rocket : rockets) {
        rocket.draw(screen);
    }
    
    for (Smoke& smoke : smokes){
        smoke.draw(screen);
    }
    
    for (Particle_beam& particle_beam : particle_beams){
        particle_beam.draw(screen);
    }

    for (Explosion& explosion : explosions){
        explosion.draw(screen);
    }

    //Draw sorted health bars
    for (int t = 0; t < 2; t++)
    {
        const int NUM_TANKS = ((t < 1) ? NUM_TANKS_BLUE : NUM_TANKS_RED);

        const int begin = ((t < 1) ? 0 : NUM_TANKS_BLUE);

        std::vector<Tank*> sorted_tanks;
        for (int i = begin; i < begin + NUM_TANKS; i++) {
            sorted_tanks.push_back(&tanks[i]);
        }

        merge_sort_tanks_health(sorted_tanks, begin, begin + NUM_TANKS, 0);

        for (int i = 0; i < NUM_TANKS; i++)
        {
            int health_bar_start_x = i * (HEALTH_BAR_WIDTH + HEALTH_BAR_SPACING) + HEALTH_BARS_OFFSET_X;
            int health_bar_start_y = (t < 1) ? 0 : (SCRHEIGHT - HEALTH_BAR_HEIGHT) - 1;
            int health_bar_end_x = health_bar_start_x + HEALTH_BAR_WIDTH;
            int health_bar_end_y = (t < 1) ? HEALTH_BAR_HEIGHT : SCRHEIGHT - 1;

            screen->bar(health_bar_start_x, health_bar_start_y, health_bar_end_x, health_bar_end_y, REDMASK);
            screen->bar(health_bar_start_x, health_bar_start_y + (int)((double)HEALTH_BAR_HEIGHT * (1 - ((double)sorted_tanks.at(i)->health / (double)TANK_MAX_HEALTH))), health_bar_end_x, health_bar_end_y, GREENMASK);
        }
    }
}


// Merge
void Tmpl8::Game::merge_tanks_health(std::vector<Tank*> v1, std::vector<Tank*> v2, std::vector<Tank*>& sorted_tanks, int begin, int end) {

    sorted_tanks.clear();
    int v1_index = 0;
    int v2_index = 0;

    while (v1_index <= (int)v1.size() || v2_index <= (int)v2.size()) {
        if ((int)v1.size() > v1_index && (int)v2.size() > v2_index) {
            if (v1.at(v1_index)->health <= v2.at(v2_index)->health) {
                sorted_tanks.push_back(v1.at(v1_index));
                v1_index++;
            }
            else {
                sorted_tanks.push_back(v2.at(v2_index));
                v2_index++;
            }
        }
        else if ((int)v1.size() > v1_index) {
            for (int i = v1_index; i < (int)v1.size(); i++)
                sorted_tanks.push_back(v1.at(i));
            break;
        }
        else if ((int)v2.size() > v2_index) {
            for (int i = v2_index; i < (int)v2.size(); i++)
                sorted_tanks.push_back(v2.at(i));
            break;
        }
    }

    return;
}

// Merge Sort
void Tmpl8::Game::merge_sort_tanks_health(std::vector<Tank*>& sorted_tanks, int begin, int end, int depth) {

    if ((end - begin) <= 1) {
        return;
    }

    std::vector<Tank*> left, right;
    left.reserve(end);
    right.reserve(end);

    int middle = (int)sorted_tanks.size() / 2;
    for (int i = 0; i < middle; i++) {
        left.push_back(sorted_tanks.at(i));
    }
    for (int i = middle; i < (int)sorted_tanks.size(); i++) {
        right.push_back(sorted_tanks.at(i));
    }


    if (pow(depth, 2) <= THREADCOUNT) {
        std::future<void> mergefut = tp.enqueue([&] {merge_sort_tanks_health(left, 0, (int)left.size(), (depth + 1)); });
        merge_sort_tanks_health(right, 0, (int)right.size(), (depth + 1));

        mergefut.wait();
    }
    else {
        merge_sort_tanks_health(left, 0, (int)left.size(), depth);
        merge_sort_tanks_health(right, 0, (int)right.size(), depth);
    }


    merge_tanks_health(left, right, sorted_tanks, begin, end);
}

// -----------------------------------------------------------
// When we reach MAX_FRAMES print the duration and speedup multiplier
// Updating REF_PERFORMANCE at the top of this file with the value
// on your machine gives you an idea of the speedup your optimizations give
// -----------------------------------------------------------
void Tmpl8::Game::measure_performance()
{
    char buffer[128];
    if (frame_count >= MAX_FRAMES)
    {
        if (!lock_update)
        {
            duration = perf_timer.elapsed();
            cout << "Duration was: " << duration << " (Replace REF_PERFORMANCE with this value)" << endl;
            lock_update = true;
        }

        frame_count--;
    }

    if (lock_update)
    {
        screen->bar(420, 170, 870, 430, 0x030000);
        int ms = (int)duration % 1000, sec = ((int)duration / 1000) % 60, min = ((int)duration / 60000);
        sprintf(buffer, "%02i:%02i:%03i", min, sec, ms);
        frame_count_font->centre(screen, buffer, 200);
        sprintf(buffer, "SPEEDUP: %4.1f", REF_PERFORMANCE / duration);
        frame_count_font->centre(screen, buffer, 340);
    }
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void Game::tick(float deltaTime)
{
    if (!lock_update)
    {
        update(deltaTime);
    }
    //draw();
    if (depth <= THREADCOUNT) {
        std::future<void> future = tp.enqueue([&]() { draw(); });
        depth++;
        future.wait();
        depth--;
    }
    else {
        draw();
    }

    measure_performance();
  
    //Print frame count
    frame_count++;
    string frame_count_string = "FRAME: " + std::to_string(frame_count);
    frame_count_font->print(screen, frame_count_string.c_str(), 350, 580);

}
