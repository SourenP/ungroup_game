#ifndef ResourceStore_hpp
#define ResourceStore_hpp

#include <vector>
#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>


class ResourceStore {
 public:
    explicit ResourceStore() {};
    ~ResourceStore() {};
    ResourceStore(const ResourceStore& temp_obj) = delete;
    ResourceStore& operator=(const ResourceStore& temp_obj) = delete;

    void addTexture(std::string key, std::string image_dir);
    sf::Texture& getTexture(std::string key);

 private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> mTextures;
};

#endif /* ResourceStore_hpp */
