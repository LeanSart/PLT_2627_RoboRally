#include "SaveSlot.h"

namespace screens {

    SaveSlot::SaveSlot(const file::path& path, const std::string& label, sf::Vector2f pos, const sf::Font& font)
        : filepath(path), displayName(label), bouton(label, pos, font, {520.f, 65.f}, 26) {}
}