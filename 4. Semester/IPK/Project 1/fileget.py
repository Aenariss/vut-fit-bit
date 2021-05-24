#!/usr/bin/env python3
#VUT FIT IPK 2021 - PROJEKT 1
#AUTOR: Vojtech Fiala <xfiala61>

import sys
import re
import socket

def stderr_exit(stderr_args, exit_code=1):
    sys.stderr.write(stderr_args)
    exit(exit_code)

def is_valid_server(strng):
    name = None
    test_str = strng[:6].lower()
    file = None

    if (re.search("^fsp://", test_str)):
        i = 6
        leng = len(strng)

        while (strng[i] != '/' and i < leng):
            i += 1
            if i >= leng:
                stderr_exit("Invalid file location!\n")

        try:
            name = strng[6:i]
            file = strng[i+1:]
        except:
            stderr_exit("Internal error has occured!\n")

        return True, name, file

    return False, name, file

def divide(strng, msg):
    i = 0
    leng = len(strng)

    while (strng[i] != ':' and i < leng):
        i += 1
        if i >= leng:
            stderr_exit(msg)

    try:
        x = strng[0:i]
        strng = strng[i+1:leng]
    except:
        stderr_exit("Internal error has occured!\n")

    return x, strng

def is_valid_port(port):
    if (re.search("^[0-9]+$", port)):
        if (int(port) < 65536 and int(port) >= 0):
            return True
    return False

def is_valid_ip(strng):
    ip, port = divide(strng, "Invalid port!\n")

    if not (is_valid_port(port)):
        stderr_exit("Invalid Port!\n")

    try:
        port = int(port)
    except:
        stderr_exit("Internal error has occured!\n")

    connection_values = [ip, port]

    if (re.findall("\s+", ip)):
        return False, connection_values

    x = ip.split('.')
    if len(x) != 4:
        return False, connection_values
        
    for word in x:
        if len(word) == 3:
            if not (re.search("^[1-2][0-9][0-9]$", word)):
                return False, connection_values
        elif len(word) == 2:
            if not (re.search("^[0-9][0-9]*$", word)):
                return False, connection_values
        elif len(word) == 1:
            if not (re.search("^[0-9]+$", word)):
                return False, connection_values
        else:
            return False, connection_values

        if int(word) > 255:
            return False, connection_values

    return True, connection_values

def arg_parse():
    option_1 = False
    option_2 = False
    error_args = "Invalid Arguments!\n"
    error_ip = "Invalid IP address!\n"
    error_server = "Invalid Server!\n"
    argv = sys.argv
    argc = len(argv)

    if (argc != 5):
        stderr_exit(error_args)

    if (argv[1] == '-n' and argv[3] == '-f'):
        option_1 = True

    elif (argv[1] == '-f' and argv[3] == '-n'):
        option_2 = True

    else:
        stderr_exit(error_args)

    if (option_1):
        valid_ip, values = is_valid_ip(argv[2])
        valid_name, name, file = is_valid_server(argv[4])
        if not (valid_ip):
            stderr_exit(error_ip)
        if not (valid_name):
            stderr_exit(error_server)

        values.append(name)
        values.append(file)
        return values

    elif (option_2):
        valid_ip, values = is_valid_ip(argv[4])
        valid_name, name, file = is_valid_server(argv[2])
        if not (valid_ip):
            stderr_exit(error_ip)
        if not (valid_name):
            stderr_exit(error_server)

        values.append(name)
        values.append(file)
        return values

    else:
        stderr_exit(error_args)

def resolve_name(ip, port, server_name):
    client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client.settimeout(30)
    message = "WHEREIS " + server_name
    message = message.encode()


    client.connect((ip, port))
    client.sendall(message)
    try:
        received = client.recv(4096)
    except:
        stderr_exit("Problem arised when connecting to server!\n")

    received = received.decode()
    if (re.search("^ERR", received)):
        stderr_exit(received + '\n')

    client.close()

    ip_ret, port_ret = divide(received[3:], "Invalid address received from nameserver!\n")
    port_ret = int(port_ret)

    return ip_ret, port_ret

def check_result(msg):
    check = msg.split(b"\r\n", 3)
    check_return = check[0]
    check_return = check_return[8:]
    length = check[1]
    length = length.decode()
    save = 0
    for i in range(0, len(length)):
        if ord(length[i]) > 47 and ord(length[i]) < 58:
            save = i
            break
    length = length[save:]
    try:
        length = int(length)
    except:
        stderr_exit("An internal error occured and the program had to be terminated")

    if not (re.search(b"^Success", check_return)):
        check_return = check_return.decode()
        stderr_exit(check_return + '\n')

    ret = check[3]
    return ret, length

def get_file(ip, port, server_name, file_nm):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.settimeout(30)
    flag_all = False

    try:
        if (file_nm[-1] == '*'):
            flag_all = True
    except:
        stderr_exit("Invalid file location!\n")

    if (flag_all):
        get = "GET " + 'index' + " FSP/1.0\r\n"

    else:
        get = "GET " + file_nm + " FSP/1.0\r\n"

    hostname = "Hostname: " + server_name + "\r\n"
    agent = "Agent: xfiala61\r\n"
    blank = "\r\n"

    message = get + hostname + agent + blank
    message = message.encode()

    try:
        client.connect((ip, port))
        client.sendall(message)
    except:
        stderr_exit("Couldn't connect to file server!\n")

    msg = b''
    try: 
        message_content = []
        while True:
            received = client.recv(4096)
            message_content.append(received)
            if not received:
                break
        msg += b"".join(message_content)
    except:
        stderr_exit("Problem arised when connecting to server!\n")

    msg, ln = check_result(msg)
    if ln != len(msg):
        stderr_exit("Problem arised when connecting to server!\n")
    
    if (flag_all):
        msg = msg.decode()
        tmp = msg.splitlines()
        ln = len(tmp)
        files = []

        for i in range(0, ln):
            files.append(tmp[i])
        for file in files:
            get_file(ip, port, server_name, file)

    else:
        i = 0
        leng = len(file_nm)
        last = 0

        while (i < leng):
            if file_nm[i] == '/':
                last = i
            i += 1

        if file_nm[last] != '/':
            file_nm = file_nm[last:]
        else:
            file_nm = file_nm[last+1:]

        try:
            f = open(file_nm, 'wb')
        except:
            stderr_exit("Invalid file!\n")

        f.write(msg)
        f.close()

    client.close()
    return True


def main():
    values = arg_parse()
    ip = values[0]
    port = values[1]
    server_name = values[2]
    file = values[3]

    server_ip, server_port = resolve_name(ip, port, server_name)
    received_file = get_file(server_ip, server_port, server_name, file)

if __name__ == "__main__":
    main()
