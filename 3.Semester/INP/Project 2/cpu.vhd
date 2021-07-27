-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2020 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Vojtech Fiala <xfiala61>
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WE    : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti 
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu
	signal counter_increase : std_logic;
	signal counter_decrease : std_logic;
	signal counter_load : std_logic;
	signal counter_out : std_logic_vector(11 downto 0);
	
	signal while_beginning_address : std_logic_vector(11 downto 0);
	signal stack_push : std_logic;
	
	signal pointer_increase : std_logic;
	signal pointer_decrease : std_logic;
	signal pointer_out : std_logic_vector(9 downto 0);
	
	signal multiplexor_select : std_logic_vector(1 downto 0);
	signal multiplexor_out : std_logic_vector(7 downto 0);
	
	type FSMstate is (
	cpu_idle,
	cpu_fetch,
	cpu_decode,
	
	pointerIncrease,
	pointerDecrease,

	currentIncrease,
	currentIncrease_pt2,
	currentIncrease_pt3,
	currentDecrease,
	currentDecrease_pt2,
	currentDecrease_pt3,
	
	whileStart,
	whileStart_pt2,
	whileStart_pt3,
	whileStart_pt4,
	whileEnd,
	whileEnd_pt2,
	whileEnd_reset,
	whileEnd_repeat,
	
	write_value,
	write_value_pt2,
	write_value_pt3,
	read_value,
	read_value_pt2,
	read_value_pt3,
	
	stop_null,
	other
	
	);
  	signal current_state : FSMstate; -- current state
  	signal next_state : FSMstate; -- next state

