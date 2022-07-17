//Rabuga Andrei-Flavius 313CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tarb.h"

TD *creeazaDirector(char *nume, TD *copii)
{
    TD *dir = malloc(sizeof(TD));
    if(!dir) return NULL;
    dir->name = malloc(sizeof(char) * 50);
    if(!dir->name) return NULL;
    memcpy(dir->name, nume, 50);
    dir->directories = copii;
    dir->files = NULL;
    dir->dr = NULL;
    dir->st = NULL;
    dir->parent = NULL;
    return dir;
}

TF *creeazaFisier(char *nume)
{
    TF *fis = malloc(sizeof(TF));
    if(!fis) return NULL;
    fis->name = malloc(sizeof(char) * 50);
    if(!fis->name) return NULL;
    memcpy(fis->name, nume, 50);
    fis->dr = NULL;
    fis->st = NULL;
    fis->parent = NULL;
    return fis;
}

void inserareFisier(TF **fis, char *fileName, TD *parinte)
{
    if((*fis) == NULL) 
    {
        (*fis) = creeazaFisier(fileName);
        (*fis)->parent = parinte;
    }
    else
    {
        if (strcmp((*fis)->name, fileName) > 0)
            inserareFisier(&((*fis)->st), fileName, parinte);
        else if (strcmp((*fis)->name, fileName) < 0)
            inserareFisier(&((*fis)->dr), fileName, parinte);
    }
}

void inserareDirector(TD **dir, char *fileName, TD *parinte, TD *copii)
{
    if((*dir) == NULL) 
    {
        (*dir) = creeazaDirector(fileName, copii);
        (*dir)->parent = parinte;
    }
    else
    {
        if (strcmp((*dir)->name, fileName) > 0)
            inserareDirector(&((*dir)->st), fileName, parinte, copii);
        else if (strcmp((*dir)->name, fileName) < 0)
            inserareDirector(&((*dir)->dr), fileName, parinte, copii);
    }
}

int cautaDirector(TD *dir, char *fileName, int cuAfisare)
{
    if(dir == NULL) return 0;
    else
    {
        if (strcmp(dir->name, fileName) == 0)
        {
            if (cuAfisare)
                printf("Directory %s already exists!\n", fileName);
            return 1;
        }
        else if (strcmp(dir->name, fileName) > 0)
            return cautaDirector(dir->st, fileName, cuAfisare);
        else if (strcmp(dir->name, fileName) < 0)
            return cautaDirector(dir->dr, fileName, cuAfisare);
    }
    return 0;
}

int cautaFisier(TF *fis, char *fileName, int cuAfisare)
{
    if(fis == NULL) return 0;
    else
    {
        if (strcmp(fis->name, fileName) == 0)
        {
            if (cuAfisare)
                printf("File %s already exists!\n", fileName);
            return 1;
        }
        else if (strcmp(fis->name, fileName) > 0)
            return cautaFisier(fis->st, fileName, cuAfisare);
        else if (strcmp(fis->name, fileName) < 0)
            return cautaFisier(fis->dr, fileName, cuAfisare);
    }
    return 0;
}

void srdFisier(TF *a)
{
    if(a != NULL)
    {
        if(a->st != NULL) srdFisier(a->st);
        printf("%s ", a->name);
        if(a->dr != NULL) srdFisier(a->dr);
    }
}

void srdDirecor(TD *a)
{
    if(a != NULL)
    {
        if(a->st != NULL) srdDirecor(a->st);
        printf("%s ", a->name);
        if(a->dr != NULL) srdDirecor(a->dr);
    }
}

void rm(TF **fis, char *fileName, TF **new, TD *parinte)
{
    if((*fis) != NULL)
    {
        if((*fis)->st != NULL) rm(&((*fis)->st), fileName, &(*new), parinte);
        if(strcmp((*fis)->name, fileName) != 0) 
            inserareFisier(&(*new), (*fis)->name, parinte);
        if((*fis)->dr != NULL) rm(&((*fis)->dr), fileName, &(*new), parinte);
    }
}

void rmdir(TD **dir, char *fileName, TD **new, TD *parinte)
{
    if((*dir) != NULL)
    {
        if((*dir)->st != NULL) rmdir(&((*dir)->st), fileName, &(*new), parinte);
        if(strcmp((*dir)->name, fileName) != 0) 
            {
                inserareDirector(&(*new), (*dir)->name, parinte, (*dir)->directories);
                (*dir)->directories = NULL;
            }
        
        if((*dir)->dr != NULL) rmdir(&((*dir)->dr), fileName, &(*new), parinte);
    }
}

void cd(TD *dir, char *fileName, TD **curent)
{
    if(dir != NULL) 
    {
        if (strcmp(dir->name, fileName) == 0)
        {
            (*curent) = dir;
        }
        else if (strcmp(dir->name, fileName) > 0)
            cd(dir->st, fileName, curent);
        else if (strcmp(dir->name, fileName) < 0)
            cd(dir->dr, fileName, curent);
    }
}

void pwd(TD *dir)
{
    if(strcmp(dir->name, "root") == 0) printf("/root");
    else
    {
        pwd(dir->parent);
        printf("/%s", dir->name);
    }
}

int findFile(TD *dir, char *fileName)
{
    if(dir != NULL)
    {
        if(cautaFisier(dir->files, fileName, 0))
        {
            printf("File %s found!\n", fileName);
            pwd(dir);
            printf("\n");
            return 1;
        }
        else
        {
            return findFile(dir->st, fileName) +
            findFile(dir->dr, fileName) +
            findFile(dir->directories, fileName);
        }
    }
    return 0;
}

