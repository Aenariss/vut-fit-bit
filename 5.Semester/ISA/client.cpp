// ISA 2021 Projekt - Reverse-engineering neznámeho protokolu 
// Autor: Vojtech Fiala

#include <iostream>
#include <tuple>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <regex>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "base64.h"

#define print(x) (std::cout << x << '\n');  // debug makro

/* Funkce na vypsani napovedy podle referencniho klienta */
void print_help() {
    std::cout << "usage: client [ <option> ... ] <command> [<args>] ...\n\n";
    std::cout << "<option> is one of\n\n";
    std::cout << "   -a <addr>, --address <addr>\n     Server hostname or address to connect to\n";
    std::cout << "   -p <port>, --port <port>\n     Server port to connect to\n";
    std::cout << "   --help, -h\n     Show this help\n";
    std::cout << "   --\n";
    std::cout << "    Do not treat any remaining argument as a switch (at this level)\n\n";
    std::cout << " Multiple single-letter switches can be combined after\n";
    std::cout << " one `-`. For example, `-h-` is the same as `-h --`.\n";
    std::cout << " Supported commands:\n";
    std::cout << "   register <username> <password>\n";
    std::cout << "   login <username> <password>\n";
    std::cout << "   list\n   send <recipient> <subject> <body>\n   fetch <id>\n   logout\n";
}

/* Funkce na zjisteni, jestli je zadany string cislo */
bool is_number(std::string string) {
    size_t limit = string.length();
    // Jestlize nejaka hodnota ve stringu neni cislo, tak se o cislo nejedna
    for (size_t i = 0; i < limit; i++) {
        if (!(isdigit(string[i])))
            return false;
    }
    return true;
}

bool is_number_or_float(std::string string) {
    size_t limit = string.length();
    for (size_t i = 0; i < limit; i++) {
        if (!(isdigit(string[i])) && string[i] != '.' && string[i] != '-')  // Musim pocitat se zapornyma cislama i s floatama
            return false;
    }
    return true;
}

/* Pomocna struktura na zpracovavani argumentu - obsahuje vsechny mozne argumenty */
struct Parsed_args {
    std::string user_name, user_password, recipient, subject, body, id;
    bool reg, list, send, fetch, logout, login;
    std::string addr, port;
};

/* Funkce na vypis adresy a portu, pouzivana v pripade chyby -- ukonci program s chybou */
void print_error(Parsed_args args) {
    std::cout << "  hostname: " << args.addr << '\n';
    std::cout << "  port number: " << args.port << '\n';
    exit(1);
}

/* Funkce pro escapovani nekterych charakteru (viz referencni klient) */
std::string escape_characters(std::string message) {
    std::string new_message = "";
    for (unsigned int i = 0; i < message.length(); i++) {
        // Jestlize je nejaky charakter "\", musim zkontrolovat chrakter hned za nim
        if (message[i] == 92) {
            // Za \ se musi nachazet i nejaky znak
            if ((i + 1) >= message.length()) {
                std::cerr << "error while escaping characters \n";
                exit(1);
            }
            else {
                if (message[i+1] == '\"') {
                    new_message += '\"';
                    i++;
                }
                else if (message[i+1] == 92) {
                    new_message += '\\';
                    i++;
                }
                else if (message[i+1] == 'n') {
                    new_message += '\n';
                    i++;
                }
            }
        }
        else {
            new_message += message[i];
        }
    }
    return new_message;
}

std::string escape_args(std::string message) {
    std::string new_message = "";
    for (unsigned int i = 0; i < message.length(); i++) {
        if (message[i] == '\"') {
            new_message += "\\\"";
        }
        else if (message[i] == '\\') {
            new_message += "\\\\";
        }
        else if (message[i] == '\n') {
            new_message += "\\n";
        }
        else {
            new_message += message[i];
        }
    }

    return new_message;
}

