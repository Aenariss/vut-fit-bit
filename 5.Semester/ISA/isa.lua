-- https://mika-s.github.io/wireshark/lua/dissector/2018/12/30/creating-port-independent-wireshark-dissectors-in-lua.html
-- unfinished capture wireshark error == spatny string.find, neescaptnuty charakter (escape zapomoci %)

-- Tvorba Proto objektu pro protokol
isa_protocol = Proto("ISA", "ISA Project Protocol")

-- Jednotlive prvky protokolu, ktere se budou zobrazovat v ramci dissectu jednotlivych paketu
status = ProtoField.string("isa_protocol.status", "Response status")
command = ProtoField.string("isa_protocol.command", "Operation code")
response = ProtoField.string("isa_protocol.message", "Message")
body = ProtoField.string("isa_protocol.body", "Body")
type = ProtoField.string("isa_protocol.type", "Type")
msg_length = ProtoField.string("isa_protocol.msg_length", "Length")
username = ProtoField.string("isa_protocol.username", "Username")
password = ProtoField.string("isa_protocol.password", "Obfuscated password")
user_id = ProtoField.string("isa_protocol.user_id", "User Token")
sender = ProtoField.string("isa_protocol.sender", "Message sender")
recipient = ProtoField.string("isa_protocol.recipient", "Message recipient")
subject = ProtoField.string("isa_protocol.subject", "Message subject")
body_message = ProtoField.string("isa_protocol.body", "Message body")
message_number = ProtoField.string("isa_protocol.body", "Message ID")
request_opcode = ProtoField.string("isa_protocol.body", "Response to Opcode")

isa_protocol.fields = { status, command, response, body, type, msg_length, username, password, user_id, sender, recipient, subject, body_message, message_number, request_opcode }

prev_cmd = "none"

-- Funkce na zjisteni typu zpravy (Response -> Ok/Error; Request -> Command; Nebo taky unknown - pro vse ostatni)
local function get_status(msg)
    local message_name = "unknown"
    local find = tostring(msg)          -- Konverze bytu na string
    local find = Struct.fromhex(find)   -- Konverze stringu, ktery je ale stale tvoren byty, na pro cloveka prijemny tvar (klasicke pismena)

    if string.find(find, "%(ok") then message_name = "ok"
    elseif string.find(find, "%(err") then message_name = "error" 
    elseif string.find(find, "%(register") then message_name = "reg"
    elseif string.find(find, "%(login") then message_name = "login"
    elseif string.find(find, "%(fetch") then message_name = "fetch"
    elseif string.find(find, "%(logout") then message_name = "logout"
    elseif string.find(find, "%(send") then message_name = "send"
    elseif string.find(find, "%(list") then message_name = "list"
    end

    if (prev_cmd == "none") then prev_cmd = message_name end

    return message_name
end

