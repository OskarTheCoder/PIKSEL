#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>

/*
Legg til Røyk effekt?
Kollision -> på skjermen som tegnes opp til høyre med rects ikke i lister
Skrekk Spill???

*/

using namespace std;

class Weapon
{
public:
    int maxAmmo;
    int curAmmo;
    int damage;
};

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
    int health;
    Weapon weapon;
    float hitRadius;
};

class Enemy
{
public:
    int worldPosX;
    int worldPosY;
    SDL_Rect rect;
    char type;
    int health;
    float hitRadius;
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

/*
Copies the pixels from a SDL2 surface.
You should free() the returned pixels when you're done with it.
*/
Uint8* copySurfacePixels(
    SDL_Surface* surface,  // surface to take pixels from
    Uint32 pixelFormat,    // usually SDL_GetWindowPixelFormat(window)
    SDL_Renderer* renderer,// main SDL2 renderer
    int* width,            // stores result width
    int* height,           // stores result height
    int* pitch)            // stores result pitch
{
    Uint8* pixels = 0;
    SDL_Surface* tmpSurface = 0;
    SDL_Texture* texture = 0;
    int sizeInBytes = 0;
    void* tmpPixels = 0;
    int tmpPitch = 0;

    tmpSurface = SDL_ConvertSurfaceFormat(surface, pixelFormat, 0);
    if (tmpSurface) {
        texture = SDL_CreateTexture(renderer, pixelFormat,
            SDL_TEXTUREACCESS_STREAMING,
            tmpSurface->w, tmpSurface->h);
    }

    if (texture) {
        if (width) {
            *width = tmpSurface->w;
        }
        if (height) {
            *height = tmpSurface->h;
        }
        if (pitch) {
            *pitch = tmpSurface->pitch;
        }
        sizeInBytes = tmpSurface->pitch * tmpSurface->h;
        pixels = (Uint8*)malloc(sizeInBytes);
        SDL_LockTexture(texture, 0, &tmpPixels, &tmpPitch);
        if (pixels != 0)
        {
            memcpy(pixels, tmpSurface->pixels, sizeInBytes);
        }
        SDL_UnlockTexture(texture);
    }

    // Cleanup
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (tmpSurface) {
        SDL_FreeSurface(tmpSurface);
    }

    return pixels;
}

class textureData {
public:
    vector<vector<vector<int>>> rgbData;
};

textureData getTextureData(SDL_Surface* textureSurf, Uint32 pf, SDL_Renderer* renderer) {

    textureData returnData = textureData();


    int w = 0, h = 0, p = 0;
    Uint8* pixels;
    pixels = copySurfacePixels(textureSurf, pf, renderer, &w, &h, &p);

    int b;
    int a;
    int g;
    int r;

    a = 0;
    b = 0;
    g = 0;
    r = 0;


    for (int x = 0; x < w; x++)
    {
        vector<vector<int>> vec;

        //cout << returnData.rgbData[-1][0][0] << endl;
        for (int y = 0; y < h; y++)
        {
            // Assuming BGRA format
            //cout << x << ", " << y << endl;


            b = pixels[4 * (y * w + x) + 0]; // Blue
            g = pixels[4 * (y * w + x) + 1]; // Green
            r = pixels[4 * (y * w + x) + 2]; // Red
            a = pixels[4 * (y * w + x) + 3]; // Alpha


            vector<int> rgb = { r,g,b,a };
            vec.push_back(rgb);


        }
        returnData.rgbData.push_back(vec);
    }
    free(pixels);
    return returnData;
};


void drawTexturedLine(SDL_Renderer* renderer, textureData* data, Uint32* pf, int x, int y1, int y2, int tile_size) {

    int h = data->rgbData.size();
    int w = data->rgbData[0].size();

    float stepX = (float)w / (float)x;

    float posXinTexture = (float)x / (float)w;
    int posxx = (int)posXinTexture;
    int posxc = posxx * w;

    // diff is the texture position (x in the texture)
    int diff = x - posxc;
    //cout << diff << endl;



    float height = y2 - y1;

    float stepY = h / height;
    float stepYtwo = height / h;


    //for (int y = height/2 * -1; y < height/2; y += stepY)
    float yTwo = height / 2 * -1;
    for (float y = h / 2 * -1; y < h / 2; y += stepY)
    {
        yTwo += stepYtwo;

        int curY = (int)y;
        //cout << curY << endl;
        //cout << h << endl;
        int r = data->rgbData[curY + h / 2][diff][0];
        int g = data->rgbData[curY + h / 2][diff][1];
        int b = data->rgbData[curY + h / 2][diff][2];
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);

        SDL_RenderDrawPoint(renderer, x, 480 / 2 + yTwo);
    }


}

SDL_Texture* t;