/* Funkce pro zpracovani argumentu */
Parsed_args parse_args(int argc, char *argv[]) {
    std::string user_name, user_password, recipient, subject, body, id;
    user_name = user_password = recipient = subject = body = id = "";
    bool reg, list, send, fetch, logout, login;
    reg = list = send = fetch = logout = login = false;
    std::string addr, port;

    if (argc == 1) {
        std::cerr << "client: expects <command> [<args>] ... on the command line, given 0 arguments\n";
        exit(1);
    }

    std::regex switch_reg("^-");
    std::regex check_reg("^--");
    std::smatch result_match;
    int found = 0;
    int a_counter = 0;
    int p_counter = 0;

    // Vytvorim si vektor stringu pro usnadneni jejich escapu
    std::vector<std::string> vec;
    for (int i = 1; i < argc; i++) {
        std::string tmp_arg(argv[i]);

        // Escapuju kazdy argument
        tmp_arg = escape_args(tmp_arg);
        vec.push_back(tmp_arg);
    }

    unsigned int limit = (unsigned int) argc-1;
    for (unsigned int i = 0; i < limit; i++) {

        if (!std::regex_search(vec[i], result_match, check_reg)) {
            if (std::regex_search(vec[i], result_match, switch_reg)) {
                size_t length = vec[i].length();
                int count = 0;
                for (size_t k = 1; k < length; k++) {
                    
                    if (vec[i][k] == 'h') {
                        print_help();
                        exit(0);
                    }

                    else if (vec[i][k] == 'p') {
                        if (i+1 >= limit) {
                            std::cerr << "client: the \"-p\" option needs 1 argument, but 0 provided\n";
                            exit(1);
                        }
                        else {
                            p_counter++;
                            // Kontrola, jestli nebyl v ramci multiargumentu zadany nejaky swtich vicekrat
                            if (p_counter > 1) {
                                std::cerr << "client: only one instance of one option from (-p --port) is allowed\n";
                                exit(1);
                            }
                            found++;
                            if (count == 0) {
                                port = vec[i+1];
                                count++;
                                // Port a jeho kontrola maji prednost pred vsim ostatnim, podle ref. reseni
                                if(!(is_number(port))) {
                                    std::cerr << "Port number is not a string\n";
                                    exit(1);
                                }
                            }
                            else {
                                if (i+2 >= limit) {
                                    std::cerr << "client: the \"-p\" option needs 1 argument, but 0 provided\n";
                                    exit(1);
                                }
                                port = vec[i+2];
                                // Port a jeho kontrola maji prednost pred vsim ostatnim, podle ref. reseni
                                if(!(is_number(port))) {
                                    std::cerr << "Port number is not a string\n";
                                    exit(1);
                                }
                            }
                        }
                    }
                    else if (vec[i][k] == 'a') {
                        if (i+1 >= limit) {
                            std::cerr << "client: the \"-a\" option needs 1 argument, but 0 provided\n";
                            exit(1);
                        }
                        else {
                            a_counter++;
                            if (a_counter > 1) {
                                std::cerr << "client: only one instance of one option from (-a --address) is allowed\n";
                                exit(1);
                            }
                            found++;
                            if (count == 0) {
                                addr = vec[i+1];
                                count++;
                            }
                            else {
                                if (i+2 >= limit) {
                                    std::cerr << "client: the \"-a\" option needs 1 argument, but 0 provided\n";
                                    exit(1);
                                }
                                addr = vec[i+2];
                            }
                        }
                    }
                    else {
                        std::cerr << "client: unknown switch: -" << vec[i][k] << '\n';
                        exit(1); 
                    }
                }
            }
        }
        // Kontrola, jestlize argument zacinal 2x pomlckou, musi to byt jeden z nasledujicich 3
        else {
            if (strcmp(vec[i].c_str(), "--port") && strcmp(vec[i].c_str(), "--address") && strcmp(vec[i].c_str(), "--help")) {
                std::cerr << "client: unknown switch: " << vec[i] << '\n';
                exit(1); 
            }
        }
        // Musi se navysit index posunuti defaultne, at se nachazi na dalsim argumentu
        if (found > 0)
            found++;
        i += found;

        // Protoze se navysoval index kvuli prepinacum, je potreba jej zkontrolovat
        if (i >= limit) {
            std::cerr << "client: expects <command> [<args>] ... on the command line, given 0 arguments\n";
            exit(1);
        }

        // Kontrola, ze po posunuti neni dalsi na rade kratky prepinac -> Zpracovava se pouze v bloku vyse, tak se tam presunu
        if (std::regex_search(vec[i], result_match, switch_reg) && found) {
            found = 0;
            i--;    // Musim odecist 'i', protoze v dalsim cyklu dojde k jeho opetovnemu navyseni, tak at se nic nepreskoci
            continue;
        }
        found = 0;

        if (!strcmp(vec[i].c_str(), "register")) {
            if (i+2 >= limit || i+2 < limit-1) {
                std::cerr << "register <username> <password>\n";
                exit(1);
            }
            else {
                user_name = vec[i+1];
                user_password = vec[i+2];
                reg = true;
                break;
            }
        }

        else if (!strcmp(vec[i].c_str(), "login")) {
            if (i+2 >= limit || i+2 < limit-1) {
                std::cerr << "login <username> <password>\n";
                exit(1);
            }
            else {
                user_name = vec[i+1];
                user_password = vec[i+2];
                login = true;
                break;
            }
        }
        else if (!strcmp(vec[i].c_str(), "list")) {
            if (i < limit-1) {
                std::cerr << "list\n";
                exit(1);
            }
            list = true;
            break;
        }
        
        else if (!strcmp(vec[i].c_str(), "send")) {
            if (i+3 >= limit || i+3 < limit-1) {
                std::cerr << "send <recipient> <subject> <body>\n";
                exit(1);
            }
            else {
                recipient = vec[i+1];
                subject = vec[i+2];
                body = vec[i+3];
                send = true;
                break;
            }
        }

        else if (!strcmp(vec[i].c_str(), "fetch")) {
            if (i+1 >= limit || i+1 < limit-1) {
                std::cerr << "fetch <id>\n";
                exit(1);
            }
            else {
                id = (char*) vec[i+1].c_str();
                fetch = true;
                break;
            }
        }

        else if (!strcmp(vec[i].c_str(), "logout")) {
            if (i < limit-1) {
                std::cerr << "logout\n";
                exit(1);
            }
            else {
                logout = true;
                break;
            }
        }

        else if (!strcmp(vec[i].c_str(), "--address")) {
            // Znovu opet kontrola, ze se argument nevyskytl vicekrat
            a_counter++;
            if (a_counter > 1) {
                std::cerr << "client: only one instance of one option from (-a --address) is allowed\n";
                exit(1);
            }
            if (i+1 >= limit) {
                std::cerr << "client: the \"--address\" option needs 1 argument, but 0 provided\n";
                exit(1);
            }
            else {
                addr = vec[i+1];
                i++;
            }
        }
        
        else if (!strcmp(vec[i].c_str(), "--port")) {
            p_counter++;
            if (p_counter > 1) {
                std::cerr << "client: only one instance of one option from (-p --port) is allowed\n";
                exit(1);
            }
            if (i+1 > limit-1) {
                std::cerr << "client: the \"--port\" option needs 1 argument, but 0 provided\n";
                exit(1);
            }
            else {
                port = vec[i+1];
                if(!(is_number(port))) {
                    std::cerr << "Port number is not a string\n";
                    exit(1);
                }
                i++;
            }
        }

        else if (!strcmp(vec[i].c_str(), "--help")) {
            print_help();
            exit(1);
        }

        else {
            std::cerr << "unknown command\n";
            exit(1);
        }
    }

    // Defaultni hodnoty
    if (port.empty())
        port = "32323";
    if (addr.empty())
        addr = "localhost";

    if (!reg && !list && !send && !fetch && !logout && !login) {
        std::cerr << "client: expects <command> [<args>] ... on the command line, given 0 arguments\n";
        exit(1);
    }

    // Vracim strukturu se vsema argumentama - kdyz je uzivatel nezadal, jsou prazdne (nullptr)
    return Parsed_args{user_name, user_password, recipient, subject, body, id,
                       reg, list, send, fetch, logout, login, (char*) addr.c_str(), (char*) port.c_str()};
}

