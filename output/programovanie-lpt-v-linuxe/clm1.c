///////////////////////////////////////////////
// Programovanie LPT v Linuxe - priklad c. 1 //
//                www.jariq.sk               //
///////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define BASEPORT 0x378

int main(void)
{

    // Ziskanie prav pre pristup k portu
    if (ioperm(BASEPORT, 3, 1))
    {
        perror("Nepodarilo sa ziskat pristup k LPT");
        exit(EXIT_FAILURE);
    }

    // Postupne rozsvietenie LED
    outb(0, BASEPORT);
    sleep(1);
    outb(1, BASEPORT);
    sleep(1);
    outb(3, BASEPORT);
    sleep(1);
    outb(7, BASEPORT);
    sleep(1);
    outb(15, BASEPORT);
    sleep(1);
    outb(31, BASEPORT);
    sleep(1);
    outb(63, BASEPORT);
    sleep(1);
    outb(127, BASEPORT);
    sleep(1);
    outb(255, BASEPORT);

    // Uvolnenie prav pre pristup k portu
    if (ioperm(BASEPORT, 3, 0))
    {
        perror("Nepodarilo sa uvolnit pristup k LPT");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}
