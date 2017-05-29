#include <string.h>
#include <stdio.h>
#include "mgeff.h"
#include "eff-common.h"
#include "effector.h"

#if 0
static int effeffector_sourcecount(EffEffector* effector)
{
    int count = 0;
    struct list_head *i;
    list_for_each(i, &effector->source_list) {
        count++;
    }
    return count;
}
#endif 

int effbaseeffector_setproperty(MGEFF_EFFECTOR _effector, int property_id, int value)
{
    EffEffector* effector = (EffEffector*)_effector;
    switch (property_id)
    {
        case MGEFF_PROPERTY_DIRECTION:
            effector->direction = value;
            return 0;
        default:
            break;
    }
    return -1;
}

int effbaseeffector_getproperty(MGEFF_EFFECTOR _effector, int property_id, int* pValue)
{
    EffEffector* effector = (EffEffector*)_effector;
    switch (property_id)
    {
        case MGEFF_PROPERTY_DIRECTION:
            *pValue = effector->direction;
            return *pValue;
        default:
            break;
    }
    return -1;
}

void effbaseeffector_begindraw(MGEFF_ANIMATION handle, MGEFF_EFFECTOR _effector)
{
    float s = 0.0f, e = 1.0f;
    mGEffAnimationSetStartValue(handle, &s);
    mGEffAnimationSetEndValue(handle, &e);
}

static void effbaserender_enddraw(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    EffEffector* effector = (EffEffector*)anim->targetobject;
    if (effector->ops->enddraw) {
        effector->ops->enddraw(handle, (MGEFF_EFFECTOR)effector);
    }
}


void effbaseeffector_rect(HDC hdc, RECT* rect)
{
    rect->left = 0;
    rect->top  = 0;
    rect->right  = GetGDCapability(hdc, GDCAP_MAXX) + 1;
    rect->bottom = GetGDCapability(hdc, GDCAP_MAXY) + 1;
}

static void effbaserender_ondraw(MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    EffEffector* effector = (EffEffector*)target;
    HDC sink_dc = effsink_get(effector->sink);
    effector->ops->ondraw(handle, (MGEFF_EFFECTOR)effector, sink_dc, id, value);
    effsink_release(effector->sink, sink_dc);
}

static EffEffector* effbaseeffector_init(MGEFF_EFFECTOR _effector)
{
    EffEffector* effector  = (EffEffector*)calloc(1, sizeof(EffEffector));
    return effector;
}

#ifdef _MGEFF_CENTERSPLITEFFECTOR
extern MGEFF_EFFECTOROPS centerspliteffector;
#endif
#ifdef _MGEFF_CUBICROTATEEFFECTOR
extern MGEFF_EFFECTOROPS cubiceffector;
extern MGEFF_EFFECTOROPS mgpluscubiceffector;
#endif
#ifdef _MGEFF_FLIPEFFECTOR
extern MGEFF_EFFECTOROPS flipeffector ;
extern MGEFF_EFFECTOROPS mgplusflipeffector;
#endif
#ifdef _MGEFF_ALPHAEFFECTOR
extern MGEFF_EFFECTOROPS alphaeffector;
#endif
#ifdef _MGEFF_ZIPEFFECTOR
extern MGEFF_EFFECTOROPS zipeffector;
#endif
#ifdef _MGEFF_ZOOMEFFECTOR
extern MGEFF_EFFECTOROPS zoomeffector;
#endif
#ifdef _MGEFF_LEAFWINDOWEFFECTOR
extern MGEFF_EFFECTOROPS leafwindoweffector;
#endif
#ifdef _MGEFF_SCROLLEFFECTOR
extern MGEFF_EFFECTOROPS scrolleffector;
#endif
#ifdef _MGEFF_PUSHEFFECTOR
extern MGEFF_EFFECTOROPS pusheffector;
#endif
#ifdef _MGEFF_CLEAREFFECTOR
extern MGEFF_EFFECTOROPS cleareffector;
#endif
#ifdef _MGEFF_FIVEPOINTEDSTAREFFECTOR
extern MGEFF_EFFECTOROPS stareffector;
#endif
#ifdef _MGEFF_RADARSCANEFFECTOR
extern MGEFF_EFFECTOROPS radarscaneffector;
#endif
#if defined(HAVE_MESA_MINIGUI) || defined(HAVE_EGL_SUPPORT)
extern MGEFF_EFFECTOROPS glrectrotateeffector;
extern MGEFF_EFFECTOROPS glcoverfloweffector;
#endif
#if defined(HAVE_MESA_MINIGUI)
extern MGEFF_EFFECTOROPS glcubeturneffector;
#endif

