#ifndef CAMERA_H
#define CAMERA_H

/*
 * Câmera orbital: gira em torno de um ponto alvo usando coordenadas esféricas
 * (azimute, elevação, distância). Essa abordagem é mais estável no freeglut
 * do que uma câmera livre com mouse-warping, e é ideal para observar objetos
 * centrados na cena.
 */
class Camera {
public:
    Camera();

    void aplicar(); // chama gluLookAt com a posição calculada a partir do estado atual

    void orbitar(float deltaAzimuteGraus, float deltaElevacaoGraus);
    void aproximar(float delta);              // zoom: delta > 0 aproxima, < 0 afasta
    void mover(float frente, float direita);  // desloca o alvo RELATIVO à direção da câmera
    void resetar();                           // volta azimute/elevação/distância/alvo aos valores iniciais

private:
    float azimute;   // ângulo horizontal em torno do alvo (graus)
    float elevacao;  // ângulo vertical (graus), limitado para evitar gimbal flip
    float distancia; // raio da esfera de órbita (distância câmera-alvo)

    float alvoX, alvoY, alvoZ; // ponto para onde a câmera está olhando
};

#endif
