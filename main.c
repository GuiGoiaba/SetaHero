#include "header.h"

// variáveis globais
int opcao = 0;
int seg = 0;

int main()
{
    // inicialização dos recursos
    ALLEGRO_TIMER *timer;
    ALLEGRO_DISPLAY *janela;
    ALLEGRO_EVENT_QUEUE *fila_eventos;
    ALLEGRO_BITMAP *seta_cima, *seta_baixo, *seta_direita, *seta_esquerda;

    // inicialização do jogo
    inicializa_allegro();
    inicializa_jogo(&janela, &seta_cima, &seta_baixo, &seta_direita, &seta_esquerda, &fila_eventos, &timer);

    // variáveis do jogo
    int vidas = 3;
    int direcao = 0;
    int ult_pont = 0;
    int pontuacao = 0;
    float vel_inim = 0;
    int espaco_inicial = 150;
    char nome_jogador[MAX_NOME] = "";

    // criação das setas
    Seta *jogador;
    Seta *setas[MAX_INIMIGOS];
    Fila *fila_setas = criar_fila();
    for (int i = 0; i < MAX_INIMIGOS; i++)
    {
        setas[i] = criar_seta(LARGURA + i * espaco_inicial, ALTURA / 2 - TAM_SETA / 2);
        enfileirar(fila_setas, setas[i]);
    }

    // loop principal
    srand(time(NULL));
    bool redraw = true;
    bool fechar = false;
    bool dentro_menu = true;

    while (!fechar)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila_eventos, &ev);
        // menu
        if (dentro_menu)
        {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                // escolha da opção
                if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    opcao = (opcao + 1) % 2;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    opcao = (opcao - 1 + 2) % 2;
                }
                // confirmação da opção
                else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    if (opcao == 0)
                    {
                        // reinicialização do jogo
                        dentro_menu = false;
                        insere_nome(nome_jogador);
                        pontuacao = 0;
                        vidas = 3;
                        vel_inim = 1.0;
                        for (int i = 0; i < MAX_INIMIGOS; i++)
                        {
                            setas[i]->x = LARGURA + i * espaco_inicial;
                        }
                    }
                    else
                    {
                        // saída do jogo
                        dentro_menu = false;
                        fechar = true;
                    }
                }
            }
            // desenho do menu
            desenha_menu(opcao, nome_jogador, ult_pont);
        }
        else
        {
            // fechar o jogo ao clicar no X
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                fechar = true;
            }
            // atualização do jogo
            else if (ev.type == ALLEGRO_EVENT_TIMER)
            {
                dentro_menu = false;
                redraw = true;

                // movimentação das setas
                for (int i = 0; i < MAX_INIMIGOS; i++)
                {
                    setas[i]->x -= (int)vel_inim;

                    if (setas[i]->x + TAM_SETA < 0)
                    {
                        vidas--;
                        free(setas[i]);
                        setas[i] = desenfileirar(fila_setas);
                        setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                    }
                }
                // atualização dos seguntos
                seg++;
                if (seg > 300)
                {
                    vel_inim += VELOCIDADE;
                    seg = 0;
                    if (vel_inim > 12)
                        fechar = true;
                }
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                // movimentação do jogador
                switch (ev.keyboard.keycode)
                {
                case (ALLEGRO_KEY_ESCAPE):
                    fechar = true;
                    break;
                case (ALLEGRO_KEY_UP):
                    direcao = cima;
                    break;
                case (ALLEGRO_KEY_DOWN):
                    direcao = baixo;
                    break;
                case (ALLEGRO_KEY_RIGHT):
                    direcao = direita;
                    break;
                case (ALLEGRO_KEY_LEFT):
                    direcao = esquerda;
                    break;
                }
                for (int i = 0; i < MAX_INIMIGOS; i++)
                {
                    // colisão com as setas
                    if (setas[i]->x < LARGURA / 2 + TAM_SETA / 2 &&
                        setas[i]->x > LARGURA / 2 - TAM_SETA / 2 &&
                        setas[i]->direcao == direcao)
                    {
                        pontuacao += 50;
                        free(setas[i]);
                        setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                    }
                    else if (setas[i]->x < LARGURA / 2 + TAM_SETA / 2 &&
                             setas[i]->x > LARGURA / 2 - TAM_SETA / 2 &&
                             setas[i]->direcao != direcao)
                    {
                        vidas--;
                        free(setas[i]);
                        setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                    }
                }
            }
            if (vidas <= 0)
            {
                // tela final do jogo
                game_over(pontuacao, nome_jogador);
                ult_pont = pontuacao;
                liberar_setas(setas, MAX_INIMIGOS);

                // Reiniciar o jogo
                for (int i = 0; i < MAX_INIMIGOS; i++)
                {
                    setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                }
                while (!dentro_menu)
                {
                    ALLEGRO_EVENT ev2;
                    al_wait_for_event(fila_eventos, &ev2);
                    if (ev2.type == ALLEGRO_EVENT_KEY_DOWN && ev2.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    {
                        dentro_menu = true;
                        break;
                    }
                }
            }
            // desenho do jogo
            if (redraw && al_is_event_queue_empty(fila_eventos))
            {
                redraw = false;
                desenha_jogo(setas, jogador, direcao, pontuacao, vidas, vel_inim, seta_cima, seta_baixo, seta_direita, seta_esquerda);
            }
        }
    }
    // liberação dos recursos
    liberar_fila(fila_setas);
    liberar_setas(setas, MAX_INIMIGOS);
    finaliza_recursos(janela, fila_eventos, timer, seta_cima, seta_baixo, seta_direita, seta_esquerda);
    return 0;
}