#include <mingl>

int main() {
    // Inicializujeme nové okno.
    // Údaje o veľkosti zadať nepotrebujeme. Okno si ich na základe jeho komponentov vypočíta samé.
    // Súradnice sa nastavia na (x: 100, y: 100).
    Window window;
    window.setTitle("Delitele cisla");

    // Ekvivalentá inicializácia je:
    // Window window{"Delitele cisla"};

    // Pridáme popis pre číselný vstup.
    // Údaje o veľkosti opäť zadať nemusíme. Sú vypočítané na základe veľkosti písma a počtu znakov.
    window.addLabel(1, "Zadaj cislo: ", 10, 10);

    // Komponent pre číselný vstup môžeme vytvoriť aj mimo okna.
    // Pridáme mu tak okraj a vstupný text zarovnáme na stred.
    // Nakoniec pridáme adresu komponentu do okna. Takto na poradí vykonaných operácií nezáleží.
    NumberInput input(2, 130, 10, 50, 20);
    input.addBorder();
    input.alignCenter();
    window.addNumberInput(&input);

    // Ďalším spôsobom ako pridať komponent do okna je zadanie adresy okna pri vytvorení komponentu.
    TextInput output(&window, 3, 10, 40, 250, 100);
    // Pridáme okraj.
    output.addBorder();

    // Keďže tento komponent slúži iba na výpis, nastavíme, aby bol len na čítanie.
    output.readOnly();

    // Tomuto komponentu umožníme, aby jeho text mohol byť na viacero riadkov a keďže ich počet môže presahovať
    // výšku komponentu, umožníme aby sme textom mohli posúvať.
    output.enableMultiLine();
    output.enableVerticalScroll();

    // Ďalším spôsobom pridania objektu do okna je vytvoriť objekt na mieste, kde sa posiela do metódy addObject() alebo
    // metódy s názvom objektu tzn. v tomto prípade addButton().
    window.addObject(Button(4, "Vypocitaj", 190, 10, [&input, &output] {

        // Objekty z okna nepotrebujeme získať z metódy find(). Zachytíme potrebné komponenty v lambda výraze a môžeme
        // k nim pristupovať.

        // Získame číslo zo vstupného pola.
        unsigned long number = input.getNumber();

        // Číslo 1 je deliteľom každého čísla. Nový riadok v operačnom systéme Windows obsahuje dva znaky '\r' a '\n'.
        std::string result = "1\r\n";

        // Pokračujeme od čísla 2.
        for (int i = 2; i < number; ++i) {
            // Ak je zvyšok po delení 0, tak je deliteľom.
            if (number % i == 0) {
                result += std::to_string(i) + "\r\n";
            }
        }

        // Samotné číslo je deliteľom.
        result += std::to_string(number);
        output.setText(result.c_str());
    }));

    // Zobrazíme okno.
    return window.show();
}