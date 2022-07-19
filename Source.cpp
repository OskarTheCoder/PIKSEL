#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

class Player
{
    public:
        int posX;
        int posY;
        float worldPosX;
        float worldPosY;
        int fov;
        int half_fov;
        int angle;
};

class Enemy
{
    public:
        int worldPosX;
        int worldPosY;
        SDL_Rect rect;
        char type;
};

int correctDegrees(int d)
{
    while (d > 360)
    {
        d -= 360;
    }
    while (d < 0)
    {
        d += 360;
    }

    return d;
}

SDL_Rect getScaledRect(float dist, SDL_Rect org)
{
    SDL_Rect returned;

    float mulip = dist / 100;
    returned.w = org.w / mulip;
    returned.h = org.h / mulip;
    return returned;

    /*if (dist < 50)
    {
        returned.w = org.w * 2;
        returned.h = org.h * 2;
        return returned;
    }
    else if (dist < 100)
    {
        returned.w = org.w * 1.7;
        returned.h = org.h * 1.7;
        return returned;
    }
    else if (dist < 150)
    {
        returned.w = org.w * 1.4;
        returned.h = org.h * 1.4;
        return returned;
    }
    else if (dist < 200)
    {
        returned.w = org.w * 1.1;
        returned.h = org.h * 1.1;
        return returned;
    }
    else if (dist < 230)
    {
        returned.w = org.w;
        returned.h = org.h;
        return returned;
    }*/

    return org;
}


int angleBetween(int degreesX, int degreesY)
{
    while (degreesY > 360)
    {
        degreesY -= 360;
    }
    while (degreesY < 0)
    {
        degreesY += 360;
    }
    while (degreesX > 360)
    {
        degreesX -= 360;
    }
    while (degreesX < 0)
    {
        degreesX += 360;
    }

    return abs( degreesY - degreesX );

}

int angleBetweenNoAbs(int degreesX, int degreesY)
{
    while (degreesY > 360)
    {
        degreesY -= 360;
    }
    while (degreesY < 0)
    {
        degreesY += 360;
    }
    while (degreesX > 360)
    {
        degreesX -= 360;
    }
    while (degreesX < 0)
    {
        degreesX += 360;
    }

    return degreesY - degreesX;

}


double angleFrom(vector<int> p1, vector<int> p2)
{

    // # Kalkulerer retningen mellom to vektorer

    int x1 = p1[0];
    int x2 = p2[0];
    int y1 = p1[1];
    int y2 = p2[1];

    int a = x2 - x1;
    int b = y2 - y1;


    double angle = atan2(b, a);
    angle = angle / 3.14 * 180;
    return angle + 90;
}


void render(SDL_Rect rect, SDL_Texture* texture, SDL_Renderer* renderer)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = rect.w;
    src.h = rect.h;

    SDL_Rect dst;
    dst.x = rect.x;
    dst.y = rect.y;
    dst.w = rect.w;
    dst.h = rect.h;


    SDL_RenderCopy(renderer, texture, &src, &dst);

}

int distance(vector<int> _p1, vector<int> _p2)
{
    float dist = sqrtf((_p2[0] - _p1[0]) * (_p2[0] - _p1[0]) + (_p2[1] - _p1[1]) * (_p2[1] - _p1[1]));
    return int(dist);
}

float distancef(vector<float> _p1, vector<float> _p2)
{
    float dist = sqrtf((_p2[0] - _p1[0]) * (_p2[0] - _p1[0]) + (_p2[1] - _p1[1]) * (_p2[1] - _p1[1]));
    return dist;
}

void cast_rays(SDL_Renderer& renderer, Player& player, vector<vector<int>> map)
{
    /*SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = 200;
    r.h = 500;
    SDL_RenderDrawRect(&renderer, &r);*/

    int leftmost_angle = player.angle - player.half_fov;
    int rightmost_angle = player.angle + player.half_fov;

    int maxCastRays = 120;
    float step = player.fov / (float)maxCastRays;

    int maxSize = 480;


    for (int l = 0; l < maxCastRays; l++) {
        //cout << l << endl;
        float curAngle = leftmost_angle + step * l;
        //cout << curAngle << endl;

        float dirX = -sin(curAngle*3.14/180);
        float dirY = cos(curAngle*3.14/180);

        for (int depth = 1; depth <= maxSize; depth++)
        {
            float curPosX = player.worldPosX + dirX * depth;
            float curPosY = player.worldPosY + dirY * depth;

            
            int mapX = curPosX / 48;
            int mapY = curPosY / 48;

            if (map[mapY][mapX] != 0) {
                if (map[mapY][mapX] == 1)
                {
                    //cout << curPosY << endl;
                    vector<float> p1 = { (float)player.worldPosX, (float)player.worldPosY };
                    vector<float> p2 = { curPosX, curPosY };
                    float dist = distancef(p1, p2);
                    //cout << dist << endl;
                    float height = 480 / dist * 10;
                    //cout << height << endl;

                    for (int i = 0; i < 4; i++)
                    {
                        SDL_SetRenderDrawColor(&renderer, 255, 255, 0, 255);
                        SDL_RenderDrawLineF(&renderer, l * 4 + i + 480, 240 - height / 2, l * 4 + i + 480, 240 + height / 2);
                    }
                }
                

                break;
                
            }

        }

    }


}

