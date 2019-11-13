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
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
 */
#include <gtest/gtest.h>
#include <mgeff/mgeff.h>
#include <sys/time.h>
#include <time.h>
/*
 * MGEFF_ANIMATION MGEFF_EXPORT mGEffAnimationCreateGroup(EffAnimGroupType type);
 * void MGEFF_EXPORT mGEffAnimationDeleteGroup(MGEFF_ANIMATION  handle);
 * void MGEFF_EXPORT mGEffAnimationAddToGroup(MGEFF_ANIMATION handle, MGEFF_ANIMATION anim);
 */

#define dbg() printf("%s %d\n", __FUNCTION__, __LINE__)
static struct timeval time1, time2, time3;

/* Test animation group create */
TEST(AnimationgroupTest, Create) {
    MGEFF_ANIMATION parallel_group, sequential_group;

    ASSERT_TRUE(mGEffInit() == 0);
    sequential_group = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL);
    ASSERT_TRUE(sequential_group != NULL);
    mGEffAnimationDelete(sequential_group);
    
    parallel_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);
    ASSERT_TRUE(parallel_group != NULL);
    
    mGEffAnimationDelete(parallel_group);
    mGEffDeinit();
}

RECT rc_Start[2] = {{0, 0, 80, 20}, {25, 25, 80, 20}};
RECT rc_End[2] = {{200, 200, 280, 220}, {225, 225, 305, 245}};

void set_sequential_property1(MGEFF_ANIMATION handle, void* target, int id, void* value)
{ 
    gettimeofday(&time1, NULL);
    
    //RECT* rc = (RECT*)value;
    //printf("rect1: (%d, %d, %d, %d)\n", rc->left, rc->top, rc->right, rc->bottom);
}

void set_sequential_property2(MGEFF_ANIMATION handle, void* target, int id, void* value)
{ 
    static int index = 1;
    
    if(index == 1) {
        gettimeofday(&time2, NULL);
        index = 0;
    }
    //RECT *rc = (RECT*)value;
    //printf("rect2: (%d, %d, %d, %d)\n", rc->left, rc->top, rc->right, rc->bottom);
}

static char g_str[11] = {0};

void set_parallel_property1(MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    int len = strlen(g_str);
    if(len < 10) {
        g_str[len] = '1';
    }
}

void set_parallel_property2(MGEFF_ANIMATION handle, void* target, int id, void* value)
{ 
    int len = strlen(g_str);
    if(len < 10) {
        g_str[len] = '0';
    }
}

MGEFF_ANIMATION create_my_animation(MGEFF_SETPROPERTY_CB setproperty_cb, int index)
{
    MGEFF_ANIMATION anim;
    anim = mGEffAnimationCreate(NULL, setproperty_cb, 0, MGEFF_RECT);
   
    mGEffAnimationSetStartValue(anim ,&rc_Start[index]);
    mGEffAnimationSetEndValue(anim, &rc_End[index]);
    mGEffAnimationSetDuration(anim, 1000);

    return anim;
}

/* Test parallel group if run alternately */
TEST(AnimationgroupTest, ParallelAddToGroup) {
    MGEFF_ANIMATION anim1, anim2;
    MGEFF_ANIMATION parallel_group;

    ASSERT_TRUE(mGEffInit() == 0);
    ASSERT_TRUE((parallel_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL)) != NULL);

    anim1 = create_my_animation(set_parallel_property1, 0);
    anim2 = create_my_animation(set_parallel_property2, 1);
    
    mGEffAnimationAddToGroup(parallel_group, anim1);
    mGEffAnimationAddToGroup(parallel_group, anim2);

    mGEffAnimationSyncRun(parallel_group);

    ASSERT_TRUE(strncmp(g_str, "1010101010", 10) == 0);
    
    mGEffAnimationDelete(parallel_group);

    mGEffDeinit();
}

/* Test sequential group if run one after another */
TEST(AnimationgroupTest, SequentialAddToGroup) {
    MGEFF_ANIMATION anim1, anim2;
    MGEFF_ANIMATION sequential_group;

    ASSERT_TRUE(mGEffInit() == 0);
    ASSERT_TRUE((sequential_group = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL)) != NULL);
    
    anim1 = create_my_animation(set_sequential_property1, 0);
    anim2 = create_my_animation(set_sequential_property2, 1);
    
    mGEffAnimationAddToGroup(sequential_group, anim1);
    mGEffAnimationAddToGroup(sequential_group, anim2);

    mGEffAnimationSyncRun(sequential_group);
    
    int temp = (time2.tv_sec -time1.tv_sec) *1000000 + time2.tv_usec - time1.tv_usec;
    ASSERT_TRUE(temp > 0);
    
    mGEffAnimationDelete(sequential_group);
    mGEffDeinit();
}

