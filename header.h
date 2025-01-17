#ifndef header_h
#define header_h

// GLOBAIS
#define LARGURA 800
#define ALTURA 600
#define FPS 60
#define MAX_NOME 20
#define TAM_SETA 50
#define VELOCIDADE 0.5f
#define MAX_INIMIGOS 3

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"

typedef struct seta
{
    int x, y;
    int direcao;
} Seta;

typedef struct lista
{
    Seta *seta;
    struct lista *proximo;
} Lista;

typedef struct fila
{
    Lista *inicio;
    Lista *fim;
} Fila;

enum teclas{cima, baixo, direita, esquerda};

Fila *criar_fila();
void enfileirar(Fila *fila, Seta *seta);
Seta *desenfileirar(Fila *fila);
void liberar_fila(Fila *fila);
Seta *criar_seta(int x, int y);
Seta *criar_jogador(int x, int y, int direcao);
void liberar_setas(Seta **setas, int max_inimigos);
ALLEGRO_DISPLAY *cria_tela();
void inicializa_allegro();
void inicializa_recursos(ALLEGRO_BITMAP **seta_cima, ALLEGRO_BITMAP **seta_baixo, ALLEGRO_BITMAP **seta_direita, ALLEGRO_BITMAP **seta_esquerda);
void configura_eventos(ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_DISPLAY *janela, ALLEGRO_TIMER *timer);
void desenha_menu(int escolha, const char *nome_jogador, int ult_pont);
void insere_nome(char *nome_jogador);
void inicializa_jogo(ALLEGRO_DISPLAY **janela, ALLEGRO_BITMAP **seta_cima, ALLEGRO_BITMAP **seta_baixo, ALLEGRO_BITMAP **seta_direita, ALLEGRO_BITMAP **seta_esquerda, ALLEGRO_EVENT_QUEUE **fila_eventos, ALLEGRO_TIMER **timer);
void desenha_jogo(Seta *setas[], Seta *jogador, int direcao, int pontuacao, int vidas, float vel_inim, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda);
void desenha_seta(Seta *seta, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda);
void game_over(int pontuacao, char *nome_jogador);
void finaliza_recursos(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda);

#endif