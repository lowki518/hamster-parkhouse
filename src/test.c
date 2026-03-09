#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define MAX_INPUT_SIZE 512

// Struktur für Anwendungszustand
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    char user_input[MAX_INPUT_SIZE];
    int running;
    int frame_count;
} AppState;

/**
 * Schreibe Benutzer-Input in data/output.txt
 * Erstellt das Verzeichnis falls nicht existent
 */
int write_output_file(const char *data) {
    // Erstelle das data-Verzeichnis falls nicht existent
    if (access("data", F_OK) == -1) {
        if (mkdir("data", 0755) == -1) {
            perror("mkdir failed");
            return -1;
        }
        printf("[INFO] Created 'data' directory\n");
    }

    // Öffne und schreibe die Datei
    FILE *file = fopen("data/output.txt", "w");
    if (!file) {
        perror("fopen failed");
        fprintf(stderr, "Error: Could not open data/output.txt\n");
        return -1;
    }

    fprintf(file, "User Input: %s\n", data);
    fprintf(file, "Timestamp: %ld\n", (long)time(NULL));
    fflush(file);
    fclose(file);

    printf("[SUCCESS] Output written to data/output.txt\n");
    printf("[DATA] Content: %s\n", data);
    return 0;
}

/**
 * Initialisiere SDL3 und erstelle Fenster + Renderer
 */
int init_graphics(AppState *app) {
    printf("[INIT] Initializing SDL3...\n");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "[ERROR] SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    printf("[INIT] Creating window (%dx%d)...\n", WINDOW_WIDTH, WINDOW_HEIGHT);
    
    app->window = SDL_CreateWindow(
        "SDL3 Desktop Application - Codespaces",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0  // Flags: 0 = default windowed mode
    );

    if (!app->window) {
        fprintf(stderr, "[ERROR] SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    printf("[INIT] Creating renderer...\n");
    app->renderer = SDL_CreateRenderer(app->window, NULL);
    if (!app->renderer) {
        fprintf(stderr, "[ERROR] SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        return -1;
    }

    // Enable V-Sync für smoother rendering
    SDL_SetRenderVSync(app->renderer, 1);

    printf("[SUCCESS] Graphics initialized\n");
    return 0;
}

/**
 * Rendere einen Frame mit Text und grafischen Elementen
 */
void render_frame(AppState *app) {
    // Clear screen mit dunklem Hintergrund
    SDL_SetRenderDrawColor(app->renderer, 20, 20, 30, 255);
    SDL_RenderClear(app->renderer);

    // ===== Zeichne UI Elements =====

    // Title Bar (dunkles Blau)
    SDL_FRect title_bar = {0, 0, WINDOW_WIDTH, 60};
    SDL_SetRenderDrawColor(app->renderer, 30, 50, 100, 255);
    SDL_RenderFillRect(app->renderer, &title_bar);

    // Border für Title Bar
    SDL_SetRenderDrawColor(app->renderer, 100, 150, 255, 255);
    SDL_RenderRect(app->renderer, &title_bar);

    // ===== Input-Display Bereich =====
    SDL_FRect input_box = {20, 80, WINDOW_WIDTH - 40, 100};
    SDL_SetRenderDrawColor(app->renderer, 40, 40, 60, 255);
    SDL_RenderFillRect(app->renderer, &input_box);
    SDL_SetRenderDrawColor(app->renderer, 100, 200, 150, 255);
    SDL_RenderRect(app->renderer, &input_box);

    // ===== Status Bereich =====
    SDL_FRect status_box = {20, 200, WINDOW_WIDTH - 40, 80};
    SDL_SetRenderDrawColor(app->renderer, 40, 60, 40, 255);
    SDL_RenderFillRect(app->renderer, &status_box);
    SDL_SetRenderDrawColor(app->renderer, 100, 255, 100, 255);
    SDL_RenderRect(app->renderer, &status_box);

    // ===== Animation Demo =====
    // Rotierendes Rechteck
    int center_x = WINDOW_WIDTH / 2;
    int center_y = WINDOW_HEIGHT - 150;
    int size = 80;

    // Berechne Rotation basierend auf Frame-Count
    float angle = (app->frame_count % 360) * 1.0f;
    
    // Zeichne animiertes Quadrat
    SDL_FRect animated_box = {
        (float)(center_x - size/2),
        (float)(center_y - size/2),
        (float)size,
        (float)size
    };

    // Farbe ändert sich mit Zeit
    int color_val = (app->frame_count * 2) % 256;
    SDL_SetRenderDrawColor(app->renderer, 100, color_val, 200, 255);
    SDL_RenderFillRect(app->renderer, &animated_box);

    // Border
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 100, 255);
    SDL_RenderRect(app->renderer, &animated_box);

    // ===== Footer =====
    SDL_FRect footer = {0, WINDOW_HEIGHT - 40, WINDOW_WIDTH, 40};
    SDL_SetRenderDrawColor(app->renderer, 30, 30, 50, 255);
    SDL_RenderFillRect(app->renderer, &footer);
    SDL_SetRenderDrawColor(app->renderer, 100, 150, 255, 255);
    SDL_RenderRect(app->renderer, &footer);

    // Präsentiere den Frame
    SDL_RenderPresent(app->renderer);

    // Inkrementiere Frame Counter
    app->frame_count++;
}

/**
 * Verarbeite SDL Events (Tastatur, Fenster-Schließung, etc.)
 */
void handle_events(AppState *app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                printf("[EVENT] Quit event received\n");
                app->running = 0;
                break;

            case SDL_EVENT_KEY_DOWN:
                printf("[EVENT] Key pressed: %s\n", SDL_GetKeyName(event.key.key));
                if (event.key.key == SDLK_ESCAPE) {
                    printf("[EVENT] ESC pressed - closing application\n");
                    app->running = 0;
                }
                break;

            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                printf("[EVENT] Window close requested\n");
                app->running = 0;
                break;

            default:
                break;
        }
    }
}

