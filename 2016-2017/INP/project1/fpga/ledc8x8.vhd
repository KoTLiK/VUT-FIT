-- INP Projekt 1:   Riadenie maticoveho displeja pomocou FPGA
-- Autor:           Milan Augustin (xaugus09)
-- Datum:           Oktober 2016

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
	ROW, LED        : out std_logic_vector (0 to 7);
	SMCLK, RESET    : in std_logic
);
end ledc8x8;

architecture main of ledc8x8 is
	-- pre proces spomalenia hodin
	signal ce_cnt:  std_logic_vector (7 downto 0);
	signal ce:      std_logic := '0';
	-- proces spomalujuci na 1Hz
	signal herz:    std_logic_vector (23 downto 0);
	signal switch:  std_logic := '0';

	-- rotacia bitu a dekodovanie LED
	signal row_cnt: std_logic_vector (7 downto 0);
	signal light:   std_logic_vector (7 downto 0);

begin

	ce_gen: process( SMCLK, RESET )
	begin
		if RESET = '1' then
			ce_cnt <= X"00";
			herz <= (others => '0');
		elsif SMCLK'event and SMCLK = '1' then
			ce_cnt <= ce_cnt + 1;
			herz <= herz + 1;
			if herz = X"708000" then
				switch <= not switch;
				herz <= (others => '0');
			end if ;
		end if ;
	end process ce_gen;
	-- spomalenie hodin SMCLK
	ce <= '1' when ce_cnt = X"FF" else '0';


	-- rotacia 1 bitu
	rotation: process( SMCLK, RESET, ce )
	begin
		if RESET = '1' then
			row_cnt <= X"80";
		elsif SMCLK'event and SMCLK = '1' and ce = '1' then
			row_cnt <= row_cnt(0) & row_cnt(7 downto 1);
		end if ;
	end process rotation;


	-- dekodovanie spravnych LED
	lighting : process( row_cnt, switch )
	begin
		if switch = '0' then
			case row_cnt is
				when X"80"  => light <=  "01111101";
				when X"40"  => light <=  "00111001";
				when X"20"  => light <=  "01010101";
				when X"10"  => light <=  "01101101";
				when X"08"  => light <=  "01111101";
				when X"04"  => light <=  "01111101";
				when X"02"  => light <=  "11111111";
				when X"01"  => light <=  "11111111";
				when others => light <=  "11111111";
			end case ;
		else
			case row_cnt is
				when X"80"  => light <=  "11000111";
				when X"40"  => light <=  "10111011";
				when X"20"  => light <=  "01111101";
				when X"10"  => light <=  "00000001";
				when X"08"  => light <=  "01111101";
				when X"04"  => light <=  "01111101";
				when X"02"  => light <=  "11111111";
				when X"01"  => light <=  "11111111";
				when others => light <=  "11111111";
			end case ;
		end if ;
	end process lighting;

	-- budis a bolo svetlo
	ROW <= row_cnt;
	LED <= light;

end main;
