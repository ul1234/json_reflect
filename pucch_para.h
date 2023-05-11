static const JsonReflect pucFmt0ParamReflect[] = {
    JSON_NUMBER(PucFmt0Param, pduIdxInner),
    JSON_RESERVED(PucFmt0Param, rsvd),

    JSON_NUMBER(PucFmt0Param, mcsBitMap),
    JSON_NUMBER_ARRAY(PucFmt0Param, cyclicShift),
    JSON_END()
};

static const JsonReflect fmt1UEParamReflect[] = {
    JSON_NUMBER(Fmt1UEParam, pduIdxInner),
    JSON_RESERVED(Fmt1UEParam, rsvd),

    JSON_NUMBER(Fmt1UEParam, m0),
    JSON_NUMBER(Fmt1UEParam, uciBitNum),
    JSON_END()
};

static const JsonReflect fmt1ParamOccReflect[] = {
    JSON_NUMBER(Fmt1ParamOcc, ueTapBitMap),
    JSON_RESERVED(Fmt1ParamOcc, rsvd),

    JSON_NUMBER(Fmt1ParamOcc, timeDomainOccIdx),
    JSON_NUMBER(Fmt1ParamOcc, userNumPerOcc),
    JSON_VAR_STRUCT_ARRAY(Fmt1ParamOcc, fmt1UEParam, userNumPerOcc, fmt1UEParamReflect),
    JSON_END()
};

static const JsonReflect pucFmt1ParamReflect[] = {
    JSON_NUMBER_ARRAY(PucFmt1Param, cyclicShift),
    JSON_NUMBER(PucFmt1Param, occNum),
    JSON_NUMBER(PucFmt1Param, minUserOccIdx),
    JSON_STRUCT_ARRAY(PucFmt1Param, fmt1ParamOcc, fmt1ParamOccReflect),
    JSON_END()
};

static const JsonReflect pucFmt2ParamReflect[] = {
    JSON_NUMBER(PucFmt2Param, pduIdxInner),
    JSON_RESERVED(PucFmt2Param, rsvd),

    JSON_NUMBER(PucFmt2Param, bitNum),
    JSON_END()
};

static const JsonReflect fmt3UciLlrPosReflect[] = {
    JSON_NUMBER(Fmt3UciLlrPos, llrStart),
    JSON_NUMBER(Fmt3UciLlrPos, llrDuration),
    JSON_END()
};

static const JsonReflect pucFmt3ParamReflect[] = {
    JSON_NUMBER(PucFmt3Param, pduIdxInner),
    JSON_BOOL(PucFmt3Param, pi2bpsk),
    JSON_BOOL(PucFmt3Param, addDmrsEnable),
    JSON_NUMBER(PucFmt3Param, rsvd1),
    JSON_NUMBER_2D_ARRAY(PucFmt3Param, dmrsSymIdx),
    JSON_NUMBER_2D_ARRAY(PucFmt3Param, cyclicShift),
    JSON_NUMBER(PucFmt3Param, rsvd2),
    JSON_BOOL(PucFmt3Param, uciPart2Enable),
    JSON_NUMBER(PucFmt3Param, part1BlockNum),
    JSON_NUMBER(PucFmt3Param, part2BlockNum),
    JSON_VAR_STRUCT_ARRAY(PucFmt3Param, part1LlrPos, part1BlockNum, fmt3UciLlrPosReflect),
    JSON_VAR_STRUCT_ARRAY(PucFmt3Param, part2LlrPos, part2BlockNum, fmt3UciLlrPosReflect),
    JSON_END()
};

static const JsonReflect pucHopParamReflect[] = {
    JSON_NUMBER(PucHopParam, prbStart),
    JSON_NUMBER(PucHopParam, prbSize),
    JSON_NUMBER(PucHopParam, startSymIdx),
    JSON_NUMBER(PucHopParam, symNum),
    JSON_NUMBER(PucHopParam, dmrsSymNum),
    JSON_NUMBER(PucHopParam, uciSymNum),
    JSON_NUMBER(PucHopParam, contSymNum),
    JSON_END()
};

static const JsonReflect pucParamReflect[] = {
    JSON_NUMBER(PucParam, pucFormat),
    JSON_NUMBER(PucParam, rxAntNum),
    JSON_RESERVED(PucParam, rsvd),

    JSON_NUMBER(PucParam, hopNum),
    JSON_VAR_STRUCT_ARRAY(PucParam, hopParam, hopNum, pucHopParamReflect),
    JSON_UNION(PucParam, formatxParam, pucFormat, (const JsonReflect *)[union的reflect]),      // 请手动修改
    JSON_END()
};

static const JsonReflect pucFmt0AlgoParamReflect[] = {
    JSON_NUMBER(PucFmt0AlgoParam, deltaOffset),
    JSON_NUMBER(PucFmt0AlgoParam, noiseTapNum),
    JSON_RESERVED(PucFmt0AlgoParam, rsvd),

    JSON_NUMBER(PucFmt0AlgoParam, threshold),
    JSON_END()
};

static const JsonReflect pucFmt1AlgoParamReflect[] = {
    JSON_NUMBER(PucFmt1AlgoParam, noiseTapNum),
    JSON_RESERVED(PucFmt1AlgoParam, rsvd),

    JSON_NUMBER(PucFmt1AlgoParam, threshold),
    JSON_END()
};

static const JsonReflect pucFmt23AlgoParamReflect[] = {
    JSON_NUMBER(PucFmt23AlgoParam, MrcIrcFlag),
    JSON_RESERVED(PucFmt23AlgoParam, rsvd),

    JSON_NUMBER(PucFmt23AlgoParam, epsilon),
    JSON_NUMBER(PucFmt23AlgoParam, beta),
    JSON_NUMBER(PucFmt23AlgoParam, segNum),
    JSON_NUMBER(PucFmt23AlgoParam, threshold),
    JSON_END()
};

static const JsonReflect pucchParaReflect[] = {
    JSON_NUMBER(PucchPara, pucchNum),
    JSON_RESERVED(PucchPara, rsvd),

    JSON_NUMBER(PucchPara, fmt0Num),
    JSON_NUMBER(PucchPara, fmt1Num),
    JSON_NUMBER(PucchPara, fmt2Num),
    JSON_NUMBER(PucchPara, fmt3Num),
    JSON_STRUCT(PucchPara, fmt0AlgoParam, pucFmt0AlgoParamReflect),
    JSON_STRUCT(PucchPara, fmt1AlgoParam, pucFmt1AlgoParamReflect),
    JSON_STRUCT(PucchPara, fmt2AlgoParam, pucFmt23AlgoParamReflect),
    JSON_STRUCT(PucchPara, fmt3AlgoParam, pucFmt23AlgoParamReflect),
    JSON_VAR_STRUCT_ARRAY(PucchPara, pucParam, pucchNum, pucParamReflect),
    JSON_END()
};