void cast_rays(SDL_Renderer& renderer, Player& player, vector<vector<int>> map, SDL_Window* screen, textureData& data, Uint32& pf)
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
                    

                    vector<float> p1 = { (float)player.worldPosX, (float)player.worldPosY };
                    vector<float> p2 = { curPosX, curPosY };
                    float dist = distancef(p1, p2);

                    //int side;
                    //vector<float> pointBeforeHit = { curPosX - dirX, curPosY - dirY };
                    /*if (pointBeforeHit[1] > mapY * 48 || pointBeforeHit[1] < mapY * 48 - 48)
                    {
                        side = 0;
                    }
                    else
                    {
                        side = 1;
                    }*/

                    //float stepX, stepY;
                    float height = 480 / dist * 10;
                    
                    //SDL_Surface* img = IMG_Load("zombie.png");
                    //Uint32 pf = SDL_GetWindowPixelFormat(screen);
                    //int w = 0, h = 0, p = 0;
                    //Uint8* pixels = copySurfacePixels(img, pf, &renderer, &w, &h, &p);
                    /*
                    if (pixels)
                    {
                        //printf("width=%d, height=%d, pitch=%d\n", w, h, p);

                        // Print color at (1,1)
                        int x = 41, y = 41;

                        // Assuming BGRA format
                        int b = pixels[4 * (y * w + x) + 0]; // Blue
                        int g = pixels[4 * (y * w + x) + 1]; // Green
                        int r = pixels[4 * (y * w + x) + 2]; // Red
                        int a = pixels[4 * (y * w + x) + 3]; // Alpha
                        //printf("Pixel at (%d,%d) has RGBA color (%d,%d,%d,%d)\n", x, y, r, g, b, a);
                    }
                    free(pixels);*/
                    /*
                    

                    

                    stepY = height / h;
                    stepX = ((curPosX - (mapX * 48)) / w);
                    //cout << (int)(stepX*64) << endl;
                    //cout << height << ", " << h << endl;
                    
                    int stepPosY, stepPosX;
                    int x, y;
                    int b, g, r, a;
                    for (float o = 0; o < w; o+=stepX)
                    {
                        stepPosX = (int)o;

                        for (float u = 0; u < h; u += stepY)
                        {
                            stepPosY = (int)u;
                            x = stepPosX;
                            y = stepPosY;

                            b = pixels[4 * (y * w + x) + 0]; // Blue
                            g = pixels[4 * (y * w + x) + 1]; // Green
                            r = pixels[4 * (y * w + x) + 2]; // Red
                            a = pixels[4 * (y * w + x) + 3]; // Alpha
                            //cout << o << ", " << u << endl;
                            //cout << o << ", " << u << endl;
                            SDL_SetRenderDrawColor(&renderer, r, g, b, a);
                            SDL_RenderDrawPointF(&renderer, x, y);

                        }
                    }*/
                    //free(pixels);

                    
                    //SDL_SetRenderDrawColor(&renderer, 127, 127, 127, 255);
                    for (int i = 0; i < 4; i++)
                    {
                        drawTexturedLine(&renderer, &data, &pf, l * 4 + i + 480, 0, height, 24);    
                        //SDL_RenderDrawLineF(&renderer, l * 4 + i + 480, 240 - height / 2, l * 4 + i + 480, 240 + height / 2);
                        
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

bool circleCollision(vector<float> p1, vector<float> p2, float r1, float r2)
{
    float dist = distancef(p1, p2);
    //cout << dist << endl;
    //cout << (r1 + r2) << endl;
    if (dist > (r1 + r2))
    {
        return false;
    }
    return true;
}




int main(int argc, char** argv)
{

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    TTF_Init();

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048) < 0) {
        cout << "bruh..." << Mix_GetError() << endl;
    }

    Mix_Music* bgm = Mix_LoadMUS("scary-forest-90162.mp3");
    cout << bgm << endl;
    Mix_Chunk* fireSoundEffect = Mix_LoadWAV("fire.wav");
    Mix_Chunk* clickSoundEffect = Mix_LoadWAV("dryfire.wav");

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

    TTF_Font* ammoFont = TTF_OpenFont("HelpMe.ttf", 20);

    SDL_Color clrFg = { 0,0,255,0 };  // ("Fg" is foreground)
    //SDL_Surface* sText = TTF_RenderText_Solid(ammoFont, "Courier 12", clrFg);
    SDL_Rect rcDest = { 0,0,0,0 };
    //SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, sText);
    //SDL_BlitSurface(sText, NULL, , &rcDest);
    SDL_Rect textRenderRect;
    textRenderRect.x = 0;
    textRenderRect.y = 0;
    textRenderRect.w = 0;
    textRenderRect.h = 0;


    vector<Enemy> enemies;
    bool fired = false;
    vector<int> centerPos = { WIN_SIZE + WIN_SIZE / 2,WIN_SIZE / 2 };

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
    enemy.hitRadius = 8;

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
    player.health = 100;
    Weapon playerWeapon = Weapon();
    playerWeapon.curAmmo = 24;
    playerWeapon.maxAmmo = 24;
    playerWeapon.damage = 1;
    player.weapon = playerWeapon;
    player.hitRadius = 8;

    enemy.hitRadius = 8;


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



    SDL_Surface* stonewallTexture = IMG_Load("stonewall.png");
    Uint32 pf = SDL_GetWindowPixelFormat(screen);

    textureData stonewallData = getTextureData(stonewallTexture, pf, renderer);

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

    bool change = true;

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    while (!(event.type == SDL_QUIT))
    {
        SDL_Delay(1000 / 60);
        SDL_PollEvent(&event);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);

        

        if (state[SDL_SCANCODE_LEFT]) {
            player.angle -= 5;
            change = true;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            player.angle += 5;
            change = true;
        }
        if (state[SDL_SCANCODE_UP]) {
            change = true;

            float dirX = -1.0 * sin(player.angle * 3.14 / 180);
            float dirY = cos(player.angle * 3.14 / 180);

            if (MAP[(player.worldPosY + dirY * 2) / TILE_SIZE][(player.worldPosX + dirX * 2) / TILE_SIZE] == 0)
            {
                bool hit = false;
                for (int e = 0; e < enemies.size(); e++)
                {
                    if (enemies[e].type == 'z')
                    {
                        if (circleCollision({ player.worldPosX + dirX * 2,player.worldPosY + dirY * 2 }, { (float)enemies[e].worldPosX,(float)enemies[e].worldPosY }, player.hitRadius, enemies[e].hitRadius)) {
                            hit = true;
                        }
                    }
                }
                if (!hit) {
                    player.worldPosX += dirX * 2;
                    player.worldPosY += dirY * 2;
                    player.posY = player.worldPosY / TILE_SIZE;
                    player.posX = player.worldPosX / TILE_SIZE;
                }
            }
        }
        if (state[SDL_SCANCODE_DOWN]) {
            change = true;

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
            change = true;
            if (!fired)
            {
                if (player.weapon.curAmmo > 0)
                {
                    Mix_PlayChannel(-1, fireSoundEffect, 0);
                    fireEffectShown = true;
                    player.weapon.curAmmo -= 1;

                    for (int r = 0; r < enemies.size(); r++)
                    {
                        if (enemies[r].type == 'z')
                        {
                            SDL_Rect* curRect = &enemies[r].rect;
                            SDL_Point point;
                            point.x = centerPos[0];
                            point.y = centerPos[1];
                            SDL_Point* constPoint = &point;
                            if (SDL_PointInRect(constPoint, curRect)) {
                                enemies[r].health -= player.weapon.damage;
                                if (enemies[r].health <= 0) {
                                    enemies[r].type = 'd';
                                }
                            }
                        }
                    }

                }
                else
                {
                    Mix_PlayChannel(-1, clickSoundEffect, 0);
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

        if (change)
        {
            SDL_RenderClear(renderer);
            cast_rays(*renderer, player, MAP, screen, stonewallData, pf);
            change = false;
        }

        for (int e = 0; e < enemies.size(); e++)
        {
            if (enemies[e].type == 'z')
            {

                Enemy currentEnemy = enemies[e];

                int aB = correctDegrees(angleFrom({ (int)player.worldPosX,(int)player.worldPosY }, { currentEnemy.worldPosX, currentEnemy.worldPosY }));
                int diff = correctDegrees(angleBetweenNoAbs(aB, player.angle));

                if (diff > (180 - player.half_fov - 50) && diff < (180 + player.half_fov + 50))
                {

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

        for (int r = (int)player.hitRadius; r > 0; r--)
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
                for (int r = (int)enemies[t].hitRadius; r > 0; r--)
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
                change = true;
            }
        }

        string m = to_string(player.weapon.curAmmo);

        string mtwo = to_string(player.weapon.maxAmmo);

        string mid = "/";

        string fullMessageStr = m + mid + mtwo;
        const char* message = fullMessageStr.c_str();
        
        SDL_Surface* sText = TTF_RenderText_Solid(ammoFont, message, clrFg);
        
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, sText);

        
        textRenderRect.x = 960 - sText->w;
        textRenderRect.y = 480 - sText->h;
        textRenderRect.w = sText->w;
        textRenderRect.h = sText->h;
        render(textRenderRect, textTexture, renderer);
        SDL_FreeSurface(sText);
        SDL_DestroyTexture(textTexture);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(ammoFont);

    SDL_DestroyWindow(screen);

    SDL_DestroyRenderer(renderer);

    IMG_Quit();

    Mix_FreeMusic(bgm);
    Mix_FreeChunk(fireSoundEffect);
    Mix_FreeChunk(clickSoundEffect);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
