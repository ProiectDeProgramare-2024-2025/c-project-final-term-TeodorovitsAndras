#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

typedef struct {
    char nume[50];
    int capacitate;
    int liber; // 1 = disponibila, 0 = indisponibila
    char data[20];     // "2025-05-01" sau "-"
    char zi[20];       // "Joi" sau "-"
    char interval[20]; // "12:00-14:00" sau "-"
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

void timer_revenire() {
    printf("\n");
    for (int i = 4; i >= 1; i--) {
        printf("\r\033[38;2;255;165;0mRevenire la meniul principal in %d secunde...\033[0m", i);
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
}

void citire_fisier(const char *file) {
    clear_screen();
    FILE *fisier = fopen(file, "r");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului %s: %s\n", file, strerror(errno));
        return;
    }

    while (nrSali < 101 && fscanf(fisier, "%49[^,],%d,%d,%19[^,],%19[^,],%19[^\n]",
            sali[nrSali].nume,
            &sali[nrSali].capacitate,
            &sali[nrSali].liber,
            sali[nrSali].data,
            sali[nrSali].zi,
            sali[nrSali].interval) == 6) {
        nrSali++;
        int ch;
        while ((ch = fgetc(fisier)) != '\n' && ch != EOF);
            }

    if (nrSali == 101) {
        printf("\033[38;2;0;153;0mAtentie: Numarul maxim de sali a fost atins.\033[0m\n");
    }

    fclose(fisier);
}


void display_menu() {
    clear_screen();
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
        while (getchar() != '\n');
        fgets(sali[nrSali].nume, sizeof(sali[nrSali].nume), stdin);
        if (strlen(sali[nrSali].nume) > 0 && sali[nrSali].nume[strlen(sali[nrSali].nume) - 1] == '\n')
            sali[nrSali].nume[strlen(sali[nrSali].nume) - 1] = '\0';

        printf("\033[38;2;102;255;102mIntroduceti capacitatea salii:\033[0m\n");
        while (scanf("%d", &sali[nrSali].capacitate) != 1 || sali[nrSali].capacitate <= 0) {
            printf("\033[38;2;255;0;0mInput invalid. Introduceti o capacitate pozitiva.\033[0m\n");
            while (getchar() != '\n');
        }

        sali[nrSali].liber = 1; // disponibila
        strcpy(sali[nrSali].data, "-");
        strcpy(sali[nrSali].zi, "-");
        strcpy(sali[nrSali].interval, "-");

        printf("\033[38;2;0;204;102mSala \033[38;2;153;255;153m%s\033[0m \033[38;2;0;204;102ma fost adaugata cu succes.\033[0m\n", sali[nrSali].nume);

        FILE *fisier = fopen("../sali.txt", "a");
        if (fisier == NULL) {
            printf("\033[38;2;255;0;0mEroare la deschiderea fisierului.\033[0m\n");
            return;
        }
        fprintf(fisier, "%s,%d,%d,%s,%s,%s\n",
            sali[nrSali].nume,
            sali[nrSali].capacitate,
            sali[nrSali].liber,
            sali[nrSali].data,
            sali[nrSali].zi,
            sali[nrSali].interval);
        fclose(fisier);

        nrSali++;
    }
}


