## Varianta termínu - 5. Fituška-v2: sdílení a řešení testových otázek studenty
<b>Popis:</b>
Úkolem zadání je vytvořit informační systém pro zlepšení přípravy studentů na zkoušky s prvky gamifikace. Smyslem je pokládat (jak studenty, tak vyučujícími) relevantní otázky k vybraným kurzům (předmětům), kde každá otázka má nějaké označení, pomocí které ji účastníci budou moci vhodně odlišit (název) a další atributy (popis, případné ilustrace, apod.). Zároveň má každá otázka přidělenou kategorii (např. kategorie: půlsemestrálka, zkouška, první cvičení). K položeným otázkám je možné vložit maximálně jednu textovou odpověď každým studentem kurzu (popis, případné ilustrace, apod.), jejichž obsah může být diskutována v diskusním vlákně odpovědi a její kvalita případně oceněna jinými studenty udělením hlasů. Správnost odpovědí je po vložení dostatečného množství odpovědí (posoudí vyučující) vyhodnocena vyučujícím kurzu a studentům správných odpovědí jsou v jejich profilu přičteny body, které získali od ostatních studentů. Uživatelé budou moci dále informační systém použít konkrétně následujícím způsobem:

<b>administrátor</b>

* spravuje uživatele

* má rovněž práva všech následujících rolí

<b>moderátor</b>

* schvaluje kurzy (např. pro ověření, že dotyčný je vyučující kurzu)

* má práva registrovaného uživatele

<b>registrovaný uživatel</b>

* zakládá kurz - stává se vyučujícím kurzu

  * schvaluje registrované studenty kurzu

  * spravuje kategorie otázek

  * pokládá otázky

  * nesmí reagovat na otázky ani psát odpovědi

  * uzavírá otázku

    * označí správné a špatné odpovědi (hlasy studentů u správných odpovědí se přičtou autorovi do profilu; správné odpovědi bez hlasů mohou rovněž dostat nějaké hlasy od vyučujícího - rozhodne vyučující hromadně)

    * napíše finální odpověď

* registruje se na kurz - stává se studentem kurzu

  * pokládá otázky

  * píše odpovědi (každý student může vložit maximálně jednu odpověď ke každé otázce; nesmí vložit odpověď ke své otázce)

  * přidává reakce k odpovědím (libovolné množství)

  * uděluje hlasy odpovědím (každý student může udělit maximálně 3 hlasy u každé otázky)

<b>neregistrovaný</b>

* vidí zadané kurzy, otázky a odpovědi

* vidí uživatele a jejich nasbírané hlasy (žebříčky pro kurzy a celkové)

* má možnost procházet a vyhledávat v otázkách

<b>Náměty na rozšíření:</b>

  * pokročilé žebříčky a statistiky vizualizované graficky

  * speciální typ otázky "časovaná", která bude zobrazena pouze několik minut a bude za větší počet bodů (rozhodněte sami), takové skryté otázky budou odlišeny od ostatních

  * mód, v kterém jsou odpovědi nejprve skryté, jejich odkrytí provede vyučující kurz a teprve poté může proběhnout diskuse, hlasování a vyhodnocení vyučujícím

