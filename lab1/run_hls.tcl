open_project -reset mxu_conv_prj
set_top MXU
add_files src/tpu.h
add_files src/mxu.cpp
add_files -tb data/feature.dat
add_files -tb data/golden.dat
add_files -tb data/weight.dat
add_files -tb src/tb_mxu.cpp

open_solution -reset "solution1"
set_part {xczu7cg-fbvb900-2-i} -tool vivado
create_clock -period 2.5 -name default

csim_design
# Do not perform any other steps
# - The basic project will be opened in the GUI 
exit