void stergere_sala() {
    clear_screen();
    if (nrSali == 0) {
        printf("\033[38;2;255;165;0mNu exista sali salvate momentan. Revenire la meniul principal...\033[0m\n");
        timer_revenire();
        return;
    }

    int continua = 1;
    while (continua) {
        printf("\033[38;2;0;102;51m=============\033[0m\n");
        printf("\033[38;2;0;102;51mStergere sala\033[0m\n");
        printf("\033[38;2;0;102;51m=============\033[0m\n");
        printf("%-5s %-30s %-15s\n", "Nr.", "Nume", "Capacitate");
        for (int i = 0; i < nrSali; i++) {
            printf("\033[38;2;144;238;144m%-5d\033[0m %-30s %-15d\n", i + 1, sali[i].nume, sali[i].capacitate);
        }

        printf("\nIntrodu numarul salii pe care vrei sa o stergi (0 pentru anulare): ");
        int alegere;
        if (scanf("%d", &alegere) != 1) {
            printf("\033[38;2;255;0;0mValoare invalida. Introdu un numar.\033[0m\n");
            while (getchar() != '\n');
            continue;
        }
        if (alegere == 0) {
            printf("\033[38;2;255;165;0mStergere anulata.\033[0m\n");
            timer_revenire();
            return;
        }
        if (alegere < 1 || alegere > nrSali) {
            clear_screen();
            printf("\033[38;2;255;0;0mNumar invalid. Incearca din nou.\033[0m\n");
            continue;
        }

        alegere--;
        printf("\033[38;2;255;0;0mSterg sala '%s'.\033[0m\n", sali[alegere].nume);

        for (int i = alegere; i < nrSali - 1; i++) {
            sali[i] = sali[i + 1];
        }
        nrSali--;

        FILE *fisier = fopen("../sali.txt", "w");
        if (fisier == NULL) {
            printf("\033[38;2;255;0;0mEroare la deschiderea fisierului pentru rescriere.\033[0m\n");
            return;
        }
        for (int i = 0; i < nrSali; i++) {
            fprintf(fisier, "%s,%d,%d,%s,%s,%s\n",
                sali[i].nume,
                sali[i].capacitate,
                sali[i].liber,
                sali[i].data,
                sali[i].zi,
                sali[i].interval);
        }
        fclose(fisier);

        printf("\033[38;2;0;204;102mSala a fost stearsa cu succes.\033[0m\n");

        int raspunsValid = 0;
        while (!raspunsValid) {
            printf("\nVrei sa stergi alta sala?\n");
            printf("1. Da\n");
            printf("2. Nu (Revenire la meniul principal)\n");
            printf("Introdu alegerea ta: ");
            int alegereContinua;
            if (scanf("%d", &alegereContinua) != 1) {
                clear_screen();
                printf("\033[38;2;255;0;0mValoare invalida. Introdu 1 sau 2.\033[0m\n");
                while (getchar() != '\n');
                continue;
            }
            if (alegereContinua == 1) {
                raspunsValid = 1;
                clear_screen();
            } else if (alegereContinua == 2) {
                raspunsValid = 1;
                continua = 0;
            } else {
                clear_screen();
                printf("\033[38;2;255;0;0mOptiune invalida. Introdu 1 sau 2.\033[0m\n");
            }
        }
    }
}



void afis_sala_liber() {
    clear_screen();
    if (nrSali == 0) {
        printf("\033[38;2;255;165;0mNu exista sali salvate momentan.\033[0m\n");
        timer_revenire();
        return;
    }

    printf("\033[38;2;0;102;51m========================\033[0m\n");
    printf("\033[38;2;0;102;51mAfisare Sali Disponibile\033[0m\n");
    printf("\033[38;2;0;102;51m========================\033[0m\n");

    int maxLen = 0;
    for (int i = 0; i < nrSali; i++) {
        int len = strlen(sali[i].nume);
        if (len > maxLen) maxLen = len;
    }

    printf("%-5s %-*s | %-10s\n", "Nr.", maxLen, "Nume", "Capacitate");
    for (int i = 0; i < maxLen + 18; i++) printf("-");
    printf("\n");

    int gasit = 0;
    for (int i = 0; i < nrSali; i++) {
        if (sali[i].liber == 1) {
            printf("\033[38;2;144;238;144m%-5d\033[0m %-*s | %-10d\n",
                   i + 1, maxLen, sali[i].nume, sali[i].capacitate);
            gasit = 1;
        }
    }

    if (!gasit) {
        printf("\033[38;2;255;0;0mNu exista sali disponibile pentru rezervare momentan!\033[0m\n");
        timer_revenire();
    } else {
        int alegere;
        printf("\n\033[38;2;255;165;0mApasa 1 pentru revenire la meniul principal: \033[0m");
        do {
            if (scanf("%d", &alegere) != 1) {
                while (getchar() != '\n');
                alegere = 0;
            }
        } while (alegere != 1);
    }
}




