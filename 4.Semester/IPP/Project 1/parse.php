<?php
ini_set('display_errors ', 'stderr');

// initialize xml 
$xw = xmlwriter_open_memory();
xmlwriter_set_indent($xw, 1);
$res = xmlwriter_set_indent_string($xw, ' ');
xmlwriter_start_document($xw, '1.0', 'UTF-8');
xmlwriter_start_element($xw, 'program');
xmlwriter_start_attribute($xw, 'language');
$inst_count = 1;

function print_help() {
    printf("\n");
    printf("Tento program slouzi k prekladu vnitrniho kodu IPP21 na format XML.\n");
    printf("Zdroj je cten ze standardniho vstupu, vysledek je psan na standardni vystup.\n");
    printf("Argumenty:\n");
    printf("     --help    -> Vypise tuto napovedu\n");
    printf("\n");
    exit(0);
}

// parse arguments
if ($argc == 2) {
    if ($argv[1] == '--help') {
        print_help();
    }
    else {
        fprintf(STDERR, "Neplatny argument! Jediny povoleny argument je --help!\n");
        exit(10);
    }
}

// Remove comments
function skip_comments($line) {
    $len = strlen($line);
    $new = '';
    $occurence = False;
    for($i = 0; $i < $len; $i++) {
        // If the line is not empty, remember it
        if (ord($line[$i]) > 32 && ord($line[$i]) != 35) {
            $occurence = True;
        }
        if ($line[$i] == '#') {
            // If you reach comment and the line is otherwise empty, return empty line
            if ($occurence == False) {
                $new = "";
                return $new;
            }
            // Else return an empty line with a newline char
            else {
                $new = $new . "\n";
                break;
            }
        }

        $new = $new . $line[$i];
    }
    
    return $new;
}

// Get operand type
function get_type($word) {
    // Vars can only contain these symbols and must not start with numbers
    if (preg_match("/^\s*(LF|GF|TF)@[A-Za-z_\-$&%*!?][A-Za-z0-9_\-$&%*!?]*\s*$/", $word))
        return 'var';
    // Integer can only consist of numbers and their parity sign
    if (preg_match("/^\s*int@(\-|\+)?[0-9]+$/", $word))
        return 'int';
    // String can consist of anything, but if there is a backslash, 3 numbers have to follow as an ASCII escape sequence
    if (preg_match("/^\s*string@.*$/", $word)) {
        $len = strlen($word);
        for ($i = 0; $i < $len; $i++) {
            // Check if the string contains any backslashes. 
            // If it does, check if there are 3 numbers following 
            if ($word[$i] == '\\') {
                if ($i + 3 <= $len-1) {
                    if (ord($word[$i+1]) < 48 || ord($word[$i+1]) > 57)
                        exit(23);
                    if (ord($word[$i+2]) < 48 || ord($word[$i+2]) > 57)
                        exit(23);
                    if (ord($word[$i+3]) < 48 || ord($word[$i+3]) > 57)
                        exit(23);
                }
                else {
                    exit(23);
                }
            }
        }
        return 'string';
    }
    // Bool can only be true or false
    if (preg_match("/^\s*bool@(true|false)$/", $word))
        return 'bool';
    if (preg_match("/^\s*nil@nil$/", $word))
        return 'nil';
    // Label can be anything but must not start with a number
    if (preg_match("/^\s*[A-Za-z_\-$&%*!?][A-Za-z0-9_\-$&%*!?]*\s*$/", $word))
        return 'label';
    return 0;
}

// An arhcetype of function with no operands
function archetype_1($instr_up) {
    global $xw;
    global $inst_count;
    xmlwriter_start_element($xw, 'instruction');
    xmlwriter_write_attribute($xw, 'order', $inst_count);
    // Increment global instruction count
    $inst_count++;
    xmlwriter_write_attribute($xw, 'opcode', $instr_up);
    xmlwriter_end_element($xw);
    return;
}

