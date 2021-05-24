#!/usr/bin/env python3

# IPP Projekt 2 - Interpret.py
# Autor: Vojtech Fiala <xfiala61>

import sys
import re
import xml.etree.ElementTree as ET

# Globalni promenne pro zjednoduseni prace
labels = {}
temp_frame_init = False
local_frame_init = False
global_frame = {}
global_frame_types = {}
temp_frame = {}
temp_frame_types = {}
local_frame = {}
local_frame_types = {}
local_frame_stack = []
local_frame_types_stack = []
call_stack = []
var_stack = []
var_stack_type = []

def print_help():
    print("Skript akceptuje nasledujici parametry:")
    print("        --help                -> Vypise tuto napovedu.")
    print("        --source=file         -> Vstupni soubor s XML reprezentaci zdrojoveho kodu. Nelze kombinovat s --input.")
    print("        --input=file          -> Soubor se vstupy pro samotnou interpretaci zadaneho zdrojoveho kodu. Nelze kombinovat s --source")
    print("        Alespon jeden z parametru --source nebo --input musi byt vzdy zadan. Pokud jeden z nich chybi, jsou data ctena ze STDIN.")

# Podpurna funkce pro ukonceni programu s chybovou hlaskou na STDERR a spravnou navratovou hodnotou
def stderr_exit(message, code):
    sys.stderr.write(message)
    exit(code)

# Funkce na ziskani cesty k souboru z argumentu
def getPath(path):
    path = path.split('=')
    if (len(path) != 2):
        stderr_exit("Neplatna cesta k souboru!", 11)
    return path[1]

# Funkce na zpracovani argumentu
def parse_args():
    help_toggle = False
    source_toggle = False
    source_path = ""
    input_path = ""
    input_toggle = False
    # Zacni az od 2., protoze 1. je nazev souboru a ten me nezajima
    for arg in sys.argv[1:]:
        if (re.search("^--help$", arg)):
            help_toggle = True
        elif (re.search("^--source=.+$", arg)):
            source_toggle = True
            source_path = getPath(arg)
        elif (re.search("^--input=.+$", arg)):
            input_toggle = True
            input_path = getPath(arg)
        else:
            stderr_exit("Neplatny argument! Pro zjisteni povolenych argumentu, pouzijte parametr --help\n", 10)

    # Kontrola, jestli nebyly zadane nepovolene kombinacce argumentu
    if (help_toggle and len(sys.argv) > 2):
        stderr_exit("Neplatna kombinace argumentu! Pro zjisteni povolenych argumentu, pouzijte parametr --help\n", 10)

    if (help_toggle):
        print_help()
        exit(0)
    
    if (not source_toggle and not input_toggle):
        stderr_exit("Neplatna kombinace argumentu! Pro zjisteni povolenych argumentu, pouzijte parametr --help\n", 10)

    return source_path, input_path

# Funkce pro zjisteni, zda-li zadane soubory existuji
def testFiles(source_path, input_path):
    if (source_path != ""):
        try:
            f = open(source_path)
            f.close()
        except:
            exit(11)
    if (input_path != ""):
        try:
            f = open(input_path)
            f.close()
        except:
            exit(11)

# Nacti obsah zadanycch souboru, pripadne stdin
def loadContent(source_path, input_path):

    testFiles(source_path, input_path)
    input_get = None
    tree = None

    # Jestlize nebyl zadany source argument, cti ze stdin
    try:
        if (source_path == ""):
            tree = ET.parse(sys.stdin)
        else:
            tree = ET.parse(source_path)
    except:
        sys.exit(31)

    if (input_path != ""):
        f = open(input_path, "r")
        sys.stdin = f
    return tree

# Funkce pro ziskani hodnoty promenne
def getVarValue(var):
    global global_frame
    global local_frame
    global temp_frame

    if (var == None):
        return ""

    # Jestlize je kratsi nez 4 (nejkratsi mozne jmeno promenne)
    # Tak rovnou vrat co jsi dostal, neni duvod ziskavat hodnotu, toto uz je hodnota
    var = str(var)
    if (len(var) < 4):
        return var

    # Nacti z pripadne promenne hodnotu
    frame = var[:3]
    name = var[3:]
    value = None
    # Zjisti ramec, podle ktereho spravne prirad promennou
    if (frame == "GF@"):
        if not (varExists(name, global_frame)):
            exit(54)
        value = global_frame.get(name)
    elif (frame == "LF@"):
        if not (frame_initialized(local_frame)):
            exit(55)
        if not (varExists(name, local_frame)):
            exit(54)
        value = local_frame.get(name)
    elif (frame == "TF@"):
        if not (frame_initialized(temp_frame)):
            exit(55)
        if not (varExists(name, temp_frame)):
            exit(54)
        value = temp_frame.get(name)
    else:
        return var

    return value

# Funkce pro ziskani typu promenne
def getVarType(symb, typee):
    global local_frame_types
    global global_frame_types
    global temp_frame_types
    if (typee == 'int'):
        return "int"
    if (typee == 'string'):
        return "string"
    if (typee == 'bool'):
        return "bool"
    if (typee == 'nil'):
        return "nil"
    if (typee == 'var'):
        frame, name = getFrame(symb)
        if (frame == "GF@"):
            return global_frame_types.get(name)
        elif (frame == "TF@"):
            return temp_frame_types.get(name)
        else:
            return local_frame_types.get(name) 

