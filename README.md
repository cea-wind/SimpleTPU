# SimpleTPU

A Tensor Processing Unit is designed to accelerate the matrix multiplication, especially for Multilayer perceptron and Convolution Nerual Network. Mainly based on the Google TPU Version 1, which architecture is introduced in [https://arxiv.org/ftp/arxiv/papers/1704/1704.04760.pdf](https://arxiv.org/ftp/arxiv/papers/1704/1704.04760.pdf "In-Datacenter Performance Analysis of a Tensor Processing Unit").

It may cost a lot of time to implementation TPU by using Hardware Description Language, even if I had tried to simplify it. So I try to use the Xilinx HLS ToolKit to complete it. 

The plan is divided into three phases.

- Phase 1: Completing the main computing module.
- Phase 2: Finish the full design of simpleTPU.
- Phase 3: Testing the simpleTPU through some real network, including as MLP and CNN.

Phase one is complete, including two basic module

- Lab1:Systolic Array
- Lab2:Relu, Normalization & Pooling 


# Relative Link  
https://www.cnblogs.com/sea-wind/p/10993958.html