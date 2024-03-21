#include "brodovi.h"

Brodovi::Brodovi()
{
    this->brojBrodova = 0;
    this->pozicije = nullptr;
}

Brodovi::Brodovi(More *more, const char *imeFajla)
    : Brodovi()
{
    
    this->Ucitaj(more, imeFajla);
    for (size_t i = 0; i < this->brojBrodova; i++)
        std::cout << this->pozicije[i].x << ','
                  << this->pozicije[i].y << '\n';
}

Brodovi::~Brodovi()
{
    delete[] this->pozicije;
}

bool Brodovi::ProveraValidnosti(More *more, OBJEKAT **kopijaMora, int x, int y)
{
    //LOKACIJA
    if (kopijaMora[y][x] == OBJEKAT::BROD ||
        kopijaMora[y][x] == OBJEKAT::OSTRVO)
        return false;
    
    //IVICE
    if (x <= 0 || x >= more->VratiX())
        return false;
    if (y <= 0 || y >= more->VratiY())
        return false;
    
    //SUSEDNA POLJA
    if (kopijaMora[y + 1][x] == OBJEKAT::OSTRVO ||
        kopijaMora[y + 1][x] == OBJEKAT::BROD)
        return false;
    if (kopijaMora[y - 1][x] == OBJEKAT::OSTRVO ||
        kopijaMora[y - 1][x] == OBJEKAT::BROD)
        return false;
    if (kopijaMora[y][x + 1] == OBJEKAT::OSTRVO ||
        kopijaMora[y][x + 1] == OBJEKAT::BROD)
        return false;
    if (kopijaMora[y][x - 1] == OBJEKAT::OSTRVO ||
        kopijaMora[y][x - 1] == OBJEKAT::BROD)
        return false;
    
    return true;
}

void Brodovi::Ucitaj(More *more, const char *imeFajla)
{
    std::ifstream fajl(imeFajla);
    if (!fajl.good())
        return;
    
    //INICIJALIZACIJA
    OBJEKAT **kopijaMora = new OBJEKAT*[more->VratiY()];
    for (size_t i = 0; i < more->VratiY(); i++)
    {
        kopijaMora[i] = new OBJEKAT[more->VratiY()];
        for (size_t j = 0; j < more->VratiY(); j++)
            kopijaMora[i][j] = more->VratiObjekat(j, i);
    }
    int brojBrodova;
    fajl >> brojBrodova;
    
    int pozX, pozY;
    int duzina;
    bool uspravno;
    for (size_t i = 0; i < brojBrodova; i++)
    {
        //ZA SVAKI BROD...
        fajl >> pozX >> pozY >> duzina >> uspravno;
        // std::cout << ' '<< pozX
        //           << ' '<< pozY
        //           << ' '<< duzina
        //           << ' '<< uspravno;
        // std::cout << '\n' << this->brojBrodova << std::endl;

        //PROVERA VALIDNOSTI
        bool validno;
        for (size_t j = 0; j < duzina; j++)
        {
            validno = this->ProveraValidnosti(more, kopijaMora,
                                              pozX + j*!uspravno,
                                              pozY + j*uspravno);
            if (duzina < 2)
                validno = false;
            if (!validno)
                break;
        }
        if (!validno)
            continue;
        
        //UPISIVANJE
        for (size_t j = 0; j < duzina; j++)
        {
            kopijaMora[pozY + j*uspravno][pozX + j*!uspravno] = OBJEKAT::BROD;
            this->Dodaj(Kordinate(pozX + j*!uspravno, pozY + j*uspravno));
        }
    }

    fajl.close();
}

bool Brodovi::DaLiPostoji(More *more, int x, int y)
{
    //PROVERA VALIDNOSTI
    if (x < 0 || x >= more->VratiX())
        return false; //TODO! promeni u throw
    if (y < 0 || y >= more->VratiY())
        return false;
    
    bool postoji = false;
    for (size_t i = 0; i < this->brojBrodova; i++)
    {
        if (this->pozicije[i] == Kordinate(x, y))
            postoji = true;
        if (postoji)
            this->pozicije[i] = this->pozicije[i + 1];
    }
    if (postoji)
        this->brojBrodova--;
    
    return postoji;
}

void Brodovi::Dodaj(Kordinate obj)
{
    Kordinate *novePozicije = new Kordinate[this->brojBrodova + 1];
    for (size_t i = 0; i < this->brojBrodova; i++)
        novePozicije[i] = this->pozicije[i];
    novePozicije[this->brojBrodova] = obj;
    
    delete this->pozicije;
    this->pozicije = novePozicije;
    
    this->brojBrodova++;
}