# Funkce pro prekonvertovani specialnich znaku stringu (\005 etc) na znaky
def convertString(string):
    ret = ""
    i = 0
    if (string == ""):
        return ret
    if (string == None):
        return None
    length = len(string)
    while i < length:
        if (string[i] == '\\'):
            try:
                val = chr(int(string[i+1] + string[i+2] + string[i+3]))
            except:
                exit(52)
            i += 3
        else:
            val = string[i]
        ret += val
        i+=1

    return ret

# Funkce na kontrolu validity vyrazu daneho typu
def validVar(var):
    if (re.search("^(LF|GF|TF)@[A-Za-z_\-$&%*!?][A-Za-z0-9_\-$&%*!?]*$", var)):
        return True
    return False

def validInt(intt):
    if (re.search("^(\-|\+)?[0-9]+$", intt)):
        return True
    return False

def validString(string):
    if (string != None):
        length = len(string)
        i = 0
        while (i < length):
            # Jestlize se ve stringu nachazi backslash, musi za nim byt 3 cisla
            if (string[i] == '\\'):
                if (i + 3 < length):
                    if (ord(string[i+1]) < 48 or ord(string[i+1]) > 57):
                        exit(32)
                    if (ord(string[i+2]) < 48 or ord(string[i+2]) > 57):
                        exit(32)
                    if (ord(string[i+3]) < 48 or ord(string[i+3]) > 57):
                        exit(32)
                else:
                    exit(32)
            i += 1

    return True

def validBool(bool_val):
    if (re.search("^(true|false)$", bool_val)):
        return True
    return False

def validNil(nil):
    if (re.search("^nil$", nil)):
        return True
    return False

def validLabel(label):
    if (re.search("^[A-Za-z_\-$&%*!?][A-Za-z0-9_\-$&%*!?]*$", label)):
        return True
    return False

def validSymb(symb_type, symb):
    if (symb_type == "string"):
        if (validString(symb)):
            return True
    elif (symb_type == "int"):
        if (validInt(symb)):
            return True
    elif (symb_type == "var"):
        if (validVar(symb)):
            return True
    elif (symb_type == "bool"):
        if (validBool(symb)):
            return True
    elif (symb_type == "nil"):
        if (validNil(symb)):
            return True
    return False

def validType(typee):
    if (re.search("^(int|string|bool)$", typee)):
        return True
    return False

# Podpurne funkce na kontrolu, jestli instrukce obsahuje spravny pocet argumentu validniho typu
def checkLengths(parent, check):
    if (len(parent) != check):
        exit(32)

# Funkce na serazeni argumentu instrukci v pripade, ze jsou argumenty 3
def sortinstrArgs(parent, arg1, arg2, arg3):
    if (arg1.tag == "arg1" and arg2.tag == "arg3" and arg3.tag == "arg2"):
        tmp = arg2
        tmpp = arg3
        arg3 = arg2
        arg2 = tmpp

        parent[2] = arg3
        parent[1] = arg2

    if (arg1.tag == "arg2" and arg2.tag == "arg1" and arg3.tag == "arg3"):
        tmp = arg2
        tmpp = arg1
        arg2 = tmpp
        arg1 = tmp
        parent[1] = arg2
        parent[0] = arg1

    if (arg1.tag == "arg2" and arg2.tag == "arg3" and arg3.tag == "arg1"):
        tmp = arg2
        tmp2 = arg1
        tmp3 = arg3

        arg3 = tmp
        arg1 = tmp3
        arg2 = tmp2

        parent[1] = arg2
        parent[2] = arg3
        parent[0] = arg1

    if (arg1.tag == "arg3" and arg2.tag == "arg2" and arg3.tag == "arg1"):
        tmp = arg1
        tmpp = arg3

        arg1 = tmpp
        arg3 = tmp
        parent[2] = arg3
        parent[0] = arg1

    if (arg1.tag == "arg3" and arg2.tag == "arg1" and arg3.tag == "arg2"):
        tmp = arg1
        tmp2 = arg2
        tmp3 = arg3

        arg3 = tmp
        arg1 = tmp2
        arg2 = tmp3

        parent[1] = arg2
        parent[2] = arg3
        parent[0] = arg1

    return parent, arg1, arg2, arg3

# Funkce na kontrolu spravnosti syntaxe jednotlivych typu argumentu promennych
def checkVar(parent):
    arg1 = parent[0]
    if (arg1.tag != "arg1"):
        exit(32)
    if (arg1.get('type') != "var"):
        exit(32)
    if (arg1.text == None):
        exit(32)
    if (not validVar(arg1.text)):
        exit(32)
    return parent

