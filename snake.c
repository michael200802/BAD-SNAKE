#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define xmin 3
#define xmax 116
#define ymin 3
#define ymax 27

typedef struct snake{
    int x;
    int y;
    struct snake *sig;
}nodo;

void showc(int i);

int getpos(int*x,int*y);

void gotoxy(int x, int y);

void borders();

void back(int x, int y);

int menu();

int touch(nodo*snak, int x, int y);

void inter(int*a,int*b);

void printb(nodo*snak, int X, int Y);

void printh(nodo*snak, int X, int Y);

void move();

void fruit(nodo*snak,int *f, int *X, int *Y);

void over(const char* ms);

int rn(int m, int M);

nodo* add(nodo* nod);

void game();

int main()
{
    borders();
    switch (menu())
    {
    case 0:
        game();
        break;
    case 1:
        exit(0);
        break;
    }
    main();
    return 0;
    
}

void showc(int i){
    CONSOLE_CURSOR_INFO CCI;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(h,&CCI);
    CCI.bVisible = i;
    SetConsoleCursorInfo(h,&CCI);
}

int getpos(int*x,int*y){
    CONSOLE_SCREEN_BUFFER_INFO CSBI;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h,&CSBI);
    *x = CSBI.dwCursorPosition.X;
    *y = CSBI.dwCursorPosition.Y;
    return CSBI.dwSize.X;
}

void gotoxy(int x, int y){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwpos;
    dwpos.X = x;
    dwpos.Y = y;
    SetConsoleCursorPosition(h,dwpos);
}

void borders(){
    //X ---> min 3 max 116
    //Y ---> min 3 max 27
    gotoxy(2,2);
    printf("%c",201);
    for(int i = 0; i < 114; i++)
    {
        printf("%c",205);
    }
    printf("%c",187);
    gotoxy(2,28);
    printf("%c",200);
    for(int i = 0; i < 114; i++)
    {
        printf("%c",205);
    }
    printf("%c\n",188);
    gotoxy(0,3);
    for(int i = 0; i < 25; i++)
    {
        printf("  %c\n",186);
    }
    for(int i = 0; i < 25; i++)
    {
        gotoxy(117,i+3);
        printf("%c",186);
    }
    gotoxy(56,16);
    printf("SALIR");
    gotoxy(56,15);
    printf("JUGAR");
}

void back(int x, int y){
    //Borra todo lo que se encuentra
    //Hasta regresar a una parte de la consola
    int X, Y, size = getpos(&X,&Y)-1;
    for(;Y != -1;Y-=1)
    {
        for(;X != -1;X-=1)
        {
            gotoxy(X,Y);
            printf(" ");
            gotoxy(X,Y);
            if(X == x && Y == y)
            {
                return;
            }
        }
        X = size;
    }
}

int menu(){
    
    char key;
    int x, y, i = 0, I;
    getpos(&x,&y);
    do
    {
        //Obtenemos la key
        key = getch();
        //Albergamos la variable i antes de que se modifique
        I = i;

        //Se modifica i si se cumplen ciertos parametros
        if(GetAsyncKeyState(VK_UP) == -32767 || key == 'W' || key == 'w')
        {
            if(i != 0)
            {
                i -= 1;
            }
        }
        else
        {
            if(GetAsyncKeyState(VK_DOWN) == -32767 || key == 'S' || key == 's')
            {
                if(i != 1)
                {
                    i++;
                }
            }
        }
        //Si se modifico i, cambiamos la apariencia del menu
        if(I != i)
        {
            showc(0);
            gotoxy(x,y+i);
            showc(1);
        }

        //Termina cuando se presiona return
    } while (GetAsyncKeyState(VK_RETURN) != -32767);

    showc(1);
    gotoxy(x,y);
    back(56,y);
    gotoxy(x,y+1);
    back(56,y+1);

    return i;
    
}

