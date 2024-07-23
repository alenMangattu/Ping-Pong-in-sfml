#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

const int WID = sf::VideoMode::getDesktopMode().width;
const int HIG = sf::VideoMode::getDesktopMode().height - 22;

void checkInput(std::vector<bool>& keysPressed);
int checkCollision(const std::vector<sf::RectangleShape>& paddles, sf::CircleShape& circle, int(&p)[2], float& ballX, float& ballY);
void draw(sf::RenderWindow& window, const std::vector<sf::RectangleShape>& paddles, const sf::CircleShape& circle, int* p, sf::Text* text, const sf::RectangleShape& line);
void checkEvent(sf::RenderWindow& window, const sf::Event& event);

int main()
{
    sf::RenderWindow window(sf::VideoMode(WID, HIG), "Pong Game", sf::Style::None);
    window.setFramerateLimit(60);

    sf::Event event;

    sf::CircleShape circle(20);
    circle.setFillColor(sf::Color::White);
    circle.setPosition(WID / 2 - circle.getRadius(), HIG / 2 - circle.getRadius());

    std::vector<sf::RectangleShape> paddles = {
        sf::RectangleShape(sf::Vector2f(WID * 0.025, HIG * 0.2)),
        sf::RectangleShape(sf::Vector2f(WID * 0.025, HIG * 0.2))
    };

    paddles[0].setPosition(0.0f, HIG / 2 - paddles[0].getSize().y / 2);
    paddles[1].setPosition(WID - paddles[1].getSize().x, HIG / 2 - paddles[1].getSize().y / 2);

    paddles[0].setFillColor(sf::Color::Red);
    paddles[1].setFillColor(sf::Color::Blue);

    sf::Font font;
    if (!font.loadFromFile("Inconsolata-Black.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
        return -1;
    }

    int p[2] = { 0, 0 };
    sf::Text text[2];

    sf::RectangleShape line(sf::Vector2f(WID * 0.005, HIG));
    line.setPosition(WID / 2 - line.getSize().x / 2, 0);

    for (int i = 0; i < 2; i++) {
        text[i].setFont(font);
        text[i].setCharacterSize((WID / HIG) * 50);
        text[i].setFillColor(sf::Color::White);
    }
    text[0].setPosition(WID / 2 - text[0].getCharacterSize() * 2, 0);
    text[1].setPosition(WID / 2 + text[0].getCharacterSize(), 0);

    draw(window, paddles, circle, p, text, line);
    sf::sleep(sf::milliseconds(1125));

    float ballX = 9, ballY = 9, negX = 1, negY = 1;
    std::vector<bool> keysPressed(2, false); // W and S keys

    while (window.isOpen()) {
        checkInput(keysPressed);
        checkEvent(window, event);
        // Player paddle control
        if (keysPressed[0] && paddles[0].getPosition().y > 0) {
            paddles[0].move(0, -15);
        }
        if (keysPressed[1] && paddles[0].getPosition().y < HIG - paddles[0].getSize().y) {
            paddles[0].move(0, 15);
        }

        // Adjust paddle[1] (AI) to follow the ball
        float paddle1Y = paddles[1].getPosition().y;
        float paddleHeight = paddles[1].getSize().y;
        float ballYPos = circle.getPosition().y + circle.getRadius();

        if (circle.getPosition().x > WID / 2)
            if (ballYPos < paddle1Y + paddleHeight / 2 && paddles[1].getPosition().y > 0) {
                paddles[1].move(0, -5 - (ballY * 0.5) ); // Move up
            } else if (ballYPos > paddle1Y + paddleHeight / 2 && paddles[1].getPosition().y < HIG - paddleHeight) {
                paddles[1].move(0, 5 + (ballY * 0.5)); // Move down
            }

        // Ball collision with top/bottom walls
        if (circle.getPosition().y <= 0 || circle.getPosition().y >= HIG - circle.getRadius() * 2)
            negY *= -1;

        // Check for paddle collisions and handle scoring
        int collision = checkCollision(paddles, circle, p, ballX, ballY);
        if (collision == 1)
            negX *= -1;
        else if (collision == 2) {
            ballX = 9, ballY = 9;
            circle.setPosition(WID / 2 - circle.getRadius(), HIG / 2 - circle.getRadius());
            paddles[0].setPosition(0.0f, HIG / 2 - paddles[0].getSize().y / 2);
            paddles[1].setPosition(WID - paddles[1].getSize().x, HIG / 2 - paddles[1].getSize().y / 2);
            draw(window, paddles, circle, p, text, line);
            sf::sleep(sf::milliseconds(1125));
        }

        // Update ball position
        circle.move(ballX * negX, ballY * negY);
        draw(window, paddles, circle, p, text, line);
    }

    return 0;
}

void checkEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.type == sf::Event::Closed)
        window.close();
}

void checkInput(std::vector<bool>& keysPressed)
{
    
    // W key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        keysPressed[0] = true;
    } else {
        keysPressed[0] = false;
    }

    // S key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        keysPressed[1] = true;
    } else {
        keysPressed[1] = false;
    }
}

int checkCollision(const std::vector<sf::RectangleShape>& paddles, sf::CircleShape& circle, int(&p)[2], float& ballX, float& ballY)
{
    if (circle.getPosition().x <= paddles[0].getPosition().x + paddles[0].getSize().x &&
        circle.getPosition().y >= paddles[0].getPosition().y - circle.getRadius() &&
        circle.getPosition().y <= paddles[0].getPosition().y + paddles[0].getSize().y + circle.getRadius()) {
        ballX++;
        ballY++;
        circle.setPosition(sf::Vector2f(paddles[0].getSize().x, circle.getPosition().y));
        std::cout << "Circle position: x, y: " << circle.getPosition().x << ", " << circle.getPosition().y << std::endl;
        return 1;
    }

    if (circle.getPosition().x + circle.getRadius() * 2 >= paddles[1].getPosition().x &&
        circle.getPosition().y >= paddles[1].getPosition().y - circle.getRadius() &&
        circle.getPosition().y <= paddles[1].getPosition().y + paddles[1].getSize().y + circle.getRadius()) {
        ballX += 0.2;
        ballY += 0.2;
        circle.setPosition(sf::Vector2f(WID - (paddles[1].getSize().x + circle.getRadius() * 2), circle.getPosition().y));
        std::cout << "Circle position: x, y: " << circle.getPosition().x << ", " << circle.getPosition().y << std::endl;
        return 1;
    }

    if (circle.getPosition().x < 0) {
        p[1]++;
        return 2;
    }
    else if (circle.getPosition().x > WID) {
        p[0]++;
        return 2;
    }
    return false;
}

void draw(sf::RenderWindow& window, const std::vector<sf::RectangleShape>& paddles, const sf::CircleShape& circle, int* p, sf::Text* text, const sf::RectangleShape& line)
{
    text[0].setString(std::to_string(p[0]));
    text[1].setString(std::to_string(p[1]));

    window.clear();
    window.draw(circle);
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(text[0]);
    window.draw(text[1]);
    window.draw(line);
    window.display();
}