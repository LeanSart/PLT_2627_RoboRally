#include "Programme.h"
#include <fstream>
#include <iostream>
#include "Pile_2_cartes.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace UMLDiagram {

    Programme::Programme(int vitesse, TypeMouvement mouvement) 
        : vitesse(vitesse), mouvement(mouvement) {
    }

    int Programme::getVitesse() {
        return vitesse;
    }

    TypeMouvement Programme::getMouvement() {
        return mouvement;
    }

    TypeMouvement Programme::stringToTypeMouvement(const std::string& str) {
        if (str == "DEMI_TOUR")         return TypeMouvement::DEMI_TOUR;
        if (str == "TOURNER_A_GAUCHE")  return TypeMouvement::TOURNER_A_GAUCHE;
        if (str == "TOURNER_A_DROITE")  return TypeMouvement::TOURNER_A_DROITE;
        if (str == "RECULER_1_CASE")    return TypeMouvement::RECULER_1_CASE;
        if (str == "AVANCER_1_CASE")    return TypeMouvement::AVANCER_1_CASE;
        if (str == "AVANCER_2_CASES")   return TypeMouvement::AVANCER_2_CASES;
        if (str == "AVANCER_3_CASES")   return TypeMouvement::AVANCER_3_CASES;
    
        throw std::invalid_argument("Type de mouvement inconnu dans le JSON : " + str);
    }

    void Programme::loadCartes(const std::string& filePath, Pile_2_cartes& stack){
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier : " + filePath);
        }
        try {
            json data = json::parse(file);
            for (const auto& item : data) {
                int vitesse = item.at("vitesse").get<int>();
                std::string mouvStr = item.at("mouvement").get<std::string>();

                TypeMouvement mouvement = stringToTypeMouvement(mouvStr);
                auto card = std::make_unique<Programme>(vitesse, mouvement);
                stack.add(std::move(card));
            }
        } 
        catch (const std::exception& e) {
            std::cerr << "Erreur de lecture du JSON : " << e.what() << std::endl;
        }

        return;
    }
}