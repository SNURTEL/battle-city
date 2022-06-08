#include "include/ActiveState_dir/Frame_dir/GameInfoGraphic.h"
#include "include/TextureLoadingError.h"
#include <sstream>

GameInfoGraphic::GameInfoGraphic(const WindowView& windowView, const FrameGraphic::GameInfo& gameInfo)
: AbstractWindow(windowView), gameInfo(gameInfo)
{loadFont();}


void GameInfoGraphic::render()
{
    float offsetLeft = windowView.leftOffset;
    float offsetTop = windowView.topOffset;
    int playerLives = *gameInfo.playerLives;
    std::stringstream ss;
    if (*gameInfo.points < 100)
        ss << "  \tPoints" << "\n\t\t" << std::to_string(*gameInfo.points) << "00";
    else
        ss << "  \tPoints" << "\n\t\t" << std::to_string(*gameInfo.points);

    std::string renderText = ss.str();
    this->renderText(renderText, offsetLeft, offsetTop);

    // To clear ss
    ss.clear();
    ss.str("");

    ss << "   \tLevel:" << "\n   \t\t" << std::to_string(*gameInfo.level);
    renderText = ss.str();
    offsetTop += 100;
    this->renderText(renderText, offsetLeft, offsetTop);

    // To clear ss
    ss.clear();
    ss.str("");

    ss << "Player's lives:" << "\n\t\t   " << std::to_string(playerLives);
    renderText = ss.str();
    offsetTop += 100;
    this->renderText(renderText, offsetLeft, offsetTop);
}


void GameInfoGraphic::renderText(std::string renderText, float leftOffset, float topOffset)
{
    sf::Text text;
    text.setFont(font);
    text.setString(renderText);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::Black);
    text.setPosition(leftOffset, topOffset);
    windowView.window->draw(text);
}


void GameInfoGraphic::loadFont()
{
    std::string path = "../src/graphic-lib/fontFolder/CharisSIL-Regular.ttf";
    if (!font.loadFromFile(path))
    {
        std::stringstream ss;
        ss << "Failed to load font: " << path;
        throw TextureLoadingError(ss.str());
    }

}