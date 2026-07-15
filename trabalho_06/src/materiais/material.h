#ifndef MATERIAL_H
#define MATERIAL_H

/*
 * Tipos de superfície reconhecidos pela fábrica Material::criar(). Cada
 * tipo carrega um "perfil" fisicamente plausível de como a luz se
 * comporta naquele tipo de material, evitando que o usuário precise
 * calcular manualmente os 10 coeficientes brutos.
 */
enum class TipoMaterial {
    PLASTICO,  // diffuse forte na cor do objeto, specular quase branco
    METAL,     // diffuse fraco, specular forte E colorido pela cor do metal
    BORRACHA,  // quase só diffuse, specular mínimo, shininess baixo
    CERAMICA   // diffuse dominante, specular moderado e esbranquiçado
};

/*
 * Agrupa os quatro parâmetros que a OpenGL usa para descrever como uma
 * superfície reage à luz, no modelo Phong/Blinn-Phong:
 *
 *   - ambiente:  cor refletida mesmo sem incidência direta de luz;
 *   - difuso:    cor da reflexão Lambertiana (depende do ângulo da luz);
 *   - especular: cor do brilho "espelhado" (depende do observador);
 *   - brilho (shininess): concentra o especular (alto=metal, baixo=plástico fosco).
 */
class Material {
public:
    Material(float ambR, float ambG, float ambB,
             float difR, float difG, float difB,
             float espR, float espG, float espB,
             float brilho);

    void aplicar() const; // chama glMaterialfv/glMaterialf para os 4 componentes

    /*
     * Fábrica: cria um material fisicamente plausível a partir de um
     * TIPO de superfície e uma cor base (0-1), sem exigir que quem chama
     * calcule os 10 coeficientes brutos manualmente.
     */
    static Material criar(TipoMaterial tipo, float corR, float corG, float corB);

    void ajustarBrilho(float delta); // soma delta ao shininess, com limites de segurança
    float obterBrilho() const;

    /*
     * Exposto publicamente para servir de cor "unlit" (glColor3f) enquanto
     * GL_LIGHTING ainda está desligado.
     */
    float difR, difG, difB;

private:
    float ambiente[4];
    float difuso[4];
    float especular[4];
    float brilho;
};

#endif
