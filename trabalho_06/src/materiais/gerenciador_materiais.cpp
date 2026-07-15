#include "gerenciador_materiais.h"

GerenciadorMateriais& GerenciadorMateriais::obterInstancia() {
    static GerenciadorMateriais instancia;
    return instancia;
}

GerenciadorMateriais::GerenciadorMateriais() {
    // Inicializar os presets disponíveis para todos os objetos geométricos
    presets = {
        { "Plastico Vermelho", "Reflete luz branca difusa, especular fraco", Material::criar(TipoMaterial::PLASTICO, 0.85f, 0.15f, 0.15f) },
        { "Metal Prateado",    "Reflexo concentrado, absorve difusa, reflexo na propria cor", Material::criar(TipoMaterial::METAL, 0.75f, 0.75f, 0.75f) },
        { "Borracha Azul",     "Quase sem brilho especular, predominantemente opaco", Material::criar(TipoMaterial::BORRACHA, 0.15f, 0.20f, 0.80f) },
        { "Ceramica Branca",   "Brilho intermediario e esbranquicado", Material::criar(TipoMaterial::CERAMICA, 0.90f, 0.90f, 0.90f) }
    };

    // Inicializar o material ativo de cada categoria com presets diferentes
    materiaisAtivos.insert({CategoriaObjeto::ESFERA, presets[0].material}); // Plastico Vermelho
    materiaisAtivos.insert({CategoriaObjeto::CONE,   presets[1].material}); // Metal Prateado
    materiaisAtivos.insert({CategoriaObjeto::CUBO,   presets[2].material}); // Borracha Azul
    materiaisAtivos.insert({CategoriaObjeto::PLANO,  presets[3].material}); // Ceramica Branca
}

const Material& GerenciadorMateriais::obterMaterialAtivo(CategoriaObjeto categoria) const {
    return materiaisAtivos.at(categoria);
}

void GerenciadorMateriais::definirMaterialAtivo(CategoriaObjeto categoria, std::size_t indicePreset) {
    if (indicePreset < presets.size()) {
        materiaisAtivos.at(categoria) = presets[indicePreset].material;
    }
}

void GerenciadorMateriais::ajustarBrilho(CategoriaObjeto categoria, float delta) {
    materiaisAtivos.at(categoria).ajustarBrilho(delta);
}

float GerenciadorMateriais::obterBrilho(CategoriaObjeto categoria) const {
    return materiaisAtivos.at(categoria).obterBrilho();
}

std::size_t GerenciadorMateriais::quantidadePresets() const {
    return presets.size();
}

const char* GerenciadorMateriais::obterNomePreset(std::size_t indice) const {
    if (indice < presets.size()) {
        return presets[indice].nome;
    }
    return "Desconhecido";
}

const char* GerenciadorMateriais::obterDescricaoPreset(std::size_t indice) const {
    if (indice < presets.size()) {
        return presets[indice].descricao;
    }
    return "";
}

const char* GerenciadorMateriais::obterNomeCategoria(CategoriaObjeto categoria) {
    switch (categoria) {
        case CategoriaObjeto::ESFERA: return "Esferas";
        case CategoriaObjeto::CONE:   return "Cones";
        case CategoriaObjeto::CUBO:   return "Cubos";
        case CategoriaObjeto::PLANO:  return "Planos";
    }
    return "Desconhecido";
}
