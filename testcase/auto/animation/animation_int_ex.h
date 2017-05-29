#ifndef _ANIMATION_INT_EX_H
#define _ANIMATION_INT_EX_H

extern void test_animation_createex();
extern void test_animation_delete();
extern void test_animation_stop(int keepalive);
extern void test_animation_pauseresume();
#if 0
extern void test_animation_restart();
#endif
extern void test_animation_setgetproperty();
extern void test_animation_setdirection(int keepalive);
extern void test_animation_setduration();
extern void test_animation_setgetvalue();
extern void test_animation_setcurveex();
extern void test_animation_syncrun(int keepalive);
extern void test_animation_asyncrun(int keepalive);
extern void test_animation_runtime ();
extern void test_animation_multiasyncrun();
#if 0
extern void test_animation_loopcount();
extern void test_animation_direction();
#endif
#endif

