bibliotecas usadas: GLUT , OpenGL , GLU
comando para baixar todas as dependências no ubuntu : sudo apt install build-essential freeglut3-dev libglu1-mesa-dev libgl1-mesa-dev
comando para compilar: gcc poligono_opengl.c -o poligono -lGL -lGLU -lglut -lm