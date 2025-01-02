#ifndef OBJETOS_H
#define OBJETOS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"

typedef struct seta Seta;

Seta criar_seta(int largura_tela, int altura_tela);
void desenha_seta(Seta seta, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda);
void desenha_menu(int escolha, const char *nome_jogador, int ult_pont);
void insere_nome(char *nome_jogador);
void pont_jogador(int pontuacao, char *nome_jogador);
void inicializa_allegro();
ALLEGRO_DISPLAY *cria_tela();
void inicializa_recursos(ALLEGRO_BITMAP **seta_cima, ALLEGRO_BITMAP **seta_baixo, ALLEGRO_BITMAP **seta_direita, ALLEGRO_BITMAP **seta_esquerda);
void inicializa_jogo(ALLEGRO_DISPLAY **janela, ALLEGRO_BITMAP **seta_cima, ALLEGRO_BITMAP **seta_baixo, ALLEGRO_BITMAP **seta_direita, ALLEGRO_BITMAP **seta_esquerda, ALLEGRO_EVENT_QUEUE **fila_eventos, ALLEGRO_TIMER **timer);
void configura_eventos(ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_DISPLAY *janela, ALLEGRO_TIMER *timer);
void desenha_jogo(int seta_jogador, int pontuacao, int vidas, float vel_inim, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda);
void finaliza_recursos(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda);

#endif