void afis_rezervari() {
    clear_screen();
    printf("\033[38;2;0;102;51m=================\033[0m\n");
    printf("\033[38;2;0;102;51mAfisare Rezervari\033[0m\n");
    printf("\033[38;2;0;102;51m=================\033[0m\n");

    if (nrSali == 0) {
        printf("\033[38;2;255;0;0mNu exista sali salvate momentan!\033[0m\n");
        timer_revenire();
        return;
    }

    int gasit = 0;
    int maxLen = 0;
    for (int i = 0; i < nrSali; i++) {
        int len = strlen(sali[i].nume);
        if (len > maxLen) maxLen = len;
    }

    printf("%-5s %-*s | %-10s | %-12s | %-10s | %-15s\n", "Nr.", maxLen, "Nume", "Capacitate", "Data", "Ziua", "Interval");
    for (int i = 0; i < maxLen + 68; i++) printf("-");
    printf("\n");

    for (int i = 0; i < nrSali; i++) {
        if (sali[i].liber == 0) {
            printf("\033[38;2;144;238;144m%-5d\033[0m %-*s | %-10d | %-12s | %-10s | %-15s\n",
                i + 1, maxLen, sali[i].nume, sali[i].capacitate,
                sali[i].data, sali[i].zi, sali[i].interval);
            gasit = 1;
        }
    }

    if (!gasit) {
        printf("\033[38;2;255;0;0mNu exista rezervari inregistrate momentan!\033[0m\n");
    }

    printf("\n\033[38;2;255;165;0mApasa 1 pentru revenire la meniul principal: \033[0m");
    int alegere;
    do {
        if (scanf("%d", &alegere) != 1) {
            while (getchar() != '\n');
            alegere = 0;
        }
    } while (alegere != 1);
}


