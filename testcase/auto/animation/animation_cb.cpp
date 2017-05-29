#include "animation_base.h"
#include "animation_cb.h"

static void finish_callback(MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bfinished = true;
}

static void statechanged_callback(MGEFF_ANIMATION handle, EffState news, EffState olds)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bstatechanged = true;
}

static void curloopchanged_callback(MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bcurloopchanged = true;
}

static void dirchanged_callback(MGEFF_ANIMATION handle)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bdirchanged = true;
}

static void setproperty_callback(MGEFF_ANIMATION handle, void *target, int id, void *value)
{
    TESTPRJ *testprj = (TESTPRJ*)mGEffAnimationGetTarget (handle);
    testprj->val.bsetproperty = true;
}

void test_animation_finish_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSetFinishedCb (handle, finish_callback);

    mGEffAnimationSyncRun(handle);
    
    EXPECT_TRUE (testprj.val.bfinished);

    mGEffAnimationDelete (handle);
}

void test_animation_statechanged_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSetStateChangedCb (handle, statechanged_callback);

    mGEffAnimationSyncRun(handle);
    
    mGEffAnimationSetProperty(handle, MGEFF_PROP_STATE, MGEFF_STATE_PAUSED);

    EXPECT_TRUE (testprj.val.bstatechanged);

    mGEffAnimationDelete (handle);
}

void test_animation_curloopchanged_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSetCurLoopChangedCb (handle, curloopchanged_callback);

//    mGEffAnimationSetProperty(handle, MGEFF_PROP_CURLOOP, 3);

    mGEffAnimationSyncRun(handle);
    
    EXPECT_TRUE (testprj.val.bstatechanged);

    mGEffAnimationDelete (handle);
}

void test_animation_dirchanged_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT);

    mGEffAnimationSetProperty(handle, MGEFF_PROP_DIRECTION, MGEFF_DIR_BACKWARD);

    mGEffAnimationSetDirChangedCb(handle, dirchanged_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bdirchanged);

    mGEffAnimationDelete (handle);
}

void test_animation_setproperty_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            setproperty_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bsetproperty);

    mGEffAnimationDelete (handle);
}

void test_animation_varmalloc_calcvalue_callback()
{
    MGEFF_ANIMATION handle = NULL;

    TESTPRJ testprj;
    ANIMAPARAM animaparam;

    //int usetime;
    int startval_int = 3, endval_int = 232;

    memset (&testprj, 0, sizeof (testprj));
    memset (&animaparam, 0, sizeof (animaparam));

    animaparam.duration = 325;
    animaparam.loopcount = 5;
    animaparam.fps = 13;
    animaparam.direction = MGEFF_DIR_FORWARD;
    animaparam.startval = (void*)&startval_int;
    animaparam.endval = (void*)&endval_int;

    handle = createex_animation ((void*)&testprj, &animaparam, MGEFF_INT,
            sizeof(int), varmalloc_callback, calcvalue_callback);

    mGEffAnimationSyncRun(handle);
    
    EXPECT_TRUE (testprj.val.bvarmalloc);
    EXPECT_TRUE (testprj.val.bcalcvalue);

    mGEffAnimationDelete (handle);
}

#if 0
void test_animation_malloc_callback()
{

}
#endif