// https://stackoverflow.com/a/2602258
/* Funkce pro nacteni user tokenu ze souboru */
std::string read_user_token() {

    std::ifstream f("login-token");
    if (!f.is_open()) {
        std::cout << "Not logged in\n";
        exit(1);
    }
    std::stringstream token;
    token << f.rdbuf();
    f.close();

    return token.str();
}

/* Vytvoreni zpravy pro prikazy "login" a "register" */
std::string create_login_register_message(Parsed_args args, std::string msg) {

    if (args.login) {
        msg += "login \"";
    }
    else if (args.reg) {
        msg += "register \"";
    }

    // U prikazu login a register se pouziva base64 kodovani pro heslo 
    std::vector<unsigned char> password(args.user_password.c_str(), args.user_password.c_str() + args.user_password.length());
    std::string encoded_password = base64_encode(&password[0], password.size());
    msg = msg + args.user_name + "\" \"" +  encoded_password + "\"";
    return msg;
}

/* Vytvoreni zpravy pro prikaz "send" */
std::string create_send_message(Parsed_args args, std::string msg) {
    std::string user_token = read_user_token();
    msg = msg + "send " + user_token + " \"" + args.recipient + "\" \"" + args.subject + "\" \"" + args.body + "\"";
    return msg;
}

/* Vytvoreni zpravy pro prikaz "fetch" */
std::string create_fetch_message(Parsed_args args, std::string msg) {
    std::string user_token = read_user_token();
    if (!is_number_or_float(args.id)) {
        std::cerr << "ERROR: id " << args.id << " is not a number\n";
        exit(1);
    }
    msg = msg +  "fetch " + user_token + " " + args.id;
    return msg;
}