def checkVarSymb(parent):
    arg1 = parent[0]
    arg2 = parent[1]
    if (arg1.tag == "arg2" and arg2.tag == "arg1"):
        tmp = arg2
        arg2 = arg1
        parent[1] = arg1
        arg1 = tmp
        parent[0] = tmp

    if (arg1.text == None):
            exit(32)
    if (arg2.get('type') != "string"):
        if (arg2.text == None):
            exit(32)
    try:
        if (arg2.get('type') == 'string'):
            arg2.text = convertString(arg2.text)
    except:
        exit(32)

    if (arg1.tag != "arg1" or arg2.tag != "arg2"):
        exit(32)
    if (arg1.get('type') != "var"):
        exit(32)
    if (arg2.get('type') != "string" and arg2.get('type') != "bool" and arg2.get('type') != "var" and arg2.get('type') != "int" and arg2.get('type') != "nil"):
        exit(32)
    if (not validVar(arg1.text)):
        exit(32)
    if (not validSymb(arg2.get('type'), arg2.text)):
        exit(32)
    return parent

def checkVarSymbSymb(parent):
    arg1 = parent[0]
    arg2 = parent[1]
    arg3 = parent[2]

    if (arg2.get('type') != "string"):
        if (arg2.text == None):
            exit(32)
    if (arg3.get('type') != "string"):
        if (arg3.text == None):
            exit(32)
    if (arg1.text == None):
        exit(32)

    try:
        if (arg2.get('type') == 'string'):
            arg2.text = convertString(arg2.text)
        
        if (arg3.get('type') == 'string'):
            arg3.text = convertString(arg3.text)
    except:
        exit(32)

    parent, arg1, arg2, arg3 = sortinstrArgs(parent, arg1, arg2, arg3)

    if (arg1.tag != "arg1" or arg2.tag != "arg2" or arg3.tag != "arg3"):

        exit(32)
    if (arg1.get('type') != "var"):
        exit(32) 
    if (arg2.get('type') != "string" and arg2.get('type') != "bool" and arg2.get('type') != "var" and arg2.get('type') != "int" and arg2.get('type') != "nil"):
        exit(32)
    if (arg3.get('type') != "string" and arg3.get('type') != "bool" and arg3.get('type') != "var" and arg3.get('type') != "int" and arg3.get('type') != "nil"):
        exit(32)
    if (not validVar(arg1.text)):
        exit(32)
    if (not validSymb(arg2.get('type'), arg2.text)):
        exit(32)
    if (not validSymb(arg3.get('type'), arg3.text)):
        exit(32)
    return parent

def checkLabelSymbSymb(parent):
    arg1 = parent[0]
    arg2 = parent[1]
    arg3 = parent[2]

    if (arg1.text == None):
            exit(32)
    if (arg2.get('type') != "string"):
        if (arg2.text == None):
            exit(32)
    if (arg3.get('type') != "string"):
        if (arg3.text == None):
            exit(32)

    try:
        if (arg2.get('type') == 'string'):
            arg2.text = convertString(arg2.text)
        
        if (arg3.get('type') == 'string'):
            arg3.text = convertString(arg3.text)
    except:
        exit(32)

    parent, arg1, arg2, arg3 = sortinstrArgs(parent, arg1, arg2, arg3)

    if (arg1.tag != "arg1" or arg2.tag != "arg2" or arg3.tag != "arg3"):
        exit(32)
    if (arg1.get('type') != "label"):
        exit(32)
    if (arg2.get('type') != "string" and arg2.get('type') != "bool" and arg2.get('type') != "var" and arg2.get('type') != "int" and arg2.get('type') != "nil"):
        exit(32)
    if (arg3.get('type') != "string" and arg3.get('type') != "bool" and arg3.get('type') != "var" and arg3.get('type') != "int" and arg3.get('type') != "nil"):
        exit(32)
    if (not validLabel(arg1.text)):
        exit(32)
    if (not validSymb(arg2.get('type'), arg2.text)):
        exit(32)
    if (not validSymb(arg3.get('type'), arg3.text)):
        exit(32)
    return parent

def checkLabel(parent):
    arg1 = parent[0]
    if (arg1.text == None):
        exit(32)
    if (arg1.tag != "arg1"):
        exit(32)
    if (arg1.get('type') != "label"):
        exit(32)
    if (not validLabel(arg1.text)):
        exit(32)
    return parent

def checkSymb(parent):
    arg1 = parent[0]
    

    if (arg1.get('type') != "string"):
        if (arg1.text == None):
            exit(32)

    try:
        if (arg1.get('type') == 'string'):
            arg1.text = convertString(arg1.text)
    except:
        exit(32)

    if (arg1.tag != "arg1"):
        exit(32)
    if (arg1.get('type') != "string" and arg1.get('type') != "bool" and arg1.get('type') != "var" and arg1.get('type') != "int" and arg1.get('type') != "nil"):
        exit(32)
    if (not validSymb(arg1.get('type'), arg1.text)):
        exit(32)
    return parent