static MGEFF_EFFECTOROPS* s_effectors_ops[] = {
#ifdef _MGEFF_CENTERSPLITEFFECTOR
    &centerspliteffector,
#endif
#ifdef _MGEFF_ALPHAEFFECTOR
    &alphaeffector,
#endif
#ifdef _MGEFF_CUBICROTATEEFFECTOR
    &cubiceffector,
    &mgpluscubiceffector,
#endif
#ifdef _MGEFF_FLIPEFFECTOR
    &flipeffector,
    &mgplusflipeffector,
#endif
#ifdef _MGEFF_ZIPEFFECTOR
    &zipeffector,
#endif
#ifdef _MGEFF_ZOOMEFFECTOR
    &zoomeffector,
#endif
#ifdef _MGEFF_LEAFWINDOWEFFECTOR
    &leafwindoweffector,
#endif
#ifdef _MGEFF_SCROLLEFFECTOR
    &scrolleffector,
#endif
#if defined(HAVE_MESA_MINIGUI) || defined(HAVE_EGL_SUPPORT)
    &glrectrotateeffector,
    &glcoverfloweffector,
#endif
#if defined(HAVE_MESA_MINIGUI)
    &glcubeturneffector,
#endif
#ifdef _MGEFF_PUSHEFFECTOR
    &pusheffector,
#endif
#ifdef _MGEFF_CLEAREFFECTOR
    &cleareffector,
#endif
#ifdef _MGEFF_FIVEPOINTEDSTAREFFECTOR
    &stareffector,
#endif
#ifdef _MGEFF_RADARSCANEFFECTOR
    &radarscaneffector,
#endif
};

/* effector list. */
static struct list_head effector_list;

int effeffector_init(void)
{
    int i = 0;
    INIT_LIST_HEAD(&effector_list);

    for (i = 0; i < TABLESIZE(s_effectors_ops); i++) {
        if (!mGEffEffectorRegister(s_effectors_ops[i])) {
            fprintf(stderr, "%s:register %d effector failed!\n", __FUNCTION__, i);
            return -1;
        }
    }
    return 0;
}

void effeffector_deinit(void)
{
    /* destroy all create effector.*/
    struct list_head *i, *j;
    list_for_each_safe(i, j, &effector_list) {
        list_del(i);
        free(i);
    }
}


MGEFF_EFFECTOROPS* mGEffEffectorGetOps(unsigned long key)
{
    struct list_head *i;
    MGEFF_EFFECTOROPS* ops = NULL;

    list_for_each(i, &effector_list) {
        EffCBObjInfo *item = (EffCBObjInfo *)i;
        ops = (MGEFF_EFFECTOROPS*)(item->obj);
        if (mGEffStr2Key(ops->name) == key) {
            break;
        }
    }
    return ops;
}

MGEFF_BOOL mGEffEffectorRegister(const MGEFF_EFFECTOROPS* ops)
{
    EffCBObjInfo* obj = NULL;
    if (!ops) return MGEFF_FALSE;

    obj = (EffCBObjInfo*)malloc(sizeof(EffCBObjInfo));
    if (!obj) return MGEFF_FALSE;

    obj->obj = (void*)ops;
    list_add_tail(&obj->list, (&effector_list));
    return MGEFF_TRUE;
}

MGEFF_BOOL mGEffEffectorUnRegister(const MGEFF_EFFECTOROPS* ops)
{
    /* remove from schedular_list. */
    struct list_head *i = NULL ;
    list_for_each(i, &effector_list) {
        EffCBObjInfo *item = (EffCBObjInfo *)i;
        if (ops == (MGEFF_EFFECTOROPS*)(item->obj)) {
            list_del(i);
            free(i);
            return MGEFF_TRUE;
        }
    }

    return MGEFF_FALSE;
}

unsigned long mGEffStr2Key (const char* str)
{
    int i,l;
    unsigned long ret = 0;

    if (str == NULL)
        return 0;

    l = (strlen(str)+1) / 2;

    for (i=0; i<l; i++) {
        unsigned short w = MAKEWORD(str[i<<1], str[(i<<1)+1]);
        ret ^= (w<<(i&0x0f));
    }

    return ret;
}

