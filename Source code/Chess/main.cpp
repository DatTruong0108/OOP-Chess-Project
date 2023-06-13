#include "ChessBoard.h"
#include <time.h>

int main()
{
    srand(time(NULL));
    RenderWindow menu(VideoMode(1600, 900), "Chess Game");
    // set background
    Image background;
    background.loadFromFile("../resources/images/interface/menu.png");
    Texture textureBackground;
    textureBackground.loadFromImage(background);
    textureBackground.setSmooth(true);
    Sprite spriteBackground(textureBackground);
    spriteBackground.setScale(1600 / spriteBackground.getLocalBounds().width, 900 / spriteBackground.getLocalBounds().height);
    // set icon for window 
    Image icon;
    icon.loadFromFile("../resources/images/interface/icon.png");
    menu.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    RectangleShape onePlayerButton(sf::Vector2f(333.8338, 93.6033));
    onePlayerButton.setPosition(168.863, 423.532); // 168.863 - 423.532: one player button
    Image onePlayer;
    onePlayer.loadFromFile("../resources/images/interface/oneplayerbutton.png");
    Texture textureOneplayer;
    textureOneplayer.loadFromImage(onePlayer);
    textureOneplayer.setSmooth(true);
    onePlayerButton.setTexture(&textureOneplayer);

    // two player button
    RectangleShape twoPlayerButton(sf::Vector2f(333.8338, 93.6033));
    twoPlayerButton.setPosition(168.863, 559.56); // 168.863 - 559.56: two player button
    Image twoPlayer;
    twoPlayer.loadFromFile("../resources/images/interface/twoplayersbutton.png");
    Texture textureTwoplayer;
    textureTwoplayer.loadFromImage(twoPlayer);
    textureTwoplayer.setSmooth(true);
    twoPlayerButton.setTexture(&textureTwoplayer);

    // exit button
    RectangleShape exitButton(sf::Vector2f(333.8338, 93.6033));
    exitButton.setPosition(168.863, 695.588); // 168.863 - 695.588: exit button
    Image exit;
    exit.loadFromFile("../resources/images/interface/exitbutton.png");
    Texture textureExit;
    textureExit.loadFromImage(exit);
    textureExit.setSmooth(true);
    exitButton.setTexture(&textureExit);

    while (menu.isOpen())
    {
        Event event;
        while (menu.pollEvent(event))
        {
            if (event.type == Event::Closed)
                menu.close();
            else if (event.type == Event::MouseButtonPressed)
            {
                if (onePlayerButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    menu.close();
                    RenderWindow oneplayermode(VideoMode(1600, 900), "One player mode");
                    Image oneplayermodebackground;
                    oneplayermodebackground.loadFromFile("../resources/images/interface/oneplayermode.png");
                    Texture textureOneplayermodebackground;
                    textureOneplayermodebackground.loadFromImage(oneplayermodebackground);
                    textureOneplayermodebackground.setSmooth(true);
                    Sprite spriteOneplayermodebackground(textureOneplayermodebackground);
                    spriteOneplayermodebackground.setScale(1600 / spriteOneplayermodebackground.getLocalBounds().width, 900 / spriteOneplayermodebackground.getLocalBounds().height);
                    oneplayermode.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

                    // easy button
                    RectangleShape easyButton(Vector2f(505.5901, 155.6937));
                    easyButton.setPosition(304.56, 416.56); // 304.56 - 416.56: easy button
                    Image easy;
                    easy.loadFromFile("../resources/images/interface/oneplayermode_easy.png");
                    Texture textureEasy;
                    textureEasy.loadFromImage(easy);
                    textureEasy.setSmooth(true);
                    easyButton.setTexture(&textureEasy);

                    // hard button
                    RectangleShape hardButton(Vector2f(505.5901, 155.6937));
                    hardButton.setPosition(890.48, 416.56); // 890.48 - 416.56: hard button
                    Image hard;
                    hard.loadFromFile("../resources/images/interface/oneplayermode_hard.png");
                    Texture textureHard;
                    textureHard.loadFromImage(hard);
                    textureHard.setSmooth(true);
                    hardButton.setTexture(&textureHard);

                    // resume button
                    RectangleShape resumeButton(Vector2f(505.5901, 155.6937));
                    resumeButton.setPosition(597.48, 675); // 597.48 - 675: resume button
                    Image resume;
                    resume.loadFromFile("../resources/images/interface/oneplayermode_resume.png");
                    Texture textureResume;
                    textureResume.loadFromImage(resume);
                    textureResume.setSmooth(true);
                    resumeButton.setTexture(&textureResume);

                    while (oneplayermode.isOpen()) {
                        Event event;
                        while (oneplayermode.pollEvent(event))
                        {
                            if (event.type == Event::Closed)
                                oneplayermode.close();
                            else if (event.type == Event::MouseButtonPressed)
                            {
                                if (easyButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                                {
                                    oneplayermode.close();
                                    try {
                                        string name = "Chess game";

                                        RenderWindow window(VideoMode(1600, 900), name);

                                        window.setFramerateLimit(120);
                                        window.setTitle(name);
                                        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

                                        ChessBoard chessboard(name, window, true);

                                        chessboard.run();
                                    }
                                    catch (const std::exception& e) {
                                        std::cerr << e.what() << std::endl;
                                        return EXIT_FAILURE;
                                    }
                                }
                                else if (hardButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                                {
                                    oneplayermode.close();
                                    try {
                                        string name = "Chess game";

                                        RenderWindow window(VideoMode(1600, 900), name);

                                        window.setFramerateLimit(120);
                                        window.setTitle(name);
                                        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

                                        ChessBoard chessboard(name, window, true);

                                        chessboard.run();
                                    }
                                    catch (const std::exception& e) {
                                        std::cerr << e.what() << std::endl;
                                        return EXIT_FAILURE;
                                    }
                                }
                                else if (resumeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                                {
                                    oneplayermode.close();
                                    try {
                                        string name = "Chess game";

                                        RenderWindow window(VideoMode(1600, 900), name);

                                        window.setFramerateLimit(120);
                                        window.setTitle(name);
                                        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

                                        ChessBoard chessboard(name, window, true);

                                        chessboard.runResume();
                                    }
                                    catch (const std::exception& e) {
                                        std::cerr << e.what() << std::endl;
                                        return EXIT_FAILURE;
                                    }
                                }
                            }
                        }
                        oneplayermode.clear();

                        oneplayermode.draw(spriteOneplayermodebackground);
                        oneplayermode.draw(easyButton);
                        oneplayermode.draw(resumeButton);
                        oneplayermode.draw(hardButton);

                        oneplayermode.display();
                    }
                }
                else if (twoPlayerButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    menu.close();
                    RenderWindow twoplayersmode(VideoMode(1600, 900), "Two players mode");
                    Image oneplayermodebackground;
                    oneplayermodebackground.loadFromFile("../resources/images/interface/twoplayersmode.png");
                    Texture textureOneplayermodebackground;
                    textureOneplayermodebackground.loadFromImage(oneplayermodebackground);
                    textureOneplayermodebackground.setSmooth(true);
                    Sprite spriteOneplayermodebackground(textureOneplayermodebackground);
                    spriteOneplayermodebackground.setScale(1600 / spriteOneplayermodebackground.getLocalBounds().width, 900 / spriteOneplayermodebackground.getLocalBounds().height);
                    twoplayersmode.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

                    // new game button
                    RectangleShape newGameButton(Vector2f(596.7305, 164.7661));
                    newGameButton.setPosition(552.32, 322.43); // 552.32 - 322.43: new game button
                    Image newgame;
                    newgame.loadFromFile("../resources/images/interface/twoplayersmode_new.png");
                    Texture textureNewgame;
                    textureNewgame.loadFromImage(newgame);
                    textureNewgame.setSmooth(true);
                    newGameButton.setTexture(&textureNewgame);

                    // resume button
                    RectangleShape resumeButton(Vector2f(596.7305, 164.766));
                    resumeButton.setPosition(552.32, 545); // 552.32 - 545: resume button
                    Image resume;
                    resume.loadFromFile("../resources/images/interface/twoplayersmode_resume.png");
                    Texture textureResume;
                    textureResume.loadFromImage(resume);
                    textureResume.setSmooth(true);
                    resumeButton.setTexture(&textureResume);

                    while (twoplayersmode.isOpen()) {
                        Event event;
                        while (twoplayersmode.pollEvent(event))
                        {
                            if (event.type == Event::Closed)
                                twoplayersmode.close();
                            else if (event.type == Event::MouseButtonPressed)
                            {
                                if (newGameButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                                {
                                    twoplayersmode.close();
                                    try {
                                        string name = "Chess game";

                                        RenderWindow window(VideoMode(1600, 900), name);

                                        window.setFramerateLimit(120);
                                        window.setTitle(name);
                                        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

                                        ChessBoard chessboard(name, window, false);

                                        chessboard.run();
                                    }
                                    catch (const std::exception& e) {
                                        std::cerr << e.what() << std::endl;
                                        return EXIT_FAILURE;
                                    }
                                }
                                else if (resumeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                                {
                                    twoplayersmode.close();
                                    try {
                                        string name = "Chess game";

                                        RenderWindow window(VideoMode(1600, 900), name);

                                        window.setFramerateLimit(120);
                                        window.setTitle(name);
                                        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

                                        ChessBoard chessboard(name, window, false);

                                        chessboard.runResume();
                                    }
                                    catch (const std::exception& e) {
                                        std::cerr << e.what() << std::endl;
                                        return EXIT_FAILURE;
                                    }
                                }
                            }
                        }
                        twoplayersmode.clear();

                        twoplayersmode.draw(spriteOneplayermodebackground);
                        twoplayersmode.draw(newGameButton);
                        twoplayersmode.draw(resumeButton);

                        twoplayersmode.display();
                    }
                }
                else if (exitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    menu.close();
                }
            }
        }

        menu.clear();

        menu.draw(spriteBackground);
        menu.draw(onePlayerButton);
        menu.draw(twoPlayerButton);
        menu.draw(exitButton);

        menu.display();
    }

    return 0;
}
