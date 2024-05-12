
#include "TextureManager.h"
#include <iostream>



TextureManager::TextureManager() {}

TextureManager* TextureManager::getInstance() {
    static TextureManager instance; // Guaranteed to be destroyed, instantiated on first use
    return &instance;
}

void TextureManager::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        std::cout << "Failed to load texture: " + filename << std::endl;
    }
    else std::cout << "Loaded texture: " + filename << std::endl;
    textures[name] = texture;
}

sf::Texture& TextureManager::getTexture(const std::string& name) {
    return textures.at(name);
}

sf::Font& TextureManager::getFont(const std::string& name) {
    return fonts.at(name);
}

void TextureManager::loadCardTextures() {
    std::string suits[] = { "hearts", "diamonds", "clubs", "spades" };
    std::string ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king", "ace" };

    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            std::string path = "Resources/Cards/" + rank + "_of_" + suit + ".png";
            sf::Texture texture;
            if (!texture.loadFromFile(path)) {
                std::cout << "Failed to load texture: " + path << std::endl;
            }
            else std::cout << "Loaded texture: " + path << std::endl;
            textures[rank + "_of_" + suit] = texture;
        }
    }
    std::string path = "Resources/Cards/back.png";
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cout << "Failed to load texture: " + path << std::endl;
    }
    textures["back"] = texture;
}

void TextureManager::loadFont(const std::string& name, const std::string& filename) {
    sf::Font font;
    if (!font.loadFromFile(filename)) {
        std::cout << "Failed to load font: " + filename << std::endl;
    }
    else std::cout << "Loaded font: " + filename << std::endl;
    fonts[name] = font;
}

void TextureManager::loadFonts() {
    loadFont("roboto", "Resources/Roboto-Black.ttf");
}