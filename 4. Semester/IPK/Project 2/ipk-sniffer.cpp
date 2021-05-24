/* 
 * IPK-sniffer.cpp
 * Projekt 2 do predmetu IPK na FIT VUT.
 * Autor: Vojtech Fiala <xfiala61>
 * 
 * Tento program vyuziva nektere funkce zalozene na funkcich z
 * volne dostupneho programu https://www.tcpdump.org/sniffex.c, jez
 * byl vydan nasledovne:
 * 
 ****************************************************************************
 * sniffex.c
 *
 * Sniffer example of TCP/IP packet capture using libpcap.
 *
 * Version 0.1.1 (2005-07-05)
 * Copyright (c) 2005 The Tcpdump Group
 *
 * This software is intended to be used as a practical example and
 * demonstration of the libpcap library; available at:
 * http://www.tcpdump.org/
 *
 ****************************************************************************
 *
 * This software is a modification of Tim Carstens' "sniffer.c"
 * demonstration source code, released as follows:
 *
 * sniffer.c
 * Copyright (c) 2002 Tim Carstens
 * 2002-01-07
 * Demonstration of using libpcap
 * timcarst -at- yahoo -dot- com
 *
 * "sniffer.c" is distributed under these terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. The name "Tim Carstens" may not be used to endorse or promote
 *    products derived from this software without prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * <end of "sniffer.c" terms>
 *
 * This software, "sniffex.c", is a derivative work of "sniffer.c" and is
 * covered by the following terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Because this is a derivative work, you must comply with the "sniffer.c"
 *    terms reproduced above.
 * 2. Redistributions of source code must retain the Tcpdump Group copyright
 *    notice at the top of this source file, this list of conditions and the
 *    following disclaimer.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. The names "tcpdump" or "libpcap" may not be used to endorse or promote
 *    products derived from this software without prior written permission.
 *
 * THERE IS ABSOLUTELY NO WARRANTY FOR THIS PROGRAM.
 * BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
 * FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
 * PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
 * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
 * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
 * REPAIR OR CORRECTION.
 *
 * IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
 * WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
 * REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
 * INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
 * OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
 * TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
 * YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
 * PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 * <end of "sniffex.c" terms>
 *
 ****************************************************************************/
#include <iostream>
#include <string>
#include <time.h>
#include <ctype.h>
#include <pcap/pcap.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>

/* https://www.tcpdump.org/pcap.html */
/* Velikost ethernetove hlavicky */
#define ETHERNET_HEADER_SIZE 14

/* https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml */
/* Definice protokolu v ipv6 */
#define IPV6_TCP 6
#define IPV6_UDP 17
#define IPV6_ICMP 58

#define ARGUMENT_ERR 10
#define INTERFACE_ERR 11
#define FILTER_ERR 12
#define PACKET_SIZE_ERROR 15

/* https://www.tcpdump.org/pcap.html */
/* sniffex.c inspired */
/* Funkce slouzici pro vypis jednotlivych radku paketu */
void print_packet_line(u_char *content, int content_len, int offset) {

	/* Offset na 4 mista */
	printf("0x%04x  ", offset);

	/* Vypis hexadecimalni hodnoty (2 mista max) */
	for(int i = 0; i < content_len; i++) {
		printf("%02x ", content[i]);
	}

	/* Vypln mezeru pokud nemam plny radek (16) znaku */
	if (content_len < 16) {
		int space_size = 16 - content_len;
		for (int i = 0; i < space_size; i++) {
			printf("   "); // 3 mezery (2 cisla + mezera za nema)
		}
	}
    /* Vypis mezeru za poslednim */
	printf("    ");

	/* Vypis znak i jako ASCII hodnotu (jestli to jde) */
	for(int i = 0; i < content_len; i++) {
		if (isprint(content[i])) {
			printf("%c", content[i]);
        }
		else {
        /* Jinak vypis jen tecku */
			printf(".");
        }
	}

    /* Na konec novy radek */
	printf("\n");
    return;
}

