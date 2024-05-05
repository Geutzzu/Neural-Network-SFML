#pragma once


#include <unordered_map>
#include <SFML/Graphics.hpp>

class TextureManager {
private:
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;

    // Private constructor
    TextureManager();

public:
    // Delete copy constructor and assignment operator
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    // Static method that controls the access to the singleton instance
    static TextureManager* getInstance();

    void loadTexture(const std::string& name, const std::string& filename);
    sf::Texture& getTexture(const std::string& name);
    sf::Font& getFont(const std::string& name);
    void loadCardTextures();
    void loadFont(const std::string& name, const std::string& filename);
    void loadFonts();
};