MGEFF_EFFECTOR mGEffEffectorCreate(unsigned long key)
{
    EffEffector* effector = NULL;
    struct list_head *i;
    MGEFF_EFFECTOROPS* ops;

    list_for_each(i, &effector_list) {
        EffCBObjInfo *item = (EffCBObjInfo *)i;
        ops = (MGEFF_EFFECTOROPS*)(item->obj);
        if (mGEffStr2Key(ops->name) == key) {
#if 0
            if (ops->init)
                effector = (EffEffector*)ops->init();
            else
                effector = (EffEffector*)effbaseeffector_init();
#else
            effector = (EffEffector*)effbaseeffector_init(NULL);
            if (ops->init)
                ops->init((MGEFF_EFFECTOR)effector);

#endif
            effector->ops = ops;
            if (ops->setproperty) 
                effector->ops->setproperty = ops->setproperty;
            else
                effector->ops->setproperty = effbaseeffector_setproperty;

            if (ops->getproperty)
                effector->ops->getproperty = ops->getproperty;
            else
                effector->ops->getproperty = effbaseeffector_getproperty;

            break;
        }
    }

    if (effector) {
        effector->key = key;
        INIT_LIST_HEAD(&effector->source_list);
    }
    return (MGEFF_EFFECTOR)effector;
}

void mGEffEffectorDelete(MGEFF_EFFECTOR handle)
{
    EffEffector* effector = (EffEffector*)handle;
    struct list_head *i, *j;
    EffHDCSource* source;
    CHECK_HANDLE(handle);

    if (effector->ops->finalize) {
        effector->ops->finalize((MGEFF_EFFECTOR)effector);
    }

    /* delete the source of effector.*/
    list_for_each(i, &effector->source_list) {
        source = (EffHDCSource *)i;
        j = i->prev;
        list_del(i);
        if (source->cb) {
            source->cb(source->hdc);
        }
        free(source);
        i = j;
    }

    /* delete the sink of effector.*/
    if (effector->sink) {
        if (effector->sink->bufdc) {
            DeleteMemDC(effector->sink->bufdc);
        }
        free(effector->sink);
    }
    free(effector);
}

void mGEffEffectorDeleteWithoutSink(MGEFF_EFFECTOR handle)
{
    EffEffector* effector = (EffEffector*)handle;
    struct list_head *i, *j;
    EffHDCSource* source;
    CHECK_HANDLE(handle);

    if (effector->ops->finalize) {
        effector->ops->finalize((MGEFF_EFFECTOR)effector);
    }


    /* delete the source of effector.*/
    list_for_each(i, &effector->source_list) {
        source = (EffHDCSource *)i;
        j = i->prev;
        list_del(i);
        if (source->cb) {
            source->cb(source->hdc);
        }
        free(source);
        i = j;
    }
    /* do not delete sink.*/

    free(effector);
}

int mGEffEffectorAppendSource(MGEFF_EFFECTOR effector, MGEFF_SOURCE source)
{
    EffEffector* r = (EffEffector*) effector;
    EffHDCSource* s = (EffHDCSource*)source;

    CHECK_HANDLE_RET(effector);
    CHECK_HANDLE_RET(source);
    list_add_tail(&s->list, (&r->source_list));

    return 0; 
}

MGEFF_SOURCE mGEffEffectorGetSource(MGEFF_EFFECTOR effector, int index)
{
    EffEffector *r = (EffEffector *)effector;
    struct list_head *i = NULL ;
    EffHDCSource* source = NULL ;
    int count = 0;

    CHECK_HANDLE_RET_NIL(effector);

    list_for_each(i, &r->source_list) {
        if (count++ == index){
            source = (EffHDCSource *)i;
            break;
        }
    }
    return (MGEFF_SOURCE)source;
}

HDC mGEffEffectorGetSourceDC(MGEFF_EFFECTOR effector, int index)
{
    HDC hdc = HDC_INVALID;
    EffHDCSource * source = (EffHDCSource*)mGEffEffectorGetSource( effector , index );
    if( source )
        hdc = source->hdc ;
    return hdc;
}


int mGEffEffectorSetSink(MGEFF_EFFECTOR effector, MGEFF_SINK sink)
{
    EffEffector* r = (EffEffector *)effector;
    EffSink* s = (EffSink*)sink;

    CHECK_HANDLE_RET(effector);
    CHECK_HANDLE_RET(sink);

    r->sink = s;
    return 0;
}

