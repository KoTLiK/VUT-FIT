-- fsm.vhd: Finite State Machine
-- Author(s): Milan Augustin
--    xaugus09 : kod1 = 2029354338   kod2 = 16234834704
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
   type t_state is (TEST1, TA_2, TA_3, TA_4, TA_5, TA_6, TA_7, TA_8, TA_9, TA_10, TB_2, TB_3, TB_4, TB_5, TB_6, TB_7, TB_8, TB_9, TB_10, TB_11, T_DONE, WRONG, PRINT_MESSAGE_FALSE, PRINT_MESSAGE_TRUE, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      next_state <= TEST1;
      if (KEY(2) = '1') then
         next_state <= TA_2;
      elsif (KEY(1) = '1') then
         next_state <= TB_2;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   when TA_2 =>
      next_state <= TA_2;
      if (KEY(0) = '1') then
         next_state <= TA_3;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TA_3 =>
      next_state <= TA_3;
      if (KEY(2) = '1') then
         next_state <= TA_4;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TA_4 =>
      next_state <= TA_4;
      if (KEY(9) = '1') then
         next_state <= TA_5;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TA_5 =>
      next_state <= TA_5;
      if (KEY(3) = '1') then
         next_state <= TA_6;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TA_6 =>
      next_state <= TA_6;
      if (KEY(5) = '1') then
         next_state <= TA_7;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TA_7 =>
      next_state <= TA_7;
      if (KEY(4) = '1') then
         next_state <= TA_8;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TA_8 =>
      next_state <= TA_8;
      if (KEY(3) = '1') then
         next_state <= TA_9;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TA_9 =>
      next_state <= TA_9;
      if (KEY(3) = '1') then
         next_state <= TA_10;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TA_10 =>
      next_state <= TA_10;
      if (KEY(8) = '1') then
         next_state <= T_DONE;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   when TB_2 =>
      next_state <= TB_2;
      if (KEY(6) = '1') then
         next_state <= TB_3;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TB_3 =>
      next_state <= TB_3;
      if (KEY(2) = '1') then
         next_state <= TB_4;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TB_4 =>
      next_state <= TB_4;
      if (KEY(3) = '1') then
         next_state <= TB_5;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TB_5 =>
      next_state <= TB_5;
      if (KEY(4) = '1') then
         next_state <= TB_6;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TB_6 =>
      next_state <= TB_6;
      if (KEY(8) = '1') then
         next_state <= TB_7;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TB_7 =>
      next_state <= TB_7;
      if (KEY(3) = '1') then
         next_state <= TB_8;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TB_8 =>
      next_state <= TB_8;
      if (KEY(4) = '1') then
         next_state <= TB_9;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TB_9 =>
      next_state <= TB_9;
      if (KEY(7) = '1') then
         next_state <= TB_10;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TB_10 =>
      next_state <= TB_10;
      if (KEY(0) = '1') then
         next_state <= TB_11;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when TB_11 =>
      next_state <= TB_11;
      if (KEY(4) = '1') then
         next_state <= T_DONE;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   -- - - - - - - - - - - - - - - - - - - - - - -
   when T_DONE =>
      next_state <= T_DONE;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_TRUE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when WRONG =>
      next_state <= WRONG;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_FALSE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG;
      end if;

   when PRINT_MESSAGE_TRUE =>
      next_state <= PRINT_MESSAGE_TRUE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;

   when PRINT_MESSAGE_FALSE =>
      next_state <= PRINT_MESSAGE_FALSE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;

   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST1;
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
   when PRINT_MESSAGE_TRUE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_MEM     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';

   when PRINT_MESSAGE_FALSE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_MEM     <= '0';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';

   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;

