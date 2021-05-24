library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- rozhrani Vigenerovy sifry
entity vigenere is
   port(
         CLK : in std_logic;
         RST : in std_logic;
         DATA : in std_logic_vector(7 downto 0);
         KEY : in std_logic_vector(7 downto 0);

         CODE : out std_logic_vector(7 downto 0)
    );
end vigenere;

-- V souboru fpga/sim/tb.vhd naleznete testbench, do ktereho si doplnte
-- znaky vaseho loginu (velkymi pismeny) a znaky klice dle vaseho prijmeni.

architecture behavioral of vigenere is

    -- Sem doplnte definice vnitrnich signalu, prip. typu, pro vase reseni,
    -- jejich nazvy doplnte tez pod nadpis Vigenere Inner Signals v souboru
    -- fpga/sim/isim.tcl. Nezasahujte do souboru, ktere nejsou explicitne
    -- v zadani urceny k modifikaci.
	type FSMstate is (Addition, Substraction);
  	signal current_state : FSMstate; -- current state
  	signal next_state : FSMstate; -- next state
	signal mealy_out : std_logic_vector(7 downto 0);
	signal shift : std_logic_vector(7 downto 0);
	signal correct_positive : std_logic_vector(7 downto 0);
	signal correct_negative : std_logic_vector(7 downto 0);
	

begin

    -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.

	--****MEALY MACHINE****--
	-- Present state register
	pstatereg: process(RST, CLK)
	begin
  		if (RST='1') then
			current_state <= Addition;		-- begin with +
   		elsif (CLK'event) and (CLK='1') then		-- follow with -
     			current_state <= next_state;
   		end if;
	end process;
	
	mealy: process(current_state, RST, DATA) is
	begin 
		next_state <= current_state;
		case current_state is
			when Addition =>			-- after addition (+) comes substraction (-), so we have to switch states accordingly and set 'return' values
				next_state <= Substraction;
				mealy_out <= "00000000";	-- add -> 'return' 0
			when Substraction =>
				next_state <= Addition;
				mealy_out <= "00000001";	-- substract -> 'return' 1
			when others => null;
		end case;

		-- check if we have an active reset signal or if the data is a number - if we do, output has to be a hashtag - signal it with a 'return' value of 2
		if RST = '1' then
			mealy_out <= "00000010";		-- else return (2)
		end if;
		if (DATA > 47) then
			if (DATA < 58) then
				mealy_out <= "00000010";	-- else return (2)
			end if;
		end if;

	end process;
	--****END OF THE MEALY MACHINE****--


	--****CALCULATE SHIFT****---
	get_shift: process (DATA, KEY) is 
		variable ASCII_SHIFT : std_logic_vector(7 downto 0);
	begin
		ASCII_SHIFT := "01000000";	-- in 8bit binary 64 is 01000000
		shift <= KEY - ASCII_SHIFT;
		-- first letter 'A' has ascii value of 65 and shifts our result by 1, therefore 65-64 = 1
	end process;
	--****END OF SHIFT CALCULATION****--

	--****SHIFT A LETTER****--
	shift_letter: process (shift, DATA) is
		variable NEW_VAL : std_logic_vector(7 downto 0);
		variable ASCII_LIMIT_Z : std_logic_vector(7 downto 0);
		variable CORRECTION : std_logic_vector(7 downto 0);
		variable ASCII_LIMIT_A : std_logic_vector(7 downto 0);	
	begin 
		ASCII_LIMIT_Z := "01011010";	-- Z has an ascii value of 90 -> 64 + 16 + 8 + 2 = 01011010
		CORRECTION := "00011010";	-- we need to shift the value by 26 to prevent overflow or underflow -> 16 + 8 + 2 = 00011010
		ASCII_LIMIT_A := "01000001";	-- A has an ascii value of 65 -> 64 + 1 = 01000001

		--****BEGIN POSTIVIE SHIFTING****--
		NEW_VAL := DATA;
		NEW_VAL := NEW_VAL + shift;
		-- if our new letter overflows the ASCII value of alphabet letters, we need to correct it -> from 91, which doesnt represent anything, we need to make an 'A', which is 65.
		-- So, we do a little correction, 91 - CORRECTION = 65   => Correction has to be 26
		if (NEW_VAL > ASCII_LIMIT_Z) then 
			NEW_VAL := NEW_VAL - CORRECTION;
		end if;
		correct_positive <= NEW_VAL;
		--****END POSITIVIE SHIFTING****--


		--****BEGIN NEGATIVE SHIFTING****--
		NEW_VAL := DATA;
		NEW_VAL := NEW_VAL - shift;
		-- If we have less than 65, for example 64, we need to add 26 to make it a 'Z', which has a value of 90
		if (NEW_VAL < ASCII_LIMIT_A) then
			NEW_VAL := NEW_VAL + CORRECTION;
		end if;
		correct_negative <= NEW_VAL;
		--****END NEGATIVE SHIFTING****---
	end process;
	--****END OF SHIFTING****--


	--****FINAL MULTIPLEXOR****--
	multiplexor: process(mealy_out, correct_positive, correct_negative)
		variable HASHTAG : std_logic_vector(7 downto 0);
	begin
		HASHTAG := "00100011"; 	-- ascii value of a hashtag (35) on an 8bit number => 32 + 3 = 32 + 1 + 2, so 00100011
   		case mealy_out is
			when "00000000" => CODE <= correct_positive;			-- when adding
			when "00000001" => CODE <= correct_negative;			-- when substracting
			when others => CODE <= HASHTAG;					-- else it has to be a hashtag
   		end case;
	end process;
	--****END OF MULTIPLEXOR****i-




	

end behavioral;