// Function to print the correct type
function correct_printing($type, $value, $arg) {
    global $xw;
    xmlwriter_start_element($xw, $arg);
    xmlwriter_write_attribute($xw, 'type', $type);
    xmlwriter_text($xw, $value);
}

// Archetype of function with 1 operand
function archetype_2($instr_up, $value) {
    global $xw;
    global $inst_count;
    $type = get_type($value);
    
    // Check operand type
    if ($type != "var") {
        // If its not a var, separate it to get its value
        $value = explode("@", $value);
        $size = count($value);
    }

    if ($type) {
        // Check syntax
        if ($instr_up == "CALL" || $instr_up == "LABEL" || $instr_up == "JUMP") {
            if ($type != 'label')
                exit(23);
        }

        if ($instr_up == "DEFVAR" || $instr_up == "POPS") {
            if ($type != 'var')
                exit(23);
        }
        if ($instr_up == "PUSHS" || $instr_up == "WRITE" || $instr_up == "DPRINT") {
            if ($type == 'label')
                exit(23);
        }
        if ($instr_up == "EXIT") {
            if ($type == 'label') 
                exit(23);
        }

        xmlwriter_start_element($xw, 'instruction');
        xmlwriter_write_attribute($xw, 'order', $inst_count);
        $inst_count++;
        xmlwriter_write_attribute($xw, 'opcode', $instr_up);
        
        if ($type != "var") {
            if ($size == 2) {
                correct_printing($type, $value[1], "arg1");
            }
            if ($size < 2) {
                correct_printing($type, $value[0], "arg1");
            }
        }
        else {
            correct_printing($type, $value, "arg1");
        }

        xmlwriter_end_element($xw);
        xmlwriter_end_element($xw);
    }
    // If the operand type is invalid (lexical error), return 23
    else {
        exit(23);
    }
    return;
}

// Archetype of function with 2 operands, but also checks the 2nd operand
function archetype_3($instr_up, $value, $value2) {
    global $xw;
    global $inst_count;
    $type = get_type($value);
    $type2 = get_type($value2);


    if ($type && $type2) {

        if ($instr_up == "INT2CHAR") {
            if ($type != 'var' || $type2 == 'label')
                exit(23);
        }
    
        if ($instr_up == "STRLEN") {
            if ($type != 'var' || $type2 == 'label')
                exit(23);
        }
    
        if ($instr_up == "TYPE" || $instr_up == "MOVE") {
            if ($type != "var" || $type2 == 'label')
                exit(23);
        }

        if ($instr_up == "NOT") {
            if ($type != "var" || $type2 == 'label')
                exit(23);
        }
    
        if ($type != "var") {
            $value = explode("@", $value);
            $size = count($value);
        }
    
        if ($type2 != "var") {
            $value2 = explode("@", $value2);
            $size2 = count($value2);
        }

        xmlwriter_start_element($xw, 'instruction');
        xmlwriter_write_attribute($xw, 'order', $inst_count);
        $inst_count++;
        xmlwriter_write_attribute($xw, 'opcode', $instr_up);

        if ($type != "var") {
            if ($size == 2) {
                correct_printing($type, $value[1], "arg1");
            }
            if ($size < 2) {
                correct_printing($type, $value[0],  "arg1");
            }
        }
        else {
            correct_printing($type, $value, "arg1");
        }
        xmlwriter_end_element($xw);

        if ($type2 != "var") {
            if ($size2 == 2) {
                correct_printing($type2, $value2[1], "arg2");
            }
            if ($size2 < 2) {
                correct_printing($type2, $value[0], "arg2");
            }
        }
        else {
            correct_printing($type2, $value, "arg2");
        }
        xmlwriter_end_element($xw);

        xmlwriter_end_element($xw);
    }
    else {
        exit(23);
    }
    return;
}

