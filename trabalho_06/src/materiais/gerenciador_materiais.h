#ifndef GERENCIADOR_MATERIAIS_H
#define GERENCIADOR_MATERIAIS_H

#include "material.h"
#include <vector>
#include <map>
#include <cstddef>

enum class CategoriaObjeto {
    ESFERA,
    CONE,
    CUBO,
    PLANO
};

class GerenciadorMateriais {
public:
    static GerenciadorMateriais& obterInstancia();

    // Obtém o material ativo para uma categoria
    const Material& obterMaterialAtivo(CategoriaObjeto categoria) const;

    // Define o material ativo (preset) para uma categoria pelo índice da lista compartilhada
    void definirMaterialAtivo(CategoriaObjeto categoria, std::size_t indicePreset);

    // Ajusta o brilho do material ativo de uma categoria
    void ajustarBrilho(CategoriaObjeto categoria, float delta);
    float obterBrilho(CategoriaObjeto categoria) const;

    // Lista de presets didáticos (Plástico, Metal, Borracha, Cerâmica)
    std::size_t quantidadePresets() const;
    const char* obterNomePreset(std::size_t indice) const;
    const char* obterDescricaoPreset(std::size_t indice) const;

    // Retorna o nome da categoria para a UI
    static const char* obterNomeCategoria(CategoriaObjeto categoria);

private:
    GerenciadorMateriais(); // Singleton

    struct Preset {
        const char* nome;
        const char* descricao;
        Material material;
    };

    std::vector<Preset> presets;
    std::map<CategoriaObjeto, Material> materiaisAtivos;
};

#endif
