#include "Pile_2_carte.h"
#include <algorithm>

Pile_2_carte::Pile_2_carte() {}

void shuffle (){
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(cards.begin(), cards.end(), rng);
}

void add (Card card){
    cards.push_back(card);
}

Card remove (int card_index){
    if (card_index < 0 || card_index >= static_cast<int>(cards.size())) {
            throw std::out_of_range("Indice de carte invalide.");
        }
    Carte carte_retiree = cards[card_index];
    cards.erase(cards.begin() + card_index);
    
    return carte_retiree;
}