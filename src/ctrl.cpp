#include "tpu.h"

void loadWeight(ap_uint<256> *ddr,WEIGHTDTYPE weight_buffer[512][MXU_COLNUM],
		unsigned offset,short addr, short len, bool enable){
	if(!enable)
		return;
	for(int i=0;i<len;i++){
#pragma HLS PIPELINE
		ap_uint<256> tmp = ddr[offset+i];
		for(int j=0;j<32;j++){
			weight_buffer[addr+i][j] = tmp(j*8+7,j*8);
		}
	}
}

void loadFeature(ap_uint<256> *ddr,FEATDTYPE unified_buffer[512][MXU_ROWNUM],
		unsigned offset,short addr, short len, bool enable){
	if(!enable)
		return;
	for(int i=0;i<len;i++){
#pragma HLS PIPELINE
		ap_uint<256> tmp = ddr[offset+i];
		for(int j=0;j<32;j++){
			unified_buffer[addr+i][j] = tmp(j*8+7,j*8);
		}
	}
}
void storeFeature(ap_uint<256> *ddr,FEATDTYPE unified_buffer[512][MXU_COLNUM],
		unsigned offset,short addr, short len, bool enable){
	if(!enable)
		return;
	for(int i=0;i<len;i++){
#pragma HLS PIPELINE
		ap_uint<256> tmp;
		for(int j=0;j<32;j++){
			tmp(j*8+7,j*8) = unified_buffer[addr+i][j];;
		}
		ddr[offset+i] = tmp;
	}
}
//set instr. set register
//run instr. run process
//eop instr. end of process
//

void instr(ap_uint<64> *ddr,unsigned &offset,ap_int<16> reggroup[96],ap_int<8> &runmode,bool enable){
#pragma HLS INTERFACE m_axi depth=8192 port=ddr
#pragma HLS ARRAY_PARTITION variable=reggroup complete dim=1
	if(!enable)
		return;
	bool isRunInstr = false;
	while(!isRunInstr){
		ap_uint<64> tmp = ddr[offset];
		offset++;
		if(tmp[63]==0){
			switch(tmp(52,48)){
			case( 0):reggroup[ 0] = tmp(15, 0);reggroup[ 1] = tmp(31,16);reggroup[ 2] = tmp(47,32);break;
			case( 1):reggroup[ 3] = tmp(15, 0);reggroup[ 4] = tmp(31,16);reggroup[ 5] = tmp(47,32);break;
			case( 2):reggroup[ 6] = tmp(15, 0);reggroup[ 7] = tmp(31,16);reggroup[ 8] = tmp(47,32);break;
			case( 3):reggroup[ 9] = tmp(15, 0);reggroup[10] = tmp(31,16);reggroup[11] = tmp(47,32);break;
			case( 4):reggroup[12] = tmp(15, 0);reggroup[13] = tmp(31,16);reggroup[14] = tmp(47,32);break;
			case( 5):reggroup[15] = tmp(15, 0);reggroup[16] = tmp(31,16);reggroup[17] = tmp(47,32);break;
			case( 6):reggroup[18] = tmp(15, 0);reggroup[19] = tmp(31,16);reggroup[20] = tmp(47,32);break;
			case( 7):reggroup[21] = tmp(15, 0);reggroup[22] = tmp(31,16);reggroup[23] = tmp(47,32);break;
			case( 8):reggroup[24] = tmp(15, 0);reggroup[25] = tmp(31,16);reggroup[26] = tmp(47,32);break;
			case( 9):reggroup[27] = tmp(15, 0);reggroup[28] = tmp(31,16);reggroup[29] = tmp(47,32);break;
			case(10):reggroup[30] = tmp(15, 0);reggroup[31] = tmp(31,16);reggroup[32] = tmp(47,32);break;
			case(11):reggroup[33] = tmp(15, 0);reggroup[34] = tmp(31,16);reggroup[35] = tmp(47,32);break;
			case(12):reggroup[36] = tmp(15, 0);reggroup[37] = tmp(31,16);reggroup[38] = tmp(47,32);break;
			case(13):reggroup[39] = tmp(15, 0);reggroup[40] = tmp(31,16);reggroup[41] = tmp(47,32);break;
			case(14):reggroup[42] = tmp(15, 0);reggroup[43] = tmp(31,16);reggroup[44] = tmp(47,32);break;
			case(15):reggroup[45] = tmp(15, 0);reggroup[46] = tmp(31,16);reggroup[47] = tmp(47,32);break;
			case(16):reggroup[48] = tmp(15, 0);reggroup[49] = tmp(31,16);reggroup[50] = tmp(47,32);break;
			case(17):reggroup[51] = tmp(15, 0);reggroup[52] = tmp(31,16);reggroup[53] = tmp(47,32);break;
			case(18):reggroup[54] = tmp(15, 0);reggroup[55] = tmp(31,16);reggroup[56] = tmp(47,32);break;
			case(19):reggroup[57] = tmp(15, 0);reggroup[58] = tmp(31,16);reggroup[59] = tmp(47,32);break;
			case(20):reggroup[60] = tmp(15, 0);reggroup[61] = tmp(31,16);reggroup[62] = tmp(47,32);break;
			case(21):reggroup[63] = tmp(15, 0);reggroup[64] = tmp(31,16);reggroup[65] = tmp(47,32);break;
			case(22):reggroup[66] = tmp(15, 0);reggroup[67] = tmp(31,16);reggroup[68] = tmp(47,32);break;
			case(23):reggroup[69] = tmp(15, 0);reggroup[70] = tmp(31,16);reggroup[71] = tmp(47,32);break;
			case(24):reggroup[72] = tmp(15, 0);reggroup[73] = tmp(31,16);reggroup[74] = tmp(47,32);break;
			case(25):reggroup[75] = tmp(15, 0);reggroup[76] = tmp(31,16);reggroup[77] = tmp(47,32);break;
			case(26):reggroup[78] = tmp(15, 0);reggroup[79] = tmp(31,16);reggroup[80] = tmp(47,32);break;
			case(27):reggroup[81] = tmp(15, 0);reggroup[82] = tmp(31,16);reggroup[83] = tmp(47,32);break;
			case(28):reggroup[84] = tmp(15, 0);reggroup[85] = tmp(31,16);reggroup[86] = tmp(47,32);break;
			case(29):reggroup[87] = tmp(15, 0);reggroup[88] = tmp(31,16);reggroup[89] = tmp(47,32);break;
			case(30):reggroup[90] = tmp(15, 0);reggroup[91] = tmp(31,16);reggroup[92] = tmp(47,32);break;
			case(31):reggroup[93] = tmp(15, 0);reggroup[94] = tmp(31,16);reggroup[95] = tmp(47,32);break;
			}
		}
		else{
			runmode = tmp(55,48);
			isRunInstr = true;
		}
	}
}


