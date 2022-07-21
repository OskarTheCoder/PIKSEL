#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include <cmath>

/*
Legg til Røyk effekt?
Kollision -> på skjermen som tegnes opp til høyre med rects ikke i lister 
Skrekk Spill???

*/

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
    int health;
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
    returned.w = org.w / min((float)1.0, mulip);
    returned.h = org.h / min((float)1.0, mulip);
    if (mulip > 1.0) {
        returned.w = 0;
        returned.h = 0;
    }
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

    return abs(degreesY - degreesX);

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
        float curAngle = leftmost_angle + step * l;
        //cout << curAngle << endl;

        float dirX = -sin(curAngle * 3.14 / 180);
        float dirY = cos(curAngle * 3.14 / 180);

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

                    for (int i = 0; i < 4; i++)
                    {
                        SDL_SetRenderDrawColor(&renderer, 127, 127, 127, 255);
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
        points.push_back({ y + x_centre , x + y_centre });
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
    SDL_Init(SDL_INIT_AUDIO);
    TTF_Init();
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048) < 0) {
        cout << "bruh..." << Mix_GetError() << endl;
    }

    Mix_Music *bgm = Mix_LoadMUS("scary-forest-90162.mp3");
    Mix_Chunk *fireSoundEffect = Mix_LoadWAV("fire.wav");

    IMG_Init(IMG_INIT_PNG);


    SDL_Window* screen = SDL_CreateWindow("Raycaster",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480 * 2, 480, 0);

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

    TTF_Font *ammoFont = TTF_OpenFont("HelpMe.ttf", 20);

    SDL_Color clrFg = { 0,0,255,0 };  // ("Fg" is foreground)
    SDL_Surface* sText = TTF_RenderText_Solid(ammoFont, "Courier 12", clrFg);
    SDL_Rect rcDest = { 0,0,0,0 };
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, sText);
    //SDL_BlitSurface(sText, NULL, , &rcDest);
    SDL_Rect textRenderRect;
    textRenderRect.x = 0;
    textRenderRect.y = 0;
    textRenderRect.w = sText->w;
    textRenderRect.h = sText->h;
    render(textRenderRect, textTexture, renderer);
    SDL_FreeSurface(sText);

    TTF_CloseFont(ammoFont);


    vector<Enemy> enemies;
    bool fired = false;
    vector<int> centerPos = {WIN_SIZE+WIN_SIZE/2,WIN_SIZE/2};
    
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
    enemy.rect.w = 39;
    enemy.rect.h = 64;
    enemy.health = 4;

    enemies.push_back(enemy);
    enemy.worldPosX += 50;
    enemy.rect.x += 50;
    enemies.push_back(enemy);

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

    

    SDL_Texture* fireEffect = IMG_LoadTexture(renderer, "firemuzzle.png");
    
    SDL_Rect fireEffectRect;
    vector<int> fireEffectPosition = { centerPos[0] + 20, centerPos[1] + 40 };
    fireEffectRect.x = fireEffectPosition[0];
    fireEffectRect.y = fireEffectPosition[1];
    fireEffectRect.w = 32;
    fireEffectRect.h = 32;
    bool fireEffectShown = false;
    int currentFireEffectLifetime = 0;
    int fireEffectLifeTime = 5;
    
    

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

        if (state[SDL_SCANCODE_RCTRL]) {
            if (!fired)
            {
                Mix_PlayChannel(-1, fireSoundEffect, 0);
                fireEffectShown = true;

                for (int r = 0; r < enemies.size(); r++) // rectsOnScreen.size()
                {
                    if (enemies[r].type == 'z')
                    {
                        SDL_Rect* curRect = &enemies[r].rect;
                        cout << curRect->x << ", " << curRect->y << endl;
                        SDL_Point point;
                        point.x = centerPos[0];
                        point.y = centerPos[1];
                        SDL_Point* constPoint = &point;
                        if (SDL_PointInRect(constPoint, curRect)) {
                            cout << " ENEMY HIT! " << endl;
                            enemies[r].health -= 1;
                            if (enemies[r].health <= 0) {
                                enemies[r].type = 'd';
                            }
                        }
                    }
                }
                fired = true;
            }
        }
        else
        {
            fired = false;
        }

        if (state[SDL_SCANCODE_LCTRL]) {
            if (!Mix_PlayingMusic()) {
                Mix_PlayMusic(bgm, -1);
            }
        }


        cast_rays(*renderer, player, MAP);

        for (int e = 0; e < enemies.size(); e++)
        {
            if (enemies[e].type == 'z')
            {

                Enemy currentEnemy = enemies[e];

                int aB = correctDegrees(angleFrom({ (int)player.worldPosX,(int)player.worldPosY }, { currentEnemy.worldPosX, currentEnemy.worldPosY }));
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

                    float part = abs(val / player.half_fov * 100);
                    
                    float add;
                    if (diff > 180)
                    {
                        add = 240.0 - (240 / 100 * part);
                    }
                    else
                    {
                        add = 240.0 + (240 / 100 * part);
                    }

                    currentEnemy.rect = getScaledRect(distancef({ player.worldPosX, player.worldPosY }, { (float)currentEnemy.worldPosX,(float)currentEnemy.worldPosY }), orgEnemyRect);
                    currentEnemy.rect.x = 480 + add;
                    currentEnemy.rect.y = orgEnemyRect.y;

                    enemies[e].rect = getScaledRect(distancef({ player.worldPosX, player.worldPosY }, { (float)currentEnemy.worldPosX,(float)currentEnemy.worldPosY }), orgEnemyRect);
                    enemies[e].rect.x = 480 + add;
                    enemies[e].rect.y = orgEnemyRect.y;

                    render(currentEnemy.rect, zombie, renderer);

                }
            }
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

        for (int r = 6; r > 0; r--)
        {
            vector<vector<int>> ps = midPointCircleDraw(centerPos[0], centerPos[1], r);
            for (int p = 0; p < ps.size(); p++)
            {
                SDL_RenderDrawPoint(renderer, ps[p][0], ps[p][1]);
            }
        }


        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int t = 0; t < enemies.size(); t++)
        {
            if (enemies[t].type == 'z')
            {
                for (int r = 8; r > 0; r--)
                {
                    vector<vector<int>> ps = midPointCircleDraw(enemies[t].worldPosX, enemies[t].worldPosY, r);
                    for (int p = 0; p < ps.size(); p++)
                    {
                        SDL_RenderDrawPoint(renderer, ps[p][0], ps[p][1]);
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLineF(renderer, player.worldPosX, player.worldPosY, player.worldPosX + (-sin(player.angle * 3.14 / 180) * 50), player.worldPosY + (cos(player.angle * 3.14 / 180) * 50));



        render(akRect, ak, renderer);
        if (fireEffectShown) {
            currentFireEffectLifetime++;
            render(fireEffectRect, fireEffect, renderer);
            if (currentFireEffectLifetime >= fireEffectLifeTime) {
                currentFireEffectLifetime = 0;
                fireEffectShown = false;
            }
        }
        TTF_Font* ammoFont = TTF_OpenFont("HelpMe.ttf", 20);

        SDL_Color clrFg = { 255,0,0,0 };  // ("Fg" is foreground)
        SDL_Surface* sText = TTF_RenderText_Solid(ammoFont, "24/24", clrFg);
        SDL_Rect rcDest = { 960-sText->w,480-sText->h,0,0 };
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, sText);
        //SDL_BlitSurface(sText, NULL, , &rcDest);
        SDL_Rect textRenderRect;
        textRenderRect.x = 960 - sText->w;
        textRenderRect.y = 480-sText->h;
        textRenderRect.w = sText->w;
        textRenderRect.h = sText->h;
        render(textRenderRect, textTexture, renderer); 
        SDL_FreeSurface(sText);

        TTF_CloseFont(ammoFont);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(screen);

    SDL_DestroyRenderer(renderer);

    IMG_Quit();

    Mix_FreeMusic(bgm);
    Mix_FreeChunk(fireSoundEffect);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}