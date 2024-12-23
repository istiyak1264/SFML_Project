#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

typedef long long ll;

class Game
{
private:
    Texture bg_texture, ground_texture, man_textures[6], jump_textures[7], coin_texture, food_texture;
    Texture obstacle_textures[3], splash_texture;
    Sprite bg_sprite1, bg_sprite2, ground_sprite1, ground_sprite2, man_sprite, obstacle_sprite, coin_sprite, food_sprite;
    Sprite splash_sprite;
    Font font;
    Text score_text;
    const int ground_speed = 1000, background_speed = ground_speed / 2;
    ll score = 0;
    bool jumping = false, coin_visible = true, food_visible = true, game_started = false;
    int anim_counter = 0, jump_counter = 0;
    float man_velocity_y = 0;
    const float gravity = 2500;
    const float jump_strength = -1500;
    Clock anim_clock, coin_food_clock;
    bool game_over = false;
    RectangleShape restart_button, quit_button, start_button;
    Text game_over_text, restart_text, quit_text, start_text;

public:
    RenderWindow &win;
    Game(RenderWindow &);
    void StartGameLoop();
    void moveBackgroundAndGround(Time &);
    void handleEvents();
    void update(Time &);
    void draw();
    void resetObstacle();
    void resetCoinAndFood();
    void startMenu();
};

Game::Game(RenderWindow &window) : win(window)
{
    splash_texture.loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/GUI_Background.png");
    splash_sprite.setTexture(splash_texture);
    splash_sprite.setScale(
        (float)win.getSize().x / splash_sprite.getGlobalBounds().width,
        (float)win.getSize().y / splash_sprite.getGlobalBounds().height);

    bg_texture.loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/Background.jpg");
    bg_sprite1.setTexture(bg_texture);
    bg_sprite2.setTexture(bg_texture);
    bg_sprite1.setPosition(0, 0);
    bg_sprite2.setPosition(bg_sprite1.getGlobalBounds().width, 0);

    ground_texture.loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/Ground.jpg");
    ground_sprite1.setTexture(ground_texture);
    ground_sprite2.setTexture(ground_texture);
    ground_sprite1.setPosition(0, 700);
    ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().width, 700);

    for (int i = 0; i < 6; i++)
        man_textures[i].loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/run" + to_string(i + 1) + ".png");

    for (int i = 0; i < 7; i++)
        jump_textures[i].loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/jump" + to_string(i + 1) + ".png");

    obstacle_textures[0].loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/obstacle1.png");
    obstacle_textures[1].loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/obstacle2.png");
    obstacle_textures[2].loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/obstacle3.png");

    coin_texture.loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/coin.png");
    food_texture.loadFromFile("/home/istiyak/Desktop/C++_Project/Assets/food.png");

    resetObstacle();
    resetCoinAndFood();

    font.loadFromFile("/home/istiyak/Desktop/C++_Project/SourGummy-Black.ttf");
    score_text.setFont(font);
    score_text.setCharacterSize(40);
    score_text.setFillColor(Color::Black);
    score_text.setPosition(20, 20);
    score_text.setString("Score: 0");

    man_sprite.setTexture(man_textures[0]);
    man_sprite.setPosition(200, 500);
    man_sprite.setScale(0.5f, 0.5f);

    game_over_text.setFont(font);
    game_over_text.setCharacterSize(60);
    game_over_text.setFillColor(Color::Red);
    game_over_text.setString("Game Over!");

    restart_button.setSize(Vector2f(200, 60));
    restart_button.setFillColor(Color::Green);

    quit_button.setSize(Vector2f(200, 60));
    quit_button.setFillColor(Color::Red);

    restart_text.setFont(font);
    restart_text.setCharacterSize(30);
    restart_text.setFillColor(Color::White);
    restart_text.setString("Restart");

    quit_text.setFont(font);
    quit_text.setCharacterSize(30);
    quit_text.setFillColor(Color::White);
    quit_text.setString("Quit");

    start_button.setSize(Vector2f(200, 60));
    start_button.setFillColor(Color::Blue);

    start_text.setFont(font);
    start_text.setCharacterSize(30);
    start_text.setFillColor(Color::White);
    start_text.setString("Start");

    Vector2f windowCenter(win.getSize().x / 2, win.getSize().y / 2);
    game_over_text.setPosition(windowCenter.x - 150, windowCenter.y - 150);

    restart_button.setPosition(windowCenter.x - 100, windowCenter.y-30);
    restart_text.setPosition(windowCenter.x - 50, windowCenter.y -20);

    quit_button.setPosition(windowCenter.x - 100, windowCenter.y + 70);
    quit_text.setPosition(windowCenter.x - 50, windowCenter.y + 80);

    start_button.setPosition(windowCenter.x - 100, windowCenter.y - 50);
    start_text.setPosition(windowCenter.x - 50, windowCenter.y - 40);
}

void Game::resetObstacle()
{
    int randomIndex = rand() % 3;
    obstacle_sprite.setTexture(obstacle_textures[randomIndex]);
    obstacle_sprite.setScale(1.5f, 1.5f);
    obstacle_sprite.setPosition(1400 + rand() % 300, 400);
}

void Game::resetCoinAndFood()
{
    coin_visible = true;
    food_visible = true;

    coin_sprite.setTexture(coin_texture);
    coin_sprite.setScale(0.3f, 0.3f);
    coin_sprite.setPosition(1400 + rand() % 500, 500);

    food_sprite.setTexture(food_texture);
    food_sprite.setScale(0.3f, 0.3f);
    food_sprite.setPosition(1400 + rand() % 500, 500);
}

void Game::startMenu()
{
    if (!game_started)
    {
        win.clear(Color::White);

        win.draw(splash_sprite);

        win.draw(start_button);
        win.draw(start_text);
        win.draw(quit_button);
        win.draw(quit_text);

        win.display();
    }
}

