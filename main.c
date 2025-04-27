#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct {
    char nume[50];
    int capacitate;
    int liber; // 1 = disponibila, 0 = indisponibila
} sala;

sala sali[101];
int nrSali = 0;

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}

void citire_fisier(const char *file) {
    clear_screen();
    FILE *fisier = fopen(file, "r");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului %s: %s\n", file, strerror(errno));
        return;
    }

    while (nrSali < 101 && fscanf(fisier, "%49[^,],%d,%d", sali[nrSali].nume, &sali[nrSali].capacitate, &sali[nrSali].liber) == 3) {
        nrSali++;
        int c;
        while ((c = fgetc(fisier)) != EOF && c != '\n');
    }

    if (nrSali == 101) {
        printf("\033[38;2;0;153;0mAtentie: Numarul maxim de sali a fost atins.\033[0m\n");
    }

    fclose(fisier);
}

void display_menu() {
    printf("\033[38;2;0;102;51m=====================================\033[0m\n");
    printf("\033[38;2;0;102;51m       Meniu Administrare Sali       \033[0m\n");
    printf("\033[38;2;0;102;51m=====================================\033[0m\n");
    printf("1. Adaugare sala\n");
    printf("2. Stergere sala\n");
    printf("3. Afisare sali disponibile\n");
    printf("4. Afisare rezervari\n");
    printf("5. Cautare sala (dupa nume, capacitate, disponibilitate)\n");
    printf("6. Rezervare sala\n");
    printf("7. Anulare rezervare\n");
    printf("8. Iesire\n");
    printf("\033[38;2;0;102;51m=====================================\033[0m\n");
    printf("Alege o optiune: ");
}

void adaugare_sala() {
    printf("\033[38;2;0;102;51m=============\033[0m\n");
    printf("\033[38;2;0;102;51mAdaugare sala\033[0m\n");
    printf("\033[38;2;0;102;51m=============\033[0m\n");

    if (nrSali == 101) {
        printf("\033[38;2;0;153;0mAtentie: Numarul maxim de sali a fost atins.\033[0m\n");
    } else {
        printf("\033[38;2;51;204;102mIntroduceti datele necesare...\033[0m\n");

        printf("\033[38;2;102;255;102mIntroduceti numele salii:\033[0m\n");
        fflush(stdin);
        fgets(sali[nrSali].nume, sizeof(sali[nrSali].nume), stdin);
        if (strlen(sali[nrSali].nume) > 0 && sali[nrSali].nume[strlen(sali[nrSali].nume) - 1] == '\n')
            sali[nrSali].nume[strlen(sali[nrSali].nume) - 1] = '\0';

        printf("\033[38;2;102;255;102mIntroduceti capacitatea salii:\033[0m\n");
        scanf("%d", &sali[nrSali].capacitate);

        int valid = 0;
        while (!valid) {
            printf("\033[38;2;102;255;102mEste sala disponibila pentru rezervare? (1 = Da, 0 = Nu):\033[0m\n");
            if (scanf("%d", &sali[nrSali].liber) != 1 || (sali[nrSali].liber != 0 && sali[nrSali].liber != 1)) {
                printf("\033[38;2;255;0;0mValoare invalida! Introduceti 1 pentru Da sau 0 pentru Nu.\033[0m\n");
                while (getchar() != '\n'); // goleste bufferul
            } else {
                valid = 1;
            }
        }

        printf("\033[38;2;0;204;102mSala \033[38;2;153;255;153m%s\033[0m \033[38;2;0;204;102ma fost adaugata.\033[0m\n", sali[nrSali].nume);

        FILE *fisier = fopen("sali.txt", "a");
        if (fisier == NULL) {
            printf("\033[38;2;0;153;0mEroare la deschiderea fisierului.\033[0m\n");
            return;
        }
        fprintf(fisier, "%s,%d,%d\n", sali[nrSali].nume, sali[nrSali].capacitate, sali[nrSali].liber);
        fclose(fisier);

        nrSali++;
    }
}

void stergere_sala() {
    printf("\033[38;2;0;102;51m=============\033[0m\n");
    printf("\033[38;2;0;102;51mStergere sala\033[0m\n");
    printf("\033[38;2;0;102;51m=============\033[0m\n");
}

void afis_sala_liber() {
    clear_screen();
    printf("\033[38;2;0;102;51m========================\033[0m\n");
    printf("\033[38;2;0;102;51mAfisare sali disponibile\033[0m\n");
    printf("\033[38;2;0;102;51m========================\033[0m\n");
    printf("%-5s %-30s %-15s\n", "Nr.", "Nume", "Capacitate");

    int gasit = 0;
    for (int i = 0; i < nrSali; i++) {
        if (sali[i].liber > 0) {
            printf("%-5d %-30s %-15d\n", i + 1, sali[i].nume, sali[i].capacitate);
            gasit = 1;
        }
    }

    if (!gasit) {
        printf("Nu exista sali disponibile!\n");
    }
}


void afis_rezervari() {
    printf("\033[38;2;0;102;51m=================\033[0m\n");
    printf("Afisare Rezervari\n");
    printf("\033[38;2;0;102;51m=================\033[0m\n");
}

void cautare_sala() {
    printf("\033[38;2;0;102;51m============\033[0m\n");
    printf("Cautare sala\n");
    printf("\033[38;2;0;102;51m============\033[0m\n");
}

void rezervare_sala() {
    printf("\033[38;2;0;102;51m==============\033[0m\n");
    printf("Rezervare sala\n");
    printf("\033[38;2;0;102;51m==============\033[0m\n");
}

void anulare_rezervare() {
    printf("\033[38;2;0;102;51m=================\033[0m\n");
    printf("Anulare rezervare\n");
    printf("\033[38;2;0;102;51m=================\033[0m\n");
}

void citire_meniu() {
    int n, k;
    do {
        display_menu();
        if (scanf("%d", &n) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (n >= 1 && n <= 7) {
            printf("Continua actiunea? (1. DA / 2. NU): ");
            if (scanf("%d", &k) != 1 || (k != 1 && k != 2)) {
                while (getchar() != '\n');
                continue;
            }
            if (k == 2) {
                clear_screen();
                continue;
            }
        } else if (n > 8 || n < 1) {
            printf("Optiune Invalida\n");
            do {
                printf("Apasa 1 pentru revenire: ");
                if (scanf("%d", &k) != 1) {
                    while (getchar() != '\n');
                }
            } while (k != 1);
            clear_screen();
            continue;
        }

        clear_screen();
        switch (n) {
            case 1: adaugare_sala(); break;
            case 2: stergere_sala(); break;
            case 3: afis_sala_liber(); break;
            case 4: afis_rezervari(); break;
            case 5: cautare_sala(); break;
            case 6: rezervare_sala(); break;
            case 7: anulare_rezervare(); break;
            case 8: printf("Iesire din program.\n"); return;
        }
    } while (n != 8);
}

int main() {
    citire_fisier("../sali.txt");
    citire_meniu();

    return 0;
}