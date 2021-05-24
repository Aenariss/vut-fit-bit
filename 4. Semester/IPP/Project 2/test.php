<?php
/*
* IPP Projekt 2 - test.php
* Autor: Vojtech Fiala <xfiala61>
*/


// Globalni promenne pro zjednoduseni
$help = False;
$recursive = False;
$int_file_given = False;
$parse_file_given = False;
$parse_only = False;
$int_only = False;

$search_dir = './';
$parse_file = './parse.php';
$int_file = './interpret.py';
$jexamxml_file = '/pub/courses/ipp/jexamxml/jexamxml.jar';
$jexamxml_config = '/pub/courses/ipp/jexamxml/options';

function print_help() {
    print("Skript akceptuje nasledujici parametry:\n");
    print("                 --help               -> Vypise tuto napovedu\n");
    print("                 --directory=path     -> Testy budou hledany v dane slozce. Pokud neni parametr uveden, hleda se v aktualni slozce.\n");
    print("                 --recursive          -> Program bude hledat testy rekurzivne i v podadresarich aktualniho adresare.\n");
    print("                 --parse-script=file  -> Soubor se skriptem parse.php. Pokud neni uveden, hleda se v aktualnim adresari.\n");
    print("                 --int-script=file    -> Soubor s interpretem interpret.py, Pokud neni uveden, hleda se v aktualnim adresari\n");
    print("                 --parse-only         -> Spusti pouze testy skriptu parse.php. Nelze kombinovat s --int-only a --int-script.\n");
    print("                 --int-only           -> Spusti pouze testy interpretu. Nelze kombinovat s --parse-only a --parse-script.\n");
    print("                 --jexamxml=file      -> JAR soubor s JExamXML nastrojem. Pokud neni uveden, uvazuje se /pub/courses/ipp/jexamxml/jexamxml.jar na serveru Merlin.\n");
    print("                 --jexamcfg=file      -> Konfigurace nastroje JExamXML. Pokud neni uvedena, uvazuje se /pub/courses/ipp/jexamxml/options na serveru Merlin.\n");
}

// Funkce pro ukonceni programu se zpravou na stderr a danou navratovou hodnotou
function stderr_exit($message, $code) {
    fwrite(STDERR, $message);
    exit($code);
}

// Pomocna funkce pro ziskani cesty k souboru/slozce z argumentu
function get_path($arg) {
    $arg = explode('=', $arg);
    if (count($arg) > 2) {
        stderr_exit("Invalid directory!\n", 41);
    }
    return $arg[1];
}

// Parsovani argumentu
function getopts() {
    global $argv;
    global $help;
    global $search_dir;
    global $recursive;
    global $parse_file;
    global $int_file;
    global $int_file_given;
    global $parse_file_given;
    global $parse_only;
    global $int_only;
    global $jexamxml_file;
    global $jexamxml_config;
    
    // Jestli je jen 1 argument, neni treba je zpracovavat (mam jen jmeno programu)
    if (count($argv) > 1) {
        // Projdi vsechny argumenty a napasuj je na povolene argumenty za pomoci regularnich vyrazu
        foreach($argv as $argument => $arg) {

            if ($argument < 1) 
                continue;

            if (preg_match("/^--help$/", $arg)) {
                $help = True;
            }
            else if (preg_match("/^--directory=.+$/", $arg)) {
                $search_dir = get_path($arg);
            }
            else if (preg_match("/^--recursive$/", $arg)) {
                $recursive = True;
            }
            else if (preg_match("/^--parse-script=.+$/", $arg)) {
                $parse_file = get_path($arg);
                $parse_file_given = True;
            }
            else if (preg_match("/^--int-script=.+$/", $arg)) {
                $int_file = get_path($arg);
                $int_file_given = True;
            }
            else if (preg_match("/^--parse-only$/", $arg)) {
                $parse_only = True;
            }
            else if (preg_match("/^--int-only$/", $arg)) {
                $int_only = True;
            }
            else if (preg_match("/^--jexamxml=.+$/", $arg)) {
                $jexamxml_file = get_path($arg);
            }
            else if (preg_match("/^--jexamcfg=.+$/", $arg)) {
                $jexamxml_config = get_path($arg);
            }
            // Jestlize se na nic z predchozich napasovat nepovedlo, argument je neplatny
            else {
                stderr_exit("Invalid parameter!\n", 10);
            }
        }
    }

    // Zkontroluj, jestli nedoslo k nepovolene kombinaci argumentu
    if ($help && count($argv) < 3) {
        print_help();
        exit(0);
    }
    else if (($help && count($argv) >= 3) || ($parse_only && $int_only) || ($parse_only && $int_file_given) || ($int_only && $parse_file_given)) {
        stderr_exit("Invalid parameter combination!\n", 10);
    }
    else if (!(file_exists($search_dir))) {
        stderr_exit("Folder not found!\n", 41);
    }
    else if (!(file_exists($parse_file)) && ($parse_only)) {
        stderr_exit("parse.php not found!\n", 41);
    }
    else if (!(file_exists($int_file)) && ($int_only)) {
        stderr_exit("interpret.py not found!\n", 41);
    }
    if (!(file_exists($jexamxml_config)) || !(file_exists($jexamxml_file))) { 
        stderr_exit("Invalid JExamlXML location!\n", 41);
    }
}

