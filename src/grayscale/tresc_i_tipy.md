Dany jest obrazek w postaci matrycy pikseli. 
Kolor każdego piksela zapisany jest na 24 bitach, po 8 bitów dla każdego koloru (RGB). 
Należy napisać procedurę w asemblerze procesora ARM, dokonującą konwersji obrazu na skalę szarości 
przez policzenie średniej ważonej.

Ponieważ oko ludzkie niejednakowo reaguje na kolory (najsilniej na zielony), 
współczynniki dla poszczególnych składowych nie mogą być jednakowe, zwykle przyjmuje się:

77 dla koloru czerwonego,
151 dla koloru zielonego
28 dla koloru niebieskiego.
Podane współczynniki sumują się do 256, co ułatwi dzielenie.

```
// współczynniki dla kolorów, nadpisywalne
R = 77
G = 151
B = 28

// na wejściu mamy piksel
pixel = uint8_t[3]{red, green, blue}

// przeliczamy rgb na skalę szarości
// >> 8 to bitshift -- łatwiejsze dzielenie w asm ARM
grayscale = (red * R + green * G + blue * B) >> 8;

// i nadpisujemy stare wartości piksela tak że przypisujemy
// red = green = blue = grayscale;
pixel = uint8_t[3]{grayscale, grayscale, grayscale}
```

Argumentami procedury powinny być:
adres matrycy: zapis zapewne wierszami, trzeba będzie wybrać sobie odpowiedni format podczas konwersji z formatu graficznego;
rozmiary matrycy: liczba wierszy i kolumn.

czyli sygnatura w pliku w C będzie oznaczona tak:
```c
// matryca ma rozmiar 3 * liczba_wierszy * liczba_kolumn bo jeden piksel to w naszej libce struct {r int, g, int, b int}
extern void do_skali_szarosci(uint32_t* matryca, uint32_t liczba_wierszy, uint32_t liczba_kolumn);
```

Dodatkowo należy napisać program główny (w C), który pobierze (jako argument) 
nazwę pliku graficznego (w formacie PPM, przypominam o Cioci Wikipedii) 
i zamieni go na odpowiednią matrycę (można funkcją z jakiejś biblioteki, byleby nie zawierała błędów ;-), 
po czym wywoła naszą procedurę. Otrzymaną matrycę należy z powrotem zamienić na plik graficzny (z rozszerzeniem .pgm!). 

czyli dostajemy .ppm, a zapisujemy jako .pgm

WCZYTUJEMY PPM UŻYWAJĄC BIBLIOTEKI, ALE ZAPISUJEMY INACZEJ! W INNYM FORMACIE

Całość powinna działać pod emulatorem QEMU dostępnym na stronie.

Należy umożliwić zmienianie wartości współczynników przez podanie dodatkowych parametrów programu głównego 
(kolejność standardowa R, G, B), ale wtedy muszą się one sumować do 256.

```c
int main(int argc, char *argv[]) {
    // R G B to powinny być prawdopodobnie ZMIENNE GLOBALNE jednobajtowe, zadeklarowane w pliku .s czyli asemblerowym
    // i w nim coś analogicznego do 
    // global R
    // czyli wyeksportowanie symbolu
    // a w c wtedy extern uint8_t R;
    if (argc == 2) {
        nazwa_pliku = argv[1];
        // nie ma dodatkowych argumentów, R G B mają domyślne wartości
    } else if (argc == 5) {
        R = to_int(argv[1]);
        G = to_int(argv[2]);
        B = to_int(argv[3]);
    } else {
        // error, zła liczba parametrów
    }
    
    image_t* image = read_ppm();
    to_grayscale(image->matrix, h, w);
    write_to_pgm(image);
}

void write_to_pgm(image_t* image) {
    // nazwa pliku taka jak oryginalna tylko zmienione extension
    // tj apple.ppm -> apple.pgm
}
```

W katalogu greyscale/images znajduje się kilka prostych plików z danymi.

plik apple.ppm już jest w skali szarości - powinien zostać niezmieniony z wyglądu

reszta powinna zostać zmieniona

program ma działać na labach, czyli ma działać na tej wirtualce 

zbyszek chce kilka przykładowych testów input/output
