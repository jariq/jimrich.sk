///////////////////////////////////////////////
// Programovanie LPT v Linuxe - priklad c. 2 //
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
    outb(11, BASEPORT+2);
    sleep(1);
    outb(1, BASEPORT);
    outb(11, BASEPORT+2);
    sleep(1);
    outb(3, BASEPORT);
    outb(11, BASEPORT+2);
    sleep(1);
    outb(7, BASEPORT);
    outb(11, BASEPORT+2);
    sleep(1);
    outb(15, BASEPORT);
    outb(11, BASEPORT+2);
    sleep(1);
    outb(31, BASEPORT);
    outb(11, BASEPORT+2);
    sleep(1);
    outb(63, BASEPORT);
    outb(11, BASEPORT+2);
    sleep(1);
    outb(127, BASEPORT);
    outb(11, BASEPORT+2);
    sleep(1);
    outb(255, BASEPORT);
    outb(11, BASEPORT+2);
    sleep(1);
    outb(255, BASEPORT);
    outb(10, BASEPORT+2);
    sleep(1);
    outb(255, BASEPORT);
    outb(8, BASEPORT+2);

    // Uvolnenie prav pre pristup k portu
    if (ioperm(BASEPORT, 3, 0))
    {
        perror("Nepodarilo sa uvolnit pristup k LPT");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}