/* https://www.tcpdump.org/pcap.html */
/* sniffex.c inspired */
/* Funkce na vypis paketu */
void print_packet_content(u_char *content, int content_len) {
    int len_rem = content_len;
	int line_width = 16;			/* Pocet bytu na radek */
	int offset = 0;					/* Offset */
	u_char *ch = content;

	/* Projdi cely paket */
	while(1) {
		/* Jestlize zbyva projit vic nez 16, vytiskni 16 cisel */
        if (len_rem > line_width) {
		    print_packet_line(ch, line_width, offset);
        }
        /* Jinak vytiskni jenom tolik, kolik zbyva a ukonci cyklus */
        else {
            print_packet_line(ch, len_rem, offset);
            break;
        }
        /* Odecti od zbyvajicich prave vytisklych 16 */
        len_rem -= line_width;
		/* Posun ukazatel aktualni hodnoty o 16 dal*/
 		ch = ch + line_width;
		/* Zvetsi offset o 10 (v hexa 10 == 16) */
		offset += line_width;
	}
    printf("\n");
    return;
}

/* Funkce na zpracovani ICMP paketu */
/* Volne zalozene na funkci na zpracovani TDP paketu z sniffex.c */
/* V pripade ICMP nemam port, takze vypisuju 
   jenom IP adresy hosta a cile a obsah */
void read_icmp(const char *dst_ip, const u_char *packet, unsigned int ip_offset, unsigned int packet_size) {
    printf(" > %s", dst_ip);
    printf(", length %d bytes\n\n", packet_size);
    auto content = (u_char *)(packet);
    if (packet_size > 0) {
		print_packet_content(content, packet_size);
	}

    return;
}

/* https://www.tcpdump.org/pcap.html */
/* Volne zalozene na funkci na zpracovani TDP paketu z sniffex.c */
/* Funkce na zpracovani TCP a UDP paketu */
/* U UDP/TCP packetu se zapisuje i port hosta/cile */
void read_tcp_udp(const char *dst_ip, const u_char *packet, unsigned int ip_offset, unsigned int packet_size, bool tcp) {

    uint16_t source, dest = 0;
    if (tcp) {
        auto packet_protocol = (struct tcphdr *)(packet + ETHERNET_HEADER_SIZE + ip_offset);
        source = ntohs(packet_protocol->th_sport);
        dest = ntohs(packet_protocol->th_dport);
    }
    else {
        auto packet_protocol = (struct udphdr *)(packet + ETHERNET_HEADER_SIZE + ip_offset);
        source = ntohs(packet_protocol->uh_sport);
        dest = ntohs(packet_protocol->uh_dport);
    }

    printf(" : %d ", source);
    printf("> %s ", dst_ip);
	printf(": %d", dest);
    printf(", length %d bytes\n\n", packet_size);
    auto content = (u_char *)(packet);
    if (packet_size > 0) {
		print_packet_content(content, packet_size);
	}

    return;
}

// http://www.qnx.com/developers/docs/6.5.0/index.jsp?topic=%2Fcom.qnx.doc.neutrino_lib_ref%2Fi%2Finet_ntop.html
/* Funkce na zpracovani IPV6 protokolu */
void read_ipv6(const u_char *packet, const struct pcap_pkthdr *header) {
    char buf_src[INET6_ADDRSTRLEN];
    char buf_dst[INET6_ADDRSTRLEN];
    auto ip = (struct ip6_hdr*) (packet + ETHERNET_HEADER_SIZE);
    printf(" %s", inet_ntop(AF_INET6, &ip->ip6_src, buf_src, sizeof(buf_src))); // Prepis ipv6 adresu na string

    if (header->len < 20) {
        fprintf(stderr, "Neplatna velikost paketu!\n");
        exit(PACKET_SIZE_ERROR);
    }
    auto payload_length = packet[18] + packet[19]; // V ipv6 je delka obsahu na pozici 18 a 19 (zjisteno z wiresharku)
    auto ip_offset = header->len - payload_length - ETHERNET_HEADER_SIZE;

    bool tcp = false;
    bool udp = false;
    bool icmp = false;

    /* Urci podprotokol */
    switch(ip->ip6_ctlun.ip6_un1.ip6_un1_nxt) {
		case IPV6_TCP:
            tcp = true;
			break;
        case IPV6_UDP:
            udp = true;
            break;
        case IPV6_ICMP:
            icmp = true;
            break;
		default:
            if (header->len > 0) {
                auto content = (u_char *)(packet);
                print_packet_content(content, header->len);
            }
			return;
	}

    /* Zpracuj paket na zaklade protokolu */
    if (tcp) {
        read_tcp_udp(inet_ntop(AF_INET6, &ip->ip6_dst, buf_dst, sizeof(buf_dst)), packet, ip_offset, header->len, true);
    }
    if (udp) {
        read_tcp_udp(inet_ntop(AF_INET6, &ip->ip6_dst, buf_dst, sizeof(buf_dst)), packet, ip_offset, header->len, false);
    }
    if (icmp) {
        read_icmp(inet_ntop(AF_INET6, &ip->ip6_dst, buf_dst, sizeof(buf_dst)), packet, ip_offset, header->len);
    }

    return;
}

