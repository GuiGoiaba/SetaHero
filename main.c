#include "header.h"

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

int opcao = 0;
int seg = 0;

int main()
{
    // Inicializa variáveis do jogo
    ALLEGRO_DISPLAY *janela = (ALLEGRO_DISPLAY *)malloc(sizeof(ALLEGRO_DISPLAY *));
    ALLEGRO_BITMAP *seta_cima, *seta_baixo, *seta_direita, *seta_esquerda = (ALLEGRO_BITMAP *)malloc(sizeof(ALLEGRO_BITMAP *));
    ALLEGRO_EVENT_QUEUE *fila_eventos = (ALLEGRO_EVENT_QUEUE *)malloc(sizeof(ALLEGRO_EVENT_QUEUE *));
    ALLEGRO_TIMER *timer = (ALLEGRO_TIMER *)malloc(sizeof(ALLEGRO_TIMER *));

    inicializa_allegro();
    inicializa_jogo(&janela, &seta_cima, &seta_baixo, &seta_direita, &seta_esquerda, &fila_eventos, &timer);

    // Inicializando seta
    int direcao = 0;
    int pontuacao = 0;
    int vidas = 3;
    int ult_pont = 0;
    int espaco_inicial = 150;
    float vel_inim = 0;
    char nome_jogador[MAX_NOME] = "";

    Fila *fila_setas = criar_fila();
    Seta *setas[MAX_INIMIGOS];
    for (int i = 0; i < MAX_INIMIGOS; i++)
    {
        setas[i] = criar_seta(LARGURA + i * espaco_inicial, ALTURA / 2 - TAM_SETA / 2);
        enfileirar(fila_setas, setas[i]);
    }

    // CONFIGURAR OBJETOS
    srand(time(NULL));
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
                        vel_inim = 0;
                        for (int i = 0; i < MAX_INIMIGOS; i++)
                        {
                            setas[i]->x = LARGURA + i * espaco_inicial;
                        }
                    }
                    else
                    {
                        dentro_menu = false;
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
                    setas[i]->x -= (int)vel_inim;

                    if (setas[i]->x + TAM_SETA < 0)
                    {
                        vidas--;
                        setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                        setas[i] = desenfileirar(fila_setas);
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
                    direcao = 0;
                    break;
                case (ALLEGRO_KEY_DOWN):
                    direcao = 1;
                    break;
                case (ALLEGRO_KEY_RIGHT):
                    direcao = 2;
                    break;
                case (ALLEGRO_KEY_LEFT):
                    direcao = 3;
                    break;
                }
                for (int i = 0; i < MAX_INIMIGOS; i++)
                {
                    if (setas[i]->x < LARGURA / 2 + TAM_SETA / 2 &&
                        setas[i]->x > LARGURA / 2 - TAM_SETA / 2 &&
                        setas[i]->direcao == direcao)
                    {
                        pontuacao += 50;
                        setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                    }
                    else if (setas[i]->x < LARGURA / 2 + TAM_SETA / 2 &&
                             setas[i]->x > LARGURA / 2 - TAM_SETA / 2 &&
                             setas[i]->direcao != direcao)
                    {
                        vidas--;
                        setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
                    }
                }
            }
            if (vidas <= 0)
            {
                pont_jogador(pontuacao, nome_jogador);
                ult_pont = pontuacao;
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
            if (redraw && al_is_event_queue_empty(fila_eventos))
            {
                redraw = false;
                desenha_jogo(setas, direcao, pontuacao, vidas, vel_inim, seta_cima, seta_baixo, seta_direita, seta_esquerda);
            }
        }
    }
    liberar_fila(fila_setas);
    for (int i = 0; i < MAX_INIMIGOS; i++)
        libera_seta(setas[i]);
    finaliza_recursos(janela, fila_eventos, timer, seta_cima, seta_baixo, seta_direita, seta_esquerda);
    return 0;
}