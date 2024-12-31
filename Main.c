#include "objetos.h"

// GLOBAIS
#define LARGURA 800
#define ALTURA 600
#define FPS 60
#define MAX_NOME 20
#define TAM_SETA 50
#define VELOCIDADE 0.5f
#define MAX_INIMIGOS 3

// CONFIGURANDO A SETA
typedef struct seta
{
    int x, y;
    int direcao;
} Seta;

int main()
{
    // Inicializa Allegro
    inicializa_allegro();

    // Inicializa variáveis do jogo
    ALLEGRO_DISPLAY *janela = cria_tela();
    ALLEGRO_BITMAP *seta_cima, *seta_baixo, *seta_direita, *seta_esquerda = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);

    // Inicializando seta
    Seta setas[MAX_INIMIGOS];
    int espaco_inicial = 150;
    for (int i = 0; i < MAX_INIMIGOS; i++)
    {
        setas[i] = criar_seta(LARGURA + i * espaco_inicial, ALTURA / 2 - TAM_SETA / 2);
    }
    inicializa_recursos(&seta_cima, &seta_baixo, &seta_direita, &seta_esquerda);
    int seta_jogador = 0;
    int pontuacao = 0;
    int vidas = 3;
    int ult_pont = 0;
    float vel_inim = 1.0f;

    // Configuração de eventos
    configura_eventos(fila_eventos, janela, timer);

    // Configuração dos objetos
    int opcao = 0;
    int seg = 0;
    char nome_jogador[MAX_NOME] = "";

    srand(time(NULL));
    al_start_timer(timer);
    bool fechar = false;
    bool redraw = true;
    bool dentro_menu = true;

    while (!fechar)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila_eventos, &ev);
        if (dentro_menu)
        {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    opcao = (opcao + 1) % 2;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    opcao = (opcao - 1 + 2) % 2;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    if (opcao == 0)
                    {
                        dentro_menu = false;
                        insere_nome(nome_jogador);
                        dentro_menu = false;
                        pontuacao = 0;
                        vidas = 3;
                        vel_inim = 1.0f;
                    }
                    else
                    {
                        fechar = true;
                    }
                }
            }
            desenha_menu(opcao, nome_jogador, ult_pont);
        }
        else
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                fechar = true;
            }
            else if (ev.type == ALLEGRO_EVENT_TIMER)
            {
                dentro_menu = false;
                redraw = true;

                for (int i = 0; i < MAX_INIMIGOS; i++)
                {
                    setas[i].x -= (int)vel_inim;

                    if (setas[i].x + TAM_SETA < 0)
                    {
                        vidas--;
                        setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                    }
                }

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
                switch (ev.keyboard.keycode)
                {
                case (ALLEGRO_KEY_ESCAPE):
                    fechar = true;
                    break;
                case (ALLEGRO_KEY_UP):
                    seta_jogador = 0;
                    break;
                case (ALLEGRO_KEY_DOWN):
                    seta_jogador = 1;
                    break;
                case (ALLEGRO_KEY_RIGHT):
                    seta_jogador = 2;
                    break;
                case (ALLEGRO_KEY_LEFT):
                    seta_jogador = 3;
                    break;
                }
                for (int i = 0; i < MAX_INIMIGOS; i++)
                {
                    if (setas[i].x < LARGURA / 2 + TAM_SETA / 2 &&
                        setas[i].x > LARGURA / 2 - TAM_SETA / 2 &&
                        setas[i].direcao == seta_jogador)
                    {
                        pontuacao += 50;
                        setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                    }
                    else if (setas[i].x < LARGURA / 2 + TAM_SETA / 2 &&
                             setas[i].x > LARGURA / 2 - TAM_SETA / 2 &&
                             setas[i].direcao != seta_jogador)
                    {
                        vidas--;
                        setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                    }
                }
            }
            if (vidas <= 0)
            {
                menu_final(pontuacao, nome_jogador);
                ult_pont = pontuacao;
                while (1)
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
            if (redraw && al_is_event_queue_empty(fila_eventos))
            {
                redraw = false;

                for (int i = 0; i < MAX_INIMIGOS; i++)
                {
                    desenha_seta(setas[i], seta_cima, seta_baixo, seta_direita, seta_esquerda);
                }

                Seta player = {LARGURA / 2 - TAM_SETA / 2, ALTURA / 2 - TAM_SETA / 2, seta_jogador};
                desenha_seta(player, seta_cima, seta_baixo, seta_direita, seta_esquerda);

                al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0), 10, 10, 0, "Pontuação: %d", pontuacao);
                al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0), LARGURA - 150, 10, 0, "Vidas: %d", vidas);
                al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0), LARGURA - 150, 30, 0, "Velocidade: %.2f", vel_inim);

                al_flip_display();
                al_clear_to_color(al_map_rgb(192, 192, 192));
                al_draw_filled_rectangle(0, 355, LARGURA, 270, al_map_rgb(128, 128, 128));
            }
        }
    }
    finaliza_recursos(janela, fila_eventos, timer, seta_cima, seta_baixo, seta_direita, seta_esquerda);
    return 0;
}