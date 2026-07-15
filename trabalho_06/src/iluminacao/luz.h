#ifndef LUZ_H
#define LUZ_H

/*
 * Modelo de iluminação atualmente ativo. Cada modo liga/desliga
 * componentes da luz para isolar o efeito de cada termo do modelo
 * Phong/Blinn-Phong, permitindo a comparação.
 */
enum class ModoIluminacao {
    AMBIENTE,    // apenas o termo ambiente
    LAMBERT,     // ambiente + difuso, sem brilho especular
    BLINN_PHONG  // ambiente + difuso + especular: modelo completo
};

/*
 * Fonte de luz pontual móvel (GL_LIGHT0). Guarda a intensidade "cheia" de
 * cada componente e, a cada frame, aplica apenas os componentes
 * permitidos pelo modo de iluminação ativo no momento.
 */
class Luz {
public:
    Luz();

    void aplicar(ModoIluminacao modo) const;

    void mover(float dx, float dy, float dz);
    void setIntensidade(float intensidade); // escala difusa e especular
    float obterIntensidade() const;

    float posX, posY, posZ;

private:
    float ambiente[4];
    float difusaBase[4];
    float especularBase[4];
    float intensidade; // multiplicador sobre difusaBase/especularBase
};

#endif
