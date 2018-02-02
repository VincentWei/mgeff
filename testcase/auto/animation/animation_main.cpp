/*
 *   This file is part of mGEff, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
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


