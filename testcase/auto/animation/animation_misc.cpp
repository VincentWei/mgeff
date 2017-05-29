
#include "animation_base.h"

void test_animation_float ()
{
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    float startval_float = 0.1f, endval_float = 1.1f;

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
    animaparam.loopcount = 13;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_float;
    animaparam.endval = (void*)&endval_float;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_FLOAT);
    mGEffAnimationSetFinishedCb (handle, finished_callback);

    mGEffAnimationSyncRun (handle);
    
    EXPECT_TRUE (testprj.val.bfinished);
    EXPECT_EQ (*(float*)animaparam.endval, *(float*)testprj.val.endval);
    mGEffAnimationDelete (handle);
}

void test_animation_double ()
{
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    double startval_double = 1429, endval_double = 1214387;

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
    animaparam.loopcount = 13;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_double;
    animaparam.endval = (void*)&endval_double;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_DOUBLE);
    mGEffAnimationSetFinishedCb (handle, finished_callback);

    mGEffAnimationSyncRun(handle);
    
    EXPECT_TRUE (testprj.val.bfinished);
    EXPECT_EQ (*(double*)animaparam.endval, *(double*)testprj.val.endval);
    mGEffAnimationDelete (handle);
}

void test_animation_rect ()
{
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    RECT startval_rect = {0, 0, 200, 200}, endval_rect = {10, 10, 100, 100};

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
    animaparam.loopcount = 8;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_rect;
    animaparam.endval = (void*)&endval_rect;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_RECT );
    mGEffAnimationSetFinishedCb (handle, finished_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bfinished);
    EXPECT_TRUE (0 == memcmp (animaparam.endval, testprj.val.endval, sizeof (RECT)));
    mGEffAnimationDelete (handle);
}

void test_animation_point ()
{
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    POINT startval_point = {123, 25}, endval_point = {0, 128};

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
    animaparam.loopcount = 8;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_point;
    animaparam.endval = (void*)&endval_point;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_POINT);
    mGEffAnimationSetFinishedCb (handle, finished_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bfinished);
    EXPECT_TRUE (0 == memcmp (animaparam.endval, testprj.val.endval, sizeof (POINT)));
    mGEffAnimationDelete (handle);
}

void test_animation_pointf ()
{
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    EffPointF startval_pointf = {1.0f, 0.0f}, endval_pointf = {1.01f, 12.0f};

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 325;
    animaparam.loopcount = 8;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_pointf;
    animaparam.endval = (void*)&endval_pointf;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_POINTF);
    mGEffAnimationSetFinishedCb (handle, finished_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bfinished);
    EXPECT_TRUE (0 == memcmp (animaparam.endval, testprj.val.endval, sizeof (EffPointF)));
    mGEffAnimationDelete (handle);
}

void test_animation_3dpoint()
{
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    EffPoint3D startval_point = {1, 0, 212}, endval_point = {21, 55, 0};

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 3025;
    animaparam.loopcount = 8;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_point;
    animaparam.endval = (void*)&endval_point;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_3DPOINT);
    mGEffAnimationSetFinishedCb (handle, finished_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bfinished);
    EXPECT_TRUE (0 == memcmp (animaparam.endval, testprj.val.endval, sizeof (EffPoint3D)));
    mGEffAnimationDelete (handle);
}

void test_animation_3dpointf()
{
    TESTPRJ testprj;
    MGEFF_ANIMATION handle;
    ANIMAPARAM animaparam;
    EffPointF3D startval_pointf = {1.0f, 0.0f, 212.0f}, endval_pointf = {21.0f, 55.0f, 0.0f};

    memset (&testprj, 0, sizeof(testprj));
    memset (&animaparam, 0, sizeof(animaparam));
    animaparam.duration = 1025;
    //animaparam.loopcount = 8;
    animaparam.fps = 13;
    animaparam.startval = (void*)&startval_pointf;
    animaparam.endval = (void*)&endval_pointf;
    handle = create_animation ((void*)&testprj, &animaparam, MGEFF_3DPOINTF);
    mGEffAnimationSetFinishedCb (handle, finished_callback);

    mGEffAnimationSyncRun(handle);

    EXPECT_TRUE (testprj.val.bfinished);

    //EffPointF3D *tmp = (EffPointF3D*)&testprj.val.endval;
    //printf ("endval: %8f, %8f, %8f\n", tmp->x, tmp->y, tmp->z);

    EXPECT_TRUE (0 == memcmp (animaparam.endval, testprj.val.endval, sizeof (EffPointF3D)));
    mGEffAnimationDelete (handle);
}


