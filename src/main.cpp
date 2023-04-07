#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

int WID = sf::VideoMode::getDesktopMode().width;
int HIG = sf::VideoMode::getDesktopMode().height;


void checkEvent(sf::RenderWindow& window, const sf::Event& event, std::vector<sf::RectangleShape>& paddles);
int checkCollision(const std::vector<sf::RectangleShape>& paddles, sf::CircleShape& circle, int(&p)[2], float& ballX, float& ballY);
void draw(sf::RenderWindow& window, const std::vector<sf::RectangleShape>& paddles, const sf::CircleShape& circle, int* p, sf::Text* text, const sf::RectangleShape& line);

int main()
{
    sf::RenderWindow window(sf::VideoMode(WID, HIG), "Pong Game", sf::Style::Fullscreen);
    sf::Event event;
    window.setFramerateLimit(60);

    sf::CircleShape circle;
    circle.setRadius(20);
    circle.setFillColor(sf::Color::White);
    circle.setPosition(WID / 2 - circle.getRadius(), HIG / 2 - circle.getRadius());

    std::vector<sf::RectangleShape> paddles = {
        sf::RectangleShape(sf::Vector2f(WID * 0.025, HIG * 0.2)),
        sf::RectangleShape(sf::Vector2f(WID * 0.025, HIG * 0.2)),
    };

    paddles[0].setPosition(0.0f, HIG / 2 - paddles[0].getSize().y / 2);
    paddles[1].setPosition(WID - paddles[1].getSize().x, HIG / 2 - paddles[0].getSize().y / 2);

    paddles[0].setFillColor(sf::Color::Red);
    paddles[1].setFillColor(sf::Color::Blue);

    sf::Font font;
    int p[2] = {0, 0};
    sf::Text text[2];

    sf::RectangleShape line(sf::Vector2f(WID * 0.005, HIG));
    line.setPosition(WID / 2 - line.getSize().x / 2, 0);

    for (int i = 0; i < 2; i++){
        text[i].setFont(font);
        text[i].setCharacterSize((WID / HIG) * 50);
        text[i].setFillColor(sf::Color::White);
    }
    text[0].setPosition(WID / 2 - text[0].getCharacterSize() * 2, 0);
    text[1].setPosition(WID / 2 + text[0].getCharacterSize(), 0);

    if (!font.loadFromFile("Inconsolata-Black.ttf"))
    std::cout << "Error loading font" << std::endl;

    draw(window, paddles, circle, p, text, line);
    sf::sleep(sf::milliseconds(1125));


    float ballX = 5, ballY = 5, negX = 1, negY = 1;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
            checkEvent(window, event, paddles);

        if ( circle.getPosition().y <= 0 ||
             circle.getPosition().y >= HIG - circle.getRadius() * 2 )
        negY *= -1;

        int collision = checkCollision(paddles, circle, p, ballX, ballY);
        if (collision == 1)
            negX *= -1;
        else if (collision == 2)
        {
            ballX = 5, ballY = 5;
            circle.setPosition(WID / 2 - circle.getRadius(), HIG / 2 - circle.getRadius());
            paddles[0].setPosition(0.0f, HIG / 2 - paddles[0].getSize().y / 2);
            paddles[1].setPosition(WID - paddles[1].getSize().x, HIG / 2 - paddles[1].getSize().y / 2);
            draw(window, paddles, circle, p, text, line);
            sf::sleep(sf::milliseconds(1125));
        }

        circle.setPosition(circle.getPosition().x + (ballX * negX), circle.getPosition().y + (ballY * negY));
        draw(window, paddles, circle, p, text, line);
    }
};

void checkEvent(sf::RenderWindow& window, const sf::Event& event, std::vector<sf::RectangleShape>& paddles)
{
    if (event.type == sf::Event::Closed)
        window.close();
    if (event.type == sf::Event::KeyPressed)
        switch (event.key.code)
        {
        case sf::Keyboard::W:
            if (paddles[0].getPosition().y > 0)
            paddles[0].setPosition(paddles[0].getPosition().x, paddles[0].getPosition().y - 15);
            break;
        case sf::Keyboard::S:
            if (paddles[0].getPosition().y < HIG - paddles[0].getSize().y)
            paddles[0].setPosition(paddles[0].getPosition().x, paddles[0].getPosition().y + 15);
            break;
        case sf::Keyboard::Up:
            if (paddles[1].getPosition().y > 0)
            paddles[1].setPosition(paddles[1].getPosition().x, paddles[1].getPosition().y - 15);
            break;
        case sf::Keyboard::Down:
            if (paddles[1].getPosition().y < HIG - paddles[1].getSize().y)
            paddles[1].setPosition(paddles[1].getPosition().x, paddles[1].getPosition().y + 15);
            break;
        }
}

int checkCollision(const std::vector<sf::RectangleShape>& paddles, sf::CircleShape& circle, int(&p)[2], float& ballX, float& ballY)
{
    if (circle.getPosition().x <= paddles[0].getPosition().x + paddles[0].getSize().x &&
        circle.getPosition().y >= paddles[0].getPosition().y - circle.getRadius() &&
        circle.getPosition().y <= paddles[0].getPosition().y + paddles[0].getSize().y + circle.getRadius())
    {
        ballX++;
        ballY++;
        circle.setPosition(sf::Vector2f(paddles[0].getSize().x, circle.getPosition().y));
        std::cout << "Circle position: x, y: " << circle.getPosition().x << ", "  << circle.getPosition().y << std::endl;
        return 1;
    }

     if (circle.getPosition().x + circle.getRadius() * 2 >= paddles[1].getPosition().x &&
        circle.getPosition().y >= paddles[1].getPosition().y - circle.getRadius() &&
        circle.getPosition().y <= paddles[1].getPosition().y + paddles[1].getSize().y + circle.getRadius())
    {
        ballX+=0.2;
        ballY+=0.2;
        circle.setPosition(sf::Vector2f(WID - (paddles[1].getSize().x + circle.getRadius() * 2), circle.getPosition().y));
        std::cout << "Circle position: x, y: " << circle.getPosition().x << ", "  << circle.getPosition().y << std::endl;
        return 1;
    }

    if (circle.getPosition().x < 0)
    {
        p[1]++;
        return 2;
    }
    else if (circle.getPosition().x > WID)
    {
        p[0]++;
        return 2;
    }
    return false;
}

void draw(sf::RenderWindow& window, const std::vector<sf::RectangleShape>& paddles, const sf::CircleShape& circle, int* p, sf::Text* text, const sf::RectangleShape& line)
{
    text[0].setString(std::to_string((int)p[0] / 2));
    text[1].setString(std::to_string((int)p[1] / 2));

    window.clear();
    window.draw(circle);
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(text[0]);
    window.draw(text[1]);
    window.draw(line);
    window.display();
}
