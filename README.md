# PLT

Pour lancer le projet :

Pour build les fichiers d'en-tête faire la commande : cmake --build build (peut-être besoin au préalable de : cmake -B build -S .)
Puis modifier les fichiers d'en-tête suivants : 
    -  Dans cartes.h(ligne 25) : virtual void loadCartes (const std::string& filePath, Pile_2_cartes& stack) = 0;
    -  Dans renderEngine.h(ligne 9) : #include <filesystem>
Puis faire la commande : make clean && make && make run

[![Actions Status](https://github.com/cbares/plt/workflows/PLT%20build/badge.svg)](https://github.com/cbares/plt/actions)

## Dépendances:

```consle
sudo apt install libboost-all-dev
```

## Mise à jour depuis le template:

* Si votre dépôt ne dépend pas du template:
```console
git remote add template https://github.com/cbares/plt
```

* Ensuite:
```console
git fetch --all
```

* puis récupérer les fichiers importants: 
```console
git checkout template/master CMakeLists.txt .github/workflows/main.yml .gitignore cmake extern/dia2code
```
