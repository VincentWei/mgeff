#include <string.h>
#include <stdio.h>
#include "mgeff.h"
#include "eff-common.h"
#include "effector-source.h"

MGEFF_SOURCE mGEffCreateSource(HDC hdc)
{
    return mGEffCreateSourceEx(hdc, NULL);
}

MGEFF_SOURCE mGEffCreateSourceEx(HDC hdc, MGEFF_DELETEHDC_CB cb)
{
    EffHDCSource* source = (EffHDCSource*)calloc(1, sizeof(EffHDCSource));
    source->hdc = hdc;
    //source->rect.right  = GetGDCapability(hdc, GDCAP_MAXX) + 1;
    //source->rect.bottom = GetGDCapability(hdc, GDCAP_MAXY) + 1;
    source->cb = cb;
    return (MGEFF_SOURCE)source;
}

HDC mGEffGetSourceDC(MGEFF_SOURCE handle)
{
    EffHDCSource* source = (EffHDCSource*)handle;
    CHECK_HANDLE_RET(handle);

    return source->hdc;
}