/* Funkce na zpracovani IPV4 protokolu */
void read_ipv4(const u_char *packet, const struct pcap_pkthdr *header) {
    auto ip = (struct ip*) (packet + ETHERNET_HEADER_SIZE);
    printf(" %s", inet_ntoa(ip->ip_src));   // Zkonvertuj IPV4 adresu na string

    /* Na zaklade analyzy ipv4 paketu ve wiresharku vim, ze musim
       delku headeru nasobit *4 */
    auto ip_offset = ip->ip_hl*4;

    bool tcp = false;
    bool udp = false;
    bool icmp = false;

    /* Urci podprotokol */
    /* Hodnoty podprotokolu zjisteny z <netinet/in.h> knihovny*/
    switch(ip->ip_p) {
		case IPPROTO_TCP:
            tcp = true;
			break;
        case IPPROTO_UDP:
            udp = true;
            break;
        case IPPROTO_ICMP:
            icmp = true;
            break;
		default:
            if (header->len > 0) {
                printf("> %s ", inet_ntoa(ip->ip_dst));
                auto content = (u_char *)(packet);
                print_packet_content(content, header->len);
            }
			return;
	}

    /* Zpracuj paket na zaklade protokolu */
    if (tcp) {
        read_tcp_udp(inet_ntoa(ip->ip_dst), packet, ip_offset, header->len, true);
    }
    if (udp) {
        read_tcp_udp(inet_ntoa(ip->ip_dst), packet, ip_offset, header->len, false);
    }
    if (icmp) {
        read_icmp(inet_ntoa(ip->ip_dst), packet, ip_offset, header->len);
    }

    return;
}

/* Funkce na zpracovani ARP packetu */
void read_arp(const u_char *packet, unsigned int packet_size) {

    auto content = (u_char *)(packet);
    printf(" ");

    if (packet_size < 39) {
        fprintf(stderr, "Neplatna velikost paketu!\n");
        exit(PACKET_SIZE_ERROR);
    }
    /* Projdi byty odpovidajici v paketu umisteni source mac adresy 
       a tiskni je ve spravnem formatu */
    for (int i = 22; i < 28;++i) {	// Source adresa, vytazene na zaklade analyzy paketu ve wiresharku
        	printf("%02x", content[i]);
       	if (i != 27)
       		printf(":");
        	}

    printf(" > ");
    for (int i = 32; i < 38;++i) {	// Target adresa
        	printf("%02x", content[i]);
       	if (i != 37)
       		printf(":");
        	}

    printf(", length %d bytes\n\n", packet_size);
    if (packet_size > 0) {
		print_packet_content(content, packet_size);
	}

    return;
}

/* Callback funkce pro cteni jednotlivych ziskanych paketu */
/* Inspirovana sniffex.c */
void read_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    char ms[10];

    /* Desetinnou cast dostavam ve statisicich, tak se jich zbav delenim 1000 a neres zbytek */
    sprintf(ms, "%03ld", std::ldiv(header->ts.tv_usec, 1000).quot);

    // https://stackoverflow.com/questions/13536886/unixtime-to-readable-date (unixtime to date)
    // https://en.cppreference.com/w/cpp/chrono/c/strftime (formatting signs)
    time_t time = (time_t)header->ts.tv_sec; // Vytvor time_t ze sekund
    struct tm *tm = localtime(&time);
    char date[90];
    char timezone[90];
    strftime(date, sizeof(date), "%Y-%m-%dT%H:%M:%S.", tm); // zformatuj to na pozadovany typ data
    strftime(timezone, sizeof(timezone), "%z", tm); // Kvuli dvojtecce v casove zone
    std::string new_timezone;
    new_timezone = new_timezone + timezone[0] + timezone[1] + timezone[2] +  ':' + timezone[3] + timezone[4];
    std::string new_time = date; // Pro jednoduchost appendu z toho udelej std::string
    new_time = new_time + ms + new_timezone;

    std::cout << new_time;

    /* Vytvor ethernetovou hlavicku pretypovanim */
    auto eptr = (struct ether_header*) packet;
 
    /* Na zaklade ethernet typu zjisti protokol */
    switch(ntohs(eptr->ether_type)) {
        case ETHERTYPE_IP:  // ipv4 
            read_ipv4(packet, header);
            break;
        case ETHERTYPE_IPV6:    // ipv6
            read_ipv6(packet, header);
            break;
        case ETHERTYPE_ARP:
            read_arp(packet, header->len);
            break;
        /* Kdyz nevis, co to je, jenom vytiskni cas, delku a obsah */
        default:
            if (header->len > 0) {
	            printf(", length %d bytes\n", header->len);
                auto content = (u_char *)(packet);
                print_packet_content(content, header->len);
            }

            return;
    }
    return;
}