def checkVarType(parent):

    arg1 = parent[0]
    arg2 = parent[1]

    if (arg1.text == None or arg2.text == None):
        exit(32)
    if (arg1.tag == "arg2" and arg2.tag == "arg1"):
        tmp = arg2
        arg2 = arg1
        parent[1] = arg1
        arg1 = tmp
        parent[0] = tmp

    if (arg1.tag != "arg1" or arg2.tag != "arg2"):
        exit(32)
    if (arg1.get('type') != "var" or arg2.get('type') != "type"):
        exit(32)
    if (not validVar(arg1.text)):
        exit(32)
    if (not validType(arg2.text)):
        exit(32)
    return parent

# Funkce pro zkontrolovani spravneho poctu a typu argumentu instrukci
def checkArgs(parent, key):
    # Jestli pocet argumentu v instrukci neodpovida ocekavanym, syntax error
    if (key == 0):
        checkLengths(parent, 0)
    elif (key == 1):
        checkLengths(parent, 1)
        parent = checkVar(parent)
    elif (key == 2):
        checkLengths(parent, 2)
        parent = checkVarSymb(parent)
    elif (key == 3):
        checkLengths(parent, 1)
        parent = checkLabel(parent)
    elif (key == 4):
        checkLengths(parent, 1)
        parent = checkSymb(parent)
    elif (key == 5):
        checkLengths(parent, 3)
        parent = checkVarSymbSymb(parent)
    elif (key == 6):
        checkLengths(parent, 2)
        parent = checkVarType(parent)
    elif (key == 7):
        checkLengths(parent, 3)
        parent = checkLabelSymbSymb(parent)
    else:
        exit(32)
    return parent

# Funkce pro zkontrolovani validity instrukci
def checkInstr(root):
    
    if (root.get('language') != "IPPcode21"):
        exit(32)

    # Povolene instrukce
    # var symb = 2
    # none = 0
    # var = 1
    # label = 3
    # symb = 4
    # var symb symb 5
    # var type 6
    # label symb symb 7

    allowed_inst = {"MOVE":2, "CREATEFRAME":0, "PUSHFRAME":0, "POPFRAME":0, "DEFVAR":1, "CALL":3, "RETURN":0, "PUSHS":4, "POPS":1,
                    "ADD":5, "SUB":5, "MUL":5, "IDIV":5, "LT":5, "GT":5, "EQ":5, "AND":5, "OR":5, "NOT":2, "INT2CHAR":2, "STRI2INT":5, "READ":6,
                    "WRITE":4, "CONCAT":5, "STRLEN":2, "GETCHAR":5, "SETCHAR":5, "TYPE":2, "LABEL":3, "JUMP":3, "JUMPIFEQ":7, "JUMPIFNEQ":7,
                    "EXIT":4, "DPRINT":4, "BREAK":0}
    order = []

    # Projdi vsechny instrukce
    for child in root:
        # Zkontroluj, jestli vubec element je typu instrukce
        if (child.tag != "instruction"):
            exit(32)
        check = child.get('order')
        code = child.get('opcode')
        # Zkontroluj, ze byla instrukce opcode zadana a neni prazdna
        if (code):
            code = code.upper()
        else:
            exit(32)
        # Zkus, jestli lze poradi prekonvertovat na cislo
        try:
            if (re.search("\.", check)):
                exit(32)
            check = int(check)
        except:
            exit(32)
        # Poradi nesmi byt zaporne a 0
        if (check < 1):
            exit(32)
        # Poradi instrukce se nemsi vyskytnout 2x
        if (check in order):
            exit(32)

        # Pridej poradi zpracovane instrukce mezi jiz existujici
        order.append(check)
        # Zkontroluj validitu instrukce
        if (not code in allowed_inst):
            exit(32)
        child = checkArgs(child, allowed_inst.get(code))
    return root

# Funkce pro zkontrolovani, jestli ma instrukce validni poradi a jejich serazeni
def sortInstr(root):

    root = checkInstr(root)
    z = 0
    length = len(root)

    # Projdi array, o kterem uz vis, ze je validni, a serad jej podle poradi
    while (z < length):
        if (z+1 < length):
            check = int(root[z].get('order'))
            peek = int(root[z+1].get('order'))
            if (check > peek):
                tmp = root[z]
                root[z] = root[z+1]
                root[z+1] = tmp
                z = 0
            else:
                z += 1
        else:
            z += 1
    
    return root

# Funkce pro zjisteni, jestli je ramec inicializovany
def frame_initialized(frame):
    global local_frame
    global local_frame_init
    global temp_frame_init
    global temp_frame
    global global_frame

    if (frame is local_frame):
        if (local_frame_init):
            return True
        else:
            return False
    if (frame is temp_frame):
        if (temp_frame_init):
            return True
        else:
            return False
    elif (frame is global_frame):
        return True

# Funkce pro kontrolu, jestli promenna existuje
def varExists(var, frame):
    if not (frame_initialized(frame)):
        exit(55)
    keys = list(frame.keys())
    if (var in keys):
        return True
    else:
        return False

# Funkce pro kontrolu pri definici, jestli uz se promenna v seznamu nenachazi
# Jeslti ne, prida ji
def checkFrameAdd(instr, frame, value=None):
    keys = list(frame.keys())
    if (instr in keys):
        exit(52)
    # Inicializuj hodnotu None
    frame.update({instr:value})

# Funkce pro ziskani framu ze jmena promenne
def getFrame(var):
    return var[:3], var[3:]

