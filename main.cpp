#include <string>
#include <vector>
#include "include/raylib.h"
#include <time.h>

using namespace std;

const int DIM_CELLA = 50;
const int N_CELLE = 16;
const float screenWidth = DIM_CELLA * N_CELLE;
const float screenHeight = DIM_CELLA * N_CELLE;

int campo[N_CELLE][N_CELLE];
int allunga = 0;
bool melapresente = false;
bool sceltauscita = false;
int stato = -1;
Texture2D Texturemela;

struct Pulsante {
    Rectangle rec;
    Color colore;
    const char* testo;
};

Pulsante bottoneinizio { { screenWidth / 2 - 150, screenHeight / 2 - 60, 300, 110 }, BLUE, "Inizio" };
Pulsante bottoneimpostazioni { { screenWidth / 2 - 150, screenHeight / 2 + 70, 300, 110 }, YELLOW, "Settings" };
Pulsante riprova = { { screenWidth / 2 - 150, screenHeight / 2 - 60, 300, 110 }, GREEN, "Riprova" };
Pulsante esci1 = { { screenWidth / 2 - 150, screenHeight / 2 + 70, 300, 110 }, RED, "Esci" };
Pulsante esci2 = { { screenWidth / 2 - 150, screenHeight / 2 + 200, 300, 110 }, RED, "Esci" };

struct Coordinate {
    int x;
    int y;
};

vector<Coordinate> serpente;

bool allungamento(int campo[][N_CELLE], int x, int y) {
    return campo[x][y] == 2;
}

bool morte(int campo[][N_CELLE], int x, int y) {
    if (x < 0 || y < 0 || x > N_CELLE - 1 || y > N_CELLE - 1) return true;
    if (campo[x][y] == 1) return true;
    return false;
}

void inizio(int &dx, int &dy) {
    serpente.clear();

    for (int i = 0; i < N_CELLE; i++) {
        for (int j = 0; j < N_CELLE; j++) {
            campo[i][j] = 0;
        }
    }
    dx = 0;
    dy = -1;
    melapresente = false;

    int testaX = 8;
    int testaY = 8;
    for (int i = 0; i < 5; ++i) {
        serpente.push_back({testaX, testaY + i});
        campo[testaX][testaY + i] = 1;
    }
}

void disegna() {
    for (int i = 0; i < N_CELLE; ++i) {
        for (int j = 0; j < N_CELLE; ++j) {
            Color c;

            if (campo[i][j] == 0) {
                c = DARKGREEN;
                DrawRectangle(i * DIM_CELLA + 2.5, j * DIM_CELLA + 5, DIM_CELLA - 5, DIM_CELLA - 5, c);
            }
            else if (campo[i][j] == 1) {
                c = RED;
                DrawRectangle(i * DIM_CELLA + 2.5, j * DIM_CELLA + 5, DIM_CELLA - 5, DIM_CELLA - 5, c);
            }
            else if (campo[i][j] == 2) {
                c = DARKGREEN;
                DrawRectangle(i * DIM_CELLA + 2.5, j * DIM_CELLA + 5, DIM_CELLA - 5, DIM_CELLA - 5, c);

                DrawTexture(Texturemela, i * DIM_CELLA + 2.5, j * DIM_CELLA + 5, WHITE);
            }
        }
    }
    for (int i = 0; i < screenHeight; i += DIM_CELLA) {
        DrawLine(i, 0, i, screenHeight, BLACK);
        DrawLine(0, i, screenWidth, i, BLACK);
    }
}

void menuinizio() {
    ClearBackground(BLACK);
    DrawRectangleRec(bottoneinizio.rec, bottoneinizio.colore);
    DrawRectangleRec(bottoneimpostazioni.rec, bottoneimpostazioni.colore);
    DrawRectangleRec(esci2.rec, esci2.colore);
    DrawText(esci2.testo, esci2.rec.x + 44, esci2.rec.y + 10, 100, BLACK);
    DrawText(bottoneimpostazioni.testo, bottoneimpostazioni.rec.x + 10, bottoneimpostazioni.rec.y + 22, 67, BLACK);
    DrawText("Gioca", screenWidth / 2 - 130, screenHeight / 2 - 50, 100, BLACK);
    DrawText("SNAKE", screenWidth / 2 - 340, screenHeight / 2 - 290, 200, WHITE);

}