//Archetype of function with 3 operands
function archetype_4($instr_up, $value, $value2, $value3) {
    global $xw;
    global $inst_count;
    $type = get_type($value);
    $type2 = get_type($value2);
    $type3 = get_type($value3);

    if ($type != "var") {
        $value = explode("@", $value);
        $size = count($value);
    }

    if ($type2 != "var") {
        $value2 = explode("@", $value2);
        $size2 = count($value2);
    }
    
    if ($type3 != "var") {
        $value3 = explode("@", $value3);
        $size3 = count($value3);
    }

    if ($type && $type2 && $type3) {

        if ($instr_up == "ADD" || $instr_up == "SUB" || $instr_up == "MUL" || $instr_up == "IDIV") {
            if ($type != "var" || $type2 == 'label' || $type3 == 'label')
                exit(23);
        }
        if ($instr_up == "LT" || $instr_up == "GT" || $instr_up == "EQ") {
            if ($type != "var" || $type2 == 'label' || $type3 == 'label')
                exit(23);
        }

        if ($instr_up == "AND" || $instr_up == "OR") {
            if ($type != "var" || $type2 == 'label' || $type3 == 'label')
                exit(23);
        }

        if ($instr_up == "STRI2INT" || $instr_up == "GETCHAR") {
            if ($type != "var" || ($type2 == 'label') || $type3 == 'label')
                exit(23);
        }

        if ($instr_up == "CONCAT") {
            if ($type != "var" || ($type2 == 'label') || $type3 == 'label')
                exit(23);
        }

        if ($instr_up == "SETCHAR") {
            if (($type != "var") || ($type2 == 'label') || $type3 == 'label')
                exit(23);
        }

        if ($instr_up == "JUMPIFEQ" || $instr_up == "JUMPIFNEQ") {
            if ($type != "label" || $type2 == 'label' || $type3 == 'label')
                exit(23);
        }
        xmlwriter_start_element($xw, 'instruction');
        xmlwriter_write_attribute($xw, 'order', $inst_count);
        $inst_count++;
        xmlwriter_write_attribute($xw, 'opcode', $instr_up);

        if ($type != "var") {
            if ($size == 2) {
                correct_printing($type, $value[1], "arg1");
            }
            if ($size < 2) {
                correct_printing($type, $value[0], "arg1");
            }
        }
        else {
            correct_printing($type, $value, "arg1");
        }
        xmlwriter_end_element($xw);

        if ($type2 != "var") {
            if ($size2 == 2) {
                correct_printing($type2, $value2[1], "arg2");
            }
            if ($size2 < 2) {
                correct_printing($type2, $value2[0], "arg2");
            }
        }
        else {
            correct_printing($type2, $value2, "arg2");
        }
        xmlwriter_end_element($xw);
        
        if ($type3 != "var") {
            if ($size3 == 2) {
                correct_printing($type3, $value3[1], "arg3");
            }
            if ($size3 < 2) {
                correct_printing($type3, $value3[0], "arg3");
            }
        }
        else {
            correct_printing($type3, $value3, "arg3");
        }
        xmlwriter_end_element($xw);

        xmlwriter_end_element($xw);
        return;
    }
    else {
        exit(23);
    }
}

// Special function to work with the READ instruction
function get_read_type($word) {
    if (preg_match("/^\s*int*$/", $word))
        return 'int';
    if (preg_match("/^\s*string$/", $word))
        return 'string';
    if (preg_match("/^\s*bool$/", $word))
        return 'bool';
    return 0;
}

