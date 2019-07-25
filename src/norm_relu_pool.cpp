
#include "tpu.h"

void relu_norm_pool(PSUMDTYPE psum_buffer[512][MXU_COLNUM],FEATDTYPE unified_buffer[16384][MXU_ROWNUM],
		ap_int<32> norm_coef[MXU_COLNUM],RELPOOL_PARAM param, bool enable){
//#pragma HLS INTERFACE bram port=unified_buffer
//#pragma HLS INTERFACE bram port=psum_buffer
//#pragma HLS ARRAY_PARTITION variable=norm_coef complete dim=1
//#pragma HLS ARRAY_PARTITION variable=unified_buffer complete dim=2
//#pragma HLS ARRAY_PARTITION variable=psum_buffer complete dim=2

	PSUMDTYPE psumreg[MXU_COLNUM];
	PSUMDTYPE psumrelu[MXU_COLNUM];
	PSUMDTYPE psumpool[MXU_COLNUM];
	FEATDTYPE relu[MXU_COLNUM];
	short pool[MXU_COLNUM];
#pragma HLS ARRAY_PARTITION variable=psumreg complete dim=1
#pragma HLS ARRAY_PARTITION variable=psumsht complete dim=1
#pragma HLS ARRAY_PARTITION variable=relu complete dim=1
#pragma HLS ARRAY_PARTITION variable=pool complete dim=1

	char pool_kw_cnt = 0;
	char pool_kh_cnt = 0;
	char pool_w_cnt = 0;
	char pool_h_cnt = 0;
	short ubuf_waddr_p1=0;
	short ubuf_waddr_p2=0;
	short ubuf_waddr_p3=0;

	if(!enable)
		return;
	for(short i=0;i<param.pool_cnt;i++){
#pragma HLS PIPELINE

		short raddr = param.psum_raddr_start%512 + (pool_h_cnt+pool_kh_cnt)*param.pool_h_step
				+ (pool_w_cnt+pool_kw_cnt);
		for(int j=0;j<MXU_COLNUM;j++){
			psumreg[j] = psum_buffer[raddr][j];
			if(psumreg[j]<0&&param.isrelu)
				psumrelu[j] = 0;
			else
				psumrelu[j] = psumreg[j];
			if(pool_kw_cnt==0&&pool_kh_cnt==0){
				psumpool[j] = psumrelu[j];
			}
			else if(param.maxpool){
				if(psumrelu[j]>psumpool[j])
					psumpool[j] = psumrelu[j];
			}
			else{
				psumpool[j] = psumpool[j] + psumrelu[j];
			}
		}

		if(pool_kw_cnt==param.pool_kw&&pool_kh_cnt==param.pool_kh){
			short ubuf_waddr = param.ubuf_waddr_start + ubuf_waddr_p1 + ubuf_waddr_p2 + ubuf_waddr_p3;
			if(ubuf_waddr_p1==param.ubuf_waddr_end1){
				if(ubuf_waddr_p2==param.ubuf_waddr_end2){
					ubuf_waddr_p2 = 0;
					ubuf_waddr_p3 = ubuf_waddr_p3 + param.ubuf_waddr_step3;
				}
				else{
		            ubuf_waddr_p2 = ubuf_waddr_p2 +  param.ubuf_waddr_step2;
				}
			}
			else{
				ubuf_waddr_p1 = ubuf_waddr_p1 + param.ubuf_waddr_step1;
			}
			for(int j=0;j<MXU_COLNUM;j++){
				long tmp;
				tmp = long(psumpool[j])*long(norm_coef[j]);
				int tmpcut = tmp>>32;
				ap_int<8> res;
				if(tmpcut>127)
					res = 127;
				else if(tmpcut<-128)
					res = -128;
				else
					res = tmpcut;
				unified_buffer[ubuf_waddr][j] = res;
			}
		}

		if(pool_kw_cnt==param.pool_kw){
			pool_kw_cnt = 0;
			if(pool_kh_cnt==param.pool_kh){
				pool_kh_cnt = 0;
				if(pool_w_cnt==param.pool_w){
					pool_w_cnt = 0;
					pool_h_cnt = pool_h_cnt + param.pool_sh;
				}
				else{
					pool_w_cnt = pool_w_cnt + param.pool_sw;
				}
			}
			else{
				pool_kh_cnt = pool_kh_cnt + 1;
			}
		}
		else{
			pool_kw_cnt = pool_kw_cnt + 1;
		}
	}
}
