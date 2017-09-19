-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Milan Augustin, xaugus09
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
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
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

   -- PC register
      signal pc_reg : std_logic_vector(11 downto 0);
      signal pc_en  : std_logic;
      signal pc_set : std_logic; -- '1' -> +=1; '0' -> -=1;
   -- CNT register
      signal cnt_reg : std_logic_vector(7 downto 0);
      signal cnt_en  : std_logic;
      signal cnt_set : std_logic; -- '1' -> +=1; '0' -> -=1;
      signal cnt_one : std_logic; -- cnt_reg <- 1;
   -- TMP register
      signal tmp_reg : std_logic_vector(7 downto 0);
      signal tmp_ld  : std_logic;
   -- PTR register
      signal ptr_reg : std_logic_vector(9 downto 0);
      signal ptr_en  : std_logic;
      signal ptr_set : std_logic; -- '1' -> +=1; '0' -> -=1;
   -- IREG register
      type instruction_type is (
         i_inc_ptr, i_dec_ptr,
         i_inc_val, i_dec_val,
         i_while_s, i_while_e,
         i_put, i_get,
         i_save, i_load,
         i_halt, i_skip
      );
      signal ireg_dec : instruction_type;
      signal ireg_ld  : std_logic;
      signal ireg_reg : std_logic_vector(7 downto 0);
   -- Multiplexor
      signal mx_sel : std_logic_vector(1 downto 0);
   -- FSM Automat
      type fsm_state is (
         idle, decode,
         fetch0, fetch1,
         inc_ptr, dec_ptr,
         inc_val, dec_val,
         inc_val2, dec_val2,
         while_s, while_s2, while_s3, while_sf0, while_sf1,
         while_e, while_e2, while_e3, while_e4, while_ef0, while_ef1,
         put, put2,
         get, get2,
         save, save2, load,
         halt, skip
      );
      signal PState : fsm_state;
      signal NState : fsm_state;

