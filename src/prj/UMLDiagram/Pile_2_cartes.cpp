#include "Pile_2_cartes.h"
#include <algorithm>
#include <random>
#include <stdexcept>
#include "Carte.h"

namespace UMLDiagram {
    Pile_2_cartes::Pile_2_cartes () = default;
    
    void Pile_2_cartes::shuffle (){
        std::random_device rd;
        std::default_random_engine rng(rd());
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    void Pile_2_cartes::add (std::unique_ptr<Carte> card){
        cards.push_back(std::move(card));
    }

    std::unique_ptr<Carte> Pile_2_cartes::remove (std::size_t card_index){
        if (card_index >= cards.size()) {
            throw std::out_of_range("Indice de carte invalide.");
        }
        std::unique_ptr<Carte> carte_retiree = std::move(cards[card_index]);
        cards.erase(cards.begin() + card_index);
        return carte_retiree;
    }

    std::size_t Pile_2_cartes::size() const { 
            return cards.size(); 
        }

    const Carte& Pile_2_cartes::at(std::size_t index) const {
            return *cards.at(index);
        }
}