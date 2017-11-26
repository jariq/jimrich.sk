///////////////////////////////////////////////
// Programovanie LPT v Linuxe - priklad c. 6 //
//                www.jariq.sk               //
///////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <signal.h>
#include <syslog.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define BASEPORT 0x378

void daemonizacia(void);
void spracovanie_signalu(int signal);
int vytazenie_cpu(void);
void rozsviet_led(int usage);

bool signal_prijaty = false;


int main(void)
{

    daemonizacia();

    // Pripojenie k syslogu
    openlog("CLM", LOG_PID, LOG_DAEMON);    

    // Odoslanie spravy na syslog
    syslog(LOG_INFO, "CLM daemon spusteny..\n");

    // Ziskanie prav pre pristup k portu
    if (ioperm(BASEPORT, 3, 1))
    {
        syslog(LOG_ERR, "Nepodarilo sa ziskat pristup k LPT\n");
        exit(EXIT_FAILURE);
    }

    // Priradenie obsluhy signalu SIGTERM
    signal(SIGTERM, spracovanie_signalu);

    // Hlavny cyklus programu
    while (!signal_prijaty)
    {
        rozsviet_led(vytazenie_cpu());
        sleep(1);
    }
    
    // Signal ukoncil cyklus - zhasenie LED
    outb(0, BASEPORT);
    outb(11, BASEPORT+2);

    // Uvolnenie prav pre pristup k portu
    if (ioperm(BASEPORT, 3, 0))
    {
        syslog(LOG_ERR, "Nepodarilo sa uvolnit pristup k LPT\n");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_INFO,"CLM daemon zastaveny..\n");
    
    // Odpojenie od syslogu
    closelog();

    exit(EXIT_SUCCESS);

}


// Funkcia na daemonizaciu procesu
void daemonizacia(void)
{
    pid_t pid, sid;
    
    // Ak je rodicovskym procesom tohto programu proces init, ktoreho PID je vzdy 1,
    // tak tento program uz je daemon, a preto sa vykonavanie funkcie ukonci 
    if (getppid() == 1)
    {
        return;
    }

    // Oddelenie detskeho procesu
    pid = fork();
    
    // Nepodarilo sa oddelit detsky proces
    // Rodicovsky proces sa ukonci s chybou
    if (pid < 0)
    {
        perror("Nepodarilo sa oddelit detsky proces");
        exit(EXIT_FAILURE);
    }
    
    // Podarilo sa oddelit detsky proces
    // Rodicovsky sa moze ukoncit
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // Odtialto je kod vykonavany pod detskym procesom (daemonom)

    // Zmena predvolenych pristupovych prav k vytvaranym suborom
    umask(0);

    // Vytvorenie novej session
    sid = setsid();
    
    // Nepodarilo sa vytvorit session
    // Proces sa ukonci s chybou
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Zmena aktualneho adresara na korenovy
    if ((chdir("/")) == -1)
    {
        exit(EXIT_FAILURE);
    }

    // Presmerovanie stdin, stdout a stderr na /dev/null
    if ((freopen( "/dev/null", "r", stdin)) == NULL)
    {
        exit(EXIT_FAILURE);
    }
    
    if ((freopen( "/dev/null", "w", stdout)) == NULL)
    {
        exit(EXIT_FAILURE);
    }
    
    if ((freopen( "/dev/null", "w", stderr)) == NULL)
    {
        exit(EXIT_FAILURE);
    }

}


// Funkcia spustena po prijaty signalu SIGTERM
void spracovanie_signalu(int signal)
{
    signal_prijaty = true;
}


// Funkcia na zistenie vytazenia CPU
// -----------------------------------------------------------------------
// Funkcia je zalozena na kode aplikacie wmcpuload, ktorej autorom
// je Seiichi SATO <ssato@sh.rim.or.jp> a je sirena pod GNU/GPL licenciou.
// Principialne je tato funkcia vysvetlena aj na adrese
// http://www.experts-exchange.com/Programming/
//        Programming_Platforms/Linux_Programming/Q_20734330.html
// -----------------------------------------------------------------------
int vytazenie_cpu(void)
{
    
    int user, nice, system, idle;
    int used, subtotal;
    static int prevused, prevsubtotal;
    float usage;

    FILE *fr;
    
    if (!(fr = fopen("/proc/stat", "r")))
    {
        syslog(LOG_ERR, "Nepodarilo sa otvorit subor /proc/stat\n");        
        exit(EXIT_FAILURE);
    }
    
    if ((fscanf(fr, "%*s %d %d %d %d", &user, &nice, &system, &idle)) == EOF)
    {
        syslog(LOG_ERR, "Nepodarilo sa precitat udaje zo suboru /proc/stat\n");
        exit(EXIT_FAILURE);
    }
    
    if ((fclose(fr)) == -1)
    {
        syslog(LOG_ERR, "Nepodarilo sa zatvorit subor /proc/stat\n");
        exit(EXIT_FAILURE);
    }

    used = user + system + nice;
    subtotal = user + nice + system + idle;
    usage = (100 * (double)(used - prevused)) / (double)(subtotal - prevsubtotal);

    prevused = used;
    prevsubtotal = subtotal;

    usage = (int) usage;
    return usage;
   
}


// Funkcia na rozsvietenie diod v zavislosti od hodnoty parametra usage
void rozsviet_led(int usage)
{
    
    if (usage <= 10)
    {
        outb(1, BASEPORT);
        outb(11, BASEPORT+2);
    }
    
    if ((usage > 10) && (usage <= 20))
    {
        outb(3, BASEPORT);
        outb(11, BASEPORT+2);
    }
    
    if ((usage > 20) && (usage <= 30))
    {
        outb(7, BASEPORT);
        outb(11, BASEPORT+2);
    }
    
    if ((usage > 30) && (usage <= 40))
    {
        outb(15, BASEPORT);
        outb(11, BASEPORT+2);
    }
    
    if ((usage > 40) && (usage <= 50))
    {
        outb(31, BASEPORT);
        outb(11, BASEPORT+2);
    }
    
    if ((usage > 50) && (usage <= 60))
    {
        outb(63, BASEPORT);
        outb(11, BASEPORT+2);
    }
    
    if ((usage > 60) && (usage <= 70))
    {
        outb(127, BASEPORT);
        outb(11, BASEPORT+2);
    }
    
    if ((usage > 70) && (usage <= 80))
    {
        outb(255, BASEPORT);
        outb(11, BASEPORT+2);
    }
    
    if ((usage > 80) && (usage <= 90))
    {
        outb(255, BASEPORT);
        outb(10, BASEPORT+2);
    }
    
    if (usage > 90)
    {
        outb(255, BASEPORT);
        outb(8, BASEPORT+2);
    }
    
}