vector<vector<int>> midPointCircleDraw(int x_centre, int y_centre, int r)
{
    vector<vector<int>> points;
    int x = r, y = 0;

    // Printing the initial point on the axes
    // after translation
    points.push_back({ x + x_centre, y + y_centre });

    // When radius is zero only a single
    // point will be printed
    if (r > 0)
    {
        points.push_back({ x + x_centre , -y + y_centre });
        points.push_back({ y + x_centre , x + y_centre});
        points.push_back({ -y + x_centre, x + y_centre });
    }

    // Initialising the value of P
    int P = 1 - r;
    while (x > y)
    {
        y++;

        // Mid-point is inside or on the perimeter
        if (P <= 0)
            P = P + 2 * y + 1;
        // Mid-point is outside the perimeter
        else
        {
            x--;
            P = P + 2 * y - 2 * x + 1;
        }

        // All the perimeter points have already been printed
        if (x < y)
            break;

        // Printing the generated point and its reflection
        // in the other octants after translation
        points.push_back({ x + x_centre , y + y_centre });
        points.push_back({ -x + x_centre , y + y_centre });
        points.push_back({ x + x_centre , -y + y_centre });
        points.push_back({ -x + x_centre , -y + y_centre });

        // If the generated point is on the line x = y then
        // the perimeter points have already been printed
        if (x != y)
        {
            points.push_back({ y + x_centre , x + y_centre });
            points.push_back({ -y + x_centre , x + y_centre });
            points.push_back({ y + x_centre , -x + y_centre });
            points.push_back({ -y + x_centre , -x + y_centre });
        }
    }

    return points;
}