#define print_time(s, t) \
    printf("time of %s: %d:%d\n", s, (int)t.tv_sec, (int)t.tv_usec);

void animation_end_cb1(MGEFF_ANIMATION handle)
{
    gettimeofday(&time1, NULL);
    print_time("animation 1", time1);
}

void animation_end_cb2(MGEFF_ANIMATION handle)
{
    gettimeofday(&time2, NULL);
    print_time("animation 2", time2);
}

void group_end_cb(MGEFF_ANIMATION handle)
{
    gettimeofday(&time3, NULL);
    print_time("group", time3);
}

/* Test parallel group whether end of the longest animation */
TEST(AnimationgroupTest, ParallelGroupEnd) {
    MGEFF_ANIMATION anim1, anim2;
    MGEFF_ANIMATION parallel_group;

    ASSERT_TRUE(mGEffInit() == 0);
    ASSERT_TRUE((parallel_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL)) != NULL);
     
    /* create animation 1 and add to group */ 
    anim1 = mGEffAnimationCreate(NULL, NULL, 0, MGEFF_INT);
    mGEffAnimationSetDuration(anim1, 100);
    mGEffAnimationSetFinishedCb(anim1, animation_end_cb1);
    mGEffAnimationAddToGroup(parallel_group, anim1);
    
    /* create animation 2 and add to group */ 
    anim2 = mGEffAnimationCreate(NULL, NULL, 0, MGEFF_INT);
    mGEffAnimationSetDuration(anim2, 100);
    mGEffAnimationSetFinishedCb(anim2, animation_end_cb2);
    mGEffAnimationAddToGroup(parallel_group, anim2);
    
    /* run animate and test result 1 */
    mGEffAnimationSetFinishedCb(parallel_group, group_end_cb);
    mGEffAnimationSyncRun(parallel_group);
    
    /* test result */
    int temp = (time3.tv_sec -time2.tv_sec) *1000000 + time3.tv_usec - time2.tv_usec;
    ASSERT_TRUE((temp >= 0) && (temp < 200000));
    
    /* reset preproty, run animate and test result 2 */
    mGEffAnimationSetDuration(anim1, 1000);
    mGEffAnimationSyncRun(parallel_group);
    
    /* test result */
    temp = (time3.tv_sec -time1.tv_sec) *1000000 + time3.tv_usec - time1.tv_usec;
    ASSERT_TRUE((temp >= 0) && (temp < 200000));
    
    mGEffAnimationDelete(parallel_group);
    mGEffDeinit();
}

/* Test sequential group whether end of the last animation */
TEST(AnimationgroupTest, SequentialGroupEnd) {
    MGEFF_ANIMATION anim1, anim2;
    MGEFF_ANIMATION sequential_group;

    ASSERT_TRUE(mGEffInit() == 0);
    ASSERT_TRUE((sequential_group = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL)) != NULL);
    
    /* create animation 1 and add to group */ 
    anim1 = mGEffAnimationCreate(NULL, NULL, 0, MGEFF_INT);
    mGEffAnimationSetDuration(anim1, 100);
    mGEffAnimationSetFinishedCb(anim1, animation_end_cb1);
    mGEffAnimationAddToGroup(sequential_group, anim1);
    
    /* create animation 2 and add to group */ 
    anim2 = mGEffAnimationCreate(NULL, NULL, 0, MGEFF_INT);
    mGEffAnimationSetDuration(anim2, 100);
    mGEffAnimationSetFinishedCb(anim2, animation_end_cb2);
    mGEffAnimationAddToGroup(sequential_group, anim2);
   
    /* run animation */ 
    mGEffAnimationSetFinishedCb(sequential_group, group_end_cb);

    mGEffAnimationSyncRun(sequential_group);
    
    /* test result */
    int temp = (time3.tv_sec -time2.tv_sec) *1000000 + time3.tv_usec - time2.tv_usec;
    ASSERT_TRUE((temp >= 0) && (temp < 200000));
    
    mGEffAnimationDelete(sequential_group);
    mGEffDeinit();
}