void  mGEffEffectorSetContext(MGEFF_EFFECTOR handle, void* context)
{
    EffEffector* anim = (EffEffector*)handle;
    CHECK_HANDLE(handle);

    anim->context = context;

}

void* mGEffEffectorGetContext(MGEFF_EFFECTOR handle)
{
    EffEffector* anim = (EffEffector*)handle;
    CHECK_HANDLE_RET_NIL(handle);

    return anim->context;
}

MGEFF_SINK mGEffEffectorGetSink(MGEFF_EFFECTOR effector)
{
    EffEffector* r = (EffEffector *)effector;
    CHECK_HANDLE_RET_NIL(effector);

    return (MGEFF_SINK)r->sink;
}

int mGEffEffectorSetProperty(MGEFF_EFFECTOR handle, int property_id, int value)
{
    int ret = -1;
    EffEffector * r= (EffEffector *)handle;
    if ( r && r->ops && r->ops->setproperty )
        ret = r->ops->setproperty(handle, property_id, value);
    return ret;
}

int mGEffEffectorGetProperty(MGEFF_EFFECTOR handle, int property_id, int *pValue)
{
    int ret = -1;
    EffEffector* r = (EffEffector *)handle;
    if ( r && r->ops && r->ops->getproperty)
        ret = r->ops->getproperty(handle, property_id, pValue);
    return ret;
}

MGEFF_EFFECTOR mGEffEffectorCreateEx(HDC src1_dc, HDC src2_dc, HDC dst_dc, unsigned long key)
{
    MGEFF_EFFECTOR effector = mGEffEffectorCreate (key);
    if (effector) {
        MGEFF_SOURCE source1 = mGEffCreateSource (src1_dc);
        MGEFF_SOURCE source2 = mGEffCreateSource (src2_dc);
        MGEFF_SINK sink = mGEffCreateHDCSink (dst_dc);

        mGEffEffectorAppendSource (effector, source1);
        mGEffEffectorAppendSource (effector, source2);
        mGEffEffectorSetSink (effector, sink);
        if (key == MGEFF_EFFECTOR_PUSH) {
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LEFT2RIGHT);
        }
        else if (key == MGEFF_EFFECTOR_ZIP) {
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LEFT2RIGHT);
        }
        else if (key == MGEFF_EFFECTOR_CENTERSPLIT) {
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_CENTER2OUTSIDE_4);
        }
        else if (key == MGEFF_EFFECTOR_FIVEPOINTEDSTAR) {
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_CLOCKWISE);
        }
    }
    return effector;
}

MGEFF_EFFECTOR mGEffEffectorCreateWithoutSink(HDC src1_dc, HDC src2_dc, unsigned long key)
{
    MGEFF_EFFECTOR effector = mGEffEffectorCreate (key);
    if (effector) {
        MGEFF_SOURCE source1 = mGEffCreateSource (src1_dc);
        MGEFF_SOURCE source2 = mGEffCreateSource (src2_dc);

        mGEffEffectorAppendSource (effector, source1);
        mGEffEffectorAppendSource (effector, source2);
        if (key == MGEFF_EFFECTOR_PUSH) {
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LEFT2RIGHT);
        }
        else if (key == MGEFF_EFFECTOR_ZIP) {
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_LEFT2RIGHT);
        }
        else if (key == MGEFF_EFFECTOR_CENTERSPLIT) {
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_DIRECTION_CENTER2OUTSIDE_4);
        }
        else if (key == MGEFF_EFFECTOR_FIVEPOINTEDSTAR) {
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_ZOOM, MGEFF_ZOOMOUT);
            mGEffEffectorSetProperty (effector, MGEFF_PROPERTY_DIRECTION, MGEFF_CLOCKWISE);
        }
    }
    return effector;
}


int mGEffFillEffectorAnimation(HWND hwnd, HDC src1_dc, HDC src2_dc, 
        HDC dst_dc, unsigned long key, int duration)
{
    MGEFF_EFFECTOR effector =  mGEffEffectorCreateEx(src1_dc, src2_dc, dst_dc, key);
    MGEFF_ANIMATION handle = mGEffAnimationCreateWithEffector (effector);

    mGEffAnimationSetDuration (handle, duration);
    mGEffAnimationAsyncRun (handle);
    mGEffAnimationWait(&hwnd, handle);

    mGEffEffectorDelete(effector);
    return 0;
}
 