/* Funkce pro tvorbu filtru na zaklade zadanych parametru */
std::string make_filter(int arg_count, std::string *args, std::string port) {
    std::string flags = "";

    /* Jestlize byl zadan port, vepis ho primo k tcp a udp castem filtru.
       Ale pouze za podminky, ze ony samotne byly zadany */
    if (port != "") {
        if (args[2] != "") {
            args[2] = "(udp and " + port + ")";
        }
        if (args[3] != "") {
            args[3] = "(tcp and " + port + ")";
        }
    }

    if (arg_count > 1)  {   // Ma smysl resit logicke spojky jen pokud mam vic nez 1 parametr
        flags += "(";
        for (int i = 0; i < 4; i++) {
            /* Pokud nejsi na zacatku a neresis prazdny flag, pridej logickou spojku or */
            if (i != 0 && args[i] != "" && flags != "(") {
                flags += " or " + args[i];
            }
            /* Jinak pridej jenom argument */
            else {
                flags += args[i];
            }
        }
        flags += ")";
    }
    else {
        for (int i = 0; i < 4; i++) {
            flags += args[i];
        }
    }

    /* Jestlize nebyl zadan tcp ani udp, ale arp nebo icmp a port */
    if ((args[0] != "" || args[1] != "") && port != "" && (args[2] == "" && args[3] == "")) {
        flags += " or " + port;
    }

    /* Jestlize byl zadan jediny argument a byl to port, nepridavej logickou spojku */
    else if (arg_count == 1 && port != "") {
        flags += port;
    }

    return flags;
}

// Funkce na vypsani napovedy v pripade neznameho parametru
void print_help() {
    std::cout << "Volani programu: ./ipk-sniffer [-i rozhrani | --interface rozhrani] {-p ­­port} {[--tcp|-t] [--udp|-u] [--arp] [--icmp] } {-n num}\n";
    std::cout << "{} => Nepovinny parametr\n";
    std::cout << "[] => Povinny parametr\n";
    std::cout << "Argumenty mohou být v libovolném pořadí\n";
}

