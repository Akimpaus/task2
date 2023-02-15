#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class Server {
private:
    const sf::IpAddress m_ip;
    const unsigned short m_port;
    sf::TcpSocket m_socket;

public:
    ~Server() {
        m_socket.disconnect();
    }

    Server(const sf::IpAddress& ip, const unsigned short port) :
        m_ip(ip),
        m_port(port) {}

    sf::Socket::Status connect() {
        return m_socket.connect(m_ip, m_port);
    }

    sf::Socket::Status send(const std::string& str) {
        return m_socket.send(str.data(), str.size());
    }
};

class Player : public sf::CircleShape {
private:
    float m_speed;

public:
    using sf::CircleShape::CircleShape;

    [[nodiscard]] float getSpeed() const noexcept {
        return m_speed;
    }

    void setSpeed(const float speed) noexcept {
        m_speed = speed;
    }
};

template<typename T>
[[nodiscard]] std::string coordsToString(const sf::Vector2<T>& coords) {
    return std::to_string(coords.x) + " " + std::to_string(coords.y);
}

int main(const int argc, const char* argv[]) {
    if (argc != 4) {
        std::cout << "client usage: client <ip> <port> <login>" << std::endl;
        return EXIT_FAILURE;
    }

    const sf::IpAddress ip       = argv[1];
    const std::string portString = argv[2];
    const std::string login      = argv[3];
    unsigned short port          = 0;
    try {
        port = std::stoi(portString);
    } catch (...) {
        std::cout << "client: failed transform string to port" << std::endl;
        return EXIT_FAILURE;
    }

    Server server(ip, port);

    std::cout << "client: attempt to connect to the server" << std::endl;
    if (server.connect() == sf::Socket::Done) {
        std::cout << "client: success connect to the server" << std::endl;
    } else {
        std::cout << "client: failed connect to the server" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "client: attempt to send the login to the server" << std::endl;
    if (server.send(login) == sf::Socket::Done) {
        std::cout << "client: success send login" << std::endl;
    } else {
        std::cout << "client: failed send login" << std::endl;
        return EXIT_FAILURE;
    }

    sf::RenderWindow window({ 800, 600 }, "client");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("font.otf")) {
        std::cout << "client: can't load the font" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Text textInstruction("Use arrows to control the ball.", font, 20);
    sf::Text textCoords;
    textCoords.setFont(font);
    textCoords.setCharacterSize(20);
    textCoords.setPosition(0, 40);
    
    Player player(10);
    player.setFillColor(sf::Color::Red);
    player.setSpeed(0.2);
    player.setPosition(400, 300);

    sf::Clock clock;
    while (window.isOpen()) {
        const sf::Vector2u windowSize = window.getSize();
        const float playerRadius      = player.getRadius();
        const float playerDiametr     = playerRadius * 2;
        const sf::Time frameTime      = clock.restart();
        const float frameMls          = frameTime.asMilliseconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const float playerDist         = player.getSpeed() * frameMls;
        const sf::Vector2 playerOldPos = player.getPosition();
        sf::Vector2f playerNewPos      = playerOldPos;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            playerNewPos += { -playerDist, 0 };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            playerNewPos += { playerDist, 0 };
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            playerNewPos += { 0, -playerDist };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            playerNewPos += { 0, playerDist };
        }

        if (playerNewPos != playerOldPos &&
            playerNewPos.x >= 0 && playerNewPos.y >= 0 &&
            playerNewPos.x + playerDiametr <= windowSize.x &&
            playerNewPos.y + playerDiametr <= windowSize.y) {
            player.setPosition(playerNewPos);
            
            const std::string coordsString = coordsToString(playerNewPos);
            std::cout << "client: attempt to send coords to the server" << std::endl;
            if (server.send(coordsString) == sf::Socket::Done) {
                std::cout << "client: success send coords" << std::endl;
            } else {
                std::cout << "client: failed send coords" << std::endl;
            }
            textCoords.setString("Coords: " + coordsString);
        }

        window.clear();
        window.draw(player);
        window.draw(textInstruction);
        window.draw(textCoords);
        window.display();
    }

    return EXIT_SUCCESS;
}