// Pomocna funkce na pridani nazvu soouboru do arraye, z nehoz soubory se potom budou spoustet v testech
function push_to_array($iterator, $array) {

    foreach($iterator as $file) {
        if($file->getExtension() == 'src') {
            array_push($array , $file->getPathname());
        }
    }
    return $array;
}

// Ziskej soubory, na kterych se potom spusti testy
function get_files() {
    global $recursive;
    global $search_dir;
    $files = [];

    // Zkus odchytnout chyby, ktere muzoou a nemusi nastat
    try {
        // Jestlize byl zadan rekurzivni parametr, projdi i podslozky
        if ($recursive) {
            $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($search_dir, RecursiveDirectoryIterator::SKIP_DOTS), RecursiveIteratorIterator::SELF_FIRST);
            $files = push_to_array($iterator, $files);
        }
        // Jinak se zpracovava jenom aktualni adresar
        else {
            $iterator = new DirectoryIterator($search_dir);
            $files = push_to_array($iterator, $files);
        }
    } 
    catch (Exception $except) {
        stderr_exit("Error occured when scanning for files!\n", 41);
    }
    
    // Serad soubory abecedne, nehlede na velikost pismen
    usort($files, 'strnatcasecmp');
    return $files;
}

// Podpurna funkce pro vytvoreni novych souboru
function create_file($file, $rc) {
    $tmp = fopen($file, "w");
    if (!$tmp) {
        stderr_exit("Internal error!\n", 11);
    }
    // .rc soubor musi obsahovat default hodnotu 0
    if ($rc) {
        fwrite($tmp, '0');
    }
    fclose($tmp);
}

// Precti soubor a ziskej obsah
function get_content($file) {
    // Zkontroluj, jestli ho vubec muzes otevrit
    try {
        $tmp = fopen($file, "r");
    }
    catch (Exception $except) {
        stderr_exit("Internal error!", 11);
    }

    // Jestlize je velikost souboru 0, je prazdny, takze vrat prazdny string
    if (filesize($file) == 0) {
        $ret_code = "";
    }
    // Jinak nacti cely soubor
    else {
        $ret_code = fread($tmp, filesize($file));
    }
    fclose($tmp);
    return $ret_code;
}

// Smaz vsechny docasne soubory
function clenaup() {
    if (file_exists("delta.xml"))
        unlink("delta.xml");
    if (file_exists("tmp_parse_out_dont_remove_anything"))
        unlink("tmp_parse_out_dont_remove_anything");
    if (file_exists("tmp_parse_out_dont_remove_anything.log"))
        unlink("tmp_parse_out_dont_remove_anything.log");
    if (file_exists("tmp_interpreter_out_dont_remove_anything"))
        unlink("tmp_interpreter_out_dont_remove_anything");
}