void config(ap_int<16> reggroup[96],MXU_PARAM &mxuparam,RELPOOL_PARAM &poolparam,LDST_PARAM &lsdtparam, ap_int<32> norm_coef[32]){
#pragma HLS INLINE
    mxuparam.isload          = reggroup[ 0].range(0,0);
	mxuparam.iscalc          = reggroup[ 0].range(1,1);
	mxuparam.isping          = reggroup[ 0].range(2,2);
	mxuparam.isfirstpsum     = reggroup[ 0].range(3,3);
	mxuparam.weight_raddr    = reggroup[ 1];
	mxuparam.ubuf_raddr_start= reggroup[ 2];
	mxuparam.ubuf_raddr_step1= reggroup[ 3];
	mxuparam.ubuf_raddr_step2= reggroup[ 4];
	mxuparam.ubuf_raddr_step3= reggroup[ 5];
	mxuparam.ubuf_raddr_end1 = reggroup[ 6];
	mxuparam.ubuf_raddr_end2 = reggroup[ 7];
	mxuparam.ubuf_raddr_end3 = reggroup[ 8];
	mxuparam.ubuf_raddr_num  = reggroup[ 9];
	mxuparam.psum_start      = reggroup[10];
	mxuparam.psum_step1      = reggroup[11];
	mxuparam.psum_end1       = reggroup[12];
	mxuparam.psum_step2      = reggroup[13];

	poolparam.isrelu    = reggroup[14].range( 0,0);
	poolparam.maxpool   = reggroup[14].range( 1,1);
	poolparam.avg_shift = reggroup[14].range( 7,4);
	poolparam.pool_kw   = reggroup[14].range(15,8);
	poolparam.pool_kh   = reggroup[15].range( 7,0);
	poolparam.pool_w    = reggroup[15].range(15,8);
	poolparam.pool_sw   = reggroup[16].range( 7,0);
	poolparam.pool_sh   = reggroup[16].range(15,8);
	poolparam.psum_raddr_start = reggroup[17];
	poolparam.pool_cnt         = reggroup[18];
	poolparam.pool_h_step      = reggroup[19];
	poolparam.avg_val          = reggroup[20];
	poolparam.ubuf_waddr_start = reggroup[21];
	poolparam.ubuf_waddr_step1 = reggroup[22];
	poolparam.ubuf_waddr_step2 = reggroup[23];
	poolparam.ubuf_waddr_step3 = reggroup[24];
	poolparam.ubuf_waddr_end1  = reggroup[25];
	poolparam.ubuf_waddr_end2  = reggroup[26];
	poolparam.ubuf_waddr_end3  = reggroup[27];

	lsdtparam.weight_addr = reggroup[28];
	lsdtparam.weight_ldlen = reggroup[29];
	ap_uint<32> tmp = (reggroup[31],reggroup[30]);
	lsdtparam.weight_offset = tmp;
	for(int i=0;i<32;i++){
#pragma HLS UNROLL
		norm_coef[i] = (reggroup[33+2*i],reggroup[32+2*i]);
	}
	return;
}
