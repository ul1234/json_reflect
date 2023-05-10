/*************************************************************************************************
*
* @file          nr_ctrl_dsp_pucch_msg.h
*
* @brief         NR主控和DSP之间的PUCCH接口定义
*
*************************************************************************************************/
#ifndef NR_CTRL_DSP_PUCCH_MSG_H
#define NR_CTRL_DSP_PUCCH_MSG_H

#include "typedef.h"
#include "nr_common.h"

/****************************************** MACROS **********************************************/
#define MAX_OCC_NUM_FMT1        (7)
#define MAX_USER_NUM_PER_OCC    (6)
#define HOP_NUM                 (2)
#define MAX_PUCCH_FMT0_1_NUM    (64)
#define MAX_PUCCH_FMT2_3_NUM    (38)
#define MAX_PUCCH_NUM           (MAX_PUCCH_FMT0_1_NUM + MAX_PUCCH_FMT2_3_NUM)
#define PUC_FMT3_MAX_DMRS_NUM   (4)

/******************************************** TYPES *********************************************/
enum PUCCH_FORMAT_ENUM
{
    PUCCH_FORMAT_0 = 0,
    PUCCH_FORMAT_1 = 1,
    PUCCH_FORMAT_2 = 2,
    PUCCH_FORMAT_3 = 3,
    PUCCH_FORMAT_BUTT
};
typedef uint8_t PUCCH_FORMAT;

enum PUCCH_EQ_SCHEME_ENUM
{
    PUCCH_EQ_MRC = 0,
    PUCCH_EQ_IRC = 1,
    PUCCH_EQ_DYNAMIC = 2
};
typedef uint8_t PUCCH_EQ_SCHEME;

typedef struct
{
    uint8_t pduIdxInner;    /* 物理层内部使用的每个UE的索引 */
    uint8_t rsvd[3];
    uint16_t mcsBitMap;     /* ARM发送需要检测的MCS值给DSP，通过bitmap指示，bit0对应mcs=0，依次类推，bit0-bit11有效 */
    uint8_t cyclicShift[NR_SYM_NUM_PER_SLOT];  /* 38.211协议 6.3.2.2.2计算得到的各符号α值，取值[0,11] */
}PucFmt0Param;

typedef struct
{
    uint8_t pduIdxInner;    /* 物理层内部使用的每个UE的索引 */
    uint8_t rsvd;
    uint8_t m0;             /* initial cyclic shift */
    uint8_t uciBitNum;      /* UCI的payload的bit数,取值[1,2] */
}Fmt1UEParam;

typedef struct
{
    uint16_t ueTapBitMap;       /* 所有ue的alpha值(0-11)的bitmap，如alpha为3，那么bit3设为1; alpha = ((NR_SC_NUM_PER_RB - m0 ) mod NR_SC_NUM_PER_RB) */
    uint8_t rsvd[2];
    uint8_t timeDomainOccIdx;   /* 时域OCC的索引值，取值[0,6] */
    uint8_t userNumPerOcc;      /* 每个OCC上复用的用户数，协议规定最多12个，实际最多6个，取值[1,6] */
    Fmt1UEParam fmt1UEParam[MAX_USER_NUM_PER_OCC];/* 当前OCC上fmt1 UE参数 */
}Fmt1ParamOcc;

typedef struct
{
    uint8_t cyclicShift[NR_SYM_NUM_PER_SLOT];   /* 38.211协议 6.3.2.2.2计算得到的各符号α值，取值[0,11]，注意不用加m0和mcs,数组内符号索引为fmt1所占符号 */
    uint8_t occNum;         /* occ数目 */
    uint8_t minUserOccIdx;  /* 复用用户数最少的OCC在数组中的索引,如果多个OCC上的用户数相同，取最小occ index的OCC在数组中的索引 */
    Fmt1ParamOcc fmt1ParamOcc[MAX_OCC_NUM_FMT1];/* fmt1 OCC级参数 */
}PucFmt1Param;

