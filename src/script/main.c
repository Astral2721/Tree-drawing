// Compilation command: gcc main.c function.c -o main.out -lSDL2 -lSDL2main

#include <SDL2/SDL.h>
#include <unistd.h>
#include <time.h>
#include "function.h"


#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 612
#define FPS 60

//position du carre dessiné dans drawGame()
int x = 0;
int y = 0;

//
int isMouseButtonDown = 0;
int toolEquipped = 0;

//
SDL_Surface* screenSurface;

//
int setColorR = 0;
int setColorG = 0;
int setColorB = 0;

// Draws the tree
void drawTree() {
    // Initializing tree parameters
    int animated = 1;
    int animationWaitingTime = 5000; // in µs

    int height = 25;
    int squareSize = 5;

    int leafFloorSize = 3;

    int treeXoffset = 5;
    int treeYoffset = 1;
    int barkYoffset = height * (leafFloorSize - 1) * squareSize;

    int maxWidth = 2 * height - 1;
    int barkHeight = height / 2 * (leafFloorSize - 1);
    int barkWidth = height / 3;

    // Draw tree leaves
    for (int i = 0 + treeYoffset; i < height + treeYoffset; i++) {
        int stars = 2 * (i - treeYoffset) + 1;
        int spaces = (maxWidth - stars) / 2;

        // Draw spaces
        for (int j = 0 + treeXoffset; j < spaces + treeXoffset; j++) {
            /* Not neccessary as 'blank' square == black square
            changeColor(0, 0, 0);  // Black color for spaces
            drawSquare(j * squareSize, i * squareSize, squareSize);
            */
        }

        // Draw leaves
        for (int j = 0 + treeXoffset; j < stars + treeXoffset; j++) {
            changeColor(0, 155, 0);  // Green color for the tree leaves
            // drawSquare((spaces + j) * squareSize, i * squareSize, squareSize);
            for (int k = 0; k < leafFloorSize; k++) {
                drawSquare((spaces + j) * squareSize, i * squareSize * leafFloorSize + squareSize * k, squareSize);
            }
            if (animated) {
                usleep(animationWaitingTime);
                actualize();
            }
        }
    }

    // Draw bark
    for (int i = 0 + treeYoffset; i < barkHeight + treeYoffset; i++) {
        int spaces = (maxWidth - (barkWidth)) / 2;

        // Draw spaces before the bark
        for (int j = 0 + treeXoffset; j < spaces + treeXoffset; j++) {
            /* Not neccessary as 'blank' square == black square
            changeColor(0, 0, 0);  // White color for spaces
            drawSquare(j * squareSize, (height + i) * squareSize, squareSize);
            */
        }

        // Draw bark (3 squares in the middle)
        for (int j = 0 + treeXoffset; j < barkWidth + treeXoffset; j++) {
            changeColor(155, 25, 45);  // Bark color (brownish)
            drawSquare((spaces + j) * squareSize, (height + (i + treeYoffset)) * squareSize + barkYoffset + treeYoffset * squareSize, squareSize);
        }
        if (animated) {
            usleep(animationWaitingTime);
            actualize();
        }
    }

}

// Draws the buttons
void drawButtons() {
    // Init buttons parameters
    //NOTE: BUTTONS ARE DEPRACATED.
    int rows = 0;
    int columns = 0;

    char* imagesPathsArray[] = {"../img/Brush.bmp", "../img/RollerBrush.bmp", "../img/Arrow.bmp"};
    char* keybinds[] = {"1", "2", "R"};
    //int ArrayLength = rows + columns - 1;

    int offsetX = -5;
    int offsetY = 5;

    int imagesPaddingX = 10;
    int imagesPaddingY = 10;

    int screenSizeX = WINDOW_WIDTH;

    int imageSize = 90;

    int startPositionX = screenSizeX - columns * imageSize + offsetX - (columns - 1) * imagesPaddingX;
    int startPositionY = offsetY;

    // Draw images
    int imageIndex = 0;
    for (int i = 0 + startPositionY; i < rows + startPositionY; i++) {
        for (int j = 0 + startPositionX; j < columns + startPositionX; j++) {
            if (imagesPathsArray[imageIndex]) {
                sprite(
                startPositionX + imageSize * (j - startPositionX) + (j - startPositionX) * imagesPaddingX,
                startPositionY + imageSize * (i - startPositionY) + (i - startPositionY) * imagesPaddingY,
                imagesPathsArray[imageIndex]
                );
                //TODO: ADD TEXT & KEYBINDS HERE
                imageIndex++;
            }
        }
    }

}

void init_game(){
    //mettre votre code d'initialisation ici
    //clear();
    drawTree();
    drawButtons();
    updateWindowSurface();
}


/* Functions for recursiveColorWindow */
// Keep track of visited positions
#define MAX_VISITS 100000

int visited[MAX_VISITS][2]; // Array to store visited positions
int visit_count = 0;

// Function to check if the position has been visited
int isVisited(int x, int y) {
    for (int i = 0; i < visit_count; i++) {
        if (visited[i][0] == x && visited[i][1] == y) {
            return 1; // Visited
        }
    }
    return 0; // Not visited
}

// Function to mark a position as visited
void markVisited(int x, int y) {
    visited[visit_count][0] = x;
    visited[visit_count][1] = y;
    visit_count++;
}