/* Vytvoreni zpravy pro prikaz "list" */
std::string create_list_message(std::string msg) {
    std::string user_token = read_user_token();
    msg = msg + "list " + user_token;
    return msg;
}

/* Vytvoreni zpravy pro prikaz "logout" */
std::string create_logout_message(std::string msg) {
    std::string user_token = read_user_token();
    msg = msg + "logout " + user_token;
    return msg;
}

/* Funkce pro vytvoreni tela TCP paketu, kterym je zprava,
 * ktera je na server poslana 
*/
std::string create_tcp_body(Parsed_args args) {
    std::string msg;
    // Zprava vzdy zacina zavorkou
    msg += '(';

    // Tvorba tela TCP paketu, ktery budu posilat
    // Podle zadanych argumentu rozlisuju, jaka zprava to bude
    if (args.list) {
        msg = create_list_message(msg);
    }

    else if (args.login || args.reg) {
        msg = create_login_register_message(args, msg);
    }

    else if (args.send) {
        msg = create_send_message(args, msg);
    }

    else if (args.fetch) {
        msg = create_fetch_message(args, msg);
    }

    else if (args.logout) {
        msg = create_logout_message(msg);
    }

    // Zprava vzdy zavorkou i konci
    msg += ')';

    return msg;
}

/* Funkce pro ziskani N-te casti odpovedi (casti uvnitr uvozovek) */
std::string get_nth_part_of_response(std::string response, int part) {
    int counter = 0;
    int begin;
    int end;
    if (part > 1)
        part += part-1;
    begin = end = -1;
    for (unsigned int i = 0; i < response.length(); i++) {
        if (response[i] == 34) {
            if (i >= 2) {
                if (response[i-1] == '\\') {
                    if (response[i-2] != '\\')
                        continue;
                }
            }
            counter++;
            if (begin != -1) {
                end = i;
                break;
            }
            if (counter == part) {
                begin = i;
            }
        }
    }
    std::string result = response.substr(begin+1, (end-begin-1));
    return result;
}