typedef struct
{
    uint8_t pduIdxInner;        /* 物理层内部使用的每个UE的索引 */
    uint8_t rsvd;
    uint16_t bitNum;            /* SR&HARQ&CSI part1三者bit数之和 */
    uint32_t *scrambSeqAddr;    /* 加扰序列在DDR中的存放地址 */
    uint32_t *llrAddr;          /* DSP处理完的LLR数据的存放地址，供HAC取数进行译码 */
}PucFmt2Param;

typedef struct
{
    uint16_t llrStart;       /* 某符号中，LLR在此PUCCH format3所占RE中的起始位置 */
    uint16_t llrDuration;    /* LLR持续数目，即所占连续RE数目 */
}Fmt3UciLlrPos;

typedef struct
{
    uint8_t pduIdxInner;        /* 物理层内部使用的每个UE的索引 */
    bool    pi2bpsk;            /* pi/2-BPSK是否使能标志，取值0:不使能，使用QPSK，1:使能，使用pi/2-BPSK */
    bool    addDmrsEnable;      /* 附加导频是否使能标志，取值0:不使能，1:使能 */
    uint8_t rsvd1;
    uint8_t dmrsSymIdx[HOP_NUM][PUC_FMT3_MAX_DMRS_NUM];
    uint8_t cyclicShift[HOP_NUM][PUC_FMT3_MAX_DMRS_NUM];    /* 38.211协议 6.3.2.2.2计算得到的各符号α值 */

    uint8_t rsvd2;
    bool    uciPart2Enable;
    uint8_t part1BlockNum;
    uint8_t part2BlockNum;
    Fmt3UciLlrPos part1LlrPos[NR_MAX_PUCCH_3_BLOCK_NUM];
    Fmt3UciLlrPos part2LlrPos[NR_MAX_PUCCH_3_BLOCK_NUM];

    uint32_t *scrambSeqAddr;    /* 加扰序列在DDR中的存放地址 */
    uint32_t *llrAddr[2];       /* DSP处理完的LLR数据的存放地址，供HAC取数进行译码，2代表2part */
}PucFmt3Param;

typedef struct
{
    /* frequency domain */
    uint16_t prbStart;      /* PRB起始索引，取值[0,274] */
    uint8_t prbSize;       /* PRB个数，取值[1,16] */

    /* time domain */
    uint8_t startSymIdx;    /* PUC所占符号中起始符号索引，取值[0,13] */
    uint8_t symNum;         /* 本跳符号个数，fmt0/2取值[1,2]，fmt1/3/4取值[4,14] */
    uint8_t dmrsSymNum;     /* 导频符号个数,取值[1-7] */
    uint8_t uciSymNum;      /* 数据符号个数,取值[1-7] */

    uint8_t contSymNum;     /* 内存连续的符号个数, 如果contSymNum < symNum, 则前contSymNum内存连续, 后(symNum-contSymNum)内存连续 */

    /* PUC data在DDR中的存放地址 */
    uint32_t *dataStartAddr[2];     /* 内存首地址, 如果 contSymNum < symNum, 则有2个首地址, 否则只有1个首地址 */

    /* ZC基序列或PN序列在DDR中的存放地址:
       fmt0数据/fmt1数据和导频/fmt3导频使用ZC序列,某hop内,各符号天线上的ZC基序列相同;
       fmt2导频使用PN序列,2sym时,不管是否跳频，将2sym的PN序列地址连续摆放 */
    uint32_t *baseSeqAddr;
}PucHopParam;

typedef union{
    PucFmt0Param fmt0Param;
    PucFmt1Param fmt1Param;
    PucFmt2Param fmt2Param;
    PucFmt3Param fmt3Param;
} PucFmtUnion;

typedef struct
{
    PUCCH_FORMAT pucFormat;         /* PUCCH格式，取值0/1/2/3，暂不支持格式 4 */
    uint8_t rxAntNum;               /* 基站接收天线个数，取值2/4 */
    uint8_t rsvd;

    uint8_t hopNum;
    PucHopParam hopParam[HOP_NUM];  /* 不跳频时，参数在数组0 */

    /* 各format需要的参数 */
    PucFmtUnion formatxParam;
}PucParam;