/* Test parallel group tree if run normally */
TEST(AnimationgroupTest, ParallelGroupTree) {
    MGEFF_ANIMATION anim[4];
    MGEFF_ANIMATION group[3];
    int i;

    ASSERT_TRUE(mGEffInit() == 0);
    
    /* Create animation group */
    ASSERT_TRUE((group[0] = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL)) != NULL);
    ASSERT_TRUE((group[1] = mGEffAnimationCreateGroup(MGEFF_PARALLEL)) != NULL);
    ASSERT_TRUE((group[2] = mGEffAnimationCreateGroup(MGEFF_PARALLEL)) != NULL);
    
    /* create animations and add them to group */
    for(i = 0; i < 4; i++) {
        anim[i] = mGEffAnimationCreate(NULL, NULL, 0, MGEFF_INT);
        mGEffAnimationAddToGroup(group[i /2], anim[i]);
    }
    /* complex group to group */
    mGEffAnimationAddToGroup(group[2], group[0]);
    mGEffAnimationAddToGroup(group[2], group[1]);

    mGEffAnimationSyncRun(group[2]);

    mGEffAnimationDelete(group[2]);
    mGEffDeinit();
}

/* Test sequential group tree if run normally */
TEST(AnimationgroupTest, SequentialGroupTree) {
    MGEFF_ANIMATION anim[4];
    MGEFF_ANIMATION group[3];
    int i;

    ASSERT_TRUE(mGEffInit() == 0);
    
    /* Create animation group */
    ASSERT_TRUE((group[0] = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL)) != NULL);
    ASSERT_TRUE((group[1] = mGEffAnimationCreateGroup(MGEFF_PARALLEL)) != NULL);
    ASSERT_TRUE((group[2] = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL)) != NULL);
    
    /* create animations and add them to group */
    for(i = 0; i < 4; i++) {
        anim[i] = mGEffAnimationCreate(NULL, NULL, 0, MGEFF_INT);
        mGEffAnimationAddToGroup(group[i /2], anim[i]);
    }
    /* complex group to group */
    mGEffAnimationAddToGroup(group[2], group[0]);
    mGEffAnimationAddToGroup(group[2], group[1]);

    mGEffAnimationSyncRun(group[2]);

    mGEffAnimationDelete(group[2]);
    mGEffDeinit();
}

static int anim_flag = 0;
static int grp_flag = 0;

void delete_cb1(MGEFF_ANIMATION handle)
{
    anim_flag++;
}

void delete_cb2(MGEFF_ANIMATION handle)
{
    grp_flag++;
}

/* Test parallel group delete */
TEST(AnimationgroupTest, parallelGroupDelete) {
    MGEFF_ANIMATION parallel_group;
    MGEFF_ANIMATION anim;

    ASSERT_TRUE(mGEffInit() == 0);
    /* Create group */
    parallel_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL);
    ASSERT_TRUE(parallel_group != NULL);
    
    /* Create animation add set the finshed callback function */
    anim = mGEffAnimationCreate(NULL, NULL, 0, MGEFF_INT);
    mGEffAnimationSetFinishedCb(anim, delete_cb1);
    mGEffAnimationAddToGroup(parallel_group, anim);
    mGEffAnimationSetFinishedCb(parallel_group, delete_cb2);
    
    mGEffAnimationSyncRun(parallel_group);
    /* Test callback function if run */
    ASSERT_TRUE(anim_flag == 1);
    ASSERT_TRUE(grp_flag == 1);
    
//    mGEffAnimationDelete(parallel_group);
  //  ASSERT_TRUE(mGEffSchedularRun(MGEFF_MODE_BLOCK) == 0);
    /* After delete group, callback function if run */
//    ASSERT_TRUE(anim_flag == 1);
//    ASSERT_TRUE(grp_flag == 1);

    mGEffDeinit();
}

/* Test sequential group delete */
TEST(AnimationgroupTest, SequentialGroupDelete) {
    MGEFF_ANIMATION sequential_group;
    MGEFF_ANIMATION anim;
    
    anim_flag = 0;
    grp_flag = 0;
    ASSERT_TRUE(mGEffInit() == 0);
    /* Create group */
    sequential_group = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL);
    ASSERT_TRUE(sequential_group != NULL);

    /* Create animation add set the finshed callback function */
    anim = mGEffAnimationCreate(NULL, NULL, 0, MGEFF_INT);
    mGEffAnimationSetFinishedCb(anim, delete_cb1);
    mGEffAnimationAddToGroup(sequential_group, anim);
    mGEffAnimationSetFinishedCb(sequential_group, delete_cb2);

    mGEffAnimationSyncRun(sequential_group);
    /* Test callback function if run */
    ASSERT_TRUE(anim_flag == 1);
    ASSERT_TRUE(grp_flag == 1);
    
