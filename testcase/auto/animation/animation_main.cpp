#include <assert.h>
#include <sys/time.h>
#include <time.h> 

#include <mgeff/mgeff.h>

#include <gtest/gtest.h>

#include "animation_misc.h"
#include "animation_base.h"
#include "animation_group.h"
#include "animation_int.h"


pthread_t thread_id;
int g_quit = 1;
void *power_pthread (void *arg)
{
    while (g_quit)
    {
        assert(0);
        // EffSchedularOnTimer();
        usleep (20000);
    }
    return NULL;
}

void create_schedular_pthread (void)
{
    pthread_create(&thread_id, NULL, power_pthread, NULL);
}

#if 1
TEST(Animation, DoneFinished){
    test_animation_finished ();
}

TEST (Animation, AnimationPause){
    test_animation_pause ();
}

TEST (Animation, Direction) {
    test_animation_direction();
}

TEST(Animantion, Loopcount){
    test_animation_loopcount ();
}

TEST(Animation, Duration){
    test_animation_duration();
}

TEST(Animation, StateChange){
    test_animation_state_change ();
}

TEST (Animation, CreateEX){
    test_animation_createex ();
}

TEST (Animation, FloatAnimationEndValue){
    test_animation_float ();
}

TEST (Animation, DoubleAnimation){
    test_animation_double ();
}

TEST (Animation, PointAnimation){
    test_animation_point ();
}

TEST (Animation, PointfAnimation){
    test_animation_pointf ();
}

TEST (Animation, RectAnimation){
    test_animation_rect ();
}

TEST (Animation, 3dpointAnimation){
    test_animation_3dpoint();
}

TEST (Animation, 3dpointfAnimation){
    test_animation_3dpointf();
}
#endif


#if 0

TEST (Animation, FPS){
    test_animation_fps ();
}
TEST (Animation, GroupFinished){
    test_animation_group_finished ();
}
TEST (Animation, GroupDuration){
    test_animation_group_duration ();
}
TEST (Animation, GroupLoopcount){
    test_animation_group_loopcount ();
}
TEST (Animation, ColorAnimation){
    test_animation_color ();
}
#endif

int main(int argc, char *argv[]) 
{
    int ret;
    mGEffInit();
//    create_schedular_pthread ();
    ::testing::InitGoogleTest(&argc, argv);
    ret=RUN_ALL_TESTS();
    return ret;
}


