/*************************************************************************************************
*
* @file          nr_ctrl_dsp_pdsch_msg.h
*
* @brief         NR主控和DSP之间的DL各信道的接口定义
*
*************************************************************************************************/
#ifndef NR_CTRL_DSP_PDSCH_MSG_H
#define NR_CTRL_DSP_PDSCH_MSG_H

#include "typedef.h"
#include "nr_common.h"

/***************************************** MACROS ***********************************************/
#define NR_MAX_DL_UE_NUM  16     // 最大UE数
#define PDSCH_MAX_PRG_NUM 160   // 最大PRG数目

#define PDSCH_PRG_TYPE_N      8 // 一个RB中( DATA DMRS) ( DATA CSI-RS) (DATA)三种类型共8种组合
#define PDSCH_PRG_RB_LEN_TYPE 2 // PRG 长度为1RB 2RB 两种情况，当PRG=4RB时，拆分成两个2RB
#define CSI_RS_PDU_TYPE_N     3 // CSI-RS PDU类型数目
#define PDSCH_MAX_ANT_N 4

enum PDSCH_MODULATE_TYPE_ENUM
{
    PDSCH_QPSK_   = 0,
    PDSCH_16QAM_  = 1,
    PDSCH_64QAM_  = 2,
    PDSCH_256QAM_ = 3
};

typedef uint8_t PDSCH_MODULATE_TYPE;

/******************************************** TYPES *********************************************/
typedef struct
{
    uint16_t         rbStart[PDSCH_MAX_PRG_NUM];     // prg 起始RB
    uint16_t         rbNum[PDSCH_MAX_PRG_NUM];       // prg RB长度
    uint16_t         pmiTableIdx[PDSCH_MAX_PRG_NUM]; // prg PMI索引
    uint16_t         dataSeqOffset[PDSCH_MAX_PRG_NUM]; // 调制后本符号内偏移
}UePrgInfo;

typedef struct
{
    bool      dmrsFlag;                                          // 1：本符号是导频， 0：本符号是数据
    uint8_t   uePrgStart[PDSCH_PRG_RB_LEN_TYPE];                 // RB长度1 和RB长度2 uePrgInfo 中索引起始位置
    uint8_t   uePrgNum[PDSCH_PRG_RB_LEN_TYPE][PDSCH_PRG_TYPE_N]; // RB长度1 和RB长度2 情况下不同类型prg的数目
    uint32_t  dataReNum;                                         // 本符号数据占用RE个数
    uint32_t  dataOffset;                                        // 距离slot首地址的RE偏移
    uint8_t   dmrsRbStart;                                       // DMRS序列首地址对应RB位置
    uint32_t  dmrsSeqOffset;                                     // 本符号本UE的DMRS序列首地址相对本slot所有小区的DMRS首地址的偏移
    uint32_t  dmrsReNum;                                         // dmrs一个PORT占用RE数目
    uint8_t   csirsPduNum;                                       // 本符号上CSI-RS配置个数
    uint8_t   csirsPduIdx[CSI_RS_PDU_TYPE_N];                                    // CSI-RS配置索引
}UePrgPos;

typedef struct
{
    UePrgPos  uePrgPos[NR_MAX_DL_UE_NUM];  // UE prg信息
    UePrgInfo uePrgInfo;                   // 本小区所有RB的prg信息
} PdschPrgInfoPerSymb;

typedef struct
{
    PDSCH_MODULATE_TYPE  modType;           // 0:qpsk,1:16qam,2:64qam,3:256QAM;
    uint8_t  layerNum;                      // 层数
    uint8_t  powerControlOffsetProfileNR;   // 功率信息 待定
    uint8_t  powerControlOffsetSSProfileNR; // 功率信息 待定
    uint8_t  dmrsCfgType;                   // DMRS 类型
    uint8_t  numDmrsCdmGrpsNoData;          // DMRS占用CDM组数
    uint16_t dmrsUsedPortsBitmap;           //12bit表示12ports，bit0 表示port1001,bit11表示port1011
    uint16_t rsv;
}PdschInfoPerUe;

typedef struct
{
    uint8_t  fdCdmGrpNum; // 取值0表示该符号无CSIRS
    uint16_t rbStart;     // RB起始
    uint16_t rbNum;       // 总的RB数
    uint8_t  reOffset;    // 一个CDMGRUOUP的re偏移
    uint8_t  row;         // 配置信息行号
    uint8_t  reStart;     // reStart表示在第一个rb中的起始re
    uint32_t seqOffset;     // 相邻两个RE的距离
    uint8_t  powerControlOffsetProfileNR;   // 功率信息 待定
    uint8_t  powerControlOffsetSSProfileNR; // 功率信息 待定
}CsirsInfoPerSymb;

typedef struct
{
    uint8_t  csirsNum; // csi-rs 配置数目
    uint8_t  rsv[3];
    CsirsInfoPerSymb  csirsInfoPerSymb[NR_SYM_NUM_PER_SLOT][CSI_RS_PDU_TYPE_N]; // 冲突时指示0,1,2
}CsirsInfoPerCell;

typedef struct
{
    uint16_t            antNum;    // 天线数
    uint16_t            ueNum;     // UE数
    uint8_t             antIdx[PDSCH_MAX_ANT_N]; // 激活天线索引
    PdschInfoPerUe      pdschInfoPerUe[NR_MAX_DL_UE_NUM];         // UE 配置信息
    PdschPrgInfoPerSymb pdschPrgInfoPerSymb[NR_SYM_NUM_PER_SLOT]; // 每个符号的PRG信息
    CsirsInfoPerCell    csirsInfoPerCell;                         // 本小区CSI-RS信息
}NrPdschCellInfo;

typedef struct
{
    uint32_t *dataAddr;  // 数据地址
    uint32_t dataLen;    // 数据长度
    uint32_t *dmrsAddr;  // DMRS地址
    uint32_t dmrsLen;    // DMRS长度
    uint32_t *csiRsAddr; // CSI-RS地址
    uint32_t csiLen;     // CSI-RS长度
}NrDlBitSeqInfo;

typedef struct
{
    uint16_t sfn;           // system frame num [0, 1023], unit of 10ms
    uint8_t  slot;          // slot, [0, 9] or [0, 19]
    uint8_t  rsv0;
    uint32_t updateCnt;     // 消息更新次数
    uint32_t cellNum;       // 小区数目
    NrDlBitSeqInfo bitSeqInfo; // 所有小区比特序列信息
    NrPdschCellInfo  cellInfo[NR_MAX_CELL_NUM]; // 小区配置信息
} NrPdschInfo;

#endif