// Function to check the instruction on each line and generate the correct xml output
function parse_input($line) {
    global $xw;
    global $inst_count;
    // Split each line by a whitechar
    $line = preg_split("/\s+/", $line);

    $size = count($line);
    // If the last character of the line is a NULL char or a newline, remove it
    if (ord($line[$size-1]) == 0 || ord($line[$size-1]) == 10) {
        array_pop($line);
        $size = count($line);
    }
    // Check if the line is of a valid size
    if ($size > 0) {
        // Convert it to lowercase and back to fix potential mistakes
        $line_lower = strtolower($line[0]);
        $instr_up = strtoupper($line_lower);

        // Check the instruction and its number of operands and print according to that
        switch ($line_lower) {
            case "defvar":
            case "label":
            case "call":
            case "pushs":
            case "pops":
            case "write":
            case "jump":
            case "exit":
            case "dprint":
                // check number of operands
                if ($size != 2)
                    exit(23);
                archetype_2($instr_up, $line[1]);
                break;

            case "move":
            case "not":
            case "int2char":
            case "type":
            case "strlen":
                if ($size != 3) {
                    exit(23);
                }
                archetype_3($instr_up, $line[1], $line[2]);
                break;

            case "createframe": 
            case "pushframe":
            case "popframe":
            case "return":
            case "break":
                if ($size != 1) {
                    exit(23);
                }
                archetype_1($instr_up);
                break;
        
            case "add":
            case "sub":
            case "mul":
            case "idiv":
            case "lt":
            case "gt":
            case "eq":
            case "and":
            case "or":
            case "stri2int":
            case "concat":
            case "getchar":
            case "setchar":
            case "jumpifeq":
            case "jumpifneq":
                if ($size != 4) {
                    exit(23);
                }
                archetype_4($instr_up, $line[1], $line[2], $line[3]);
                break;
            
            // Read is special
            case "read":
                if ($size != 3) {
                    exit(23);
                }
                // Get special types used in read
                $type = get_type($line[1]);
                if ($type != 'var') {
                    exit(23);
                }
                if ($type) {
                    xmlwriter_start_element($xw, 'instruction');
                    xmlwriter_write_attribute($xw, 'order', $inst_count);
                    $inst_count++;
                    xmlwriter_write_attribute($xw, 'opcode', $instr_up);
    
                    xmlwriter_start_element($xw, 'arg1');
                    xmlwriter_write_attribute($xw, 'type', $type);
                    xmlwriter_text($xw, $line[1]);
                    xmlwriter_end_element($xw);
                    $res = get_read_type($line[2]);
                    if ($res) {
                        xmlwriter_start_element($xw, 'arg2');
                        xmlwriter_write_attribute($xw, 'type', 'type');
                        xmlwriter_text($xw, $line[2]);
                        xmlwriter_end_element($xw);
                        xmlwriter_end_element($xw);
                    }
                    else {
                        exit(23);
                    }
                }
        
                else {
                    exit(23);
                }
                break;
            // If the instruction was not matched, exit
            default:
                exit(22);
        }
    }
    return;
}

// Check if the line is empty
function check_empty($line) {
    // Remove comments from it
    $line = skip_comments($line);
    if (preg_match("/[A-Za-z0-9_\-$&%*!?]*/", $line)) {
        // If you found some white characters and nothing else, it's empty
        if ((strlen($line) == 0) || ($line=="\n"))
            return True;
        return False;
    }
    return True;
}

function read_input() {
    global $xw;
    // The code must begin with a header
    $header = False;
    // While stdin is not empty
    while ($line = fgets(STDIN)) {
        // Check if the preogram has a header
        if ($header == False) {
            // If the line is not empty and is in the beginning, it must be the header
            if (check_empty($line) == False) {
                $test = strtolower($line);
                $test = skip_comments($test);
                if (preg_match("/^\s*\.ippcode21\s*$/", $test)) {
                    xmlwriter_text($xw, "IPPcode21");
                    $header = True;
                }
                else {
                    // If there is no header, exit
                    exit(21);
                }
            }
        }
        else {
            // Parse instructions
            $line = skip_comments($line);
            // The header must not repeat
            if (preg_match("/^\s*\.ippcode21\s*$/", $line))
                exit(22);
            // If the line is empty, I dont care
            if (preg_match("/^\s*$/", $line))
                continue;
            parse_input($line);
        }
    }
}

// Main function
function main_fc() {
    read_input();
}

// Call main
main_fc();
// End XML output
xmlwriter_end_attribute($xw);
xmlwriter_end_element($xw);
xmlwriter_end_document($xw);
echo xmlwriter_output_memory($xw);
// exit with a success
exit(0);

?>