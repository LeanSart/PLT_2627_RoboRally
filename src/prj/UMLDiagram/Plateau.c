#include "Plateau.h"
#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <filesystem>
#include <nlohmann/json.hpp>

Plateau::Plateau(int ligne, int colonne, Orientation Orientation,const std::filesystem::path& BoardPath)
:ligne(ligne), colonne(colonne), Orientation(Orientation),grille(loadJSONFile(BoardPath)){}

Case_Plateau Plateau::getCase (int ligne, int colonne){return Grille.at(colonne).at(ligne)}

/**
 * Charge un fichier JSON depuis un chemin donné.
 * 
 * @param filePath Chemin vers le fichier JSON
 * @return std::optional<nlohmann::json> Le contenu du JSON ou std::nullopt en cas d'erreur
 */
using json = nlohmann::json;
std::optional<json> loadJSONFile(const std::filesystem::path& filePath){
    // 1. Vérification de l'existence du fichier
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "Erreur : Le fichier n'existe pas : " << filePath << std::endl;
        return std::nullopt;
    }

    // 2. Ouverture du flux de fichier
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier : " << filePath << std::endl;
        return std::nullopt;
    }

    // 3. Lecture et parsing avec gestion des exceptions
    try {
        json data = json::parse(file);
        return data;
    } catch (const json::parse_error& e) {
        std::cerr << "Erreur de parsing JSON : " << e.what() << std::endl;
        return std::nullopt;
    }
}
