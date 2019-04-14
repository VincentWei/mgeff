# Release Notes

## Version 1.2.1

The MiniGUI development team announces the availability of mGEff 1.2.0.
All users of MiniGUI are encouraged to test this version carefully, and
report any bugs and incompatibilities on GitHub:

https://github.com/VincentWei/mgeff

### What's new in this version

  * Fix a bug which will cause SEGV on 64-bit platform.

## Version 1.2.0

The MiniGUI development team announces the availability of mGEff 1.2.0.
All users of MiniGUI are encouraged to test this version carefully, and
report any bugs and incompatibilities on GitHub:

https://github.com/VincentWei/mgeff

### What's new in this version

  * Tune code to comply with MiniGUI 3.2.x.
  * Tune code for 64-bit platform.
  * Tune code to eliminate all warnings.

### API changes

We now use a `intptr_t` integer for the identifier of an animation.
So you can pass a pointer as the identifier of the animation on 64-bit
platform.

As a result, the prototype of SetProperty callback function of an animation
changed from

    typedef void (*MGEFF_SETPROPERTY_CB)(MGEFF_ANIMATION handle,
        void *target, int id, void *value);

to

    typedef void (*MGEFF_SETPROPERTY_CB)(MGEFF_ANIMATION handle,
        void *target, intptr_t id, void *value);

The function to create an animation object changed from

    MGEFF_ANIMATION mGEffAnimationCreateEx(void *target,
        MGEFF_SETPROPERTY_CB setproperty, int id, int size,
        MGEFF_VARMALLOC_CB cb1, MGEFF_CALCVALUE_CB cb2);

to

    MGEFF_ANIMATION mGEffAnimationCreateEx(void *target,
        MGEFF_SETPROPERTY_CB setproperty, intptr_t id, int size,
        MGEFF_VARMALLOC_CB cb1, MGEFF_CALCVALUE_CB cb2);

And the following function changed from

    MGEFF_ANIMATION mGEffAnimationCreate(void *target,
        MGEFF_SETPROPERTY_CB setproperty, int id,
        enum EffVariantType varianttype);

to

    MGEFF_ANIMATION mGEffAnimationCreate(void *target,
        MGEFF_SETPROPERTY_CB setproperty, intptr_t id,
        enum EffVariantType varianttype);

