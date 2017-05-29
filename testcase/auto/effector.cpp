#include <gtest/gtest.h>
#include <mgeff/mgeff.h>
#include <sys/times.h>
#include <stdlib.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <sys/times.h>

#define MODE_AMOUNT 3


void run_effector(BOOL is_syn )
{
    HDC hdc1 = 0;
    HDC hdc2 = 0;
    HDC dst_dc = 0;

    int s_alpha = 50, e_alpha = 200;

    MGEFF_ANIMATION handle = NULL ;

    ASSERT_TRUE(mGEffInit() == 0 );


    MGEFF_EFFECTOR effector = mGEffEffectorCreate(MGEFF_EFFECTOR_ALPHA);
    ASSERT_TRUE( effector ) ;

    MGEFF_SOURCE source1 = mGEffCreateSource(hdc1);
    ASSERT_TRUE( source1 ) ;
    MGEFF_SOURCE source2 = mGEffCreateSource(hdc2);
    ASSERT_TRUE( source2 ) ;
    MGEFF_SINK sink = mGEffCreateHDCSink(dst_dc);
    ASSERT_TRUE( sink ) ;

    ASSERT_TRUE(mGEffEffectorAppendSource(effector, source1) == 0 ) ;
    ASSERT_TRUE(mGEffEffectorAppendSource(effector, source2) == 0 ) ;

    mGEffEffectorSetSink(effector, sink);

    handle = mGEffAnimationCreateWithEffector(effector);

    ASSERT_TRUE( handle ) ;

    mGEffAnimationSetStartValue(handle, &s_alpha);
    mGEffAnimationSetEndValue(handle, &e_alpha);
    mGEffAnimationSetDuration(handle, 5000);


    if( is_syn ) 
        ASSERT_TRUE( mGEffAnimationSyncRun(handle) );
    else
    {
        ASSERT_TRUE( mGEffAnimationAsyncRun(handle) );
        ASSERT_TRUE( mGEffAnimationWait( NULL,handle) );
    }


    mGEffEffectorDelete(effector);

    mGEffDeinit ();
}

void test_effector_create()
{
    MGEFF_EFFECTOR effector = mGEffEffectorCreate(MGEFF_EFFECTOR_ALPHA);
    ASSERT_TRUE( effector );
}

void run ( BOOL is_syn )
{
#if 0
//    g_default_sch.mode = mode;
    //init
    ASSERT_TRUE(mGEffInit() == 0);
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

    g_default_sch.handle = handle;

    if( is_syn ) 
        ASSERT_TRUE( mGEffAnimationSyncRun(handle) );
    else
        ASSERT_TRUE( mGEffAnimationAsyncRun(handle) ) ;
#endif

}

void test_effector() 
{
    ASSERT_TRUE(mGEffInit() == 0 );
    MGEFF_EFFECTOROPS info;
    bzero( &info ,sizeof(info)) ;
    strcpy( info.name,"test" ) ;
    ASSERT_TRUE( mGEffEffectorRegister( &info ) ) ;
    unsigned long key = mGEffStr2Key( info.name );
    MGEFF_EFFECTOR effector = mGEffEffectorCreate (key);
    ASSERT_TRUE( effector );
    mGEffEffectorDelete( effector ) ;

    ASSERT_TRUE( mGEffEffectorUnRegister( &info ) ) ;

    effector = mGEffEffectorCreate (key);

    ASSERT_FALSE( effector );

    mGEffEffectorDelete( effector ) ;

    mGEffDeinit ();
}


TEST(EffectorTest, Effector) {
    test_effector();
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