MGEFF_ANIMATION mGEffAnimationCreateWithEffector(MGEFF_EFFECTOR effector)
{
    MGEFF_ANIMATION handle = NULL;
    EffEffector* r = (EffEffector*) effector;

    struct list_head *i;
    MGEFF_EFFECTOROPS* ops;

    CHECK_HANDLE_RET_NIL(effector);
    list_for_each(i, &effector_list) {
        EffCBObjInfo *item = (EffCBObjInfo *)i;
        ops = (MGEFF_EFFECTOROPS*)(item->obj);
        if (mGEffStr2Key(ops->name) == r->key) {
                r->ops->ondraw = ops->ondraw;
                handle = mGEffAnimationCreate(effector, effbaserender_ondraw, 
                        0, ops->varianttype);
                if (handle) {
                    mGEffAnimationSetFinishedCb(handle, effbaserender_enddraw);
                    if (r->ops->begindraw)
                        r->ops->begindraw(handle, effector);
                    else {
                        effbaseeffector_begindraw(handle, effector);
                    }
                }
            break;
        }
    }

    if (handle == NULL) {
        fprintf(stderr, "MGEFF-EFFECTOR> can not find relative effector ops.\n");
    }
    return handle;
}

static void roller_delete_whitedc(MGEFF_ANIMATION handle)
{
    EffAnimation* anim = (EffAnimation*)handle;
    CHECK_HANDLE(handle);
    //DeleteMemDC(mGEffEffectorGetSourceDC((MGEFF_EFFECTOR)anim->targetobject, 1));
    DeleteMemDC(mGEffEffectorGetSourceDC((MGEFF_EFFECTOR)anim->targetobject, 0));
    mGEffEffectorDelete((MGEFF_EFFECTOR)anim->targetobject);
}

MGEFF_ANIMATION mGEffCreateRollerAnimation(HDC dc_rollup, HDC dc_expand, HDC dc_dst)
{
    RECT rc, rollup_rc, expand_rc;
    HDC white_dc_rollup = CreateCompatibleDC(dc_rollup);
    HDC white_dc_expand = CreateCompatibleDC(dc_rollup);
    MGEFF_EFFECTOR zoom_rollup, zoom_expand;
    MGEFF_ANIMATION anim_rollup, anim_expand, seq_group;

    SetBrushColor(white_dc_rollup, RGB2Pixel(white_dc_rollup, 0xff, 0xff, 0xff));
    effbaseeffector_rect(white_dc_rollup, &rc);
    FillBox(white_dc_rollup, 0, 0, RECTW(rc), RECTH(rc));

    SetBrushColor(white_dc_expand, RGB2Pixel(white_dc_expand, 0xff, 0xff, 0xff));
    effbaseeffector_rect(white_dc_expand, &rc);
    FillBox(white_dc_expand, 0, 0, RECTW(rc), RECTH(rc));

    zoom_rollup  = mGEffEffectorCreateEx(white_dc_rollup, dc_rollup, dc_dst, MGEFF_EFFECTOR_ZOOM);
    mGEffSetBufferSink(mGEffEffectorGetSink(zoom_rollup), dc_dst);
    effbaseeffector_rect(dc_rollup, &rollup_rc);
    anim_rollup  = mGEffAnimationCreateWithEffector(zoom_rollup);
    mGEffAnimationSetStartValue(anim_rollup, &rollup_rc);
    rollup_rc.right = rollup_rc.left = RECTW(rollup_rc)/2;
    mGEffAnimationSetEndValue(anim_rollup, &rollup_rc);
    mGEffAnimationSetFinishedCb(anim_rollup, roller_delete_whitedc);

    zoom_expand  = mGEffEffectorCreateEx(white_dc_expand, dc_expand, dc_dst, MGEFF_EFFECTOR_ZOOM);
    mGEffSetBufferSink(mGEffEffectorGetSink(zoom_expand), dc_dst);
    effbaseeffector_rect(dc_expand, &expand_rc);
    anim_expand  = mGEffAnimationCreateWithEffector(zoom_expand);
    mGEffAnimationSetEndValue(anim_expand, &expand_rc);
    expand_rc.right = expand_rc.left = RECTW(expand_rc)/2;
    mGEffAnimationSetStartValue(anim_expand, &expand_rc);
    mGEffAnimationSetFinishedCb(anim_expand, roller_delete_whitedc);

    /*anim_rollup、anim_expand add to seq_group. */
    seq_group = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL);
    mGEffAnimationAddToGroup(seq_group, anim_rollup);
    mGEffAnimationAddToGroup(seq_group, anim_expand);

    return seq_group;
}