int touch(nodo*snak, int x, int y){
    
    //Si la cabeza toco los bordes
    if(xmin > x || x > xmax || ymin > y || y > ymax)
    {
        //Regresamos uno, en señal de que se jodio
        return 1;
    }
    //Recorremos toda la lista para ver si la cabeza toco el cuerpo
    //Si el siguiente nodo existe, caso contrario solo existia la cabeza
    if(snak->sig != NULL)
    {
        //Nos movemos al siguiente nodo, (pues la cabeza nunca podria tocar a la anterior parte suya)
        snak = snak->sig;
        //Si la siguiente parte existe, caso contrario no existe
        if(snak->sig != NULL)
        {
            //Nos movemos a la siguiente parte
            snak = snak->sig;
            //Recorremos el resto de la lista
            while (snak != NULL)
            {
                //Si la pos de la cabeza concuerda con la pos de alguna parte
                if(x == snak->x && y == snak->y)
                {
                    //Regresamos uno, en señal de que se jodio
                    return 1;
                }
                //Caso contrario proseguimos
                snak = snak->sig;
            }
            
        }
    }
    //Si ninguna wea mala se confirmo
    //Regresamos 0 en señar de bienestar xd
    return 0;
}

void inter(int*a,int*b){
    //Intercambiamos valores
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

void printb(nodo*snak, int X, int Y){
    //Imprimimos el resto del cuerpo

    //Consite en mover la anterior parte del cuerpo a la pos de la siguiente

    int i, x = snak->x, y = snak->y;

    snak = snak->sig;

    inter(&X,&snak->x);
    inter(&Y,&snak->y);

    if(snak->sig == NULL)
    {
        i = 1;
        inter(&x,&X);
        inter(&y,&Y);
        if(X != snak->x && Y == snak->y)
        {
            i = i*205;
        }
        else
        {
            if(X == snak->x && Y != snak->y)
            {
                i = i*186;
            }
        }

        gotoxy(x,y);
        printf(" ");

        gotoxy(snak->x,snak->y);
        printf("%c",i);

        return;
    }
    
    while (1)
    {
        if(X != snak->x && Y == snak->y)
        {
            i = 205;
        }
        else
        {
            if(X == snak->x && Y != snak->y)
            {
                i = 186;
            }
        }
        
        gotoxy(X,Y);
        printf(" ");
        
        
        gotoxy(snak->x,snak->y);
        printf("%c",i);
        if(snak->sig!=NULL)
        {
            snak = snak->sig;
            inter(&X,&snak->x);
            inter(&Y,&snak->y);
        }
        else
        {
            break;
        }
    }
}

void printh(nodo*snak, int X, int Y){
    //Imprimimos la cabeza, i es el simbolo de la cabeza
    unsigned char i;
    //Vemos si se movio algo (siempre se movera pero por si acaso..)
    if(snak->x != X || Y != snak->y)
    {
        //El simbolo (i) que representara la cabeza cambia segun el tipo de movimiento dado
        if(X != snak->x && Y == snak->y)
        {
            if(X > snak->x)
            {
                i = 17;
            }
            else
            {
                i = 16;
            }
        }
        else
        {
            if(X == snak->x && Y != snak->y)
            {
                if (Y > snak->y)
                {
                    i = 30;
                }
                else
                {
                    i = 31;
                }
                
            }
        }
        //Borramos la anterior cabeza
        gotoxy(X,Y);
        printf(" ");
        //Imprimimos la nueva
        gotoxy(snak->x,snak->y);
        printf("%c",i);
        //Si el siguiente nodo no es null
        //Existe un cuerpo
        if(snak->sig != NULL)
        {
            //Movemos el cuerpo
            printb(snak,X,Y);
        }
    }
}

void move(nodo*snak, int*i, int *X, int *Y){
    //Se mueve

    //Si el user mueve a la snake
    //Cambia la dir de la snake (i)

    *X = snak->x; 
    *Y = snak->y;

    nodo* aux;

    //Si el user mueve a la snake
    //Cambia la dir de la snake (i)
    int d = GetAsyncKeyState('W') != 0 || GetAsyncKeyState(VK_UP) != 0, n = *i != 1 || snak->sig == NULL;

    if(d && n)
    {

        snak->y = snak->y - 1;
        *i = 0;
        
    }
    else
    {
        d = GetAsyncKeyState('S') != 0 || GetAsyncKeyState(VK_DOWN) != 0;
        n = *i != 0 || snak->sig == NULL;
        if (d && n)
        {

            snak->y= snak->y + 1;
            *i = 1;

        }
        else
        {
            d = GetAsyncKeyState('A') != 0 || GetAsyncKeyState(VK_LEFT) != 0;
            n = *i != 3 || snak->sig == NULL;
            if (d && n)
            {
                snak->x= snak->x - 1;
                *i = 2;
            }
            else
            {
                d = GetAsyncKeyState('D') != 0 || GetAsyncKeyState(VK_RIGHT) != 0;
                n = *i != 2 || snak->sig == NULL;
                if (d && n)
                {

                    snak->x = snak->x + 1;
                    *i = 3;

                }
                else
                {
                    //Si el chingado user no movio a la serpiente
                    //Se usa la variable i para mover a la serpiente
                    switch (*i)
                    {
                    case 0:
                        snak->y = snak->y - 1;
                        break;
                    case 1:
                        snak->y= snak->y + 1;
                        break;
                    case 2:
                        snak->x= snak->x - 1;
                        break;
                    case 3:
                        snak->x = snak->x + 1;
                        break;
                        
                    }
                }
            }
        }
    }
    
}

void fruit(nodo*snak,int *f, int *X, int *Y){
    //LA FRUTA
    //no vale la pena explicar esto ._.
    if(*f < 0)
    {
        if(*X == snak->x && *Y == snak->y)
        {
            *f = (*f)*-1;
            snak = add(snak);
        }
        return;
    }
    
    do
    {
        *X = rn(xmin,xmax);
        *Y = rn(ymin,ymax);
        if(touch(snak,*X,*Y))
        {
             
        }
        else
        {
            break;
        }
    } while (1);
    
    gotoxy(*X,*Y);
    printf("%c",254);
    *f = (*f + 1)*-1;

}

void over(const char* ms){
    //GAME OVER PERRA
    system("cls");
    gotoxy(59-strlen(ms)/2,15);
    printf(ms);
    gotoxy(41,16);
    printf("Pulsa cualquier tecla para continuar");
    system("pause>nul");
    system("cls");
    showc(1);
}

int rn(int m, int M){
    //Numero random (no usamos semilla pq es mucha wea por poca recompensa)
    return m + rand()%(M +1 -m);
}

nodo* add(nodo* nod){
    //Añadimos un nodo nuevo
    nodo *aux = (nodo*)malloc(sizeof(nodo)), *aux2;
    aux->sig = NULL;
    //Si no existe ningun nodo, creamos uno que sera la cabeza
    if(nod == NULL)
    {
        //La pos de la cabeza es aleatorio
        aux->x = rn(xmin,xmax);
        aux->y = rn(ymin,ymax);
        nod = aux;
    }
    else
    {
        //Caso contrario, la nueva parte guardara la pos de la anterior parte
        int x, y;
        aux2 = nod;
        while (aux2->sig != NULL)
        {
            aux2 = aux2->sig; 
        }
        x = aux2->x;
        y = aux2->y;
        aux->x = x;
        aux->y = y;
        aux2->sig = aux;
        //Esto no importara debido a que luego la snake se movera y esta parte nisiquiera se tendra que modificar
    }
    //Se regresan los cambios en la lista
    return nod;
}

void game(){
    //Bye cursor
    showc(0);
    //Iniciamos la lista
    nodo *snak = NULL;
    //Creamos la cabeza
    snak = add(snak);
    //Movemos el cursor a la pos de la cabeza
    gotoxy(snak->x,snak->y);
    //Imprimimos la cabeza
    printf("%c",31);

    //d es la direccion (por defecto es 1)
    //x y son las coordenadas pre move() de la cabeza
    //f nos dice cuantas frutas se han comido (por defecto es uno)
    //fx y fy son la pos de la fruta
    int d = 1, x, y, f = 1, fx, fy;
    do
    {
        //La serpiente se mueve
        move(snak,&d,&x,&y);
        //Se comprueba el bienestar de la serpiente (se murio o no)
        if(touch(snak,snak->x,snak->y) == 0)
        {
            //Imprimimos el resto del cuerpo
            printh(snak,x,y);
            //Vemos si creamos una nueva fruta o si ya existe una
            fruit(snak,&f,&fx,&fy);
            //Si la serpiente crecio tanto que ocupo todo el chingado mapa
            if(abs(f) == (xmax-xmin)*(ymax-ymin))
            {
                over("HAS GANADO");
                break;
            }
        }
        else
        {
            //La serpiente ripeo
            over("HAS PERDIDO");
            break;
        }
        //Descansa pc
        Sleep(150);

        //Infinitoooooooo
    }
    while(1);
}
