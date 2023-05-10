static const JsonReflect uePrgInfoReflect[] = {
    JSON_NUMBER_ARRAY(UePrgInfo, rbStart),
    //JSON_VAR_NUMBER_ARRAY(UePrgInfo, rbStart, [array个数的指示变量名]),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_NUMBER_ARRAY(UePrgInfo, rbNum),
    //JSON_VAR_NUMBER_ARRAY(UePrgInfo, rbNum, [array个数的指示变量名]),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_NUMBER_ARRAY(UePrgInfo, pmiTableIdx),
    //JSON_VAR_NUMBER_ARRAY(UePrgInfo, pmiTableIdx, [array个数的指示变量名]),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_NUMBER_ARRAY(UePrgInfo, dataSeqOffset),
    //JSON_VAR_NUMBER_ARRAY(UePrgInfo, dataSeqOffset, [array个数的指示变量名]),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_END()
};

static const JsonReflect uePrgPosReflect[] = {
    JSON_BOOL(UePrgPos, dmrsFlag),
    JSON_NUMBER_ARRAY(UePrgPos, uePrgStart),
    //JSON_VAR_NUMBER_ARRAY(UePrgPos, uePrgStart, [array个数的指示变量名]),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_NUMBER_2D_ARRAY(UePrgPos, uePrgNum),
    JSON_NUMBER(UePrgPos, dataReNum),
    JSON_NUMBER(UePrgPos, dataOffset),
    JSON_NUMBER(UePrgPos, dmrsRbStart),
    JSON_NUMBER(UePrgPos, dmrsSeqOffset),
    JSON_NUMBER(UePrgPos, dmrsReNum),
    JSON_NUMBER(UePrgPos, csirsPduNum),
    JSON_NUMBER_ARRAY(UePrgPos, csirsPduIdx),
    //JSON_VAR_NUMBER_ARRAY(UePrgPos, csirsPduIdx, [array个数的指示变量名]),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_END()
};

static const JsonReflect pdschPrgInfoPerSymbReflect[] = {
    JSON_STRUCT_ARRAY(PdschPrgInfoPerSymb, uePrgPos, uePrgPosReflect),
    //JSON_VAR_STRUCT_ARRAY(PdschPrgInfoPerSymb, uePrgPos, [array个数的指示变量名], uePrgPosReflect),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_STRUCT(PdschPrgInfoPerSymb, uePrgInfo, uePrgInfoReflect),
    JSON_END()
};

static const JsonReflect pdschInfoPerUeReflect[] = {
    JSON_NUMBER(PdschInfoPerUe, modType),
    JSON_NUMBER(PdschInfoPerUe, layerNum),
    JSON_NUMBER(PdschInfoPerUe, powerControlOffsetProfileNR),
    JSON_NUMBER(PdschInfoPerUe, powerControlOffsetSSProfileNR),
    JSON_NUMBER(PdschInfoPerUe, dmrsCfgType),
    JSON_NUMBER(PdschInfoPerUe, numDmrsCdmGrpsNoData),
    JSON_NUMBER(PdschInfoPerUe, dmrsUsedPortsBitmap),
    JSON_RESERVED(PdschInfoPerUe, rsv),

    JSON_END()
};

static const JsonReflect csirsInfoPerSymbReflect[] = {
    JSON_NUMBER(CsirsInfoPerSymb, fdCdmGrpNum),
    JSON_NUMBER(CsirsInfoPerSymb, rbStart),
    JSON_NUMBER(CsirsInfoPerSymb, rbNum),
    JSON_NUMBER(CsirsInfoPerSymb, reOffset),
    JSON_NUMBER(CsirsInfoPerSymb, row),
    JSON_NUMBER(CsirsInfoPerSymb, reStart),
    JSON_NUMBER(CsirsInfoPerSymb, seqOffset),
    JSON_NUMBER(CsirsInfoPerSymb, powerControlOffsetProfileNR),
    JSON_NUMBER(CsirsInfoPerSymb, powerControlOffsetSSProfileNR),
    JSON_END()
};

static const JsonReflect csirsInfoPerCellReflect[] = {
    JSON_NUMBER(CsirsInfoPerCell, csirsNum),
    JSON_RESERVED(CsirsInfoPerCell, rsv),

    JSON_STRUCT_ARRAY(CsirsInfoPerCell, csirsInfoPerSymb, csirsInfoPerSymbReflect),
    //JSON_VAR_STRUCT_ARRAY(CsirsInfoPerCell, csirsInfoPerSymb, [array个数的指示变量名], csirsInfoPerSymbReflect),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_END()
};

static const JsonReflect nrPdschCellInfoReflect[] = {
    JSON_NUMBER(NrPdschCellInfo, antNum),
    JSON_NUMBER(NrPdschCellInfo, ueNum),
    JSON_NUMBER_ARRAY(NrPdschCellInfo, antIdx),
    //JSON_VAR_NUMBER_ARRAY(NrPdschCellInfo, antIdx, [array个数的指示变量名]),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_STRUCT_ARRAY(NrPdschCellInfo, pdschInfoPerUe, pdschInfoPerUeReflect),
    //JSON_VAR_STRUCT_ARRAY(NrPdschCellInfo, pdschInfoPerUe, [array个数的指示变量名], pdschInfoPerUeReflect),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_STRUCT_ARRAY(NrPdschCellInfo, pdschPrgInfoPerSymb, pdschPrgInfoPerSymbReflect),
    //JSON_VAR_STRUCT_ARRAY(NrPdschCellInfo, pdschPrgInfoPerSymb, [array个数的指示变量名], pdschPrgInfoPerSymbReflect),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_STRUCT(NrPdschCellInfo, csirsInfoPerCell, csirsInfoPerCellReflect),
    JSON_END()
};

static const JsonReflect nrDlBitSeqInfoReflect[] = {
    JSON_NUMBER(NrDlBitSeqInfo, dataLen),
    JSON_NUMBER(NrDlBitSeqInfo, dmrsLen),
    JSON_NUMBER(NrDlBitSeqInfo, csiLen),
    JSON_END()
};

static const JsonReflect nrPdschInfoReflect[] = {
    JSON_NUMBER(NrPdschInfo, sfn),
    JSON_NUMBER(NrPdschInfo, slot),
    JSON_NUMBER(NrPdschInfo, rsv0),
    JSON_NUMBER(NrPdschInfo, updateCnt),
    JSON_NUMBER(NrPdschInfo, cellNum),
    JSON_STRUCT(NrPdschInfo, bitSeqInfo, nrDlBitSeqInfoReflect),
    JSON_STRUCT_ARRAY(NrPdschInfo, cellInfo, nrPdschCellInfoReflect),
    //JSON_VAR_STRUCT_ARRAY(NrPdschInfo, cellInfo, [array个数的指示变量名], nrPdschCellInfoReflect),       // 请手动修改, 挑选使用固定的array还是var array
    JSON_END()
};

