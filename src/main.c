#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <allegro.h>
#include "ptask.h" 
#include <math.h>
//-------------------------------------------------------------
// GLOBAL CONSTANTS
//-------------------------------------------------------------
#define XWIN 640
// window x resolution
#define YWIN 480
// window y resolution
#define XBASE 40
// X start for the message
#define YBASE 50
// Y level for the first task
#define YINC 30
// Y increment for the other tasks
#define BKG 0
// background color
//-------------------------------------------------------------
#define N 10
// max number of tasks
#define LEN 80
// max message length
#define PER 30
// base period
#define PINC 20
// period increment
#define Vzero 30
// initial speed ball
#define CANNON_X 60
// X position cannon
#define CANNON_Y 60
// Y position cannon
#define G 9.8
// Gravity
#define T 500
// Trajectory period
//-------------------------------------------------------------
// GLOBAL VARIABLES
//-------------------------------------------------------------
int end = 0;
// end flag
char mes[N][LEN+1];
// buf for MAXT mes of length LEN

struct point{
    double x;
    double y;
};

struct point tmp[XWIN];

void display_init()
{
    allegro_init();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    clear_to_color(screen, BKG);
   // textout_ex(screen, font, "Table", 300, 200, 12, 14);
    
    install_keyboard();

}

/*void* hello(void* arg)
{
int i, k = 0;
// task and character index
int x, y;
char buf[2];
// temp buffer for 1 character string
i = task_argument(arg);
sprintf(mes[i], "I'M TASK %d, T = %d", i, task_period(i));
while (!end) {
x = XBASE + k*8;
y = YBASE + i*YINC;
sprintf(buf, "%c", mes[i][k]);
textout_ex(screen, font, buf, x, y, 2+i, BKG);
k = k + 1;
if (mes[i][k] == '\0') {
k = 0;
textout_ex(screen, font, mes[i], XBASE, y, BKG, BKG);
}
wait_for_period(i);
}
}
*/

void trajectory_calc2(double angle)
{
    int i, col, dcol = 0; /* indice del task  */
    int x, y;             /* coordinate grafiche pallina  */
    int ox, oy;           /* vecchia posizione pallina    */
    int x0;               /* posizione iniziale X pallina */
    float vx, vy;         /* velocità della pallina   */
    float t, tx;          /* variabile temporale      */
    // float	z, z0;		/* posizione della pallina  */
    float dt; /* incremento temporale     */
    double a; /* variabile di appoggio    */
    // long	j, k;

    i = ptask_get_index();
    col = 2 + i % 14;
    y = oy = (CANNON_Y);
    x = ox = x0 = CANNON_X;
    int j = 0;
    a = 2. * G * (float)(CANNON_Y);
    vy = sqrt(a);
    vx = Vzero * cos(angle);
    tx = 0.0;
    t = vy / G;
    //dt = ptask_get_period(i, MILLI) / 100.;
    dt = T / 100;
    while (x <= XWIN) {
        x = x0 + (vx * tx);
        y = YBASE + vy * t - .5 * G * t * t;
        if (y < YBASE) {
            t = 0.0;
            //pthread_mutex_lock(&mxv);
            //v0[i] = .9 * v0[i];
            //vy = v0[i];
            //pthread_mutex_unlock(&mxv);
            //y = YBASE + vy * t - .5 * G * t * t;
            y = YBASE;
        }
        /*if (x > XWIN) {
            tx = 0.0;
            x0 = XWIN;
            vx = -vx;
            x = x0 + vx * tx;
        }
        if (x < 0) {
            tx = 0.0;
            x0 = 0;
            vx = -vx;
            x = x0 + vx * tx;
        } */
        printf("X val: %d\n",x);
        tmp[j].x = x;
        printf("Y val: %d\n",y);
        tmp[j].y = y;
        putpixel(screen, tmp[i].x, YWIN - XBASE - tmp[i].y, 15);
        oy = y;
        ox = x;
        t += dt;
        tx += dt;
        sleep(1);
    }
}



void trajectory_calc(double angle)
{
    double x_old, y_old, x_new, y_new;
    int i=0;
    double xzero, yzero;
   
    double Vx = Vzero * cos(angle);
    double Vy = Vzero * sin(angle);
    double Vy_new;
    x_old = y_old = 0;
    x_new=xzero = CANNON_X;
    y_new=yzero = CANNON_Y;
    double dt;
    dt = T / 100;
    //t[CANNON_X][CANNON_Y]=5
    while(x_new + CANNON_X <= XWIN)
    {
        
        //x_old = x_new;
        //y_old = y_new;
        //x_new = x_old + (Vx * (x_new - CANNON_X)); 
        //x_new +=0.0001;
        /* x_new = xzero + (Vx * dt); 
        y_new = yzero +   
            (Vy * dt) - (0.5 * G * dt * dt);
            */
        x_new += 1;
        y_new = yzero + (Vy * x_new) - (0.5 * G * x_new * x_new);
        tmp[i].x = x_new;
        printf("X_new: %f\n",tmp[i].x);
        /*if(y_new <= 50)
        {
            y_new = 50;
        }*/
        tmp[i].y = y_new;
        printf("Y_new: %f\n",tmp[i].y);
        Vy_new = Vy - (G * x_new);
        Vy = Vy_new;
        i++;
        //dt = dt + 0.1;
       // sleep(1);
    }
    i  = 0;
    printf("Print  Trajectory\n");
    while(i < XWIN)
    {
        printf("X: %f   Y: %f\n", tmp[i].x, tmp[i].y);
        //sleep(0.4);
        i++;
    }

}


int main(void)
{
    int c, k;

    display_init();

    do
    {
        k = 0;
        if (keypressed())
        {
            c = readkey();
            k = c >> 8;
            //double ang = rand() % 90;
            if (k == KEY_SPACE)
            {
                printf("Eseguo traiettoria\n");
                //task_create(k,hello,PER+k*PINC,PER+k*PINC,50,0);
                //k++;
                trajectory_calc2(45);
                for(int i = 0; i<XWIN;i++)
                {
                   putpixel(screen, tmp[i].x, YWIN - XBASE - tmp[i].y, 15);
                }
            }
        }
    } while (k != KEY_ESC);

    end = 1;
    allegro_exit();
    return 0;
}