/* Hlavni ridici funkce */
int main(int argc, char *argv[]) {

    char *interface = nullptr;
    bool i_flag = true;
    int packets_to_show = 1;

    /* Stringy slouzici jako multifunkcni flagy */
    std::string arp_flag = "";
    std::string icmp_flag = "";
    std::string udp_flag = "";
    std::string tcp_flag = "";
    std::string port = "";

    // https://man7.org/linux/man-pages/man3/getopt.3.html
    /* Copyright (c) 1993 by Thomas Koenig (ig25@rz.uni-karlsruhe.de)
    and Copyright 2006-2008, Michael Kerrisk <mtk.manpages@gmail.com> */
    struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"udp", no_argument, 0, 'u'},
        {"arp", no_argument, 0, 1},
        {"icmp", no_argument, 0, 2},
        {"tcp", no_argument, 0, 't'},
        {nullptr,  optional_argument, 0, 'i'},
        {nullptr,  required_argument, 0, 'n'},
        {nullptr, required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    int arg_count = 0;
    int index;

    /* Zpracuj vsechny argumenty */
    while (true) {
        switch (getopt_long(argc, argv, ":tuhi: n:p:", long_options, &index)) {
            case 1:
                arp_flag = "arp";   // Spravne nastav multifunkcni vlajku
                arg_count++;    // Navys pocet argumentu
                continue;
            case 2:
                icmp_flag = "(icmp or icmp6)"; // icmp jsou specialni - jeden pro ipv4 a druhy pro ipv6
                arg_count++;
                continue;
            case 'u':
                udp_flag = "udp";
                arg_count++;
                continue;
            case 't':
                tcp_flag = "tcp";
                arg_count++;
                continue;
            /* Specialni case pro chybejici povinny operand */
            case ':':
                switch (optopt) {
                    /* Jestlize mi chybi povinny operand u 'i', je to v poradku */
                    case 'i':
                        continue;
                    /* Jinde uz to ale v poradku neni */
                    default:
                        fprintf(stderr, "%s\n", "Neplatny argument!\n");
                        print_help();
                        exit(ARGUMENT_ERR);
                        break;
                }
            case 'i':
                interface = optarg;
                i_flag = false;
                continue;
            case 'p':
                /* Try & catch kvuli vecem typu '-p text' */
                try {
                /* Port by nemel byt zaporny nebo nad 65535 */
                    if (std::stoi(optarg) < 0 || std::stoi(optarg) > 65535) {
                        fprintf(stderr, "%s\n", "Neplatny port!");
                        exit(ARGUMENT_ERR);
                    }
                } 
                catch (const std::invalid_argument &e) {
                    fprintf(stderr, "%s\n", "Neplatny port!");
                    exit(ARGUMENT_ERR);
                }
                arg_count++;
                port = "port ";
                port.append(optarg);
                continue;
            case 'n':
                packets_to_show = std::stoi(optarg);
                if (packets_to_show <= 0) {
                    fprintf(stderr, "%s\n", "Neplatne mnozstvi paketu!");
                    exit(ARGUMENT_ERR);
                }
                continue;
            case -1:    // -1 znaci, ze uz neni co dal zpracovavat
                break;
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
                break;
            case '?':
            default:
                fprintf(stderr, "%s\n", "Neplatny argument!\n");
                print_help();
                exit(ARGUMENT_ERR);
                break;
        }
        break;
    }

    /* Tvoreni filtru */
    std::string args[4] = {arp_flag, icmp_flag, udp_flag, tcp_flag};    // Array stringu
    std::string flags = make_filter(arg_count, args, port);  // Filtr

    pcap_if_t *tmp;
    char errbuf[PCAP_ERRBUF_SIZE];  // String pro vypis pripadnych chyb

    /* https://www.winpcap.org/docs/docs_412/html/group__wpcap__tut1.html (vypis rozhrani)*/
    if (i_flag) {
        /* Nacti vsechna dostupna rozhrani */
        pcap_findalldevs(&tmp, errbuf);

        /* Jestli zadne neni, je neco spatne */
        if (tmp == nullptr) {
            fprintf(stderr, "Zadna rozhrani nenalezena!\n");
            exit(INTERFACE_ERR);
        }

        /* Cykli skrz vsechny rozhrani a vypisuj je */
        for(; tmp != nullptr; tmp=tmp->next) {
            printf("%s\n", tmp->name);
        }

        /* Uvolni pamet a skonci */
        pcap_freealldevs(tmp);
        exit(EXIT_SUCCESS);
    }


    pcap_t *handle;
    /* Otevri rozhrani pro sniffing */
	handle = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);
    /* Jestli se to nepovedlo, je neco spatne */
	if (handle == NULL) {
		fprintf(stderr, "Nepovedlo se otevrit rozhrani %s! Chyba: %s\n", interface, errbuf);
		exit(INTERFACE_ERR);
	}

    // https://stackoverflow.com/questions/7352099/stdstring-to-char/7352131#:~:text=Although%20the%20other%20answers%20are,a%20pointer%20to%20the%20string.
    char* filter = const_cast<char*>(flags.c_str());

    // https://www.tcpdump.org/pcap.html
    bpf_u_int32 net = 0;
    struct bpf_program fp;
    if (pcap_compile(handle, &fp, filter, 0, net) == -1) {
		fprintf(stderr, "Nebylo mozne aplikovat filtr %s! Chyba %s\n", filter, pcap_geterr(handle));
		exit(FILTER_ERR);
	}

    if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Nepovedlo se aplikovat filtr %s! Chyba %s\n", filter, pcap_geterr(handle));
		exit(FILTER_ERR);
	}

    /* Chytej pakety, dokud jich nezpracujes zadany pocet */
    pcap_loop(handle, packets_to_show, read_packet, NULL);

    /* Ukonci spojeni */
	pcap_close(handle);

	return(EXIT_SUCCESS);
}