# Funkce na pridani promenne do ramce
def addToFrame(instr):
    global global_frame
    global local_frame
    global temp_frame
    global temp_frame_init
    global local_frame_init
    global global_frame_types
    global local_frame_types
    global temp_frame_types

    frame = instr[:3]
    name = instr[3:]
    # Zjisti ramec, podle ktereho spravne prirad promennou
    if (frame == "GF@"):
        checkFrameAdd(name, global_frame)
        checkFrameAdd(name, global_frame_types)
    elif (frame == "LF@"):
        if (local_frame_init):
            checkFrameAdd(name, local_frame)
            checkFrameAdd(name, local_frame_types)
        else:
            exit(55)
    else:
        if (temp_frame_init):
            checkFrameAdd(name, temp_frame)
            checkFrameAdd(name, temp_frame_types)
        # Jestli TF neexistuje, error
        else:
            exit(55)

# Funkce pro zkontrolovani existence promenne v ramci
def varPresent(new_type, instr):
    global global_frame
    global local_frame
    global temp_frames

    # Kontroluj jenom jestli je typ promenne var
    if (new_type == 'var'):
        # Ziskej jeji frame a nazev
        frame, name = getFrame(instr)
        # Zkontroluj na zaklade ziskaneho framu pritomnost v nem
        if (frame == "GF@"):
            if not (varExists(name, global_frame)):
                exit(54)

        elif (frame == "LF@"):
            if not (varExists(name, local_frame)):
                exit(54)

        elif (frame == "TF@"):
            if not (varExists(name, temp_frame)):
                exit(54)

# Funkcce pro aktualizaci hodnoty promenne
def updateVar(var, value, typee):
    global global_frame
    global local_frame
    global temp_frame
    global global_frame_types
    global local_frame_types
    global temp_frame_types

    # Ziskej frame na kontrolu existence a prirazeni
    frame, name = getFrame(var)
    if (frame == "GF@"):
        if not (varExists(name, global_frame)):
            exit(54)
        else:
            # Prirad jak hodnotu, tak typ
            global_frame.update({name:value})
            global_frame_types.update({name:typee})

    elif (frame == "LF@"):
        if not (varExists(name, local_frame)):
            exit(54)
        else:
            local_frame.update({name:value})
            local_frame_types.update({name:typee})

    elif (frame == "TF@"):
        if not (varExists(name, temp_frame)):
            exit(54)
        else:
            temp_frame.update({name:value})
            temp_frame_types.update({name:typee})

# Podpurna funkce pro aritmeticke instrukce slouzici na dalsi kontroly typu
def arithmeticTypes(new_type, symb, new_type_2, symb2):
    varPresent(new_type, symb)
    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "int")

    varPresent(new_type_2, symb2)
    new_type_2 = getVarType(symb2, new_type_2)
    typeCheck(new_type_2, "int")
    
# Konvertovani hodnot typu bool
def convertBool(val1, val2):
    if (val1 == "false"):
        val1 = False
    else:
        val1 = True
    if (val2 == "false"):
        val2 = False
    else:
        val2 = True

    return val1, val2

# Funkce na pomoc s Eq instrukcema
def wrapEq(new_type, new_type_2, symb, symb2):
    result = None
    flag = True
    if (new_type == "nil"):
        flag = False
        if (new_type_2 == "nil"):
            result = True
        else:
            result = False

    if (new_type_2 == 'nil'):
        flag = False
        if (new_type == 'nil'):
            result = True
        else:
            result = False
            
    if (flag):
        if (new_type != new_type_2):
            exit(53)

    val1 = getVarValue(symb)
    val2 = getVarValue(symb2)
    if (new_type != "nil" and new_type_2 != "nil"):
        if (new_type == "int"):
            result = (int(getVarValue(symb)) == int(getVarValue(symb2)))
        elif (new_type == "bool"):
            val1, val2 = convertBool(val1, val2)

            result = val1 == val2
        elif (new_type == "string"):
            result = val1 == val2
        else:
            exit(53)
    return result

# Funkce na overeni danych typu
def typeCheck(new_type, type2):
    if (new_type == None):
        exit(56)
    if (new_type != type2):
        exit(53)

# Zpracovani instrukci
def parseExit(instr):
    new_type = instr.get('type')
    instr = instr.text
    # Zkontroluj, jeslti odpovida typ (a jestlize je to var, jestli promenna existuje)
    varPresent(new_type, instr)
    new_type = getVarType(instr, new_type)

    # Exit musi byt typu int
    typeCheck(new_type, "int")

    instr = getVarValue(instr)
    # Zkontroluj, jeslti to neni float
    if (re.search("\.", instr)):
        exit(57)
    else:
        try:
            ret = int(instr)
        except:
            exit(57)
        
        # Zkontroluj, zda je navratova hodnota validni
        if (ret >= 0 and ret <= 49):
            exit(ret)
        exit(57)

def parseLabel(position, instr):
    global labels

    checkFrameAdd(instr.text, labels, position)