int main() {
    string s = "snake ";
    srand(time(NULL));
    InitWindow(screenWidth, screenHeight, s.c_str());
    SetTargetFPS(60);
    Image imgMela = LoadImage("mela.png");
    ImageResize(&imgMela, DIM_CELLA, DIM_CELLA);
    Texturemela = LoadTextureFromImage(imgMela);

    int conta = 0, dx = 0, dy = -1;
    inizio(dx, dy);

    while (!WindowShouldClose() && !sceltauscita) {
        if (stato == 1) {
            if (!melapresente) {
                int a, b;
                do {
                    a = rand() % N_CELLE;
                    b = rand() % N_CELLE;
                } while (campo[a][b] != 0);
                campo[a][b] = 2;
                melapresente = true;
            }

            if (IsKeyPressed(KEY_UP) && dy != 1) {
                dx = 0; dy = -1;
            }
            if (IsKeyPressed(KEY_DOWN) && dy != -1) {
                dx = 0; dy = 1;
            }
            if (IsKeyPressed(KEY_LEFT) && dx != 1) {
                dx = -1; dy = 0;
            }
            if (IsKeyPressed(KEY_RIGHT) && dx != -1) {
                dx = 1; dy = 0;
            }

            if (conta % 20 == 0) {
                Coordinate nuovaTesta = serpente.at(0);
                nuovaTesta.x += dx;
                nuovaTesta.y += dy;

                if (morte(campo, nuovaTesta.x, nuovaTesta.y)) {
                    stato = 0;
                }

                if (!morte(campo, nuovaTesta.x, nuovaTesta.y)) {
                    if (allungamento(campo, nuovaTesta.x, nuovaTesta.y)) {
                        allunga = 1;
                        melapresente = false;
                    }

                    campo[nuovaTesta.x][nuovaTesta.y] = 1;
                    serpente.insert(serpente.begin(), nuovaTesta);

                    if (allunga != 1) {
                        campo[serpente.back().x][serpente.back().y] = 0;
                        serpente.pop_back();
                    }

                    allunga = 0;
                }
            }
        }

        BeginDrawing();
        ClearBackground(DARKGREEN);

        if (stato == -1) {
            menuinizio();
            if (CheckCollisionPointRec(GetMousePosition(), bottoneinizio.rec)) {
                bottoneinizio.colore = DARKBLUE;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    stato = 1;
                }
            } else {
                bottoneinizio.colore = BLUE;
            }
            if (CheckCollisionPointRec(GetMousePosition(), esci2.rec)) {
                esci2.colore = MAROON;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    break;
                }
            } else {
                esci2.colore = RED;
            }
            if (CheckCollisionPointRec(GetMousePosition(), bottoneimpostazioni.rec)) {
                bottoneimpostazioni.colore = GOLD;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    stato = 2;
                }
            } else {
                bottoneimpostazioni.colore = YELLOW;
            }
        }

        if (stato == 1) {
            disegna();
        }

        if (stato == 0) {
            ClearBackground(BLACK);

            if (CheckCollisionPointRec(GetMousePosition(), riprova.rec)) {
                riprova.colore = LIME;
            } else {
                riprova.colore = GREEN;
            }

            if (CheckCollisionPointRec(GetMousePosition(), esci1.rec)) {
                esci1.colore = MAROON;
            } else {
                esci1.colore = RED;
            }

            DrawRectangleRec(riprova.rec, riprova.colore);
            DrawText(riprova.testo, riprova.rec.x + 15, riprova.rec.y + 22, 67, BLACK);

            DrawRectangleRec(esci1.rec, esci1.colore);
            DrawText(esci1.testo, esci1.rec.x + 44, esci1.rec.y + 10, 100, BLACK);

            if (CheckCollisionPointRec(GetMousePosition(), esci1.rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                break;
            } else if (CheckCollisionPointRec(GetMousePosition(), riprova.rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                stato = 1;
                inizio(dx, dy);
            }
        }

        EndDrawing();
        conta++;
    }

    CloseWindow();
    return 0;
}