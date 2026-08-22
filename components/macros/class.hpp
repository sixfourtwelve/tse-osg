#pragma once

#define TSE_NON_COPYABLE(TYPE)                                                                                         \
    TYPE(const TYPE&) = delete;                                                                                        \
    TYPE& operator=(const TYPE&) = delete;

#define TSE_NON_MOVABLE(TYPE)                                                                                          \
    TYPE(TYPE&&) = delete;                                                                                             \
    TYPE& operator=(TYPE&&) = delete;

#define TSE_NON_COPYABLE_NON_MOVABLE(TYPE)                                                                             \
    TSE_NON_COPYABLE(TYPE)                                                                                             \
    TSE_NON_MOVABLE(TYPE)