def parseJump(instr):
    global labels

    keys = list(labels.keys())
    label_name = instr.text
    if not (label_name in keys):
        exit(52)
    
    # Vrat hodnotu, na kterou se ma skocit
    value = labels.get(label_name)
    return value

def parseDefvar(instr):

    addToFrame(instr.text)

def parseCreateframe():
    global temp_frame
    global temp_frame_init

    temp_frame = {}
    temp_frame_init = True

def parsePushframe():
    global temp_frame
    global temp_frame_init
    global local_frame_stack
    global local_frame
    global local_frame_init
    global local_frame_types
    global temp_frame_types

    if not (temp_frame_init):
        exit(55)

    local_frame_stack.append(temp_frame)
    local_frame_types_stack.append(temp_frame_types)
    local_frame = temp_frame
    local_frame_types = temp_frame_types
    local_frame_init = True
    temp_frame_init = False
    temp_frame = {}
    temp_frame_types = {}

def parsePopframe():
    global temp_frame
    global temp_frame_init
    global local_frame_stack
    global local_frame
    global local_frame_init
    global local_frame_types_stack
    global local_frame_types
    global temp_frame_types

    if (len(local_frame_stack) == 0):
        exit(55)
    
    # Nahrej do TF posledni ramec ze stacku LF
    temp_frame = local_frame_stack.pop()
    temp_frame_types = local_frame_types_stack.pop()
    temp_frame_init = True
    # Jestlize jich na stacku bylo vic, tak novy LF je posledni ze stacku (nemaz ho!)
    if (len(local_frame_stack) > 0):
        local_frame = local_frame_stack[-1]
        local_frame_types = local_frame_types_stack[-1]
    else:
        local_frame_init = False
        local_frame = {}
        local_frame_types = {}

def parseCall(instr, i):
    global labels
    global call_stack

    keys = list(labels.keys())
    label_name = instr.text
    if not (label_name in keys):
        exit(52)

    value = labels.get(label_name)
    call_stack.append(i)
    return value-1

def parseReturn():
    global call_stack
    if (len(call_stack) == 0):
        exit(56)
    value = call_stack.pop()
    return value

def parseType(var, symb):
    typee = symb.get('type')
    var = var.text
    symb = symb.text

    varPresent(typee, symb)

    value = getVarType(symb, typee)
    if (value == None):
        value = ""

    updateVar(var, value, "string")

def parseWrite(symb):
    typee = symb.get('type')
    symb = symb.text
    if (typee == "var"):
        typee = getVarType(symb, typee)
    value = getVarValue(symb)
    if (typee == None):
        exit(56)
    elif (typee == "int"):
        try:
            value = int(value)
        except:
            exit(53)
    elif (typee == "nil"):
        value = ""

    print(value, end='')

def parseMove(var, symb):
    new_type = symb.get('type')
    var = var.text
    symb = symb.text

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    if (new_type == None):
        exit(56)
    symb = getVarValue(symb)

    updateVar(var, symb, new_type)