#if 0
    mGEffAnimationDelete(sequential_group);
    ASSERT_TRUE(mGEffSchedularRun(MGEFF_MODE_BLOCK) == 0);
    /* After delete group, callback function if run */
    ASSERT_TRUE(anim_flag == 1);
    ASSERT_TRUE(grp_flag == 1);
#endif

    mGEffDeinit();
}

/* Test group pause */
MGEFF_ANIMATION g_pause_group[4];

void pause_cb1(MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    mGEffAnimationPause(g_pause_group[2]);
    mGEffAnimationPause(handle);
    gettimeofday(&time1, NULL);
    print_time("group 1", time1);
}

void pause_cb2(MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    gettimeofday(&time2, NULL);
    print_time("group 2", time2);
}

void pause_cb3(MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    gettimeofday(&time3, NULL);
    print_time("group 3", time3);
}

TEST(AnimationgroupTest, Paused) {
    MGEFF_ANIMATION anim[3];
    int i;
    
    time1.tv_sec = 0; time1.tv_usec = 0;
    time2.tv_sec = 0; time2.tv_usec = 0;
    time3.tv_sec = 0; time3.tv_usec = 0;

    ASSERT_TRUE(mGEffInit() == 0);
    
    /* Create group */
    for(i = 0; i < 4; i++)
    {
        g_pause_group[i] = mGEffAnimationCreateGroup(MGEFF_SEQUENTIAL);
        ASSERT_TRUE(g_pause_group[i] != NULL);
    }
    
    /* Create animation */
    anim[0] = mGEffAnimationCreate(NULL, pause_cb1, 0, MGEFF_INT);
    mGEffAnimationAddToGroup(g_pause_group[0], anim[0]);
    anim[1] = mGEffAnimationCreate(NULL, pause_cb2, 0, MGEFF_INT);
    mGEffAnimationAddToGroup(g_pause_group[1], anim[1]);
    anim[2] = mGEffAnimationCreate(NULL, pause_cb3, 0, MGEFF_INT);
    mGEffAnimationAddToGroup(g_pause_group[2], anim[2]);
   
    /* Add to group */ 
    mGEffAnimationAddToGroup(g_pause_group[3], g_pause_group[0]);
    mGEffAnimationAddToGroup(g_pause_group[3], g_pause_group[1]);
    mGEffAnimationAddToGroup(g_pause_group[3], g_pause_group[2]);

    mGEffAnimationSyncRun(g_pause_group[3]);
    
    ASSERT_TRUE((time3.tv_sec == 0) && (time3.tv_usec == 0));
    ASSERT_TRUE((time2.tv_sec - time3.tv_sec) > 0);
    
    mGEffAnimationDelete(g_pause_group[3]);
    mGEffDeinit();
}

/* Test group stop */
MGEFF_ANIMATION g_stop_group;
static int stop_flag = 0;

void stop_cb1(MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    mGEffAnimationPause(g_stop_group);
}

void stop_cb2(MGEFF_ANIMATION handle, void* target, int id, void* value)
{
    stop_flag = 1;
}

TEST(AnimationgroupTest, Stop) {
    MGEFF_ANIMATION anim[2];
    
    ASSERT_TRUE(mGEffInit() == 0);
    
    /* Create group */
    ASSERT_TRUE((g_stop_group = mGEffAnimationCreateGroup(MGEFF_PARALLEL)) != NULL);
    
    /* Create animation */
    anim[0] = mGEffAnimationCreate(NULL, stop_cb1, 0, MGEFF_INT);
    anim[1] = mGEffAnimationCreate(NULL, stop_cb2, 0, MGEFF_INT);
    /* Add to group */ 
    mGEffAnimationAddToGroup(g_stop_group, anim[0]);
    mGEffAnimationAddToGroup(g_stop_group, anim[1]);
   
    mGEffAnimationSyncRun(g_stop_group);
    ASSERT_TRUE(stop_flag == 0);
    
    mGEffAnimationDelete(g_stop_group);
    mGEffDeinit();
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