int main(int argc, char** argv)
{

    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);


    SDL_Window* screen = SDL_CreateWindow("Raycaster",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480*2, 480, 0);

    SDL_Event event = SDL_Event();

    vector<vector<int>> MAP = { 
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

   


    const int WIN_SIZE = 480;
    const int TILE_SIZE = WIN_SIZE / MAP.size();

    SDL_Renderer* renderer = SDL_CreateRenderer(screen, -1, 0);

    SDL_Texture* ak = IMG_LoadTexture(renderer, "ak.png");
    SDL_Rect akRect;
    akRect.x = 500;
    akRect.y = 240;
    akRect.w = 480;
    akRect.h = 240;

    SDL_Texture* zombie = IMG_LoadTexture(renderer, "zombie.png");
    Enemy enemy = Enemy();
    SDL_Rect orgEnemyRect;
   
    enemy.worldPosX = 100;
    enemy.worldPosY = 255;
    enemy.type = 'z';
    enemy.rect.x = 480 + 240 - 50;
    enemy.rect.y = 240 - 90;
    enemy.rect.w = 111;
    enemy.rect.h = 200;

    orgEnemyRect.x = 480 + 240 - 50;
    orgEnemyRect.y = 240 - 90;
    orgEnemyRect.w = 111;
    orgEnemyRect.h = 200;
   
    int oldDiff = 0;

    Player player = Player();
    player.posX = 2;
    player.posY = 2;
    player.worldPosX = 2 * TILE_SIZE;
    player.worldPosY = 2 * TILE_SIZE;
    player.fov = 60;
    player.half_fov = 30;
    player.angle = 180;


    vector<SDL_Rect> map_rects_g = {};
    for (int r = 0; r < MAP.size(); r++)
    {
        for (int c = 0; c < MAP.size(); c++)
        {
            if (MAP[r][c] == 0)
            {
                SDL_Rect rect;
                rect.x = c * TILE_SIZE;
                rect.y = r * TILE_SIZE;
                rect.w = TILE_SIZE;
                rect.h = TILE_SIZE;
                map_rects_g.push_back(rect);
            }
        }
    }
        
    vector<SDL_Rect> map_rects_w = {};
    for (int r = 0; r < MAP.size(); r++)
    {
        for (int c = 0; c < MAP.size(); c++)
        {
            if (MAP[r][c] == 1)
            {
                SDL_Rect rect;
                rect.x = c * TILE_SIZE;
                rect.y = r * TILE_SIZE;
                rect.w = TILE_SIZE;
                rect.h = TILE_SIZE;
                map_rects_w.push_back(rect);
            }
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    while (!(event.type == SDL_QUIT))
    {
        SDL_Delay(1000 / 60);
        SDL_PollEvent(&event);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (state[SDL_SCANCODE_LEFT]) {
            player.angle -= 5;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            player.angle += 5;
        }
        if (state[SDL_SCANCODE_UP]) {

            float dirX = -1.0 * sin(player.angle * 3.14 / 180);
            float dirY = cos(player.angle * 3.14 / 180);

            if (MAP[(player.worldPosY + dirY * 2) / TILE_SIZE][(player.worldPosX + dirX * 2) / TILE_SIZE] == 0)
            {
                player.worldPosX += dirX * 2;
                player.worldPosY += dirY * 2;
                player.posY = player.worldPosY / TILE_SIZE;
                player.posX = player.worldPosX / TILE_SIZE;
            }
        }
        if (state[SDL_SCANCODE_DOWN]) {
            // Backwards / Baklengs
            float dirX = sin(player.angle * 3.14 / 180);
            float dirY = -1 * cos(player.angle * 3.14 / 180);
            
            if (MAP[(player.worldPosY + dirY * 2) / TILE_SIZE][(player.worldPosX + dirX * 2) / TILE_SIZE] == 0)
            {
                player.worldPosX += dirX;
                player.worldPosY += dirY;
                player.posY = player.worldPosY / TILE_SIZE;
                player.posX = player.worldPosX / TILE_SIZE;
            }
        }

        cast_rays(*renderer, player, MAP);



        //cout << angleFrom({ 0,0 }, { 1,1 }) << endl;
        int aB = correctDegrees(angleFrom({ (int)player.worldPosX,(int)player.worldPosY }, { enemy.worldPosX, enemy.worldPosY }));
        int diff = correctDegrees(angleBetweenNoAbs(aB, player.angle));
        /*if (diff != oldDiff)
        {
            cout << diff << endl;
            oldDiff = diff;
        }*/


        if (diff > (180 - player.half_fov - 50) && diff < (180 + player.half_fov + 50))
        {
            //cout << diff - 180 << endl;


            float val = 180 - diff;
            //cout << val << endl;
            float part = abs(val / player.half_fov * 100);
            //cout << part << "%" << endl;
            float add;
            if (diff > 180)
            {
                add = 240.0 - (240 / 100 * part);
            }
            else
            {
                add = 240.0 + (240 / 100 * part);
            }

            //cout << distancef({ player.worldPosX, player.worldPosY }, { (float)enemy.worldPosX,(float)enemy.worldPosY }) << endl;
            enemy.rect = getScaledRect(distancef({ player.worldPosX, player.worldPosY }, { (float)enemy.worldPosX,(float)enemy.worldPosY }), orgEnemyRect);
            enemy.rect.x = 480 + add;
            enemy.rect.y = orgEnemyRect.y;

            render(enemy.rect, zombie, renderer);
        }
        
        // Background / Bakgrunn
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        for (int i = 0; i < map_rects_g.size(); i++)
        {
            SDL_RenderFillRect(renderer, &map_rects_g[i]);
        }

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        for (int i = 0; i < map_rects_w.size(); i++)
        {
            SDL_RenderFillRect(renderer, &map_rects_w[i]);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (int y = 0; y < MAP.size(); y++)
        {
            SDL_RenderDrawLine(renderer, 0, y * TILE_SIZE, WIN_SIZE, y * TILE_SIZE);
        }

        for (int x = 0; x < MAP.size(); x++)
        {
            SDL_RenderDrawLine(renderer, x * TILE_SIZE, 0, x * TILE_SIZE, WIN_SIZE);
        }


        // Player / Spiller
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        for (int r = 8; r > 0; r--)
        {
            vector<vector<int>> ps = midPointCircleDraw(player.worldPosX, player.worldPosY, r);
            for (int p = 0; p < ps.size(); p++)
            {
                SDL_RenderDrawPoint(renderer, ps[p][0], ps[p][1]);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int r = 8; r > 0; r--)
        {
            vector<vector<int>> ps = midPointCircleDraw(enemy.worldPosX, enemy.worldPosY, r);
            for (int p = 0; p < ps.size(); p++)
            {
                SDL_RenderDrawPoint(renderer, ps[p][0], ps[p][1]);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLineF(renderer, player.worldPosX, player.worldPosY, player.worldPosX + ( - sin(player.angle * 3.14 / 180) * 50), player.worldPosY + (cos(player.angle * 3.14 / 180)*50));
        
        

        render(akRect, ak, renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(screen);

    SDL_DestroyRenderer(renderer);

    IMG_Quit();

    SDL_Quit();

    return 0;
}