int findDirector(TD *dir, char *fileName)
{
    if(dir != NULL)
    {
        if(cautaDirector(dir->directories, fileName, 0))
        {
            printf("Directory %s found!\n", fileName);
            pwd(dir);
            printf("/%s\n", fileName);
            return 1;
        }
        else
        {
            return findDirector(dir->st, fileName) + 
            findDirector(dir->dr, fileName) +
            findDirector(dir->directories, fileName);
        }
    }
    return 0;
}

// void distruge(TArb *r) 	/* functie auxiliara - distruge toate nodurile */
// {
// 	if (!(*r)) return;
// 	distruge (&((*r)->st));     /* distruge subarborele stang */
// 	distruge (&((*r)->dr));     /* distruge subarborele drept */
//     free((*r)->info);
// 	free ((*r));             /* distruge nodul radacina */
// }

void distrugeDirector(TD *dir)
{
    free(dir->name);
    free(dir);
}

void distrugeFisier(TF *fis)
{
    free(fis->name);
    free(fis);
}

void distrugeToateFisierele(TF *fis)
{
    if(fis != NULL)
    {
        distrugeToateFisierele(fis->st);
        distrugeToateFisierele(fis->dr);
        free(fis->name);
        free(fis);
    }
}

void distrugeToateDirectoarele(TD *dir)
{
    if(dir != NULL)
    {
        distrugeToateDirectoarele(dir->directories);
        distrugeToateDirectoarele(dir->st);
        distrugeToateDirectoarele(dir->dr);
        distrugeToateFisierele(dir->files);
        free(dir->name);
        free(dir);
    }
}

int main()
{
    char *nume = malloc(55);
    strcpy(nume, "root");
    TD *root = creeazaDirector(nume, NULL);
    TD *curent = root; //pointer
    free(nume);

    size_t l = 0;
    char *line = NULL, *cuvant;
    while(getline(&line, &l, stdin) != -1)
    {
        cuvant = strtok(line, " ");
        if(strcmp(cuvant, "touch") == 0)
        {
            cuvant = strtok(NULL, " ");
            if(cuvant[strlen(cuvant) - 1] == '\n') cuvant[strlen(cuvant) - 1] = '\0';
            if(!cautaDirector(curent->directories, cuvant, 1) &&
                !cautaFisier(curent->files, cuvant, 1))
                inserareFisier(&(curent->files), cuvant, curent);
        }
        else if(strcmp(cuvant, "mkdir") == 0)
        {
            cuvant = strtok(NULL, " ");
            if(cuvant[strlen(cuvant) - 1] == '\n') cuvant[strlen(cuvant) - 1] = '\0';
            if(!cautaDirector(curent->directories, cuvant, 1) &&
                !cautaFisier(curent->files, cuvant, 1))
                    inserareDirector(&(curent->directories), cuvant, curent, NULL);
        }
        else if(strcmp(cuvant, "ls\n") == 0)
        {
            srdDirecor(curent->directories);
            srdFisier(curent->files);
            printf("\n");
        }
        else if(strcmp(cuvant, "rm") == 0)
        {
            cuvant = strtok(NULL, " ");
            if(cuvant[strlen(cuvant) - 1] == '\n') cuvant[strlen(cuvant) - 1] = '\0';
            if(cautaFisier(curent->files, cuvant, 0))
            {
                TF *new = NULL;
                rm(&(curent->files), cuvant, &new, curent);
                distrugeToateFisierele(curent->files);
                curent->files = new;
            }
            else printf("File %s doesn't exist!\n", cuvant);
        }
        else if(strcmp(cuvant, "rmdir") == 0)
        {
            cuvant = strtok(NULL, " ");
            if(cuvant[strlen(cuvant) - 1] == '\n') cuvant[strlen(cuvant) - 1] = '\0';
            if(cautaDirector(curent->directories, cuvant, 0))
            {
                TD *new = NULL;
                rmdir(&(curent->directories), cuvant, &new, curent);
                distrugeToateDirectoarele(curent->directories);
                curent->directories = new;
            }
            else printf("Directory %s doesn't exist!\n", cuvant);
        }
        else if(strcmp(cuvant, "cd") == 0)
        {
            cuvant = strtok(NULL, " ");
            if(cuvant[strlen(cuvant) - 1] == '\n') cuvant[strlen(cuvant) - 1] = '\0';
            if(strcmp(cuvant, "..") == 0) 
            {
                if(strcmp(curent->name, "root") != 0) curent = curent->parent;
            }
            else if(cautaDirector(curent->directories, cuvant, 0))
            {
                cd(curent->directories, cuvant, &(curent));
            }
            else printf("Directory not found!\n");
        }
        else if(strcmp(cuvant, "pwd\n") == 0)
        {
            pwd(curent);
            printf("\n");
        }
        else if(strcmp(cuvant, "find") == 0)
        {
            cuvant = strtok(NULL, " ");
            if(cuvant[strlen(cuvant) - 1] == '\n') cuvant[strlen(cuvant) - 1] = '\0';
            if(strcmp(cuvant, "-f") == 0) 
            {
                cuvant = strtok(NULL, " ");
                if(cuvant[strlen(cuvant) - 1] == '\n') cuvant[strlen(cuvant) - 1] = '\0';
                if(!findFile(root, cuvant))
                    printf("File %s not found!\n", cuvant);
            }
            else if(strcmp(cuvant, "-d") == 0) 
            {
                cuvant = strtok(NULL, " ");
                if(cuvant[strlen(cuvant) - 1] == '\n') cuvant[strlen(cuvant) - 1] = '\0';
                if(!findDirector(root, cuvant))
                   printf("Directory %s not found!\n", cuvant);
            }
        }
        else if(strcmp(cuvant, "quit\n") == 0)
        {
            break;
        }
    }
    free(line);
    distrugeToateDirectoarele(root);
    return 0;
}