begin
 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --   - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --   - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly.
 

	--****PC****--
	program_counter: process(CLK, RESET, counter_increase, counter_decrease, counter_load, stack_push) is
	begin
		if (RESET = '1') then
			counter_out <= "000000000000";
		elsif (CLK'event) and (CLK = '1') then
			if (counter_load = '1') then			-- Kdyz byl zavolan load, vrat se na zacatek while cyklu
				counter_out <= while_beginning_address;
			elsif (counter_increase = '1') then
				counter_out <= counter_out + 1;		-- posun se dopredu
				if stack_push = '1' then		-- jestli byl volan stack_push, uloz momentalni pozici
					while_beginning_address <= counter_out;
				end if;
			elsif (counter_decrease = '1') then
				counter_out <= counter_out - 1;		-- posun se dozadu
				if stack_push = '1' then
					while_beginning_address <= counter_out;
				end if;
			end if;
		end if;
	end process;
	CODE_ADDR <= counter_out;					-- zapis do adresy
	--****END OF PC****--

	--****PTR****--
	pointer: process(CLK, RESET, pointer_increase, pointer_decrease) is
	begin
		if (RESET = '1') then
			pointer_out <= "0000000000";
		elsif (CLK'event) and (CLK = '1') then
			if (pointer_increase = '1') then	
				pointer_out <= pointer_out + 1;
			elsif (pointer_decrease = '1') then
				pointer_out <= pointer_out - 1;
			end if;
		end if;
	end process;
	DATA_ADDR <= pointer_out;
	--****END OF PTR****--

	--****MULTIPLEXOR****--
	multiplexor: process(multiplexor_select, CLK, RESET) is
	begin
   		if (RESET = '1') then
				multiplexor_out <= "00000000";
		elsif (CLK'event) and (CLK = '1') then
			case multiplexor_select is
				when "00" =>				-- kdyz 00, zapis vstup
					multiplexor_out <= IN_DATA;
				when "01" =>				-- kdyz 01, sniz aktualni bunku o 1
					multiplexor_out <= DATA_RDATA - 1;
				when "10" =>				-- kdyz 10, zvys aktualni bunku o 1
					multiplexor_out <= DATA_RDATA + 1;
				when others =>
					multiplexor_out <= "00000000";
			end case;
		end if;
	end process;
	DATA_WDATA <= multiplexor_out;
	--****END OF MULTIPLEXOR****--
	
	
	--****MEALY MACHINE****--
	pstatereg: process(RESET, CLK, EN) is
	begin
  		if (RESET = '1') then
			current_state <= cpu_idle;					-- zacatek					
   		elsif (CLK'event) and (CLK = '1') then	
				if EN = '1' then
					current_state <= next_state;			-- dalsi stav
				end if;
		end if;
	end process;
	
	mealy: process(current_state, CODE_DATA, DATA_RDATA, OUT_BUSY, IN_VLD, counter_out) is
	begin 
		-- inicializace hodnot
		multiplexor_select <= "00";
		DATA_EN <= '0';
		DATA_WE <= '0';
		OUT_WE <= '0';
		IN_REQ <= '0';
		CODE_EN <= '0';
		counter_increase <= '0';
		counter_decrease <= '0';
		counter_load <= '0';
		stack_push <= '0';
		pointer_increase <= '0';
		pointer_decrease <= '0';
		
		case current_state is
			when cpu_idle =>			-- z necinnosti prejdi do stavu nacitani instrukce
				next_state <= cpu_fetch;
			when cpu_fetch =>			-- nacti instrukci
				next_state <= cpu_decode;
				CODE_EN <= '1';			-- povol cinnost
			when cpu_decode =>			-- zpracuj instrukci
				case CODE_DATA is
					when X"3E" =>
						next_state <= pointerIncrease;
					when X"3C" =>
						next_state <= pointerDecrease;
					when X"2B" =>
						next_state <= currentIncrease;
					when X"2D" =>
						next_state <= currentDecrease;
					when X"5B" =>
						next_state <= whileStart;
					when X"5D" =>
						next_state <= whileEnd;
					when X"2E" =>
						next_state <= write_value;
					when X"2C" =>
						next_state <= read_value;
					when X"00" =>
						next_state <= stop_null;
					when others =>
						next_state <= other;
				end case;
		
			when pointerIncrease =>
				next_state <= cpu_fetch;	-- pokracuj dalsi instrukci
				pointer_increase <= '1';
				counter_increase <= '1';
				
			when pointerDecrease =>
				next_state <= cpu_fetch;
				pointer_decrease <= '1';
				counter_increase <= '1';
				
			when currentIncrease =>
				next_state <= currentIncrease_pt2;
				DATA_EN <= '1';		
				DATA_WE <= '0';			-- nacti RDATA
			when currentIncrease_pt2 =>
				next_state <= currentIncrease_pt3;
				multiplexor_select <= "10";	-- WDATA++
			when currentIncrease_pt3 =>
				next_state <= cpu_fetch;
				DATA_EN <= '1';		
				DATA_WE <= '1';			-- prepis data
				counter_increase <= '1';	
				
			when currentDecrease =>
				next_state <= currentDecrease_pt2;
				DATA_EN <= '1';
				DATA_WE <= '0';
			when currentDecrease_pt2 =>
				next_state <= currentDecrease_pt3;
				multiplexor_select <= "01";	-- WDATA--
			when currentDecrease_pt3 =>
				next_state <= cpu_fetch;
				DATA_EN <= '1';
				DATA_WE <= '1';
				counter_increase <= '1';

			when whileStart =>
				next_state <= whileStart_pt2;
				counter_increase <= '1';
				stack_push <= '1';
				DATA_EN <= '1';
				DATA_WE <= '0';
			when whileStart_pt2 =>
				if DATA_RDATA = "00000000" then
					next_state <= whileStart_pt3;
					CODE_EN <= '1';
				else 
					next_state <= cpu_fetch;
				end if;
			when whileStart_pt3 =>
				if CODE_DATA = X"5D" then
					next_state <= cpu_fetch;	-- kdyz narazis na ukoncovaci zavorku, dej prostor pro ukonceni
				else
					next_state <= whileStart_pt4;	-- jinak cykli dal
				end if;
			when whileStart_pt4 =>
				next_state <= whileStart_pt3;
				counter_increase <= '1';
				CODE_EN <= '1';

			when whileEnd =>
				next_state <= whileEnd_pt2;
				DATA_EN <= '1';
				DATA_WE <= '0';
			when whileEnd_pt2 =>
				if DATA_RDATA = "00000000" then
					next_state <= whileEnd_reset;
				else 
					next_state <= whileEnd_repeat;
				end if;
			when whileEnd_reset =>
				next_state <= cpu_fetch;
				counter_increase <= '1';
			when whileEnd_repeat =>
				next_state <= cpu_fetch;
				counter_load <= '1';
			
			when write_value =>
				next_state <= write_value_pt2;
				DATA_EN <= '1';
				DATA_WE <= '0';		-- nacti RDATA, at se muzou vypsat
			when write_value_pt2 =>
				if OUT_BUSY = '1' then	-- jestli je "obsazeno", opakuj dokud se neuvolni
					next_state <= write_value_pt2;
					DATA_EN <= '1';
					DATA_WE <= '0';	
				else
					next_state <= write_value_pt3;	-- jinak pokracuj dal
				end if;
			when write_value_pt3 =>
				next_state <= cpu_fetch;
				OUT_WE <= '1';		-- povol zapis
				counter_increase <= '1';

			when read_value =>
				next_state <= read_value_pt2;
				IN_REQ <= '1';
				multiplexor_select <= "00";
			when read_value_pt2 =>
				if IN_VLD = '1' then
					next_state <= read_value_pt3;
				else 
					next_state <= read_value_pt2;
					IN_REQ <= '1';
					multiplexor_select <= "00";
				end if;
			when read_value_pt3 =>
				next_state <= cpu_fetch;
				DATA_EN <= '1';
				DATA_WE <= '1';		-- zapis WDATA
				counter_increase <= '1';

			when stop_null =>
				next_state <= stop_null;
			
			when other =>
				counter_increase <= '1';
				next_state <= cpu_fetch;
				
			when others =>
				next_state <= other;
			
		end case;
	end process;
	OUT_DATA <= DATA_RDATA;
	--****END OF THE MEALY MACHINE****--
end behavioral;
 