void cautare_sala() {
    if (nrSali == 0) {
        printf("\033[38;2;255;0;0mNu exista sali salvate momentan!\033[0m\n");
        timer_revenire();
        return;
    }

    int continua = 1;
    while (continua) {
        clear_screen();
        printf("\033[38;2;0;102;51m============\033[0m\n");
        printf("Cautare sala\n");
        printf("\033[38;2;0;102;51m============\033[0m\n");

        int criteriu = 0;
        while (1) {
            printf("\nAlege criteriul de cautare:\n");
            printf("1. Dupa nume\n");
            printf("2. Dupa capacitate (>=)\n");
            printf("3. Dupa disponibilitate\n");
            printf("Introdu optiunea ta: ");
            if (scanf("%d", &criteriu) != 1) {
                printf("\033[38;2;255;0;0mInput invalid. Introdu un numar.\033[0m\n");
                while (getchar() != '\n');
                continue;
            }
            if (criteriu < 1 || criteriu > 3) {
                printf("\033[38;2;255;0;0mOptiune invalida. Alege intre 1 si 3.\033[0m\n");
                continue;
            }
            break;
        }

        int maxLen = 0;
        for (int i = 0; i < nrSali; i++) {
            int len = strlen(sali[i].nume);
            if (len > maxLen) maxLen = len;
        }

        if (criteriu == 1) { // cautare dupa nume
            printf("\n\033[38;2;0;153;153mIntrodu numele salii (sau o parte din nume):\033[0m ");
            while (getchar() != '\n');
            char nume_cautat[50];
            fgets(nume_cautat, sizeof(nume_cautat), stdin);
            if (strlen(nume_cautat) > 0 && nume_cautat[strlen(nume_cautat) - 1] == '\n')
                nume_cautat[strlen(nume_cautat) - 1] = '\0';

            char nume_cautat_lower[50];
            for (int i = 0; nume_cautat[i]; i++) {
                nume_cautat_lower[i] = tolower(nume_cautat[i]);
            }
            nume_cautat_lower[strlen(nume_cautat)] = '\0';

            int gasit = 0;
            printf("\nRezultatele cautarii:\n");
            printf("%-5s %-*s | %-10s | %-15s | %-12s | %-10s | %-15s\n", "Nr.", maxLen, "Nume", "Capacitate", "Disponibilitate", "Data", "Ziua", "Interval");
            for (int i = 0; i < maxLen + 85; i++) printf("-");
            printf("\n");

            for (int i = 0; i < nrSali; i++) {
                char nume_sala_lower[50];
                for (int j = 0; sali[i].nume[j]; j++) {
                    nume_sala_lower[j] = tolower(sali[i].nume[j]);
                }
                nume_sala_lower[strlen(sali[i].nume)] = '\0';

                if (strstr(nume_sala_lower, nume_cautat_lower) != NULL) {
                    printf("\033[38;2;144;238;144m%-5d\033[0m %-*s | %-10d | %s",
                        i + 1, maxLen, sali[i].nume, sali[i].capacitate,
                        sali[i].liber ? "\033[38;2;0;255;0mDisponibila  \033[0m" : "\033[38;2;255;0;0mIndisponibila\033[0m");

                    if (sali[i].liber == 0) {
                        printf(" | %-12s | %-10s | %-15s", sali[i].data, sali[i].zi, sali[i].interval);
                    } else {
                        printf(" | %-12s | %-10s | %-15s", "-", "-", "-");
                    }
                    printf("\n");

                    gasit = 1;
                }
            }
            if (!gasit) {
                printf("\033[38;2;255;0;0mNicio sala nu corespunde cautarii!\033[0m\n");
            }

        } else if (criteriu == 2) { // cautare dupa capacitate
            int capacitate_min;
            while (1) {
                printf("\n\033[38;2;0;153;153mIntrodu capacitatea minima:\033[0m ");
                if (scanf("%d", &capacitate_min) != 1 || capacitate_min < 0) {
                    printf("\033[38;2;255;0;0mInput invalid. Introdu un numar pozitiv.\033[0m\n");
                    while (getchar() != '\n');
                } else {
                    break;
                }
            }

            int gasit = 0;
            printf("\nRezultatele cautarii:\n");
            printf("%-5s %-*s | %-10s | %-15s | %-12s | %-10s | %-15s\n", "Nr.", maxLen, "Nume", "Capacitate", "Disponibilitate", "Data", "Ziua", "Interval");
            for (int i = 0; i < maxLen + 85; i++) printf("-");
            printf("\n");

            for (int i = 0; i < nrSali; i++) {
                if (sali[i].capacitate >= capacitate_min) {
                    printf("\033[38;2;144;238;144m%-5d\033[0m %-*s | %-10d | %s",
                        i + 1, maxLen, sali[i].nume, sali[i].capacitate,
                        sali[i].liber ? "\033[38;2;0;255;0mDisponibila  \033[0m" : "\033[38;2;255;0;0mIndisponibila\033[0m");

                    if (sali[i].liber == 0) {
                        printf(" | %-12s | %-10s | %-15s", sali[i].data, sali[i].zi, sali[i].interval);
                    } else {
                        printf(" | %-12s | %-10s | %-15s", "-", "-", "-");
                    }
                    printf("\n");

                    gasit = 1;
                }
            }
            if (!gasit) {
                printf("\033[38;2;255;0;0mNicio sala nu indeplineste criteriul de capacitate!\033[0m\n");
            }

        } else if (criteriu == 3) { // cautare dupa disponibilitate
            int disponibil;
            while (1) {
                printf("\n\033[38;2;0;153;153mIntrodu disponibilitatea cautata (1 = Disponibila, 0 = Indisponibila):\033[0m ");
                if (scanf("%d", &disponibil) != 1 || (disponibil != 0 && disponibil != 1)) {
                    printf("\033[38;2;255;0;0mInput invalid. Introdu 1 sau 0.\033[0m\n");
                    while (getchar() != '\n');
                } else {
                    break;
                }
            }

            int gasit = 0;
            printf("\nRezultatele cautarii:\n");
            printf("%-5s %-*s | %-10s | %-15s | %-12s | %-10s | %-15s\n", "Nr.", maxLen, "Nume", "Capacitate", "Disponibilitate", "Data", "Ziua", "Interval");
            for (int i = 0; i < maxLen + 85; i++) printf("-");
            printf("\n");

            for (int i = 0; i < nrSali; i++) {
                if (sali[i].liber == disponibil) {
                    printf("\033[38;2;144;238;144m%-5d\033[0m %-*s | %-10d | %s",
                        i + 1, maxLen, sali[i].nume, sali[i].capacitate,
                        sali[i].liber ? "\033[38;2;0;255;0mDisponibila    \033[0m" : "\033[38;2;255;0;0mIndisponibila  \033[0m");

                    if (sali[i].liber == 0) {
                        printf(" | %-12s | %-10s | %-15s", sali[i].data, sali[i].zi, sali[i].interval);
                    } else {
                        printf(" | %-12s | %-10s | %-15s", "-", "-", "-");
                    }
                    printf("\n");

                    gasit = 1;
                }
            }
            if (!gasit) {
                printf("\033[38;2;255;0;0mNicio sala nu corespunde disponibilitatii cautate!\033[0m\n");
            }
        }

        int optiune_continua = 0;
        while (1) {
            printf("\n\033[38;2;0;153;153mMai doresti sa faci o cautare? (1 = Da / 2 = Nu):\033[0m ");
            if (scanf("%d", &optiune_continua) != 1) {
                printf("\033[38;2;255;0;0mInput invalid. Introdu 1 sau 2.\033[0m\n");
                while (getchar() != '\n');
                continue;
            }
            if (optiune_continua == 1) {
                break;
            } else if (optiune_continua == 2) {
                continua = 0;
                break;
            } else {
                printf("\033[38;2;255;0;0mOptiune invalida. Alege 1 sau 2.\033[0m\n");
            }
        }
    }

    timer_revenire();
}