-- Funkce slouzici pro vyhledavani substringu uvnitr tela zpravy (Zpravidla uvnitr uvozovek)
-- Vyhledava take telo zpravy uvnitr dvou protichudnych zavorek ()
local function get_char_index(buffer, n, needle)
    
    local buff_len = buffer:len()
    local first_quote = -1
    local second_quote = -1
    local counter = 0
    local my_string
    local my_string_prev
    local limit
    local block_size = 36   -- velikost bloku, na ktery delim buffer -- Bez rozdeleni nelze cist vic nez 36 znaku
    if n > 1 then 
        limit = n + n-1 
    else 
        limit = n
    end
    local i = 1
    local k = 0
    local flag = 0
    local begin_flag = 0
    local remains_to_read = buff_len
    while k <= buff_len do  -- Ctu, dokud neprectu cely buffer
        -- Prvotni urceni cteciho bloku
        if remains_to_read >= block_size and flag == 0 then -- Pokud zbyva precist  vic nez je velikost bloku, zacinam ctenim prvnich 36 znaku
            my_string = tostring(buffer(k, block_size))
            my_string = Struct.fromhex(my_string)
            my_string_prev = my_string
            flag = 1
            begin_flag = 1
        elseif flag == 0 then                               -- Jinak nactu rovnou cely buffer
            my_string = tostring(buffer)
            my_string = Struct.fromhex(my_string)
            my_string_prev = my_string
            flag = 1
            begin_flag = 1
        end

        -- Urcovani aktualniho cteciho bloku
        local mod = k % block_size                          -- Podle aktualni pozice v bufferu urcuju, kdy ccist dalsi blok
        if mod == 0 and k ~= 0 then
            remains_to_read = remains_to_read - block_size  -- Od poctu neprectenych bytu odectu velikost bloku
            if remains_to_read >= block_size then           -- Jestlize stale zbyva precist vic nez/a nebo velikost bloku, nactu dalsich 36 znaku
                local tmp_buf = buffer(k, block_size)
                my_string_prev = my_string
                my_string = tostring(tmp_buf)
                my_string = Struct.fromhex(my_string)
            elseif remains_to_read > 0 then                 -- Jinak nactu cely zbytek bufferu
                local tmp_buf = buffer(k, buff_len-k)
                my_string_prev = my_string
                my_string = tostring(tmp_buf)
                my_string = Struct.fromhex(my_string)
            else
                break
            end
        end

        local char = my_string:sub(i,i)                     -- Charakter na prave ctene pozici v bufferu
        -- Urceni zpracovani zavorek
        if needle == '(' then           
            if char == ')' then 
                if first_quote ~= -1 then                   -- Jestlize uz byla nactena oteviraci ( zavorka, musim zkontrolovat, jestli muzu blok zavrit dalsi
                    if i > 2 then                           -- Za pomoci predchazejicih 2 znaku urcim, jestli se jedna o konec "bloku" a nebo o zavorku v textu
                        local prev_char = my_string:sub(i-1,i-1)
                        local prev_prev_char = my_string:sub(i-2,i-2)
                        if prev_char == '\"' then           -- Jestlize je predchozi charakter ", tzn. mam "), vypada to nadejne
                            if prev_prev_char ~= "\\" then  -- Jestlize uvozovka neni escapnuta, tzn. NEmam \"), je zavorka ukoncujici a muzu prestat hledat
                                second_quote = k+1
                                break
                            end
                        end
                    elseif i > 1 then  -- Muzu byt ale i na pomezi
                        local prev_char = my_string:sub(i-1,i-1)
                        local prev_prev_char = my_string_prev:sub(block_size,block_size)
                        if prev_char == '\"' then
                            if prev_prev_char ~= "\\" then
                                second_quote = k+1
                                break
                            end
                        end
                        break
                    else -- Jestlize jsem na zacatku noveho cteciho bloku, musim zkontrolovat blok predchozi
                        local prev_char = my_string_prev:sub(block_size,block_size)
                        local prev_prev_char = my_string_prev:sub(block_size-1,block_size-1)
                        if prev_char == '\"' then
                            if prev_prev_char ~= "\\" then
                                second_quote = k+1
                                break
                            end
                        end
                        break
                    end
                end
            elseif char == '(' then                             -- Jestlize jsem objevil oteviraci zavorku, nemusim ji nijak zvlast resit
                counter = counter + 1
                if counter == n then
                    first_quote = k+1
                end
            end
        -- Zpracovani jinaciho hledaneho znaku (V mem pripade tady pujde jenom uvozovky - ") na stejnem principu jako vyse
        else 
            if char == needle then
                if i > 1 then
                    local tmp_char = my_string:sub(i-1,i-1) -- U uvozovek staci kontrolovat pouze predchozi znak
                    if tmp_char ~= '\\' then                -- Ten nesmi byt backslash -> Uvozovka neni escapnuta
                        counter = counter + 1
                        if first_quote ~= -1 then
                            second_quote = k+1
                            break
                        end
                        if counter == limit then
                            first_quote = k+1
                        end
                    end
                else
                    if begin_flag == 1 then
                        counter = counter + 1
                        if counter == limit then
                            first_quote = k+1
                        end
                    else
                        local tmp_char = my_string_prev:sub(block_size,block_size)  -- Jestlize byla uvozovka na zacatku noveho bloku, musim kontrolovat predchozi
                        if tmp_char ~= '\\' then
                            counter = counter + 1
                            if first_quote ~= -1 then
                                second_quote = k+1
                                break
                            end
                            if counter == limit then
                                first_quote = k+1
                            end
                        end
                    end
                end
            end
        end
        begin_flag = 0
        if (i == block_size) then i = 0 end -- Jestlize index v aktualnim bloku dosahl velikosti bloku, je potreba ho vynulovat
        i = i + 1                           -- A hned ho zase inkrementovat, protoze stringy v LUA zacinaji z nejakeho duvodu na 1
        k = k + 1                           -- Inkrementuje se i aktualni cteci index v celem bufferu
    end
    if first_quote == -1 then first_quote = 0 end   -- Jestlize jsem znak nenasel, dam zacatek na pozici 0, at nedochazi k prilisnym problemuim
    if second_quote == -1 then second_quote = buff_len end  -- Jestlize nebyl nalezen ukoncovaci znak, vezmu cely zbytek zpravy
    return first_quote, second_quote
end

-- Funkce na nalezeni casti stringu obsahujici cislo ktere muze byt i float
local function get_number_or_float(buffer)
    local buff_len = buffer:len()
    local my_string = tostring(buffer(0, buff_len))
    local my_string = Struct.fromhex(my_string)
    local flag = 0
    local n_begin = -1
    local n_end = -1
    for i=1,buff_len do                                     -- Iteruju skrz cely buffer
        local char = my_string:sub(i,i)
        local byte_compare = string.byte(char)              -- Zkonvertuju si nacteny charakter na ascii hodnotu
        if ((byte_compare >= 48 and byte_compare <= 57) or char == '.' or char == '-') then -- Jestlize ascii hodnota odpovida cislu (a nebo - a .), asi to bude cislo
            if (n_begin ~= -1) then n_end = i end           -- Jestlize uz jsem predtim nejake cislo nasel, muzu posunout ukoncovaci pozici
            if (flag == 0) then n_begin = i flag = 1 end    -- Prvni nalezeno cislo = zacatek cele ciselne casti
        end
    end
    if n_end == -1 then n_end = n_begin end                 -- Jestlize jsem nenasel konec, znamena to nejspis, ze bylo cislo jen 1 a proto nastavim konec na zacatek
    return n_begin, n_end

end

-- Funkce na nalezeni casti stringu obsahujici cislo ktere muze byt i float
local function get_number(buffer)
    local buff_len = buffer:len()
    local my_string = tostring(buffer(0, buff_len))
    local my_string = Struct.fromhex(my_string)
    local flag = 0
    local n_begin = -1
    local n_end = -1
    for i=1,buff_len do                                     -- Iteruju skrz cely buffer
        local char = my_string:sub(i,i)
        local byte_compare = string.byte(char)              -- Zkonvertuju si nacteny charakter na ascii hodnotu
        if (byte_compare >= 48 and byte_compare <= 57) then -- Jestlize ascii hodnota odpovida cislu (a nebo - a .), asi to bude cislo
            if (n_begin ~= -1) then n_end = i end           -- Jestlize uz jsem predtim nejake cislo nasel, muzu posunout ukoncovaci pozici
            if (flag == 0) then n_begin = i flag = 1 end    -- Prvni nalezeno cislo = zacatek cele ciselne casti
        end
    end
    if n_end == -1 then n_end = n_begin end                 -- Jestlize jsem nenasel konec, znamena to nejspis, ze bylo cislo jen 1 a proto nastavim konec na zacatek
    return n_begin, n_end

end


-- Funkce na pridani casti mezi uvozovkami do stromove struktury, ktera se vypisuje ve wiresharku
function add_quote_to_tree(subtree, buffer, max_length, n, field)
    local first_quote, second_quote = get_char_index(buffer(0,max_length-1), n, '\"')
    subtree:add(field, buffer(first_quote, second_quote-first_quote-1))
    return first_quote, second_quote
end

-- Funkce pro pridani cisel do stromove struktury
function add_number_to_tree(subtree, buffer, max_length, field, prev_quote)
    local begin_n, end_n = get_number_or_float(buffer(prev_quote, max_length-prev_quote))
    subtree:add(field, buffer(prev_quote+begin_n-1, end_n-begin_n+1))
end

-- Funkce pro zpracovani error messages ve wiresharku
function command_errors(my_string, max_length, subtree, buffer, syntax, cmd)
    if (string.find(my_string, syntax)) then
        subtree:add(request_opcode):append_text(cmd)
        add_quote_to_tree(subtree, buffer, max_length, 1, response)
    end
end

-- Funkce pro zpracovavani jednotlivych odpovedi
function dissect(max_length, buffer, pinfo, tree)

    pinfo.cols.protocol = isa_protocol.name     -- Nastaveni kolonky "Protocol" ve wiresharku na ISA
    local subtree = tree:add(isa_protocol, buffer(), "ISA Protocol Payload")    -- Nastaveni nazvu substromu, ve kterem se zobrazuji udaje o ISA paketu

    local msg_type
    if max_length < 10 then msg_type = get_status(buffer(0, max_length))        -- Jestlize je zprava kratsi nez 10 znaku (nejspis nemuze nastat), nactu status jenom z teto casti
    else msg_type = get_status(buffer(0,10)) end                                -- Jinak status nacitam z prvnich 10 znaku

    if (msg_type == "ok" or msg_type == "error") then                           -- Jestlize je zprava typu ok a nebo error, jedna se o Response ze strany serveru

        subtree:add(type):append_text("Response")

        local response_body = buffer(0, max_length-1)
        local my_string = tostring(response_body)
        local my_string = Struct.fromhex(my_string)

        -- Vypisy serverovych odpovedi --
        if (msg_type == "ok") then 
            subtree:add(status, buffer(1,3))

            -- ok (() znaci zpravu typu list, ktera ale byla prazdna -> Doted uzivateli nikdo nenapsal :(
            if (string.find(my_string, "%(ok %(%)")) then 
                subtree:add(request_opcode):append_text("list")
                subtree:add(body, buffer(0, max_length))
            
            -- Zpracovani list responsu
            elseif(string.find(my_string, "ok %(%(")) then -- ok (( znaci zacatek listu
                subtree:add(request_opcode):append_text("list")
                local endpoint = max_length-3+1
                local initial_offset = 5   -- kvuli tomu, at nemam problemy s dvojtyma zavorkama
                local i = initial_offset
                local k = 1
                -- while loop pro vypisovani jednotlivych zprav z list commandu
                while i < endpoint do   -- Dokud jsem neprecetl cely buffer, tak pracuju
                    local left_bracket, right_bracket = get_char_index(buffer(initial_offset, max_length-7), k, "(")
                    -- local response_body = buffer(left_bracket+initial_offset-2, right_bracket-left_bracket) -- Pro ucely pripadneho debugovani to tady radsi necham
                    if (right_bracket == max_length) then return end
                    local first_quote, second_quote = get_char_index(buffer(left_bracket+initial_offset-1, right_bracket-left_bracket), 1, "\"")            -- sender
                    if (second_quote == max_length) then return end
                    local first_quote_msg, second_quote_msg = get_char_index(buffer(left_bracket+initial_offset-1, right_bracket-left_bracket), 2, "\"")    -- subject
                    if (second_quote_msg== max_length) then return end
                    local beg, endd = get_number(buffer(left_bracket+initial_offset, first_quote))                                                          -- message ID
                    if (beg == -1 and endd == -1) then              -- Jestlize doslo k chybe pri hledani message ID, posunu se rovnou na dalsi zpravu (Nemelo by nastat)
                        i = right_bracket + 2 + initial_offset
                        k = k + 1
                    else                                            -- Jinak muzu do wiresharku pridat nactenou zpravu
                        subtree:add(message_number, buffer(beg+left_bracket+initial_offset-1, endd))
                        subtree:add(sender, buffer(first_quote+left_bracket+initial_offset-1, second_quote-first_quote-1))
                        subtree:add(subject, buffer(first_quote_msg+left_bracket+initial_offset-1, second_quote_msg-first_quote_msg-1))
                        k = k + 1
                        -- Posunuti aktualni pozice o nactenou oteviraci zavorku (, initial offset a dodatek
                        -- +2 kvuli tomu, ze je na zacatku odecitam od max_length (kvuli funkcionalite), initial_offset self-explanatory
                        i = right_bracket + 2 + initial_offset 
                    end
                    if (right_bracket == 0) then break end -- proti zacykleni -> Zavorka nebyla nalezena, uz jsem na konci
                end
            
            -- Zpracovani fetch responsu
            elseif(string.find(my_string, "%(ok %(")) then -- (ok (" znaci fetch zpravu
                local initial_offset = 1
                subtree:add(request_opcode):append_text("fetch")
                local left_bracket, right_bracket = get_char_index(buffer(1, max_length-1), 1, "(")
                if (right_bracket == max_length) then return end
                local first_quote, second_quote = get_char_index(buffer(left_bracket+initial_offset, right_bracket-left_bracket-1), 1, "\"")            -- sender
                subtree:add(sender, buffer(first_quote+left_bracket+initial_offset, second_quote-first_quote-1))
                if (second_quote == max_length) then return end
                local first_quote_msg, second_quote_msg = get_char_index(buffer(left_bracket+initial_offset, right_bracket-left_bracket-1), 2, "\"")    -- subject
                if (second_quote_msg == max_length) then return end
                local first_quote, second_quote = get_char_index(buffer(left_bracket+initial_offset, right_bracket-left_bracket-1), 3, "\"")            -- body
                subtree:add(subject, buffer(first_quote_msg+left_bracket+initial_offset, second_quote_msg-first_quote_msg-1))
                if (second_quote == max_length) then return end
                subtree:add(body_message, buffer(first_quote+left_bracket+initial_offset, second_quote-first_quote-1))

            -- Logout response
            elseif(string.find(my_string, "logged out")) then 
                subtree:add(request_opcode):append_text("logout")
                add_quote_to_tree(subtree, buffer, max_length, 1, response)

            -- Login response
            elseif(string.find(my_string, "user logged in")) then 
                subtree:add(request_opcode):append_text("login")
                add_quote_to_tree(subtree, buffer, max_length, 1, response) -- response message
                add_quote_to_tree(subtree, buffer, max_length, 2, user_id)
            
            -- Register response
            elseif(string.find(my_string, "registered user ")) then
                subtree:add(request_opcode):append_text("register")
                add_quote_to_tree(subtree, buffer, max_length, 1, response)

            -- Send response
            elseif(string.find(my_string, "message sent")) then
                subtree:add(request_opcode):append_text("send")
                add_quote_to_tree(subtree, buffer, max_length, 1, response)
            end

        -- Vypisy chybovych hlasek --
        else
            subtree:add(status, buffer(1,4))
            command_errors(my_string, max_length, subtree, buffer, "incorrect password", "login")       -- login err 1
            command_errors(my_string, max_length, subtree, buffer, "unknown user", "login")             -- login err 2
            command_errors(my_string, max_length, subtree, buffer, "already registered", "register")    -- register
            command_errors(my_string, max_length, subtree, buffer, "message id not found", "fetch")     -- fetch
            command_errors(my_string, max_length, subtree, buffer, "unknown recipient", "send")         -- send
            command_errors(my_string, max_length, subtree, buffer, "wrong arguments", "fetch")          -- negative number given to fetch
            command_errors(my_string, max_length, subtree, buffer, "incorrect login", prev_cmd)         -- who knows, lets find out from the previous packet
            prev_cmd = "none"
        end

    -- Nepovedlo se priradit zadny odpovidajici typ -- Asi neni z meho protokolu
    elseif msg_type == "unknown" then
        subtree:add(type):append_text("Packet might not be from ISA Protocol. Or is perhaps only a part of one big message.")
        subtree:add(body, buffer(0, max_length))

    -- Jestli to neni ok, error ani unknown, tak je to jeden z client-side prikazu
    else
        subtree:add(type):append_text("Request")    -- Ve vsech techto pripadech to bude request
        if (msg_type == "reg") then
            subtree:add(command, buffer(1,8))                                                               -- register ma 8 pismen a zacina v bufferu na pozici 1
            add_quote_to_tree(subtree, buffer, max_length, 1, username)                                     -- register ma 2 argumenty - username
            add_quote_to_tree(subtree, buffer, max_length, 2, password)                                     -- password

        elseif (msg_type == "login") then
            subtree:add(command, buffer(1,5))
            add_quote_to_tree(subtree, buffer, max_length, 1, username)
            add_quote_to_tree(subtree, buffer, max_length, 2, password)

        elseif (msg_type == "fetch")  then
            subtree:add(command, buffer(1,5))
            local first_quote, second_quote = add_quote_to_tree(subtree, buffer, max_length, 1, user_id)    -- fetch zasila user token
            add_number_to_tree(subtree, buffer, max_length, message_number, second_quote)                   -- a message ID

        elseif (msg_type == "logout") then
            subtree:add(command, buffer(1,6))
            add_quote_to_tree(subtree, buffer, max_length, 1, user_id)                                      -- logout zasila jen user token

        elseif (msg_type == "send") then
            subtree:add(command, buffer(1,4))
            add_quote_to_tree(subtree, buffer, max_length, 1, user_id)                                      -- user token
            add_quote_to_tree(subtree, buffer, max_length, 2, recipient)                                    -- recipient
            add_quote_to_tree(subtree, buffer, max_length, 3, subject)                                      -- subject
            add_quote_to_tree(subtree, buffer, max_length, 4, body_message)                                 -- message body

        elseif (msg_type == "list") then
            subtree:add(command, buffer(1,4))
            add_quote_to_tree(subtree, buffer, max_length, 1, user_id)                                      -- user token
        end
    end
    subtree:add(msg_length):append_text(max_length) -- Ve vsech pripadech, nehlede na typ zpravy, na jeji konec prilepim delku TCP body segmentu
end

-- Hlavni dissector funkce, ktera je spoustena pro kazdy paket ISA protokolu (tzn. kazdy paket na portu 32323)
function isa_protocol.dissector(buffer, pinfo, tree)
    local max_length = buffer:len()
    if max_length == 0 then                     -- Pokud je delka 0 (Nemelo by nastat), skoncim
        return 
    end

    if (max_length > 32731) then                -- Pokud je delka vetsi nez 32731, tak zpracuju i dalsi segment
        pinfo.cols.protocol = isa_protocol.name     -- Nastaveni kolonky "Protocol" ve wiresharku na ISA
        pinfo.desegment_len = DESEGMENT_ONE_MORE_SEGMENT
    end
    dissect(max_length, buffer, pinfo, tree)

end

-- Prirazeni default portu 32323, ktery bude dissector zpracovavat
local tcp_port = DissectorTable.get("tcp.port")
tcp_port:add(32323, isa_protocol)