begin

   -- ----------------------------------------------------------------------------
   -- PC register
   -- ----------------------------------------------------------------------------
      PC_register : process( RESET, CLK, pc_en, pc_set )
      begin
         if (RESET = '1') then
            pc_reg <= (others => '0');
         elsif (CLK'event) and (CLK = '1') and (pc_en = '1') then
            if (pc_set = '1') then
               pc_reg <= pc_reg + 1;
            else
               pc_reg <= pc_reg - 1;
            end if ;
         end if ;
      end process PC_register;

      CODE_ADDR <= pc_reg;

   -- ----------------------------------------------------------------------------
   -- CNT register
   -- ----------------------------------------------------------------------------
      CNT_register : process( RESET, CLK, cnt_en, cnt_set, cnt_one )
      begin
         if (RESET = '1') then
            cnt_reg <= (others => '0');
         elsif (CLK'event) and (CLK = '1') and (cnt_en = '1') then
            if (cnt_one = '1') then
               cnt_reg <= X"01";
            elsif (cnt_set = '1') then
               cnt_reg <= cnt_reg + 1;
            else
               cnt_reg <= cnt_reg - 1;
            end if ;
         end if ;
      end process CNT_register;

   -- ----------------------------------------------------------------------------
   -- TMP register
   -- ----------------------------------------------------------------------------
      TMP_register : process( RESET, CLK, tmp_ld )
      begin
         if (RESET = '1') then
            tmp_reg <= (others => '0');
         elsif (CLK'event) and (CLK = '1') and (tmp_ld = '1') then
            tmp_reg <= DATA_RDATA;
         end if ;
      end process TMP_register;

   -- ----------------------------------------------------------------------------
   -- PTR register
   -- ----------------------------------------------------------------------------
      PTR_register : process( RESET, CLK, pc_en, pc_set )
      begin
         if (RESET = '1') then
            ptr_reg <= (others => '0');
         elsif (CLK'event) and (CLK = '1') and (ptr_en = '1') then
            if (ptr_set = '1') then
               ptr_reg <= ptr_reg + 1;
            else
               ptr_reg <= ptr_reg - 1;
            end if ;
         end if ;
      end process PTR_register;

      DATA_ADDR <= ptr_reg;

   -- ----------------------------------------------------------------------------
   -- IREG register
   -- ----------------------------------------------------------------------------
      IREG_register : process( RESET, CLK, ireg_ld )
      begin
         if (RESET = '1') then
            ireg_reg <= (others => '0');
         elsif (CLK'event) and (CLK = '1') and (ireg_ld = '1') then
            ireg_reg <= CODE_DATA;
         end if ;
      end process IREG_register;

   -- ----------------------------------------------------------------------------
   -- IREG decoder
   -- ----------------------------------------------------------------------------
      IREG_decoder : process( ireg_reg )
      begin
         case ireg_reg is
            when  X"3E" => ireg_dec <= i_inc_ptr; --  >
            when  X"3C" => ireg_dec <= i_dec_ptr; --  <
            when  X"2B" => ireg_dec <= i_inc_val; --  +
            when  X"2D" => ireg_dec <= i_dec_val; --  -
            when  X"5B" => ireg_dec <= i_while_s; --  [
            when  X"5D" => ireg_dec <= i_while_e; --  ]
            when  X"2E" => ireg_dec <= i_put;     --  .
            when  X"2C" => ireg_dec <= i_get;     --  ,
            when  X"24" => ireg_dec <= i_save;    --  $
            when  X"21" => ireg_dec <= i_load;    --  !
            when  X"00" => ireg_dec <= i_halt;    --  null
            when others => ireg_dec <= i_skip; -- ignorovanie zvysnych moznosti
         end case ;
      end process IREG_decoder;

   -- ----------------------------------------------------------------------------
   -- Multiplexor
   -- ----------------------------------------------------------------------------
      with mx_sel select DATA_WDATA <=
         tmp_reg         when "00",
         DATA_RDATA + 1  when "01",
         DATA_RDATA - 1  when "10",
         IN_DATA         when "11",
         X"00"           when others;

   -- ----------------------------------------------------------------------------
   -- FSM Present State
   -- ----------------------------------------------------------------------------
      FSM_Pstate : process( RESET, CLK, EN, NState )
      begin
         if (RESET = '1') then
            PState <= idle;
         elsif (CLK'event) and (CLK = '1') and (EN = '1') then
            PState <= NState;
         end if ;
      end process FSM_Pstate;

   -- ----------------------------------------------------------------------------
   -- FSM Automat
   -- ----------------------------------------------------------------------------
   FSM_Automat : process( PState, ireg_dec, DATA_RDATA, cnt_reg, OUT_BUSY, IN_VLD )
   begin
      OUT_DATA <= (others => 'Z');
      DATA_RDWR <= '0';
      IN_REQ <= '0';

      NState <= idle;
      CODE_EN <= '0';
      DATA_EN <= '0';
      OUT_WE <= '0';
      cnt_one <= '0';
      cnt_set <= '0';
      cnt_en <= '0';
      pc_set <= '0';
      pc_en <= '0';
      ptr_set <= '0';
      ptr_en <= '0';
      tmp_ld <= '0';
      ireg_ld <= '0';
      mx_sel <= "00";

      case PState is
         when idle => NState <= fetch0;
         when fetch0 =>
            NState <= fetch1;
            CODE_EN <= '1';
         when fetch1 =>
            NState <= decode;
            ireg_ld <= '1';
         when decode =>
            case ireg_dec is
               when i_inc_ptr => NState <= inc_ptr;
               when i_dec_ptr => NState <= dec_ptr;
               when i_inc_val => NState <= inc_val;
               when i_dec_val => NState <= dec_val;
               when i_while_s => NState <= while_s;
               when i_while_e => NState <= while_e;
               when i_put => NState <= put;
               when i_get => NState <= get;
               when i_save => NState <= save;
               when i_load => NState <= load;
               when i_halt => NState <= halt;
               when i_skip => NState <= skip;
               --when others =>
            end case ;

         when inc_ptr => NState <= fetch0;
            ptr_en <= '1';
            ptr_set <= '1'; -- ptr_reg++
            pc_en <= '1';
            pc_set <= '1'; -- pc_reg++
         when dec_ptr => NState <= fetch0;
            ptr_en <= '1';
            ptr_set <= '0'; -- ptr_reg--
            pc_en <= '1';
            pc_set <= '1'; -- pc_reg++

         when inc_val => NState <= inc_val2;
            DATA_EN <= '1';
            DATA_RDWR <= '1';
         when inc_val2 => NState <= fetch0;
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            mx_sel <= "01";
            pc_en <= '1';
            pc_set <= '1'; -- pc_reg++

         when dec_val => NState <= dec_val2;
            DATA_EN <= '1';
            DATA_RDWR <= '1';
         when dec_val2 => NState <= fetch0;
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            mx_sel <= "10";
            pc_en <= '1';
            pc_set <= '1'; -- pc_reg++

         when while_s => NState <= while_s2;
            pc_en <= '1';
            pc_set <= '1'; -- pc_reg++
            DATA_EN <= '1';
            DATA_RDWR <= '1';
         when while_s2 =>
            if (DATA_RDATA = X"00") then
               cnt_en <= '1';
               cnt_one <= '1';
               NState <= while_sf0;
            else
               NState <= fetch0;
            end if ;
         when while_sf0 =>
            NState <= while_sf1;
            CODE_EN <= '1';
         when while_sf1 =>
            NState <= while_s3;
            ireg_ld <= '1';
         when while_s3 =>
            if (cnt_reg /= X"00") then
               if (ireg_dec = i_while_s) then -- (ireg_reg = X"5B")
                  cnt_en <= '1';
                  cnt_set <= '1';
               elsif (ireg_dec = i_while_e) then -- (ireg_reg = X"5D")
                  cnt_en <= '1';
                  cnt_set <= '0';
               end if ;
               NState <= while_sf0;
               pc_en <= '1';
               pc_set <= '1'; -- pc_reg++
            else
               NState <= decode;
            end if ;

         when while_e => NState <= while_e2;
            DATA_EN <= '1';
            DATA_RDWR <= '1';
         when while_e2 =>
            if (DATA_RDATA = X"00") then
               pc_en <= '1';
               pc_set <= '1'; -- pc_reg++
               NState <= fetch0;
            else
               cnt_en <= '1';
               cnt_one <= '1';
               pc_en <= '1';
               pc_set <= '0'; -- pc_reg--
               NState <= while_ef0;
            end if ;
         when while_ef0 =>
            NState <= while_ef1;
            CODE_EN <= '1';
         when while_ef1 =>
            NState <= while_e3;
            ireg_ld <= '1';
         when while_e3 =>
            if (cnt_reg /= X"00") then
               if (ireg_dec = i_while_e) then -- (ireg_reg = X"5D")
                  cnt_en <= '1';
                  cnt_set <= '1';
               elsif (ireg_dec = i_while_s) then -- (ireg_reg = X"5B")
                  cnt_en <= '1';
                  cnt_set <= '0';
               end if ;
               NState <= while_e4;
            else
               NState <= decode;
            end if ;
         when while_e4 => NState <= while_ef0;
            if (cnt_reg = X"00") then
               pc_en <= '1';
               pc_set <= '1'; -- pc_reg++
            else
               pc_en <= '1';
               pc_set <= '0'; -- pc_reg--
            end if ;

         when put =>
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            if (OUT_BUSY = '1') then
               NState <= put;
            else
               NState <= put2;
            end if ;
         when put2 => NState <= fetch0;
            OUT_WE <= '1';
            OUT_DATA <= DATA_RDATA;
            pc_en <= '1';
            pc_set <= '1'; -- pc_reg++

         when get =>
            NState <= get2;
            IN_REQ <= '1';
         when get2 =>
            if (IN_VLD = '1') then
               DATA_EN <= '1';
               DATA_RDWR <= '0';
               mx_sel <= "11";
               IN_REQ <= '0';
               NState <= fetch0;
               pc_en <= '1';
               pc_set <= '1'; -- pc_reg++
            else
               NState <= get2;
            end if ;

         when save => NState <= save2;
            DATA_EN <= '1';
            DATA_RDWR <= '1';
         when save2 => NState <= fetch0;
            tmp_ld <= '1';
            pc_en <= '1';
            pc_set <= '1'; -- pc_reg++

         when load => NState <= fetch0;
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            mx_sel <= "00";
            pc_en <= '1';
            pc_set <= '1'; -- pc_reg++

         when halt => NState <= halt;
         when skip => NState <= fetch0;
            pc_en <= '1';
            pc_set <= '1'; -- pc_reg++
         when others => NState <= fetch0;
      end case ;
   end process FSM_Automat;

end behavioral;