void rezervare_sala() {
    clear_screen();
    printf("\033[38;2;0;102;51m==============\033[0m\n");
    printf("\033[38;2;0;102;51mRezervare Sala\033[0m\n");
    printf("\033[38;2;0;102;51m==============\033[0m\n");

    if (nrSali == 0) {
        printf("\033[38;2;255;0;0mNu exista sali salvate momentan!\033[0m\n");
        timer_revenire();
        return;
    }

    int disponibile = 0;
    printf("%-5s %-30s %-15s\n", "Nr.", "Nume", "Capacitate");
    for (int i = 0; i < nrSali; i++) {
        if (sali[i].liber == 1) {
            printf("\033[38;2;144;238;144m%-5d\033[0m %-30s %-15d\n", i + 1, sali[i].nume, sali[i].capacitate);
            disponibile = 1;
        }
    }

    if (!disponibile) {
        printf("\033[38;2;255;0;0mNu exista sali disponibile pentru rezervare!\033[0m\n");
        timer_revenire();
        return;
    }

    printf("\nIntrodu numarul salii pe care vrei sa o rezervi: ");
    int alegere;
    while (scanf("%d", &alegere) != 1 || alegere < 1 || alegere > nrSali || sali[alegere - 1].liber == 0) {
        printf("\033[38;2;255;0;0mInput invalid sau sala nu este disponibila. Incearca din nou.\033[0m\n");
        while (getchar() != '\n');
        printf("\nIntrodu numarul salii pe care vrei sa o rezervi: ");
    }
    int idx = alegere - 1;
    while (getchar() != '\n');

    int an, luna, zi;
    char dataRezervare[20];
    while (1) {
        printf("\n\033[38;2;0;153;153mIntrodu data rezervarii (format AAAA-LL-ZZ):\033[0m ");
        fgets(dataRezervare, sizeof(dataRezervare), stdin);
        if (strlen(dataRezervare) > 0 && dataRezervare[strlen(dataRezervare) - 1] == '\n')
            dataRezervare[strlen(dataRezervare) - 1] = '\0';

        if (sscanf(dataRezervare, "%d-%d-%d", &an, &luna, &zi) == 3 &&
            an >= 2024 && luna >= 1 && luna <= 12 && zi >= 1 && zi <= 31) {
            break;
        } else {
            printf("\033[38;2;255;0;0mFormat invalid. Introdu data corect!\033[0m\n");
        }
    }
    strcpy(sali[idx].data, dataRezervare);

    printf("\nAlege ziua saptamanii:\n");
    printf("1. Luni\n2. Marti\n3. Miercuri\n4. Joi\n5. Vineri\n6. Sambata\n7. Duminica\n");
    int ziSaptamana;
    const char *zile[] = {"Luni", "Marti", "Miercuri", "Joi", "Vineri", "Sambata", "Duminica"};

    while (1) {
        printf("\033[38;2;0;153;153mIntrodu numarul zilei (1-7):\033[0m ");
        if (scanf("%d", &ziSaptamana) != 1 || ziSaptamana < 1 || ziSaptamana > 7) {
            printf("\033[38;2;255;0;0mInput invalid. Alege un numar intre 1 si 7.\033[0m\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
    strcpy(sali[idx].zi, zile[ziSaptamana - 1]);
    while (getchar() != '\n');

    char interval[20];
    while (1) {
        printf("\n\033[38;2;0;153;153mIntrodu intervalul orar (format HH:MM-HH:MM):\033[0m ");
        fgets(interval, sizeof(interval), stdin);
        if (strlen(interval) > 0 && interval[strlen(interval) - 1] == '\n')
            interval[strlen(interval) - 1] = '\0';

        int h1, m1, h2, m2;
        if (sscanf(interval, "%d:%d-%d:%d", &h1, &m1, &h2, &m2) == 4 &&
            h1 >= 0 && h1 <= 23 && h2 >= 0 && h2 <= 23 &&
            m1 >= 0 && m1 <= 59 && m2 >= 0 && m2 <= 59 &&
            (h1 < h2 || (h1 == h2 && m1 < m2))) {
            break;
        } else {
            printf("\033[38;2;255;0;0mFormat invalid sau interval invalid. Introdu corect (HH:MM-HH:MM)!\033[0m\n");
        }
    }
    strcpy(sali[idx].interval, interval);

    sali[idx].liber = 0;

    FILE *fisier = fopen("../sali.txt", "w");
    if (fisier == NULL) {
        printf("\033[38;2;255;0;0mEroare la deschiderea fisierului pentru actualizare!\033[0m\n");
        return;
    }
    for (int i = 0; i < nrSali; i++) {
        fprintf(fisier, "%s,%d,%d,%s,%s,%s\n",
                sali[i].nume,
                sali[i].capacitate,
                sali[i].liber,
                sali[i].data,
                sali[i].zi,
                sali[i].interval);
    }
    fclose(fisier);

    printf("\n\033[38;2;0;204;102mSala '%s' a fost rezervata cu succes!\033[0m\n", sali[idx].nume);
    timer_revenire();
}


void anulare_rezervare() {
    clear_screen();
    printf("\033[38;2;0;102;51m=================\033[0m\n");
    printf("\033[38;2;0;102;51mAnulare Rezervare\033[0m\n");
    printf("\033[38;2;0;102;51m=================\033[0m\n");

    if (nrSali == 0) {
        printf("\033[38;2;255;0;0mNu exista sali salvate momentan!\033[0m\n");
        timer_revenire();
        return;
    }

    int continua = 1;
    while (continua) {
        int rezervari_existente = 0;
        int maxLen = 0;
        for (int i = 0; i < nrSali; i++) {
            int len = strlen(sali[i].nume);
            if (len > maxLen) maxLen = len;
        }
        printf("\n");

        printf("%-5s %-*s | %-10s | %-12s | %-10s | %-15s\n", "Nr.", maxLen, "Nume", "Capacitate", "Data", "Ziua", "Interval");
        for (int i = 0; i < maxLen + 68; i++) printf("-");
        printf("\n");

        for (int i = 0; i < nrSali; i++) {
            if (sali[i].liber == 0) {
                printf("\033[38;2;144;238;144m%-5d\033[0m %-*s | %-10d | %-12s | %-10s | %-15s\n",
                    i + 1, maxLen, sali[i].nume, sali[i].capacitate,
                    sali[i].data, sali[i].zi, sali[i].interval);
                rezervari_existente = 1;
            }
        }

        if (!rezervari_existente) {
            printf("\033[38;2;255;0;0mNu exista rezervari de anulat!\033[0m\n");
            timer_revenire();
            return;
        }

        printf("\nIntrodu numarul salii pentru care vrei sa anulezi rezervarea: ");
        int alegere;
        while (scanf("%d", &alegere) != 1 || alegere < 1 || alegere > nrSali || sali[alegere - 1].liber == 1) {
            printf("\033[38;2;255;0;0mInput invalid sau sala nu este rezervata. Incearca din nou.\033[0m\n");
            while (getchar() != '\n');
            printf("\nIntrodu numarul salii pentru care vrei sa anulezi rezervarea: ");
        }
        int idx = alegere - 1;
        while (getchar() != '\n');

        sali[idx].liber = 1; // Devine disponibila
        strcpy(sali[idx].data, "-");
        strcpy(sali[idx].zi, "-");
        strcpy(sali[idx].interval, "-");

        FILE *fisier = fopen("../sali.txt", "w");
        if (fisier == NULL) {
            printf("\033[38;2;255;0;0mEroare la deschiderea fisierului pentru actualizare!\033[0m\n");
            return;
        }
        for (int i = 0; i < nrSali; i++) {
            fprintf(fisier, "%s,%d,%d,%s,%s,%s\n",
                    sali[i].nume,
                    sali[i].capacitate,
                    sali[i].liber,
                    sali[i].data,
                    sali[i].zi,
                    sali[i].interval);
        }
        fclose(fisier);

        printf("\n\033[38;2;0;204;102mRezervarea pentru sala '%s' a fost anulata cu succes!\033[0m\n", sali[idx].nume);

        int raspuns;
        printf("\n\033[38;2;0;153;153mVrei sa anulezi alta rezervare?\033[0m\n");
        printf("1. Da\n2. Nu (Revenire la meniul principal)\n");
        printf("Introdu alegerea ta: ");
        while (scanf("%d", &raspuns) != 1 || (raspuns != 1 && raspuns != 2)) {
            printf("\033[38;2;255;0;0mInput invalid. Introdu 1 pentru Da sau 2 pentru Nu.\033[0m\n");
            while (getchar() != '\n');
            printf("Introdu alegerea ta: ");
        }
        if (raspuns == 2) {
            continua = 0;
        }
    }

    timer_revenire();
}



void citire_meniu() {
    clear_screen();
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