/* Funkce pro vypsani odpovedi na prikaz "list" */
void print_list_messages(std::string buffer_string) {
    std::vector<std::string> list_of_strings;
    int first_bracket, second_bracket;
    first_bracket = second_bracket = -1;
    bool flag1, flag2;
    flag1 = flag2 = false;
    bool flag_begin = true;
    for(unsigned int i = 1; i < buffer_string.length()-1; i++) {    // Zacinam od 1, at nectu prvni zavorku na zacatku zpravy


        if (buffer_string[i] == '(' && i < 8) {     // 8 Kvuli tomu, at nectu zavorky ve zprave, ale jen "oteviraci"
            first_bracket = i;
            flag1 = true;
            flag_begin = false;
        }
        else if (buffer_string[i] == '(' && !flag_begin) {
            // Kontrola, ze zavorka je opravdu zacatek a ne soucast textu
            if (i > 3) {
                if (buffer_string[i-1] == ' ' && buffer_string[i-2] == ')' && buffer_string[i-3] == '\"' && buffer_string[i-4] != '\\') {
                    first_bracket = i;
                    flag1 = true;
                }
            }
            // Else by nemel nikdy nastat
            else {
                first_bracket = i;
                flag1 = true;
            }
        }
        else if (buffer_string[i] == ')') {
            if (i > 1) {
                // Kontrola, ze zavorka je opravdu konec zpravy a ne soucast textu
                if (buffer_string[i-1] == '\"' && buffer_string[i-2] != '\\') {
                    second_bracket = i;
                    flag2 = true;
                }
            }
            // Else by nemel nikdy nastat
            else {
                second_bracket = i;
                flag2 = true;
            }
        }

        if (flag1 && flag2) {
            std::string message = buffer_string.substr(first_bracket+1, (second_bracket-first_bracket-1));
            list_of_strings.push_back(message);
            if (message == "") {
                break;
            }
            flag1 = flag2 = false;
        }
    }
    for (std::string message : list_of_strings) {
        std::string number_part = "";
        for (unsigned int i = 0; i < message.length(); i++) {
            if (isdigit(message[i]))
                number_part += message[i];
            else
                break;
        }
        std::cout << number_part << ':' << '\n';
        std::cout << "  From: " <<  escape_characters(get_nth_part_of_response(message, 1)) << '\n';
        std::cout << "  Subject: " << escape_characters(get_nth_part_of_response(message, 2)) << '\n';
    }


}

/* Vypsani odpovedi na ostatni prikazy */
void print_response(Parsed_args args, std::string buffer) {
    std::regex error_reg("^\\(err");
    std::regex ok_reg("^\\(ok+");
    std::smatch result_match;

    if (std::regex_search(buffer, result_match, ok_reg)) {
        /* 
         * login: User logged in
         * register: registered user <user>
         * list: Not logged in
         * send: message sent
         * fetch: Not logged in, message id not found
         * logout: logged out
        */

        // User logged in + vytvori login-token
        if (args.login) {
            std::string body = get_nth_part_of_response(buffer, 1);
            std::cout << "SUCCESS: " << body << '\n';
            std::string token = get_nth_part_of_response(buffer, 2);
            std::ofstream login_token("login-token");
            login_token << "\"" << token << "\"";
            login_token.close();
        }   // registered user <user>
        else if (args.reg) {
            std::string body = escape_characters(get_nth_part_of_response(buffer, 1));
            std::cout << "SUCCESS: " << body << '\n';
        }   // SUCCESS: zpravy
        else if (args.list) {
            std::cout << "SUCCESS:\n";
            print_list_messages(buffer);

        }   // message sent
        else if (args.send) {
            std::string body = get_nth_part_of_response(buffer, 1);
            std::cout << "SUCCESS: " << body << '\n';
        }
        else if (args.fetch) {
            std::string sender = escape_characters(get_nth_part_of_response(buffer, 1));
            std::string subject = escape_characters(get_nth_part_of_response(buffer, 2));
            std::string body = escape_characters(get_nth_part_of_response(buffer, 3));

            std::cout << "SUCCESS:\n\n" << "From: " << sender << '\n';
            std::cout << "Subject: " << subject << "\n\n";
            std::cout << body;
        }   // logged out + smaze login-token
        else if (args.logout) {

            if (remove("login-token") != 0) {
                std::cout << "Internal client error when logging out!\n";
                exit(1);
            }

            std::string body = get_nth_part_of_response(buffer, 1);
            std::cout << "SUCCESS: " << body << '\n';
        }

    }
    else if (std::regex_search(buffer, result_match, error_reg)) {
        /* Vypada to, ze vsechno jsou 1 radkove errory a nemusim rozlisovat, odkud prisli
         * login: incorrect password, unknown user
         * register: user already registered
         * list: Not logged in
         * send: Not logged in, unknown recipient
         * fetch: Not logged in, message id not found
         * logout: Not logged in
        */

        std::string body = get_nth_part_of_response(buffer, 1);
        std::cerr << "ERROR: " << body << '\n';
    }
    
    else {
        std::cerr << "Unknown internal server-side error (client received wrong packet)\n";
        exit(1);
    }
}

