#include <gtest/gtest.h>
#include <mgeff/mgeff.h>
#include <sys/times.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <sys/times.h>

#if 0
void mycb(MGEFF_ANIMATION handle, void* target, int id, void* value) {
    float *a = (float *)value;
    printf("%ld %s %d %f\n", times(NULL), __FUNCTION__, __LINE__, *a);
}

TEST(SchedularTest, Simple) {
    ASSERT_TRUE(mGEffInit() == 0);
    MGEFF_ANIMATION animation;
    animation = mGEffAnimationCreate(NULL, mycb, 1, MGEFF_FLOAT);
    ASSERT_TRUE(animation);
    float a;
    a = 0.5;
    mGEffAnimationSetStartValue(animation, &a);
    a = 100.9;
    mGEffAnimationSetEndValue(animation, &a);
    mGEffAnimationSetDuration(animation, 2000);
    mGEffAnimationStart(animation, 0);
    ASSERT_TRUE(mGEffSchedularRun(MGEFF_MODE_THREAD) == 0);
    sleep(2);
}

void mycb_rect(MGEFF_ANIMATION handle, void* target, int id, void* value) {
    RECT *rc = (RECT *)value;
    printf("%ld %s %d %d,%d,%d,%d\n", times(NULL), __FUNCTION__, __LINE__, rc->left, rc->top, rc->right, rc->bottom);
}

TEST(SchedularTest, Rect) {
    ASSERT_TRUE(mGEffInit() == 0);
    MGEFF_ANIMATION animation;
    animation = mGEffAnimationCreate(NULL, mycb_rect, 1, MGEFF_RECT);
    ASSERT_TRUE(animation);
    RECT rc;
    rc.left = 100; rc.right = 200; rc.top = 50; rc.bottom = 250;
    mGEffAnimationSetStartValue(animation, &rc);
    rc.left = 300; rc.right = 500; rc.top = 20; rc.bottom = 200;
    mGEffAnimationSetEndValue(animation, &rc);
    mGEffAnimationSetDuration(animation, 2000);
    mGEffAnimationStart(animation, 0);
    ASSERT_TRUE(mGEffSchedularRun(MGEFF_MODE_THREAD) == 0);
}
#endif

#define MODE_AMOUNT 3

static struct _g_default_sch {
    int mode;
    MGEFF_ANIMATION handle;
    clock_t cb_time;
    pthread_t tid;
} g_default_sch;
#if 0
static int g_mgeff_mode;
static clock_t g_cb_time;
static pthread_t g_tid_child;
static MGEFF_ANIMATION g_handle;
#endif
static struct _g_other_sch {
    MGEFF_SCHEDULAR schedular;
    MGEFF_ANIMATION handle;
    int is_running;
    pthread_t tid;
} g_other_sch[MODE_AMOUNT];

void set_property (MGEFF_ANIMATION handle, void *target, int id, void *value)
{
    g_default_sch.cb_time = times(NULL);
    g_default_sch.tid = pthread_self ();
}

void set_others_property (MGEFF_ANIMATION handle, void *target, int id, void *value)
{
    g_other_sch[id].is_running = 1;
    g_other_sch[id].tid = pthread_self ();
    return;
}

int run (int mode)
{
    g_default_sch.mode = mode;
    //init
    mGEffInit ();
    //do
    MGEFF_ANIMATION handle;
    HWND hButton = NULL;
    handle = mGEffAnimationCreate ((void *)hButton, set_property, 0, MGEFF_INT);
    int value;
    value = 1;
    mGEffAnimationSetStartValue (handle, &value);
    value = 10;
    mGEffAnimationSetEndValue (handle, &value);
    mGEffAnimationSetDuration (handle, 200);
    mGEffAnimationSetProperty(handle, MGEFF_PROP_LOOPCOUNT, 3);
    mGEffAnimationStart (handle, 0);
    g_default_sch.handle = handle;
    //run schedular
    mGEffSchedularRun (mode);
    return 0;
}

int run_other_schedulars ()
{
    int i = 0;
    for (i = 0; i < MODE_AMOUNT; ++i) {
        if(i == MGEFF_MODE_TIMER)
           continue; 

        g_other_sch[i].schedular = mGEffSchedularCreate ();
        MGEFF_ANIMATION handle;
        HWND hButton = NULL;
        handle = mGEffAnimationCreate ((void *)hButton, set_others_property, i, MGEFF_INT);
        int value;
        value = 1;
        mGEffAnimationSetStartValue (handle, &value);
        value = 10;
        mGEffAnimationSetEndValue (handle, &value);
        mGEffAnimationSetDuration (handle, 200);
        mGEffAnimationSetProperty(handle, MGEFF_PROP_LOOPCOUNT, 3);
        mGEffAnimationStartEx (g_other_sch[i].schedular, handle, 0);
        g_other_sch[i].handle = handle;
        mGEffSchedularRunEx (g_other_sch[i].schedular, i);
    }

    return 0;
}

void destroy_other_schedulars ()
{
    int i = 0;
    for (i = 0; i < MODE_AMOUNT; ++i) {
        if(i == MGEFF_MODE_TIMER)
           continue; 
        mGEffSchedularDestroy (g_other_sch[i].schedular);
    }
}

int thread_check (void)
{
    if (g_default_sch.mode == MGEFF_MODE_THREAD)
        return pthread_self () == g_default_sch.tid ? 1 : 0;
    return pthread_self () == g_default_sch.tid ? 0 : 1;
}

int time_check (void)
{
    return times (NULL) - g_default_sch.cb_time;
}

TEST(SchedularTest, BlockMode) {
    run (MGEFF_MODE_BLOCK);
    ASSERT_TRUE(thread_check () == 0);
    ASSERT_TRUE(time_check () < 1000);
    mGEffDeinit ();
}

TEST(SchedularTest, ThreadMode) {
    run (MGEFF_MODE_THREAD);
    usleep (1);
    pthread_join (g_default_sch.tid, NULL);
    ASSERT_TRUE(thread_check () == 0);
    ASSERT_TRUE(time_check () < 1000);
    mGEffDeinit ();
}

// remove test case of timer-mode because we have to make MiniGUI initialized before use it.
#if 0
TEST(SchedularTest, TimerMode) {
    run (MGEFF_MODE_TIMER);
    ASSERT_TRUE(thread_check () == 0);
    ASSERT_TRUE(time_check () < 1);
    mGEffDeinit ();
}
#endif

TEST(SchedularTest, MultiSchedulars) {
    run (MGEFF_MODE_THREAD);
    run_other_schedulars ();
    //do
    ASSERT_TRUE(thread_check () == 0);
    ASSERT_TRUE(time_check () < 2000);
    ASSERT_TRUE(g_other_sch[MGEFF_MODE_BLOCK].is_running);
    usleep (1);
    pthread_join (g_default_sch.tid, NULL);
    pthread_join (g_other_sch[MGEFF_MODE_THREAD].tid, NULL);
    ASSERT_TRUE(g_other_sch[MGEFF_MODE_THREAD].is_running);
    //ASSERT_TRUE(g_is_sch_running[MGEFF_MODE_TIMER]);
    
    destroy_other_schedulars ();
    mGEffDeinit ();
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