typedef struct
{
    uint8_t deltaOffset;    /* 算法参数，抽头offset，目前固定配置为0 */ //提取出来放到CELL级，ARM确定下放哪儿
    uint8_t noiseTapNum;    /* 算法参数，参与噪声估计的时域抽头数，取值3或6 */
    uint8_t rsvd[2];
    uint32_t threshold;     /* 算法参数，DTX检测门限，取值范围待定 */
}PucFmt0AlgoParam;

typedef struct
{
    uint8_t noiseTapNum;    /* 算法参数，参与噪声估计的时域抽头数，取值3或6; 目前算法定为6,不需要此参数 */
    uint8_t rsvd[3];
    uint32_t threshold;     /* 算法参数，DTX检测门限，取值范围待定 */
}PucFmt1AlgoParam;

typedef struct
{
    PUCCH_EQ_SCHEME MrcIrcFlag; /* 算法参数，MRC/IRC模式，取值0:MRC,1:IRC,2: MRC/IRC自适应 */
    uint8_t rsvd;
    uint16_t epsilon;       /* 算法参数，MRC/IRC切换门限系数，默认值是3，定点为768，(0,16,8) */
    uint16_t beta;          /* 算法参数，对角加载系数，浮点为0.0005，定点为0x0083，(0,16,-2) */
    uint16_t segNum;        /* 算法参数，RB个数按照粒度Ruu Granularity分为seg的个数，取值范围待定 */
    uint32_t threshold;     /* 算法参数，DTX检测门限，取值范围待定 */
}PucFmt23AlgoParam;

typedef struct
{
    uint8_t pucchNum;       /* PUCCH总个数 */
    uint8_t rsvd[3];

    uint8_t fmt0Num;
    uint8_t fmt1Num;
    uint8_t fmt2Num;
    uint8_t fmt3Num;

    PucFmt0AlgoParam fmt0AlgoParam;
    PucFmt1AlgoParam fmt1AlgoParam;
    PucFmt23AlgoParam fmt2AlgoParam;
    PucFmt23AlgoParam fmt3AlgoParam;

    PucParam pucParam[MAX_PUCCH_NUM];   /* 所有PUCCH的参数 */
}PucchPara;

typedef struct
{
    uint8_t pduIdxInner;
    uint8_t mcsValue;       /* DSP检出的MCS的值 */
    uint8_t rsvd;
    bool    isDtx;          /* DTX状态：1：DTX; 0:非DTX */
    int32_t snr;            /* SNR，线性值，由ARM转成FAPI需要的单位 */
}PucchFmt0Rpt;

typedef struct
{
    uint8_t pduIdxInner;
    uint8_t rsvd[3];
    uint8_t uciBitNum;          /* UCI比特个数，取值1或2 */
    uint8_t uciDecodeValue[2];  /* UCI解调结果, 每个bit保存到uint8_t中 */
    bool    isDtx;              /* DTX状态：1：DTX; 0:非DTX */
    int32_t snr;                /* SNR，线性值，由ARM转成FAPI需要的单位 */
}PucchFmt1Rpt;

typedef struct
{
    uint8_t pduIdxInner;
    uint8_t rsvd[2];
    bool    isDtxM;     /* DSP计算的DTXm状态：1：DTX; 0:非DTX */
    int32_t snr;        /* SNR，线性值，由ARM转成FAPI需要的单位  */
}PucchFmt23Rpt;

typedef struct
{
    uint8_t pucchFmt0Num;
    uint8_t pucchFmt1Num;
    uint8_t pucchFmt23Num;
    uint8_t rsv[2];

    PucchFmt0Rpt pucchFmt0Rpt[MAX_PUCCH_FMT0_1_NUM];
    PucchFmt1Rpt pucchFmt1Rpt[MAX_PUCCH_FMT0_1_NUM];
    PucchFmt23Rpt pucchFmt23Rpt[MAX_PUCCH_FMT2_3_NUM];
}PucchRpt;

#endif // NR_CTRL_DSP_PUCCH_MSG_H