/* Funkce na kontrolu argumentu, ktera vlastne akorat zkontroluje, jestli je port cislo -- v momentalni fazi USELESS*/
void check_args(Parsed_args args) {
    if (!(is_number(args.port))) {
        std::cerr << "Port number is not a string\n";
        exit(1);
    }
}

// https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
// Nize se nachazi text nutny kvuli vyuziti kodu z dane stranky
/* 
Permission is granted to make and distribute verbatim copies of this
manual provided the copyright notice and this permission notice are
preserved on all copies.

Permission is granted to copy and distribute modified versions of this
manual under the conditions for verbatim copying, provided that the
entire resulting derived work is distributed under the terms of a
permission notice identical to this one.

Since the Linux kernel and libraries are constantly changing, this
manual page may be incorrect or out-of-date.  The author(s) assume no
responsibility for errors or omissions, or for damages resulting from
the use of the information contained herein.  The author(s) may not
have taken the same level of care in the production of this manual,
which is licensed free of charge, as they might when working
professionally.

Formatted or processed versions of this manual, if unaccompanied by
the source, must acknowledge the copyright and authors of this work.
*/
/*
 * Hlavni ridici funkce, ktera posila zpravy na server a zpracovava odpovedi
*/
void send_and_receive(Parsed_args args) {
    
    struct addrinfo hints;
    struct addrinfo *result, *tmp;
    int sock, s;
    char buffer[2048];

    hints.ai_family = AF_UNSPEC;     // Pro IPv4 i IPv6
    hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM pro TCP
    hints.ai_flags = 0;              // Nastavuju v podstate jenom kvuli getaddrinfo
    hints.ai_protocol = 0;           // Protokol pro socktype

    // Konverze adresy (i pripadneho hostname) na neco, s cim muzu pracovat
    s = getaddrinfo(args.addr.c_str(), args.port.c_str(), &hints, &result);
    if (s != 0) {
        std::cerr << "Error with address resolution \n";
        exit(1);
    }

    /* getaddrinfo vraci seznam adresovych struktur, takze musim zjistit,
     * na jakou z nich se mi povede pripojit 
    */
    for (tmp = result; tmp != NULL; tmp = tmp->ai_next) {
        sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
        // Kdyz byla tahle adresa neuspech (Nepovedlo se vytvorit socket), pokracuju dal
        if (sock == -1)
            continue;

        // Pokud se povedlo vytvorit socket a pripojit se na nej, muzu koncit prohledavani
        if (connect(sock, tmp->ai_addr, tmp->ai_addrlen) != -1)
            break;
    }

    // Pokud se na zadnou adresu pripojit nepovedlo, je neco spatne a musim skoncit
    if (tmp == NULL) {
        std::cerr << "tcp connection failed:" << '\n';
        print_error(args);
    }
    
    // Pokud jsem se dostal az sem, tak se vsechno povedlo a ja muzu poslat paket se zpravou
    check_args(args);
    std::string msg = create_tcp_body(args);
    send(sock, msg.c_str(), msg.length(), 0);

    std::string received_msg = "";
    int valread = -1;

    // Pokud by byla serverova odpoved moc velka na 1 paket, musim resit i to, ze ji prectu celou
    do {
        valread = read(sock, buffer, 2048);
        // Server uz nema co poslat, tak muzu skoncit
        if (valread == 0)
            break;
        // Jestlize se nepovedlo zpracovat odpoved, doslo k nejake chybe po ceste paketu
        else if (valread == -1) {
            std::cerr << "Unknown error during packet reading\n";
            exit(1);
        }

        std::string tmp_buf (buffer);
        // Po ceste se na paket nalepi neporadek, tak ho musim useknout na ocekavanou hodnotu
        received_msg += tmp_buf.substr(0, valread);
    } while (valread > 0);

    // Jestlize k chybe nedoslo, muzu uvolnit pamet, vypsat odpoved serveru a skoncit
    close(sock);
    freeaddrinfo(result);

    print_response(args, received_msg);
}

int main(int argc, char **argv) {
    Parsed_args args = parse_args(argc, argv);
    send_and_receive(args);
    return 0;
}
