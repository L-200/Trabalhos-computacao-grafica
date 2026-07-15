#include "camera.h"
#include <GL/glu.h>
#include <cmath>

namespace {
    const float PI = 3.14159265358979323846f;
    inline float paraRadianos(float graus) { return graus * PI / 180.0f; }

    /*
     * Valores iniciais da câmera, reaproveitados pelo construtor e por
     * resetar(), para não duplicar os "números mágicos".
     */
    const float AZIMUTE_INICIAL = 45.0f;
    const float ELEVACAO_INICIAL = 25.0f;
    const float DISTANCIA_INICIAL = 10.0f; // aumentada para enquadrar os objetos mais afastados
}

Camera::Camera()
    : azimute(AZIMUTE_INICIAL), elevacao(ELEVACAO_INICIAL), distancia(DISTANCIA_INICIAL),
      alvoX(0.0f), alvoY(0.0f), alvoZ(0.0f) {}

void Camera::aplicar() {
    /*
     * Conversão de coordenadas esféricas (azimute, elevação, distância)
     * para coordenadas cartesianas da posição da câmera, relativas ao alvo.
     * É o mesmo princípio de converter latitude/longitude/raio em posição
     * sobre um globo: a câmera "orbita" o alvo sobre uma esfera imaginária.
     */
    float elevRad = paraRadianos(elevacao);
    float azimRad = paraRadianos(azimute);

    float x = alvoX + distancia * cosf(elevRad) * sinf(azimRad);
    float y = alvoY + distancia * sinf(elevRad);
    float z = alvoZ + distancia * cosf(elevRad) * cosf(azimRad);

    gluLookAt(
        x, y, z,              // posição do olho (câmera) no mundo
        alvoX, alvoY, alvoZ,  // ponto para onde a câmera olha
        0.0f, 1.0f, 0.0f      // vetor "up": define a orientação vertical da câmera
    );
}

void Camera::orbitar(float deltaAzimuteGraus, float deltaElevacaoGraus) {
    azimute += deltaAzimuteGraus;
    elevacao += deltaElevacaoGraus;

    /*
     * Trava a elevação pouco antes de +-90 graus para evitar que a câmera
     * "vire de cabeça para baixo" (gimbal flip) ao cruzar o polo da esfera.
     */
    if (elevacao > 89.0f) elevacao = 89.0f;
    if (elevacao < -89.0f) elevacao = -89.0f;
}

void Camera::aproximar(float delta) {
    distancia -= delta;
    if (distancia < 2.0f) distancia = 2.0f;    // evita a câmera entrar nos objetos
    if (distancia > 40.0f) distancia = 40.0f;  // evita perder a cena de vista
}

void Camera::mover(float frente, float direita) {
    /*
     * Movimento RELATIVO à orientação atual da câmera, não ao mundo.
     * Sem isso, W/A/S/D só "fazem sentido" quando azimute = 0; em
     * qualquer outro ângulo (como o inicial, 45 graus), as teclas
     * pareceriam trocadas, pois moveriam nos eixos fixos X/Z do mundo
     * em vez de para onde a câmera está de fato olhando.
     */
    float azimRad = paraRadianos(azimute);

    /*
     * Vetor "frente": direção XZ para onde a câmera está olhando
     * (do olho em direção ao alvo, projetada no plano XZ).
     */
    float frenteX = -sinf(azimRad);
    float frenteZ = -cosf(azimRad);

    /*
     * Vetor "direita": perpendicular ao "frente" no plano XZ
     * (equivalente a cross(frente, up), com up = (0,1,0)).
     */
    float direitaX = cosf(azimRad);
    float direitaZ = -sinf(azimRad);

    alvoX += frente * frenteX + direita * direitaX;
    alvoZ += frente * frenteZ + direita * direitaZ;
}

void Camera::resetar() {
    azimute = AZIMUTE_INICIAL;
    elevacao = ELEVACAO_INICIAL;
    distancia = DISTANCIA_INICIAL;
    alvoX = alvoY = alvoZ = 0.0f;
}
