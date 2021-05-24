-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST_MAIN, TEST_BAD, TEST_GOOD, TEST1_20, TEST1_2, TEST1_202, TEST1_2024, TEST1_20240, TEST1_202407, TEST1_2024074, TEST1_20240746, TEST1_202407465, TEST2_20241, TEST2_202413, TEST2_2024130, TEST2_20241306, TEST2_202413064, PRINT_BAD, PRINT_GOOD, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST_MAIN;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_MAIN =>
      next_state <= TEST_MAIN;
		if (KEY(2) = '1') then
         next_state <= TEST1_2;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1_20 =>
      next_state <= TEST1_20;
		if (KEY(2) = '1') then
         next_state <= TEST1_202;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1_2 =>
      next_state <= TEST1_2;
		if (KEY(0) = '1') then
         next_state <= TEST1_20;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1_202 =>
      next_state <= TEST1_202;
		if (KEY(4) = '1') then
         next_state <= TEST1_2024;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1_2024 =>
      next_state <= TEST1_2024;
		if (KEY(0) = '1') then
         next_state <= TEST1_20240;
		elsif (KEY(1) = '1') then
         next_state <= TEST2_20241;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1_20240 =>
      next_state <= TEST1_20240;
		if (KEY(7) = '1') then
         next_state <= TEST1_202407;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2_20241 =>
      next_state <= TEST2_20241;
		if (KEY(3) = '1') then
         next_state <= TEST2_202413;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2_202413 =>
      next_state <= TEST2_202413;
		if (KEY(0) = '1') then
         next_state <= TEST2_2024130;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2_2024130 =>
      next_state <= TEST2_2024130;
		if (KEY(6) = '1') then
         next_state <= TEST2_20241306;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2_20241306 =>
      next_state <= TEST2_20241306;
		if (KEY(4) = '1') then
         next_state <= TEST2_202413064;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2_202413064 =>
      next_state <= TEST2_202413064;
		if (KEY(8) = '1') then
         next_state <= TEST_GOOD;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1_202407 =>
      next_state <= TEST1_202407;
		if (KEY(4) = '1') then
         next_state <= TEST1_2024074;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1_2024074 =>
      next_state <= TEST1_2024074;
		if (KEY(6) = '1') then
         next_state <= TEST1_20240746;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1_20240746 =>
      next_state <= TEST1_20240746;
		if (KEY(5) = '1') then
         next_state <= TEST1_202407465;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1_202407465 =>
      next_state <= TEST1_202407465;
		if (KEY(6) = '1') then
         next_state <= TEST_GOOD;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_BAD =>
      next_state <= TEST_BAD;
      if (KEY(15) = '1') then
         next_state <= PRINT_BAD; 
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_GOOD =>
      next_state <= TEST_GOOD;
      if (KEY(15) = '1') then
         next_state <= PRINT_GOOD; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_BAD;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_BAD =>
      next_state <= PRINT_BAD;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when PRINT_GOOD =>
      next_state <= PRINT_GOOD ;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST_MAIN; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST_MAIN;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_MAIN | TEST_BAD | TEST_GOOD | TEST1_20 | TEST1_2 | TEST1_202 | TEST1_2024 | TEST1_20240 | TEST1_202407 | TEST1_2024074 | TEST1_20240746 | TEST1_202407465 | TEST2_20241 | TEST2_202413 | TEST2_2024130 | TEST2_20241306 | TEST2_202413064  =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_BAD =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
	when PRINT_GOOD =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   end case;
end process output_logic;

end architecture behavioral;

