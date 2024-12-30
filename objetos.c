#include "objetos.h"

#define LARGURA 800
#define ALTURA 600
#define MAX_NOME 20
#define TAM_SETA 50

typedef struct seta{
    int x, y;
    int type;
} Seta;

Seta criar_seta(int x, int y){
    Seta seta;
    seta.x = x;
    seta.y = y;
    seta.type = rand() % 4;
    return seta;
}

void desenha_seta(Seta seta, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda){
    switch (seta.type){
    case 0: // Cima
        al_draw_bitmap(seta_cima, seta.x, seta.y, 0);
        break;
    case 1: // Baixo
        al_draw_bitmap(seta_baixo, seta.x, seta.y, 0);
        break;
    case 2: // Direita
        al_draw_bitmap(seta_direita, seta.x, seta.y, 0);
        break;
    case 3: // Esquerda
        al_draw_bitmap(seta_esquerda, seta.x, seta.y, 0);
        break;
    }
}

void desenha_menu(int escolha, const char *nome_jogador, int ult_pont){
    ALLEGRO_FONT *fonte = al_create_builtin_font();
    al_clear_to_color(al_map_rgb(192, 192, 192));

    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 4, ALLEGRO_ALIGN_CENTRE, "SETA HERO");

    if (nome_jogador[0] != '\0'){
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 4 + 40, ALLEGRO_ALIGN_CENTRE,
                      "Jogador: %s", nome_jogador);
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 4 + 80, ALLEGRO_ALIGN_CENTRE,
                      "Ultima Pontuacao: %d", ult_pont);
    }

    const char *opcoes[] = {"Iniciar Jogo", "Sair"};
    for (int i = 0; i < 2; i++){
        if (i == escolha){
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2 + i * 40, ALLEGRO_ALIGN_CENTRE, opcoes[i]);
        }
        else{
            al_draw_text(fonte, al_map_rgb(128, 128, 128), LARGURA / 2, ALTURA / 2 + i * 40, ALLEGRO_ALIGN_CENTRE, opcoes[i]);
        }
    }
    al_flip_display();
}

void insere_nome(char *nome_jogador){
    ALLEGRO_FONT *fonte = al_create_builtin_font();
    al_clear_to_color(al_map_rgb(192, 192, 192));

    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 3, ALLEGRO_ALIGN_CENTRE, "Digite seu nome:");
    al_flip_display();

    ALLEGRO_EVENT_QUEUE *input_teclado = al_create_event_queue();
    al_register_event_source(input_teclado, al_get_keyboard_event_source());

    int inicio = 0;
    bool cabou = false;

    while (!cabou){
        ALLEGRO_EVENT ev;
        al_wait_for_event(input_teclado, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            cabou = true;
        }
        if (ev.type == ALLEGRO_EVENT_KEY_CHAR){
            if (ev.keyboard.unichar >= 32 && ev.keyboard.unichar <= 126 && inicio < MAX_NOME - 1){
                nome_jogador[inicio++] = (char)ev.keyboard.unichar;
                nome_jogador[inicio] = '\0';
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && inicio > 0){
                nome_jogador[--inicio] = '\0';
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                cabou = true;
            }

            al_clear_to_color(al_map_rgb(192, 192, 192));
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 3, ALLEGRO_ALIGN_CENTRE, "Digite seu nome:");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2, ALLEGRO_ALIGN_CENTRE, nome_jogador);
            al_flip_display();
        }
    }
    al_destroy_event_queue(input_teclado);
}

void menu_final(int pontuacao, char *nome_jogador){
    ALLEGRO_FONT *fonte = al_create_builtin_font();
    al_clear_to_color(al_map_rgb(192, 192, 192));

    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 3, ALLEGRO_ALIGN_CENTRE, "Fim de Jogo!");
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2, ALLEGRO_ALIGN_CENTRE, "Jogador: %s", nome_jogador);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2 + 40, ALLEGRO_ALIGN_CENTRE, "Pontuação Final: %d", pontuacao);
    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2 + 80, ALLEGRO_ALIGN_CENTRE, "Pressione Enter para voltar ao Menu");

    al_flip_display();
}

void inicializa_allegro() {
    if (!al_init()) {
        printf("Erro ao inicializar o Allegro.\n");
        exit(EXIT_FAILURE);
    }
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
}

ALLEGRO_DISPLAY* cria_tela() {
    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA, ALTURA);
    if (!janela) {
        printf("Erro ao criar a tela.\n");
        exit(EXIT_FAILURE);
    }
    return janela;
}

void inicializa_recursos(ALLEGRO_BITMAP **seta_cima, ALLEGRO_BITMAP **seta_baixo, ALLEGRO_BITMAP **seta_direita, ALLEGRO_BITMAP **seta_esquerda) {
    *seta_cima = al_load_bitmap("Assets/seta_cima.png");
    *seta_baixo = al_load_bitmap("Assets/seta_baixo.png");
    *seta_direita = al_load_bitmap("Assets/seta_direita.png");
    *seta_esquerda = al_load_bitmap("Assets/seta_esquerda.png");
    if (!*seta_cima || !*seta_baixo || !*seta_direita || !*seta_esquerda) {
        printf("Erro ao carregar os bitmaps.\n");
        exit(EXIT_FAILURE);
    }
}

void configura_eventos(ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_DISPLAY *janela, ALLEGRO_TIMER *timer) {
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
}

void menu_principal(ALLEGRO_EVENT_QUEUE *fila_eventos, int *menu_option, bool *dentro_menu, char *nome_jogador, int *pontuacao, int *vidas, float *vel_inim, bool *fechar, int ult_pont) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(fila_eventos, &ev);
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
            *menu_option = (*menu_option + 1) % 2;
        } else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
            *menu_option = (*menu_option - 1 + 2) % 2;
        } else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            if (*menu_option == 0) {
                *dentro_menu = false;
                insere_nome(nome_jogador);
                *pontuacao = 0;
                *vidas = 3;
                *vel_inim = 1.0f;
            } else {
                *fechar = true;
            }
        }
    }
    desenha_menu(*menu_option, nome_jogador, ult_pont);
}

void jogo(Seta *seta, int *pontuacao, int *vidas, float *vel_inim, bool *fechar, bool *redraw, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda, ALLEGRO_EVENT_QUEUE *fila_eventos, int seta_jogador) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(fila_eventos, &ev);
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        *fechar = true;
    } else if (ev.type == ALLEGRO_EVENT_TIMER) {
        *redraw = true;
        seta->x -= (int)*vel_inim;
        if (seta->x + TAM_SETA < 0) {
            (*vidas)--;
            *seta = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
        }
    }

    if (*redraw && al_is_event_queue_empty(fila_eventos)) {
        *redraw = false;
        desenha_seta(*seta, seta_cima, seta_baixo, seta_direita, seta_esquerda);
        Seta player = {LARGURA / 2 - TAM_SETA / 2, ALTURA / 2 - TAM_SETA / 2, seta_jogador};
        desenha_seta(player, seta_cima, seta_baixo, seta_direita, seta_esquerda);
        al_flip_display();
    }
}

void finaliza_recursos(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda) {
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_bitmap(seta_cima);
    al_destroy_bitmap(seta_baixo);
    al_destroy_bitmap(seta_direita);
    al_destroy_bitmap(seta_esquerda);
}
