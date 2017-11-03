list (APPEND sample_targets
    animation_sync
    animation_async
    animation_control
    animation_callback
    animation_setproperty
    animation_setcontext
    animation_curve
    animation_window
    group_animation_sequential
    group_animation_parallel
    effector
    effector_custom
)

foreach(_sample_target  ${sample_targets})
    add_executable(${_sample_target} "${_sample_target}.c")
endforeach(_sample_target)

