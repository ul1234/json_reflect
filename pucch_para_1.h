static const JsonReflect pucchFmt0RptReflect[] = {
    JSON_NUMBER(PucchFmt0Rpt, pduIdxInner),
    JSON_NUMBER(PucchFmt0Rpt, mcsValue),
    JSON_RESERVED(PucchFmt0Rpt, rsvd),

    JSON_BOOL(PucchFmt0Rpt, isDtx),
    JSON_NUMBER(PucchFmt0Rpt, snr),
    JSON_END()
};

static const JsonReflect pucchFmt1RptReflect[] = {
    JSON_NUMBER(PucchFmt1Rpt, pduIdxInner),
    JSON_RESERVED(PucchFmt1Rpt, rsvd),

    JSON_NUMBER(PucchFmt1Rpt, uciBitNum),
    JSON_NUMBER_ARRAY(PucchFmt1Rpt, uciDecodeValue),
    JSON_BOOL(PucchFmt1Rpt, isDtx),
    JSON_NUMBER(PucchFmt1Rpt, snr),
    JSON_END()
};

static const JsonReflect pucchFmt23RptReflect[] = {
    JSON_NUMBER(PucchFmt23Rpt, pduIdxInner),
    JSON_RESERVED(PucchFmt23Rpt, rsvd),

    JSON_BOOL(PucchFmt23Rpt, isDtxM),
    JSON_NUMBER(PucchFmt23Rpt, snr),
    JSON_END()
};

static const JsonReflect pucchRptReflect[] = {
    JSON_NUMBER(PucchRpt, pucchFmt0Num),
    JSON_NUMBER(PucchRpt, pucchFmt1Num),
    JSON_NUMBER(PucchRpt, pucchFmt23Num),
    JSON_RESERVED(PucchRpt, rsv),

    JSON_STRUCT_ARRAY(PucchRpt, pucchFmt0Rpt, pucchFmt0RptReflect),
    JSON_VAR_STRUCT_ARRAY(PucchRpt, pucchFmt1Rpt, pucchFmt1Num, pucchFmt1RptReflect),
    JSON_VAR_STRUCT_ARRAY(PucchRpt, pucchFmt23Rpt, pucchFmt23Num, pucchFmt23RptReflect),
    JSON_END()
};

