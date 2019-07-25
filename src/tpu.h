#include "ap_int.h"

#define MXU_COLNUM 32
#define MXU_ROWNUM 32
#define WEIGHTDTYPE char
#define FEATDTYPE char
#define PSUMDTYPE ap_int<32>


struct MXU_PARAM{
	bool isload;
	bool iscalc;
	bool isping;
	bool isfirstpsum;

	short weight_raddr;
	short ubuf_raddr_start;
	short ubuf_raddr_step1;
	short ubuf_raddr_step2;
	short ubuf_raddr_step3;
	short ubuf_raddr_end1;
	short ubuf_raddr_end2;
	short ubuf_raddr_end3;
	short ubuf_raddr_num;
	short psum_start;
	short psum_step1;
	short psum_end1;
	short psum_step2;
};
struct RELPOOL_PARAM{
	bool isrelu;
	short psum_raddr_start;

	bool maxpool; // max pool or average pool
	char pool_kw;
	char pool_kh;
	char pool_w;
	char pool_sw;
	char pool_sh;
	short pool_cnt; // output_num*pool_kw*pool_kh
	short pool_h_step;

	short avg_val;
	ap_uint<4> avg_shift;

	short ubuf_waddr_start;
	short ubuf_waddr_step1;
	short ubuf_waddr_step2;
	short ubuf_waddr_step3;
	short ubuf_waddr_end1;
	short ubuf_waddr_end2;
	short ubuf_waddr_end3;
};

struct LDST_PARAM{
	unsigned weight_offset;
	short weight_addr;
	short weight_ldlen;
};

void MXU(FEATDTYPE ubuf[16384][MXU_ROWNUM],WEIGHTDTYPE weight[512][MXU_COLNUM],
		PSUMDTYPE psum[512][MXU_COLNUM],MXU_PARAM mxuparam, bool enable);
void relu_norm_pool(PSUMDTYPE psum_buffer[512][MXU_COLNUM],FEATDTYPE unified_buffer[16384][MXU_ROWNUM],
		ap_int<32> norm_coef[MXU_COLNUM],RELPOOL_PARAM param, bool enable);
void loadWeight(ap_uint<256> *ddr,WEIGHTDTYPE weight_buffer[512][MXU_COLNUM],
		unsigned offset,short addr, short len, bool enable);
void loadFeature(ap_uint<256> *ddr,FEATDTYPE unified_buffer[512][MXU_ROWNUM],
		unsigned offset,short addr, short len, bool enable);
void storeFeature(ap_uint<256> *ddr,FEATDTYPE unified_buffer[512][MXU_COLNUM],
		unsigned offset,short addr, short len, bool enable);
void instr(ap_uint<64> *ddr,unsigned &offset,ap_int<16> reggroup[96],ap_int<8> &runmode,bool enable);
void config(ap_int<16> reggroup[96],MXU_PARAM &mxuparam,RELPOOL_PARAM &poolparam,
		LDST_PARAM &lsdtparam, ap_int<32> norm_coef[32]);
void tpu(ap_uint<256> *ddr,ap_uint<64> *ddr_instr);