/**
 * Gebe Ressourcen frei und beende SDL
 */
void cleanup(AppState *app) {
    printf("[CLEANUP] Freeing resources...\n");

    if (app->renderer) {
        SDL_DestroyRenderer(app->renderer);
        printf("[CLEANUP] Renderer destroyed\n");
    }
    if (app->window) {
        SDL_DestroyWindow(app->window);
        printf("[CLEANUP] Window destroyed\n");
    }
    SDL_Quit();
    printf("[CLEANUP] SDL quit\n");
}

/**
 * Lese Benutzer-Input vom Terminal
 */
int read_user_input(AppState *app) {
    printf("\n");
    printf("=====================================\n");
    printf("  SDL3 Codespaces Application\n");
    printf("=====================================\n");
    printf("\n");
    printf("Enter your text (max %d characters):\n", MAX_INPUT_SIZE - 1);
    printf("> ");
    fflush(stdout);

    // Lese Input von stdin
    if (fgets(app->user_input, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "[ERROR] Failed to read input from stdin\n");
        return -1;
    }

    // Entferne Newline am Ende
    size_t len = strlen(app->user_input);
    if (len > 0 && app->user_input[len - 1] == '\n') {
        app->user_input[len - 1] = '\0';
    }

    printf("[INPUT] Received: '%s' (%zu characters)\n", app->user_input, strlen(app->user_input));

    return 0;
}

/**
 * Main Entry Point
 */
int main(int argc, char *argv[]) {
    AppState app = {0};
    app.running = 1;
    app.frame_count = 0;

    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║  SDL3 Application Starting...          ║\n");
    printf("║  Running in: GitHub Codespaces        ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n");

    // ===== PHASE 1: Read User Input =====
    printf("[PHASE 1] Reading user input from terminal...\n");
    if (read_user_input(&app) != 0) {
        fprintf(stderr, "[ERROR] Failed to read input\n");
        return 1;
    }

    // ===== PHASE 2: Write Output File =====
    printf("[PHASE 2] Writing output to file...\n");
    if (write_output_file(app.user_input) != 0) {
        fprintf(stderr, "[ERROR] Failed to write output file\n");
        return 1;
    }

    // ===== PHASE 3: Initialize Graphics =====
    printf("[PHASE 3] Initializing SDL3 graphics...\n");
    if (init_graphics(&app) != 0) {
        fprintf(stderr, "[ERROR] Failed to initialize graphics\n");
        return 1;
    }

    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║  Application ready!                   ║\n");
    printf("║  Displaying GUI window...             ║\n");
    printf("║  Press ESC or close window to exit    ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n");

    // ===== PHASE 4: Main Loop =====
    printf("[PHASE 4] Entering main loop...\n");
    
    Uint64 frame_start;
    const int TARGET_FPS = 60;
    const int FRAME_TIME_MS = 1000 / TARGET_FPS;  // ~16.67ms per frame

    while (app.running) {
        frame_start = SDL_GetTicks();

        // Verarbeite Events
        handle_events(&app);

        // Rendere Frame
        render_frame(&app);

        // Frame Rate Limiting
        Uint64 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_TIME_MS) {
            SDL_Delay(FRAME_TIME_MS - frame_time);
        }

        // Debug Info jeden 60. Frame
        if (app.frame_count % 60 == 0) {
            printf("[LOOP] Frame: %d | FPS: ~%d\n", app.frame_count, TARGET_FPS);
        }
    }

    // ===== PHASE 5: Cleanup =====
    printf("[PHASE 5] Closing application...\n");
    cleanup(&app);

    FILE *file = fopen("../data/output.txt", "a");
    fputs("TEST!!!\n", file);
    fclose(file);


    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║  Application closed successfully      ║\n");
    printf("║  Output saved to: data/output.txt     ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("NEWWWWWW\n");
    printf("\n");

    return 0;
}