def parseAdd(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    arithmeticTypes(new_type, symb, new_type_2, symb2)
    
    result = (int(getVarValue(symb)) + int(getVarValue(symb2)))

    updateVar(var, result, "int")

def parseSub(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    arithmeticTypes(new_type, symb, new_type_2, symb2)
    
    result = (int(getVarValue(symb)) - int(getVarValue(symb2)))

    updateVar(var, result, "int")

def parseMul(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text
    
    arithmeticTypes(new_type, symb, new_type_2, symb2)
    
    result = (int(getVarValue(symb)) * int(getVarValue(symb2)))

    updateVar(var, result, "int")

def parseIdiv(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    arithmeticTypes(new_type, symb, new_type_2, symb2)
    
    val1 = int(getVarValue(symb))
    val2 = int(getVarValue(symb2))
    if (val2 == 0):
        exit(57)
    result = (val1 // val2)

    updateVar(var, result, "int")

def parseLt(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    if (new_type == None):
        exit(56)

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    if (new_type_2 == None):
        exit(56)
        
    result = None
    if (new_type != new_type_2):
        exit(53)

    val1 = getVarValue(symb)
    val2 = getVarValue(symb2)
    if (new_type == "int"):
        result = (int(getVarValue(symb)) < int(getVarValue(symb2)))
    elif (new_type == "bool"):
        val1, val2 = convertBool(val1, val2)
        result = val1 < val2
    elif (new_type != "nil"):
        result = val1 < val2
    else:
        exit(53)

    result = str(result).lower()
    updateVar(var, result, "bool")

def parseGt(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text
    
    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    if (new_type == None):
        exit(56)

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    if (new_type_2 == None):
        exit(56)
        
    result = None
    if (new_type != new_type_2):
        exit(53)
    val1 = getVarValue(symb)
    val2 = getVarValue(symb2)
    if (new_type == "int"):
        result = (int(getVarValue(symb)) > int(getVarValue(symb2)))
    elif (new_type == "bool"):
        val1, val2 = convertBool(val1, val2)
        result = val1 > val2
    elif (new_type == "string"):
        result = val1 > val2
    else:
        exit(53)

    result = str(result).lower()
    updateVar(var, result, "bool")

def parseEq(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    if (new_type == None):
        exit(56)

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    if (new_type_2 == None):
        exit(56)
        
    result = wrapEq(new_type, new_type_2, symb, symb2)

    result = str(result).lower()
    updateVar(var, result, "bool")

def parseAnd(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "bool")

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    typeCheck(new_type_2, "bool")

    val1 = getVarValue(symb)
    val2 = getVarValue(symb2)
    val1, val2 = convertBool(val1, val2)

    result = val1 and val2
    result = str(result).lower()

    updateVar(var, result, "bool")

def parseOr(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "bool")

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    typeCheck(new_type_2, "bool")

    val1 = getVarValue(symb)
    val2 = getVarValue(symb2)
    val1, val2 = convertBool(val1, val2)

    result = val1 or val2
    result = str(result).lower()

    updateVar(var, result, "bool")

def parseNot(var, symb):
    new_type = symb.get('type')
    typee = var.get('type')
    var = var.text
    symb = symb.text
    global global_frame
    global local_frame
    global temp_frame
    global global_frame_types
    global local_frame_types
    global temp_frame_types

    varPresent(new_type, symb)
    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "bool")
    
    val1 = getVarValue(symb)
    if (val1 == "false"):
        val1 = False
    else:
        val1 = True

    result = not val1
    result = str(result).lower()

    updateVar(var, result, "bool")

def parseJumpifeq(label, symb, symb2, i):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    label = label.text
    symb = symb.text
    symb2 = symb2.text
    global labels

    keys = list(labels.keys())
    if not (label in keys):
        exit(52)

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    if (new_type == None):
        exit(56)

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    if (new_type_2 == None):
        exit(56)
        
    result = wrapEq(new_type, new_type_2, symb, symb2)

    if (result):
        # Vrat hodnotu, na kterou se ma skocit
        value = labels.get(label)
        return value-1
    else:
        return i

def parseJumpifneq(label, symb, symb2, i):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    label = label.text
    symb = symb.text
    symb2 = symb2.text
    global labels

    keys = list(labels.keys())
    if not (label in keys):
        exit(52)

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    if (new_type == None):
        exit(56)

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    if (new_type_2 == None):
        exit(56)
        
    result = wrapEq(new_type, new_type_2, symb, symb2)

    if not (result):
        # Vrat hodnotu, na kterou se ma skocit
        value = labels.get(label)
        return value-1
    else:
        return i
    
def parseInt2char(var, symb):
    new_type = symb.get('type')
    var = var.text
    symb = symb.text

    varPresent(new_type, symb)
        
    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "int")

    try:
        val1 = getVarValue(symb)
        if (int(val1) < 0):
            exit(58)
        result = chr(int(val1))
    except:
        exit(58)

    updateVar(var, result, "string")
    
def parseStri2int(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text
    
    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "string")

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    typeCheck(new_type_2, "int")

    val1 = getVarValue(symb)
    val2 = getVarValue(symb2)

    try:
        if (int(val2) < 0):
            exit(58)
    except:
        exit(58)

    try:
        result = ord(val1[int(val2)])
    except:
        exit(58)
    
    updateVar(var, result, "int")

def parseRead(var, typee):
    new_type = typee.get('type')
    var = var.text
    typee = typee.text

    if (new_type != "type"):
        exit(53)

    x = None
    insert_type = None
    if (typee == 'int'):
        try:
            x = int(input())
            insert_type = "int"
        except:
            x = "nil"
            insert_type = "nil"
    elif (typee == 'bool'):
        try:
            x = input()
            insert_type = "bool"
            if (x.lower() != "true"):
                x = "false"
            else:
                x = "true"
        except EOFError:
            x = "nil"
            insert_type = "nil"

    elif (typee == 'string'):
        insert_type = "string"
        try:
            x = input()
            insert_type = "string"
        except EOFError:
            x = "nil"
            insert_type = "nil"

    updateVar(var, x, insert_type)

def parseConcat(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "string")

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    typeCheck(new_type_2, "string")
    
    val1 = getVarValue(symb)
    val2 = getVarValue(symb2)

    result = val1 + val2
    
    updateVar(var, result, "string")

def parseStrlen(var, symb):
    new_type = symb.get('type')
    var = var.text
    symb = symb.text

    varPresent(new_type, symb)
    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "string")

    result = len(getVarValue(symb))

    updateVar(var, result, "int")

def parseGetchar(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "string")

    varPresent(new_type_2, symb2)

    new_type_2 = getVarType(symb2, new_type_2)
    typeCheck(new_type_2, "int")
    
    val1 = getVarValue(symb)
    val2 = getVarValue(symb2)

    try:
        if (int(val2) < 0):
            exit(58)
        result = val1[int(val2)]
    except:
        exit(58)
    
    updateVar(var, result, "string")

def parseSetchar(var, symb, symb2):
    new_type = symb.get('type')
    new_type_2 = symb2.get('type')
    var = var.text
    symb = symb.text
    symb2 = symb2.text

    varPresent('var', var)
    typee = getVarType(var, 'var')

    typeCheck(typee, "string")

    varPresent(new_type, symb)

    new_type = getVarType(symb, new_type)
    typeCheck(new_type, "int")

    varPresent(new_type_2, symb2)
        
    new_type_2 = getVarType(symb2, new_type_2)
    typeCheck(new_type_2, "string")

    val3 = getVarValue(var) # String kde menim
    val1 = getVarValue(symb) # Pozice jakou menim
    val2 = getVarValue(symb2) # Novy char

    try:
        if (int(val1) < 0 or int(val1) >= len(val3)):
            exit(58)
        if (val3 == ""):
            exit(58)
        if (val2 == ""):
            exit(58)
    except:
        exit(58)

    try:
        val2 = val2[0]  # Pokud je tohle delsi > 1, beru prvni
        result = val3[:int(val1)] + val2 + val3[int(val1)+1:]
    except:
        exit(58)
    
    updateVar(var, result, "string")

def parsePushs(symb):
    new_type = symb.get('type')
    symb = symb.text

    global var_stack

    varPresent(new_type, symb)
    new_type = getVarType(symb, new_type)
    if (new_type == None):
        exit(56)

    var_stack.append(getVarValue(symb))
    var_stack_type.append(new_type)

def parsePops(var):
    var = var.text
    global var_stack

    try:
        result = var_stack.pop()
        typee = var_stack_type.pop()
    except:
        exit(56)

    updateVar(var, result, typee)

# Ridici funkce zpracovavajici jednotlive instrukce
def parseInstructions(root):
    # Nejprve je potreba zjistit pozice vsech labelu
    i = 0
    length = len(root)
    while (i < length):
        if (root[i].get('opcode').upper() == 'LABEL'):
            parseLabel(i, root[i][0])
            # Jakmile ho zpracuju, smazu ho, neni ho dal potreba resit
            root.remove(root[i])
            length = len(root)
            i -= 1
        i+=1
    length = len(root)
    i = 0

    while (i < length):
        instr = root[i].get('opcode').upper()
        if (instr == 'EXIT'):
            parseExit(root[i][0])
        if (instr == 'JUMP'):
            i = parseJump(root[i][0])
            i -= 1
        if (instr == 'DEFVAR'):
            parseDefvar(root[i][0])
        if (instr == 'CREATEFRAME'):
            parseCreateframe()
        if (instr == 'PUSHFRAME'):
            parsePushframe()
        if (instr == 'POPFRAME'):
            parsePopframe()
        if (instr == 'CALL'):
            i = parseCall(root[i][0], i)
        if (instr == 'RETURN'):
            i = parseReturn()
        if (instr == 'TYPE'):
            parseType(root[i][0], root[i][1])
        if (instr == 'WRITE'):
            parseWrite(root[i][0])
        if (instr == 'MOVE'):
            parseMove(root[i][0], root[i][1])
        if (instr == 'ADD'):
            parseAdd(root[i][0], root[i][1], root[i][2])
        if (instr == 'SUB'):
            parseSub(root[i][0], root[i][1], root[i][2])
        if (instr == 'MUL'):
            parseMul(root[i][0], root[i][1], root[i][2])
        if (instr == 'IDIV'):
            parseIdiv(root[i][0], root[i][1], root[i][2])
        if (instr == 'LT'):
            parseLt(root[i][0], root[i][1], root[i][2])
        if (instr == 'GT'):
            parseGt(root[i][0], root[i][1], root[i][2])
        if (instr == 'EQ'):
            parseEq(root[i][0], root[i][1], root[i][2])
        if (instr == 'AND'):
            parseAnd(root[i][0], root[i][1], root[i][2])
        if (instr == 'OR'):
            parseOr(root[i][0], root[i][1], root[i][2])
        if (instr == 'NOT'):
            parseNot(root[i][0], root[i][1])
        if (instr == 'JUMPIFEQ'):
            i = parseJumpifeq(root[i][0], root[i][1], root[i][2], i)
        if (instr == 'JUMPIFNEQ'):
            i = parseJumpifneq(root[i][0], root[i][1], root[i][2], i)
        if (instr == 'INT2CHAR'):
            parseInt2char(root[i][0], root[i][1])
        if (instr == 'STRI2INT'):
            parseStri2int(root[i][0], root[i][1], root[i][2])
        if (instr == 'READ'):
            parseRead(root[i][0], root[i][1])
        if (instr == 'CONCAT'):
            parseConcat(root[i][0], root[i][1], root[i][2])
        if (instr == 'STRLEN'):
            parseStrlen(root[i][0], root[i][1])
        if (instr == 'GETCHAR'):
            parseGetchar(root[i][0], root[i][1], root[i][2])
        if (instr == 'SETCHAR'):
            parseSetchar(root[i][0], root[i][1], root[i][2])
        if (instr == 'PUSHS'):
            parsePushs(root[i][0])
        if (instr == 'POPS'):
            parsePops(root[i][0])
        i += 1

def main():
    source_path, input_path = parse_args()
    source = loadContent(source_path, input_path)
    root = source.getroot()
    root = sortInstr(root)
    parseInstructions(root)

if __name__ == "__main__":
    try:
        main()
    except (MemoryError, BufferError):
        exit(99)