void Game::StartGameLoop()
{
    Clock clock;
    while (win.isOpen())
    {
        Time dt = clock.restart();
        handleEvents();
        if (game_started)
        {
            update(dt);
            draw();
        }
        else
        {
            startMenu();
        }
        win.display();
    }
}

void Game::handleEvents()
{
    Event event;
    while (win.pollEvent(event))
    {
        if (event.type == Event::Closed)
            win.close();

        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::C)
                win.close();
            if (event.key.code == Keyboard::Space && !jumping && !game_over && game_started)
            {
                jumping = true;
                man_velocity_y = jump_strength;
            }
        }

        if (game_over)
        {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
                if (restart_button.getGlobalBounds().contains(mouse_pos))
                {
                    game_over = false;
                    score = 0;
                    score_text.setString("Score: 0");
                    resetObstacle();
                    resetCoinAndFood();
                }
                else if (quit_button.getGlobalBounds().contains(mouse_pos))
                {
                    win.close();
                }
            }
        }

        if (!game_started)
        {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
                if (start_button.getGlobalBounds().contains(mouse_pos))
                {
                    game_started = true;
                    resetObstacle();
                    resetCoinAndFood();
                }
                else if (quit_button.getGlobalBounds().contains(mouse_pos))
                {
                    win.close();
                }
            }
        }
    }
}

void Game::update(Time &dt)
{
    if (game_over)
        return;

    moveBackgroundAndGround(dt);

    man_velocity_y += gravity * dt.asSeconds();
    man_sprite.move(0, man_velocity_y * dt.asSeconds());

    if (man_sprite.getPosition().y >= 500)
    {
        man_sprite.setPosition(200, 500);
        jumping = false;
    }

    if (anim_clock.getElapsedTime().asMilliseconds() > 100)
    {
        if (jumping)
        {
            jump_counter++;
            man_sprite.setTexture(jump_textures[min(jump_counter / 2, 6)]);
        }
        else
        {
            anim_counter = (anim_counter + 1) % 6;
            man_sprite.setTexture(man_textures[anim_counter]);
        }
        anim_clock.restart();
    }

    if (jump_counter / 2 >= 6)
        jump_counter = 0;

    obstacle_sprite.move(-ground_speed * dt.asSeconds(), 0);
    if (obstacle_sprite.getPosition().x < -100)
    {
        resetObstacle();
        score += 50;
    }

    if (coin_visible)
        coin_sprite.move(-ground_speed * dt.asSeconds(), 0);
    if (coin_sprite.getPosition().x < -100)
        coin_visible = false;

    if (food_visible)
        food_sprite.move(-ground_speed * dt.asSeconds(), 0);
    if (food_sprite.getPosition().x < -100)
        food_visible = false;

    if (man_sprite.getGlobalBounds().intersects(obstacle_sprite.getGlobalBounds()))
    {
        game_over = true;
    }

    if (man_sprite.getGlobalBounds().intersects(coin_sprite.getGlobalBounds()) && coin_visible)
    {
        score += 10;
        coin_visible = false;
    }

    if (man_sprite.getGlobalBounds().intersects(food_sprite.getGlobalBounds()) && food_visible)
    {
        score += 10;
        food_visible = false;
    }

    score_text.setString("Score: " + to_string(score));
}

void Game::moveBackgroundAndGround(Time &dt)
{
    bg_sprite1.move(-background_speed * dt.asSeconds(), 0);
    bg_sprite2.move(-background_speed * dt.asSeconds(), 0);
    ground_sprite1.move(-ground_speed * dt.asSeconds(), 0);
    ground_sprite2.move(-ground_speed * dt.asSeconds(), 0);

    if (bg_sprite1.getPosition().x + bg_sprite1.getGlobalBounds().width <= 0)
        bg_sprite1.setPosition(bg_sprite2.getPosition().x + bg_sprite2.getGlobalBounds().width, 0);

    if (bg_sprite2.getPosition().x + bg_sprite2.getGlobalBounds().width <= 0)
        bg_sprite2.setPosition(bg_sprite1.getPosition().x + bg_sprite1.getGlobalBounds().width, 0);

    if (ground_sprite1.getPosition().x + ground_sprite1.getGlobalBounds().width <= 0)
        ground_sprite1.setPosition(ground_sprite2.getPosition().x + ground_sprite2.getGlobalBounds().width, 700);

    if (ground_sprite2.getPosition().x + ground_sprite2.getGlobalBounds().width <= 0)
        ground_sprite2.setPosition(ground_sprite1.getPosition().x + ground_sprite1.getGlobalBounds().width, 700);
}

void Game::draw()
{
    win.clear(Color::White);

    win.draw(bg_sprite1);
    win.draw(bg_sprite2);
    win.draw(ground_sprite1);
    win.draw(ground_sprite2);
    win.draw(man_sprite);

    if (coin_visible)
        win.draw(coin_sprite);
    if (food_visible)
        win.draw(food_sprite);

    win.draw(obstacle_sprite);

    win.draw(score_text);

    if (game_over)
    {
        win.draw(game_over_text);
        win.draw(restart_button);
        win.draw(restart_text);
        win.draw(quit_button);
        win.draw(quit_text);
    }
}

int main()
{
    srand(time(0));
    VideoMode desktop = VideoMode::getDesktopMode();
    int screenWidth = desktop.width, screenHeight = desktop.height;
    int windowWidth = 1350, windowHeight = 900;

    RenderWindow window(VideoMode(windowWidth, windowHeight), "", Style::None);
    window.setPosition(Vector2i((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2));

    Game game(window);
    game.StartGameLoop();
    return 0;
}
