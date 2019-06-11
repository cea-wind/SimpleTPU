open_project -reset relu_norm_pool_prj
set_top relu_norm_pool
add_files src/tpu.h
add_files src/relu_norm_pool.cpp
add_files -tb src/tb_pool.cpp

open_solution -reset "solution1"
set_part {xczu7cg-fbvb900-2-i} -tool vivado
create_clock -period 2.5 -name default

csim_design
# Do not perform any other steps
# - The basic project will be opened in the GUI 
exit