void recursiveColorWindow(int mousePosX, int mousePosY) {
    int animBucket = 1;
    int sqSize = 25;
    int animBucketWaitTime = 2000; // In µs

    // Check if the position has been visited
    if (isVisited(mousePosX, mousePosY)) {
        return; // Stop recursion for visited positions
    }

    // Mark the current position as visited
    markVisited(mousePosX, mousePosY);

    // Draw the current square
    drawSquare(mousePosX, mousePosY, sqSize);
    actualize();
    if (animBucket) {
        usleep(animBucketWaitTime);
    }

    // Recursion
    if (mousePosX - sqSize > - sqSize) { // Left
        recursiveColorWindow(mousePosX - sqSize, mousePosY);
    }
    if (mousePosY - sqSize > - sqSize) { // Up
        recursiveColorWindow(mousePosX, mousePosY - sqSize);
    }
    if (mousePosX + sqSize < WINDOW_WIDTH) { // Right
        recursiveColorWindow(mousePosX + sqSize, mousePosY);
    }
    if (mousePosY + sqSize < WINDOW_HEIGHT) { // Down
        recursiveColorWindow(mousePosX, mousePosY + sqSize);
    }
}



void colorSquare(int mousePosX, int mousePosY) {
    int drawSize = 5;
    isMouseButtonDown = 1;

    changeColor(setColorR, setColorG, setColorB);
    drawSquare(mousePosX, mousePosY, drawSize);
}


void onClickPress(int mousePosX, int mousePosY){

    switch (toolEquipped) {
        case 0:
            break;
        case 1:
            colorSquare(mousePosX, mousePosY);
            break;
        case 2:
            if (isMouseButtonDown) {
                break;
            }
            changeColor(setColorR, setColorG, setColorB);
            recursiveColorWindow(mousePosX, mousePosY);
            clear();
            changeColor(setColorR, setColorG, setColorB);
            drawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            init_game();
            break;
    }

}

void onClickRelease() {
    isMouseButtonDown = 0;
}

void drawGame(){
    /* Ici je dessine mon jeu
     * exemple position x, y modifiés dans KeyPressed() et utilisés pour
     * pouvoir deplacer la figure à chaque boucle de gameLoop()
     */
    //drawSquare(x,y,200);
    //changeColor(255,255,0);
    //drawCircle(100,100,100);
    actualize();
    if (isMouseButtonDown) {
        int mousePosX, mousePosY;
        SDL_GetMouseState(&mousePosX, &mousePosY);
        onClickPress(mousePosX, mousePosY);
    }
    usleep(1000000 / FPS); // 60 images par seconde | 1000000 = 1 seconde
}

void getRandomColor() {
    setColorR = rand() % 255;
    setColorG = rand() % 255;
    setColorB = rand() % 255;
    changeColor(setColorR, setColorG, setColorB);
    drawRect(WINDOW_WIDTH - 35, WINDOW_HEIGHT - 15, 30, 10);
}

void KeyPressed(SDL_Keycode touche){
    /** @brief event.key.keysym.sym renvoi la touche appuyé
     *
     */
    switch (touche) {
        // Voir doc SDL_Keycode pour plus de touches https://wiki.libsdl.org/SDL_Keycode
        // ici exemple ou appuyer sur les touches Q | D modifie x (position du carré dans drawGame())
        case SDLK_0:
            printf("0");
            toolEquipped = 0;
            break;
        case SDLK_1:
            //touche 1 appuyé
            printf("1");
            toolEquipped = 1;
            break;
        case SDLK_2:
            printf("2");
            toolEquipped = 2;
            break;
        case SDLK_r:
            // Reset drawing
            printf("R");
            init_game();
            break;
        case SDLK_c:
            printf("C");
            getRandomColor();
            break;
        case SDLK_ESCAPE:
            freeAndTerminate();
            break;
        default:
            break;
    }
}
void joyButtonPressed(int mousePosX, int mousePosY){
}

void gameLoop() {
    int programLaunched = 1;
    while (programLaunched == 1) {
        // Boucle pour garder le programme ouvert
        // lorsque programLaunched est different de 1
        // on sort de la boucle, donc de la fonction
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // boucle pour la gestion d'évenement
            switch (event.type) {
                /** event.type renvoi le type d'evenement qui se passe
                 * (si on appuie sur une touche, clique de souris...)
                 * en fonction du type d'évènement on à alors
                 * différentes information accessibles
                 * voir doc pour plus d'event https://wiki.libsdl.org/SDL_EventType
                 */
                case SDL_QUIT:
                    // quand on clique sur fermer la fénêtre en haut à droite
                    programLaunched = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    /* clique de la souris
                     * event.motion.y | event.motion.x pour les positions de la souris
                     */
                    onClickPress(event.motion.x, event.motion.y);
                    printf("position de la souris x : %d , y : %d\n", event.motion.x, event.motion.y);
                    break;
                case SDL_MOUSEBUTTONUP:
                    onClickRelease();
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                case SDL_JOYBUTTONDOWN:
                    break;
                default:
                    break;
            }
        }
        /* coder ici pour que le code s'execute après chaque évenement
         * exemple dessiner un carré avec position int x, int y qu'on peut
         * deplacer lorsqu'on appuie sur une touche
         */
        drawGame();
    }
}

int main(){
    srand(time(NULL));

    /** @description 3 fonctions dans le main qui permettent de créer l'application et la maintenir ouverte :
     *  init(...) : initialiser la SDL/ fenêtre
     *  gameLoop() : boucle de jeu dans laquelle l'application reste ouverte
     *  freeAndTerminate() : quitte le programme proprement
     */
    init(WINDOW_WIDTH, WINDOW_HEIGHT);
    init_game();
    gameLoop();
    printf("Fin du programme\n");
    freeAndTerminate();
}