// Vytvor html hlavicku
function html_print_beginning() {
    print("<head>
    <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">
    <title>IPP Výsledky testů</title>
    <style type=\"text/css\">
    body {
        font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    }
    
    table, th, td
    {
      border: 1px solid black;
      padding: 2 5 2 7px;
      text-align: center;
    }
    table
    {
    border-collapse:collapse;
    width:95%;
    }
    th
    {
    background-color:#212F3D;
    color:white;
    }
    .good
    {
    background-color:green;
    color:white
    }
    .bad
    {
    background-color:red;
    color:white
    }
    .gray {
        background-color: #A9A9A9;
        color: black;
    }
    .silver {
        background-color: #D3D3D3;
        color: black;
    }
    
    </style>
    </head>
    <body>
    <p style=\"font-size: 20px; text-align: center;\">Pro skok na výsledky klikněte <a href=\"#Results\">ZDE</a></p>
    <table>
    <th>Název testu</th><th>Očekávaná návratová hodnota</th><th>Získaná návratová hodnota</th><th>Porovnání výstupu<th>Výsledek</th>");
}

// Pridej uspesny test do html
function html_add_correct($file, $expected_ret, $my_ret, $i) {
    if ($i % 2) {
        print("<tr class=\"gray\">
        <td>".$file."</td>
        <td>".$expected_ret."</td>
        <td>".$my_ret."</td>
        <td class=\"good\">✔</td>
        <td class=\"good\">✔</td>
        </tr>");
    } 
    else {
        print("<tr class=\"silver\">
        <td>".$file."</td>
        <td>".$expected_ret."</td>
        <td>".$my_ret."</td>
        <td class=\"good\">✔</td>
        <td class=\"good\">✔</td>
        </tr>");
    }
}

// Pridej neuspesny test do html
function html_add_wrong($file, $expected_ret, $my_ret, $i) {
    if ($i % 2) {
        print("<tr class=\"gray\">
        <td>".$file."</td>
        <td>".$expected_ret."</td>
        <td>".$my_ret."</td>
        <td class=\"bad\">✖</td>
        <td class=\"bad\">✖</td>
        </tr>");
    } 
    else {
        print("<tr class=\"silver\">
        <td>".$file."</td>
        <td>".$expected_ret."</td>
        <td>".$my_ret."</td>
        <td class=\"bad\">✖</td>
        <td class=\"bad\">✖</td>
        </tr>");
    }
}

// Pridej celkove vysledky do html
function html_print_result($correct, $all) {
$wrong = $all - $correct;
    print("</table>
    <div id=\"Results\" style=\"font-size: 30px; text-align: center;font-weight: bold;\">Výsledky</div>
    <div style=\"font-size: 25px; text-align: center;font-weight: bold;\">Úspěšných: " . $correct . "</div>
    <div style=\"font-size: 25px; text-align: center;font-weight: bold;\">Neúspěšných: " . $wrong . "</div>
    <div style=\"font-size: 25px; text-align: center;font-weight: bold;\">Celkem: " . $all . "</div>");
    
    if ($all == $correct) {
        print("<div style=\"font-size: 30px; text-align: center;font-weight: bold; color: green; padding-top:2rem;\">Všechny testy proběhly úspěšně!</div>");
    }
}

// Pridej konec html souboru
function html_print_end() {
    print("</body>
    </html>");
}

// Podpurna funkcce na rozhodnuti jestli je vysledek testu spravne, pokud ocekavana navratova hodnota neni 0
function check_ret_not_zero($ret, $cmp, $file, $i) {
    if ($ret == $cmp) {
        html_add_correct($file, $ret, $cmp, $i);
        return True;
    }
    else {
        html_add_wrong($file, $ret, $cmp, $i);
        return False;
    }
}

//  Podpurna funkcce na rozhodnuti jestli je vysledek testu spravne, pokud ocekavana navratova hodnota je 0
function check_ret_zero($results, $check, $file, $ret, $cmp, $i) {
    if ($results) {
        if (!($check)) {
            html_add_correct($file, $ret, $cmp, $i);
            return True;
        }
        else {
            html_add_wrong($file, $ret, $cmp, $i);
            return False;
        }
    }
    else {
        html_add_wrong($file, $ret, $cmp, $i);
        return False;
    }
}

// Hlavni ridici funkce, ktera spousti jednotlive testy
function run_tests($files) {
    global $parse_only;
    global $int_only;
    global $parse_file;
    global $int_file;
    global $jexamxml_config;
    global $jexamxml_file;
    $tmp_parse_out = "tmp_parse_out_dont_remove_anything";
    $tmp_int_out = "tmp_interpreter_out_dont_remove_anything";
    $output = NULL;
    $correct = 0;
    $all_files = count($files);
    $i = 0;
    html_print_beginning();

    // Projdi vsechny .src soubory
    foreach ($files as $file) {

        // Zkontroluj, jestli soubory .out, .in a .rc existuji, pripadne je vytvor
        $pure_filename = str_replace('.src', '', $file);
        $out_file = $pure_filename . '.out';
        $in_file = $pure_filename . '.in';
        $rc_file = $pure_filename . '.rc';

        if (!file_exists($out_file)) {
            create_file($out_file, 0);
        }
        if (!file_exists($in_file)) {
            create_file($in_file, 0);
        }
        if (!file_exists($rc_file)) {
            create_file($rc_file, 1);
        }

        if ($parse_only) {
            $parser_return = 0;
            $jexam_return = 0;
            // Ziskej ocekavanou navratovou hodnotu
            $ret = get_content($rc_file);
            exec("php7.4 {$parse_file} < {$file} > {$tmp_parse_out}", $output, $parser_return);

            // Jestlize to nebyla 0, nemusis porovnavat vystup
            if ($ret == 0)
                exec("java -jar {$jexamxml_file} {$tmp_parse_out} {$out_file} delta.xml {$jexamxml_config}", $output, $jexam_return);

            if ($ret != 0) {
                if (check_ret_not_zero($ret, $parser_return, $file, $i))
                    $correct++;
            }
            else {
                $results = ($ret == $parser_return);
                if (check_ret_zero($results, $jexam_return, $file, $ret, $parser_return, $i))
                    $correct++;
            }
        }
        else if ($int_only) {
            $int_return = 0;
            $diff_ret = 0;
            $ret = get_content($rc_file);
            exec("python3.8 {$int_file} --source={$file} --input={$in_file} > {$tmp_int_out}", $output, $int_return);
            if ($ret == 0) {
                exec("diff {$out_file} {$tmp_int_out}", $output, $diff_ret);
            }

            if ($ret != 0) {
                if (check_ret_not_zero($ret, $int_return, $file, $i))
                    $correct++;
            }
            else {
                $results = ($ret == $int_return);
                if (check_ret_zero($results, $diff_ret, $file, $ret, $int_return, $i))
                    $correct++;
            }
        }
        else {
            $parser_return = 0;
            $int_return = 0;
            $diff_ret = 0;
            $ret = get_content($rc_file);
            exec("php7.4 {$parse_file} < {$file} > {$tmp_parse_out}", $output, $parser_return);
            exec("python3.8 {$int_file} --source={$tmp_parse_out} --input={$in_file} > {$tmp_int_out}", $output, $int_return);

            if ($ret == 0)
                exec("diff {$out_file} {$tmp_int_out}", $output, $diff_ret);

            if ($ret != 0) {
                if (check_ret_not_zero($ret, $int_return, $file, $i))
                    $correct++;
            }
            else {
                $results = ($ret == $int_return);
                if (check_ret_zero($results, $diff_ret, $file, $ret, $int_return, $i))
                    $correct++;
            }
        }
        $i++;
    }
    html_print_result($correct, $all_files);
    html_print_end();
}

function main_fc() {
    getopts();
    $files = get_files();
    run_tests($files);
    clenaup();
}

main_fc()
?>
