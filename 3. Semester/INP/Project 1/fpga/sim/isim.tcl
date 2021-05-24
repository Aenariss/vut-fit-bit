proc isim_script {} {

   add_divider "Signals of the Vigenere Interface"
   add_wave_label "" "CLK" /testbench/clk
   add_wave_label "" "RST" /testbench/rst
   add_wave_label "-radix ascii" "DATA" /testbench/tb_data
   add_wave_label "-radix ascii" "KEY" /testbench/tb_key
   add_wave_label "-radix ascii" "CODE" /testbench/tb_code

   add_divider "Vigenere Inner Signals"
   add_wave_label "-radix dec" "SHIFT_VALUE" /testbench/uut/shift
   add_wave_label "-radix ascii" "POSITIVE_RESULT" /testbench/uut/correct_positive
   add_wave_label "-radix ascii" "NEGATIVE_RESULT" /testbench/uut/correct_negative
   add_wave_label "" "CURRENT_STATE" /testbench/uut/current_state
   add_wave_label "" "NEXT_STATE" /testbench/uut/next_state
   add_wave_label "-radix dec" "MEALY_RESULT" /testbench/uut/mealy_out
   # sem doplnte vase vnitrni signaly. chcete-li v diagramu zobrazit desitkove
   # cislo, vlozte do prvnich uvozovek: -radix dec

   run 8 ns
}
