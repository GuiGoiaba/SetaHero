<h1>Seta-Hero</h1>
Jogo simples de ritmo

## Descrição

Nesse jogo a tela se comportar como uma esteira onde serão enfileirados símbolos aleatórios correspondentes as setas do teclado (para cima, para baixo, para direita e para esquerda).
## Ferramenta

Linguagem C em conjunto à biblioteca Allegro5

Mais informações acerca em [Allegro 5 (A game programming library)](http://liballeg.org/)
## Compilação

Se você estiver no Linux ou Mac e tiver o Allegro5 e o gcc instalados, execute compile.sh para compilar este código. Não tenho ideia de como isso funcionaria no Windows, embora o Allegro ofereça suporte ao mesmo, então você pode tentar.

```
gcc -c funcoes.c
gcc -o SetaHero main.c funcoes.o -lallegro -lallegro_primitives -lallegro_image -lallegro_ttf -lallegro_font
```
## Para jogar

```
./SetaHero
```
