
#include "tpu.h"

void ex_module(FEATDTYPE unified_buffer[16384][MXU_ROWNUM],WEIGHTDTYPE weight_buffer[512][MXU_COLNUM],
		ap_int<32> norm_coef[MXU_COLNUM],MXU_PARAM mxuparam,RELPOOL_PARAM poolparam,
		bool is_MXU,bool is_relu_norm_pool){
#pragma HLS INLINE off
#pragma HLS DEPENDENCE variable=unified_buffer inter false
#pragma HLS DEPENDENCE variable=unified_buffer intra false
	static PSUMDTYPE psum_buffer1[512][MXU_COLNUM];
	static PSUMDTYPE psum_buffer2[512][MXU_COLNUM];
#pragma HLS ARRAY_PARTITION variable=psum_buffer1 complete dim=2
#pragma HLS ARRAY_PARTITION variable=psum_buffer2 complete dim=2
	if((is_MXU&&mxuparam.psum_start<512) || (is_relu_norm_pool&&poolparam.psum_raddr_start>=512) )
	{
		MXU(unified_buffer,weight_buffer,psum_buffer1,mxuparam,is_MXU);
		relu_norm_pool(psum_buffer2,unified_buffer,norm_coef,poolparam,is_relu_norm_pool);
	}
	else{
		MXU(unified_buffer,weight_buffer,psum_buffer2,mxuparam,is_MXU);
		relu_norm_pool(psum_buffer1,unified_buffer,norm_coef,poolparam,is_relu_norm_pool);
	}

}

void tpu(ap_uint<256> *ddr,ap_uint<64> *ddr_instr){
#pragma HLS INTERFACE m_axi depth=16384 port=ddr
#pragma HLS INTERFACE m_axi depth=3300 port=ddr_instr

	static FEATDTYPE unified_buffer[16384][MXU_ROWNUM];
#pragma HLS RESOURCE variable=unified_buffer core=RAM_S2P_BRAM
	static WEIGHTDTYPE weight_buffer[512][MXU_COLNUM];
#pragma HLS RESOURCE variable=weight_buffer core=RAM_S2P_BRAM
	static ap_int<32> norm_coef[MXU_COLNUM];
#pragma HLS ARRAY_PARTITION variable=unified_buffer complete dim=2
#pragma HLS ARRAY_PARTITION variable=weight_buffer complete dim=2
#pragma HLS ARRAY_PARTITION variable=norm_coef complete dim=0

	ap_int<16> reggroup[96];
#pragma HLS ARRAY_PARTITION variable=reggroup complete dim=0
	MXU_PARAM mxuparam;
	RELPOOL_PARAM poolparam;
	LDST_PARAM ldstparam;
	unsigned instr_offset = 0;
	bool is_load_weight;
	bool is_MXU;
	bool is_relu_norm_pool;
	// load img
	loadFeature(ddr,unified_buffer, 0,0, 512*25, true);
	bool eop = false;
	ap_int<8> runmode = 0;	//0 nop, bit[0] loadweight;bit[1] mxu; bit[2] pool; bit[7] eop;
	instr(ddr_instr,instr_offset,reggroup,runmode,true);
	while(runmode[7]==0)
	{
#pragma HLS DEPENDENCE variable=unified_buffer inter false
#pragma HLS DEPENDENCE variable=unified_buffer intra false
#pragma HLS DEPENDENCE variable=weight_buffer inter false
#pragma HLS DEPENDENCE variable=weight_buffer intra false

		config(reggroup,mxuparam,poolparam,ldstparam,norm_coef);
		is_load_weight = runmode[0]==1;
		is_MXU = runmode[1]==1;
		is_relu_norm_pool = runmode[2]==1;
		instr(ddr_instr,instr_offset,reggroup,runmode,true);
		loadWeight(ddr,weight_buffer,ldstparam.weight_offset,ldstparam.weight_addr,
					ldstparam.weight_ldlen,is_load_weight);
		ex_module(unified_buffer,weight_buffer,norm_coef,mxuparam,poolparam,is_MXU,is_relu_norm_pool);
	}

	storeFeature(ddr,unified_buffer, 512*25+72*25+72,14000, 512, true);
}
